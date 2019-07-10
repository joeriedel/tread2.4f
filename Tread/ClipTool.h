// CLIPTOOL.H
// Copyright © 1999 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#ifndef CLIPTOOL_H
#define CLIPTOOL_H

#include "MapObject.h"
#include "Mathlib.h"
#include "HotSpotManager.h"
#include "TExport.h"

// $sb "cliptool.h block1"

class _T_EXPORT CClipTool : public CBaseObject, public CBaseHotSpotGenerator, public CBaseHotSpotEventResponder
{
	friend class CClipToolModeWinResponder;
	friend class CGDIClipToolRenderer;
	friend class CGLClipToolRenderer;

private:

	bool m_bValid;
	vec3_t m_vPts[2];
	float m_fX, m_fY;
	CPlane m_Plane;
	int m_nViewType;
	CHotSpot* m_pDragSpot;
	CLinkedList<CBaseObject> m_SrcList;
	CLinkedList<CBaseObject> m_FrontList;
	CLinkedList<CBaseObject> m_BackList;

public:

	CClipTool();

	int GetObjectClass() { return OBJECT_CLASS_CLIPTOOL; }
	int GetObjectInterfaces() { return I_UI; }

	virtual CBaseObject* Clone() { return new CClipTool(*this); }

	// Notifies us that the selection changed.
	void SelectionChanged(CTread3DDoc* pDocument);
	void DoClip(CTread3DDoc* pDocument, bool bSplit=false);

	CHotSpot* GetDragSpot() { return m_pDragSpot; }

	// Make hotspots.
	void GenerateHotSpots(CHotSpotManager* pHotSpotManager, CMapView* pView);
	void OnHotSpotDrag(CWindowEventManager* pEventManager, CHotSpotManager* pHotSpotManager, CHotSpot* pHotSpot, CMapView* pView, CTread3DDoc* pDocument, int nMouseX, int nMouseY, int nButtons, int nFlags);
	void OnHotSpotMouseUp(CWindowEventManager* pEventManager, CHotSpotManager* pHotSpotManager, CHotSpot* pHotSpot, CMapView* pView, CTread3DDoc* pDocument, int nMouseX, int nMouseY, int nButtons, int nFlags);
	void OnHotSpotMouseDown(CWindowEventManager* pEventManager, CHotSpotManager* pHotSpotManager, CHotSpot* pHotSpot, CMapView* pView, CTread3DDoc* pDocument, int nMouseX, int nMouseY, int nButtons, int nFlags);
};

// $eb

#endif