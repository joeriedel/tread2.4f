// HOTSPOTMANAGER.CPP
// Copyright © 1999 Joe Riedel.
// Author: Joe Riedel.
#include "stdafx.h"
#include "EventManager.h"
#include "HotSpotManager.h"
#include "RenderAPI.h"
#include "TreadSys.h"
#include "MapView.h"

#define HOTSPOT_SIZE	3

CHotSpot::CHotSpot()
{
	m_fMX = m_fMY = 0.0F;
	m_nCursor = TC_DEFAULT;
	m_nDragRestrictions = m_nDrawFlags = 0;
	m_nWX = m_nWY = 0;
	m_pData = NULL;
	m_pEventResponder = NULL;
	m_pObject = NULL;
	m_nViewTypeFilters = 0;
	m_nClass = 0;
}

CHotSpot::~CHotSpot()
{
}

void CHotSpot::CopyToLXY()
{
	m_fLX = m_fMX;
	m_fLY = m_fMY;
}

void CHotSpot::ComputeMapXY(CMapView* pView)
{
	m_fMX = WX_TO_MX((float)m_nWX, pView);
	m_fMY = WY_TO_MY((float)m_nWY, pView);
}

void CHotSpot::ComputeWindowXY(CMapView* pView)
{
	// Build the Win X/Y
	m_nWX = (int)MX_TO_WX(m_fMX, pView);
	m_nWY = (int)MY_TO_WY(m_fMY, pView);
}

CHotSpotManager::CHotSpotManager()
{
	m_nTrackState = TS_NONE;
	m_pTrackSpot = NULL;
	m_pStoreList = NULL;
	m_bGetFromSave = true;
	m_bMulti=false;
	m_pSelList = new CLinkedList<HSPOTREF>();
	m_pHotSpotList = new CLinkedList<CHotSpot>();
	m_pGeneratorList = new CLinkedList<CBaseHotSpotModeGenerator>();
}

CHotSpotManager::~CHotSpotManager()
{
	if(m_pHotSpotList != NULL)
		delete m_pHotSpotList;
	if(m_pGeneratorList != NULL)
		delete m_pGeneratorList;
	if(m_pStoreList != NULL)
		delete m_pStoreList;
	if(m_pSelList != NULL)
		delete m_pSelList;

	
	m_pSelList = NULL;
	m_pStoreList = NULL;
	m_pHotSpotList = NULL;
	m_pGeneratorList = NULL;
}

bool CHotSpotManager::EnableMultipleSelection(bool bEnable)
{
	m_bMulti = bEnable;
	return true;
}

void CHotSpotManager::AddHotSpotToSelection(CHotSpot* pSpot)
{
	HSPOTREF* pRef = new HSPOTREF();
	pRef->pSpot = pSpot;
	pSpot->m_nObjectRenderFlags |= RF_SELECTED;
	m_pSelList->AddItem(pRef);
}

void CHotSpotManager::ClearSelectedHotSpots()
{
	HSPOTREF* pRef;
	m_pSelList->First();
	for(pRef = m_pSelList->GetCurItem(); pRef != NULL; pRef = m_pSelList->GetNextItem())
		pRef->pSpot->m_nObjectRenderFlags &= ~RF_SELECTED;
	
	m_pSelList->DestroyList();
}

void CHotSpotManager::RemoveHotSpotFromSelection(CHotSpot* pSpot)
{
	// Find the ref.
	HSPOTREF* pRef;
	m_pSelList->First();
	for(pRef = m_pSelList->GetCurItem(); pRef != NULL; pRef = m_pSelList->GetNextItem())
	{
		if(pRef->pSpot == pSpot)
		{
			m_pSelList->DeleteItem(pRef);
			pSpot->m_nObjectRenderFlags &= ~RF_SELECTED;
			return;
		}
	}
}

void CHotSpotManager::InitIdenticalSearch()
{
	m_pIdentSpot = NULL;
	m_bGetFromSave = false;
	m_pHotSpotList->First();
}

