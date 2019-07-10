// EVENTMANAGER.CPP
// Copyright © 1999 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#include "stdafx.h"
#include "LinkedList.h"
#include "EventManager.h"
#include "MapView.h"
#include "ChildFrm.h"
#include "Mode.h"
#include "TreadSys.h"
#include "Tread3DDoc.h"
#include "HotSpotManager.h"
#include "DefaultActionResponder.h"

#define SCROLL_TIMER_INTERVAL		50
#define EM_TIMER_ID					7776

CModeChangeEventManager::CModeChangeEventManager()
{
	m_pResponderList = new CLinkedList<CBaseModeChangeResponder>();
}

CModeChangeEventManager::~CModeChangeEventManager()
{
	if(m_pResponderList != NULL)
		delete m_pResponderList;
	m_pResponderList = NULL;
}

void CModeChangeEventManager::AddEventResponder(CBaseModeChangeResponder* pResponder)
{
	m_pResponderList->AddItem(pResponder);
}

void CModeChangeEventManager::InvokeAction(int nAction, void* pParam, CTread3DDoc* pDocument)
{
	CBaseModeChangeResponder* pResp;
	int nMode = GetMapManager()->GetModePtr()->GetMode();
	int nSubmode = GetMapManager()->GetModePtr()->GetSubmode();
	int nRecvCount=0;

	Debug("sysInvokeAction: action-code %d\n", nAction);
	Debug("... processing\n");

	m_pResponderList->First();
	for(pResp = m_pResponderList->GetCurItem(); pResp != NULL; pResp = m_pResponderList->GetNextItem())
	{
		if((pResp->m_nMode & nMode) == nMode &&
			(pResp->m_nSubmode & nSubmode) == nSubmode)
		{
			nRecvCount++;
			if(pResp->OnAction(nAction, pParam, this, GetMapManager()->GetModePtr(), pDocument) == ARF_DEFAULT)
				CDefaultActionResponder::OnAction(nAction, pParam, this, GetMapManager()->GetModePtr(), pDocument);
		}

	}

	Debug("... %d reciever(s)\n", nRecvCount);
}

void CModeChangeEventManager::OnModeChange(CMode* pModeManager, CTread3DDoc* pDocument, int nOldMode, int nOldSubmode, int nNewMode, int nNewSubmode)
{
	int nRecvCount=0;
	CBaseModeChangeResponder* pResp;

	Debug("sysModeChange: old: %d, %d new: %d, %d\n", nOldMode, nOldSubmode, nNewMode, nNewSubmode);
	Debug("... processing\n");

	// Don't respond to same mode.
	if((nNewMode&nOldMode)==nNewMode)
	{
		if((nNewSubmode&nOldSubmode)==nNewSubmode)
			return;
	}
	
	// Notify old ones first.
	if(nOldMode != 0)
	{
		m_pResponderList->First();
		for(pResp = m_pResponderList->GetCurItem(); pResp != NULL; pResp = m_pResponderList->GetNextItem())
		{
			if((pResp->m_nMode & nOldMode) == nOldMode &&
				(pResp->m_nSubmode & nOldSubmode) == nOldSubmode)
			{
				nRecvCount++;
				pResp->OnModeChange(this, pModeManager, pDocument, nOldMode, nOldSubmode, nNewMode, nNewSubmode);
			}
		}
	}

	Debug("... %d switch recievers\n", nRecvCount);
	nRecvCount=0;

	// Notify attached.
	m_pResponderList->First();
	for(pResp = m_pResponderList->GetCurItem(); pResp != NULL; pResp = m_pResponderList->GetNextItem())
	{
		if((pResp->m_nMode & nNewMode) == nNewMode &&
			(pResp->m_nSubmode & nNewSubmode) == nNewSubmode)
		{
			nRecvCount++;
			pResp->OnModeChange(this, pModeManager, pDocument, nOldMode, nOldSubmode, nNewMode, nNewSubmode);
		}
	}

	Debug("... %d recievers\n", nRecvCount++);
}

