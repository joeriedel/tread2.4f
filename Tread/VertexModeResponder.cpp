// VERTEXMODERESPONDER.CPP
// Copyright © 1999 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#include "stdafx.h"
#include "RenderAPI.h"
#include "Tread3DDoc.h"
#include "EventManager.h"
#include "HotSpotManager.h"
#include "MapObject.h"
#include "VertexModeResponder.h"

CVertexModeResponder::CVertexModeResponder()
{
	m_nMode = MODE_VERTEX;
	m_nSubmode = ALL_MODE_FLAGS;
}

void CVertexModeResponder::OnModeChange(CModeChangeEventManager* pEventManager, CMode* pModeManager, CTread3DDoc* pDocument, int nOldMode, int nOldSubmode, int nNewMode, int nNewSubmode)
{
	if(nNewMode & MODE_VERTEX)
	{
		pDocument->BuildHotSpots();
		pDocument->AddGlobalRenderFlags(RF_NOSELBOX|RF_NOSELBOXSIZE);
		pDocument->UpdateWindows(FLAT_VIEWS);
	}
	else
		pDocument->RemoveGlobalRenderFlags(RF_NOSELBOX|RF_NOSELBOXSIZE);
}

int CVertexModeResponder::OnAction(int nAction, void* pParam, CModeChangeEventManager* pEventManager, CMode* pModeManager, CTread3DDoc* pDocument)
{
	// Select based on the action invoked.
	switch( nAction )
	{
	case EF_ACTION_PAINT:

		pDocument->SetSelectedObjectsTexture(GetTexAPI()->GetCurTextureName());
		pDocument->UpdateWindows();
		break;

	case EF_ACTION_UNDO:

		pDocument->GetUndoAPI()->Undo(pDocument);
		//pDocument->AdjustViews();		
		pDocument->UpdateWindows();
		break;

	case EF_ACTION_REDO:

		pDocument->GetUndoAPI()->Redo(pDocument);
		//pDocument->AdjustViews();
		pDocument->UpdateWindows();
		break;
	}

	return ARF_HANDLED;
}
