// CLIPTOOLMODERESPONDER.CPP
// Copyright © 1999 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#include "stdafx.h"
#include "ClipToolModeResponder.h"
#include "Tread3DDoc.h"
#include "TreadSys.h"
#include "ClipTool.h"
#include "Geometry.h"
#include "RenderAPI.h"

CClipToolModeResponder::CClipToolModeResponder()
{
	m_nMode = MODE_CLIP;
	m_nSubmode = ALL_MODES;
}

int CClipToolModeResponder::OnAction(int nAction, void* pParam, CModeChangeEventManager* pEventManager, CMode* pModeManager, CTread3DDoc* pDocument)
{
	CClipTool* pClipTool = (CClipTool*)G_FindObject(pDocument->GetUIObjectList(), OBJECT_CLASS_CLIPTOOL);

	switch(nAction)
	{
	case EF_ACTION_PAINT:

		pDocument->SetSelectedObjectsTexture(GetTexAPI()->GetCurTextureName());
		pDocument->UpdateWindows();
		break;

	case EF_ACTION_CLIP:
		pClipTool->DoClip(pDocument, pDocument->GetModePtr()->GetSubmode() == CLIP_SPLIT);
		if(pDocument->GetModePtr()->GetSubmode() == CLIP_SPLIT)
			pDocument->ClearSelection();
		pDocument->BuildHotSpots();
		pDocument->UpdateWindows();
		break;
	}	

	return ARF_HANDLED;
}

void CClipToolModeResponder::OnModeChange(CModeChangeEventManager* pEventManager, CMode* pModeManager, CTread3DDoc* pDocument, int nOldMode, int nOldSubmode, int nNewMode, int nNewSubmode)
{
	if(nNewMode&MODE_CLIP)
	{
		// Make a new clip tool object.
		CClipTool* pClipTool = new CClipTool();

		pDocument->AddObjectToMap(pClipTool);
		pDocument->AddGlobalRenderFlags(RF_NO3DSELBOX);
		pDocument->RemoveDrawFilters(OBJECT_CLASS_CLIPTOOL);
		pClipTool->SelectionChanged(pDocument);
		pDocument->BuildHotSpots();
		pDocument->UpdateWindows();
	}
	else
	{
		// Find the cliptool and delete it.
		CClipTool* pClipTool = (CClipTool*)G_FindObject(pDocument->GetUIObjectList(), OBJECT_CLASS_CLIPTOOL);
		if(pClipTool != NULL)
			pDocument->GetUIObjectList()->DeleteItem(pClipTool);
		pDocument->AddDrawFilters(OBJECT_CLASS_CLIPTOOL);
		//pDocument->RemoveGlobalRenderFlags(RF_NOSELBOX|RF_NOSELBOXSIZE);
	}
}