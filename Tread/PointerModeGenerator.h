// POINTERMODEGENERATOR.H
// Copyright © 1999 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#ifndef POINTERMODEGENERATOR_H
#define POINTERMODEGENERATOR_H

#include "HotSpotManager.h"

class CPointerModeGenerator : public CBaseHotSpotModeGenerator, public CBaseHotSpotEventResponder
{
private:

	bool m_bSnap;
	bool m_bMoved;
	float m_fX, m_fY;
	int m_nX, m_nY;
	vec3_t m_vSize;
	vec3_t m_vOrigin;
	vec3_t m_vTotalRot;
	vec3_t m_vMoveTotal;

	void BuildBrushHandles(CLinkedList<CBaseObject>* pList, CMapView* pView, CHotSpotManager* pHotSpotManager, bool bSel);

public:

	CPointerModeGenerator();

	virtual void GenerateHotSpots(CHotSpotManager* pHotSpotManager, CTread3DDoc* pDocument, CMapView* pView);
	virtual void OnHotSpotDragRotate(CWindowEventManager* pEventManager, CHotSpotManager* pHotSpotManager, CHotSpot* pHotSpot, CMapView* pView, CTread3DDoc* pDocument, int nMouseX, int nMouseY, int nButtons);
	virtual void OnHotSpotDragNormal(CWindowEventManager* pEventManager, CHotSpotManager* pHotSpotManager, CHotSpot* pHotSpot, CMapView* pView, CTread3DDoc* pDocument, int nMouseX, int nMouseY, int nButtons);
	virtual void OnHotSpotDragObject(CWindowEventManager* pEventManager, CHotSpotManager* pHotSpotManager, CHotSpot* pHotSpot, CMapView* pView, CTread3DDoc* pDocument, int nMouseX, int nMouseY, int nButtons);

	// HotSpotResponder
	virtual void OnHotSpotDrag(CWindowEventManager* pEventManager, CHotSpotManager* pHotSpotManager, CHotSpot* pHotSpot, CMapView* pView, CTread3DDoc* pDocument, int nMouseX, int nMouseY, int nButtons, int nFlags);
	virtual void OnHotSpotMouseUp(CWindowEventManager* pEventManager, CHotSpotManager* pHotSpotManager, CHotSpot* pHotSpot, CMapView* pView, CTread3DDoc* pDocument, int nMouseX, int nMouseY, int nButtons, int nFlags);
	virtual void OnHotSpotMouseDown(CWindowEventManager* pEventManager, CHotSpotManager* pHotSpotManager, CHotSpot* pHotSpot, CMapView* pView, CTread3DDoc* pDocument, int nMouseX, int nMouseY, int nButtons, int nFlags);
	virtual void OnHotSpotMouseEnter(CWindowEventManager* pEventManager, CHotSpotManager* pHotSpotManager, CHotSpot* pHotSpot, CMapView* pView, CTread3DDoc* pDocument, int nMouseX, int nMouseY, int nButtons, int nFlags);
	virtual void OnHotSpotMouseLeave(CWindowEventManager* pEventManager, CHotSpotManager* pHotSpotManager, CHotSpot* pHotSpot, CMapView* pView, CTread3DDoc* pDocument, int nMouseX, int nMouseY, int nButtons, int nFlags);

};

#endif