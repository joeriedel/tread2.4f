// VERTEXMODEWINRESPONDER.CPP
// Copyright © 1999 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#include "stdafx.h"
#include "EventManager.h"
#include "Tread3DDoc.h"
#include "VertexModeWinResponder.h"
#include "RenderAPI.h"
#include "MapView.h"
#include "Geometry.h"
#include "TreadSys.h"
#include "GameAPI.h"
#include "MapObject.h"

#define NONE		0x00000000
#define DRAGGING	0x00000001
#define SIZING		0x00000002

CVertexModeWinResponder::CVertexModeWinResponder()
{
	m_nMode = MODE_VERTEX;
	m_nSubmode = ALL_MODES;
	m_nState = NONE;
	m_bMoved = false;
	m_bCanSwitch = false;
	m_bSnapped = false;
}

void CVertexModeWinResponder::OnKeyDown(CBaseWindowEventResponder* pParent, CWindowEventManager* pEventManager, CMapView* pView, CTread3DDoc* pDocument, int nKeyCode)
{
}

void CVertexModeWinResponder::OnMouseMove(CBaseWindowEventResponder* pParent, CWindowEventManager* pEventManager, CMapView* pView, CTread3DDoc* pDocument, int nMouseX, int nMouseY, int nButtons)
{
	// Any state?
	if(m_nState == NONE)
		return;

	switch(m_nState)
	{
	case DRAGGING:

		if(!m_bSnapped)
		{
			// Lock?
			if(pDocument->GetGridSnap())
			{
				m_fX = G_SnapFloat(m_fX, pDocument->GetGridSize());
				m_fY = G_SnapFloat(m_fY, pDocument->GetGridSize());

				// Snap the selections.
				if(pDocument->GetSelectedObjectCount(OBJECT_CLASS_ALL&~OBJECT_CLASS_ENTITY) > 0)
				{
					GDATA GData;
					GData.m_pList = pDocument->GetSelectedObjectList();
					VectorCopy(pDocument->m_vSelMins, GData.m_vMins);
					G_Snap(&GData, pDocument->GetGridSize(), m_vMoveTotal);
					m_bMoved = !VectorIsZero(m_vMoveTotal);
				}
				else
				{
					// Translate and snap each entity.
					CBaseObject* pObject;
					CEntity* pEntity;
					CLinkedList<CBaseObject>* pList = pDocument->GetSelectedObjectList();
					pList->First();

					for(pObject = pList->GetCurItem(); pObject != NULL; pObject = pList->GetNextItem())
					{
						pEntity = (CEntity*)pObject;
						G_SnapVector(pEntity->m_vOrigin, pEntity->m_vOrigin, pDocument->GetGridSize());
					}
				}

				m_bSnapped = true;

				// Update.
				int nFlags;

				if(pDocument->GetUpdateAllViews())
					nFlags = FLAT_VIEWS;
				else
					nFlags = pView->m_nView;

				if(pDocument->GetGlobalRenderFlags()&RF_REALTIME3D)
					nFlags |= VIEW_3D;

				pDocument->UpdateWindows(nFlags);
			}
		}

		// Drag the selections.
		float fX = WX_TO_MX((float)nMouseX, pView);
		float fY = WY_TO_MY((float)nMouseY, pView);

		// Snap?
		if(pDocument->GetGridSnap())
		{
			fX = G_SnapFloatDelta(fX, m_fX, pDocument->GetGridSize());
			fY = G_SnapFloatDelta(fY, m_fY, pDocument->GetGridSize());
		}

		// Change?
		float fDX = fX - m_fX;
		float fDY = fY - m_fY;

		if(fDX == 0.0F && fDY == 0.0F)
			return;

		m_bMoved = true;

		// Translate the selections.
		vec3_t vTrans;

		vTrans[VI(VEC_X, pView)] = fDX;
		vTrans[VI(VEC_Y, pView)] = fDY;
		vTrans[VI(VEC_Z, pView)] = 0.0F;

		VectorAdd(vTrans, m_vMoveTotal, m_vMoveTotal);

		if(pDocument->GetSelectedObjectCount(OBJECT_CLASS_ALL&~OBJECT_CLASS_ENTITY) > 0)
		{
			GDATA GData;
			GData.m_pList = pDocument->GetSelectedObjectList();
			G_Translate(vTrans, &GData);
		}
		else
		{
			// Translate and snap each entity.
			CBaseObject* pObject;
			CEntity* pEntity;
			CLinkedList<CBaseObject>* pList = pDocument->GetSelectedObjectList();
			pList->First();

			for(pObject = pList->GetCurItem(); pObject != NULL; pObject = pList->GetNextItem())
			{
				pEntity = (CEntity*)pObject;
				pEntity->Translate(vTrans);

				// Snap?
				if(pDocument->GetGridSnap())
					G_SnapVector(pEntity->m_vOrigin, pEntity->m_vOrigin, pDocument->GetGridSize());
			}
		}

		m_fX = fX;
		m_fY = fY;

		// Update.
		int nFlags;
		if(pDocument->GetUpdateAllViews())
			nFlags = FLAT_VIEWS;
		else
			nFlags = pView->m_nView;

		if(pDocument->GetGlobalRenderFlags()&RF_REALTIME3D)
			nFlags |= VIEW_3D;
		pDocument->UpdateWindows(nFlags);
		break;
	}
}

