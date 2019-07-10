// BRUSH.H
// Copyright © 1999 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#ifndef BRUSH_H
#define BRUSH_H

#include "LinkedList.h"
#include "MapObject.h"
#include "Face.h"
#include "Texture.h"
#include "HotSpotManager.h"
#include "TExport.h"

// $sb "brush.h block1"
class _T_EXPORT CQBrushPlane : public CLLObject, public CVisgroupRefObject
{
public:

	CQBrushPlane()
	{
		m_bUseFlags = false;
	}

	CQBrushPlane(const CPlane& PlaneToCopy)
	{
		m_Plane.m_vDist = PlaneToCopy.m_vDist;
		VectorCopy(PlaneToCopy.m_vNormal, m_Plane.m_vNormal);
		m_vShifts[0] = 0.0F; m_vShifts[1] = 0.0F;
		m_vScales[0] = 0.0F; m_vScales[1] = 0.0F;
		m_fRotate = 0.0F;
		m_bUseFlags = false;
	}

	CPlane m_Plane;
	CString m_sTexture;
	vec2_t m_vShifts;
	vec2_t m_vScales;
	vec_t m_fRotate;

	bool m_bUseFlags;
	int m_nSurface, m_nContents, m_nValue;

};


// A Quake Style Brush.
class _T_EXPORT CQBrush : public CBaseGroupableObject, 
				public CBaseTransformableObject, 
				public CBaseTextureableObject,
				public CBaseCheckableObject,
				public CVisgroupRefObject,
				public CBaseHotSpotGenerator,
				public CBaseCarveableObject
{

public:

	friend class CGDIBrushRenderer;
	friend class CGL3DBrushRenderer;

public:

	CQBrush();
	CQBrush(const CQBrush& BrushToCopy);
	virtual ~CQBrush();
	
	CLinkedList<CFace>* m_pFaceList;

	// Make hotspots.
	virtual void GenerateHotSpots(CHotSpotManager* pHotSpotManager, CMapView* pView, CBaseHotSpotEventResponder* pResp=NULL, bool bHandles=false, bool bSel=false);

	// Brush functions.
	void SplitBrushByPlane(CPlane* pPlane, CQBrush** pFront, CQBrush** pBack);
	CObjectGroup* HollowBrush(float fDepth);
	int ClassifyBrush(CPlane* pPlane);
	bool CheckBrush();

	static CQBrush* CreateBrushFromPlaneList(CLinkedList<CQBrushPlane>& PlaneList);

	virtual int InitAfterLoad(CTread3DDoc* pDocument);

	// CBaseObject
	virtual int GetObjectClass() { return OBJECT_CLASS_BRUSH; }
	virtual int GetObjectInterfaces() { return I_GROUPABLE | I_STORABLE | I_TRANSFORMABLE 
												| I_TEXTUREABLE | I_CHECKABLE | I_VISGROUPREF | I_CARVE | I_HOLLOW | I_CARVEABLE | I_LINK; }
	virtual void* GetInterface(int nInterface);

	virtual CBaseObject* Clone() { return new CQBrush(*this); }

	// CBaseStorableObject
	virtual bool ReadObject(CFile& hFile, int nVersion);
	virtual bool WriteObject(CFile& hFile, int nVersion);

	// CBaseTransformableObject
	virtual void Scale(vec3_t vScale);	// Scales the objects size.
	virtual void Scale(vec3_t vScale, vec3_t vOrigin);
	virtual void Translate(vec3_t vTranslation);
	virtual void Rotate(vec3_t vRot);
	virtual void Rotate(vec3_t vOrigin, vec3_t vRot);
	virtual void InsertIntoRange(vec3_t vMins, vec3_t vMaxs);
	virtual void Reflect(int nAxis, vec3_t vFlipOrigin);
	virtual void ComputeOrigin();
	virtual void BuildRange();
	virtual bool In2DRange(vec3_t vMins, vec3_t vMaxs, int nView);

	// Carvable.
	virtual bool ObjectsIntersect(CBaseObject* pObject);
	virtual CObjectGroup* CSGSubtract(CBaseObject* pObject);
	virtual int GetCarveClasses() const { return OBJECT_CLASS_BRUSH|OBJECT_CLASS_GROUP; }

	// CBaseTextureableObject
	// Events.
	virtual void OnTextureChange();
	virtual void OnAttributeChange();
	virtual void RebuildTexture();
};
// $eb

#endif