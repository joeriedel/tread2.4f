// CAMERA.CPP
// Copyright © 1999 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#include "stdafx.h"
#include "HotSpotManager.h"
#include "Camera.h"
#include "RenderAPI.h"
#include "TreadSys.h"
#include "MapView.h"
#include "Tread3DDoc.h"
#include "Geometry.h"
#include "futils.h"

int CCamera::InitAfterLoad(CTread3DDoc* pDocument)
{
	// Find our view that we're attached to.
	m_pView = pDocument->GetViewByIndex(m_nView);
	if(m_pView == NULL)
		return IALF_DELETE;
	// Selected?
	if(m_nObjectRenderFlags & RF_SELECTED)
		m_pView->SetCamera(this);

	return IALF_OK;
}

bool CCamera::ReadObject(CFile& hFile, int nVersion)
{
	int bSel;

	hFile.Read(&bSel, 4);
	ReadInt(hFile, m_nView);
	ReadVec3t(hFile, m_vOrigin);
	ReadVec3t(hFile, m_vLook);

	if(bSel)
		m_nObjectRenderFlags = RF_SELECTED;
	else
		m_nObjectRenderFlags = 0;

	return true;
}

bool CCamera::WriteObject(CFile& hFile, int nVersion)
{
	int bSel = (m_nObjectRenderFlags&RF_SELECTED)==RF_SELECTED;

	hFile.Write(&bSel, 4);
	WriteInt(hFile, m_nView);
	WriteVec3t(hFile, m_vOrigin);
	WriteVec3t(hFile, m_vLook);

	return true;
}

void CCamera::SetView(CMapView* pView, CTread3DDoc* pDocument)
{
	m_pView = pView;
	m_nView = pDocument->GetViewIndex(pView);
}

// Make hotspots.
void CCamera::GenerateHotSpots(CHotSpotManager* pHotSpotManager, CMapView* pView)
{
	CHotSpot* pSpot;

	// Make a hotpot for the base
	pSpot = new CHotSpot();

	pSpot->m_nCursor = TC_IDC_CROSS;
	pSpot->m_nDrawFlags = HSDF_NODRAW;
	pSpot->m_fMX = this->m_vOrigin[VI(VEC_X, pView)];
	pSpot->m_fMY = this->m_vOrigin[VI(VEC_Y, pView)];
	pSpot->CopyToLXY();
	pSpot->ComputeWindowXY(pView);
	pSpot->m_nDragRestrictions = HSDR_NONE;
	pSpot->m_pEventResponder = this;
	pSpot->m_pObject = this;
	pSpot->m_nDataFlags = CF_ORIGIN;

	pHotSpotManager->AddHotSpot(pSpot);

	// Make a hotspot for the look.
	pSpot = new CHotSpot();

	pSpot->m_nCursor = TC_IDC_CROSS;
	pSpot->m_nDrawFlags = HSDF_NODRAW;
	pSpot->m_fMX = this->m_vLook[VI(VEC_X, pView)];
	pSpot->m_fMY = this->m_vLook[VI(VEC_Y, pView)];
	pSpot->CopyToLXY();
	pSpot->ComputeWindowXY(pView);
	pSpot->m_nDragRestrictions = HSDR_NONE;
	pSpot->m_pEventResponder = this;
	pSpot->m_pObject = this;
	pSpot->m_nDataFlags = CF_LOOK;

	pHotSpotManager->AddHotSpot(pSpot);

}

CCamera::CCamera() : CBaseObject(), CBaseHotSpotGenerator(), CBaseHotSpotEventResponder()
{
	m_pOldCam = NULL;
}

