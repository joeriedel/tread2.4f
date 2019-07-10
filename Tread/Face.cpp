// FACE.CPP
// Copyright © 1999 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#include "stdafx.h"
#include <stdio.h>
#include "MathLib.h"
#include "Face.h"
#include "MapObject.h"
#include "TreadSys.h"
#include "Tread3DDoc.h"
#include "RenderAPI.h"
#include "Brush.h"

// Texture mapping done by Tread is identical to how QE3 does it, not to 
// say it is flawless. Each face has an axis aligned texture projected
// onto it. This causes small texture warping of non axial planes but the
// warping is minute. In exchange we get textures that align naturally on
// walls of different orientation.

// Defines base axis and there s,t axial directions.
vec3_t	g_vBaseAxis[18] =
{
{0,0,1}, {1,0,0}, {0,-1,0},			// floor
{0,0,-1}, {1,0,0}, {0,-1,0},		// ceiling
{1,0,0}, {0,1,0}, {0,0,-1},			// west wall
{-1,0,0}, {0,1,0}, {0,0,-1},		// east wall
{0,1,0}, {1,0,0}, {0,0,-1},			// south wall
{0,-1,0}, {1,0,0}, {0,0,-1}			// north wall
};

// Constructors.
CFace::CFace() 
	: CWinding()
	, CBaseTextureableObject()
{
	m_nNumTexCoords = 0;
	m_vTexCoords = NULL;
	m_bTexInit = false;
}

CFace::CFace(int nNumPoints)
	: CWinding(nNumPoints)
	, CBaseTextureableObject()
{
	m_nNumTexCoords = 0;
	m_vTexCoords = NULL;
	m_bTexInit = false;
}

CFace::CFace(const CFace& FaceToCopy)
	: CWinding(FaceToCopy)
	, CBaseTextureableObject(FaceToCopy)
{
	m_nNumTexCoords = 0;
	m_vTexCoords = NULL;
	CopyTexCoords(FaceToCopy.m_nNumTexCoords, FaceToCopy.m_vTexCoords);
	m_bTexInit = FaceToCopy.m_bTexInit;
}

CFace::CFace(const CWinding& WindingToCopy)
	: CWinding(WindingToCopy)
	, CBaseTextureableObject()
{
	m_nNumTexCoords = 0;
	m_vTexCoords = NULL;
	m_bTexInit = false;
	GenerateTextureCoords();
}

CFace::CFace(int nNumPoints, vec3_t* vPoints)
	: CWinding(nNumPoints, vPoints)
	, CBaseTextureableObject()
{
	m_nNumTexCoords = 0;
	m_vTexCoords = NULL;
	m_bTexInit = false;
	GenerateTextureCoords();
}

CFace::~CFace()
{
	FreeTexCoords();
}

CBaseObject* CFace::Clone()
{ 
	return new CFace(*this); 
}

void CFace::CopyPoints(int nNumPoints, vec3_t* vPoints)
{
	CWinding::CopyPoints(nNumPoints, vPoints);

	m_nNumTexCoords = 0;
	m_vTexCoords = NULL;
	m_bTexInit = false;
	GenerateTextureCoords();
}

void CFace::CopyPoints(int nNumPoints, vec3_t* vPoints, vec_t** vTexs)
{
	CWinding::CopyPoints(nNumPoints, vPoints);

	CopyTexCoords(nNumPoints, (float**)vTexs);
}

void CFace::CopyWinding(const CWinding& WindingToCopy)
{
	m_nNumTexCoords = 0;
	m_vTexCoords = NULL;
	m_bTexInit = false;
	CopyPoints(WindingToCopy.GetNumPoints(), WindingToCopy.GetPoints());
	GenerateTextureCoords();
}

CFace* CFace::BaseFaceFromPlane(CPlane* pPlane)
{
	CFace* pFace;
	CWinding* pWinding = CWinding::BaseWindingFromPlane(pPlane);
	if(pWinding == NULL)
		return NULL;

	pFace = (CFace*)NewBaseObject(OBJECT_CLASS_FACE);//new CFace(*pWinding);
	pFace->CopyWinding(*pWinding);
	delete pWinding;
	return pFace;
}

