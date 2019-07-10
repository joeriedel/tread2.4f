// TREADMAP1X.CPP
// Copyright © 1999 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#include "stdafx.h"
#include "TreadMapAPI.h"
#include "TreadMap1X.h"
#include "TreadSys.h"
#include "Tread3DDoc.h"
#include "Geometry.h"
#include "MapObject.h"
#include "FUtils.h"
#include "Visgroup.h"
#include "Camera.h"
#include "Entity.h"
#include "MapView.h"
#include "Brush.h"
#include "Face.h"
#include "szstring.h"

#define VISGROUP_CATEGORY	"Default"

bool CTreadMap1X::ReadHeader(CFile& hFile, TREAD2XHEADERINFO* pHeader, CTread3DDoc* pDocument)
{
	int nVersion;

	ReadInt(hFile, nVersion);
	if(nVersion != _1X_HEADER_VERSION)
	{
		Error("1X0: Bad/Unsupported header version");
		return false;
	}

	Debug("... version: %d\n", nVersion);

	// Read the game name.
	ReadNString(hFile, pHeader->m_sGame, 32);
	ReadInt(hFile, pHeader->m_nGridSize);
	ReadBool(hFile, pHeader->m_bGridSnap);

	int i;
	float temp;
	for(i = 1; i < 4; i++)
	{
		ReadFloat(hFile, pHeader->m_fScale[i]);
		ReadFloat(hFile, temp);
		ReadFloat(hFile, temp);
		ReadFloat(hFile, pHeader->m_fX[i]);
		ReadFloat(hFile, pHeader->m_fY[i]);
	}

	pHeader->m_fScale[0] = 1.0F;
	pHeader->m_fX[0] = 1.0F; pHeader->m_fY[0] = 1.0F;
	pHeader->m_nViewTypes[0] = VIEW_3D;
	pHeader->m_nViewTypes[1] = VIEW_TOP;
	pHeader->m_nViewTypes[2] = VIEW_SIDE;
	pHeader->m_nViewTypes[3] = VIEW_FRONT;
	pHeader->m_nRenderFlags = RF_REALTIME3D|RF_TEXTURED;
	pHeader->m_nValue = 0;
	pHeader->m_nNumStates = 0;
	pHeader->m_pRunStates = NULL;

	return true;
}

bool CTreadMap1X::ReadEntity(CFile& hFile, CEntity* pOwner, CTread3DDoc* pDocument)
{
	int i, temp, nNumKeys;
	int nNumBrushes, nNumEnts;
	ENTITYKEY* pKey;
	CString sName, sValue;
	CEntity* pEntity = (CEntity*)NewBaseObject(OBJECT_CLASS_ENTITY);
	vec3_t vOrigin, mins, maxs;
	bool bWorldspawn = false;

	// Read the keys.
	ReadInt(hFile, nNumKeys);
	for(i = 0; i < nNumKeys; i++)
	{
		ReadLongString(hFile, sName);
		ReadLongString(hFile, sValue);
		
		if(sName == "classname")
		{
			pEntity->SetClassName(sValue);
			continue;
		}

		pKey = new ENTITYKEY();
		pKey->sName = sName;
		pKey->sValue = sValue;
		pEntity->GetKeyList()->AddItem(pKey);
	}

	// Is this the worldspawn entity?
	if(pEntity->GetClassName() == "worldspawn")
	{
		bWorldspawn = true;
		//pDocument->SetWorldspawn(pEntity);
	}

	// Read the visgroup.
	ReadLongString(hFile, sName);
	if(sName != "")
	{
		pEntity->BindToDocument(pDocument);
		pEntity->SetVisgroup(sName, VISGROUP_CATEGORY);
	}

	// Read origin.
	i = sizeof(mins);
	hFile.Read(vOrigin, 3*4);
	ReadInt(hFile, temp);
	hFile.Read(mins, 3*4);
	hFile.Read(maxs, 3*4);

	pEntity->SetOrigin(vOrigin);
	
	// Read number of brushes.
	ReadInt(hFile, nNumBrushes);
	for(i = 0; i < nNumBrushes; i++)
	{
		if(!ReadBrush(hFile, (bWorldspawn) ? NULL : pEntity, pDocument))
			return false;
	}

	// Read sub entities.
	ReadInt(hFile, nNumEnts);
	for(i = 0; i < nNumEnts; i++)
	{
		if(!ReadEntity(hFile, (bWorldspawn) ? NULL : pEntity, pDocument))
			return false;
	}

	// Who do we add too?
	if(pEntity->GetClassName() == "info_group_77")
	{
		// Make a group from this object.
		CObjectGroup* pGrp = new CObjectGroup(*pEntity);
		delete pEntity;

		if(pOwner != NULL)
			pOwner->AddObject(pGrp);
		else
			pDocument->AddObjectToMap(pGrp);
	}
	else// if(bWorldspawn==false)
	{
		if(pOwner != NULL)
			pOwner->AddObject(pEntity);
		else
			pDocument->AddObjectToMap(pEntity);
	}

	return true;
}

