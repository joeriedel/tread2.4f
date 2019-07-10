// CLIPTOOL.CPP
// Copyright © 1999 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#include "stdafx.h"
#include "ClipTool.h"
#include "Tread3DDoc.h"
#include "Geometry.h"
#include "RenderAPI.h"
#include "MapView.h"

CClipTool::CClipTool()
{
	m_bValid = false;
	m_nViewType = 0;
}

// Notifies us that the selection changed.
void CClipTool::SelectionChanged(CTread3DDoc* pDocument)
{
	// When the selection changes, recopy the selection list
	// and invalidate ourself.
	m_bValid = false;

	// Delete anything we have already.
	m_SrcList.DestroyList();
	m_FrontList.DestroyList();
	m_BackList.DestroyList();

	G_CopyList(pDocument->GetSelectedObjectList(), &m_SrcList);
}

void CClipTool::DoClip(CTread3DDoc* pDocument, bool bSplit)
{
	// Perform the clip operation by deleting the selected objects
	// and inserting the front/back lists as the new selections.
	CBaseObject* pObj;

	pDocument->DeleteSelection();
	
	m_FrontList.First();
	for(;;)
	{
		pObj = m_FrontList.RemoveItem(LL_HEAD);
		if(pObj == NULL)
			break;
		pDocument->AddObjectToSelection(pObj);
	}

	if(bSplit)
	{
		m_BackList.First();
		for(;;)
		{
			pObj = m_BackList.RemoveItem(LL_HEAD);
			if(pObj == NULL)
				break;
			pDocument->AddObjectToSelection(pObj);
		}
	}
	else
		m_BackList.DestroyList();

	m_SrcList.DestroyList();

	SelectionChanged(pDocument);
}

// Make hotspots.
void CClipTool::GenerateHotSpots(CHotSpotManager* pHotSpotManager, CMapView* pView)
{
	if(!m_bValid)
		return;
	if(m_nViewType != pView->m_nView)
		return;

	// Place hotspots over the two endpoints.
	CHotSpot* pSpot;

	for(int i = 0; i < 2; i++)
	{
		// Make a hotpot for the base
		pSpot = new CHotSpot();
		if(pSpot == NULL)
			Fatal("Memory allocation failed in CClipTool::GenerateHotSpots");

		pSpot->m_nClass = OBJECT_CLASS_CLIPTOOL;
		pSpot->m_fMX = m_vPts[i][VI(VEC_X, pView)];
		pSpot->m_fMY = m_vPts[i][VI(VEC_Y, pView)];
		pSpot->ComputeWindowXY(pView);
		pSpot->m_nDrawFlags = HSDF_SQUARE;
		pSpot->m_nCursor = TC_IDC_CROSS;
		pSpot->m_nDragRestrictions = HSDR_NONE;
		pSpot->m_pData = m_vPts[i];
		pSpot->m_pEventResponder = this;
		pHotSpotManager->AddHotSpot(pSpot);

		if(i == 1)
			m_pDragSpot = pSpot;
	}
}

void CClipTool::OnHotSpotDrag(CWindowEventManager* pEventManager, CHotSpotManager* pHotSpotManager, CHotSpot* pHotSpot, CMapView* pView, CTread3DDoc* pDocument, int nMouseX, int nMouseY, int nButtons, int nHSFlags)
{
	// Snap?
	if(pDocument->GetGridSnap())
	{
		m_fX = G_SnapFloatDelta(WX_TO_MX(nMouseX, pView), m_fX, pDocument->GetGridSize());
		m_fY = G_SnapFloatDelta(WY_TO_MY(nMouseY, pView), m_fY, pDocument->GetGridSize());
	}
	else
	{
		m_fX = WX_TO_MX(nMouseX, pView);
		m_fY = WY_TO_MY(nMouseY, pView);
	}

	vec_t* pPt = (vec_t*)pHotSpot->m_pData;
	pPt[VI(VEC_X, pView)] = m_fX;
	pPt[VI(VEC_Y, pView)] = m_fY;
	pHotSpot->m_fMX = m_fX;
	pHotSpot->m_fMY = m_fY;

	// Compute the plane.
	vec3_t vVec1, vVec2, vOrtho;

	VectorSubtract(m_vPts[1], m_vPts[0], vVec1);
	vVec2[VI(VEC_X, pView)] = 0.0F;
	vVec2[VI(VEC_Y, pView)] = 0.0F;
	vVec2[VI(VEC_Z, pView)] = -1.0F;
	VectorNormalize(vVec1);
	CalcOrthoVector(vVec1, vVec2, vOrtho);
	VectorCopy(vOrtho, m_Plane.m_vNormal);
	m_Plane.m_vDist = DotProduct(pPt, vOrtho);

	// Split the object list.
	m_FrontList.DestroyList();
	m_BackList.DestroyList();
	G_SplitObjectList(&m_Plane, &m_SrcList, &m_FrontList, &m_BackList);
	
	int nFlags = pView->m_nView;
	if(pDocument->GetGlobalRenderFlags()&RF_REALTIME3D)
		nFlags |= VIEW_3D;
	pDocument->UpdateWindows(nFlags);
}

void CClipTool::OnHotSpotMouseUp(CWindowEventManager* pEventManager, CHotSpotManager* pHotSpotManager, CHotSpot* pHotSpot, CMapView* pView, CTread3DDoc* pDocument, int nMouseX, int nMouseY, int nButtons, int nFlags)
{
	pDocument->RemoveGlobalRenderFlags(RF_FAST);
	pDocument->UpdateWindows();
}

void CClipTool::OnHotSpotMouseDown(CWindowEventManager* pEventManager, CHotSpotManager* pHotSpotManager, CHotSpot* pHotSpot, CMapView* pView, CTread3DDoc* pDocument, int nMouseX, int nMouseY, int nButtons, int nFlags)
{
	if(pDocument->GetGridSnap())
	{
		m_fX = G_SnapFloat(WX_TO_MX(nMouseX, pView), pDocument->GetGridSize());
		m_fY = G_SnapFloat(WY_TO_MY(nMouseY, pView), pDocument->GetGridSize());
	}

	pDocument->AddGlobalRenderFlags(RF_FAST);
}