void CFace::OnHotSpotMouseUp(CWindowEventManager* pEventManager, CHotSpotManager* pHotSpotManager, CHotSpot* pHotSpot, CMapView* pView, CTread3DDoc* pDocument, int nMouseX, int nMouseY, int nButtons, int nFlags)
{
	//CWinding::OnHotSpotMouseUp(pEventManager, pHotSpotManager, pHotSpot, pView, pDocument, nMouseX, nMouseY, nButtons);
	ComputeOrigin();
	ComputePlane();
	if(m_pParent == NULL)
		GenerateTextureCoords();
	
	if(nFlags == HEF_NONE || ((nFlags&(HEF_GROUP|HEF_TAIL))==(HEF_GROUP|HEF_TAIL)))
	{

		// Dispatch.
		CHotSpot* pSpot;
		pHotSpotManager->RestoreIdenticalSearch();
		for(pSpot = pHotSpotManager->GetNextIdenticalHotSpot(NULL, NULL, 0); pSpot != NULL; pSpot = pHotSpotManager->GetNextIdenticalHotSpot(NULL, NULL, 0))
			pSpot->m_pEventResponder->OnHotSpotMouseUp(pEventManager, pHotSpotManager, pHotSpot, pView, pDocument, nMouseX, nMouseY, nButtons, HEF_MULTI);

		// Parent is a textureable object?
		if(m_pParent && (m_pParent->GetObjectInterfaces()&I_TEXTUREABLE))
			((CBaseTextureableObject*)m_pParent->GetInterface(I_TEXTUREABLE))->RebuildTexture();

		pEventManager->EnableAutoScroll(NULL, false);
		pDocument->RemoveGlobalRenderFlags(RF_FAST/*|RF_NOHOTSPOTS|RF_NOSELBOX*/);
		pDocument->BuildSelectionBBox();
		pDocument->BuildHotSpots();
		pDocument->UpdateWindows();
	}
}

void CFace::Reflect(int nAxis, vec3_t vFlipOrigin)
{
	CWinding::Reflect(nAxis, vFlipOrigin);
	GenerateTextureCoords();
}

void* CFace::GetInterface(int nInterface)
{
	switch(nInterface)
	{
	case I_GROUPABLE:
		return (CBaseGroupableObject*)this;
	case I_TRANSFORMABLE:
		return (CBaseTransformableObject*)this;
	case I_TEXTUREABLE:
		return (CBaseTextureableObject*)this;
	case I_VISGROUPREF:
		return (CVisgroupRefObject*)this;
	}

	return NULL;
}

// Called when face is flipped.
void CFace::Flip()
{
	CWinding::Flip();
	GenerateTextureCoords();
}

// Splits a face by a plane.
void CFace::SplitFaceByPlane(CPlane* pPlane, CFace** pFront, CFace** pBack)
{
	CWinding* pFW, *pBW;

	SplitWindingByPlane(pPlane, &pFW, &pBW);
	
	if(pFW)
	{
		*pFront = (CFace*)NewBaseObject(OBJECT_CLASS_FACE);//new CFace(*pFW);
		(*pFront)->CopyWinding(*pFW);
		delete pFW;
		(*pFront)->SetTextureName(this->GetTextureName());
	}
	else
		*pFront = NULL;

	if(pBW)
	{
		*pBack = (CFace*)NewBaseObject(OBJECT_CLASS_FACE);//new CFace(*pBW);
		(*pBack)->CopyWinding(*pBW);
		delete pBW;
		(*pBack)->SetTextureName(this->GetTextureName());
	}
	else
		*pBack = NULL;
}

// Splits the face and returns only one side.
CFace* CFace::ChopFaceByPlane(CPlane* pPlane, bool bReturnFront)
{
	CFace* pFront, *pBack;

	SplitFaceByPlane(pPlane, &pFront, &pBack);
	if(bReturnFront)
		return pFront;
	return pBack;
}

// Called when the texture changes.
void CFace::OnTextureChange()
{
	if(GetTexturePtr() == NULL)
		return;
	m_nSurface = m_pTexture->m_nSurface;
	m_nValue = m_pTexture->m_nValue;
	m_nContents = m_pTexture->m_nContents;
}

void CFace::OnAttributeChange()
{
	if(GetTexturePtr() == NULL)
		return;

	m_nObjectRenderFlags &= ~(RF_TRANSPARENT|RF_FOG);
	m_nObjectRenderFlags |= m_pTexture->m_pReader->TranslateSurfaceToRenderFlags(m_nSurface);
}

