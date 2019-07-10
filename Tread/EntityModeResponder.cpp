// VERTEXMODERESPONDER.CPP
// Copyright © 1999 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#include "stdafx.h"
#include "RenderAPI.h"
#include "Tread3DDoc.h"
#include "EventManager.h"
#include "HotSpotManager.h"
#include "MapObject.h"
#include "EntityModeResponder.h"

#ifdef _WIN32
#include "ViewControl.h"
#endif

CEntityModeResponder::CEntityModeResponder()
{
	m_nMode = MODE_ENTITY;
	m_nSubmode = ALL_MODE_FLAGS;
}

void CEntityModeResponder::OnModeChange(CModeChangeEventManager* pEventManager, CMode* pModeManager, CTread3DDoc* pDocument, int nOldMode, int nOldSubmode, int nNewMode, int nNewSubmode)
{
	if(nNewMode & MODE_ENTITY)
	{
		// This will force the HS managers of the views
		// to delete the hotspots left over from an old mode
		// like camera mode.
		pDocument->BuildHotSpots();

#ifdef _WIN32
		CObjectView* pView = (CObjectView*)GetView(OBJECT_VIEW);
		if(pView != NULL)
			pView->LoadEntityList();
#endif
		
		// All drawing in this mode cannot be detected by clicks.
		pDocument->AddGlobalRenderFlags(RF_FAST);
		pDocument->UpdateWindows(FLAT_VIEWS);
	}
	else
		pDocument->RemoveGlobalRenderFlags(RF_FAST);
}