void CCamera::OnHotSpotDrag(CWindowEventManager* pEventManager, CHotSpotManager* pHotSpotManager, CHotSpot* pHotSpot, CMapView* pView, CTread3DDoc* pDocument, int nMouseX, int nMouseY, int nButtons, int nHSFlags)
{
	pHotSpot->m_nWX = nMouseX;
	pHotSpot->m_nWY = nMouseY;
	pHotSpot->ComputeMapXY(pView);

	// Snap Test.
	/*pHotSpot->m_fMX = G_SnapFloatDelta(pHotSpot->m_fMX, pHotSpot->m_fLX, pDocument->GetGridSize());
	pHotSpot->m_fMY = G_SnapFloatDelta(pHotSpot->m_fMY, pHotSpot->m_fLY, pDocument->GetGridSize());
	pHotSpot->CopyToLXY();*/

	float fDX, fDY;

	switch(pHotSpot->m_nDataFlags)
	{
	case CF_ORIGIN:
		
		fDX = pHotSpot->m_fMX - this->m_vOrigin[VI(VEC_X, pView)];
		fDY = pHotSpot->m_fMY - this->m_vOrigin[VI(VEC_Y, pView)];
		this->m_vOrigin[VI(VEC_X, pView)] = pHotSpot->m_fMX;
		this->m_vOrigin[VI(VEC_Y, pView)] = pHotSpot->m_fMY;
		break;

	case CF_LOOK:
		
		fDX = pHotSpot->m_fMX - this->m_vLook[VI(VEC_X, pView)];
		fDY = pHotSpot->m_fMY - this->m_vLook[VI(VEC_Y, pView)];
		this->m_vLook[VI(VEC_X, pView)] = pHotSpot->m_fMX;
		this->m_vLook[VI(VEC_Y, pView)] = pHotSpot->m_fMY;
		break;
	}

	if(nButtons&EF_SHIFT)
	{
		switch(pHotSpot->m_nDataFlags)
		{
		case CF_ORIGIN:
			this->m_vLook[VI(VEC_X, pView)] += fDX;
			this->m_vLook[VI(VEC_Y, pView)] += fDY;
			break;
		case CF_LOOK:
			this->m_vOrigin[VI(VEC_X, pView)] += fDX;
			this->m_vOrigin[VI(VEC_Y, pView)] += fDY;
			break;
		}
	}

	// Update flat views.
	int nFlags = pView->m_nView;
	if(pDocument->GetGlobalRenderFlags()&RF_REALTIME3D)
		this->m_pView->RedrawWindow();

	pDocument->UpdateWindows(nFlags);
}

void CCamera::OnHotSpotMouseUp(CWindowEventManager* pEventManager, CHotSpotManager* pHotSpotManager, CHotSpot* pHotSpot, CMapView* pView, CTread3DDoc* pDocument, int nMouseX, int nMouseY, int nButtons, int nFlags)
{
	pEventManager->EnableAutoScroll(NULL, false);

	// We may need to refresh the camera.
	if((pDocument->GetGlobalRenderFlags()&RF_REALTIME3D)==0)
		this->m_pView->RedrawWindow();

	pDocument->BuildHotSpots();
	pDocument->UpdateWindows(FLAT_VIEWS);
}

void CCamera::OnHotSpotMouseDown(CWindowEventManager* pEventManager, CHotSpotManager* pHotSpotManager, CHotSpot* pHotSpot, CMapView* pView, CTread3DDoc* pDocument, int nMouseX, int nMouseY, int nButtons, int nFlags)
{
	pEventManager->EnableAutoScroll(pHotSpotManager);
	this->m_pView->SetCamera(this);
	pDocument->UpdateWindows(pView->m_nView);
}

void CCamera::OnHotSpotMouseEnter(CWindowEventManager* pEventManager, CHotSpotManager* pHotSpotManager, CHotSpot* pHotSpot, CMapView* pView, CTread3DDoc* pDocument, int nMouseX, int nMouseY, int nButtons, int nFlags)
{
	// Set the view to this camera?
	if((nButtons&EF_SHIFT) == 0)
		return;

	m_pOldCam = this->m_pView->GetCamera();
	if(m_pOldCam == this)
		return;

	this->m_pView->SetCamera(this);
	pDocument->UpdateWindows(VIEW_3D);
}

void CCamera::OnHotSpotMouseLeave(CWindowEventManager* pEventManager, CHotSpotManager* pHotSpotManager, CHotSpot* pHotSpot, CMapView* pView, CTread3DDoc* pDocument, int nMouseX, int nMouseY, int nButtons, int nFlags)
{
	// Restore old camera.
	if(m_pOldCam == this || m_pOldCam == NULL)
		return;

	this->m_pView->SetCamera(m_pOldCam);
	m_pOldCam = NULL;
	pDocument->UpdateWindows(VIEW_3D);
}