void CHotSpotManager::SaveIdenticalSearch(CHotSpot* pHotSpot, CMapView* pView, int nFlags)
{
	// Save each hotspot in a new list.
	InitIdenticalSearch();
	CHotSpot* pSpot;
	HSPOTREF* pRef;

	if(m_pStoreList != NULL)
		delete m_pStoreList;
	m_pStoreList = new CLinkedList<HSPOTREF>();

	for(;;)
	{
		pSpot = GetNextIdenticalHotSpot(pHotSpot, pView, nFlags);
		if(pSpot == NULL)
			break;
		pRef = new HSPOTREF();
		pRef->pSpot = pSpot;
		m_pStoreList->AddItem(pRef);
	}
}

void CHotSpotManager::RestoreIdenticalSearch()
{
	m_bGetFromSave = true;
	m_pStoreList->First();
	m_pIdentSpot = NULL;
}

CHotSpot* CHotSpotManager::GetNextIdenticalHotSpot(CHotSpot* pHotSpot, CMapView* pView, int nFlags)
{
	if(m_bGetFromSave)
	{
		HSPOTREF* pRef;
		if(m_pIdentSpot == NULL)
			pRef = m_pStoreList->GetCurItem();
		else
			pRef = m_pStoreList->GetNextItem();
		if(pRef == NULL)
			return NULL;
		m_pIdentSpot = pRef->pSpot;
		return m_pIdentSpot;
	}

	if(m_pIdentSpot == NULL)
		m_pIdentSpot = m_pHotSpotList->GetCurItem();

	for(; m_pIdentSpot != NULL; m_pIdentSpot = m_pHotSpotList->GetNextItem())
	{
		// Identical to model?
		if(nFlags & ISF_CLASS)
			if(m_pIdentSpot->m_nClass != pHotSpot->m_nClass)
				continue;
		if(nFlags & ISF_CODE)
			if(m_pIdentSpot->m_nCode != pHotSpot->m_nCode)
				continue;
		if(nFlags & ISF_MASK)
			if(((nFlags&ISF_MASKS)&m_pIdentSpot->m_nFlags) != (m_pIdentSpot->m_nFlags&ISF_MASKS))
				continue;
		if(nFlags & ISF_WINX)
			if(m_pIdentSpot->m_nWX != pHotSpot->m_nWX)
				continue;
		if(nFlags & ISF_WINY)
			if(m_pIdentSpot->m_nWY != pHotSpot->m_nWY)
				continue;
		if(pView != NULL)
		{
			if(nFlags & ISF_PTX)
				if(m_pIdentSpot->m_vPt[VI(VEC_X, pView)] != pHotSpot->m_vPt[VI(VEC_X, pView)])
					continue;
			if(nFlags & ISF_PTY)
				if(m_pIdentSpot->m_vPt[VI(VEC_Y, pView)] != pHotSpot->m_vPt[VI(VEC_Y, pView)])
					continue;
			if(nFlags & ISF_PTZ)
				if(m_pIdentSpot->m_vPt[VI(VEC_Z, pView)] != pHotSpot->m_vPt[VI(VEC_Z, pView)])
					continue;
		}

		// For next time.
		CHotSpot* pSpot = m_pIdentSpot;
		m_pIdentSpot = m_pHotSpotList->GetNextItem();
		return pSpot;
	}

	return NULL;
}

void CHotSpotManager::AddModeGenerator(CBaseHotSpotModeGenerator* pHG)
{
	m_pGeneratorList->AddItem(pHG);
}

void CHotSpotManager::AddHotSpot(CHotSpot* pSpot)
{
	m_pHotSpotList->AddItem(pSpot);
}

void CHotSpotManager::DeleteHotSpots()
{
	//if(m_pHotSpotList->IsEmpty())
	//	return;
	m_pHotSpotList->DestroyList();
}