CWindowEventManager::CWindowEventManager()
{
	m_bAutoScroll = false;
	m_bTimerActive = false;
	m_pGlobalResponder = NULL;
	m_pResponderList = new CLinkedList<CBaseWindowEventResponder>();
}

CWindowEventManager::~CWindowEventManager()
{
	if(m_pResponderList != NULL)
		delete m_pResponderList;
	m_pResponderList = NULL;
}

void CWindowEventManager::OnTimer(CMapView* pView, CTread3DDoc* pDocument, int nCode)
{
	// Ignore if not ours.
	if(nCode != EM_TIMER_ID)
		return;

	// Don't redraw from scroll messages.
	pView->EnableRedraw(false);

#ifdef _WIN32
	// Move it.
	if(m_nMouseX < pView->m_rcClientRect.left)
		pView->SendMessage(WM_HSCROLL, SB_LINELEFT);
	if(m_nMouseX > pView->m_rcClientRect.right)
		pView->SendMessage(WM_HSCROLL, SB_LINERIGHT);
	if(m_nMouseY < pView->m_rcClientRect.top)
		pView->SendMessage(WM_VSCROLL, SB_LINELEFT);
	if(m_nMouseY > pView->m_rcClientRect.bottom)
		pView->SendMessage(WM_VSCROLL, SB_LINERIGHT);
#endif

	pView->EnableRedraw();

	// Invoke the mousemove.
	m_pAutoScrollResponder->OnMouseMove(NULL, this, pView, pDocument, m_nMouseX, m_nMouseY, m_nButtons);
}

void CWindowEventManager::EnableAutoScroll(CBaseWindowEventResponder* pResp, bool bAuto)
{
	m_bAutoScroll = bAuto;
	if(bAuto)
		m_pAutoScrollResponder = pResp;
	else
		m_pAutoScrollResponder = NULL;
}

void CWindowEventManager::AddEventResponder(CBaseWindowEventResponder* pResponder)
{
	m_pResponderList->AddItem(pResponder);
}

void CWindowEventManager::SetGlobalResponder(CBaseWindowEventResponder* pResponder)
{
	m_pGlobalResponder = pResponder;
}

void CWindowEventManager::OnMouseMove(CMapView* pView, CTread3DDoc* pDocument, int nMouseX, int nMouseY, int nButtons)
{
	int nMode, nSubmode;
	CBaseWindowEventResponder* pResp;

	m_nMouseX = nMouseX;
	m_nMouseY = nMouseY;
	m_nButtons = nButtons;

	// Are we in range?
	if(m_bAutoScroll)
	{

		if(m_nMouseX < pView->m_rcClientRect.left || m_nMouseX > pView->m_rcClientRect.right ||
			m_nMouseY < pView->m_rcClientRect.top || m_nMouseY > pView->m_rcClientRect.bottom)
		{
			if(!m_bTimerActive)
			{
				// Make a timer.
				pView->SetTimer(EM_TIMER_ID, SCROLL_TIMER_INTERVAL, NULL);
				m_bTimerActive = true;
			}

		}
		else
		{
			if(m_bTimerActive)
			{
				pView->KillTimer(EM_TIMER_ID);
				m_bTimerActive = false;
			}
		}
	}


	if(m_pGlobalResponder != NULL)
	{
		m_pGlobalResponder->OnMouseMove(NULL, this, pView, pDocument, nMouseX, nMouseY, nButtons);
		return;
	}

	nMode = pDocument->GetModePtr()->GetMode();
	nSubmode = pDocument->GetModePtr()->GetSubmode();

	// Found one?
	if(pView->GetHotSpotManager()->Track(this, pView, pDocument, nMouseX, nMouseY, nButtons))
		return;

	m_pResponderList->First();
	for(pResp = m_pResponderList->GetCurItem(); pResp != NULL; pResp = m_pResponderList->GetNextItem())
	{
		if((pResp->m_nMode & nMode) == nMode && (pResp->m_nSubmode & nSubmode) == nSubmode)
			pResp->OnMouseMove(NULL, this, pView, pDocument, nMouseX, nMouseY, nButtons);
	}

}

