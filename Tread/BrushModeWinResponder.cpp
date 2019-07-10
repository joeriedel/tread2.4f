// BRUSHMODEWINRESPONDER.CPP
// Copyright © 1999 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#include "stdafx.h"
#include "EventManager.h"
#include "Tread3DDoc.h"
#include "BrushModeWinResponder.h"
#include "RenderAPI.h"
#include "MapView.h"
#include "Geometry.h"
#include "TreadSys.h"
#include "Models.h"
#include "GameAPI.h"
#include "MapObject.h"
#include "UndoAPI.h"

CBrushModeWinResponder::CBrushModeWinResponder()
{ 
	m_nMode = MODE_BRUSH;
	m_nSubmode = ALL_MODES;
	m_bSizing = false; 
}

void CBrushModeWinResponder::OnMouseMove(CBaseWindowEventResponder* pParent, CWindowEventManager* pEventManager, CMapView* pView, CTread3DDoc* pDocument, int nMouseX, int nMouseY, int nButtons)
{
	// Don't care if not sizing.
	if(!m_bSizing)
		return;
	
	if(pDocument->GetGridSnap())
	{
		pDocument->m_vSelMaxs[VI(VEC_X, pView)] = G_SnapFloatDelta(WX_TO_MX(nMouseX, pView), pDocument->m_vSelMaxs[VI(VEC_X, pView)], pDocument->GetGridSize());
		pDocument->m_vSelMaxs[VI(VEC_Y, pView)] = G_SnapFloatDelta(WY_TO_MY(nMouseY, pView), pDocument->m_vSelMaxs[VI(VEC_Y, pView)], pDocument->GetGridSize());
	}
	else
	{
		pDocument->m_vSelMaxs[VI(VEC_X, pView)] = WX_TO_MX(nMouseX, pView);
		pDocument->m_vSelMaxs[VI(VEC_Y, pView)] = WY_TO_MY(nMouseY, pView);
	}

	// Compute size.
	ComputeRangeOrigin(pDocument->m_vSelMins, pDocument->m_vSelMaxs, pDocument->m_vSelCenter);

	int nFlags = FLAT_VIEWS;
	if(pDocument->GetGlobalRenderFlags()&RF_REALTIME3D)
		nFlags |= VIEW_3D;
	pDocument->UpdateWindows(nFlags);
}

void CBrushModeWinResponder::OnMouseDown(CBaseWindowEventResponder* pParent, CWindowEventManager* pEventManager, CMapView* pView, CTread3DDoc* pDocument, int nMouseX, int nMouseY, int nButtons)
{
	// Store the point.
	pDocument->m_vSelMins[VI(VEC_X, pView)] = WX_TO_MX(nMouseX, pView);
	pDocument->m_vSelMins[VI(VEC_Y, pView)] = WY_TO_MY(nMouseY, pView);

	// Setup z.
	CMapView* pOther = pDocument->GetOtherViewType(pView->m_nView);
	if(pOther == NULL)
		pDocument->m_vSelMins[VI(VEC_Z, pView)] = 0.0F;
	else
		pDocument->m_vSelMins[VI(VEC_Z, pView)] = pOther->m_vMapCenter[V2I(VU(pView), pOther)];
	
	// Snap?
	if(pDocument->GetGridSnap())
		G_SnapVector(pDocument->m_vSelMins, pDocument->m_vSelMins, pDocument->GetGridSize());

	VectorCopy(pDocument->m_vSelMins, pDocument->m_vSelMaxs);

	if(pOther == NULL)
		pDocument->m_vSelMaxs[VI(VEC_Z, pView)] = (float)pDocument->GetGridSize();
	else
		pDocument->m_vSelMaxs[VI(VEC_Z, pView)] = pDocument->m_vSelMins[VI(VEC_Z, pView)] + (float)pDocument->GetGridSize();

	ComputeRangeOrigin(pDocument->m_vSelMins, pDocument->m_vSelMaxs, pDocument->m_vSelCenter);

	// Is this a model or a brush?
	MODEL* pModel = GetGameAPI()->GetCurrentGame()->GetCurrentModel();
	if(pModel == NULL)
	{
		Warning("No currently selected model.\n");
		return;
	}

	if((pModel->m_Header.m_nFlags&(T2XHF_MODEL|T2XHF_RTOBJECT))==(T2XHF_MODEL|T2XHF_RTOBJECT))
	{
		Warning("Model with bad header flags!\n");
		return;
	}

	if(pModel->m_Header.m_nFlags&T2XHF_MODEL)
	{
		// Copy the models geometry;
		CLinkedList<CBaseObject>* pList = new CLinkedList<CBaseObject>();
		
		// Copy.
		G_CopyList(pModel->m_pObjectList, pList);

		GDATA GData;

		GData.m_nFlags = GF_ORIGIN|GF_RANGE;
		GData.m_pList = pList;
		G_InitFromSelection(pList, pDocument, pView, pDocument->m_vSelMins, pDocument->m_vSelMaxs, pDocument->m_vSelCenter);
		if(!(pModel->m_Header.m_nFlags&T2XHF_NOAUTOSIZE))
		{
			G_Build(&GData);
			G_SetOrigin(pDocument->m_vSelMins, &GData);
			G_Build(&GData);
		}

		pDocument->ClearSelection();
		G_AddToSelection(pList, pDocument);
		delete pList;
		pDocument->RemoveGlobalRenderFlags(RF_FORCESELBOX);
		pDocument->UpdateWindows();
		m_bIgnore = true;
		m_bSizing = false;
		return;
	}

	m_bIgnore = false;

	m_bSizing = true;
	pEventManager->EnableAutoScroll(this);

	/*UNDOITEM* pItem = CUndoAPI::CreateUndoItem(pDocument->GetSelectedObjectList());
	pItem->nFlags = UAF_CREATE|UAF_SELECT;
	pDocument->GetUndoAPI()->AddUndoItem(pItem);*/

	// Force the bbox to be drawn.
	pDocument->AddGlobalRenderFlags(RF_FORCESELBOX);
	int nFlags = FLAT_VIEWS;
	if(pDocument->GetGlobalRenderFlags()&RF_REALTIME3D)
		nFlags |= VIEW_3D;
	pDocument->UpdateWindows(nFlags);
}