void CHotSpotManager::BuildHotSpots(CTread3DDoc* pDocument, CMapView* pView)
{

	int nGCount=0;
	CBaseHotSpotModeGenerator* pHG;

	m_nTrackState = TS_TRACKING;
	m_pTrackSpot = NULL;

	// Kill all current hotspots.
	DeleteHotSpots();

	int nMode = pDocument->GetModePtr()->GetMode();
	int nSubmode = pDocument->GetModePtr()->GetSubmode();

	m_pGeneratorList->First();
	for(pHG = m_pGeneratorList->GetCurItem(); pHG != NULL; pHG = m_pGeneratorList->GetNextItem())
	{
		if((pHG->m_nMode & nMode) == nMode &&
			(pHG->m_nSubmode & nSubmode) == nSubmode)
		{
			nGCount++;
			pHG->GenerateHotSpots(this, pDocument, pView);
		}
	}

}

bool CHotSpotManager::OverSpot(CHotSpot* pSpot, int nMouseX, int nMouseY)
{
	int nDiffX = abs((float)pSpot->m_nWX - nMouseX);
	int nDiffY = abs((float)pSpot->m_nWY - nMouseY);

	if(nDiffX <= HOTSPOT_SIZE && nDiffY <= HOTSPOT_SIZE)
		return true;

	return false;
}

bool CHotSpotManager::FindTrackSpot(CWindowEventManager* pManager, CMapView* pView, CTread3DDoc* pDocument, int nMouseX, int nMouseY, int nButtons)
{
	CHotSpot* pSpot, *pTrack=NULL;

	m_pHotSpotList->First();
	for(pSpot = m_pHotSpotList->GetCurItem(); pSpot != NULL; pSpot = m_pHotSpotList->GetNextItem())
	{
		if(OverSpot(pSpot, nMouseX, nMouseY))
		{
			pTrack = pSpot;
			break;
		}
	}

	// Different?
	if(pTrack != m_pTrackSpot)
	{
		if(m_pTrackSpot != NULL)
			m_pTrackSpot->m_pEventResponder->OnHotSpotMouseLeave(pManager, this, m_pTrackSpot, pView, pDocument, nMouseX, nMouseY, nButtons, HEF_NONE);

		// Fire the event, and set the cursor.
		if(pTrack != NULL)
		{
			pTrack->m_pEventResponder->OnHotSpotMouseEnter(pManager, this, pTrack, pView, pDocument, nMouseX, nMouseY, nButtons, HEF_NONE);
			TreadSetCursor(pView, pTrack->m_nCursor);
		}
		else
			TreadSetCursor(pView, TC_DEFAULT);
	}

	m_pTrackSpot = pTrack;
	return pTrack != NULL;
}

bool CHotSpotManager::Track(CWindowEventManager* pEventManager, CMapView* pView, CTread3DDoc* pDocument, int nMouseX, int nMouseY, int nButtons)
{
	if(nButtons&EF_SHIFT)
	{
		TreadSetCursor(pView, TC_DEFAULT);
		/*if(m_pHotSpotList->IsEmpty() == false)
		{
			DeleteHotSpots();
			pView->RedrawWindow();
		}*/
		return false;
	}

	if(FindTrackSpot(pEventManager, pView, pDocument, nMouseX, nMouseY, nButtons))
	{
		if((nButtons&EF_LBUTTON)==0)
		{
			// Got a track spot, set us as the global receiver.
			pEventManager->SetGlobalResponder(this);
			m_nTrackState = TS_TRACKING;
			return true;
		}
	}

	return false;
}

