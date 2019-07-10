// CCAMERAMODEWINRESPONDER.CPP
// Copyright © 1999 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#include "stdafx.h"
#include "EventManager.h"
#include "HotSpotManager.h"
#include "Tread3DDoc.h"
#include "CameraModeWinResponder.h"
#include "MapView.h"
#include "Geometry.h"
#include "TreadSys.h"
#include "GameAPI.h"
#include "MapObject.h"
#include "Camera.h"

CCameraModeWinResponder::CCameraModeWinResponder()
{
	m_nMode = MODE_CAMERA;
	m_nSubmode = ALL_MODES;
}


void CCameraModeWinResponder::OnMouseDown(CBaseWindowEventResponder* pParent, CWindowEventManager* pEventManager, CMapView* pView, CTread3DDoc* pDocument, int nMouseX, int nMouseY, int nButtons)
{
	// If we're not holding control we do nothing.
	if((nButtons&EF_CONTROL) == 0)
		return;

	CMapView* p3DView = pDocument->GetViewByType(VIEW_3D);
	if(p3DView == NULL)
		return;

	// Insert a new camera, and start dragging the base.
	CCamera* pCam = new CCamera();

	// Fill in position.
	CMapView* pOther = pDocument->GetOtherViewType(pView->m_nView);
	if(pOther == NULL)
	{
		pCam->m_vOrigin[VI(VEC_Z, pView)] = 0.0F;
		pCam->m_vOrigin[VI(VEC_Z, pView)] = 0.0F;
	}
	else
	{
		pCam->m_vOrigin[VI(VEC_Z, pView)] = pOther->m_vMapCenter[V2I(VU(pView), pOther)];
		pCam->m_vLook[VI(VEC_Z, pView)] = pOther->m_vMapCenter[V2I(VU(pView), pOther)];
	}

	pCam->m_vOrigin[VI(VEC_X, pView)] = WX_TO_MX(nMouseX, pView);
	pCam->m_vLook[VI(VEC_X, pView)] = pCam->m_vOrigin[VI(VEC_X, pView)];
	pCam->m_vOrigin[VI(VEC_Y, pView)] = WY_TO_MY(nMouseY, pView);
	pCam->m_vLook[VI(VEC_Y, pView)] = pCam->m_vOrigin[VI(VEC_Y, pView)];

	// FIX ME: At some point when we have more than 1
	// 3D view we need to figure out which one to bind this camera to.
	pCam->SetView(p3DView, pDocument);

	// Delete all the current hotspots.
	pView->GetHotSpotManager()->DeleteHotSpots();
	pView->SetCamera(pCam);
	pDocument->AddObjectToMap(pCam);
	
	CHotSpot* pSpot = new CHotSpot();
	
	pSpot->m_nCursor = TC_IDC_CROSS;
	pSpot->m_nDrawFlags = HSDF_NODRAW;
	pSpot->m_fMX = pCam->m_vOrigin[VI(VEC_X, pView)];
	pSpot->m_fMY = pCam->m_vOrigin[VI(VEC_Y, pView)];
	pSpot->CopyToLXY();
	pSpot->ComputeWindowXY(pView);
	pSpot->m_nDragRestrictions = HSDR_NONE;
	pSpot->m_pEventResponder = pCam;
	pSpot->m_pObject = pCam;
	pSpot->m_nDataFlags = CF_ORIGIN;

	pView->GetHotSpotManager()->AddHotSpot(pSpot);
	pView->GetHotSpotManager()->SetDragSpot(pSpot, nMouseX, nMouseY, nButtons, pDocument, pView, pView->GetWindowEventManager());

	pDocument->UpdateWindows();
}