void CBrushModeWinResponder::OnMouseUp(CBaseWindowEventResponder* pParent, CWindowEventManager* pEventManager, CMapView* pView, CTread3DDoc* pDocument, int nMouseX, int nMouseY, int nButtons)
{
	if(m_bIgnore)
		return;

	m_bSizing = false;

	pEventManager->EnableAutoScroll(NULL, false);

	// Create a new model of the type selected.
	MODEL* pModel = GetGameAPI()->GetCurrentGame()->GetCurrentModel();
	if(pModel == NULL)
	{
		Warning("No currently selected model.\n");
		pDocument->RemoveGlobalRenderFlags(RF_FORCESELBOX);
		pDocument->UpdateWindows(FLAT_VIEWS);
		return;
	}

	// Set the size of the model.
	vec3_t vSize;
	vec3_t vOrigin;

	OrderRange(pDocument->m_vSelMins, pDocument->m_vSelMaxs);
	ComputeRangeOrigin(pDocument->m_vSelMins, pDocument->m_vSelMaxs, vOrigin);
	VectorSubtract(pDocument->m_vSelMaxs, pDocument->m_vSelMins, vSize);
	
	if(vSize[0] < 1.0F || vSize[1] < 1.0F || vSize[2] < 1.0F)
	{
		Warning("Degenerate brush not created.\n");
		pDocument->RemoveGlobalRenderFlags(RF_FORCESELBOX);
		pDocument->UpdateWindows();
		return;
	}
		
	// Copy the models geometry;
	CLinkedList<CBaseObject>* pList = new CLinkedList<CBaseObject>();
	
	if(pModel->m_Header.m_nFlags&T2XHF_RTOBJECT)
	{
		// Ask the system to make it.
		CBaseObject* pObj = NewBaseObject(pModel->m_Header.m_nClass);
		if(pObj == NULL)
		{
			Warning("unable to instantiate object of type: %x\n", pModel->m_Header.m_nClass);
			return;
		}

		pList->AddItem(pObj);
	}
	else
	{
		// Copy.
		G_CopyList(pModel->m_pObjectList, pList);
	}

	GDATA GData;

	GData.m_nFlags = GF_ORIGIN|GF_RANGE;
	GData.m_pList = pList;
	G_InitFromSelection(pList, pDocument, pView, pDocument->m_vSelMins, pDocument->m_vSelMaxs, vOrigin);

	if(!(pModel->m_Header.m_nFlags&T2XHF_NOAUTOSIZE))
	{
		G_Build(&GData);
		G_SetSize(vSize, &GData);
		G_SetOrigin(vOrigin, &GData);
		G_Build(&GData);
	}

	// Copy to the map.
	//if(pModel->m_nFlags&T2XHF_BRUSH)
	G_TextureList(pList, GetTexAPI()->GetCurTextureName());
	
	pDocument->ClearSelection();
	G_AddToSelection(pList, pDocument);
	delete pList;
	
	/*UNDOITEM* pItem = CUndoAPI::CreateUndoItem(pDocument->GetSelectedObjectList());
	pItem->nFlags = UAF_CREATE|UAF_SELECT;
	pDocument->GetUndoAPI()->AddUndoItem(pItem);*/

	pDocument->RemoveGlobalRenderFlags(RF_FORCESELBOX);
	pDocument->UpdateWindows();
}
