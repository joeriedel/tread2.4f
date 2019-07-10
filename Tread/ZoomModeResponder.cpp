// ZOOMMODERESPONDER.CPP
// Copyright © 1999 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#include "stdafx.h"
#include "EventManager.h"
#include "TreadSys.h"
#include "Tread3DDoc.h"
#include "RenderAPI.h"
#include "ZoomModeResponder.h"

CZoomModeResponder::CZoomModeResponder()
{
	m_nMode = MODE_ZOOM;
	m_nSubmode = ALL_MODES;
}

void CZoomModeResponder::OnModeChange(CModeChangeEventManager* pEventManager, CMode* pModeManager, CTread3DDoc* pDocument, int nOldMode, int nOldSubmode, int nNewMode, int nNewSubmode)
{
	if(nNewMode&MODE_ZOOM)
	{
		pDocument->SetWindowCursors(TC_IDC_ZOOM);
		pDocument->BuildHotSpots();	// Delete.
		pDocument->AddGlobalRenderFlags(RF_FAST);
	}
	else
	{
		pDocument->SetWindowCursors(TC_DEFAULT);
		pDocument->RemoveGlobalRenderFlags(RF_FAST);
	}

	pDocument->UpdateWindows(FLAT_VIEWS);
}

