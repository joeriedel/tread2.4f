// 3DWINRESPONDER.CPP
// Copyright © 1999 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#include "stdafx.h"
#include "EventManager.h"
#include "Tread3DDoc.h"
#include "3DWinResponder.h"
#include "RenderAPI.h"
#include "MapView.h"
#include "Geometry.h"
#include "TreadSys.h"
#include "GameAPI.h"
#include "MapObject.h"
#include "Face.h"
#include "Brush.h"
#include "PluginAPI.h"

#ifdef _WIN32
#include "DialogControl.h"
#endif

#define STATE_NONE			0
#define STATE_CAMROT		1
#define STATE_CAMMOVE		2
#define STATE_CAMSTRAFE		3

#define PIXELS_PER_DEGREE	1.0f
#define PIXELS_PER_UNIT	1.0f

C3DWinResponder::C3DWinResponder()
{
	m_nMode = MODE_POINTER|MODE_VERTEX|MODE_CLIP|MODE_FACE|MODE_CAMERA;
	m_nSubmode = ALL_MODES;
	m_nState = STATE_NONE;
}

void C3DWinResponder::OnMouseMove(CBaseWindowEventResponder* pParent, CWindowEventManager* pEventManager, CMapView* pView, CTread3DDoc* pDocument, int nMouseX, int nMouseY, int nButtons)
{
	// State.
	if(m_nState == STATE_NONE)
		return;

	// Get cam vector.
	vec3_t vCam;
	CCamera* pCam = pView->GetCamera();

	if(pCam == NULL)
		return;

	VectorSubtract(pCam->GetLook(), pCam->GetOrigin(), vCam);
	
	if(m_nState == STATE_CAMROT)
	{
		// Rotate the camera around.
		int nDiffX = nMouseX - m_nX;
		int nDiffY = nMouseY - m_nY;

		if(abs((float)nDiffX) < PIXELS_PER_DEGREE)
			nDiffX = 0;
		if(abs((float)nDiffY) < PIXELS_PER_DEGREE)
			nDiffY = 0;

		if(nDiffX == 0 && nDiffY == 0)
			return;

		float fXTwist =  (nDiffX / PIXELS_PER_DEGREE)/180.0f*PI;
		float fYTwist = -(nDiffY / PIXELS_PER_DEGREE)/180.0f*PI;

		// Get up, left vectors.
		vec3_t vLeft, vUp;

		vUp[0] = 0.0f;
		vUp[1] = 0.0f;
		vUp[2] = 1.0f;

		CalcOrthoVector(vCam, vUp, vLeft);
		CalcOrthoVector(vCam, vLeft, vUp);

		// Make Q's.
		vec4_t qLeft, qUp, qT;

		Q_SetFromAxisRotation(vUp, fXTwist, qUp);
		Q_SetFromAxisRotation(vLeft, fYTwist, qLeft);

		// Combine and transform.
		Q_Multiply(qUp, qLeft, qT);
		Q_RotateVector(qT, vCam, vCam);
		VectorAdd(pCam->GetOrigin(), vCam, pCam->GetLook());

		pView->RedrawWindow();

		
	}
	else if(m_nState == STATE_CAMMOVE)
	{
		if(abs((float)nMouseY-m_nY) < PIXELS_PER_UNIT)
			return;

		VectorNormalize(vCam);
		// Move along camera vector.
		float fScale = -(nMouseY - m_nY)/PIXELS_PER_UNIT;
		VectorScale(vCam, fScale, vCam);

		VectorAdd(pCam->GetOrigin(), vCam, pCam->GetOrigin());
		VectorAdd(pCam->GetLook(), vCam, pCam->GetLook());

		pView->RedrawWindow();
	}
	else if(m_nState == STATE_CAMSTRAFE)
	{
		// Move straigt up and down on Y movement.
		// Move left/right on x.

		VectorNormalize(vCam);

		int nDiffX = nMouseX - m_nX;
		int nDiffY = nMouseY - m_nY;

		if(abs((float)nDiffX) < PIXELS_PER_UNIT)
			nDiffX = 0;
		if(abs((float)nDiffY) < PIXELS_PER_UNIT)
			nDiffY = 0;

		if(nDiffX == 0 && nDiffY == 0)
			return;
		
		float fMoveX = -nDiffX / PIXELS_PER_UNIT;
		float fMoveY = -nDiffY / PIXELS_PER_UNIT;

		// Altitude.
		vec3_t vAlt;

		vAlt[0] = 0.0f;
		vAlt[1] = 0.0f;
		vAlt[2] = fMoveY;

		// Move along the left vector.
		// Get up, left vectors.
		vec3_t vLeft, vUp;

		vUp[0] = 0.0f;
		vUp[1] = 0.0f;
		vUp[2] = 1.0f;

		CalcOrthoVector(vUp, vCam, vLeft);
		VectorScale(vLeft, fMoveX, vLeft);
		VectorAdd(vLeft, vAlt, vLeft);
		VectorAdd(pCam->GetLook(), vLeft, pCam->GetLook());
		VectorAdd(pCam->GetOrigin(), vLeft, pCam->GetOrigin());

		pView->RedrawWindow();
	}

	// Lock to position.
	pView->SetCursorToCenter();
}

