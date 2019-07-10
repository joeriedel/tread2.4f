// PATCHMESH.CPP
// Copyright © 2000 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#include "stdafx.h"
#include "patchmesh.h"
#include "geometry.h"
#include "treadsys.h"
#include "tread3d.h"
#include "renderapi.h"
#include "mapview.h"
#include "futils.h"

CPatchMesh::CPatchMesh()
: CBaseGroupableObject()
, CBaseTransformableObject()
, CBaseTextureableObject()
, CVisgroupRefObject()
, CBaseHotSpotEventResponder()
, CBaseHotSpotGenerator()
{
	m_nWidth = m_nHeight = 0;
	m_bNeedRebuild = false;
	m_nResolution = 0;
}

CPatchMesh::CPatchMesh(const CPatchMesh& ToCopy)
: CBaseGroupableObject(ToCopy)
, CBaseTransformableObject(ToCopy)
, CBaseTextureableObject(ToCopy)
, CVisgroupRefObject(ToCopy)
, CBaseHotSpotEventResponder(ToCopy)
, CBaseHotSpotGenerator(ToCopy)
{
	CPatchMesh* pToCopy = (CPatchMesh*)&ToCopy;
	m_bNeedRebuild = ToCopy.m_bNeedRebuild;
	
	CFace* pFace;
	for(pFace = pToCopy->m_FaceList.ResetPos(); pFace != NULL; pFace = pToCopy->m_FaceList.GetNextItem())
	{
		pFace = (CFace*)pFace->Clone();
		pFace->m_pParent = this;
		m_FaceList.AddItem(pFace);
	}

	m_nWidth = ToCopy.m_nWidth;
	m_nHeight = ToCopy.m_nHeight;
	m_nResolution = ToCopy.m_nResolution;
}

CPatchMesh::~CPatchMesh()
{}

int CPatchMesh::m_snDefaultPatchSize = PATCH_SIZE_CODE_3x3;
void CPatchMesh::SetDefaultPatchSize(int nSize)
{
	m_snDefaultPatchSize = nSize;
}

int  CPatchMesh::GetDefaultPatchSize()
{
	return m_snDefaultPatchSize;
}

void* CPatchMesh::GetInterface(int nInterface)
{
	switch(nInterface)
	{
	case I_TRANSFORMABLE:
		return (CBaseTransformableObject*)this;
	case I_GROUPABLE:
		return (CBaseGroupableObject*)this;
	case I_TEXTUREABLE:
		return (CBaseTextureableObject*)this;
	case I_VISGROUPREF:
		return (CVisgroupRefObject*)this;
	}

	return NULL;
}

void CPatchMesh::OnTextureChange()
{
	m_bNeedRebuild = true;
}

// Storable Object.
bool CPatchMesh::ReadObject(CFile& hFile, int nVersion)
{
	int i, k, j;

	if(!CBaseTextureableObject::ReadTextureInfo(hFile, nVersion))
		return false;

	// Load the visgroup info.
	if(!CVisgroupRefObject::Read(hFile))
		return false;

	// Read the width/height and each pt.
	ReadInt(hFile, m_nWidth);
	ReadInt(hFile, m_nHeight);

	for(i = 0; i < m_nWidth; i++)
	{
		for(k = 0; k <m_nHeight; k++)
		{
			for(j = 0; j < 5; j++)
				ReadFloat(hFile, m_vCtrls[i][k][j]);
		}
	}

	m_bNeedRebuild = true;
	return true;
}

bool CPatchMesh::WriteObject(CFile& hFile, int nVersion)
{
	int i, k, j;

	if(!CBaseTextureableObject::WriteTextureInfo(hFile, nVersion))
		return false;
	if(!CVisgroupRefObject::Write(hFile))
		return false;

	WriteInt(hFile, m_nWidth);
	WriteInt(hFile, m_nHeight);
	for(i = 0; i < m_nWidth; i++)
	{
		for(k = 0; k < m_nHeight; k++)
		{
			for(j = 0; j < 5; j++)
				WriteFloat(hFile, m_vCtrls[i][k][j]);
		}
	}

	return true;
}

int CPatchMesh::InitAfterLoad(CTread3DDoc* pDocument)
{
	return IALF_OK;
}

void CPatchMesh::InitFromSelection(CTread3DDoc* pDocument, CMapView* pView, vec3_t vMins, vec3_t vMaxs, vec3_t vOrigin)
{
	int i, k;
	float u, v;
	float ustep, vstep, ulen, vlen;
	vec3_t vPt;

	m_nWidth = GET_PATCH_WIDTH(m_snDefaultPatchSize);
	m_nHeight = GET_PATCH_HEIGHT(m_snDefaultPatchSize);

	ulen = (vMaxs[VI(VEC_X, pView)] - vMins[VI(VEC_X, pView)]);
	vlen = (vMaxs[VI(VEC_Y, pView)] - vMins[VI(VEC_Y, pView)]);

	ustep = ulen / (m_nWidth-1);
	vstep = vlen / (m_nHeight-1);

	u = 0;
	for(i = 0; i < m_nWidth; i++, u += ustep)
	{
		v = 0;
		for(k = 0; k < m_nHeight; k++, v += vstep)
		{
			VectorCopy(vMins, vPt);
			vPt[VI(VEC_X, pView)] += u;
			vPt[VI(VEC_Y, pView)] += v;
			VectorCopy(vPt, m_vCtrls[i][k]);
			m_vCtrls[i][k][4] = u/ulen;	// Fake UV for now.
			m_vCtrls[i][k][5] = v/vlen;
		}
	}

	m_bNeedRebuild = true;
}

