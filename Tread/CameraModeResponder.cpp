// CAMERAMODERESPONDER.CPP
// Copyright © 1999 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#include "stdafx.h"
#include "RenderAPI.h"
#include "Tread3DDoc.h"
#include "EventManager.h"
#include "HotSpotManager.h"
#include "MapObject.h"
#include "Camera.h"
#include "CameraModeResponder.h"
#include "MapView.h"

CCameraModeResponder::CCameraModeResponder()
{
	m_nMode = MODE_CAMERA;
	m_nSubmode = ALL_MODE_FLAGS;
}

int CCameraModeResponder::OnAction(int nAction, void* pParam, CModeChangeEventManager* pEventManager, CMode* pModeManager, CTread3DDoc* pDocument)
{
	switch(nAction)
	{
	case EF_ACTION_DELETE:
		{
			// Delete the current camera.
			CBaseObject* pObj;
			CCamera* pCam, *pNewCam, *pDelCam;
			CLinkedList<CBaseObject>* pList = pDocument->GetUIObjectList();
			
			// Find the selected camera, and a new one to jump too.
			pNewCam = pDelCam = NULL;
			pList->First();
			for(pObj = pList->GetCurItem(); pObj != NULL; pObj = pList->GetNextItem())
			{
				if(pObj->GetObjectClass() != OBJECT_CLASS_CAMERA)
					continue;

				pCam = (CCamera*)pObj; 

				if(pCam->m_nObjectRenderFlags&RF_SELECTED)
					pDelCam = pCam;
				else
					pNewCam = pCam;

				// Found both.
				if(pDelCam && pNewCam)
					break;
			}

			// Didn't find a new camera?
			if(!pDelCam || !pNewCam)
				break;

			pDocument->RemoveObjectFromMap(pDelCam);
			pDelCam->GetView()->SetCamera(pNewCam);
			delete pDelCam;
			pDocument->BuildHotSpots();
			pDocument->UpdateWindows();
		}
		break;

	case EF_ACTION_PAINT:

		pDocument->SetSelectedObjectsTexture(GetTexAPI()->GetCurTextureName());
		pDocument->UpdateWindows();
		break;

	case EF_ACTION_CENTER:
		{
			CCamera* pCam;
			CBaseObject* pObj;
			CLinkedList<CBaseObject>* pList = pDocument->GetUIObjectList();

			pList->First();
			for(pObj = pList->GetCurItem(); pObj != NULL; pObj = pList->GetNextItem())
			{
				if(pObj->GetObjectClass() == OBJECT_CLASS_CAMERA)
				{
					pCam = (CCamera*)pObj;
					if(pCam->m_nObjectRenderFlags&RF_SELECTED)
					{
						pDocument->CenterViewsOnUIObject(pCam);
						break;
					}
				}
			}

			pDocument->UpdateWindows(FLAT_VIEWS);
		}
		break;
	}

	return ARF_HANDLED;
}

void CCameraModeResponder::OnModeChange(CModeChangeEventManager* pEventManager, CMode* pModeManager, CTread3DDoc* pDocument, int nOldMode, int nOldSubmode, int nNewMode, int nNewSubmode)
{
	if(nNewMode&MODE_CAMERA)
	{
		// Build the hotspots.
		pDocument->BuildHotSpots();
		// Make the camera visible.
		pDocument->RemoveDrawFilters(OBJECT_CLASS_CAMERA);
		pDocument->AddGlobalRenderFlags(RF_FAST);
		// Repaint the flat views.
		pDocument->UpdateWindows(FLAT_VIEWS);
	}
	else
	{
		pDocument->AddDrawFilters(OBJECT_CLASS_CAMERA);
		pDocument->RemoveGlobalRenderFlags(RF_FAST);
	}
}