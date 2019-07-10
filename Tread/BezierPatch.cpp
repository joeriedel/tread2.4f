// BEZIERPATCH.CPP
// Copyright © 1999 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#include "stdafx.h"
#include "Mathlib.h"
#include "Face.h"
#include "BezierPatch.h"
#include "TreadSys.h"
#include "Tread3DDoc.h"
#include "RenderAPI.h"
#include "MapView.h"
#include "Geometry.h"

#define DEFAULT_PATH_DEPTH	3

CBezierPatch::CBezierPatch() 
			: CBaseGroupableObject()
			, CBaseTransformableObject()
			, CBaseTextureableObject()
{
	m_nNumFaces = 0;
	m_bNoBuild = false;
	m_pPatchList = new CLinkedList<BEZIERMATRIX>;
	m_pFaceList = new CLinkedList<CFace>;
}

CBezierPatch::CBezierPatch(const CBezierPatch& PatchToCopy)
			: CBaseGroupableObject(PatchToCopy)
			, CBaseTransformableObject(PatchToCopy)
			, CBaseTextureableObject(PatchToCopy)
{
	m_nNumFaces = 0;
	m_bNoBuild = false;
	m_pPatchList = new CLinkedList<BEZIERMATRIX>();
	m_pFaceList = new CLinkedList<CFace>();
	
	int i, k;
	for(i = 0; i < 16; i++)
		for(k = 0; k < 3; k++)
			m_BaseMatrix.m_vPts[i][k] = PatchToCopy.m_BaseMatrix.m_vPts[i][k];

	BuildPatch(GetMapManager()->GetDocPtr()->GetCurveSubDepth());
}

CBezierPatch::~CBezierPatch()
{
	if(m_pPatchList != NULL)
		delete m_pPatchList;
	if(m_pFaceList != NULL)
		delete m_pFaceList;

	m_pPatchList = NULL;
	m_pFaceList = NULL;
}

void* CBezierPatch::GetInterface(int nInterface)
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

void CBezierPatch::Reflect(int nAxis, vec3_t vFlipOrigin)
{
	int i;
	vec_t* p;
	
	// The distance should be mirrored to the opposite side of the origin.
	for(i = 0; i < 16; i++)
	{
		p = m_BaseMatrix.m_vPts[i];
		p[nAxis] = vFlipOrigin[nAxis] + (vFlipOrigin[nAxis] - p[nAxis]);
	}

	CBezierPatch::BuildPatch(GetMapManager()->GetDocPtr()->GetCurveSubDepth());
}

void CBezierPatch::OnHotSpotDrag(CWindowEventManager* pEventManager, CHotSpotManager* pHotSpotManager, CHotSpot* pHotSpot, CMapView* pView, CTread3DDoc* pDocument, int nMouseX, int nMouseY, int nButtons, int nHSFlags)
{
	// Snap?
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

	int nFlags = ISF_WINX|ISF_WINY|ISF_CLASS|ISF_PTZ;

	// Mask out certain point types if holding buttons.
	if((nButtons&(EF_CONTROL|EF_SHIFT)) == (EF_CONTROL|EF_SHIFT))
		nFlags |= ISF_MASK|ISF_MASK1;
	else
	if(nButtons&EF_CONTROL)
		nFlags |= ISF_MASK|ISF_MASK2;
	else 
	if(nButtons&EF_SHIFT)
		nFlags |= ISF_MASK|ISF_MASK3;

	/*
	CHotSpot* pSpot;
	pHotSpotManager->InitIdenticalSearch();
	for(pSpot = pHotSpotManager->GetNextIdenticalHotSpot(pHotSpot, pView, nFlags); pSpot != NULL; pSpot = pHotSpotManager->GetNextIdenticalHotSpot(pHotSpot, pView, nFlags))
	*/
	{
		vec_t* vP;
		vP = (vec_t*)pHotSpot->m_pData;
		vP[VI(VEC_X, pView)] = m_fX;
		vP[VI(VEC_Y, pView)] = m_fY;
		pHotSpot->m_fMX = m_fX;
		pHotSpot->m_fMY = m_fY;
	}

	this->BuildPatch(pDocument->GetCurveSubDepth());

	nFlags = FLAT_VIEWS;
	if(pDocument->GetGlobalRenderFlags()&RF_REALTIME3D)
		nFlags |= VIEW_3D;
	pDocument->UpdateWindows(nFlags);
}

