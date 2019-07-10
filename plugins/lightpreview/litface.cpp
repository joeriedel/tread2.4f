// LITFACE.CPP
// Copyright © 2000 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#include "stdafx.h"
#include "../TreadAPI.h"
#include "LitFace.h"

int g_nNumLightmapsBuilt=0;
float g_fSceneMax=0.0f;

CLitFace::CLitFace() : CFace()
{
	m_pLightmaps = new CLinkedList<CLightmap>();
}

CLitFace::CLitFace(const CLitFace& FaceToCopy) : CFace(FaceToCopy)
{
	m_pLightmaps = new CLinkedList<CLightmap>();
}

CLitFace::~CLitFace()
{
	m_pLightmaps->DestroyList();
	delete m_pLightmaps;
}

CLightmapTexture::~CLightmapTexture()
{
	Free();
}

//
// We want to ensure that any physical changes to the face, kill the lightmaps, otherwise we'll
// see some funny artifacts.
//
void CLitFace::Scale(vec3_t vScale)
{
	m_pLightmaps->DestroyList();
	CFace::Scale(vScale);
}

void CLitFace::Scale(vec3_t vScale, vec3_t vOrigin)
{
	m_pLightmaps->DestroyList();
	CFace::Scale(vScale, vOrigin);
}

void CLitFace::SetOrigin(vec3_t vOrigin)
{
	m_pLightmaps->DestroyList();
	CFace::SetOrigin(vOrigin);
}

void CLitFace::Translate(vec3_t vTranslation)
{
	m_pLightmaps->DestroyList();
	CFace::Translate(vTranslation);
}

void CLitFace::Rotate(vec3_t vRot)
{
	m_pLightmaps->DestroyList();
	CFace::Rotate(vRot);
}

void CLitFace::Rotate(vec3_t vOrigin, vec3_t vRot)
{
	m_pLightmaps->DestroyList();
	CFace::Rotate(vOrigin, vRot);
}

void CLitFace::Reflect(int nAxis, vec3_t vOrigin)
{
	m_pLightmaps->DestroyList();
	CFace::Reflect(nAxis, vOrigin);
}

void CLitFace::CreateLightmaps()
{
	m_pLightmaps->DestroyList();
	// Build the lightmaps from this face.
	MakeLightmapsFromWinding_r(this);

	g_nNumLightmapsBuilt += m_pLightmaps->GetCount();
}

#define SUBDIVIDE_SIZE	256.0f

//
// We need to split up the winding because some cards (like the voodoo chipset) do not allow
// texture sizes of > 256x256. We are forced to break them down into a managable size by the GL
// driver.
//
void CLitFace::MakeLightmapsFromWinding_r(CWinding* pWinding)
{
	int i;
	int nNumPoints = pWinding->GetNumPoints();
	vec3_t* pPoints = pWinding->GetPoints();
	vec3_t vEdge, vMid;
	vec_t* pA, *pB;
	vec_t vLength;
	CPlane Plane;

	// If this winding is too large in any dimension, split it on that edge and recurse.

	// Test each edge of the winding.
	for(i = 0; i < nNumPoints; i++)
	{
		pA = pPoints[i];
		pB = pPoints[(i+1)%nNumPoints];

		VectorSubtract(pB, pA, vEdge);
		vLength = VectorLength(vEdge);

		// Let it slide if need be.
		if(vLength >= SUBDIVIDE_SIZE+16.0f)
		{
			// Split along this edge.
			VectorNormalize(vEdge);
			VectorCopy(vEdge, Plane.m_vNormal);
			VectorScale(vEdge, vLength/2.0f, vEdge);
			VectorAdd(vEdge, pA, vMid);

			// The plane that splits this winding on this edge has a vector normal of the edge
			// and passes through the vMid point.
			Plane.m_vDist = DotProduct(vMid, Plane.m_vNormal);

			CWinding* pWF, *pWB;

			// Split.
			pWinding->SplitWindingByPlane(&Plane, &pWF, &pWB);
			if(pWF != NULL && pWB != NULL)
			{
				// Valid split.
				// Subdivide.
				MakeLightmapsFromWinding_r(pWF);
				MakeLightmapsFromWinding_r(pWB);

				// Kill the windings.
				delete pWF;
				delete pWB;

				return;
			}

			Warning("MakeLightmapsFromWinding_r: bad subdivision edge\n");
			if(pWF != NULL)
				delete pWF;
			if(pWB != NULL)
				delete pWB;
		}
	}

	/*pWinding = new CWinding(*pWinding);
	pWinding->Flip();*/

	// All edges passed so make a lightmap.
	CLightmap* pLightmap = new CLightmap(pWinding->GetPoints(), pWinding->GetNumPoints(), pWinding->GetPlanePtr()->m_vNormal, pWinding->GetPlanePtr()->m_vDist);

	// Add to lightmap list.
	m_pLightmaps->AddItem(pLightmap);
}