void CWindowEventManager::OnMouseDown(CMapView* pView, CTread3DDoc* pDocument, int nMouseX, int nMouseY, int nButtons)
{
	int nMode, nSubmode;
	CBaseWindowEventResponder* pResp;

	m_nMouseX = nMouseX;
	m_nMouseY = nMouseY;
	m_nButtons = nButtons;


	if(m_pGlobalResponder != NULL)
	{
		pView->SetCapture();
		m_pGlobalResponder->OnMouseDown(NULL, this, pView, pDocument, nMouseX, nMouseY, nButtons);
		return;
	}
	
	nMode = pDocument->GetModePtr()->GetMode();
	nSubmode = pDocument->GetModePtr()->GetSubmode();

	m_pResponderList->First();
	for(pResp = m_pResponderList->GetCurItem(); pResp != NULL; pResp = m_pResponderList->GetNextItem())
	{
		if((pResp->m_nMode & nMode) == nMode && (pResp->m_nSubmode & nSubmode) == nSubmode)
			pResp->OnMouseDown(NULL, this, pView, pDocument, nMouseX, nMouseY, nButtons);
	}

	if(m_bAutoScroll)
		pView->SetCapture();
}

void CWindowEventManager::OnMouseUp(CMapView* pView, CTread3DDoc* pDocument, int nMouseX, int nMouseY, int nButtons)
{
	int nMode, nSubmode;
	CBaseWindowEventResponder* pResp;

	m_nMouseX = nMouseX;
	m_nMouseY = nMouseY;
	m_nButtons = nButtons;

	if(m_bAutoScroll)
		ReleaseCapture();
	if(m_bTimerActive)
	{
		pView->KillTimer(EM_TIMER_ID);
		m_bTimerActive = false;
	}

	if(m_pGlobalResponder != NULL)
	{
		if(!m_bAutoScroll)
			ReleaseCapture();
		m_pGlobalResponder->OnMouseUp(NULL, this, pView, pDocument, nMouseX, nMouseY, nButtons);
		return;
	}

	nMode = pDocument->GetModePtr()->GetMode();
	nSubmode = pDocument->GetModePtr()->GetSubmode();

	m_pResponderList->First();
	for(pResp = m_pResponderList->GetCurItem(); pResp != NULL; pResp = m_pResponderList->GetNextItem())
	{
		if((pResp->m_nMode & nMode) == nMode && (pResp->m_nSubmode & nSubmode) == nSubmode)
		{
			pResp->OnMouseUp(NULL, this, pView, pDocument, nMouseX, nMouseY, nButtons);
			pDocument->SetModifiedFlag(true);
		}
	}
}

void CWindowEventManager::OnKeyDown(CMapView* pView, CTread3DDoc* pDocument, int nKeyCode)
{
	int nMode, nSubmode;
	CBaseWindowEventResponder* pResp;

	if(m_pGlobalResponder != NULL)
	{
		m_pGlobalResponder->OnKeyDown(NULL, this, pView, pDocument, nKeyCode);
		return;
	}

	nMode = pDocument->GetModePtr()->GetMode();
	nSubmode = pDocument->GetModePtr()->GetSubmode();

	m_pResponderList->First();
	for(pResp = m_pResponderList->GetCurItem(); pResp != NULL; pResp = m_pResponderList->GetNextItem())
	{
		if((pResp->m_nMode & nMode) == nMode && (pResp->m_nSubmode & nSubmode) == nSubmode)
			pResp->OnKeyDown(NULL, this, pView, pDocument, nKeyCode);
	}
}
