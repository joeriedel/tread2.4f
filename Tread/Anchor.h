// ANCHOR.H
// Copyright © 1999 Joe Riedel, Nick Randal.
// Author: Joe Riedel
#ifndef ANCHOR_H
#define ANCHOR_H

#include "MathLib.h"
#include "MapObject.h"
#include "HotSpotManager.h"

// $sb "anchor.h block1"

class CMapView;
class CAnchor : public CBaseObject, public CBaseHotSpotGenerator, public CBaseHotSpotEventResponder
{
private:

	vec3_t m_vOrigin;
	CString m_sName;

public:

	CAnchor();

	CString GetName() { return m_sName; }
	void SetName(CString sName) { m_sName = sName; }

	vec_t* GetOrigin() { return m_vOrigin; }

	// Make hotspots.
	virtual void GenerateHotSpots(CHotSpotManager* pHotSpotManager, CMapView* pView);
	virtual void OnHotSpotDrag(CWindowEventManager* pEventManager, CHotSpotManager* pHotSpotManager, CHotSpot* pHotSpot, CMapView* pView, CTread3DDoc* pDocument, int nMouseX, int nMouseY, int nButtons, int nFlags);
	virtual void OnHotSpotMouseUp(CWindowEventManager* pEventManager, CHotSpotManager* pHotSpotManager, CHotSpot* pHotSpot, CMapView* pView, CTread3DDoc* pDocument, int nMouseX, int nMouseY, int nButtons, int nFlags);
	virtual void OnHotSpotMouseDown(CWindowEventManager* pEventManager, CHotSpotManager* pHotSpotManager, CHotSpot* pHotSpot, CMapView* pView, CTread3DDoc* pDocument, int nMouseX, int nMouseY, int nButtons, int nFlags);
	virtual void OnHotSpotMouseEnter(CWindowEventManager* pEventManager, CHotSpotManager* pHotSpotManager, CHotSpot* pHotSpot, CMapView* pView, CTread3DDoc* pDocument, int nMouseX, int nMouseY, int nButtons, int nFlags);
	virtual void OnHotSpotMouseLeave(CWindowEventManager* pEventManager, CHotSpotManager* pHotSpotManager, CHotSpot* pHotSpot, CMapView* pView, CTread3DDoc* pDocument, int nMouseX, int nMouseY, int nButtons, int nFlags);

	virtual bool ReadObject(CFile& hFile, int nVersion);
	virtual bool WriteObject(CFile& hFile, int nVersion);

	virtual CBaseObject* Clone() { return new CAnchor(*this); }

	// Get's the object class.
	virtual int GetObjectClass() { return OBJECT_CLASS_ANCHOR; }

	// If an higher object wants to be read/written it should
	// extend this to return the I_STORABLE interface.
	virtual int GetObjectInterfaces() { return I_BASE|I_UI|I_STORABLE; }

	virtual int InitAfterLoad(CTread3DDoc* pDocument);

	friend class CGDIAnchorRenderer;
	friend class CGLAnchorRenderer;
	friend class CPointerModeWinResponder;
	friend class CCameraModeWinResponder;
};
// $eb
#endif