void CLitFace::AddLight(vec3_t vLightPos, vec3_t vLightColor, vec_t vBrightness, CLinkedList<CBaseObject>* pList)
{
	// Need to build lightmaps?
	if(m_pLightmaps->IsEmpty())
	{
		ComputeOrigin();
		ComputePlane();
		CreateLightmaps();
	}

	//
	// Back facing?
	//
	
	vec3_t vRay;
	VectorSubtract(m_vOrigin, vLightPos, vRay);
	if(DotProduct(vRay, m_Plane.m_vNormal) > -0.001F)	// Can't hit.
		return;

	// Too far?
	if(DotProduct(m_Plane.m_vNormal, vLightPos) - m_Plane.m_vDist > vBrightness)
		return;

	CLightmap* pLightmap;
	m_pLightmaps->First();
	for(pLightmap = m_pLightmaps->GetCurItem(); pLightmap != NULL; pLightmap = m_pLightmaps->GetNextItem())
		pLightmap->AddLight(this, m_Plane.m_vNormal, m_Plane.m_vDist, vLightPos, vLightColor, vBrightness, pList);
}

void CLitFace::ResetLight()
{
	CLightmap* pLightmap;
	m_pLightmaps->First();
	for(pLightmap = m_pLightmaps->GetCurItem(); pLightmap != NULL; pLightmap = m_pLightmaps->GetNextItem())
		pLightmap->Reset();
}

void CLitFace::BuildLightmaps()
{
	CLightmap* pLightmap;
	m_pLightmaps->First();
	for(pLightmap = m_pLightmaps->GetCurItem(); pLightmap != NULL; pLightmap = m_pLightmaps->GetNextItem())
		pLightmap->BuildLightmap();
}

CLightmap::CLightmap(vec3_t* vPoints, int nNumPoints, vec3_t vNormal, vec_t vDist)
{
	m_nNumLightPts = 0;
	m_pLightPts = NULL;
	m_pTexture = NULL;
	m_pFragTexs = NULL;
	
	// Copy fragment points.
	m_pFragPts = new vec3_t[nNumPoints];
	memcpy(m_pFragPts, vPoints, sizeof(vec3_t)*nNumPoints);
	m_nNumFragPts = nNumPoints;

	SetupLightmap(vNormal, vDist);
}

CLightmap::~CLightmap()
{
	if(m_pTexture != NULL)
	{
		// We may need to update the 3d view.
		CBaseWindowRenderer* pWR = GetMapManager()->GetDocPtr()->GetViewByType(VIEW_3D)->GetWindowRenderer();
		if(pWR != NULL && pWR->GetRendererUID() == GL3DWINDOWRENDERER_UID)
			((CGL3DWindowRenderer*)pWR)->DetachTexture(m_pTexture);

		delete m_pTexture;
	}

	if(m_pLightPts != NULL)
		delete[] m_pLightPts;
	if(m_pFragPts)
		delete[] m_pFragPts;
	if(m_pFragTexs)
		delete[] m_pFragTexs;
}