void CHotSpotManager::OnMouseMove(CBaseWindowEventResponder* pParent, CWindowEventManager* pEventManager, CMapView* pView, CTread3DDoc* pDocument, int nMouseX, int nMouseY, int nButtons)
{
	// What's our tracking state?
	switch(m_nTrackState)
	{
	case TS_BBOX:

		// Store.
		pDocument->m_vSelMins[VI(VEC_X, pView)] = WX_TO_MX(nMouseX, pView);
		pDocument->m_vSelMins[VI(VEC_Y, pView)] = WY_TO_MY(nMouseY, pView);
		
		// Draw.
		pDocument->UpdateWindows(FLAT_VIEWS);

		break;

	case TS_TRACKING:

		// Hopefully the mouse button isn't down.
		if(nButtons & (EF_LBUTTON|EF_RBUTTON))
		{
			Console("HSM: TS_TRACKING w/ EF_LBUTTON|EF_RBUTTON\n");
			return;
		}

		// Still over a track spot.
		if((nButtons&EF_SHIFT)==0 && FindTrackSpot(pEventManager, pView, pDocument, nMouseX, nMouseY, nButtons))
			return;
		if(nButtons&EF_SHIFT)
			m_pTrackSpot = NULL;

		// Not on the trackspot anymore, disable global responder.
		pEventManager->SetGlobalResponder(NULL);
		m_nTrackState = TS_NONE;
		break;

	case TS_DRAGGING:

		if(!(nButtons & (EF_LBUTTON|EF_RBUTTON)))
		{
			Console("HSM: TS_DRAGGING w/o EF_LBUTTON|EF_RBUTTON\n");
			return;
		}

		if(m_pSelList->GetCount()  < 2)
		{
			if((m_pTrackSpot->m_nDragRestrictions&HSDR_HORZ)==HSDR_HORZ)
				m_nMouseX = nMouseX;
			else
				m_nMouseX = m_pTrackSpot->m_nWX;
			if((m_pTrackSpot->m_nDragRestrictions&HSDR_VERT)==HSDR_VERT)
				m_nMouseY = nMouseY;
			else
				m_nMouseY = m_pTrackSpot->m_nWY;
			m_nButtons = nButtons;

			// Send the event down the responder chain.
			m_pTrackSpot->m_pEventResponder->OnHotSpotDrag(pEventManager, this, m_pTrackSpot, pView, pDocument, m_nMouseX, m_nMouseY, m_nButtons, HEF_NONE);
		}
		else
		{
			HSPOTREF* pRef;
			int nFlags=0;

			// See if the trackspot moves.
			
			m_nButtons = nButtons;

			// Apply restrictions.
			if((m_pTrackSpot->m_nDragRestrictions&HSDR_HORZ)==HSDR_HORZ)
				m_nMouseX = nMouseX;
			else
				m_nMouseX = m_pTrackSpot->m_nWX;

			if((m_pTrackSpot->m_nDragRestrictions&HSDR_VERT)==HSDR_VERT)
				m_nMouseY = nMouseY;
			else
				m_nMouseY = m_pTrackSpot->m_nWY;

			float fOldX = m_pTrackSpot->m_fMX;
			float fOldY = m_pTrackSpot->m_fMY;

			m_pTrackSpot->m_pEventResponder->OnHotSpotDrag(pEventManager, this, m_pTrackSpot, pView, pDocument, m_nMouseX, m_nMouseY, m_nButtons, HEF_GROUP|HEF_HEAD);

			//m_pTrackSpot->ComputeWindowXY(pView);

			float fDeltaX = m_pTrackSpot->m_fMX - fOldX;
			float fDeltaY = m_pTrackSpot->m_fMY - fOldY;

			//if(fDeltaX == 0.0F && fDeltaY == 0.0F)
			//	return;

			// Convert to window coords.
			//fDeltaX = MX_TO_WX(fDeltaX, pView);
			//fDeltaY = MY_TO_WY(fDeltaY, pView);

			//Debug("Delta: %d %d\n", nDeltaX, nDeltaY);

			m_pSelList->First();
			for(pRef = m_pSelList->GetCurItem(); pRef != NULL; pRef = m_pSelList->GetNextItem())
			{
				if(pRef->pSpot == m_pTrackSpot && pRef->next != NULL)
					continue;

				nFlags=HEF_GROUP;
				if(pRef->next == NULL)
					nFlags |= HEF_TAIL;

				// Apply restrictions.
				if((pRef->pSpot->m_nDragRestrictions&HSDR_HORZ)==HSDR_HORZ)
					m_nMouseX = MX_TO_WX(pRef->pSpot->m_fMX + fDeltaX, pView);
				else
					m_nMouseX = pRef->pSpot->m_nWX;

				if((pRef->pSpot->m_nDragRestrictions&HSDR_VERT)==HSDR_VERT)
					m_nMouseY = MY_TO_WY(pRef->pSpot->m_fMY + fDeltaY, pView);
				else
					m_nMouseY = pRef->pSpot->m_nWY;

				pRef->pSpot->m_pEventResponder->OnHotSpotDrag(pEventManager, this, pRef->pSpot, pView, pDocument, m_nMouseX, m_nMouseY, m_nButtons, nFlags);
			}
		}

		break;
	}

}

