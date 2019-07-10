// POINTERMODEWINRESPONDER.CPP
// Copyright © 1999 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#include "stdafx.h"
#include "EventManager.h"
#include "Tread3DDoc.h"
#include "PointerModeWinResponder.h"
#include "RenderAPI.h"
#include "MapView.h"
#include "Geometry.h"
#include "TreadSys.h"
#include "GameAPI.h"
#include "MapObject.h"
#include "PluginAPI.h"

#define NONE		0x00000000
#define DRAGGING	0x00000001
#define SIZING		0x00000002
#define BBOX		0x00000003
#define MOUSESCROLL	0x00000004

#define SENSATIVITY	3

CPointerModeWinResponder::CPointerModeWinResponder()
{
	m_nMode = MODE_POINTER;
	m_nSubmode = ALL_MODES;
	
	m_nState = NONE;
	m_bMoved = false;
	m_bCanSwitch = false;
}

void CPointerModeWinResponder::OnKeyDown(CBaseWindowEventResponder* pParent, CWindowEventManager* pEventManager, CMapView* pView, CTread3DDoc* pDocument, int nKeyCode)
{
	switch(nKeyCode)
	{
	case 'Q':
		pDocument->InvokeAction(EF_ACTION_CLOSEHORZ);
		break;
	case 'W':
		pDocument->InvokeAction(EF_ACTION_CLOSEVERT);
		break;
	}
}

void CPointerModeWinResponder::OnMouseMove(CBaseWindowEventResponder* pParent, CWindowEventManager* pEventManager, CMapView* pView, CTread3DDoc* pDocument, int nMouseX, int nMouseY, int nButtons)
{
	// Any state?
	if(m_nState == NONE)
		return;

	switch(m_nState)
	{
	case MOUSESCROLL:

		m_fX += m_fX - WX_TO_MX(nMouseX, pView);
		m_fY += m_fY - WY_TO_MY(nMouseY, pView);

		pView->SetMapXY(m_fX, m_fY);
		pView->SetCursorToCenter();
		pView->RedrawWindow();

		break;

	case BBOX:

		// Plug in coords.
		pDocument->m_vSelMaxs[VI(VEC_X, pView)] = WX_TO_MX(nMouseX, pView);
		pDocument->m_vSelMaxs[VI(VEC_Y, pView)] = WY_TO_MY(nMouseY, pView);
		pView->RedrawWindow();

		break;

	case DRAGGING:

		if(abs((float)nMouseX-m_nX) < SENSATIVITY)
			if(abs((float)nMouseY-m_nY) < SENSATIVITY)
				return;

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
					G_Snap(&GData, pDocument->GetGridSize(), vMoveTotal);
					m_bMoved = !VectorIsZero(vMoveTotal);
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
						G_SnapVector(pEntity->m_vOrigin, pEntity->m_vOrigin, pDocument->GetGridSize(), vMoveTotal);
					}

					// For undo.
					m_bMoved = false;//!VectorIsZero(vMoveTotal);
				}
			}

			m_bSnapped = true;
		}
		else
		{
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

			m_nX = nMouseX;
			m_nY = nMouseY;
			m_bMoved = true;

			// Translate the selections.
			vec3_t vTrans;

			vTrans[VI(VEC_X, pView)] = fDX;
			vTrans[VI(VEC_Y, pView)] = fDY;
			vTrans[VI(VEC_Z, pView)] = 0.0F;

			VectorAdd(vTrans, vMoveTotal, vMoveTotal);

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
		}

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

