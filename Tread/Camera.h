// CAMERA.H
// Copyright © 1999 Joe Riedel, Nick Randal.
// Author: Joe Riedel
#ifndef CAMERA_H
#define CAMERA_H

#include "MathLib.h"
#include "MapObject.h"
#include "HotSpotManager.h"
#include "TExport.h"

// $sb "camera.h block1"

#define CF_ORIGIN	0x00000001
#define CF_LOOK		0x00000002

class CMapView;
class _T_EXPORT CCamera : public CBaseObject, public CBaseHotSpotGenerator, public CBaseHotSpotEventResponder
{
private:

	int m_nView;
	CMapView* m_pView;
	vec3_t m_vOrigin, m_vLook;
	CCamera* m_pOldCam;

public:

	CCamera();

	vec_t* GetOrigin() { return m_vOrigin; }
	vec_t* GetLook() { return m_vLook; }
	CMapView* GetView() { return m_pView; }
	void SetView(CMapView* pView, CTread3DDoc* pDocument);

	// Make hotspots.
	virtual void GenerateHotSpots(CHotSpotManager* pHotSpotManager, CMapView* pView);
	virtual void OnHotSpotDrag(CWindowEventManager* pEventManager, CHotSpotManager* pHotSpotManager, CHotSpot* pHotSpot, CMapView* pView, CTread3DDoc* pDocument, int nMouseX, int nMouseY, int nButtons, int nFlags);
	virtual void OnHotSpotMouseUp(CWindowEventManager* pEventManager, CHotSpotManager* pHotSpotManager, CHotSpot* pHotSpot, CMapView* pView, CTread3DDoc* pDocument, int nMouseX, int nMouseY, int nButtons, int nFlags);
	virtual void OnHotSpotMouseDown(CWindowEventManager* pEventManager, CHotSpotManager* pHotSpotManager, CHotSpot* pHotSpot, CMapView* pView, CTread3DDoc* pDocument, int nMouseX, int nMouseY, int nButtons, int nFlags);
	virtual void OnHotSpotMouseEnter(CWindowEventManager* pEventManager, CHotSpotManager* pHotSpotManager, CHotSpot* pHotSpot, CMapView* pView, CTread3DDoc* pDocument, int nMouseX, int nMouseY, int nButtons, int nFlags);
	virtual void OnHotSpotMouseLeave(CWindowEventManager* pEventManager, CHotSpotManager* pHotSpotManager, CHotSpot* pHotSpot, CMapView* pView, CTread3DDoc* pDocument, int nMouseX, int nMouseY, int nButtons, int nFlags);

	virtual bool ReadObject(CFile& hFile, int nVersion);
	virtual bool WriteObject(CFile& hFile, int nVersion);

	// Get's the object class.
	virtual int GetObjectClass() { return OBJECT_CLASS_CAMERA; }

	virtual CBaseObject* Clone() { return new CCamera(*this); }

	// If an higher object wants to be read/written it should
	// extend this to return the I_STORABLE interface.
	virtual int GetObjectInterfaces() { return I_BASE|I_UI|I_STORABLE; }

	virtual int InitAfterLoad(CTread3DDoc* pDocument);

	friend class CGDICameraRenderer;
	friend class CGLCameraRenderer;
	friend class CTreadMap1X;
	friend class CCameraModeWinResponder;
};

// $eb

#endif