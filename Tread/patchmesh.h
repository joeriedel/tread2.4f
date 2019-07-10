// PATCHMESH.H
// Copyright © 2000 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#ifndef PATCHMESH_H
#define PATCHMESH_H

#include "linkedlist.h"
#include "mathlib.h"
#include "mapobject.h"
#include "face.h"

// $sb "patchmesh block1"

#define MAKE_PATCH_SIZE_CODE(w, h) (w+(h<<8))
#define GET_PATCH_WIDTH(c) (c&0xFF)
#define GET_PATCH_HEIGHT(c) ((c&0xFF00)>>8)

#define PATCH_SIZE_CODE_3x3		MAKE_PATCH_SIZE_CODE(3, 3)
#define PATCH_SIZE_CODE_3x5		MAKE_PATCH_SIZE_CODE(3, 5)
#define PATCH_SIZE_CODE_3x7		MAKE_PATCH_SIZE_CODE(3, 7)
#define PATCH_SIZE_CODE_3x9		MAKE_PATCH_SIZE_CODE(3, 9)
#define PATCH_SIZE_CODE_3x11	MAKE_PATCH_SIZE_CODE(3, 11)
#define PATCH_SIZE_CODE_3x13	MAKE_PATCH_SIZE_CODE(3, 13)
#define PATCH_SIZE_CODE_3x15	MAKE_PATCH_SIZE_CODE(3, 15)

#define PATCH_SIZE_CODE_5x3		MAKE_PATCH_SIZE_CODE(5, 3)
#define PATCH_SIZE_CODE_5x5		MAKE_PATCH_SIZE_CODE(5, 5)
#define PATCH_SIZE_CODE_5x7		MAKE_PATCH_SIZE_CODE(5, 7)
#define PATCH_SIZE_CODE_5x9		MAKE_PATCH_SIZE_CODE(5, 9)
#define PATCH_SIZE_CODE_5x11	MAKE_PATCH_SIZE_CODE(5, 11)
#define PATCH_SIZE_CODE_5x13	MAKE_PATCH_SIZE_CODE(5, 13)
#define PATCH_SIZE_CODE_5x15	MAKE_PATCH_SIZE_CODE(5, 15)

#define PATCH_SIZE_CODE_7x3		MAKE_PATCH_SIZE_CODE(7, 3)
#define PATCH_SIZE_CODE_7x5		MAKE_PATCH_SIZE_CODE(7, 5)
#define PATCH_SIZE_CODE_7x7		MAKE_PATCH_SIZE_CODE(7, 7)
#define PATCH_SIZE_CODE_7x9		MAKE_PATCH_SIZE_CODE(7, 9)
#define PATCH_SIZE_CODE_7x11	MAKE_PATCH_SIZE_CODE(7, 11)
#define PATCH_SIZE_CODE_7x13	MAKE_PATCH_SIZE_CODE(7, 13)
#define PATCH_SIZE_CODE_7x15	MAKE_PATCH_SIZE_CODE(7, 15)

#define PATCH_SIZE_CODE_9x3		MAKE_PATCH_SIZE_CODE(9, 3)
#define PATCH_SIZE_CODE_9x5		MAKE_PATCH_SIZE_CODE(9, 5)
#define PATCH_SIZE_CODE_9x7		MAKE_PATCH_SIZE_CODE(9, 7)
#define PATCH_SIZE_CODE_9x9		MAKE_PATCH_SIZE_CODE(9, 9)
#define PATCH_SIZE_CODE_9x11	MAKE_PATCH_SIZE_CODE(9, 11)
#define PATCH_SIZE_CODE_9x13	MAKE_PATCH_SIZE_CODE(9, 13)
#define PATCH_SIZE_CODE_9x15	MAKE_PATCH_SIZE_CODE(9, 15)

#define PATCH_SIZE_CODE_11x3	MAKE_PATCH_SIZE_CODE(11, 3)
#define PATCH_SIZE_CODE_11x5	MAKE_PATCH_SIZE_CODE(11, 5)
#define PATCH_SIZE_CODE_11x7	MAKE_PATCH_SIZE_CODE(11, 7)
#define PATCH_SIZE_CODE_11x9	MAKE_PATCH_SIZE_CODE(11, 9)
#define PATCH_SIZE_CODE_11x11	MAKE_PATCH_SIZE_CODE(11, 11)
#define PATCH_SIZE_CODE_11x13	MAKE_PATCH_SIZE_CODE(11, 13)
#define PATCH_SIZE_CODE_11x15	MAKE_PATCH_SIZE_CODE(11, 15)

#define PATCH_SIZE_CODE_13x3	MAKE_PATCH_SIZE_CODE(13, 3)
#define PATCH_SIZE_CODE_13x5	MAKE_PATCH_SIZE_CODE(13, 5)
#define PATCH_SIZE_CODE_13x7	MAKE_PATCH_SIZE_CODE(13, 7)
#define PATCH_SIZE_CODE_13x9	MAKE_PATCH_SIZE_CODE(13, 9)
#define PATCH_SIZE_CODE_13x11	MAKE_PATCH_SIZE_CODE(13, 11)
#define PATCH_SIZE_CODE_13x13	MAKE_PATCH_SIZE_CODE(13, 13)
#define PATCH_SIZE_CODE_13x15	MAKE_PATCH_SIZE_CODE(13, 15)