void CPatchMesh::GenerateHotSpots(CHotSpotManager* pHotSpotManager, CMapView* pView)
{
}

void CPatchMesh::OnHotSpotDrag(CWindowEventManager* pEventManager, CHotSpotManager* pHotSpotManager, CHotSpot* pHotSpot, CMapView* pView, CTread3DDoc* pDocument, int nMouseX, int nMouseY, int nButtons, int nFlags)
{
}

void CPatchMesh::OnHotSpotMouseUp(CWindowEventManager* pEventManager, CHotSpotManager* pHotSpotManager, CHotSpot* pHotSpot, CMapView* pView, CTread3DDoc* pDocument, int nMouseX, int nMouseY, int nButtons, int nFlags)
{
}

void CPatchMesh::OnHotSpotMouseDown(CWindowEventManager* pEventManager, CHotSpotManager* pHotSpotManager, CHotSpot* pHotSpot, CMapView* pView, CTread3DDoc* pDocument, int nMouseX, int nMouseY, int nButtons, int nFlags)
{
}

void CPatchMesh::SamplePatch(int nUOff, int nVOff, vec_t fU, vec_t fV, vec_t vOut[5])
{
	float vCt[3][5];
	int i, k;
	float a, b, c;

	if(nUOff > MAX_MESH_POINTS-3 || nVOff > MAX_MESH_POINTS-3)
	{
		Warning("CPatchMess::SamplePatch(): offs out of bounds\n");
		return;
	}

	// Evaluates the quadric function at a given u,v axis interval.
	for(i = 0; i < 3; i++)
	{
		for(k = 0; k < 5; k++) // X, Y, Z, S, T
		{
			a = m_vCtrls[nUOff+0][nVOff+i][k];
			b = m_vCtrls[nUOff+1][nVOff+i][k];
			c = m_vCtrls[nUOff+2][nVOff+i][k];
			vCt[i][k] = (a-2*b+c)*fU*fU+(2*b-2*a)*fU+a;
		}
	}

	for(k = 0; k < 5; k++)
	{
		a = vCt[0][k];
		b = vCt[1][k];
		c = vCt[2][k];
		vOut[k] = (a-2*b+c)*fV*fV+(2*b-2*a)*fV+a;			
	}
}

CFace* CPatchMesh::FaceFromSamples(vec_t vCtrls[4][5])
{
	int i, k;
	vec3_t vPts[4];
	vec2_t vTexs[4];
	vec_t* vptr[4];

	for(i = 0; i < 4; i++)
	{
		for(k = 0; k < 3; k++)
		{
			vPts[i][k] = vCtrls[i][k];
		}
	}
	
	for(i = 0; i < 4; i++)
	{
		vptr[i] = vTexs[i];
		for(k = 3; k < 5; k++)
		{
			vTexs[i][k-3] = vCtrls[i][k];
		}
	}

	CFace* pFace = (CFace*)NewBaseObject(OBJECT_CLASS_FACE);
	pFace->CopyPoints(4, vPts, vptr);

	return pFace;
}

CLinkedList<CFace>* CPatchMesh::GetFaceList()
{
	Rebuild();
	return &m_FaceList;
}

void CPatchMesh::Scale(vec3_t vScale)
{
	Scale(vScale, m_vOrigin);	
}

void CPatchMesh::Scale(vec3_t vScale, vec3_t vOrigin)
{
	int i, k;
	vec3_t vNeg = {-vOrigin[0], -vOrigin[1], -vOrigin[2]};

	Translate(vNeg);
	
	for(i = 0; i < m_nWidth; i++)
	{
		for(k = 0; k < m_nHeight; k++)
		{
			VectorMultiply(m_vCtrls[i][k], vScale, m_vCtrls[i][k]);
		}
	}


	Translate(vOrigin);

	m_bNeedRebuild = true;
}

void CPatchMesh::Translate(vec3_t vTranslation)
{
	int i, k;
	
	for(i = 0; i < m_nWidth; i++)
	{
		for(k = 0; k < m_nHeight; k++)
		{
			VectorAdd(m_vCtrls[i][k], vTranslation, m_vCtrls[i][k]);
		}
	}

	m_bNeedRebuild = true;
}

void CPatchMesh::Rotate(vec3_t vRot)
{
	Rotate(vRot, m_vOrigin);
}

void CPatchMesh::Rotate(vec3_t vOrigin, vec3_t vRot)
{
	int i, k;
	vec3_t vNeg = {-vOrigin[0], -vOrigin[1], -vOrigin[2]};

	Translate(vNeg);

	for(i = 0; i < m_nWidth; i++)
	{
		for(k = 0; k < m_nHeight; k++)
		{
			VectorRotate(m_vCtrls[i][k], vRot, m_vCtrls[i][k]);
		}
	}

	Translate(vRot);
	
	m_bNeedRebuild = true;
}

