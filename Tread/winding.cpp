// WINDING.CPP
// Copyright © 1999 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#include "stdafx.h"
#include <stdio.h>
#include "MathLib.h"
#include "Winding.h"
#include "TreadSys.h"
#include "Tread3DDoc.h"
#include "MapView.h"
#include "RenderAPI.h"
#include "Geometry.h"
#include "futils.h"

#define ON_EPSILON	0.01F
#define CUT_EPSILON	0.01F
#define BOGUS_RANGE	655535.0F

_T_EXPORT vec_t CWinding::m_sfDX = 0.0F;
_T_EXPORT vec_t CWinding::m_sfDY = 0.0F;

// Constructors.
CWinding::CWinding() 
	: CBaseGroupableObject()
	, CBaseTransformableObject()
{
	m_nNumPoints = 0; 
	m_vPoints = NULL;
}

CWinding::CWinding(int nNumPoints)
	: CBaseGroupableObject()
	, CBaseTransformableObject()
{
	m_nNumPoints = 0; 
	m_vPoints = NULL;
	if(nNumPoints > MAX_WINDING_POINTS)
	{
		Error("CWinding::CWinding(int nNumPoints): Max Points.");
		return;
	}
	m_nNumPoints = nNumPoints;
	m_vPoints = new vec3_t[nNumPoints];
	if(m_vPoints == NULL)
	{
		m_nNumPoints = 0;
		Error("CWinding::CWinding(int nNumPoints): Unable to allocate memory.");
	}
}

CWinding::CWinding(int nNumPoints, vec3_t* vPoints)
	: CBaseGroupableObject()
	, CBaseTransformableObject()
{
	m_nNumPoints = 0; 
	m_vPoints = NULL;
	CWinding::CopyPoints(nNumPoints, vPoints);
}

CWinding::CWinding(const CWinding& WindingToCopy)
	: CBaseGroupableObject(WindingToCopy)
	, CBaseTransformableObject(WindingToCopy)
{
	m_nNumPoints = 0; 
	m_vPoints = NULL;
	CWinding::CopyPoints(WindingToCopy.m_nNumPoints, WindingToCopy.m_vPoints);
}

CWinding::~CWinding()
{
	FreePoints();
}

void* CWinding::GetInterface(int nInterface)
{
	switch(nInterface)
	{
	case I_GROUPABLE:
		return (CBaseGroupableObject*)this;
	case I_TRANSFORMABLE:
		return (CBaseTransformableObject*)this;
	}

	return NULL;
}

bool CWinding::In2DRange(vec3_t vMins, vec3_t vMaxs, int nView)
{
	int i, k;

	for(i = 0; i < m_nNumPoints; i++)
	{
		for(k = VEC_X; k <= VEC_Y; k++)
		{
			if(m_vPoints[i][VMI(nView, k)] < vMins[VMI(nView, k)] ||
				m_vPoints[i][VMI(nView, k)] > vMaxs[VMI(nView, k)])
					break;
		}

		if(k == VEC_Z)
			return true;
	}

	return false;
}

// Classifies winding based on plane.
int CWinding::ClassifyWinding(CPlane* pPlane)
{
	return ClassifyWinding(pPlane, ON_EPSILON);
}

int  CWinding::ClassifyWinding(CPlane* pPlane, float fEpsilon)
{
	int i;
	int nFront=0, nBack=0, nOn=0;
	float fDist;

	// Classify.
	for(i = 0; i < m_nNumPoints; i++)
	{
		fDist = DotProduct(m_vPoints[i], pPlane->m_vNormal) - pPlane->m_vDist;
		if(fDist < -fEpsilon)
			nBack++;
		else if(fDist > fEpsilon)
			nFront++;
		else
			nOn++;
	}

	if(nBack > 0 && nFront > 0)
		return PSIDE_BOTH;

	if(nBack == 0 && nFront > 0)
		return PSIDE_FRONT;
	if(nFront == 0 && nBack > 0)
		return PSIDE_BACK;

	return PSIDE_ON;
}

int  CWinding::BiasClassifyWinding(CPlane* pPlane, float fFront)
{
	int i;
	int nFront=0, nBack=0, nOn=0;
	float fDist;

	// Classify.
	for(i = 0; i < m_nNumPoints; i++)
	{
		fDist = DotProduct(m_vPoints[i], pPlane->m_vNormal) - pPlane->m_vDist;
		if(fDist < fFront)
			nBack++;
		else
			nFront++;
	}

	if(nBack > 0 && nFront > 0)
		return PSIDE_BOTH;

	if(nBack == 0 && nFront > 0)
		return PSIDE_FRONT;

	return PSIDE_BACK;
}

