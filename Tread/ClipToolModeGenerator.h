// CLIPTOOLMODEGENERATOR.H
// Copyright © 1999 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#ifndef CLIPTOOLMODEGENERATOR_H
#define CLIPTOOLMODEGENERATOR_H

#include "HotSpotManager.h"

class CClipToolModeGenerator : public CBaseHotSpotModeGenerator
{
public:

	CClipToolModeGenerator();

	virtual void GenerateHotSpots(CHotSpotManager* pHotSpotManager, CTread3DDoc* pDocument, CMapView* pView);

};

#endif