void CPatchMesh::InsertIntoRange(vec3_t vMins, vec3_t vMaxs)
{
	BuildRange();
	InsertRange(m_vMins, vMins, vMaxs);
	InsertRange(m_vMaxs, vMins, vMaxs);
}

bool CPatchMesh::In2DRange(vec3_t vMins, vec3_t vMaxs, int nView)
{
	int i, k, j;

	// See if the 2d point is in the range.
	for(i = 0; i < m_nWidth; i++)
	{
		for(j = 0; j < m_nHeight; j++)
		{
			for(k = VEC_X; k <= VEC_Y; k++)
			{
				if(m_vCtrls[i][k][VMI(nView, k)] < vMins[VMI(nView, k)] ||
					m_vCtrls[i][k][VMI(nView, k)] > vMaxs[VMI(nView, k)])
						break;
			}

			if(k == VEC_Z)
				return true;
		}
	}

	return false;
}

void CPatchMesh::Reflect(int nAxis, vec3_t vFlipOrigin)
{
	int i, k;
	vec_t* p;
	
	// The distance should be mirrored to the opposite side of the origin.
	for(i = 0; i < m_nWidth; i++)
	{
		for(k = 0; k < m_nHeight; k++)
		{
			p = m_vCtrls[i][k];
			p[nAxis] = vFlipOrigin[nAxis] + (vFlipOrigin[nAxis] - p[nAxis]);
		}
	}
}

void CPatchMesh::ComputeOrigin()
{
	// Sum all points, and divide.
	int i, k;
	float num;
	vec3_t vAcc;

	VectorClear(vAcc);
	for(i = 0; i < m_nWidth; i++)
	{
		for(k = 0; k < m_nHeight; k++)
			VectorAdd(m_vCtrls[i][k], vAcc, vAcc);
	}

	num = m_nWidth+m_nHeight;
	VectorScale(vAcc, 1/num, m_vOrigin);
}

void CPatchMesh::BuildRange()
{
	int i, k;

	InitializeRange(m_vMins, m_vMaxs);
	for(i = 0; i < m_nWidth; i++)
	{
		for(k = 0; k < m_nHeight; k++)
		{
			InsertRange(m_vCtrls[i][k], m_vMins, m_vMaxs);
		}
	}
}

#define MAX_PATCH_SAMPLES	256
vec_t vPatchSamples[MAX_PATCH_SAMPLES][MAX_PATCH_SAMPLES][5];

void CPatchMesh::Rebuild()
{
	int i, k, j, h, nsamples;
	int nvPatchOff, nuPatchOff;
	int nU, nV;
	float u, v, ustep, vstep, samples;

	if(!m_bNeedRebuild && (m_nResolution == GetRegInt(I_CURVESUBDEPTH)))
		return;

	m_bNeedRebuild = false;
	m_nResolution = GetRegInt(I_CURVESUBDEPTH);

	samples = 3-1;//2*m_nResolution;//pow(2, m_nResolution) - 1;
	ustep = 1 / samples;
	vstep = ustep;

	nsamples = (int)samples;

	
	nU=nV=0;
	for(nuPatchOff = 0; nuPatchOff < m_nWidth; nuPatchOff += 3)
	{
		for(nvPatchOff = 0; nvPatchOff < m_nHeight; nvPatchOff += 3)
		{
			u = 0;
			nU = nuPatchOff*nsamples;
			for(i = 0; i < nsamples; i++, u += ustep, nU++)
			{
				v = 0;
				nV = nvPatchOff*nsamples;
				for(k = 0; k < nsamples; k++, v += vstep, nV++)
				{
					SamplePatch(nuPatchOff, nvPatchOff, u, v, vPatchSamples[nU][nV]);
				}
			}
		}
	}

	CFace* pFace;
	vec_t vCtrls[4][5];

	m_FaceList.DestroyList();

	// Extract each face.
	for(i = 0; i < nU-1; i++)
	{
		for(k = 0; k < nV-1; k++)
		{
			for(j = 0; j < 4; j++)
			{
				switch(j)
				{
				case 0:
					nuPatchOff = 0;
					nvPatchOff = 0;
					break;
				case 1:
					nuPatchOff = 1;
					nvPatchOff = 0;
					break;
				case 2:
					nuPatchOff = 1;
					nvPatchOff = 1;
					break;
				case 3:
					nuPatchOff = 0;
					nvPatchOff = 1;
					break;
				}

				for(h = 0; h < 5; h++)
					vCtrls[j][h] = vPatchSamples[i+nuPatchOff][k+nvPatchOff][h];
			}

			// Make a face from the completed controls.
			pFace = FaceFromSamples(vCtrls);
			if(pFace == NULL)
			{
				Warning("CPatchMesh::Rebuild(): failed to make face!\n");
				return;
			}
			pFace->m_pParent = this;
			m_FaceList.AddItem(pFace);
			pFace->SetTextureName(this->GetTextureName());
		}
	}
}
