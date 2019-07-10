// QUAKEMAPFILE.CPP
// Copyright © 1999 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#include "stdafx.h"
#include "TreadMapAPI.h"
#include "QuakeMapFile.h"
#include "szstring.h"
#include "Entity.h"
#include "Brush.h"
#include "Face.h"
#include "MapObject.h"
#include "TreadSys.h"
#include "Tread3DDoc.h"
#include "GameAPI.h"
#include "Tokenizer.h"
#include "RenderAPI.h"
#include "Geometry.h"
#include <math.h>
#include <fstream.h>

bool CQuakeMapFile::WriteMap(CFile& hFile, TREAD2XHEADERINFO* pHeader, CTread3DDoc* pDocument, CLinkedList<CBaseObject>* pObjectList, CLinkedList<CBaseObject>* pSelectedObjectList, CLinkedList<CBaseObject>* pUIObjectList)
{
	CString sFilename = hFile.GetFilePath();
	hFile.Close();
	fstream file;

	file.open(sFilename, ios::out);

	m_nEntityCount=0;
	m_nBrushCount=0;
	m_nFaceCount=0;

	Debug("... writing quake map\n");
	Debug("... worldspawn\n");

	if(!WriteWorldspawn(pDocument->GetWorldspawn(), pDocument, pObjectList, pSelectedObjectList, file))
	{
		Error("CQuakeMapFile::WriteMap(): unable to write worldspawn");
		return false;
	}

	// Normal.|OBJECT_CLASS_LENTITY
	if(!WriteObjectList(pObjectList, OBJECT_CLASS_ENTITY|OBJECT_CLASS_LENTITY|OBJECT_CLASS_GROUP, pDocument, file))
		return false;
	if(!WriteObjectList(pSelectedObjectList, OBJECT_CLASS_ENTITY|OBJECT_CLASS_LENTITY|OBJECT_CLASS_GROUP, pDocument, file))
		return false;

	file.close();

	// Restore so MFC can close without dying.
	hFile.Open(sFilename, CFile::typeBinary | CFile::modeWrite);

	Debug("... %d entity(s)\n", m_nEntityCount);
	Debug("... %d brush(es)\n", m_nBrushCount);
	Debug("... %d face(s)\n", m_nFaceCount);

	return true;
}

bool CQuakeMapFile::ReadMap(CFile& hFile, TREAD2XHEADERINFO* pHeader, CTread3DDoc* pDocument, CLinkedList<CBaseObject>* pObjectList, CLinkedList<CBaseObject>* pUIObjectList)
{
	CString sToken;
	CTokenizer Tokenizer;
	CEntity* pEntity;
	
	pHeader->m_nFlags = T2XHF_IGNORE;
	pDocument->AddGlobalRenderFlags(RF_NOENTITYFILL|RF_NOBILINEARFILTER|RF_NO3DVIEW);

	Debug("... reading quake map\n");
	Debug("... initing token parsing\n");

	// Load in the tokenizer.
	if(!Tokenizer.InitParsing(hFile, WHOLE_FILE))
	{
		Error("CQuakeMapFile::ReadMap(): unable to load map script!\n");
		return false;
	}

	m_nEntityCount=1;
	m_nBrushCount=0;
	m_nFaceCount=0;

	PrgSetPos(0);
	PrgSetRange(0, Tokenizer.GetNumBytes());

	Debug("... parsing entities\n");

	while(Tokenizer.GetToken(sToken))
	{
		Tokenizer.UngetToken();

		switch(ReadEntity(&pEntity, pDocument, Tokenizer))
		{
		case QRF_OK:
			pDocument->AddObjectToMap((CBaseObject*)pEntity);
			break;
		case QRF_WORLDSPAWN:
			break;
		case QRF_ERROR:
			break;
		case QRF_FATAL:
			return false;
		}
	}

	PrgSetPos(0);

	Debug("... %d entity(s)\n", m_nEntityCount);
	Debug("... %d brush(es)\n", m_nBrushCount);
	Debug("... %d face(s)\n", m_nFaceCount);

	return true;
}

