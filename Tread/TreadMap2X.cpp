// TREADMAP2X.CPP
// Copyright © 1999 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#include "stdafx.h"
#include "TreadMapAPI.h"
#include "TreadMap2X.h"
#include "TreadSys.h"
#include "Tread3DDoc.h"
#include "Geometry.h"
#include "MapObject.h"
#include "FUtils.h"
#include "GameAPI.h"
#include "Entity.h"

bool CTreadMap2X::WriteHeader(CFile& hFile, TREAD2XHEADERINFO* pHeader, CTread3DDoc* pDocument)
{
	int i;
	WriteNString(hFile, _2X_HEADER_TAG, 7);
	WriteInt(hFile, _2X_HEADER_MAGIC);
	WriteInt(hFile, _2X_HEADER_VERSION);
	WriteInt(hFile, pHeader->m_nFlags);
	WriteInt(hFile, pHeader->m_nValue);

	// Write settings.
	if(pHeader->m_nFlags&T2XHF_TRD2FILE)
	{
		// Write the run states.
		WriteInt(hFile, pHeader->m_nNumStates);
		for(i = 0; i < pHeader->m_nNumStates; i++)
		{
			WriteInt(hFile, pHeader->m_pRunStates[i].nValue);
			WriteString(hFile, pHeader->m_pRunStates[i].sName);
			WriteString(hFile, pHeader->m_pRunStates[i].sValue);
			WriteString(hFile, pHeader->m_pRunStates[i].sCmdLine);
		}

		// Write the doc info.
		WriteInt(hFile, pHeader->m_nRenderFlags);
		WriteBool(hFile, pHeader->m_bGridSnap);
		WriteInt(hFile, pHeader->m_nGridSize);

		// Write each view type.
		for(i = 0; i < 4; i++)
			WriteInt(hFile, pHeader->m_nViewTypes[i]);
		for(i = 0; i < 4; i++)
			WriteFloat(hFile, pHeader->m_fScale[i]);
		for(i = 0; i < 4; i++)
			WriteFloat(hFile, pHeader->m_fX[i]);
		for(i = 0; i < 4; i++)
			WriteFloat(hFile, pHeader->m_fY[i]);
			
		WriteString(hFile, pHeader->m_sGame);

		WriteFloat(hFile, pHeader->m_fHollowDepth);
		WriteInt(hFile, pHeader->m_nCurveSubDepth);
		WriteBool(hFile, pHeader->m_bBrushHandles);
		WriteBool(hFile, pHeader->m_bDrawTargets);
		WriteBool(hFile, pHeader->m_bUpdateAllViews);

	}
	
	WriteString(hFile, pHeader->m_sAuthor);
	WriteString(hFile, pHeader->m_sComments);
	WriteString(hFile, pHeader->m_sCategory);
	WriteString(hFile, pHeader->m_sName);
	
	return true;
}

bool CTreadMap2X::ReadHeader(CFile& hFile, TREAD2XHEADERINFO* pHeader, CTread3DDoc* pDocument)
{
	int i, nCount;
	int nVersion;
	
	// NOTE TAG/MAGIC should have been read.
	ReadInt(hFile, nVersion);
	if(nVersion > _2X_HEADER_VERSION)
	{
		Warning("Bad/Unsupported header version.\n");
		return false;
	}
	pHeader->m_nVersion = nVersion;

	Debug("... version: %d\n", nVersion);

	// Read flags.
	ReadInt(hFile, pHeader->m_nFlags);
	ReadInt(hFile, pHeader->m_nValue);

	if(pHeader->m_nFlags&T2XHF_TRD2FILE)
	{
		Debug("... map file\n");

		// Read the states.
		ReadInt(hFile, nCount);
		pHeader->m_nNumStates = nCount;
		if(nCount <= 0)
			pHeader->m_pRunStates = NULL;
		else
		{
			pHeader->m_pRunStates = new RUNSTATE[nCount];
			for(i = 0; i < nCount; i++)
			{
				ReadInt(hFile, pHeader->m_pRunStates[i].nValue);
				ReadString(hFile, pHeader->m_pRunStates[i].sName);
				ReadString(hFile, pHeader->m_pRunStates[i].sValue);
				if(nVersion > _2X_HEADER_VERSION_5)
					ReadString(hFile, pHeader->m_pRunStates[i].sCmdLine);
			}
		}

		ReadInt(hFile, pHeader->m_nRenderFlags);
		ReadBool(hFile, pHeader->m_bGridSnap);
		ReadInt(hFile, pHeader->m_nGridSize);

		// Read each view type.
		for(i = 0; i < 4; i++)
			ReadInt(hFile, pHeader->m_nViewTypes[i]);
		for(i = 0; i < 4; i++)
			ReadFloat(hFile, pHeader->m_fScale[i]);
		for(i = 0; i < 4; i++)
			ReadFloat(hFile, pHeader->m_fX[i]);
		for(i = 0; i < 4; i++)
			ReadFloat(hFile, pHeader->m_fY[i]);

		// Game.
		ReadString(hFile, pHeader->m_sGame);

		if(pHeader->m_sGame == "")
		{
			Error("Bad tread header!\n");
			return false;
		}

		// Anything after version 1 has the hollow depth in it.
		if(pHeader->m_nVersion > _2X_HEADER_VERSION_1)
		{
			ReadFloat(hFile, pHeader->m_fHollowDepth);
			ReadInt(hFile, pHeader->m_nCurveSubDepth);
		}

		if(pHeader->m_nVersion > _2X_HEADER_VERSION_8)
			ReadBool(hFile, pHeader->m_bBrushHandles);
		if(pHeader->m_nVersion > _2X_HEADER_VERSION_9)
			ReadBool(hFile, pHeader->m_bDrawTargets);
		if(pHeader->m_nVersion > _2X_HEADER_VERSION_10)
			ReadBool(hFile, pHeader->m_bUpdateAllViews);
	}
	else
		Debug("... model/brush file\n");

	// Author.
	ReadString(hFile, pHeader->m_sAuthor);
	// Comments.
	ReadString(hFile, pHeader->m_sComments);
	// Category.
	ReadString(hFile, pHeader->m_sCategory);
	// Name.
	ReadString(hFile, pHeader->m_sName);

	return true;
}

