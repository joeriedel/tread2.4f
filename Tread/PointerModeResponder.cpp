// POINTERMODERESPONDER.CPP
// Copyright © 1999 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#include "stdafx.h"
#include "RenderAPI.h"
#include "Tread3DDoc.h"
#include "EventManager.h"
#include "HotSpotManager.h"
#include "MapObject.h"
#include "PointerModeResponder.h"
#include "Geometry.h"
#include "TreadSys.h"
#include "Brush.h"
#include "MapView.h"
#include "UndoAPI.h"
#include "GameAPI.h"
#include "Anchor.h"

CPointerModeResponder::CPointerModeResponder()
{
	m_nMode = MODE_POINTER;
	m_nSubmode = ALL_MODE_FLAGS;
}

void CPointerModeResponder::OnModeChange(CModeChangeEventManager* pEventManager, CMode* pModeManager, CTread3DDoc* pDocument, int nOldMode, int nOldSubmode, int nNewMode, int nNewSubmode)
{
	if(nNewMode&MODE_POINTER)
	{
		if(nOldMode&MODE_POINTER)
			return;
		pDocument->BuildSelectionBBox();
		pDocument->BuildHotSpots();
		pDocument->AddGlobalRenderFlags(RF_NO3DSELBOX);
		pDocument->RemoveGlobalRenderFlags(RF_NOSELBOX);
		pDocument->UpdateWindows();
	}
}

int CPointerModeResponder::OnAction(int nAction, void* pParam, CModeChangeEventManager* pEventManager, CMode* pModeManager, CTread3DDoc* pDocument)
{
	return ARF_DEFAULT;
}