int CQuakeMapFile::ReadEntity(CEntity** ppEntity, CTread3DDoc* pDocument, CTokenizer& Tokenizer)
{
	CEntity* pEntity=NULL;
	CObjectGroup* pGroup;
	CQBrush* pBrush;
	bool bGroup=false, bWorld=false;
	CString sToken, sValue;
	CString sClassName;

	// Skip {
	Tokenizer.Skip();

	pEntity = (CEntity*)NewBaseObject(OBJECT_CLASS_ENTITY);

	for(;;)
	{
		if(!Tokenizer.GetToken(sToken))
		{
			Error("CQuakeMapFile::ReadEntity(): Unexpected EOF!\n");
			delete pEntity;
			return QRF_FATAL;
		}

		if(sToken == "}")
			break;

		// Brush?
		if(sToken == "{")
		{
			if(!bGroup && !bWorld && pEntity == NULL)
			{
				Error("CQuakeMapFile::ReadEntity(): Brush before classname!\n");
				delete pEntity;
				return QRF_FATAL;
			}

			Tokenizer.UngetToken();
			switch(ReadBrush(&pBrush, pDocument, Tokenizer))
			{
			case QRF_OK:

				if(bWorld)
					pDocument->AddObjectToMap(pBrush);
				else if(bGroup)
					pGroup->AddObject(pBrush);
				else
					pEntity->AddObject(pBrush);

				break;

			case QRF_ERROR:
				break;
			case QRF_FATAL:
				delete pEntity;
				return QRF_FATAL;
			}

		}
		else
		{
			// Read value.
			if(!Tokenizer.GetToken(sValue))
			{
				Error("CQuakeMapFile::ReadEntity(): Unexpected EOF!\n");
				return QRF_FATAL;
			}

			if(sToken == "classname")
			{
				sClassName = sValue;
				bGroup = sClassName == "info_group";
				bWorld = sClassName == "worldspawn";

				// Make an object.
				if(bGroup)
					pGroup = new CObjectGroup();
				
				pEntity->SetClassName(sClassName);

				if(bWorld)
					pDocument->SetWorldspawn(pEntity);
			}
			else
			{
				/*if(sClassName == "")
				{
					Error("CQuakeMapFile::ReadEntity(): expected classname!\n");
					return QRF_FATAL;
				}*/

				// Filter?
				if(bWorld)
				{
					if(sToken == "origin" || sToken == "wad")
						continue;
				}
				else if(!bGroup)
				{
					if(sToken == "origin")
					{
						sscanf(sValue, "%f %f %f", &pEntity->m_vOrigin[0], &pEntity->m_vOrigin[1], &pEntity->m_vOrigin[2]);
						continue;
					}
				}
				else
				{
					Warning("CQuakeMapFile::ReadEntity(): skipped useless key/pair on info_group!\n");
					continue;
				}

				ENTITYKEY* pKey = new ENTITYKEY();
				pKey->sName = sToken;
				pKey->sValue = sValue;

				// Special?
				if(bWorld)
				{
					// Load key/value pairs into document.
					pDocument->GetWorldspawn()->GetKeyList()->AddItem(pKey);	
				}
				else if(!bGroup)
					pEntity->GetKeyList()->AddItem(pKey);
			}
		}
	}

	if(bGroup)
	{
		*ppEntity = (CEntity*)pGroup;
		return QRF_GROUP;
	}

	*ppEntity = pEntity;

	PrgSetPos(Tokenizer.GetByteOffset());

	m_nEntityCount++;
	return bWorld ? QRF_WORLDSPAWN : QRF_OK;
}

int CQuakeMapFile::ReadBrush(CQBrush** ppBrush, CTread3DDoc* pDocument, CTokenizer& Tokenizer)
{
	CString sToken;
	CLinkedList<CQBrushPlane> m_PlaneList;
	CQBrushPlane* pPlane;

	// Skip {
	Tokenizer.Skip();

	for(;;)
	{
		if(!Tokenizer.GetToken(sToken))
		{
			Error("CQuakeMapFile::ReadBrush(): unexpected EOF!\n");
			return QRF_FATAL;
		}

		if(sToken == "}")
			break;

		Tokenizer.UngetToken();

		// Read a plane.
		pPlane = new CQBrushPlane();
		switch(ReadPlane(pPlane, pDocument, Tokenizer))
		{
		case QRF_OK:

			if(FindDuplicatePlane(&m_PlaneList, pPlane))
			{
				Warning("CQuakeMapFile::ReadBrush(): skipping duplicate plane!\n");
				delete pPlane;
				break;
			}

			m_PlaneList.AddItem(pPlane);
			break;

		case QRF_ERROR:
			delete pPlane;
			break;
		case QRF_FATAL:
			return QRF_FATAL;
		}
	}

	// Messed up brush?
	// Must have at least four planes
	if(m_PlaneList.GetCount() < 4)
	{
		Warning("CQuakeMapFile::ReadBrush(): Bad brush plane #. Brush was skipped.\n");
		return QRF_ERROR;
	}

	// Create the brush.
	CQBrush* pBrush = CQBrush::CreateBrushFromPlaneList(m_PlaneList);
	if(pBrush == NULL)
	{
		Warning("CQuakeMapFile::ReadBrush(): CreateBrushFromPlaneList() failed! Skipping brush!\n");
		return QRF_ERROR;
	}

	*ppBrush = pBrush;

	m_nBrushCount++;
	
	PrgSetPos(Tokenizer.GetByteOffset());

	return QRF_OK;
}

