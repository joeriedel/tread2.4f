// BEZIERPATCH.H
// Copyright © 1999 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#ifndef BEZIERPATCH_H
#define BEZIERPATCH_H

#include "MathLib.h"
#include "LinkedList.h"
#include "MapObject.h"
#include "Face.h"
#include "TExport.h"

#define DEFAULT_PATCH_DEPTH	3

// Stores 4 edges of bezier curves.
struct BEZIERMATRIX
{
	struct BEZIERMATRIX* prev, *next;

	vec3_t m_vPts[16];
};

class CBezierPatch : public CBaseGroupableObject,
					 public CBaseTransformableObject,
					 public CBaseTextureableObject,
					 public CVisgroupRefObject,
					 public CBaseHotSpotEventResponder,
					 public CBaseHotSpotGenerator
{
private:

	bool m_bNoBuild;
	int m_nNumFaces;
	float m_fX, m_fY;
	CLinkedList<BEZIERMATRIX>* m_pPatchList;
	CLinkedList<CFace>* m_pFaceList;

	BEZIERMATRIX m_BaseMatrix;	// The base patch via which all others are generated.

	// Generates faces from patch list.
	void GenerateFacesFromPatchList();
	// Generates the final patch list from the m_BaseMatrix
	void GeneratePatchList(int nDepth);
	void QuadSplitPatch(BEZIERMATRIX* pSourceMatrix, BEZIERMATRIX* pOutMatrix[4]);
	void QuadSplitPatchList(CLinkedList<BEZIERMATRIX>* pPatchList, CLinkedList<BEZIERMATRIX>* pOutList);
	void USplitPatch(BEZIERMATRIX* pSourceMatrix, BEZIERMATRIX* pOutMatrix[2]);
	void VSplitPatch(BEZIERMATRIX* pSourceMatrix, BEZIERMATRIX* pOutMatrix[2]);
	void SplitControls(vec3_t vControls[4], vec3_t vControlsA[4], vec3_t vControlsB[4]);

public:

	friend class CGDIBezierPatchRenderer;
	friend class CGLBezierPatchRenderer;
	friend class CGL3DBezierPatchRenderer;

	CBezierPatch();
	CBezierPatch(const CBezierPatch& PatchToCopy);
	virtual ~CBezierPatch();

	// Returns the # of faces the patch has.
	int GetNumFaces() { return m_nNumFaces; }

	// CBaseObject
	virtual int GetObjectClass() { return OBJECT_CLASS_BPATCH; }
	virtual int GetObjectInterfaces() { return I_GROUPABLE | I_VISGROUPREF | I_TEXTUREABLE | I_TRANSFORMABLE | I_STORABLE; }
	virtual void* GetInterface(int nInterface);

	// Generates the curve.
	void BuildPatch(int nSubdivisionDepth);
	
	virtual void OnTextureChange();

	// Storable Object.
	virtual bool ReadObject(CFile& hFile, int nVersion);
	virtual bool WriteObject(CFile& hFile, int nVersion);

	virtual CBaseObject* Clone() { return new CBezierPatch(*this); }

	virtual int InitAfterLoad(CTread3DDoc* pDocument);
	virtual void InitAsModel(CTread3DDoc* pDocument);

	// BaseTransformable.
	virtual void Scale(vec3_t vScale);	// Scales the objects size.
	virtual void Scale(vec3_t vScale, vec3_t vOrigin);
	virtual void Translate(vec3_t vTranslation);
	virtual void Rotate(vec3_t vRot);
	virtual void Rotate(vec3_t vOrigin, vec3_t vRot);
	virtual void InsertIntoRange(vec3_t vMins, vec3_t vMaxs);
	virtual void Reflect(int nAxis, vec3_t vFlipOrigin);
	virtual void ComputeOrigin();
	virtual void BuildRange();

	// Hotspot.
	virtual void GenerateHotSpots(CHotSpotManager* pHotSpotManager, CMapView* pView);
	virtual void OnHotSpotDrag(CWindowEventManager* pEventManager, CHotSpotManager* pHotSpotManager, CHotSpot* pHotSpot, CMapView* pView, CTread3DDoc* pDocument, int nMouseX, int nMouseY, int nButtons, int nFlags);
	virtual void OnHotSpotMouseUp(CWindowEventManager* pEventManager, CHotSpotManager* pHotSpotManager, CHotSpot* pHotSpot, CMapView* pView, CTread3DDoc* pDocument, int nMouseX, int nMouseY, int nButtons, int nFlags);
	virtual void OnHotSpotMouseDown(CWindowEventManager* pEventManager, CHotSpotManager* pHotSpotManager, CHotSpot* pHotSpot, CMapView* pView, CTread3DDoc* pDocument, int nMouseX, int nMouseY, int nButtons, int nFlags);

	static CBezierPatch* TestPatch();
};

#endif