void CHotSpotManager::SetDragSpot(CHotSpot* pDragSpot, int nMouseX, int nMouseY, int nButtons, CTread3DDoc* pDocument, CMapView* pView, CWindowEventManager* pEventManager)
{
	m_pTrackSpot = pDragSpot;
	m_nTrackState = TS_DRAGGING;
	pEventManager->SetGlobalResponder(this);
	pDragSpot->m_pEventResponder->OnHotSpotMouseDown(pEventManager, this, pDragSpot, pView, pDocument, nMouseX, nMouseY, nButtons, HEF_NONE);
	TreadSetCursor(pView, pDragSpot->m_nCursor);
}

void CHotSpotManager::SetTrackSpot(CHotSpot* pTrackSpot)
{
	m_pTrackSpot = pTrackSpot;
}

bool CHotSpotManager::BeginBBoxSelection(CBaseWindowEventResponder* pParent, CWindowEventManager* pEventManager, CMapView* pView, CTread3DDoc* pDocument, int nMouseX, int nMouseY, int nButtons)
{
	// We become the controller.
	pEventManager->SetGlobalResponder(this);
	pEventManager->EnableAutoScroll(this);

	m_nTrackState = TS_BBOX;
	m_nOldRenderFlags = pDocument->GetGlobalRenderFlags();

	if((nButtons&EF_CONTROL) == 0)
		ClearSelectedHotSpots();

	pDocument->RemoveGlobalRenderFlags(RF_ALL);
	pDocument->AddGlobalRenderFlags(RF_NOSELBOX|RF_NOSELBOXSIZE|RF_FAST);
	pView->AddGlobalRenderFlags(RF_FORCESELBOX);

	pDocument->m_vSelMins[VI(VEC_X, pView)] = WX_TO_MX(nMouseX, pView);
	pDocument->m_vSelMins[VI(VEC_Y, pView)] = WY_TO_MY(nMouseY, pView);
	VectorCopy(pDocument->m_vSelMins, pDocument->m_vSelMaxs);

	// Remove all selected hotspots.
	pDocument->UpdateWindows(FLAT_VIEWS);
	return true;
}

