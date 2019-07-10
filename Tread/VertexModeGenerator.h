// VERTEXMODEGENERATOR.H
// Copyright © 1999 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#ifndef VERTEXMODEGENERATOR_H
#define VERTEXMODEGENERATOR_H

#include "HotSpotManager.h"

class CVertexModeGenerator : public CBaseHotSpotModeGenerator
{
private:

	void GenerateListHotSpots(CLinkedList<CBaseObject>* pList, CHotSpotManager* pHotSpotManager, CTread3DDoc* pDocument, CMapView* pView);

public:

	CVertexModeGenerator();

	virtual void GenerateHotSpots(CHotSpotManager* pHotSpotManager, CTread3DDoc* pDocument, CMapView* pView);

};

#endif