bool CLightmap::BrushHitsRay(vec3_t vStart, vec3_t vEnd, vec3_t vRay, vec_t vLength, CFace* pTestFace, CQBrush* pBrush)
{
	CFace* pFace;

	if(pBrush->m_pFaceList->InList(pTestFace))
		return false;

	pBrush->m_pFaceList->First();
	for(pFace = pBrush->m_pFaceList->GetCurItem(); pFace != NULL; pFace = pBrush->m_pFaceList->GetNextItem())
	{
		if(FaceHitsRay(vStart, vEnd, vRay, vLength, pFace))
			return true;
	}

	return false;
}

// Lummel ratio.
#define LUMEL_RATIO	16

bool CLightmap::FaceHitsRay(vec3_t vStart, vec3_t vEnd, vec3_t vRay, vec_t vRayLength, CFace* pFace)
{
	// On both sides?
	int i, nNumPts;
	vec3_t vMids, vClipRay;
	vec_t vD1, vD2, vDot, vDist, vTest;
	vec_t* vPA, *vPB, *vPC;
	vec3_t* vPts;
	vec3_t vEdge, vOrtho;
	CPlane* pPlane = pFace->GetPlanePtr();

	vD1 = DotProduct(vStart, pPlane->m_vNormal) - pPlane->m_vDist;
	vD2 = DotProduct(vEnd, pPlane->m_vNormal) - pPlane->m_vDist;

	if(vD1 > 0.0f && vD2 > 0.0f)
		return false;
	if(vD1 < 0.0f && vD2 < 0.0f)
		return false;

	// Compute intersection with plane.
	vDot = vD1 / (vD1 - vD2);

	for(i = 0; i < 3; i++)
	{
		// Avoid roundoffs.
		if(pPlane->m_vNormal[i] == 1.0F)
			vMids[i] = pPlane->m_vDist;
		else if(pPlane->m_vNormal[i] == -1.0F)
			vMids[i] = -pPlane->m_vDist;
		else
			vMids[i] = vStart[i] + vDot * (vEnd[i] - vStart[i]);
	}

	// Too far off?
	VectorSubtract(vMids, vStart, vClipRay);
	if(VectorLength(vClipRay) > vRayLength-(LUMEL_RATIO/2))
		return false;

	// Bounded by winding?
	nNumPts = pFace->GetNumPoints();
	vPts = pFace->GetPoints();

	for(i = 0; i < nNumPts; i++)
	{
		vPA = vPts[i];
		vPB = vPts[(i+1)%nNumPts];
		vPC = vPts[(i+2)%nNumPts];

		VectorSubtract(vPB, vPA, vEdge);
		VectorNormalize(vEdge);
		CalcOrthoVector(vEdge, pPlane->m_vNormal, vOrtho);

		vDist = DotProduct(vPA, vOrtho);

		// Check.
		vTest = DotProduct(vPC, vOrtho) - vDist;

		if(vTest < 0.0f)	// Flip
		{
			vDist = -vDist;
			VectorScale(vOrtho, -1.0f, vOrtho);
		}

		// Is mid on front?
		vTest = DotProduct(vMids, vOrtho) - vDist;
		if(vTest < 0.0f)
			return false;	// Isn't bounded.
	}

	return true;
}

bool CLightmap::BrushListHitsRay(vec3_t vStart, vec3_t vEnd, vec3_t vRay, vec_t vLength, CFace* pTestFace, CLinkedList<CBaseObject>* pList)
{
	CQBrush* pBrush;
	CBaseObject* pObj;

	pList->First();
	for(pObj = pList->GetCurItem(); pObj != NULL; pObj = pList->GetNextItem())
	{
		if(pObj->GetObjectClass() == OBJECT_CLASS_GROUP)
		{
			if(BrushListHitsRay(vStart, vEnd, vRay, vLength, pTestFace, ((CObjectGroup*)pObj)->GetObjectList()))
				return true;
			continue;
		}
		if(pObj->GetObjectClass() != OBJECT_CLASS_BRUSH)
			continue;

		pBrush = (CQBrush*)pObj;
		if(BrushHitsRay(vStart, vEnd, vRay, vLength, pTestFace, pBrush))
			return true;
	}

	return false;
}