void CHotSpotManager::OnMouseDown(CBaseWindowEventResponder* pParent, CWindowEventManager* pEventManager, CMapView* pView, CTread3DDoc* pDocument, int nMouseX, int nMouseY, int nButtons)
{
	// Only left clicks work here.
	if(nButtons & EF_RBUTTON)
		return;

	m_nMouseX = nMouseX;
	m_nMouseY = nMouseY;
	m_nButtons = nButtons;

	switch(m_nTrackState)
	{
	case TS_TRACKING:
		
		// Ctrl?
		/*if(nButtons&EF_CONTROL)
		{
			// Allowing multiple selections?
			if(m_bMulti)
			{
				// toggle.
				if(m_pTrackSpot->m_nObjectRenderFlags&RF_SELECTED)
					RemoveHotSpotFromSelection(m_pTrackSpot);
				else
					AddHotSpotToSelection(m_pTrackSpot);

				pDocument->UpdateWindows(FLAT_VIEWS);
				return;
			}
		}*/

		// Switch to dragging the hotspot.
		m_nTrackState = TS_DRAGGING;

		if((m_pTrackSpot->m_nDragRestrictions&HSDR_HORZ)==HSDR_HORZ)
			m_nMouseX = nMouseX;
		else
			m_nMouseX = m_pTrackSpot->m_nWX;
		if((m_pTrackSpot->m_nDragRestrictions&HSDR_VERT)==HSDR_VERT)
			m_nMouseY = nMouseY;
		else
			m_nMouseY = m_pTrackSpot->m_nWY;

		m_nOldX = nMouseX;
		m_nOldY = nMouseY;

		if((m_pTrackSpot->m_nObjectRenderFlags&RF_SELECTED) == 0)
			ClearSelectedHotSpots();

		// Send the event down the responder chain.
		m_pTrackSpot->m_pEventResponder->OnHotSpotMouseDown(pEventManager, this, m_pTrackSpot, pView, pDocument, nMouseX, nMouseY, nButtons, HEF_NONE);


		break;

	case TS_DRAGGING:

		Console("HSM: Mousedown in TS_DRAGGING\n");
		break;
	}
}

