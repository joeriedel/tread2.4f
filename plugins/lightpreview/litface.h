// LITFACE.H
// Copyright © 2000 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#ifndef LITFACE_H
#define LITFACE_H

#include "../TreadAPI.h"

//
// This face object stores lightmaps to be blended with the normal face.
//

extern int g_nNumLightmapsBuilt;
extern float g_fSceneMax;

class CLightmapTexture : public CTexture
{
public:

	virtual ~CLightmapTexture();

	bool m_bModified;

	bool Load()
	{ return true; }

	// Loads only info.
	bool LoadInfo()
	{ return true; }
	
	// Free's any memory.
	bool Free()
	{
		return FreeData();
	}

	// Free's the texture data.
	bool FreeData()
	{
		if(m_pData != NULL)
			delete[] m_pData;
		m_pData = NULL;
		return true;
	}

	// Converts a texture to 24bit color.
	bool To24BitColor()
	{
		return true;
	}

	bool ApplyGamma()
	{
		return true;
	}
};

class CLightmap : public CLLObject
{
private:

	void BuildLightPoints();
	void BuildCorners(int nXAxis, int nYAxis, int nZAxis, vec3_t vNormal, vec_t vDist);
	int MakePowerOf2(int n);
	void SetupLightmap(vec3_t vNormal, vec_t vDist);
	bool BrushHitsRay(vec3_t vStart, vec3_t vEnd, vec3_t vRay, vec_t vLength, CFace* pTestFace, CQBrush* pBrush);
	bool FaceHitsRay(vec3_t vStart, vec3_t vEnd, vec3_t vRay, vec_t vLength, CFace* pFace);
	bool BrushListHitsRay(vec3_t vStart, vec3_t vEnd, vec3_t vRay, vec_t vLength, CFace* pTestFace, CLinkedList<CBaseObject>* pList);

public:

	CLightmap(vec3_t* vPoints, int nNumPoints, vec3_t vNormal, vec_t vDist);
	virtual ~CLightmap();

	vec3_t* m_vTexColor;
	CLightmapTexture* m_pTexture;

	int m_nNumFragPts;
	vec3_t* m_pFragPts;
	vec2_t* m_pFragTexs;

	vec2_t m_vCornerTex[4];
	vec3_t m_vCorners[4];

	vec_t m_vTexDist[2];
	vec_t m_vUVLengths[2];
	vec3_t m_vTexEdges[2];

	int m_nNumLightPts;
	vec3_t* m_pLightPts;

	void AddLight(CFace* pFace, vec3_t vFaceNormal, vec_t vFaceDist, vec3_t vLightPos, vec3_t vLightColor, vec_t vBrightness, CLinkedList<CBaseObject>* pList);
	void BuildLightmap();
	void Reset();
};

// Because we don't override GetObjectClass() this face is really an OBJECT_CLASS_FACE and will be treated as such.

// This class name was not intended to be sexual in any way... you perverts.
class CLitFace : public CFace
{
private:

	CLinkedList<CLightmap>* m_pLightmaps;

	void CreateLightmaps();
	void MakeLightmapsFromWinding_r(CWinding* pWinding);

public:

	CLitFace();
	CLitFace(const CLitFace& FaceToCopy);

	virtual ~CLitFace();

	void AddLight(vec3_t vLightPos, vec3_t vLightColor, vec_t vBrightness, CLinkedList<CBaseObject>* pList);
	void BuildLightmaps();
	void ResetLight();

	virtual CBaseObject* Clone() { return new CLitFace(*this); }

	CLinkedList<CLightmap>* GetLightmaps() { return m_pLightmaps; }

	// Any operation like this kills the lightmaps.
	virtual void Scale(vec3_t vScale);
	virtual void Scale(vec3_t vScale, vec3_t vOrigin);
	virtual void SetOrigin(vec3_t vOrigin);
	virtual void Translate(vec3_t vTranslation);
	virtual void Rotate(vec3_t vRot);
	virtual void Rotate(vec3_t vOrigin, vec3_t vRot);
	virtual void Reflect(int nAxis, vec3_t vOrigin);
};

#endif