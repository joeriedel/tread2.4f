// FACE.H
// Copyright © 1999 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#ifndef FACE_H
#define FACE_H

#include "MathLib.h"
#include "MapObject.h"
#include "Winding.h"
#include "TExport.h"

#define FACE_STORABLE_OBJECT_ID	0xD0000000

class CQBrush;

// $sb "face.h block1"
class _T_EXPORT CFace :   public CWinding, 
				public CBaseTextureableObject,
				public CVisgroupRefObject
{
public:

	friend class CGL3DFaceRenderer;
	friend class CGLFaceRenderer;
	friend class CPreviewDialog;
	friend class CGL3DBrushRenderer;
	friend class CQBrush;
	friend class CTreadMap1X;

protected:

	bool m_bTexInit;
	int m_nNumTexCoords;
	vec_t** m_vTexCoords;

	void GenerateTextureCoords();
	void FreeTexCoords();
	void CopyTexCoords(int nNumTexCoords, vec_t** vTexCoords);
	bool AllocTexCoords(int nNumTexCoords);

	// CWinding.
	virtual void SplitWindingByPlane(CPlane* pPlane, CWinding** pFront, CWinding** pBack)
	{ CWinding::SplitWindingByPlane(pPlane, pFront, pBack); }
	
	virtual CWinding* ChopWindingByPlane(CPlane* pPlane, bool bReturnFront = true)
	{ return CWinding::ChopWindingByPlane(pPlane, bReturnFront); }
	
public:

	CFace();
	CFace(int nNumPoints);
	CFace(int nNumPoints, vec3_t* vPoints);
	CFace(const CFace& FaceToCopy);
	CFace(const CWinding& WindingToCopy);
	virtual ~CFace();

	CBaseObject* m_pParent;

	static void TextureAxisFromPlane(CPlane *pPlane, vec3_t vXV, vec3_t vYV);
	static void MakeTVecs(vec3_t vUV[2], vec3_t vOut[2], vec_t vScales[2], vec_t fRot);
	static void ProjectTexture(vec3_t vUV[2], vec_t vShifts[2], int nNumCoords, vec3_t* vCoords, vec_t** vTexCoords);


	virtual vec_t** GetTextureCoords() { return m_vTexCoords; }
	virtual void CopyWinding(const CWinding& WindingToCopy);
	virtual void CopyPoints(int nNumPoints, vec3_t* vPoints);
	virtual void CopyPoints(int nNumPoints, vec3_t* vPoints, vec_t** vTexs);

	// CWinding
	virtual void Flip();
	virtual void SplitFaceByPlane(CPlane* pPlane, CFace** pFront, CFace** pBack);
	virtual CFace* ChopFaceByPlane(CPlane* pPlane, bool bReturnFront = true);
	static CFace* BaseFaceFromPlane(CPlane* pPlane);

	// CBaseObject.
	virtual int GetObjectClass() { return OBJECT_CLASS_FACE; }
	virtual int GetObjectInterfaces() { return CWinding::GetObjectInterfaces() | I_TEXTUREABLE | I_VISGROUPREF; }
	virtual void* GetInterface(int nInterface);

	// CBaseStorableObject.
	virtual bool ReadObject(CFile& hFile, int nVersion);
	virtual bool WriteObject(CFile& hFile, int nVersion);

	virtual CBaseObject* Clone();

	// CBaseTexturableObject
	// Return True if object supports texturing manipulations.
	virtual bool SupportsTextureManipulations() { return true; }

	virtual void Scale(vec3_t vScale);
	virtual void Scale(vec3_t vScale, vec3_t vOrigin);
	virtual void SetOrigin(vec3_t vOrigin);
	virtual void Translate(vec3_t vTranslation);
	virtual void Rotate(vec3_t vRot);
	virtual void Rotate(vec3_t vOrigin, vec3_t vRot);
	virtual void Reflect(int nAxis, vec3_t vOrigin);

	// Events.
	virtual void OnTextureChange();
	virtual void OnAttributeChange();
	virtual void RebuildTexture();
	
	virtual void OnHotSpotMouseUp(CWindowEventManager* pEventManager, CHotSpotManager* pHotSpotManager, CHotSpot* pHotSpot, CMapView* pView, CTread3DDoc* pDocument, int nMouseX, int nMouseY, int nButtons, int nFlags);
};
// $eb
#endif