bool CTreadMap1X::ReadBrush(CFile& hFile, CEntity* pOwner, CTread3DDoc* pDocument)
{
	int i, temp;
	int nNumFaces;
	vec3_t mins, maxs, origin;
	CString sName;
	CFace* pFace;
	CQBrush* pBrush = (CQBrush*)NewBaseObject(OBJECT_CLASS_BRUSH);//new CQBrush();

	ReadLongString(hFile, sName);
	if(sName != "")
	{
		pBrush->BindToDocument(pDocument);
		pBrush->SetVisgroup(sName, VISGROUP_CATEGORY);
	}

	// Read the number of faces.
	ReadInt(hFile, nNumFaces);
	ReadInt(hFile, temp);
	hFile.Read(mins, 3*4);
	hFile.Read(maxs, 3*4);
	hFile.Read(origin, 3*4);

	for(i = 0; i < nNumFaces; i++)
	{
		pFace = ReadFace(hFile, pDocument);
		if(pFace == NULL)
		{
			Error("CTreadMap1X::ReadBrush(): ReadFace() returned NULL");
			return false;
		}

		pFace->m_pParent = pBrush;
		pBrush->m_pFaceList->AddItem(pFace);
	}

	// What in gods name are we adding this to?
	if(pOwner)
		pOwner->AddObject(pBrush);
	else
		pDocument->AddObjectToMap(pBrush);

	return true;
}

struct OLDPLANE
{
	float dist;
	float vec[3];
};

CFace* CTreadMap1X::ReadFace(CFile& hFile, CTread3DDoc* pDocument)
{
	int i;
	int temp;
	int nNumPoints;
	OLDPLANE plane;
	vec3_t* vPoints;
	CString sTexture;
	vec3_t tvecs[2];
	float texp[2];
	float shift[2], scale[2], rotate;
	int nSurface, nContents, nValue;

	ReadInt(hFile, nNumPoints);
	vPoints = new vec3_t[nNumPoints];
	if(nNumPoints == NULL)
		return NULL;

	// Skip the flags.
	ReadInt(hFile, temp);
	// Read the plane.
	hFile.Read(&plane, sizeof(plane));
	
	ReadLongString(hFile, sTexture);
	// Remove the .wal extension.
	sTexture = SetFileExtension(sTexture, "");

	hFile.Read(shift, 8);
	hFile.Read(scale, 8);
	hFile.Read(&rotate, 4);
	hFile.Read(tvecs, 4*3*2);

	// Read the points, backwards.
	for(i = nNumPoints-1; i >= 0; i--)
	{
		hFile.Read(&vPoints[i], 4*3);
		// Skip tex point.
		hFile.Read(texp, 4*2);
	}

	// Read contents, surface, value.
	ReadInt(hFile, nContents);
	ReadInt(hFile, nSurface);
	ReadInt(hFile, nValue);
	// Skip pads.
	ReadInt(hFile, temp);
	ReadInt(hFile, temp);

	// Make a face.
	CFace* pFace = (CFace*)NewBaseObject(OBJECT_CLASS_FACE);//new CFace(nNumPoints, vPoints);
	pFace->CopyPoints(nNumPoints, vPoints);
	delete[] vPoints;

	// Set texture, suppress event.
	pFace->m_bTexInit = false;
	pFace->SetTextureName(sTexture, false);
	pFace->m_nContents = nContents;
	pFace->m_nSurface  = nSurface;
	pFace->m_nValue    = nValue;
	pFace->SetTexScales(scale, false);
	pFace->SetTexShifts(shift, false);
	pFace->SetRotation(rotate, false);
	pFace->RebuildTexture();

	return pFace;
}

bool CTreadMap1X::ReadCameras(CFile& hFile, CTread3DDoc* pDocument)
{
	int i, nCount;
	CCamera* pCam;

	ReadInt(hFile, nCount);
	for(i = 0; i < nCount; i++)
	{
		pCam = new CCamera();
		hFile.Read(pCam->m_vOrigin, 4*3);
		hFile.Read(pCam->m_vLook, 4*3);
		pCam->m_nView = 0;
		pDocument->AddObjectToMap(pCam);
		if(i == 0)
			pDocument->GetViewByIndex(0)->SetCamera(pCam);
	}

	Debug("... %d camera(s)\n", nCount);

	return true;
}

bool CTreadMap1X::ReadVisgroups(CFile& hFile, CTread3DDoc* pDocument)
{
	int i, nCount;
	CString sName;
	CVisgroup* pVg;
	bool bPending, bVisible;

	ReadInt(hFile, nCount);
	for(i = 0; i < nCount; i++)
	{
		ReadLongString(hFile, sName);
		ReadBool(hFile, bPending);
		ReadBool(hFile, bVisible);
		pVg = pDocument->GetVisgroupAPI()->AddVisgroup(sName, VISGROUP_CATEGORY);
		if(pVg == NULL)
		{
			Error("CTreadMap1X::ReadVisgroups(): out of memory.");
			return false;
		}

		pVg->SetVisible(bVisible);
		pVg->SetPending(bPending);
	}

	Debug("... %d visgroup(s)\n", nCount);

	return true;
}

bool CTreadMap1X::ReadEntities(CFile& hFile, CTread3DDoc* pDocument)
{
	int i, nNumEnts;

	ReadInt(hFile, nNumEnts);

	PrgSetRange(0, nNumEnts);
	for(i = 0; i < nNumEnts; i++)
	{
		if(!ReadEntity(hFile, NULL, pDocument))
		{
			PrgSetPos(0);
			return false;
		}

		PrgStepIt();
	}

	PrgSetPos(0);

	Debug("... %d entity(s)\n", nNumEnts);

	return true;
}

bool CTreadMap1X::ReadMap(CFile& hFile, TREAD2XHEADERINFO* pHeader, CTread3DDoc* pDocument, CLinkedList<CBaseObject>* pObjectList, CLinkedList<CBaseObject>* pUIObjectList)
{
	Debug("... reading 1x\n");
	Debug("... header\n");
	if(!ReadHeader(hFile, pHeader, pDocument))
		return false;

	// Read visgroups.
	if(!ReadVisgroups(hFile, pDocument))
		return false;

	if(!ReadCameras(hFile, pDocument))
		return false;

	if(!ReadEntities(hFile, pDocument))
		return false;

	return true;
}