void CPointerModeWinResponder::OnMouseDown(CBaseWindowEventResponder* pParent, CWindowEventManager* pEventManager, CMapView* pView, CTread3DDoc* pDocument, int nMouseX, int nMouseY, int nButtons)
{
	int nSelectionFilters = OBJECT_CLASS_ALL&(~pDocument->GetClassSelectionFilters());
	
	// Click on something?
	if(!pView->GetWindowRenderer())
		return;

	// Both scrolls around the window.
	if((nButtons&(EF_RBUTTON|EF_LBUTTON))==(EF_RBUTTON|EF_LBUTTON))
	{
		m_nState = MOUSESCROLL;
		ShowCursor(false);
		pView->SetCapture();
		pView->SetCursorToCenter();
		m_fX = pView->m_vMapCenter[0];
		m_fY = pView->m_vMapCenter[1];
		pView->AddGlobalRenderFlags(RF_FAST);
		return;
	}

	if(nButtons&EF_RBUTTON)
	{
		pDocument->StoreXY(nMouseX, nMouseY, pView);
#ifdef _WIN32
		TreadPopupMenu(IDR_MAIN_POPUP, nMouseX, nMouseY, pView);
#endif
		return;
	}

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
				pDocument->BuildSelectionBBox();
				pDocument->BuildHotSpots();


				/*int nFlags;

				if(pDocument->GetUpdateAllViews())
					nFlags = FLAT_VIEWS;
				else
					nFlags = pView->m_nView;

				pDocument->UpdateWindows(nFlags);*/
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
		m_nX = nMouseX;
		m_nY = nMouseY;
		m_fX = WX_TO_MX((float)nMouseX, pView);
		m_fY = WY_TO_MY((float)nMouseY, pView);

		m_bSnapped = false;

		m_bMoved = false;
		VectorClear(vMoveTotal);

		// Duplicate the objects.
		if((nButtons&(EF_CONTROL|EF_SHIFT)) == (EF_CONTROL|EF_SHIFT))
			pDocument->DuplicateSelection();

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

		/*int nFlags;

		if(pDocument->GetUpdateAllViews())
			nFlags = FLAT_VIEWS;
		else
			nFlags = pView->m_nView;

		pDocument->UpdateWindows(nFlags);*/
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
							G_Snap(&GData, pDocument->GetGridSize(), vMoveTotal);
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
								G_SnapVector(pEntity->m_vOrigin, pEntity->m_vOrigin, pDocument->GetGridSize(), vMoveTotal);
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

					/*int nFlags;

					if(pDocument->GetUpdateAllViews())
						nFlags = FLAT_VIEWS;
					else
						nFlags = pView->m_nView;

					pDocument->UpdateWindows(nFlags);*/
					pDocument->UpdateWindows(pView->m_nView);
					pDocument->CycleColorIfSelected();
					return;
				}
			}

			// Clear the selection.
			if((nButtons&EF_CONTROL)==0)
			{
				pDocument->ClearSelection();
				// Post the event to the plugin system.
				GetPluginAPI()->PostEvent(PEF_SELCHANGE, 0, pDocument);
			}
		}
		
		pDocument->AddGlobalRenderFlags(RF_FAST|RF_NOSELBOX|RF_NOSELBOXSIZE);
		pView->AddGlobalRenderFlags(RF_FORCESELBOX);

		// Plug in coords.
		pDocument->m_vSelMins[VI(VEC_X, pView)] = WX_TO_MX(nMouseX, pView);
		pDocument->m_vSelMins[VI(VEC_Y, pView)] = WY_TO_MY(nMouseY, pView);
		VectorCopy(pDocument->m_vSelMins, pDocument->m_vSelMaxs);
		m_nState = BBOX;

		pDocument->BuildHotSpots();

		/*int nFlags;

		if(pDocument->GetUpdateAllViews())
			nFlags = FLAT_VIEWS;
		else
			nFlags = pView->m_nView;

		pDocument->UpdateWindows(nFlags);*/
		pDocument->UpdateWindows(pView->m_nView);
		pDocument->CycleColorIfSelected();
	}
}