void CVertexModeWinResponder::OnMouseDown(CBaseWindowEventResponder* pParent, CWindowEventManager* pEventManager, CMapView* pView, CTread3DDoc* pDocument, int nMouseX, int nMouseY, int nButtons)
{
	int nSelectionFilters = OBJECT_CLASS_ALL&(~pDocument->GetClassSelectionFilters());
	
	// Click on something?
	if(!pView->GetWindowRenderer())
		return;

	if(nButtons&EF_RBUTTON)
	{
#ifdef _WIN32
		TreadPopupMenu(IDR_MAIN_POPUP, nMouseX, nMouseY, pView);
#endif
		return;
	}

	m_bMoved = m_bCanSwitch = false;
	pEventManager->EnableAutoScroll(this);

	CBaseObject* pObject = pView->GetWindowRenderer()->GetHitObject(pView, pDocument, nSelectionFilters, nMouseX, nMouseY);
	if(pObject != NULL)
	{
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
				pDocument->BuildSelectionBBox();
				pDocument->BuildHotSpots();
				pDocument->UpdateWindows(pView->m_nView);
				pDocument->CycleColorIfSelected();
				return;
			}

			m_bCanSwitch = true;
		}
		else if((nButtons&EF_CONTROL)==0 || (nButtons&(EF_CONTROL|EF_SHIFT))==(EF_CONTROL|EF_SHIFT))
			pDocument->ClearSelection();

		VectorClear(m_vMoveTotal);

		pDocument->AddObjectToSelection(pObject);
		// Build the bbox.
		pDocument->BuildSelectionBBox();

		// Drag.
		m_fX = WX_TO_MX((float)nMouseX, pView);
		m_fY = WY_TO_MY((float)nMouseY, pView);

		m_bSnapped = false;

		// Duplicate the objects.
		if((nButtons&(EF_CONTROL|EF_SHIFT)) == (EF_CONTROL|EF_SHIFT))
			pDocument->DuplicateSelection();

		// Drag the objects.
		m_nState = DRAGGING;
		pView->GetHotSpotManager()->DeleteHotSpots();
		pDocument->AddGlobalRenderFlags(RF_NOHOTSPOTS|RF_FAST);
		pDocument->UpdateWindows(pView->m_nView);
		pDocument->CycleColorIfSelected();
		return;
	}
	else
	{
		if(pDocument->GetSelectedObjectList()->IsEmpty() == false)
		{
			// Did we click inside the bbox?
			m_fX = WX_TO_MX((float)nMouseX, pView);
			m_fY = WY_TO_MY((float)nMouseY, pView);

			// Do a BBox selection.
			pView->GetHotSpotManager()->BeginBBoxSelection(pParent, pEventManager, pView, pDocument, nMouseX, nMouseY, nButtons);
		}
		else
			return;
	}
}

void CVertexModeWinResponder::OnMouseUp(CBaseWindowEventResponder* pParent, CWindowEventManager* pEventManager, CMapView* pView, CTread3DDoc* pDocument, int nMouseX, int nMouseY, int nButtons)
{
	pEventManager->EnableAutoScroll(NULL, false);

	if(m_nState == NONE)
		return;

	// Undo.
	if(m_nState == DRAGGING && m_bMoved)
	{
		UNDOITEM* pItem = CUndoAPI::CreateUndoItem(pDocument->GetSelectedObjectList());
		pItem->nFlags = UAF_TRANSLATE;
		VectorCopy(m_vMoveTotal, pItem->vUVals);
		pDocument->GetUndoAPI()->AddUndoItem(pItem, pDocument);
	}

	m_nState = NONE;
	pDocument->RemoveGlobalRenderFlags(RF_NOHOTSPOTS|RF_FAST);
	pDocument->BuildSelectionBBox();
	pDocument->BuildHotSpots();
	pDocument->UpdateWindows();
}