// Called when an attribute changes.
void CFace::RebuildTexture()
{
	GenerateTextureCoords();
}

void CFace::Scale(vec3_t vScale)
{
	CWinding::Scale(vScale);
	GenerateTextureCoords();
}

void CFace::Scale(vec3_t vScale, vec3_t vOrigin)
{
	CWinding::Scale(vScale, vOrigin);
	GenerateTextureCoords();
}

void CFace::SetOrigin(vec3_t vOrigin)
{
	CWinding::Scale(vOrigin);
	GenerateTextureCoords();
}

void CFace::Translate(vec3_t vTranslation)
{
	CWinding::Translate(vTranslation);
	GenerateTextureCoords();
}

void CFace::Rotate(vec3_t vRot)
{
	CWinding::Rotate(vRot);
	GenerateTextureCoords();
}

void CFace::Rotate(vec3_t vOrigin, vec3_t vRot)
{
	CWinding::Rotate(vOrigin, vRot);
	GenerateTextureCoords();
}

// Writes the object.
bool CFace::WriteObject(CFile& hFile, int nVersion)
{
	// Store winding.
	CWinding::WriteObject(hFile, nVersion);

	// Store the texture data.
	CBaseTextureableObject::WriteTextureInfo(hFile, nVersion);	

	// Store Visgroup info.
	CVisgroupRefObject::Write(hFile);

	return true;
}

// Reads the object.
bool CFace::ReadObject(CFile& hFile, int nVersion)
{
	// Load the winding.
	if(!CWinding::ReadObject(hFile, nVersion))
		return false;

	// Load the texture info.
	if(!CBaseTextureableObject::ReadTextureInfo(hFile, nVersion))
		return false;

	// Load the visgroup info.
	if(!CVisgroupRefObject::Read(hFile))
		return false;

	//AllocTexCoords(m_nNumPoints);
	GenerateTextureCoords();

	return true;
}

void CFace::TextureAxisFromPlane(CPlane *pPlane, vec3_t vXV, vec3_t vYV)
{
	int		nBestAxis;
	float	fDot,fBest;
	int		i;
	
	fBest = 0;
	nBestAxis = 0;
	
	for(i = 0; i < 6; i++)
	{
		fDot = DotProduct (pPlane->m_vNormal, g_vBaseAxis[i*3]);
		if (fDot > fBest)
		{
			fBest = fDot;
			nBestAxis = i;
		}
	}
	
	VectorCopy(g_vBaseAxis[nBestAxis*3+1], vXV);
	VectorCopy(g_vBaseAxis[nBestAxis*3+2], vYV);
}

// Makes texture vecs.
void CFace::MakeTVecs(vec3_t vVecs[2], vec3_t vTVecs[2], vec_t vScales[2], vec_t fRot)
{
	int i, j;
	int	sv, tv;
	float ns, nt;
	float ang, sinv, cosv;
	vec3_t vTemp[2];

	if (vScales[0] == 0.0F)
		vScales[0] = 1.0F;
	if (vScales[1] == 0.0F)
		vScales[1] = 1.0F;

	// rotate axis
	if (fRot == 0)
		{ sinv = 0 ; cosv = 1; }
	else if (fRot == 90)
		{ sinv = 1 ; cosv = 0; }
	else if (fRot == 180)
		{ sinv = 0 ; cosv = -1; }
	else if (fRot == 270)
		{ sinv = -1 ; cosv = 0; }
	else
	{	
		ang = fRot / 180 * PI;
		sinv = (float)sin(ang);
		cosv = (float)cos(ang);
	}

	if (vVecs[0][0])
		sv = 0;
	else if (vVecs[0][1])
		sv = 1;
	else
		sv = 2;
				
	if (vVecs[1][0])
		tv = 0;
	else if (vVecs[1][1])
		tv = 1;
	else
		tv = 2;
	
	for (i=0 ; i<2 ; i++)
	{
		ns = cosv * vVecs[i][sv] - sinv * vVecs[i][tv];
		nt = sinv * vVecs[i][sv] + cosv * vVecs[i][tv];
		vTemp[i][sv] = ns;
		vTemp[i][tv] = nt;
	}
	
	for (i=0 ; i<2 ; i++)
		for (j=0 ; j<3 ; j++)
			vTVecs[i][j] = vTemp[i][j] / vScales[i];
}