void CLightmap::AddLight(CFace* pFace, vec3_t vFaceNormal, vec_t vFaceDist, vec3_t vLightPos, vec3_t vLightColor, vec_t vBrightness, CLinkedList<CBaseObject>* pList)
{
	int i, j;
	vec_t vDist;
	vec_t vDot;
	vec_t vLight;
	vec3_t vRay;
	vec3_t vAddColor;

	for(i = 0; i < m_nNumLightPts; i++)
	{
		VectorSubtract(m_pLightPts[i], vLightPos, vRay);
		vDist = VectorLength(vRay);

		VectorNormalize(vRay);
		vDot = DotProduct(vRay, vFaceNormal);
		if(vDot > -0.001)	// Behind.
			continue;

		vLight = (vBrightness - vDist) * -vDot;
		if(vLight <= 0)
			continue;

		// See if it's blocked.
		if(pList != NULL)
		{
			if(BrushListHitsRay(vLightPos, m_pLightPts[i], vRay, vDist, pFace, pList))
				continue;
		}

		// Multiply.
		VectorScale(vLightColor, vLight, vAddColor);
		VectorAdd(vAddColor, m_vTexColor[i], m_vTexColor[i]);
		for(j = 0; j < 3; j++)
			if(m_vTexColor[i][j] > g_fSceneMax)
				g_fSceneMax = m_vTexColor[i][j];
	}
}

void CLightmap::BuildLightmap()
{
	int i;

	// Divide each by max.
	for(i = 0; i < m_nNumLightPts; i++)
	{
		m_pTexture->m_pData[i*3] = m_vTexColor[i][2]/g_fSceneMax*255.0f;
		m_pTexture->m_pData[i*3+1] = m_vTexColor[i][1]/g_fSceneMax*255.0f;
		m_pTexture->m_pData[i*3+2] = m_vTexColor[i][0]/g_fSceneMax*255.0f;
	}
}

void CLightmap::Reset()
{
	if(m_pTexture == NULL)
		return;

	m_pTexture->m_bModified = true;

	// Clear the texture.
	memset(m_pTexture->m_pData, 0, m_pTexture->m_nSize);
	memset(m_vTexColor, 0, sizeof(vec3_t)*m_pTexture->m_nSize);
}

