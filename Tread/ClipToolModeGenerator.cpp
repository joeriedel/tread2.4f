// CLIPTOOLMODEGENERATOR.CPP
// Copyright © 1999 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#include "stdafx.h"
#include "MapObject.h"
#include "HotSpotManager.h"
#include "ClipToolModeGenerator.h"
#include "MapView.h"
#include "Tread3DDoc.h"
#include "TreadSys.h"
#include "ClipTool.h"
#include "Geometry.h"

CClipToolModeGenerator::CClipToolModeGenerator()
{
	m_nMode = MODE_CLIP;
	m_nSubmode = ALL_MODES;
}

void CClipToolModeGenerator::GenerateHotSpots(CHotSpotManager* pHotSpotManager, CTread3DDoc* pDocument, CMapView* pView)
{
	CClipTool* pClipTool = (CClipTool*)G_FindObject(pDocument->GetUIObjectList(), OBJECT_CLASS_CLIPTOOL);
	if(pClipTool == NULL)
		return;

	pClipTool->GenerateHotSpots(pHotSpotManager, pView);
}