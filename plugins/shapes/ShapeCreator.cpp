// SHAPECREATOR.CPP
// Copyright © 2000 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#include "stdafx.h"
#include "../TreadAPI.h"
#include "ShapeCreator.h"

// Creates an arch that runs along major axis.
CObjectGroup* CShapeCreator::CreateArch(int nMajorAxis, int nNumSides, float fRadius, float fWidth, float fDepth)
{
	int i, j;
	int nSwingAxis, nNumPoints;
	float fRad, fRadStep;
	vec3_t* pA[4];
	vec3_t vPt, vRot, vF[4];
	CObjectGroup* pGrp;
	CQBrush* pBrush;
	CFace* pFace;

	if(nNumSides < 2)
	{
		Warning("ShapeCreator::CreateArch(): invalid numsides\n");
		return NULL;
	}

	if(nMajorAxis < VEC_X || nMajorAxis > VEC_Z)
	{
		Warning("ShapeCreator::CreateArch(): bad vector axis\n");
		return NULL;
	}

	switch(nMajorAxis)
	{
	case VEC_X:
		nSwingAxis = VEC_Y; break;
	case VEC_Y:
	case VEC_Z:
		nSwingAxis = VEC_X; break;
	}

	nNumPoints = nNumSides + 1;

	fRadStep = PI/(nNumPoints-1);

	// Create base arch hulls.
	for(i = 0; i < 4; i++)
	{
		pA[i] = new vec3_t[nNumPoints];
		VectorClear(vPt);
		switch(i)
		{
		case 0: 
			vPt[nSwingAxis] = fRadius;
			break;
		case 1:
			vPt[nSwingAxis] = fRadius+fWidth; 
			break;
		case 2:
			vPt[nMajorAxis] = fDepth;
			vPt[nSwingAxis] = fRadius+fWidth; 
			break;
		case 3:
			vPt[nMajorAxis] = fDepth;
			vPt[nSwingAxis] = fRadius; 
			break;
		}

		fRad = fRadStep;
		VectorCopy(vPt, pA[i][0]);

		for(j = 1; j < nNumPoints; j++)
		{
			VectorClear(vRot);
			vRot[nMajorAxis] = fRad;
			VectorRotate(vPt, vRot, pA[i][j]);
			fRad += fRadStep;
		}
	}

	// Create brushes from arch hulls.
	pGrp = (CObjectGroup*)NewBaseObject(OBJECT_CLASS_GROUP);
	for(i = 0; i < nNumSides; i++)
	{
		pBrush = (CQBrush*)NewBaseObject(OBJECT_CLASS_BRUSH);

		// Each brush has 6 faces.
		for(j = 0; j < 6; j++)
		{
			switch(j)
			{
			case 0:

				VectorCopy(pA[0][i], vF[0]);
				VectorCopy(pA[1][i], vF[1]);
				VectorCopy(pA[1][i+1], vF[2]);
				VectorCopy(pA[0][i+1], vF[3]);
				break;

			case 1:

				VectorCopy(pA[1][i], vF[0]);
				VectorCopy(pA[2][i], vF[1]);
				VectorCopy(pA[2][i+1], vF[2]);
				VectorCopy(pA[1][i+1], vF[3]);
				break;

			case 2:

				VectorCopy(pA[2][i], vF[0]);
				VectorCopy(pA[3][i], vF[1]);
				VectorCopy(pA[3][i+1], vF[2]);
				VectorCopy(pA[2][i+1], vF[3]);
				break;

			case 3:

				VectorCopy(pA[3][i], vF[0]);
				VectorCopy(pA[0][i], vF[1]);
				VectorCopy(pA[0][i+1], vF[2]);
				VectorCopy(pA[3][i+1], vF[3]);
				break;

			case 4:

				VectorCopy(pA[0][i+1], vF[0]);
				VectorCopy(pA[1][i+1], vF[1]);
				VectorCopy(pA[2][i+1], vF[2]);
				VectorCopy(pA[3][i+1], vF[3]);
				break;

			case 5:

				VectorCopy(pA[3][i], vF[0]);
				VectorCopy(pA[2][i], vF[1]);
				VectorCopy(pA[1][i], vF[2]);
				VectorCopy(pA[0][i], vF[3]);
				break;

			}

			pFace = (CFace*)NewBaseObject(OBJECT_CLASS_FACE);
			pFace->CopyPoints(4, vF);

			pBrush->m_pFaceList->AddItem(pFace);
			pFace->m_pParent = pBrush;
		}

		pBrush->SetTextureName(GetTexAPI()->GetCurTextureName(), true);
		pGrp->AddObject(pBrush);
	}

	// Delete the arch hulls.
	for(i = 0; i < 4; i++)
		delete[] pA[i];

	return pGrp;
}