void CBezierPatch::OnHotSpotMouseDown(CWindowEventManager* pEventManager, CHotSpotManager* pHotSpotManager, CHotSpot* pHotSpot, CMapView* pView, CTread3DDoc* pDocument, int nMouseX, int nMouseY, int nButtons, int nHSFlags)
{
	if(pDocument->GetGridSnap())
	{
		m_fX = G_SnapFloat(WX_TO_MX(nMouseX, pView), pDocument->GetGridSize());
		m_fY = G_SnapFloat(WY_TO_MY(nMouseY, pView), pDocument->GetGridSize());
	}

	int nFlags = ISF_WINX|ISF_WINY|ISF_CLASS;

	// Mask out certain point types if holding buttons.
	if((nButtons&(EF_CONTROL|EF_SHIFT)) == (EF_CONTROL|EF_SHIFT))
		nFlags |= ISF_MASK|ISF_MASK1;
	else
	if(nButtons&EF_CONTROL)
		nFlags |= ISF_MASK|ISF_MASK2;
	else 
	if(nButtons&EF_SHIFT)
		nFlags |= ISF_MASK|ISF_MASK3;

	// Find the first spot that matches this criteria.
	CHotSpot* pSpot;
	pHotSpotManager->InitIdenticalSearch();
	pSpot = pHotSpotManager->GetNextIdenticalHotSpot(pHotSpot, pView, nFlags);
	if(pSpot != NULL)
		pHotSpotManager->SetTrackSpot(pSpot);

	pDocument->AddGlobalRenderFlags(RF_FAST/*|RF_NOHOTSPOTS*/|RF_NOSELBOX);
	pDocument->UpdateWindows(FLAT_VIEWS);
}

void CBezierPatch::OnHotSpotMouseUp(CWindowEventManager* pEventManager, CHotSpotManager* pHotSpotManager, CHotSpot* pHotSpot, CMapView* pView, CTread3DDoc* pDocument, int nMouseX, int nMouseY, int nButtons, int nFlags)
{
	pDocument->RemoveGlobalRenderFlags(RF_FAST|RF_NOHOTSPOTS|RF_NOSELBOX);
	pDocument->BuildSelectionBBox();
	pDocument->BuildHotSpots();
	pDocument->UpdateWindows();
}