// Returns true if the winding crosses the plane.
bool CWinding::WindingCrossesPlane(CPlane* pPlane)
{
	return ClassifyWinding(pPlane) == PSIDE_BOTH;
}

// Inserts face geamory into range.
void CWinding::InsertIntoRange(vec3_t vMins, vec3_t vMaxs)
{
	int i;

	for(i = 0; i < m_nNumPoints; i++)
		InsertRange(m_vPoints[i], vMins, vMaxs);
}

// Computes a faces plane.
// A face goes counter clockwise from the front.
void CWinding::ComputePlane()
{
	int		i;
	vec3_t	v1, v2;
	vec3_t vec3_origin = {0.0F, 0.0F, 0.0F};

	if(m_nNumPoints < 3)
	{
		Warning("CWinding::ComputePlane(): Degenerate face w/ less than 3 points.\n");
		return;
	}

	// Check for collinear points.
	for(i = 0; i < m_nNumPoints; i++)
	{
		VectorSubtract(m_vPoints[i], m_vPoints[(i+1)%m_nNumPoints], v1);
		VectorSubtract(m_vPoints[(i+2)%m_nNumPoints], m_vPoints[(i+1)%m_nNumPoints], v2);

		// Either of them are zero we have a collapsed edge which is bad.
		if(VectorCompare(v1, vec3_origin, 0.001F) || VectorCompare(v2, vec3_origin, 0.001F))
			continue;

		CalcOrthoVector(v1, v2, m_Plane.m_vNormal);
		if(!VectorCompare(m_Plane.m_vNormal, vec3_origin, 0.01F))
			break;
	}

	if(i >= m_nNumPoints)
		Warning("Face with no normal!\n");

	m_Plane.m_vDist = DotProduct(m_Plane.m_vNormal, m_vPoints[0]);
	m_nPlaneType = ClassifyVector(m_Plane.m_vNormal);
}

// Frees points.
void CWinding::FreePoints()
{
	if(m_vPoints != NULL)
		delete[] m_vPoints;
	m_vPoints = NULL;
	m_nNumPoints = 0;
}

// Copies the points array passed.
void CWinding::CopyPoints(int nNumPoints, vec3_t* vPoints)
{
	FreePoints();
	if(nNumPoints > MAX_WINDING_POINTS)
	{
		Error("CWinding::CopyPoints(): Max Points");
		return;
	}
	m_nNumPoints = nNumPoints;
	m_vPoints = new vec3_t[nNumPoints];
	if(m_vPoints == NULL)
	{
		Error("CWinding::CopyPoints(): unable to allocate memory.");
		FreePoints();
		return;
	}
	int i;
	for(i = 0; i < m_nNumPoints; i++)
		VectorCopy(vPoints[i], m_vPoints[i]);
	ComputePlane();
	ComputeOrigin();
}

// Flips the face.
void CWinding::Flip()
{
	// Reverse all the points.
	int i, k;
	vec3_t vTempPoints[MAX_WINDING_POINTS];

	if(m_nNumPoints > MAX_WINDING_POINTS)
	{
		Error("CWinding::Flip(): too many points on face.");
		return;
	}

	for(k = 0, i = m_nNumPoints-1; k < m_nNumPoints; k++, i--)
		VectorCopy(m_vPoints[i], vTempPoints[k]);
	for(i = 0; i < m_nNumPoints; i++)
		VectorCopy(vTempPoints[i], m_vPoints[i]);

	m_Plane.Flip();
}

// Checks the winding.
bool CWinding::CheckWinding()
{
	int		i;
	vec3_t	v1, v2, vc;
	vec3_t vec3_origin = {0.0F, 0.0F, 0.0F};

	if(m_nNumPoints < 3)
		return false;
	
	// Check for collinear points.
	for(i = 0; i < m_nNumPoints; i++)
	{
		VectorSubtract(m_vPoints[i], m_vPoints[(i+1)%m_nNumPoints], v1);
		VectorSubtract(m_vPoints[(i+2)%m_nNumPoints], m_vPoints[(i+1)%m_nNumPoints], v2);

		// Either of them are zero we have a collapsed edge which is bad.
		if(VectorCompare(v1, vec3_origin, 0.001F) || VectorCompare(v2, vec3_origin, 0.001F))
			continue;

		CalcOrthoVector(v1, v2, vc);
		if(!VectorCompare(vc, vec3_origin, 0.01F))
			break;
	}

	if(i >= m_nNumPoints)
		return false;

	return true;
}