void CLightmap::BuildCorners(int nXAxis, int nYAxis, int nZAxis, vec3_t vNormal, vec_t vDist)
{
	int i;
	vec3_t vPt;

	// Find top/left point.
	vPt[nXAxis] = 99999;
	vPt[nYAxis] = 99999;
	
	for(i = 0; i < m_nNumFragPts; i++)
	{
		if(m_pFragPts[i][nXAxis] < vPt[nXAxis])
			vPt[nXAxis] = m_pFragPts[i][nXAxis];
		if(m_pFragPts[i][nYAxis] < vPt[nYAxis])
			vPt[nYAxis] = m_pFragPts[i][nYAxis];
	}

	VectorCopy(vPt, m_vCorners[0]);

	// Find top/right point.
	vPt[nXAxis] = -99999;
	vPt[nYAxis] = 99999;
	
	for(i = 0; i < m_nNumFragPts; i++)
	{
		if(m_pFragPts[i][nXAxis] > vPt[nXAxis])
			vPt[nXAxis] = m_pFragPts[i][nXAxis];
		if(m_pFragPts[i][nYAxis] < vPt[nYAxis])
			vPt[nYAxis] = m_pFragPts[i][nYAxis];
	}

	VectorCopy(vPt, m_vCorners[1]);

	// Find bottom/right point.
	vPt[nXAxis] = -99999;
	vPt[nYAxis] = -99999;
	
	for(i = 0; i < m_nNumFragPts; i++)
	{
		if(m_pFragPts[i][nXAxis] > vPt[nXAxis])
			vPt[nXAxis] = m_pFragPts[i][nXAxis];
		if(m_pFragPts[i][nYAxis] > vPt[nYAxis])
			vPt[nYAxis] = m_pFragPts[i][nYAxis];
	}

	VectorCopy(vPt, m_vCorners[2]);

	// Find bottom/left point.
	vPt[nXAxis] = 99999;
	vPt[nYAxis] = -99999;
	
	for(i = 0; i < m_nNumFragPts; i++)
	{
		if(m_pFragPts[i][nXAxis] < vPt[nXAxis])
			vPt[nXAxis] = m_pFragPts[i][nXAxis];
		if(m_pFragPts[i][nYAxis] > vPt[nYAxis])
			vPt[nYAxis] = m_pFragPts[i][nYAxis];
	}

	VectorCopy(vPt, m_vCorners[3]);

	// Solve the z coordinates of the lightmap, it lies on the face plane.
	for(i = 0; i < 4; i++)
		m_vCorners[i][nZAxis] = (vDist-(m_vCorners[i][nXAxis]*vNormal[nXAxis])-(m_vCorners[i][nYAxis]*vNormal[nYAxis]))/vNormal[nZAxis];

	// Setup U/V.
	m_vUVLengths[0] = fabs(m_vCorners[2][nXAxis] - m_vCorners[0][nXAxis]);
	m_vUVLengths[1] = fabs(m_vCorners[2][nYAxis] - m_vCorners[0][nYAxis]);
}

void CLightmap::BuildLightPoints()
{
	int i, k;
	int nIndex;
	float fStep;
	vec3_t vLStep, vTStep;
	vec3_t vLEdge, vTEdge;
	vec3_t vPt;

	// Setup top edge.
	VectorSubtract(m_vCorners[1], m_vCorners[0], vTEdge);
	fStep = VectorLength(vTEdge) / (m_pTexture->m_nWidth-1);
	VectorNormalize(vTEdge);
	
	// X Texture edge.
	VectorCopy(vTEdge, m_vTexEdges[0]);
	m_vTexDist[0] = DotProduct(m_vCorners[0], vTEdge);

	VectorScale(vTEdge, fStep, vTStep);

	// Setup left edge.
	VectorSubtract(m_vCorners[3], m_vCorners[0], vLEdge);
	fStep = VectorLength(vLEdge) / (m_pTexture->m_nHeight-1);
	VectorNormalize(vLEdge);

	// Y Texture edge.
	VectorCopy(vLEdge, m_vTexEdges[1]);
	m_vTexDist[1] = DotProduct(m_vCorners[0], vLEdge);

	VectorScale(vLEdge, fStep, vLStep);

	m_nNumLightPts = m_pTexture->m_nWidth*m_pTexture->m_nHeight;
	if(m_pLightPts)
		delete[] m_pLightPts;
	m_pLightPts = new vec3_t[m_nNumLightPts];

	// Start at top/left corner.
	nIndex = 0;
	VectorClear(vLEdge);

	for(i = 0; i < m_pTexture->m_nHeight; i++)
	{
		VectorClear(vTEdge);
		for(k = 0; k < m_pTexture->m_nWidth; k++)
		{
			VectorAdd(vTEdge, vLEdge, vPt);
			VectorAdd(m_vCorners[0], vPt, m_pLightPts[nIndex++]);
			VectorAdd(vTEdge, vTStep, vTEdge);
		}
		VectorAdd(vLEdge, vLStep, vLEdge);
	}
}

int CLightmap::MakePowerOf2(int n)
{
	int b_mask;
	
	if(n == 0)
		return 1;

	b_mask=0x80000000;
	while(!(n & b_mask))
		b_mask >>= 1;
	n = -(b_mask); 
	
	return n;
}