void CPointerModeWinResponder::OnMouseUp(CBaseWindowEventResponder* pParent, CWindowEventManager* pEventManager, CMapView* pView, CTread3DDoc* pDocument, int nMouseX, int nMouseY, int nButtons)
{
	pEventManager->EnableAutoScroll(NULL, false);

	if(m_nState == NONE)
		return;

	if(m_nState == MOUSESCROLL)
	{
		ReleaseCapture();
		while(ShowCursor(true) < 0) {}
		m_nState = NONE;
		pView->RemoveGlobalRenderFlags(RF_FAST);
		pDocument->UpdateWindows(ALL_VIEWS);
		return;
	}

	if(m_nState == BBOX)
	{
		// Add objects to the map based on the selection box.
		CBaseObject* pObj;
		CLinkedList<CBaseObject>* pList = pDocument->GetSelectedObjectList();
		CLinkedList<CBaseObject>* pSel = new CLinkedList<CBaseObject>();
		CLinkedList<CBaseObject>* pNew = new CLinkedList<CBaseObject>();
		
		OrderRange(pDocument->m_vSelMins, pDocument->m_vSelMaxs);

		pList->First();
		for(;;)
		{
			pObj = pList->GetCurItem();
			if(pObj == NULL)
				break;
			if(pObj->GetObjectInterfaces()&I_TRANSFORMABLE)
			{
				CBaseTransformableObject* pT = (CBaseTransformableObject*)pObj->GetInterface(I_TRANSFORMABLE);
				if(pT != NULL)
				{
					if(pT->In2DRange(pDocument->m_vSelMins, pDocument->m_vSelMaxs,
						pView->m_nView))
					{
						pList->RemoveItem(pObj);
						pSel->AddItem(pObj);
						continue;
					}
				}
			}
			pList->GetNextItem();
		}

		pList = pDocument->GetObjectList();
		pList->First();
		for(;;)
		{
			pObj = pList->GetCurItem();
			if(pObj == NULL)
				break;
			// Test for visibility.
			if(pObj->GetObjectInterfaces()&I_VISGROUPREF)
			{
				CVisgroupRefObject* pVR = (CVisgroupRefObject*)pObj->GetInterface(I_VISGROUPREF);
				if(pVR != NULL)
				{
					if(pVR->IsVisible() == false)
					{
						pList->GetNextItem();
						continue;
					}
				}
			}

			if(pObj->GetObjectInterfaces()&I_TRANSFORMABLE)
			{
				CBaseTransformableObject* pT = (CBaseTransformableObject*)pObj->GetInterface(I_TRANSFORMABLE);
				if(pT != NULL)
				{
					if(pT->In2DRange(pDocument->m_vSelMins, pDocument->m_vSelMaxs,
						pView->m_nView))
					{
						pList->RemoveItem(pObj);
						pNew->AddItem(pObj);
						continue;
					}
				}
			}
			pList->GetNextItem();
		}

		if(pNew->IsEmpty() == false)
			pDocument->RemoveGlobalRenderFlags(RF_NOSELBOX|RF_NOSELBOXSIZE);

		// Add all object into selection list.
		pNew->First();
		for(;;)
		{
			pObj = pNew->RemoveItem(LL_HEAD);
			if(pObj == NULL)
				break;
			pDocument->AddObjectToSelection(pObj);
		}
		delete pNew;

		// Add object to normal map.
		pSel->First();
		for(;;)
		{
			pObj = pSel->RemoveItem(LL_HEAD);
			if(pObj == NULL)
				break;
			pDocument->AddObjectToMap(pObj);
		}

		delete pSel;

		pDocument->UpdateObjectCounts();
		pView->RemoveGlobalRenderFlags(RF_FORCESELBOX);

		// Post the event to the plugin system.
		GetPluginAPI()->PostEvent(PEF_SELCHANGE, 0, pDocument);
		pDocument->CycleColorIfSelected();
	}

	// Undo.
	if(m_nState == DRAGGING && m_bMoved)
	{
		UNDOITEM* pItem = CUndoAPI::CreateUndoItem(pDocument->GetSelectedObjectList());
		pItem->nFlags = UAF_TRANSLATE;
		VectorCopy(vMoveTotal, pItem->vUVals);
		pDocument->GetUndoAPI()->AddUndoItem(pItem, pDocument);
	}
	
	// Didn't move? switch modes.
	if(m_bCanSwitch && !m_bMoved)
	{
		// Switch.
		if((pDocument->GetModePtr()->GetSubmode()&POINTER_ROTATE)==0)
			pDocument->GetModePtr()->SetSubmode(POINTER_ROTATE);
		else
			pDocument->GetModePtr()->SetSubmode(SUB_NONE);
	}

	m_nState = NONE;
	pDocument->RemoveGlobalRenderFlags(RF_NOHOTSPOTS|RF_FAST);
	pDocument->BuildSelectionBBox();
	pDocument->BuildHotSpots();
	pDocument->UpdateWindows();
}
