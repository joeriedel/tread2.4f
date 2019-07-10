// ANCHOR.CPP
// Copyright © 1999 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#include "stdafx.h"
#include "HotSpotManager.h"
#include "Anchor.h"
#include "RenderAPI.h"
#include "TreadSys.h"
#include "MapView.h"
#include "Tread3DDoc.h"
#include "Geometry.h"
#include "futils.h"

int CAnchor::InitAfterLoad(CTread3DDoc* pDocument)
{
	return IALF_OK;
}

bool CAnchor::ReadObject(CFile& hFile, int nVersion)
{
	ReadString(hFile, m_sName);
	ReadVec3t(hFile, m_vOrigin);
	
	return true;
}

bool CAnchor::WriteObject(CFile& hFile, int nVersion)
{
	WriteString(hFile, m_sName);
	WriteVec3t(hFile, m_vOrigin);
	
	return true;
}

// Make hotspots.
void CAnchor::GenerateHotSpots(CHotSpotManager* pHotSpotManager, CMapView* pView)
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

	pHotSpotManager->AddHotSpot(pSpot);

}

CAnchor::CAnchor() : CBaseObject(), CBaseHotSpotGenerator(), CBaseHotSpotEventResponder()
{
	m_sName = "Anchor";
}

void CAnchor::OnHotSpotDrag(CWindowEventManager* pEventManager, CHotSpotManager* pHotSpotManager, CHotSpot* pHotSpot, CMapView* pView, CTread3DDoc* pDocument, int nMouseX, int nMouseY, int nButtons, int nFlags)
{
	pHotSpot->m_nWX = nMouseX;
	pHotSpot->m_nWY = nMouseY;
	pHotSpot->ComputeMapXY(pView);

	float fDX, fDY;

	fDX = pHotSpot->m_fMX - this->m_vOrigin[VI(VEC_X, pView)];
	fDY = pHotSpot->m_fMY - this->m_vOrigin[VI(VEC_Y, pView)];
	this->m_vOrigin[VI(VEC_X, pView)] = pHotSpot->m_fMX;
	this->m_vOrigin[VI(VEC_Y, pView)] = pHotSpot->m_fMY;

	// Update flat views.
	int nRedraw = pView->m_nView;
	if(pDocument->GetGlobalRenderFlags()&RF_REALTIME3D)
		nRedraw |= VIEW_3D;

	pDocument->UpdateWindows(nRedraw);
}

void CAnchor::OnHotSpotMouseUp(CWindowEventManager* pEventManager, CHotSpotManager* pHotSpotManager, CHotSpot* pHotSpot, CMapView* pView, CTread3DDoc* pDocument, int nMouseX, int nMouseY, int nButtons, int nFlags)
{
	pEventManager->EnableAutoScroll(NULL, false);

	pDocument->BuildHotSpots();
	pDocument->UpdateWindows(FLAT_VIEWS);
}

void CAnchor::OnHotSpotMouseDown(CWindowEventManager* pEventManager, CHotSpotManager* pHotSpotManager, CHotSpot* pHotSpot, CMapView* pView, CTread3DDoc* pDocument, int nMouseX, int nMouseY, int nButtons, int nFlags)
{
	pEventManager->EnableAutoScroll(pHotSpotManager);
	pDocument->UpdateWindows(pView->m_nView);
}

void CAnchor::OnHotSpotMouseEnter(CWindowEventManager* pEventManager, CHotSpotManager* pHotSpotManager, CHotSpot* pHotSpot, CMapView* pView, CTread3DDoc* pDocument, int nMouseX, int nMouseY, int nButtons, int nFlags)
{
}

void CAnchor::OnHotSpotMouseLeave(CWindowEventManager* pEventManager, CHotSpotManager* pHotSpotManager, CHotSpot* pHotSpot, CMapView* pView, CTread3DDoc* pDocument, int nMouseX, int nMouseY, int nButtons, int nFlags)
{
}