void CBezierPatch::GenerateHotSpots(CHotSpotManager* pHotSpotManager, CMapView* pView)
{
	CHotSpot* pSpot;
	CHotSpot* pSpots[16];

	for(int n = 0; n < 16; n++)
	{
		// Make a hotpot for the base
		pSpot = new CHotSpot();
		if(pSpot == NULL)
			Fatal("Memory allocation failed in CBezierPatch::GenerateHotSpots");

		pSpots[n] = pSpot;
		pSpot->m_fMX = m_BaseMatrix.m_vPts[n][VI(VEC_X, pView)];
		pSpot->m_fMY = m_BaseMatrix.m_vPts[n][VI(VEC_Y, pView)];
		pSpot->ComputeWindowXY(pView);
		pSpot->m_nDrawFlags = HSDF_SQUARE;
		switch(n)
		{
		case 5:
		case 6:
			pSpot->m_nDrawFlags = HSDF_ROUND;
			break;
		case 9:
		case 10:
			pSpot->m_nDrawFlags = HSDF_ROUND;
			break;
		}
		// NOTE:
		// corners = MASK1
		// vector endpoints = MASK2
		// internal controls = MASK3
		switch(n)
		{
		case 0: pSpot->m_nFlags = ISF_MASK1; break;

		case 1: 
		case 2: pSpot->m_nFlags = ISF_MASK2; break;

		case 3: pSpot->m_nFlags = ISF_MASK1; break;
		case 4: pSpot->m_nFlags = ISF_MASK2; break;

		case 5:
		case 6: pSpot->m_nFlags = ISF_MASK3; break;

		case 7:
		case 8: pSpot->m_nFlags = ISF_MASK2; break;

		case 9:
		case 10: pSpot->m_nFlags = ISF_MASK3; break;

		case 11: pSpot->m_nFlags = ISF_MASK2; break;
		case 12: pSpot->m_nFlags = ISF_MASK1; break;

		case 13:
		case 14: pSpot->m_nFlags = ISF_MASK2; break;
		case 15: pSpot->m_nFlags = ISF_MASK1; break;
		}

		
		pSpot->m_nCursor = TC_IDC_CROSS;
		pSpot->m_nDragRestrictions = HSDR_NONE;
		pSpot->m_pData = m_BaseMatrix.m_vPts[n];
		pSpot->m_pEventResponder = this;
		pSpot->m_nClass = OBJECT_CLASS_BPATCH;
		VectorCopy(m_BaseMatrix.m_vPts[n], pSpot->m_vPt);
		pHotSpotManager->AddHotSpot(pSpot);
	}

	// Setup lines between corners, and their vector endpoints.
	pSpots[1]->m_pLineSpot  = pSpots[0];
	pSpots[1]->m_nDrawFlags = HSDF_ROUND|HSDF_LINE;
	pSpots[2]->m_pLineSpot  = pSpots[3];
	pSpots[2]->m_nDrawFlags = HSDF_ROUND|HSDF_LINE;
	pSpots[4]->m_pLineSpot  = pSpots[0];
	pSpots[4]->m_nDrawFlags = HSDF_ROUND|HSDF_LINE;
	pSpots[7]->m_pLineSpot  = pSpots[3];
	pSpots[7]->m_nDrawFlags = HSDF_ROUND|HSDF_LINE;
	pSpots[8]->m_pLineSpot  = pSpots[12];
	pSpots[8]->m_nDrawFlags = HSDF_ROUND|HSDF_LINE;
	pSpots[11]->m_pLineSpot  = pSpots[15];
	pSpots[11]->m_nDrawFlags = HSDF_ROUND|HSDF_LINE;
	pSpots[13]->m_pLineSpot  = pSpots[12];
	pSpots[13]->m_nDrawFlags = HSDF_ROUND|HSDF_LINE;
	pSpots[14]->m_pLineSpot  = pSpots[15];
	pSpots[14]->m_nDrawFlags = HSDF_ROUND|HSDF_LINE;
}

void CBezierPatch::ComputeOrigin()
{
	BuildRange();
	ComputeRangeOrigin(m_vMins, m_vMaxs, m_vOrigin);
}

void CBezierPatch::BuildRange()
{
	InitializeRange(m_vMins, m_vMaxs);
	InsertIntoRange(m_vMins, m_vMaxs);
}

void CBezierPatch::Scale(vec3_t vScale)
{
	CBezierPatch::Scale(vScale, m_vOrigin);
}

void CBezierPatch::Scale(vec3_t vScale, vec3_t vOrigin)
{
	int i;
	vec3_t vInvOrigin;

	m_bNoBuild = true;

	VectorScale(vOrigin, -1.0F, vInvOrigin);
	CBezierPatch::Translate(vInvOrigin);

	for(i = 0; i < 16; i++)
		VectorMultiply(m_BaseMatrix.m_vPts[i], vScale, m_BaseMatrix.m_vPts[i]);

	m_bNoBuild = false;

	CBezierPatch::Translate(vOrigin);
}

void CBezierPatch::Translate(vec3_t vTranslation)
{
	// Translate the base matrix.
	int i;
	for(i = 0; i < 16; i++)
		VectorAdd(m_BaseMatrix.m_vPts[i], vTranslation, m_BaseMatrix.m_vPts[i]);

	CBezierPatch::BuildPatch(GetMapManager()->GetDocPtr()->GetCurveSubDepth());
}