// Creates a winding from a plane.
CWinding* CWinding::BaseWindingFromPlane(CPlane* pPlane)
{

	int		i, x;
	vec_t	vMax, v;
	vec3_t	vOrg, vRight, vUp;
	CWinding* pWinding;
	vec3_t vec3_origin = {0.0F, 0.0F, 0.0F};

	// Find the major axis
	vMax = -BOGUS_RANGE;
	x = -1;
	for (i = 0 ; i < 3; i++)
	{
		v = (float)fabs(pPlane->m_vNormal[i]);
		if (v > vMax)
		{
			x = i;
			vMax = v;
		}
	}

	// Didn't find the plane?
	if (x == -1)
	{
		Warning("CWinding::BaseWindingFromPlane(): Unable to find major axis for plane\n");
		return(NULL);
	}
		
	VectorCopy (vec3_origin, vUp);	

	switch (x)
	{
	case 0:
	case 1:
		vUp[2] = 1;
		break;		
	case 2:
		vUp[0] = 1;
		break;		
	}

	v = DotProduct(vUp, pPlane->m_vNormal);
	VectorMA (vUp, -v, pPlane->m_vNormal, vUp);
	VectorNormalize (vUp);
		
	VectorScale(pPlane->m_vNormal, pPlane->m_vDist, vOrg);
	
	CrossProduct(vUp, pPlane->m_vNormal, vRight);
	
	VectorScale(vUp, BOGUS_RANGE, vUp);
	VectorScale(vRight, BOGUS_RANGE, vRight);

	
	// Project a really big	axis aligned box onto the plane
	pWinding = new CWinding(4);
	
	VectorSubtract(vOrg, vRight, pWinding->m_vPoints[0]);
	VectorAdd(pWinding->m_vPoints[0], vUp, pWinding->m_vPoints[0]);
	
	VectorAdd(vOrg, vRight, pWinding->m_vPoints[1]);
	VectorAdd(pWinding->m_vPoints[1], vUp, pWinding->m_vPoints[1]);
	
	VectorAdd(vOrg, vRight, pWinding->m_vPoints[2]);
	VectorSubtract(pWinding->m_vPoints[2], vUp, pWinding->m_vPoints[2]);
	
	VectorSubtract(vOrg, vRight, pWinding->m_vPoints[3]);
	VectorSubtract(pWinding->m_vPoints[3], vUp, pWinding->m_vPoints[3]);
		
	pWinding->m_Plane = *pPlane;
	pWinding->ComputeOrigin();

	return(pWinding);	
}

// Splits the winding and only returns one side.
CWinding* CWinding::ChopWindingByPlane(CPlane* pPlane, bool bReturnFront)
{
	CWinding* pFront, *pBack;

	SplitWindingByPlane(pPlane, &pFront, &pBack);
	if(bReturnFront)
		return pFront;
	return pBack;
}

