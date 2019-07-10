// CLIPTOOLMODEWINRESPONDER.CPP
// Copyright © 1999 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#include "stdafx.h"
#include "EventManager.h"
#include "Tread3DDoc.h"
#include "ClipToolModeWinResponder.h"
#include "RenderAPI.h"
#include "MapView.h"
#include "Geometry.h"
#include "TreadSys.h"
#include "GameAPI.h"
#include "MapObject.h"
#include "ClipTool.h"
#include "PluginAPI.h"

#define NONE		0x00000000
#define DRAGGING	0x00000001
#define SIZING		0x00000002

CClipToolModeWinResponder::CClipToolModeWinResponder()
{
	m_nMode = MODE_CLIP;
	m_nSubmode = ALL_MODES;
	m_nState = NONE;
	m_bMoved = false;
	m_bCanSwitch = false;
}

void CClipToolModeWinResponder::OnKeyDown(CBaseWindowEventResponder* pParent, CWindowEventManager* pEventManager, CMapView* pView, CTread3DDoc* pDocument, int nKeyCode)
{
	CClipTool* pClipTool = (CClipTool*)G_FindObject(pDocument->GetUIObjectList(), OBJECT_CLASS_CLIPTOOL);

	if(nKeyCode == ' ')
	{
		pDocument->InvokeAction(EF_ACTION_CLIP, NULL);
		return;
	}

	pClipTool->SelectionChanged(pDocument);

}

void CClipToolModeWinResponder::OnMouseMove(CBaseWindowEventResponder* pParent, CWindowEventManager* pEventManager, CMapView* pView, CTread3DDoc* pDocument, int nMouseX, int nMouseY, int nButtons)
{
	// Any state?
	if(m_nState == NONE)
		return;

	switch(m_nState)
	{
	case DRAGGING:

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
		int nFlags = pView->m_nView;
		if(pDocument->GetGlobalRenderFlags()&RF_REALTIME3D)
			nFlags |= VIEW_3D;
		pDocument->UpdateWindows(nFlags);
		break;
	}
}