void C3DWinResponder::OnMouseUp(CBaseWindowEventResponder* pParent, CWindowEventManager* pEventManager, CMapView* pView, CTread3DDoc* pDocument, int nMouseX, int nMouseY, int nButtons)
{
	if(m_nState != STATE_NONE)
	{
		if(pDocument->GetModePtr()->IsMode(MODE_CAMERA))
		{
			// Redraw flat views.
			pDocument->UpdateWindows(FLAT_VIEWS);
		}

		ReleaseCapture();
		while(ShowCursor(TRUE) < 0) {}
	}

	m_nState = STATE_NONE;
}

void C3DWinResponder::OnMouseDown(CBaseWindowEventResponder* pParent, CWindowEventManager* pEventManager, CMapView* pView, CTread3DDoc* pDocument, int nMouseX, int nMouseY, int nButtons)
{
	//int nSelectionFilters = OBJECT_CLASS_ALL&(~pDocument->GetClassSelectionFilters());

	// Click on something?
	if(!pView->GetWindowRenderer())
		return;

	// Move the camera around?
	if((nButtons&~EF_RBUTTON)==0)
	{
		m_nState = STATE_CAMROT;

		pView->SetCapture();
		ShowCursor(FALSE);

		// Lock to position.
		m_nX = pView->m_vWinCenter[0];
		m_nY = pView->m_vWinCenter[1];
		pView->SetCursorToCenter();

		return;
	}

	// Camera mode?
	if(pDocument->GetModePtr()->IsMode(MODE_CAMERA))
	{
		// Left button is down.
		if((nButtons&~EF_LBUTTON)==0)
		{
			m_nState = STATE_CAMMOVE;
			
			pView->SetCapture();
			ShowCursor(FALSE);

			// Lock to position.
			m_nX = pView->m_vWinCenter[0];
			m_nY = pView->m_vWinCenter[1];
			pView->SetCursorToCenter();

			return;
		}
		else if((nButtons&(EF_LBUTTON|EF_RBUTTON)) == (EF_LBUTTON|EF_RBUTTON))
		{
			m_nState = STATE_CAMSTRAFE;

			pView->SetCapture();
			ShowCursor(FALSE);

			// Lock to position.
			m_nX = pView->m_vWinCenter[0];
			m_nY = pView->m_vWinCenter[1];
			pView->SetCursorToCenter();

			return;
		}
	}

	/*if(nButtons&EF_RBUTTON)
	{
		TreadPopupMenu(IDR_MAIN_POPUP, nMouseX, nMouseY, pView);
		return;
	}*/

	pDocument->RemoveGlobalRenderFlags(RF_NOSELBOXSIZE|RF_NOSELBOX);
	pDocument->AddViewGlobalRenderFlags(VIEW_3D, RF_SELBOXONTOP);

	CBaseObject* pObject = pView->GetWindowRenderer()->GetHitObject(pView, pDocument, OBJECT_CLASS_ALL, nMouseX, nMouseY);

	//
	// Quick block for face mode.
	//
	if(pDocument->GetModePtr()->IsMode(MODE_FACE))
	{
		// Clear the selection.
		if((nButtons&EF_CONTROL) == 0)
		{
			pDocument->ClearSurfaceSelection();
#ifdef _WIN32
			GetSurfaceDialog()->LoadFaceValues(false);
#endif
		}
		if(pObject != NULL)
		{
			// If it's a face, add it.
			if(pObject->GetObjectClass() == OBJECT_CLASS_FACE)
			{
				// Already selected?
				if(pObject->m_nObjectRenderFlags&RF_SELECTED)
					pDocument->RemoveObjectFromSurfaceSelection(pObject);
				else if((nButtons&EF_SHIFT)==0)
					pDocument->AddObjectToSurfaceSelection(pObject);
				else // Shifting?
				{
					// Find the parent.
					CBaseObject* pParent = ((CFace*)pObject)->m_pParent;
					if(pParent->GetObjectClass() == OBJECT_CLASS_BRUSH)
					{
						CQBrush* pBrush = (CQBrush*)pParent;
						CFace* pF;

						// Select each face in the brush.
						pBrush->m_pFaceList->First();
						for(pF = pBrush->m_pFaceList->GetCurItem(); pF != NULL; pF = pBrush->m_pFaceList->GetNextItem())
						{
							if(pF->m_nObjectRenderFlags&RF_SELECTED)
								continue;
							pDocument->AddObjectToSurfaceSelection(pF);
						}
					}
				}

#ifdef _WIN32
				GetSurfaceDialog()->LoadFaceValues(pDocument->GetSurfaceObjList()->IsEmpty() == false);	
#endif
				GetPluginAPI()->PostEvent(PEF_SELCHANGE, 0, pDocument);
			}
		}

		pDocument->UpdateWindows();
		return;
	}

	if(pObject != NULL)
	{
		// Face? We break it into a brush for now.
		if(pObject->GetObjectClass()==OBJECT_CLASS_FACE)
			pObject = ((CFace*)pObject)->m_pParent;

		// What do we add?
		CBaseGroupableObject* pGObj = (CBaseGroupableObject*)pObject->GetInterface(I_GROUPABLE);
		if(pGObj != NULL)
		{
			// Get the head group.
			CObjectGroup* pHead = pGObj->GetHeadGroup();
			if(pHead != NULL)
				pObject = pHead;
		}
		
		// Add to selection.
		if(pDocument->GetSelectedObjectList()->InList(pObject))
		{
			// Only control is down?
			if((nButtons&(EF_CONTROL|EF_SHIFT))==EF_CONTROL)
			{
				pDocument->AddObjectToMap(pObject);

				GetPluginAPI()->PostEvent(PEF_SELCHANGE, 0, pDocument);

				if(pDocument->GetSelectedObjectCount(OBJECT_CLASS_ALL&~OBJECT_CLASS_ENTITY) == 0)
					pDocument->AddGlobalRenderFlags(RF_NOSELBOXSIZE|RF_NOSELBOX);
				if(pDocument->GetSelectedObjectCount(OBJECT_CLASS_ALL&~OBJECT_CLASS_ENTITY) > 1)
					pDocument->RemoveViewGlobalRenderFlags(VIEW_3D, RF_SELBOXONTOP);
				pDocument->BuildSelectionBBox();
				pDocument->BuildHotSpots();
				pDocument->UpdateWindows();
				return;
			}
		}
		else if((nButtons&EF_CONTROL)==0 || (nButtons&(EF_CONTROL|EF_SHIFT))==(EF_CONTROL|EF_SHIFT))
			pDocument->ClearSelection();

		if(pObject->GetObjectClass() == OBJECT_CLASS_ENTITY)
			if(pDocument->GetSelectedObjectCount() == 0)
				pDocument->AddGlobalRenderFlags(RF_NOSELBOXSIZE|RF_NOSELBOX);

		pDocument->AddObjectToSelection(pObject);

		GetPluginAPI()->PostEvent(PEF_SELCHANGE, 0, pDocument);

		// Build the bbox.
		pDocument->BuildSelectionBBox();
		pDocument->BuildHotSpots();
		pDocument->UpdateWindows();
	}
	else
	{
		pDocument->ClearSelection();

		GetPluginAPI()->PostEvent(PEF_SELCHANGE, 0, pDocument);

		pDocument->BuildSelectionBBox();
		pDocument->BuildHotSpots();
		pDocument->UpdateWindows();
	}
}