// Splits a winding by a plane.
void CWinding::SplitWindingByPlane(CPlane* pPlane, CWinding** pFront, CWinding** pBack)
{
	int i, j;
	int nSides[MAX_WINDING_POINTS+4];
	int nCounts[3];
	int	nNumFront, nNumBack;
	int nMaxPts;
	vec_t fDot;
	vec_t fDists[MAX_WINDING_POINTS+4];
	vec_t* vP1, *vP2;
	vec3_t vMids;
	vec3_t* vFront, *vBack;


	*pFront = *pBack = NULL;
	memset(nCounts, 0, sizeof(nCounts));

	// Classify each point based on the plane.
	for(i = 0; i < this->m_nNumPoints; i++)
	{
		fDot = DotProduct(this->m_vPoints[i], pPlane->m_vNormal) - pPlane->m_vDist;
		if(fDot > CUT_EPSILON)
			nSides[i] = PSIDE_FRONT;
		else if(fDot < -CUT_EPSILON)
			nSides[i] = PSIDE_BACK;
		else
			nSides[i] = PSIDE_ON;

		fDists[i] = fDot;	// Store dot for later.

		// Increment side counts.
		nCounts[nSides[i]]++;
	}
	
	// Copy.
	nSides[i] = nSides[0];
	fDists[i] = fDists[0];

	// How many do we have?
	if(!nCounts[PSIDE_FRONT])	// No front sides?
	{
		*pBack = new CWinding(*this);
		return;
	}
	else if(!nCounts[PSIDE_BACK])	// No back sides?
	{
		*pFront = new CWinding(*this);
		return;
	}

	nMaxPts = this->m_nNumPoints+4;
	nNumFront = nNumBack = 0;

	vFront = new vec3_t[nMaxPts];
	vBack  = new vec3_t[nMaxPts];
	if(vFront == NULL || vBack == NULL)
	{
		Error("CWinding::SplitWindingByPlane(): Unable to allocate memory.");
		return;
	}

	// Generate faces.
	for(i = 0; i < this->m_nNumPoints; i++)
	{
			// This really shouldn't happen.
		if(nNumFront > nMaxPts || nNumBack > nMaxPts)
		{
			Error("CWinding::SplitWindingByPlane(): Points exceeded estimate.");
			return;
		}

		vP1 = this->m_vPoints[i];
		
		if(nSides[i] == PSIDE_FRONT)
			VectorCopy(vP1, vFront[nNumFront++]);
		else if(nSides[i] == PSIDE_BACK)
			VectorCopy(vP1, vBack[nNumBack++]);
		else if(nSides[i] == PSIDE_ON)
		{
			VectorCopy(vP1, vFront[nNumFront++]);
			VectorCopy(vP1, vBack[nNumBack++]);
			continue;
		}

		// Do we need to generate a split?
		if(nSides[i+1] == PSIDE_ON || nSides[i+1] == nSides[i])
			continue;

		// Generate a split point.
		vP2 = this->m_vPoints[(i+1)%this->m_nNumPoints];

		// Compute scalar.
		fDot = fDists[i] / (fDists[i] - fDists[i+1]);

		for(j = 0; j < 3; j++)
		{
			// Avoid roundoffs.
			if(pPlane->m_vNormal[j] == 1.0F)
				vMids[j] = pPlane->m_vDist;
			else if(pPlane->m_vNormal[j] == -1.0F)
				vMids[j] = -pPlane->m_vDist;
			else
				vMids[j] = vP1[j] + fDot * (vP2[j] - vP1[j]);

		}

		// Copy to both faces.
		VectorCopy(vMids, vFront[nNumFront++]);
		VectorCopy(vMids, vBack[nNumBack++]);

	}

	// Create faces.
	if(nNumFront > 0)
	{
		*pFront = new CWinding(nNumFront, vFront);
		(*pFront)->m_Plane = this->m_Plane;
		(*pFront)->ComputeOrigin();
	}
	if(nNumBack > 0)
	{
		*pBack = new CWinding(nNumBack, vBack);
		/*if((*pBack)->CheckWinding() == false)
		{
			delete *pBack;
			*pBack = NULL;
		}
		else
		{*/
		(*pBack)->m_Plane = this->m_Plane;
		(*pBack)->ComputeOrigin();
		//}
	}

	delete[] vFront;
	delete[] vBack;
	
}

// Writes a winding.
bool CWinding::WriteObject(CFile &hFile, int nVersion)
{
	int i, k;

	// Write the points.
	WriteInt(hFile, m_nNumPoints);

	for(i = 0; i< m_nNumPoints; i++)
		for(k = 0; k < 3; k++)
			WriteFloat(hFile, m_vPoints[i][k]);

	return true;
}

// Read the object.
bool CWinding::ReadObject(CFile& hFile, int nVersion)
{
	int i, k;

	FreePoints();

	if(!hFile.Read(&m_nNumPoints, 4))
		return false;
	
	m_vPoints = new vec3_t[m_nNumPoints];
	if(m_vPoints == NULL)
	{
		m_nNumPoints = 0;
		return false;
	}

	for(i = 0; i < m_nNumPoints; i++)
		for(k = 0; k < 3; k++)
			if(!ReadFloat(hFile, m_vPoints[i][k]))
				return false;

	ComputePlane();
	ComputeOrigin();

	return true;
}

// Computes the origin of the winding.
void CWinding::ComputeOrigin()
{
	int i;
	
	m_vOrigin[0] = 0.0F; m_vOrigin[1] = 0.0F; m_vOrigin[2] = 0.0F;

	for(i = 0; i < m_nNumPoints; i++)
		VectorAdd(m_vOrigin, m_vPoints[i], m_vOrigin);

	VectorScale(m_vOrigin, 1.0F/(float)m_nNumPoints, m_vOrigin);
}