void CBezierPatch::Rotate(vec3_t vRot)
{
	Rotate(m_vOrigin, vRot);
}

void CBezierPatch::Rotate(vec3_t vOrigin, vec3_t vRot)
{
	int i;
	vec3_t vNegOrigin = { -vOrigin[0], -vOrigin[1], -vOrigin[2] };

	m_bNoBuild = true;

	CBezierPatch::Translate(vNegOrigin);

	for(i = 0; i < 16; i++)
		VectorRotate(m_BaseMatrix.m_vPts[i], vRot, m_BaseMatrix.m_vPts[i]);

	m_bNoBuild = false;

	CBezierPatch::Translate(vOrigin);
	
}

void CBezierPatch::InsertIntoRange(vec3_t vMins, vec3_t vMaxs)
{
	// Insert the faces.
	CFace* pFace;

	m_pFaceList->First();
	for(pFace = m_pFaceList->GetCurItem(); pFace != NULL; pFace = m_pFaceList->GetNextItem())
		pFace->InsertIntoRange(vMins, vMaxs);
}

// Called when the texture changes.
void CBezierPatch::OnTextureChange()
{
	CFace* pFace;

	m_pFaceList->First();
	for(pFace = m_pFaceList->GetCurItem(); pFace != NULL; pFace = m_pFaceList->GetNextItem())
		pFace->SetTextureName(m_sTexture);
}

// Generates the curve.
void CBezierPatch::BuildPatch(int nSubdivisionDepth)
{
	if(m_bNoBuild)
		return;
	GeneratePatchList(nSubdivisionDepth);
	GenerateFacesFromPatchList();
	BuildRange();
	OnTextureChange();
}

void CBezierPatch::InitAsModel(CTread3DDoc* pDocument)
{
	CBezierPatch::BuildPatch(pDocument->GetCurveSubDepth());
}

// Makes a test curve.
CBezierPatch* CBezierPatch::TestPatch()
{
	int i;
	/*vec3_t vPts[16] = { {0, 0, 0}, {171, 0, 0}, {342, 0, 0 }, {512, 0, 0}, 
						{0, 171, 256}, {171, 171, 0}, {342, 171, 0}, {512, 171, 256},
						{0, 342, 256}, {171, 342, 0}, {342, 342, 0}, {512, 342, 256},
						{0, 512, 0}, {171, 512, 0}, {342, 512, 0}, {512, 512, 0} };
	*/
	vec3_t vPts[16] = { {0, 0, 0}, {171, 0, 0}, {342, 0, 0 }, {512, 0, 0}, 
						{0, 171, 0}, {171, 171, 0}, {342, 171, 0}, {512, 171, 0},
						{0, 342, 0}, {171, 342, 0}, {342, 342, 0}, {512, 342, 0},
						{0, 512, 0}, {171, 512, 0}, {342, 512, 0}, {512, 512, 0} };

	CBezierPatch* pPatch = new CBezierPatch();
	for(i = 0; i < 16; i++)
		VectorCopy(vPts[i], pPatch->m_BaseMatrix.m_vPts[i]);

	return pPatch;

}

bool CBezierPatch::ReadObject(CFile& hFile, int nVersion)
{
	int i, k;

	if(!CBaseTextureableObject::ReadTextureInfo(hFile, nVersion))
		return false;

	// Load the visgroup info.
	if(!CVisgroupRefObject::Read(hFile))
		return false;

	for(i = 0; i < 16; i++)
		for(k = 0; k < 3; k++)
			hFile.Read(&m_BaseMatrix.m_vPts[i][k], 4);

	return true;
}

int CBezierPatch::InitAfterLoad(CTread3DDoc* pDocument)
{
	BuildPatch(pDocument->GetCurveSubDepth());

	return IALF_OK;
}