void CLightmap::SetupLightmap(vec3_t vNormal, vec_t vDist)
{
	int i;

	if(m_pTexture != NULL)
		delete m_pTexture;
	m_pTexture = new CLightmapTexture();
	m_pTexture->m_bModified = false;
	
	GetTexAPI()->AssignID(m_pTexture);

	// Find major axis.
	int nAxis;
	float fMax = -99999;

	for(i = 0; i < 3; i++)
	{
		if(fabs(vNormal[i]) > fMax)
		{
			fMax = fabs(vNormal[i]);
			nAxis = i;
		}
	}


	// Make the corner points based on major axis.
	if(nAxis == VEC_X)
		// X becomes Z, Y becomes X, Z becomes Y
		BuildCorners(VEC_Y, VEC_Z, VEC_X, vNormal, vDist);
	else if(nAxis == VEC_Y)
		// X becomes X, Y becomes Z, Z becomes Y
		BuildCorners(VEC_X, VEC_Z, VEC_Y, vNormal, vDist);
	else if(nAxis == VEC_Z)
		// X becomes X, Y becomes Y, Z becomes Z
		BuildCorners(VEC_X, VEC_Y, VEC_Z, vNormal, vDist);

	// Setup the lightmap data.
	m_pTexture->m_nWidth  = MakePowerOf2((int)m_vUVLengths[0]+0.5f)/LUMEL_RATIO;
	if(m_pTexture->m_nWidth < 2)
		m_pTexture->m_nWidth = 2;
	if(m_pTexture->m_nWidth > 256)
		m_pTexture->m_nWidth = 256;
	m_pTexture->m_nHeight = MakePowerOf2((int)m_vUVLengths[1]+0.5f)/LUMEL_RATIO;
	if(m_pTexture->m_nHeight < 2)
		m_pTexture->m_nHeight = 2;
	if(m_pTexture->m_nHeight > 256)
		m_pTexture->m_nHeight = 256;

	m_pTexture->m_nSclWidth	 = m_pTexture->m_nWidth;
	m_pTexture->m_nSclHeight = m_pTexture->m_nHeight;

	m_pTexture->m_nSize = m_pTexture->m_nWidth*m_pTexture->m_nHeight*3;
	m_pTexture->m_pData = new unsigned char[m_pTexture->m_nSize];
	m_pTexture->m_nBits = 24;

	m_vTexColor = new vec3_t[m_pTexture->m_nSize];

	Reset();

	BuildLightPoints();

	// Compute fragment texture coords.
	if(m_pFragTexs != NULL)
		delete[] m_pFragTexs;

	m_pFragTexs = new vec2_t[m_nNumFragPts];
	for(i = 0; i < m_nNumFragPts; i++)
	{
		m_pFragTexs[i][0] = (DotProduct(m_pFragPts[i], m_vTexEdges[0]) - m_vTexDist[0]) / m_vUVLengths[0];

		// Get rid of stupid gl borders.
		if(m_pFragTexs[i][0] < (1.0f/(m_pTexture->m_nWidth)))
			m_pFragTexs[i][0] = 1.0f/(m_pTexture->m_nWidth)/2;
		if(m_pFragTexs[i][0] > (1.0f-(1.0f/(m_pTexture->m_nWidth))))
			m_pFragTexs[i][0] = (1.0f-(1.0f/(m_pTexture->m_nWidth)/2));

		m_pFragTexs[i][1] = (DotProduct(m_pFragPts[i], m_vTexEdges[1]) - m_vTexDist[1]) / m_vUVLengths[1];

		// Get rid of stupid gl borders.
		if(m_pFragTexs[i][1] < (1.0f/(m_pTexture->m_nHeight)))
			m_pFragTexs[i][1] = 1.0f/(m_pTexture->m_nHeight)/2;
		if(m_pFragTexs[i][1] > (1.0f-(1.0f/(m_pTexture->m_nHeight))))
			m_pFragTexs[i][1] = (1.0f-(1.0f/(m_pTexture->m_nHeight)/2));

	}
}