bool CQuakeMapFile::FindDuplicatePlane(CLinkedList<CQBrushPlane>* pPlaneList, CQBrushPlane* pPlane)
{
	CQBrushPlane* pP;

	pPlaneList->First();
	for(pP = pPlaneList->GetCurItem(); pP != NULL; pP = pPlaneList->GetNextItem())
	{
		if(VectorCompare(pP->m_Plane.m_vNormal, pPlane->m_Plane.m_vNormal, 0.001F))
			return true;
	}

	return false;
}

int CQuakeMapFile::ReadPlane(CQBrushPlane* pPlane, CTread3DDoc* pDocument, CTokenizer& Tokenizer)
{
	// Read the 3pt plane def.
	int i;
	vec3_t v[3];
	vec3_t vE[2];
	CString sToken;
	CString sTexture;
	
	for(i = 0; i < 3; i++)
	{
		Tokenizer.Skip();	// (
		Tokenizer.GetToken(sToken);
		v[i][0] = (float)atof(sToken);
		Tokenizer.GetToken(sToken);
		v[i][1] = (float)atof(sToken);
		Tokenizer.GetToken(sToken);
		v[i][2] = (float)atof(sToken);
		Tokenizer.Skip(); // )
	}

	// Get the texture name.
	if(!Tokenizer.GetToken(sTexture))
	{
		Error("CQuakeMapFile::ReadPlane(): Missing brush texture!\n");
		return QRF_FATAL;
	}

	sTexture.MakeLower();
	pPlane->m_sTexture = sTexture;

	// Read shifts, scales, and rotations.
	Tokenizer.GetToken(sToken);
	pPlane->m_vShifts[0] = (float)atof(sToken);
	Tokenizer.GetToken(sToken);
	pPlane->m_vShifts[1] = (float)atof(sToken);
	Tokenizer.GetToken(sToken);
	pPlane->m_fRotate = (float)atof(sToken);
	Tokenizer.GetToken(sToken);
	pPlane->m_vScales[0] = (float)atof(sToken);
	Tokenizer.GetToken(sToken);
	pPlane->m_vScales[1] = (float)atof(sToken);

	Tokenizer.GetToken(sToken);
	if(sToken != "}" && sToken != "(")
		Tokenizer.Skip(2);	
	else
		Tokenizer.UngetToken();

	// Make the plane.
	VectorSubtract(v[0], v[1], vE[0]);
	VectorSubtract(v[2], v[1], vE[1]);
	CalcOrthoVector(vE[0], vE[1], pPlane->m_Plane.m_vNormal);

	// Bad normal?
	if(VectorIsZero(pPlane->m_Plane.m_vNormal))
	{
		Warning("CQuakeMapFile::ReadPlane(): bad plane normal!\n");
		return QRF_ERROR;
	}

	// Get a distance.
	pPlane->m_Plane.m_vDist = DotProduct(v[0], pPlane->m_Plane.m_vNormal);

	m_nFaceCount++;
	return QRF_OK;
}

bool CQuakeMapFile::WriteGroup(CObjectGroup* pGroup, int nClass, CTread3DDoc* pDocument, fstream& file)
{
	return WriteObjectList(pGroup->GetObjectList(), nClass, pDocument, file);
}

bool CQuakeMapFile::WriteObjectList(CLinkedList<CBaseObject>* pList, int nClass, CTread3DDoc* pDocument, fstream& file)
{
	CBaseObject* pObject;
	
	pList->First();
	for(pObject = pList->GetCurItem(); pObject != NULL; pObject = pList->GetNextItem())
		if(pObject->GetObjectClass()&nClass)
			if(!WriteObject(pObject, nClass, pDocument, file))
				return false;

	return true;
}

bool CQuakeMapFile::WriteObject(CBaseObject* pObject, int nClass, CTread3DDoc* pDocument, fstream& file)
{
	switch(pObject->GetObjectClass())
	{
	case OBJECT_CLASS_BRUSH:
		if(!WriteBrush((CQBrush*)pObject, pDocument, file))
			return false;
		break;
	case OBJECT_CLASS_ENTITY:
	case OBJECT_CLASS_LENTITY:
		if(!WriteEntity((CEntity*)pObject, pDocument, file))
			return false;
	case OBJECT_CLASS_GROUP:
		if(!WriteGroup((CObjectGroup*)pObject, nClass, pDocument, file))
			break;
	}

	return true;	

}