void CWinding::Reflect(int nAxis, vec3_t vFlipOrigin)
{
	int i;
	vec_t* p;
	
	// The distance should be mirrored to the opposite side of the origin.
	for(i = 0; i < m_nNumPoints; i++)
	{
		p = m_vPoints[i];
		p[nAxis] = vFlipOrigin[nAxis] + (vFlipOrigin[nAxis] - p[nAxis]);
	}

	CWinding::Flip();
}

// Scale the winding.
void CWinding::Scale(vec3_t vScale, vec3_t vOrigin)
{
	int i;
	vec3_t vInvOrigin;

	VectorScale(vOrigin, -1.0F, vInvOrigin);
	CWinding::Translate(vInvOrigin);

	for(i = 0; i < m_nNumPoints; i++)
		VectorMultiply(m_vPoints[i], vScale, m_vPoints[i]);

	CWinding::Translate(vOrigin);

	ComputePlane();
}

void CWinding::Scale(vec3_t vScale)
{
	vec3_t vOrigin = {0.0F, 0.0F, 0.0F};
	CWinding::Scale(vScale, vOrigin);
}

// Set's the origin of the winding.
void CWinding::SetOrigin(vec3_t vOrigin)
{
	vec3_t vDiff;

	VectorSubtract(vOrigin, m_vOrigin, vDiff);
	CWinding::Translate(vDiff);
	VectorCopy(vOrigin, m_vOrigin);
	ComputePlane();
}

// Translates the winding.
void CWinding::Translate(vec3_t vTranslation)
{
	int i;

	for(i = 0; i < m_nNumPoints; i++)
		VectorAdd(m_vPoints[i], vTranslation, m_vPoints[i]);

	VectorAdd(m_vOrigin, vTranslation, m_vOrigin);
	ComputePlane();
}

// Rotates the object.
void CWinding::Rotate(vec3_t vRot)
{
	CWinding::Rotate(m_vOrigin, vRot);
}

// Rotates the object about an origin.
void CWinding::Rotate(vec3_t vOrigin, vec3_t vRot)
{
	int i;
	vec3_t vNegOrigin = { -vOrigin[0], -vOrigin[1], -vOrigin[2] };

	CWinding::Translate(vNegOrigin);
	for(i = 0; i < m_nNumPoints; i++)
		VectorRotate(m_vPoints[i], vRot, m_vPoints[i]);
	CWinding::Translate(vOrigin);
	ComputeOrigin();
	ComputePlane();
}

// Make hotspots.
void CWinding::GenerateHotSpots(CHotSpotManager* pHotSpotManager, CMapView* pView)
{
	CHotSpot* pSpot;


	for(int n = 0; n < m_nNumPoints; n++)
	{
		// Make a hotpot for the base
		pSpot = new CHotSpot();
		if(pSpot == NULL)
			Fatal("Memory allocation failed in CWinding::GenerateHotSpots");

		pSpot->m_nClass = OBJECT_CLASS_WINDING;
		pSpot->m_fMX = m_vPoints[n][VI(VEC_X, pView)];
		pSpot->m_fMY = m_vPoints[n][VI(VEC_Y, pView)];
		pSpot->ComputeWindowXY(pView);
		pSpot->m_nDrawFlags = HSDF_SQUARE;
		pSpot->m_nCursor = TC_IDC_CROSS;
		pSpot->m_nDragRestrictions = HSDR_NONE;
		pSpot->m_pData = m_vPoints[n];
		pSpot->m_pEventResponder = this;
		VectorCopy(m_vPoints[n], pSpot->m_vPt);
		pHotSpotManager->AddHotSpot(pSpot);
	}
}