#define PATCH_SIZE_CODE_15x3	MAKE_PATCH_SIZE_CODE(15, 3)
#define PATCH_SIZE_CODE_15x5	MAKE_PATCH_SIZE_CODE(15, 5)
#define PATCH_SIZE_CODE_15x7	MAKE_PATCH_SIZE_CODE(15, 7)
#define PATCH_SIZE_CODE_15x9	MAKE_PATCH_SIZE_CODE(15, 9)
#define PATCH_SIZE_CODE_15x11	MAKE_PATCH_SIZE_CODE(15, 11)
#define PATCH_SIZE_CODE_15x13	MAKE_PATCH_SIZE_CODE(15, 13)
#define PATCH_SIZE_CODE_15x15	MAKE_PATCH_SIZE_CODE(15, 15)

//
// Quake3 uses quadric curves, not the original cubic ones i coded over a year ago!
//
class CPatchMesh : public CBaseGroupableObject,
					 public CBaseTransformableObject,
					 public CBaseTextureableObject,
					 public CVisgroupRefObject,
					 public CBaseHotSpotEventResponder,
					 public CBaseHotSpotGenerator
{
protected:

	static m_snDefaultPatchSize;

#define MAX_MESH_POINTS	15

	int m_nWidth, m_nHeight;
	int m_nResolution;
	bool m_bNeedRebuild;
	CLinkedList<CFace> m_FaceList;
	vec_t m_vCtrls[MAX_MESH_POINTS][MAX_MESH_POINTS][5]; // X, Y, Z, S, T

	void SamplePatch(int nUOff, int nVOff, vec_t fU, vec_t fV, vec_t vOut[5]);
	CFace* FaceFromSamples(vec_t vCtrls[4][5]);

public:

	static void SetDefaultPatchSize(int nSize);
	static int  GetDefaultPatchSize();

	CPatchMesh();
	CPatchMesh(const CPatchMesh& ToCopy);
	virtual ~CPatchMesh();

	// CBaseObject
	virtual int GetObjectClass() { return OBJECT_CLASS_PATCHMESH; }
	virtual int GetObjectInterfaces() { return I_GROUPABLE | I_VISGROUPREF | I_TEXTUREABLE | I_TRANSFORMABLE | I_STORABLE; }
	virtual void* GetInterface(int nInterface);

	virtual void OnTextureChange();

	// Storable Object.
	virtual bool ReadObject(CFile& hFile, int nVersion);
	virtual bool WriteObject(CFile& hFile, int nVersion);

	virtual CBaseObject* Clone() { return new CPatchMesh(*this); }

	virtual int InitAfterLoad(CTread3DDoc* pDocument);
	virtual void InitFromSelection(CTread3DDoc* pDocument, CMapView* pView, vec3_t vMins, vec3_t vMaxs, vec3_t vOrigin);

	// BaseTransformable.
	virtual void Scale(vec3_t vScale);	// Scales the objects size.
	virtual void Scale(vec3_t vScale, vec3_t vOrigin);
	virtual void Translate(vec3_t vTranslation);
	virtual void Rotate(vec3_t vRot);
	virtual void Rotate(vec3_t vOrigin, vec3_t vRot);
	virtual void InsertIntoRange(vec3_t vMins, vec3_t vMaxs);
	virtual bool In2DRange(vec3_t vMins, vec3_t vMaxs, int nView);
	virtual void Reflect(int nAxis, vec3_t vFlipOrigin);
	virtual void ComputeOrigin();
	virtual void BuildRange();

	// Hotspot.
	virtual void GenerateHotSpots(CHotSpotManager* pHotSpotManager, CMapView* pView);
	virtual void OnHotSpotDrag(CWindowEventManager* pEventManager, CHotSpotManager* pHotSpotManager, CHotSpot* pHotSpot, CMapView* pView, CTread3DDoc* pDocument, int nMouseX, int nMouseY, int nButtons, int nFlags);
	virtual void OnHotSpotMouseUp(CWindowEventManager* pEventManager, CHotSpotManager* pHotSpotManager, CHotSpot* pHotSpot, CMapView* pView, CTread3DDoc* pDocument, int nMouseX, int nMouseY, int nButtons, int nFlags);
	virtual void OnHotSpotMouseDown(CWindowEventManager* pEventManager, CHotSpotManager* pHotSpotManager, CHotSpot* pHotSpot, CMapView* pView, CTread3DDoc* pDocument, int nMouseX, int nMouseY, int nButtons, int nFlags);

	CLinkedList<CFace>* GetFaceList();
	void Rebuild();
};

// $eb

#endif