bool CBezierPatch::WriteObject(CFile& hFile, int nVersion)
{
	if(!CBaseTextureableObject::WriteTextureInfo(hFile, nVersion))
		return false;

	// Store Visgroup info.
	CVisgroupRefObject::Write(hFile);

	int i, k;
	for(i = 0; i < 16; i++)
		for(k = 0; k < 3; k++)
			hFile.Write(&m_BaseMatrix.m_vPts[i][k], 4);

	return true;
}

// Generates faces from patch list.
void CBezierPatch::GenerateFacesFromPatchList()
{
	int i;
	CFace* pFace;
	BEZIERMATRIX* pPatch;
	vec3_t vPts[4];

	m_pFaceList->DestroyList();

	i = 0;
	m_pPatchList->First();
	for(pPatch = m_pPatchList->GetCurItem(); pPatch != NULL; pPatch = m_pPatchList->GetNextItem(), i++)
	{
		// Take corners.
		VectorCopy(pPatch->m_vPts[0], vPts[0]);
		VectorCopy(pPatch->m_vPts[12], vPts[1]);
		VectorCopy(pPatch->m_vPts[15], vPts[2]);
		VectorCopy(pPatch->m_vPts[3], vPts[3]);

		pFace = new CFace(4, vPts);
		m_pFaceList->AddItem(pFace);
	}

	if(i != m_nNumFaces)
		Debug("bezierMatrix: %x : built %i face(s)\n", this, i);
	m_nNumFaces = i;
}	

// Generates the final patch list.
void CBezierPatch::GeneratePatchList(int nDepth)
{
	int i;
	BEZIERMATRIX* pPatches[4];
	CLinkedList<BEZIERMATRIX> List[2];

	m_pPatchList->DestroyList();
	if(nDepth == 0)
		return;
	if(nDepth == 1)
	{
		QuadSplitPatch(&m_BaseMatrix, pPatches);
		for(i = 0; i < 4; i++)
			m_pPatchList->AddItem(pPatches[i]);
		return;
	}

	// Make the original list.
	QuadSplitPatch(&m_BaseMatrix, pPatches);
	
	// Make first list.
	for(i = 0; i < 4; i++)
		List[0].AddItem(pPatches[i]);

	for(i = 0; i < nDepth-2; i++)
	{
		QuadSplitPatchList(&List[i%2], &List[!(i%2)]);
		List[i%2].DestroyList();
	}

	// Split into original list.
	QuadSplitPatchList(&List[i%2], m_pPatchList);
	List[i%2].DestroyList();
}

// Splits a patch list into new patches.
void CBezierPatch::QuadSplitPatchList(CLinkedList<BEZIERMATRIX>* pPatchList, CLinkedList<BEZIERMATRIX>* pOutList)
{
	int i;
	BEZIERMATRIX* pPatch, *Quads[4];

	pPatchList->First();
	for(pPatch = pPatchList->GetCurItem(); pPatch != NULL; pPatch = pPatchList->GetNextItem())
	{
		QuadSplitPatch(pPatch, Quads);
		for(i = 0; i < 4; i++)
			pOutList->AddItem(Quads[i]);
	}
}

// Splits a patch into four patches.
void CBezierPatch::QuadSplitPatch(BEZIERMATRIX* pSourceMatrix, BEZIERMATRIX* pOutMatrix[4])
{
	BEZIERMATRIX* pTemp[2];
	BEZIERMATRIX* pTopQuarter[2], *pBottomQuarter[2];

	USplitPatch(pSourceMatrix, pTemp);

	// Split those in the V direction.
	VSplitPatch(pTemp[0], pTopQuarter);
	VSplitPatch(pTemp[1], pBottomQuarter);

	pOutMatrix[0] = pTopQuarter[0];
	pOutMatrix[1] = pTopQuarter[1];
	pOutMatrix[2] = pBottomQuarter[0];
	pOutMatrix[3] = pBottomQuarter[1];

	// Delete the temps.
	delete pTemp[0];
	delete pTemp[1];
}