bool CTreadMap2X::WriteMap(CFile& hFile, TREAD2XHEADERINFO* pHeader, CTread3DDoc* pDocument, CLinkedList<CBaseObject>* pObjectList, CLinkedList<CBaseObject>* pSelectedObjectList, CLinkedList<CBaseObject>* pUIObjectList)
{
	int nNumObjects=0;
	
	Debug("... writing 2x\n");
	Debug("... header\n");

	if(!WriteHeader(hFile, pHeader, pDocument))
		return false;

	// Write the texture MRU.
	if(pHeader->m_nFlags&T2XHF_TRD2FILE)
	{
		Debug("... tex mru\n");

		int nLen;
		TEXMRUREF* pRef;
		CLinkedList<TEXMRUREF>* pList = pDocument->GetTextureMRU();

		nLen = pList->GetCount();
		WriteInt(hFile, nLen);

		pList->First();
		for(pRef = pList->GetCurItem(); pRef != NULL; pRef = pList->GetNextItem())
			WriteString(hFile, pRef->sName);	

		Debug("... visgroups\n");

		// Write the visgroups.
		if(!pDocument->GetVisgroupAPI()->WriteVisgroups(hFile, _2X_HEADER_VERSION))
			return false;
	}

	nNumObjects = G_GetInterfaceCount(pObjectList, I_STORABLE);
	if(pSelectedObjectList != NULL)
		nNumObjects += G_GetInterfaceCount(pSelectedObjectList, I_STORABLE);
	if(pUIObjectList != NULL)
		nNumObjects += G_GetInterfaceCount(pUIObjectList, I_STORABLE);

	Debug("... worldspawn\n");

	// Worldspawn?
	if(pDocument != NULL)
	{
		if(pDocument->GetWorldspawn() != NULL)
			nNumObjects++;
	}

	// Write the objects.
	WriteInt(hFile, nNumObjects);

	PrgSetRange(0, nNumObjects);
	PrgSetPos(0);
	PrgSetStep(1);

	if(pDocument != NULL)
	{
		if(pDocument->GetWorldspawn() != NULL)
		{
			CEntity* pEnt = pDocument->GetWorldspawn();
			if(!pEnt->WriteObjectClass(hFile))
				return false;
			if(!pEnt->WriteObject(hFile, pHeader->m_nVersion))
				return false;
		}
	}

	PrgStepIt();

	if(!G_WriteObjectList(pHeader->m_nVersion, hFile, pObjectList))
		return false;

	if(pSelectedObjectList != NULL)
	{
		if(!G_WriteObjectList(pHeader->m_nVersion, hFile, pSelectedObjectList))
			return false;
	}

	if(pUIObjectList != NULL)
	{
		if(!G_WriteObjectList(pHeader->m_nVersion, hFile, pUIObjectList))
			return false;
	}

	Debug("... I_STORABLE\n");
	Debug("... wrote %d base chunk(s)\n", nNumObjects);

	PrgSetPos(0);

	return true;
}

bool CTreadMap2X::ReadMap(CFile& hFile, TREAD2XHEADERINFO* pHeader, CTread3DDoc* pDocument, CLinkedList<CBaseObject>* pObjectList, CLinkedList<CBaseObject>* pUIObjectList)
{
	int i, nCount;
	bool bResult;

	Debug("... reading 2x\n");
	Debug("... header\n");

	if(!ReadHeader(hFile, pHeader, pDocument))
		return false;

	// Read the MRU.
	if(pHeader->m_nFlags&T2XHF_TRD2FILE)
	{
		Debug("... tex mru\n");

		CString sName;
		TEXMRUREF* pRef;

		ReadInt(hFile, nCount);
		for(i = 0; i < nCount; i++)
		{
			ReadString(hFile, sName);
			pRef = new TEXMRUREF();
			pRef->sName = sName;
			pDocument->GetTextureMRU()->AddItem(pRef);
		}
	

		if(pHeader->m_nVersion > _2X_HEADER_VERSION_4)
		{
			Debug("... visgroups\n");
			// Read the visgroups.
			if(!pDocument->GetVisgroupAPI()->ReadVisgroups(hFile, pHeader->m_nVersion))
				return false;
		}
	}

	// Read the count.
	ReadInt(hFile, nCount);
	if(nCount == 0)
		return true;

	gnRecurseCount=0;
	PrgSetRange(0, nCount);

	bResult = G_ReadObjectList(nCount, pHeader->m_nVersion, hFile, pObjectList, pUIObjectList);

	PrgSetPos(0);

	Debug("... read %d chunk(s)\n", nCount);

	return bResult;
}
