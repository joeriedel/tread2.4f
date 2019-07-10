// BRUSHMODERESPONDER.CPP
// Copyright © 1999 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#include "stdafx.h"
#include "RenderAPI.h"
#include "Tread3DDoc.h"
#include "EventManager.h"
#include "HotSpotManager.h"
#include "MapObject.h"
#include "BrushModeResponder.h"

#ifdef _WIN32
#include "ViewControl.h"
#endif

CBrushModeResponder::CBrushModeResponder()
{
	m_nMode = MODE_BRUSH;
	m_nSubmode = ALL_MODE_FLAGS;
}

void CBrushModeResponder::OnModeChange(CModeChangeEventManager* pEventManager, CMode* pModeManager, CTread3DDoc* pDocument, int nOldMode, int nOldSubmode, int nNewMode, int nNewSubmode)
{
	if(nNewMode & MODE_BRUSH)
	{
		// This will force the HS managers of the views
		// to delete the hotspots left over from an old mode
		// like camera mode.
		pDocument->BuildHotSpots();

#ifdef _WIN32
		CObjectView* pView = (CObjectView*)GetView(OBJECT_VIEW);
		if(pView != NULL)
			pView->LoadBrushList();
#endif

		pDocument->SetWindowCursors(TC_IDC_CROSS);
		pDocument->AddGlobalRenderFlags(RF_FAST|RF_NOSELBOX);
		pDocument->RemoveGlobalRenderFlags(RF_NO3DSELBOX|RF_FORCESELBOX);
		pDocument->UpdateWindows();
	}
	else
	{
		pDocument->SetWindowCursors(TC_DEFAULT);
		pDocument->RemoveGlobalRenderFlags(RF_FAST|RF_SELBOXONTOP|RF_FORCESELBOX|RF_NOSELBOX);
	}
}