void CWinding::OnHotSpotDrag(CWindowEventManager* pEventManager, CHotSpotManager* pHotSpotManager, CHotSpot* pHotSpot, CMapView* pView, CTread3DDoc* pDocument, int nMouseX, int nMouseY, int nButtons, int nFlags)
{
	// Snap?
	if(((nFlags&HEF_GROUP)==0) || ((nFlags&HEF_HEAD)==HEF_HEAD))
	{
		if(pDocument->GetGridSnap())
		{
			m_fX = G_SnapFloatDelta(WX_TO_MX(nMouseX, pView), m_fX, pDocument->GetGridSize());
			m_fY = G_SnapFloatDelta(WY_TO_MY(nMouseY, pView), m_fY, pDocument->GetGridSize());
		}
		else
		{
			m_fX = WX_TO_MX(nMouseX, pView);
			m_fY = WY_TO_MY(nMouseY, pView);
		}
	}

	if(nFlags&HEF_GROUP)
	{
		if(nFlags&HEF_HEAD)
		{
			vec_t* vP;
			vP = (vec_t*)pHotSpot->m_pData;
			vP[VI(VEC_X, pView)] = m_fX;
			vP[VI(VEC_Y, pView)] = m_fY;
			m_sfDX = m_fX - pHotSpot->m_fMX;
			m_sfDY = m_fY - pHotSpot->m_fMY;
			pHotSpot->m_fMX = m_fX;
			pHotSpot->m_fMY = m_fY;
		}
		else
		{
			// Translate;
			vec_t* vP;
			vP = (vec_t*)pHotSpot->m_pData;
			vP[VI(VEC_X, pView)] += m_sfDX;
			vP[VI(VEC_Y, pView)] += m_sfDY;
			pHotSpot->m_fMX += m_sfDX;
			pHotSpot->m_fMY += m_sfDY;
		}

		if(nFlags&HEF_TAIL)
		{
			int nFlags = pView->m_nView;
			if(pDocument->GetGlobalRenderFlags()&RF_REALTIME3D)
				nFlags |= VIEW_3D;
			pDocument->UpdateWindows(nFlags);
		}
	}
	else
	{
		CHotSpot* pSpot;
		pHotSpotManager->RestoreIdenticalSearch();
		for(pSpot = pHotSpotManager->GetNextIdenticalHotSpot(NULL, NULL, 0); pSpot != NULL; pSpot = pHotSpotManager->GetNextIdenticalHotSpot(NULL, NULL, 0))
		{
			vec_t* vP;
			vP = (vec_t*)pSpot->m_pData;
			vP[VI(VEC_X, pView)] = m_fX;
			vP[VI(VEC_Y, pView)] = m_fY;
			pSpot->m_fMX = m_fX;
			pSpot->m_fMY = m_fY;
		}

		int nFlags;

		if(pDocument->GetUpdateAllViews())
			nFlags = FLAT_VIEWS;
		else
			nFlags = pView->m_nView;

		if(pDocument->GetGlobalRenderFlags()&RF_REALTIME3D)
			nFlags |= VIEW_3D;
		pDocument->UpdateWindows(nFlags);
	}
}

void CWinding::OnHotSpotMouseDown(CWindowEventManager* pEventManager, CHotSpotManager* pHotSpotManager, CHotSpot* pHotSpot, CMapView* pView, CTread3DDoc* pDocument, int nMouseX, int nMouseY, int nButtons, int nHSFlags)
{
	pEventManager->EnableAutoScroll(pHotSpotManager);

	if(pDocument->GetGridSnap())
	{
		m_fX = G_SnapFloat(WX_TO_MX(nMouseX, pView), pDocument->GetGridSize());
		m_fY = G_SnapFloat(WY_TO_MY(nMouseY, pView), pDocument->GetGridSize());
	}

	int nFlags = ISF_WINX|ISF_WINY|ISF_CLASS;

	if(nButtons&EF_CONTROL)
		nFlags |= ISF_PTZ;

	pHotSpotManager->InitIdenticalSearch();
	pHotSpotManager->SaveIdenticalSearch(pHotSpot, pView, nFlags);

	pDocument->AddGlobalRenderFlags(RF_FAST);
	pDocument->UpdateWindows(pView->m_nView);
}

void CWinding::OnHotSpotMouseUp(CWindowEventManager* pEventManager, CHotSpotManager* pHotSpotManager, CHotSpot* pHotSpot, CMapView* pView, CTread3DDoc* pDocument, int nMouseX, int nMouseY, int nButtons, int nFlags)
{
	pEventManager->EnableAutoScroll(pHotSpotManager, false);
	ComputeOrigin();
	ComputePlane();
	if(nFlags == HEF_NONE || ((nFlags&(HEF_GROUP|HEF_TAIL))==(HEF_GROUP|HEF_TAIL)))
	{
		pDocument->RemoveGlobalRenderFlags(RF_FAST);
		pDocument->BuildSelectionBBox();
		pDocument->BuildHotSpots(FLAT_VIEWS&(~pView->m_nView));
		pDocument->UpdateWindows();
	}
}

