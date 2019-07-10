// VERTEXMODEGENERATOR.CPP
// Copyright © 1999 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#include "stdafx.h"
#include "MapObject.h"
#include "HotSpotManager.h"
#include "VertexModeGenerator.h"
#include "MapView.h"
#include "Tread3DDoc.h"
#include "TreadSys.h"
#include "Brush.h"
#include "PatchMesh.h"

CVertexModeGenerator::CVertexModeGenerator()
{
	m_nMode = MODE_VERTEX;
	m_nSubmode = ALL_MODES;
}

void CVertexModeGenerator::GenerateListHotSpots(CLinkedList<CBaseObject>* pList, CHotSpotManager* pHotSpotManager, CTread3DDoc* pDocument, CMapView* pView)
{
	CBaseObject* pObject;
	
	pList->First();
	for(pObject = pList->GetCurItem(); pObject != NULL; pObject = pList->GetNextItem())
	{

		switch(pObject->GetObjectClass())
		{
		case OBJECT_CLASS_LENTITY:
		case OBJECT_CLASS_GROUP:
			{
				CObjectGroup* pGrp = (CObjectGroup*)pObject;
				GenerateListHotSpots(pGrp->GetObjectList(), pHotSpotManager, pDocument, pView);
				break;
			}
		case OBJECT_CLASS_BRUSH:
			{
				CQBrush* pBrush;
				pBrush = (CQBrush*)pObject;
				pBrush->GenerateHotSpots(pHotSpotManager, pView);
				break;
			}
		case OBJECT_CLASS_PATCHMESH:
			{
				CPatchMesh* pPatch;
				pPatch = (CPatchMesh*)pObject;
				pPatch->GenerateHotSpots(pHotSpotManager, pView);
				break;
			}
		}
	}
}

void CVertexModeGenerator::GenerateHotSpots(CHotSpotManager* pHotSpotManager, CTread3DDoc* pDocument, CMapView* pView)
{
	GenerateListHotSpots(pDocument->GetSelectedObjectList(), pHotSpotManager, pDocument, pView);	
}