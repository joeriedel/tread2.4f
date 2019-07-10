// CAMERAMODEGENERATOR.H
// Copyright © 1999 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#ifndef CAMERAMODEGENERATOR_H
#define CAMERAMODEGENERATOR_H

#include "HotSpotManager.h"

class CCameraModeGenerator : public CBaseHotSpotModeGenerator
{
public:

	CCameraModeGenerator();

	virtual void GenerateHotSpots(CHotSpotManager* pHotSpotManager, CTread3DDoc* pDocument, CMapView* pView);

};

#endif