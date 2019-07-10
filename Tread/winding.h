// WINDING.H
// Copyright © 1999 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#ifndef WINDING_H
#define WINDING_H

#include "MathLib.h"
#include "MapObject.h"
#include "HotSpotManager.h"
#include "TExport.h"

// $sb "winding.h block1"

#define WINDING_STORABLE_OBJECT_ID	0xE0000000
#define MAX_WINDING_POINTS			256

#define PSIDE_FRONT	1
#define PSIDE_BACK	2
#define PSIDE_ON	4
#define PSIDE_BOTH	8

class _T_EXPORT CWinding :    public CBaseGroupableObject, 
					public CBaseTransformableObject,
					public CBaseHotSpotEventResponder,
					public CBaseHotSpotGenerator

{

public:

	friend class CGDIWindingRenderer;

protected:

	CPlane m_Plane;
	int		m_nPlaneType;
	int		m_nNumPoints;
	vec3_t*	m_vPoints;
	float m_fX, m_fY;
	
	static vec_t m_sfDX;
	static vec_t m_sfDY;

	void ComputePlane();
	void FreePoints();

public:

	CWinding();
	CWinding(int nNumPoints);
	CWinding(int nNumPoints, vec3_t* vPoints);
	CWinding(const CWinding& WindingToCopy);
	virtual ~CWinding();

	// CWinding functions.
	CPlane* GetPlanePtr() { return &m_Plane; }
	// Makes a copy of the points array passed.
	virtual void CopyPoints(int nNumPoints, vec3_t* vPoints);
	virtual void SplitWindingByPlane(CPlane* pPlane, CWinding** pFront, CWinding** pBack);
	virtual CWinding* ChopWindingByPlane(CPlane* pPlane, bool bReturnFront = true);
	virtual void Flip();

	int GetNumPoints() const { return m_nNumPoints; }
	vec3_t* GetPoints() const { return m_vPoints; }

	bool WindingCrossesPlane(CPlane* pPlane);
	int  ClassifyWinding(CPlane* pPlane);
	int  ClassifyWinding(CPlane* pPlane, float fEpsilon);
	int  BiasClassifyWinding(CPlane* pPlane, float fFront);
	bool CheckWinding();
	static CWinding* BaseWindingFromPlane(CPlane* pPlane);

	virtual CBaseObject* Clone() { return new CWinding(*this); }

	// CBaseObject
	virtual int GetObjectClass() { return OBJECT_CLASS_WINDING; }
	virtual int GetObjectInterfaces() { return I_GROUPABLE | I_STORABLE | I_TRANSFORMABLE | I_CHECKABLE; }
	virtual void* GetInterface(int nInterface);

	// CBaseStorableObject.
	virtual bool ReadObject(CFile& hFile, int nVersion);
	virtual bool WriteObject(CFile& hFile, int nVersion);

	// CBaseTransformableObject
	virtual void Scale(vec3_t vScale);
	virtual void Scale(vec3_t vScale, vec3_t vOrigin);
	virtual void SetOrigin(vec3_t vOrigin);
	virtual void Translate(vec3_t vTranslation);
	virtual void Rotate(vec3_t vRot);
	virtual void Rotate(vec3_t vOrigin, vec3_t vRot);
	virtual void InsertIntoRange(vec3_t vMins, vec3_t vMaxs);
	virtual void ComputeOrigin();
	virtual void Reflect(int nAxis, vec3_t vOrigin);
	virtual bool In2DRange(vec3_t vMins, vec3_t vMaxs, int nView);

	// Hotspot.
	virtual void GenerateHotSpots(CHotSpotManager* pHotSpotManager, CMapView* pView);
	virtual void OnHotSpotDrag(CWindowEventManager* pEventManager, CHotSpotManager* pHotSpotManager, CHotSpot* pHotSpot, CMapView* pView, CTread3DDoc* pDocument, int nMouseX, int nMouseY, int nButtons, int nFlags);
	virtual void OnHotSpotMouseDown(CWindowEventManager* pEventManager, CHotSpotManager* pHotSpotManager, CHotSpot* pHotSpot, CMapView* pView, CTread3DDoc* pDocument, int nMouseX, int nMouseY, int nButtons, int nFlags);
	virtual void OnHotSpotMouseUp(CWindowEventManager* pEventManager, CHotSpotManager* pHotSpotManager, CHotSpot* pHotSpot, CMapView* pView, CTread3DDoc* pDocument, int nMouseX, int nMouseY, int nButtons, int nFlags);
};

// $eb

#endif