void CClipToolModeWinResponder::OnMouseDown(CBaseWindowEventResponder* pParent, CWindowEventManager* pEventManager, CMapView* pView, CTread3DDoc* pDocument, int nMouseX, int nMouseY, int nButtons)
{
	int nSelectionFilters = OBJECT_CLASS_ALL&(~OBJECT_CLASS_ENTITY);
	
	CClipTool* pClipTool = (CClipTool*)G_FindObject(pDocument->GetUIObjectList(), OBJECT_CLASS_CLIPTOOL);
	pClipTool->m_nViewTypeFilters = ALL_VIEWS&(~pView->m_nView);

	// Click on something?
	if(!pView->GetWindowRenderer())
		return;

	pDocument->RemoveGlobalRenderFlags(RF_NOSELBOXSIZE|RF_NOSELBOX);
	pDocument->AddViewGlobalRenderFlags(VIEW_3D, RF_SELBOXONTOP);

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

				// Post the event to the plugin system.
				GetPluginAPI()->PostEvent(PEF_SELCHANGE, 0, pDocument);

				if(pDocument->GetSelectedObjectCount(OBJECT_CLASS_ALL&~OBJECT_CLASS_ENTITY) == 0)
					pDocument->AddGlobalRenderFlags(RF_NOSELBOXSIZE|RF_NOSELBOX);
				if(pDocument->GetSelectedObjectCount(OBJECT_CLASS_ALL&~OBJECT_CLASS_ENTITY) > 1)
					pDocument->RemoveViewGlobalRenderFlags(VIEW_3D, RF_SELBOXONTOP);
				pClipTool->SelectionChanged(pDocument);
				pDocument->BuildSelectionBBox();
				pDocument->BuildHotSpots();
				pDocument->UpdateWindows(pView->m_nView);
				return;
			}

			m_bCanSwitch = true;
		}
		else if((nButtons&EF_CONTROL)==0 || (nButtons&(EF_CONTROL|EF_SHIFT))==(EF_CONTROL|EF_SHIFT))
			pDocument->ClearSelection();

		pDocument->AddObjectToSelection(pObject);
		// Build the bbox.
		pDocument->BuildSelectionBBox();

		// Drag.
		m_fX = WX_TO_MX((float)nMouseX, pView);
		m_fY = WY_TO_MY((float)nMouseY, pView);

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
				G_Snap(&GData, pDocument->GetGridSize());
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
		}

		// Duplicate the objects.
		if((nButtons&(EF_CONTROL|EF_SHIFT)) == (EF_CONTROL|EF_SHIFT))
			pDocument->DuplicateSelection();

		pClipTool->SelectionChanged(pDocument);

		// Post the event to the plugin system.
		GetPluginAPI()->PostEvent(PEF_SELCHANGE, 0, pDocument);

		// Drag the objects.
		m_nState = DRAGGING;
		pView->GetHotSpotManager()->DeleteHotSpots();
		pDocument->AddGlobalRenderFlags(RF_NOHOTSPOTS|RF_FAST);
		if(pDocument->GetSelectedObjectCount(OBJECT_CLASS_ALL&~OBJECT_CLASS_ENTITY) == 0 && pDocument->GetSelectedObjectCount(OBJECT_CLASS_ENTITY) == 1)
			pDocument->AddGlobalRenderFlags(RF_NOSELBOXSIZE|RF_NOSELBOX);
		if(pDocument->GetSelectedObjectCount(OBJECT_CLASS_ALL&~OBJECT_CLASS_ENTITY) > 1)
			pDocument->RemoveViewGlobalRenderFlags(VIEW_3D, RF_SELBOXONTOP);
		pDocument->UpdateWindows(pView->m_nView);
		return;
	}
	else
	{
		if(pDocument->GetSelectedObjectList()->IsEmpty() == false)
		{
			// Did we click inside the bbox?
			m_fX = WX_TO_MX((float)nMouseX, pView);
			m_fY = WY_TO_MY((float)nMouseY, pView);

			if(m_fX >= pDocument->m_vSelMins[VI(VEC_X, pView)] && m_fX <= pDocument->m_vSelMaxs[VI(VEC_X, pView)])
			{
				if(m_fY >= pDocument->m_vSelMins[VI(VEC_Y, pView)] && m_fY <= pDocument->m_vSelMaxs[VI(VEC_Y, pView)])
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
							G_Snap(&GData, pDocument->GetGridSize());
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
					}

					// Duplicate the objects.
					if((nButtons&(EF_CONTROL|EF_SHIFT)) == (EF_CONTROL|EF_SHIFT))
					{
						pDocument->DuplicateSelection();
						// Post the event to the plugin system.
						GetPluginAPI()->PostEvent(PEF_SELCHANGE, 0, pDocument);
					}

					// Drag the objects.
					m_bCanSwitch = true;
					m_nState = DRAGGING;
					pView->GetHotSpotManager()->DeleteHotSpots();
					pDocument->AddGlobalRenderFlags(RF_NOHOTSPOTS|RF_FAST);
					pDocument->UpdateWindows(pView->m_nView);
					return;
				}
			}
			
			// Setup clip points.

			int i;
			for(i = 0; i < 2; i++)
			{
				pClipTool->m_vPts[i][VI(VEC_X, pView)] = WX_TO_MX(nMouseX, pView);
				pClipTool->m_vPts[i][VI(VEC_Y, pView)] = WY_TO_MY(nMouseY, pView);
				pClipTool->m_vPts[i][VI(VEC_Z, pView)] = 0.0F;
			}

			pClipTool->m_nViewType = pView->m_nView;
			pClipTool->m_bValid = true;

			// Build the spots.
			pDocument->BuildHotSpots();

			// Start dragging the clip tool.
			pView->GetHotSpotManager()->SetDragSpot(pClipTool->GetDragSpot(), nMouseX, nMouseY, nButtons, pDocument, pView, pView->GetWindowEventManager());
			pDocument->AddGlobalRenderFlags(RF_FAST);
			pDocument->UpdateWindows(pView->m_nView);
		}
		else
			return;
	}
}

void CClipToolModeWinResponder::OnMouseUp(CBaseWindowEventResponder* pParent, CWindowEventManager* pEventManager, CMapView* pView, CTread3DDoc* pDocument, int nMouseX, int nMouseY, int nButtons)
{
	CClipTool* pClipTool = (CClipTool*)G_FindObject(pDocument->GetUIObjectList(), OBJECT_CLASS_CLIPTOOL);

	pEventManager->EnableAutoScroll(NULL, false);

	if(m_nState == NONE)
		return;

	// Didn't move? switch modes.
	if(m_bCanSwitch && !m_bMoved)
	{
		// Switch.
		if(pDocument->GetModePtr()->GetSubmode() == SUB_NONE)
			pDocument->GetModePtr()->SetSubmode(POINTER_ROTATE);
		else
			pDocument->GetModePtr()->SetSubmode(SUB_NONE);
	}

	m_nState = NONE;

	pClipTool->SelectionChanged(pDocument);
	pDocument->RemoveGlobalRenderFlags(RF_NOHOTSPOTS|RF_FAST);
	pDocument->BuildSelectionBBox();
	pDocument->BuildHotSpots();
	pDocument->UpdateWindows();
}
