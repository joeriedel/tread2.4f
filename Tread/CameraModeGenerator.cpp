// CAMERAMODEGENERATOR.CPP
// Copyright © 1999 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#include "stdafx.h"
#include "MapObject.h"
#include "HotSpotManager.h"
#include "CameraModeGenerator.h"
#include "MapView.h"
#include "Tread3DDoc.h"
#include "TreadSys.h"

CCameraModeGenerator::CCameraModeGenerator()
{
	m_nMode = MODE_CAMERA;
	m_nSubmode = ALL_MODES;
}

void CCameraModeGenerator::GenerateHotSpots(CHotSpotManager* pHotSpotManager, CTread3DDoc* pDocument, CMapView* pView)
{
	CBaseObject* pObject;
	CCamera* pCam;

	// Have all camera generator their hotspots.
	pDocument->GetUIObjectList()->First();
	for(pObject = pDocument->GetUIObjectList()->GetCurItem(); pObject != NULL; pObject = pDocument->GetUIObjectList()->GetNextItem())
	{
		if(pObject->GetObjectClass() != OBJECT_CLASS_CAMERA)
			continue;

		pCam = (CCamera*)pObject;
		pCam->GenerateHotSpots(pHotSpotManager, pView);
	}
}