void CHotSpotManager::OnMouseUp(CBaseWindowEventResponder* pParent, CWindowEventManager* pEventManager, CMapView* pView, CTread3DDoc* pDocument, int nMouseX, int nMouseY, int nButtons)
{
	if(nButtons & EF_RBUTTON)
		return;

	// Reset the event state to tracking.
	switch(m_nTrackState)
	{
	case TS_BBOX:
		{
			pEventManager->SetGlobalResponder(NULL);
			pEventManager->EnableAutoScroll(NULL, false);
			
			// Restore.
			pDocument->RemoveGlobalRenderFlags(RF_ALL);
			pDocument->AddGlobalRenderFlags(m_nOldRenderFlags);
			pView->RemoveGlobalRenderFlags(RF_FORCESELBOX);

			pDocument->m_vSelMins[VI(VEC_Z, pView)] = (float)-0x7FFFFFFF;
			pDocument->m_vSelMaxs[VI(VEC_Z, pView)] = (float)0x7FFFFFFF;

			// Order the range, and add the hotspots to the selection.
			OrderRange(pDocument->m_vSelMins, pDocument->m_vSelMaxs);

			// See what is in the box.
			CHotSpot* pSpot;
			m_pHotSpotList->First();
			for(pSpot = m_pHotSpotList->GetCurItem(); pSpot != NULL; pSpot = m_pHotSpotList->GetNextItem())
			{
				if(pSpot->m_fMX >= pDocument->m_vSelMins[VI(VEC_X, pView)] &&
					pSpot->m_fMX <= pDocument->m_vSelMaxs[VI(VEC_X, pView)] &&
					pSpot->m_fMY >= pDocument->m_vSelMins[VI(VEC_Y, pView)] &&
					pSpot->m_fMY <= pDocument->m_vSelMaxs[VI(VEC_Y, pView)])
				{
					if(nButtons&EF_CONTROL)
					{
						if(pSpot->m_nObjectRenderFlags&RF_SELECTED)
							RemoveHotSpotFromSelection(pSpot);
						else
							AddHotSpotToSelection(pSpot);
					}
					else
						AddHotSpotToSelection(pSpot);

				}
			}

			// Redraw the views.
			pDocument->UpdateWindows(FLAT_VIEWS);

		}	
		break;

	case TS_TRACKING:

		Console("HSM: Mouseup in TS_TRACKING\n");
		break;

	case TS_DRAGGING:
		{
			m_nTrackState = TS_NONE;
			TreadSetCursor(pView, TC_DEFAULT);
			pEventManager->SetGlobalResponder(NULL);

			if(m_pTrackSpot == NULL)
				break;

			if(m_pSelList->GetCount()  < 2)
			{
				if((m_pTrackSpot->m_nDragRestrictions&HSDR_HORZ)==HSDR_HORZ)
					m_nMouseX = nMouseX;
				else
					m_nMouseX = m_pTrackSpot->m_nWX;
				if((m_pTrackSpot->m_nDragRestrictions&HSDR_VERT)==HSDR_VERT)
					m_nMouseY = nMouseY;
				else
					m_nMouseY = m_pTrackSpot->m_nWY;
				m_nButtons = nButtons;

				// Send the event down the responder chain.
				m_pTrackSpot->m_pEventResponder->OnHotSpotMouseUp(pEventManager, this, m_pTrackSpot, pView, pDocument, m_nMouseX, m_nMouseY, m_nButtons, HEF_NONE);
			}
			else
			{
				HSPOTREF* pRef;
				int nFlags=0;

				// See if the trackspot moves.
				m_nButtons = nButtons;

				// Apply restrictions.
				if((m_pTrackSpot->m_nDragRestrictions&HSDR_HORZ)==HSDR_HORZ)
					m_nMouseX = nMouseX;
				else
					m_nMouseX = m_pTrackSpot->m_nWX;

				if((m_pTrackSpot->m_nDragRestrictions&HSDR_VERT)==HSDR_VERT)
					m_nMouseY = nMouseY;
				else
					m_nMouseY = m_pTrackSpot->m_nWY;

				float fOldX = m_pTrackSpot->m_fMX;
				float fOldY = m_pTrackSpot->m_fMY;

				m_pTrackSpot->m_pEventResponder->OnHotSpotMouseUp(pEventManager, this, m_pTrackSpot, pView, pDocument, m_nMouseX, m_nMouseY, m_nButtons, HEF_GROUP|HEF_HEAD);

				if(m_pTrackSpot == NULL)
					break;

				//m_pTrackSpot->ComputeWindowXY(pView);

				float fDeltaX = m_pTrackSpot->m_fMX - fOldX;
				float fDeltaY = m_pTrackSpot->m_fMY - fOldY;

				// Convert to window coords.
				//fDeltaX = MX_TO_WX(fDeltaX, pView);
				//fDeltaY = MY_TO_WY(fDeltaY, pView);

				//Debug("Delta: %d %d\n", nDeltaX, nDeltaY);

				m_pSelList->First();
				for(pRef = m_pSelList->GetCurItem(); pRef != NULL; pRef = m_pSelList->GetNextItem())
				{
					if(pRef->pSpot == m_pTrackSpot && pRef->next != NULL)
						continue;

					nFlags=HEF_GROUP;
					if(pRef->next == NULL)
						nFlags |= HEF_TAIL;

					// Apply restrictions.
					if((pRef->pSpot->m_nDragRestrictions&HSDR_HORZ)==HSDR_HORZ)
						m_nMouseX = MX_TO_WX(pRef->pSpot->m_fMX + fDeltaX, pView);
					else
						m_nMouseX = pRef->pSpot->m_nWX;

					if((pRef->pSpot->m_nDragRestrictions&HSDR_VERT)==HSDR_VERT)
						m_nMouseY = MY_TO_WY(pRef->pSpot->m_fMY + fDeltaY, pView);
					else
						m_nMouseY = pRef->pSpot->m_nWY;

					pRef->pSpot->m_pEventResponder->OnHotSpotMouseUp(pEventManager, this, pRef->pSpot, pView, pDocument, m_nMouseX, m_nMouseY, m_nButtons, nFlags);
				}
			}

			m_pTrackSpot = NULL;

		}
		break;
	}
}

void CHotSpotManager::OnKeyDown(CBaseWindowEventResponder* pParent, CWindowEventManager* pEventManager, CMapView* pView, CTread3DDoc* pDocument, int nKeyCode) 
{
}