// Creates a cylinder that runs along the MajorAxis.
CQBrush* CShapeCreator::CreateCylinder(int nMajorAxis, int nNumSides, float fRadius, float fLength)
{
	int i;
	int nSwingAxis;
	vec3_t vFlat, vRot;
	vec3_t* pFlat, *pFlat2;
	vec3_t vFacePts[4];
	float fRad;
	float fRadStep;
	CFace* pFace;
	CQBrush* pBrush;

	if(nNumSides < 3)
	{
		Warning("ShapeCreator::CreateCylinder(): invalid numsides\n");
		return NULL;
	}

	if(nMajorAxis < VEC_X || nMajorAxis > VEC_Z)
	{
		Warning("ShapeCreator::CreateCylinder(): bad vector axis\n");
		return NULL;
	}

	switch(nMajorAxis)
	{
	case VEC_X:
		nSwingAxis = VEC_Y; break;
	case VEC_Y:
	case VEC_Z:
		nSwingAxis = VEC_X; break;
	}

	// Take a base edge point, and swing it 360 degrees, to make the two flats.
	VectorClear(vFlat);
	vFlat[nSwingAxis] = fRadius;

	pFlat = new vec3_t[nNumSides];
	pFlat2 = new vec3_t[nNumSides];
	VectorCopy(vFlat, pFlat[0]);
	VectorCopy(vFlat, pFlat2[0]);
	pFlat2[0][nMajorAxis] += fLength;

	fRad = fRadStep = PI/(nNumSides/2);
	for(i = 1; i < nNumSides; i++)
	{
		VectorClear(vRot);
		vRot[nMajorAxis] = fRad;
		VectorRotate(vFlat, vRot, pFlat[i]);
		VectorCopy(pFlat[i], pFlat2[i]);
		pFlat2[i][nMajorAxis] += fLength;
		fRad += fRadStep;
	}

	pBrush = (CQBrush*)NewBaseObject(OBJECT_CLASS_BRUSH);

	// Make faces by sharing points from the 2 flat windings.
	for(i = 0; i < nNumSides; i++)
	{
		VectorCopy(pFlat[i], vFacePts[0]);
		VectorCopy(pFlat2[i], vFacePts[1]);
		VectorCopy(pFlat2[(i+1)%nNumSides], vFacePts[2]);
		VectorCopy(pFlat[(i+1)%nNumSides], vFacePts[3]);

		pFace = (CFace*)NewBaseObject(OBJECT_CLASS_FACE);
		pFace->CopyPoints(4, vFacePts);
		pBrush->m_pFaceList->AddItem(pFace);
		pFace->m_pParent = pBrush;
	}

	// Add the two top/bottom windings.
	pFace = (CFace*)NewBaseObject(OBJECT_CLASS_FACE);
	pFace->CopyPoints(nNumSides, pFlat);
	pBrush->m_pFaceList->AddItem(pFace);
	pFace->m_pParent = pBrush;

	pFace = (CFace*)NewBaseObject(OBJECT_CLASS_FACE);
	pFace->CopyPoints(nNumSides, pFlat2);
	pFace->Flip();
	pBrush->m_pFaceList->AddItem(pFace);
	pFace->m_pParent = pBrush;

	pBrush->SetTextureName(GetTexAPI()->GetCurTextureName(), true);

	delete[] pFlat;
	delete[] pFlat2;

	return pBrush;
}

// Creates a cone brush that is cenetered at fLength/2 of the major axis.
CQBrush* CShapeCreator::CreateCone(int nMajorAxis, int nNumSides, float fRadius, float fLength)
{
	int i;
	int nSwingAxis;
	vec3_t vTip, vFlat, vRot;
	vec3_t* pFlat;
	vec3_t vFacePts[3];
	float fRad;
	float fRadStep;
	CFace* pFace;
	CQBrush* pBrush;

	if(nNumSides < 3)
	{
		Warning("ShapeCreator::CreateCone(): invalid numsides\n");
		return NULL;
	}

	if(nMajorAxis < VEC_X || nMajorAxis > VEC_Z)
	{
		Warning("ShapeCreator::CreateCone(): bad vector axis\n");
		return NULL;
	}

	switch(nMajorAxis)
	{
	case VEC_X:
		nSwingAxis = VEC_Y; break;
	case VEC_Y:
	case VEC_Z:
		nSwingAxis = VEC_X; break;
	}

	// End point is down the majorAxis.
	VectorClear(vTip);
	vTip[nMajorAxis] = fabs(fLength);

	// Take a base edge point, and swing it 360 degrees, to make the flat.
	VectorClear(vFlat);
	vFlat[nSwingAxis] = fRadius;

	pFlat = new vec3_t[nNumSides];
	VectorCopy(vFlat, pFlat[0]);

	fRad = fRadStep = PI/(nNumSides/2);
	for(i = 1; i < nNumSides; i++)
	{
		VectorClear(vRot);
		vRot[nMajorAxis] = fRad;
		VectorRotate(vFlat, vRot, pFlat[i]);
		VectorClamp(pFlat[i]);
		fRad += fRadStep;
	}

	pBrush = (CQBrush*)NewBaseObject(OBJECT_CLASS_BRUSH);

	// Take the flat winding and the tip and form windings.
	// NOTE: The flat is going clockwise, but we need the faces
	// access the flat edges in reverse order to be CW from the front.
	for(i = 0; i < nNumSides; i++)
	{
		VectorCopy(pFlat[i], vFacePts[0]);
		VectorCopy(vTip, vFacePts[1]);
		VectorCopy(pFlat[(i+1)%nNumSides], vFacePts[2]);

		pFace = (CFace*)NewBaseObject(OBJECT_CLASS_FACE);
		pFace->CopyPoints(3, vFacePts);
		pBrush->m_pFaceList->AddItem(pFace);
		pFace->m_pParent = pBrush;
	}

	// Add the flat winding.
	pFace = (CFace*)NewBaseObject(OBJECT_CLASS_FACE);
	pFace->CopyPoints(nNumSides, pFlat);
	pBrush->m_pFaceList->AddItem(pFace);
	pFace->m_pParent = pBrush;

	pBrush->SetTextureName(GetTexAPI()->GetCurTextureName(), true);

	delete[] pFlat;
	return pBrush;
}