// Projects a texture based on the UV vVecs onto the given points.
void CFace::ProjectTexture(vec3_t vVecs[2], vec_t vShifts[2], int nNumCoords, vec3_t* vCoords, vec_t** vTexCoords)
{
	int i;

	// Compute texture coords.
	for(i = 0; i < nNumCoords; i++)
	{
		vTexCoords[i][0] = DotProduct(vCoords[i], vVecs[0]) + vShifts[0];
		vTexCoords[i][1] = DotProduct(vCoords[i], vVecs[1]) + vShifts[1];
	}
}

// Generate the faces texture.
void CFace::GenerateTextureCoords()
{
	int i, j;
	vec3_t vTVecs[2];
	vec3_t vVecs[2];
	int		sv, tv;
	float	ang, sinv, cosv;
	float	ns, nt;
	float fNewX, fNewY;

	if(m_nNumTexCoords == 0)
		AllocTexCoords(m_nNumPoints);

	// Recompute the base axis.
	TextureAxisFromPlane(&m_Plane, vVecs[0], vVecs[1]);

	if (m_vTexScales[0] == 0.0F)
		m_vTexScales[0] = 1.0F;
	if (m_vTexScales[1] == 0.0F)
		m_vTexScales[1] = 1.0F;


	// rotate axis
	if (m_vRotation == 0)
		{ sinv = 0 ; cosv = 1; }
	else if (m_vRotation == 90)
		{ sinv = 1 ; cosv = 0; }
	else if (m_vRotation == 180)
		{ sinv = 0 ; cosv = -1; }
	else if (m_vRotation == 270)
		{ sinv = -1 ; cosv = 0; }
	else
	{	
		ang = m_vRotation / 180 * PI;
		sinv = (float)sin(ang);
		cosv = (float)cos(ang);
	}

	if (vVecs[0][0])
		sv = 0;
	else if (vVecs[0][1])
		sv = 1;
	else
		sv = 2;
				
	if (vVecs[1][0])
		tv = 0;
	else if (vVecs[1][1])
		tv = 1;
	else
		tv = 2;
					
	for (i=0 ; i<2 ; i++)
	{
		ns = cosv * vVecs[i][sv] - sinv * vVecs[i][tv];
		nt = sinv * vVecs[i][sv] + cosv * vVecs[i][tv];
		vVecs[i][sv] = ns;
		vVecs[i][tv] = nt;
	}

	for (i=0 ; i<2 ; i++)
		for (j=0 ; j<3 ; j++)
			vTVecs[i][j] = vVecs[i][j] / m_vTexScales[i];


	bool bTexLock = (GetMapManager()->GetModePtr()->GetFlags()&FLAG_TEXLOCK) && m_bTexInit;
	if(bTexLock)
	{
		fNewX = DotProduct(m_vPoints[0], vTVecs[0]);
		fNewY = DotProduct(m_vPoints[0], vTVecs[1]);

		m_vTexShifts[0] = m_vTexCoords[0][0] - fNewX;
		m_vTexShifts[1] = m_vTexCoords[0][1] - fNewY;
	}

	// Compute texture coords.
	for(i = 0; i < m_nNumTexCoords; i++)
	{
		m_vTexCoords[i][0] = DotProduct(m_vPoints[i], vTVecs[0]) + m_vTexShifts[0];
		m_vTexCoords[i][1] = DotProduct(m_vPoints[i], vTVecs[1]) + m_vTexShifts[1];
	}

	m_bTexInit = true;

}
/*
// Generate the faces texture.
void CFace::GenerateTextureCoords()
{
	vec3_t vVecs[2], vTVecs[2];

	if(m_nNumTexCoords == 0)
		AllocTexCoords(m_nNumPoints);

	// Recompute the base axis.
	TextureAxisFromPlane(&m_Plane, vVecs[0], vVecs[1]);
	
	bool bTexLock = false;//(GetMapManager()->GetModePtr()->GetFlags()&FLAG_TEXLOCK) && m_bTexInit;
	
	*/
	/*if(bTexLock)
	{
		fNewX = DotProduct(m_vPoints[0], vTVecs[0]);
		fNewY = DotProduct(m_vPoints[0], vTVecs[1]);

		m_vTexShifts[0] = m_vTexCoords[0][0] - fNewX;
		m_vTexShifts[1] = m_vTexCoords[0][1] - fNewY;
	}*/
	/*
	if(bTexLock)
	{
		int i, k;
		vec_t vShifts[2];

		vShifts[0] = vShifts[1] = 0.0f;

		// Store vec values.
		vec_t** vTexs = new vec_t*[m_nNumPoints];
		for(i = 0; i < m_nNumPoints; i++)
		{
			vTexs[i] = new vec_t[2];
			for(k = 0; k < 2; k++)
				vTexs[i][k] = m_vTexCoords[i][k];
		}

		
		// Make new vecs.
		MakeTVecs(vVecs, vTVecs, m_vTexScales, m_vRotation);
		ProjectTexture(vTVecs, m_vTexShifts, m_nNumPoints, m_vPoints, m_vTexCoords);

		// Find a point tha moved.
		for(i = 0; i < m_nNumPoints; i++)
		{
			for(k = 0; k < 2; k++)
			{
				if(m_vTexCoords[i][k] > vTexs[i][k]+0.01 || m_vTexCoords[i][k] < vTexs[i][k]-0.01)
					break;
			}

			if(k != 2)
				break;
		}

		// Found one.
		if(i != m_nNumPoints)
		{
			// Adjust rotation based on change.
			float fX = vTexs[i][0] - m_vTexCoords[i][0];
			float fY = vTexs[i][1] - m_vTexCoords[i][1];
			
			// Increase rotation.
			m_vRotation += (float)atan2(fY, fX);
*/
			// Retexture and adjust shifts.
			/*MakeTVecs(vVecs, vTVecs, m_vTexScales, m_vRotation);
			ProjectTexture(vTVecs, m_vTexShifts, m_nNumPoints, m_vPoints, m_vTexCoords);

			// Find a point tha moved.
			for(i = 0; i < m_nNumPoints; i++)
			{
				for(k = 0; k < 2; k++)
				{
					if(m_vTexCoords[i][k] > vTexs[i][k]+0.01 || m_vTexCoords[i][k] < vTexs[i][k]-0.01)
						break;
				}

				if(k != 2)
					break;
			}

			// Found one.
			if(i != m_nNumPoints)
			{
				// Adjust shifts.
				for(k = 0; k < 2; k++)
					m_vTexShifts[k] += vTexs[i][k] - m_vTexCoords[i][k];
			}*/

			// Perform normal texturing.
			// Free temp texs.
