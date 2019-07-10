// FACEMODERESPONDER.CPP
// Copyright © 1999 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#include "stdafx.h"
#include "FaceModeResponder.h"
#include "TreadSys.h"
#include "Tread3DDoc.h"
#include "RenderAPI.h"
#include "Brush.h"
#include "Face.h"

#ifdef _WIN32
#include "DialogControl.h"
#endif

CFaceModeResponder::CFaceModeResponder()
{
	m_nMode = MODE_FACE;
	m_nSubmode = ALL_MODES;
}

void CFaceModeResponder::AddGroup(CObjectGroup* pGroup, CTread3DDoc* pDocument)
{
	// Add the selected objects to the surface selection.
	CQBrush* pBrush;
	CFace* pFace;
	CBaseObject* pO;
	CLinkedList<CBaseObject>* pList = pGroup->GetObjectList();

	pList->First();
	for(pO = pList->GetCurItem(); pO != NULL; pO = pList->GetNextItem())
	{
		if(pO->GetObjectClass() == OBJECT_CLASS_GROUP)
		{
			AddGroup((CObjectGroup*)pO, pDocument);
			continue;
		}

		if(pO->GetObjectClass() != OBJECT_CLASS_BRUSH)
			continue;

		pBrush = (CQBrush*)pO;
		pBrush->m_pFaceList->First();
		for(pFace = pBrush->m_pFaceList->GetCurItem(); pFace != NULL; pFace = pBrush->m_pFaceList->GetNextItem())
			pDocument->AddObjectToSurfaceSelection(pFace);
	}

}

void CFaceModeResponder::OnModeChange(CModeChangeEventManager* pEventManager, CMode* pModeManager, CTread3DDoc* pDocument, int nOldMode, int nOldSubmode, int nNewMode, int nNewSubmode)
{
	if(nNewMode&MODE_FACE)
	{
		pDocument->ClearSurfaceSelection();
		pDocument->AddGlobalRenderFlags(RF_NOHOTSPOTS|RF_NOSELBOX|RF_NO3DSELOUTLINE);

		// Add the selected objects to the surface selection.
		CQBrush* pBrush;
		CFace* pFace;
		CBaseObject* pO;
		CLinkedList<CBaseObject>* pList = pDocument->GetSelectedObjectList();
	
		pList->First();
		for(pO = pList->GetCurItem(); pO != NULL; pO = pList->GetNextItem())
		{
			if(pO->GetObjectClass() == OBJECT_CLASS_GROUP)
			{
				AddGroup((CObjectGroup*)pO, pDocument);
				continue;
			}

			if(pO->GetObjectClass() != OBJECT_CLASS_BRUSH)
				continue;

			pBrush = (CQBrush*)pO;
			pBrush->m_pFaceList->First();
			for(pFace = pBrush->m_pFaceList->GetCurItem(); pFace != NULL; pFace = pBrush->m_pFaceList->GetNextItem())
				pDocument->AddObjectToSurfaceSelection(pFace);
		}

		pDocument->ClearSelection();
		pDocument->UpdateWindows();

#ifdef _WIN32
		GetSurfaceDialog()->LoadFaceValues(pDocument->GetSurfaceObjList()->IsEmpty() == false);
#endif

	}
	else
	{
		pDocument->RemoveGlobalRenderFlags(RF_NO3DSELOUTLINE);
		pDocument->ClearSurfaceSelection();
	}
}

int CFaceModeResponder::OnAction(int nAction, void* pParam, CModeChangeEventManager* pEventManager, CMode* pModeManager, CTread3DDoc* pDocument)
{
	switch(nAction)
	{
	case EF_ACTION_PAINT:

		pDocument->SetTextureName(GetTexAPI()->GetCurTextureName());
#ifdef _WIN32		
		GetSurfaceDialog()->LoadFaceValues(pDocument->GetSurfaceObjList()->IsEmpty() == false);
#endif
		pDocument->UpdateWindows(VIEW_3D);
		break;
	}

	return ARF_HANDLED;
}