// Splits control points.
void CBezierPatch::SplitControls(vec3_t vControls[4], vec3_t vControlsA[4], vec3_t vControlsB[4])
{
	vec3_t vTemp, vTemp2;

	// R0=Q0.
	VectorCopy(vControls[0], vControlsA[0]);
	// R1=(Q0+Q1)/2
	VectorAdd(vControls[0], vControls[1], vTemp);
	VectorScale(vTemp, 0.5F, vControlsA[1]);
	// R2=R1/2+(Q1+Q2)/4
	VectorScale(vControlsA[1], 0.5F, vTemp);
	VectorAdd(vControls[1], vControls[2], vTemp2);
	VectorScale(vTemp2, 0.25F, vTemp2);
	VectorAdd(vTemp, vTemp2, vControlsA[2]);
	// S3=Q3.
	VectorCopy(vControls[3], vControlsB[3]);
	// S2=(Q2+Q3)/2
	VectorAdd(vControls[2], vControls[3], vTemp);
	VectorScale(vTemp, 0.5F, vControlsB[2]);
	// S1=(Q1+Q2)/4+S2/2
	VectorAdd(vControls[1], vControls[2], vTemp);
	VectorScale(vTemp, 0.25F, vTemp);
	VectorScale(vControlsB[2], 0.5F, vTemp2);
	VectorAdd(vTemp, vTemp2, vControlsB[1]);
	// R3=(R2+S1)/2
	VectorAdd(vControlsA[2], vControlsB[1], vTemp);
	VectorScale(vTemp, 0.5F, vControlsA[3]);
	// S0=R3
	VectorCopy(vControlsA[3], vControlsB[0]);
}

// Splits a patch along the U edge.
void CBezierPatch::USplitPatch(BEZIERMATRIX* pSourceMatrix, BEZIERMATRIX* pOutMatrix[2])
{
	int i, k;
	vec3_t vControls[4];
	vec3_t OutA[4], OutB[4];
	BEZIERMATRIX* pMA, *pMB;

	pMA = new BEZIERMATRIX();
	pMB = new BEZIERMATRIX();

	// Split down each row of the source matrix.
	for(i = 0; i < 16; i += 4)
	{
		for(k = i; k < i+4; k++)
			VectorCopy(pSourceMatrix->m_vPts[k], vControls[k-i]);

		// Split the control vectors.
		SplitControls(vControls, OutA, OutB);

		// Fill new matrix.
		for(k = i; k < i+4; k++)
			VectorCopy(OutA[k-i], pMA->m_vPts[k]);
		// Fill new matrix.
		for(k = i; k < i+4; k++)
			VectorCopy(OutB[k-i], pMB->m_vPts[k]);
	}

	pOutMatrix[0] = pMA;
	pOutMatrix[1] = pMB;
}

// Splits a patch along the V edge.
void CBezierPatch::VSplitPatch(BEZIERMATRIX* pSourceMatrix, BEZIERMATRIX* pOutMatrix[2])
{
	int i, k;
	vec3_t vControls[4];
	vec3_t OutA[4], OutB[4];
	BEZIERMATRIX* pMA, *pMB;

	pMA = new BEZIERMATRIX();
	pMB = new BEZIERMATRIX();

	// Split down each column of the source matrix.
	for(i = 0; i < 4; i++)
	{
		// Pull out one column.
		for(k = i; k < i+13; k += 4)
			VectorCopy(pSourceMatrix->m_vPts[k], vControls[(k-i)>>2]);

		// Split the controls.
		SplitControls(vControls, OutA, OutB);

		// Fill the new matrix.
		for(k = i; k < i+13; k += 4)
			VectorCopy(OutA[(k-i)>>2], pMA->m_vPts[k]);
		// Fill the new matrix.
		for(k = i; k < i+13; k += 4)
			VectorCopy(OutB[(k-i)>>2], pMB->m_vPts[k]);
	}

	pOutMatrix[0] = pMA;
	pOutMatrix[1] = pMB;
}