/*			for(i = 0; i < m_nNumPoints; i++)
				delete[] vTexs[i];
			delete vTexs;
		}
	}
	
	MakeTVecs(vVecs, vTVecs, m_vTexScales, m_vRotation);
	ProjectTexture(vTVecs, m_vTexShifts, m_nNumPoints, m_vPoints, m_vTexCoords);

	m_bTexInit = true;
}*/

// Free's the texture coords.
void CFace::FreeTexCoords()
{
	int i;

	if(m_vTexCoords == NULL)
		return;

	for(i = 0; i < m_nNumTexCoords; i++)
	{
		if(m_vTexCoords[i] != NULL)
			delete[] m_vTexCoords[i];
		m_vTexCoords[i] = NULL;
	}

	delete m_vTexCoords;
	m_vTexCoords = NULL;
	m_nNumTexCoords = 0;
}

// Allocates texture coords.
bool CFace::AllocTexCoords(int nNumTexCoords)
{
	int i;

	FreeTexCoords();
	m_vTexCoords = new vec_t*[nNumTexCoords];
	if(m_vTexCoords == NULL)
		return false;

	for(i = 0; i < nNumTexCoords; i++)
	{
		m_vTexCoords[i] = new vec_t[2];
		if(m_vTexCoords[i] == NULL)
			return false;
	}

	m_nNumTexCoords = nNumTexCoords;
	return true;
}

// Copies tex coords.
void CFace::CopyTexCoords(int nNumTexCoords, vec_t** vTexCoords)
{
	int i, k;
	
	AllocTexCoords(nNumTexCoords);
	for(i = 0; i < nNumTexCoords; i++)
		for(k = 0; k < 2; k++)
			m_vTexCoords[i][k] = vTexCoords[i][k];
}