bool CQuakeMapFile::WriteWorldspawn(CEntity* pEntity, CTread3DDoc* pDocument, CLinkedList<CBaseObject>* pObjectList, CLinkedList<CBaseObject>* pSelectedObjectList, fstream& file)
{	
	file << "{\n";
	file << "\"classname\" \"worldspawn\"\n";

	// Write keys;
	ENTITYKEY* pKey;
	CLinkedList<ENTITYKEY>* pKeyList = pEntity->GetKeyList();
	pKeyList->First();
	for(pKey = pKeyList->GetCurItem(); pKey != NULL; pKey = pKeyList->GetNextItem())
	{
		if(pKey->sName == "$SURFACE_77X" || pKey->sName == "$CONTENTS_77X")
			continue;
		if(pKey->sValue == "0" || pKey->sValue == "")
			continue;
		file << "\"" << pKey->sName << "\"" << " \"" << pKey->sValue << "\"\n";
	}

	// Write the .wad list.
	file << "\"wad\" \"";
	bool bSemi = false;

	GAMEPAKFILE* pPak;
	CLinkedList<GAMEPAKFILE>* pPakList = GetGameAPI()->GetCurrentGame()->GetPakFileList();
	pPakList->First();
	for(pPak = pPakList->GetCurItem(); pPak != NULL; pPak = pPakList->GetNextItem())
	{
		if(bSemi)
			file << ";";
		bSemi = true;
		file << pPak->sFilename;
	}
	file << "\"\n";

	// Normal.
	if(!WriteObjectList(pObjectList, OBJECT_CLASS_BRUSH|OBJECT_CLASS_GROUP, pDocument, file))
		return false;
	if(!WriteObjectList(pSelectedObjectList, OBJECT_CLASS_BRUSH|OBJECT_CLASS_GROUP, pDocument, file))
		return false;

	file << "}\n";

	return true;
}

bool CQuakeMapFile::WriteFace(CFace* pFace, CTread3DDoc* pDocument, fstream& file)
{
	int i;
	// Just skip.
	if(pFace->GetNumPoints() < 3)
	{
		Warning("CQuakeMapFile::WriteFace(): < 3 pts.");
		return true;
	}

	// Write the 3 point plane def.
	vec3_t* vPoints = pFace->GetPoints();
	for(i = 0; i < 3; i++)
		file << "( " << (int)(floor(vPoints[i][0] - 0.5) + 1) << " " << (int)(floor(vPoints[i][1] - 0.5) + 1) << " " << (int)(floor(vPoints[i][2] - 0.5) + 1) << " ) ";

	CString sTexture = pFace->GetTextureName();
	sTexture.MakeLower();
	file << sTexture;

	vec_t vShifts[2], vScales[2];
	pFace->GetTexScales(vScales);
	pFace->GetTexShifts(vShifts);

	file << " " << (int)vShifts[0] << " " << (int)vShifts[1] << " " << (int)pFace->GetRotation() << " " << vScales[0] << " " << vScales[1];
	file << "\n";

	m_nFaceCount++;
	return true;
}

bool CQuakeMapFile::WriteBrush(CQBrush* pBrush, CTread3DDoc* pDocument, fstream& file)
{
	CFace* pFace;

	file << "{\n";
	
	pBrush->m_pFaceList->First();
	for(pFace = pBrush->m_pFaceList->GetCurItem(); pFace != NULL; pFace = pBrush->m_pFaceList->GetNextItem())
	{
		if(!WriteFace(pFace, pDocument, file))
			return false;
	}

	file << "}\n";

	m_nBrushCount++;
	return true;
}

bool CQuakeMapFile::WriteEntity(CEntity* pEntity, CTread3DDoc* pDocument, fstream& file)
{
	CBaseObject* pObject;

	file << "{\n";

	// Write keys;
	file << "\"classname\" \"" << pEntity->GetClassName() << "\"\n";
	file << "\"origin\" \"" << (int)pEntity->m_vOrigin[0] << " " << (int)pEntity->m_vOrigin[1] << " " << (int)pEntity->m_vOrigin[2] << "\"\n";

	ENTITYKEY* pKey;
	CLinkedList<ENTITYKEY>* pKeyList = pEntity->GetKeyList();
	pKeyList->First();
	for(pKey = pKeyList->GetCurItem(); pKey != NULL; pKey = pKeyList->GetNextItem())
	{
		if(pKey->sValue == "0" || pKey->sValue == "")
			continue;
		file << "\"" << pKey->sName << "\"" << " \"" << pKey->sValue << "\"\n";
	}

	CLinkedList<CBaseObject>* pObjectList = pEntity->GetObjectList();
	pObjectList->First();
	for(pObject = pObjectList->GetCurItem(); pObject != NULL; pObject = pObjectList->GetNextItem())
	{
		if((pObject->GetObjectClass()&OBJECT_CLASS_BRUSH)==0)
			continue;
		if(!WriteBrush((CQBrush*)pObject, pDocument, file))
			return false;
	}

	file << "}\n";

	m_nEntityCount++;
	return true;
}
