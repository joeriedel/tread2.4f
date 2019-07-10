// BRUSH.CPP
// Copyright © 1999 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#include "stdafx.h"
#include "LinkedList.h"
#include "MathLib.h"
#include "MapObject.h"
#include "Face.h"
#include "Brush.h"
#include "TreadSys.h"
#include "Tread3DDoc.h"
#include "RenderApi.h"
#include "Geometry.h"
#include "TexAPI.h"
#include "MapView.h"
#include "futils.h"

#define PSIDE_ONBACK	5
#define PSIDE_ONFRONT	6

CQBrush::CQBrush()
	: CBaseGroupableObject() 
	, CBaseTransformableObject()
	, CBaseTextureableObject()
{
	m_pFaceList = new CLinkedList<CFace>();
}

CQBrush::CQBrush(const CQBrush& BrushToCopy)
	: CBaseGroupableObject(BrushToCopy) 
	, CBaseTransformableObject(BrushToCopy)
	, CBaseTextureableObject(BrushToCopy)
{
	m_pFaceList = new CLinkedList<CFace>();	// Duplicate faces & list.
	
	// Assign each face to this brush.
	CFace* pFace;
	BrushToCopy.m_pFaceList->First();
	for(pFace = BrushToCopy.m_pFaceList->GetCurItem(); pFace != NULL; pFace = BrushToCopy.m_pFaceList->GetNextItem())
	{
		pFace = (CFace*)pFace->Clone();
		pFace->m_pParent = this;
		m_pFaceList->AddItem(pFace);
	}

	ComputeOrigin();
}

CQBrush::~CQBrush()
{
	if(m_pFaceList != NULL)
		delete m_pFaceList;
	m_pFaceList = NULL;
}

// Builds the brush range.
void CQBrush::BuildRange()
{
	CFace* pFace;

	InitializeRange(m_vMins, m_vMaxs);
	
	m_pFaceList->First();
	for(pFace = m_pFaceList->GetCurItem(); pFace != NULL; pFace = m_pFaceList->GetNextItem())
		pFace->InsertIntoRange(m_vMins, m_vMaxs);

}

void* CQBrush::GetInterface(int nInterface)
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
	case I_CARVEABLE:
		return (CBaseCarveableObject*)this;
	}

	return NULL;
}

void CQBrush::OnAttributeChange()
{
	CFace* pFace;
	m_nObjectRenderFlags &= ~(RF_TRANSPARENT|RF_FOG);

	m_pFaceList->First();
	for(pFace = m_pFaceList->GetCurItem(); pFace != NULL; pFace = m_pFaceList->GetNextItem())
	{
		pFace->OnAttributeChange();
		m_nObjectRenderFlags |= (pFace->m_nObjectRenderFlags&~RF_SELECTED);
	}
}

int CQBrush::InitAfterLoad(CTread3DDoc* pDocument)
{
	OnAttributeChange();
	BindToDocument(pDocument);

	return IALF_OK;
}

bool CQBrush::In2DRange(vec3_t vMins, vec3_t vMaxs, int nView)
{
	CFace* pFace;

	m_pFaceList->First();
	for(pFace = m_pFaceList->GetCurItem(); pFace != NULL; pFace = m_pFaceList->GetNextItem())
		if(pFace->In2DRange(vMins, vMaxs, nView))
			return true;

	return false;
}

// Computes the origin of this brush.
void CQBrush::ComputeOrigin()
{
	CQBrush::BuildRange();

	/*CFace* pFace;

	VectorClear(m_vOrigin);

	m_pFaceList->First();
	for(pFace = m_pFaceList->GetCurItem(); pFace != NULL; pFace = m_pFaceList->GetNextItem())
		VectorAdd(m_vOrigin, pFace->m_vOrigin, m_vOrigin);

	VectorScale(m_vOrigin, 1.0f/m_pFaceList->GetCount(), m_vOrigin);*/
	ComputeRangeOrigin(m_vMins, m_vMaxs, m_vOrigin);
}

void CQBrush::Reflect(int nAxis, vec3_t vFlipOrigin)
{
	CFace* pFace;

	m_pFaceList->First();
	for(pFace = m_pFaceList->GetCurItem(); pFace != NULL; pFace = m_pFaceList->GetNextItem())
		pFace->Reflect(nAxis, vFlipOrigin);
}

// Writes an object.
bool CQBrush::WriteObject(CFile& hFile, int nVersion)
{
	int nNumFaces;
	CFace* pFace;

	if(!CVisgroupRefObject::Write(hFile))
		return false;

	// Write the # of faces.
	nNumFaces = m_pFaceList->GetCount();
	int nSwp = LOAD_INTEL_INT(nNumFaces);
	hFile.Write(&nSwp, 4);
	
	m_pFaceList->First();
	for(pFace = m_pFaceList->GetCurItem(); pFace != NULL; pFace = m_pFaceList->GetNextItem())
		if(!pFace->WriteObject(hFile, nVersion))
			return false;

	return true;
}

// Reads an object.
bool CQBrush::ReadObject(CFile& hFile, int nVersion)
{
	CFace* pFace;
	int i, nNumFaces;

	if(!CVisgroupRefObject::Read(hFile))
		return false;

	// Destroy the list.
	m_pFaceList->DestroyList();

	// Read the # of faces.
	ReadInt(hFile, nNumFaces);
	for(i = 0; i < nNumFaces; i++)
	{
		pFace = (CFace*)NewBaseObject(OBJECT_CLASS_FACE);//new CFace();
		if(!pFace->ReadObject(hFile, nVersion))
			return false;
		m_pFaceList->AddToTail(pFace);
		pFace->m_pParent = this;
	}

	return true;
}

// Scales the brush.
void CQBrush::Scale(vec3_t vScale)
{
	CQBrush::Scale(vScale, m_vOrigin);
}

void CQBrush::Scale(vec3_t vScale, vec3_t vOrigin)
{
	CFace* pFace;

	m_pFaceList->First();
	for(pFace = m_pFaceList->GetCurItem(); pFace != NULL; pFace = m_pFaceList->GetNextItem())
		pFace->Scale(vScale, vOrigin);
	CQBrush::ComputeOrigin();
}

// Translates the brush.
void CQBrush::Translate(vec3_t vTranslation)
{
	CFace* pFace;

	m_pFaceList->First();
	for(pFace = m_pFaceList->GetCurItem(); pFace != NULL; pFace = m_pFaceList->GetNextItem())
		pFace->Translate(vTranslation);

	CQBrush::ComputeOrigin();
}

// Rotates the brush.
void CQBrush::Rotate(vec3_t vOrigin, vec3_t vRot)
{
	CFace* pFace;

	m_pFaceList->First();
	for(pFace = m_pFaceList->GetCurItem(); pFace != NULL; pFace = m_pFaceList->GetNextItem())
		pFace->Rotate(vOrigin, vRot);

	CQBrush::ComputeOrigin();
}

void CQBrush::Rotate(vec3_t vRot)
{
	CQBrush::Rotate(m_vOrigin, vRot);
}

// Inserts brush into range.
void CQBrush::InsertIntoRange(vec3_t vMins, vec3_t vMaxs)
{
	CQBrush::BuildRange();
	InsertRange(m_vMins, vMins, vMaxs);
	InsertRange(m_vMaxs, vMins, vMaxs);
}

// Called when the texture changes.
void CQBrush::OnTextureChange()
{
	CFace* pFace;

	// Update all the faces.
	m_pFaceList->First();
	for(pFace = m_pFaceList->GetCurItem(); pFace != NULL; pFace = m_pFaceList->GetNextItem())
		pFace->SetTextureName(m_sTexture);

	OnAttributeChange();
}

void CQBrush::RebuildTexture()
{
	CFace* pFace;

	// Update all the faces.
	m_pFaceList->First();
	for(pFace = m_pFaceList->GetCurItem(); pFace != NULL; pFace = m_pFaceList->GetNextItem())
		pFace->RebuildTexture();
}

// Makes sure every point in the brush is on the back of the other faces.
bool CQBrush::CheckBrush() 
{
	CLinkedList<CFace> List;
	float fDist;
	CFace* pFace, *pFace2;
	int i;

	m_pFaceList->First();
	for(pFace = m_pFaceList->GetCurItem(); pFace != NULL; pFace = m_pFaceList->GetNextItem())
		if(!pFace->CheckWinding())
		{
			//Warning("Bad brush face!\n");
			return false;
		}

	List.MakeWrapperList(*m_pFaceList);

	m_pFaceList->First();
	for(pFace = m_pFaceList->GetCurItem(); pFace != NULL; pFace = m_pFaceList->GetNextItem())
	{
		for(i = 0; i < pFace->m_nNumPoints; i++)
		{
			List.First();
			for(pFace2 = List.GetCurItem(); pFace2 != NULL; pFace2 = List.GetNextItem())
			{
				if(pFace2 == pFace)
					continue;

				fDist = DotProduct(pFace->m_vPoints[i], pFace2->m_Plane.m_vNormal) - pFace2->m_Plane.m_vDist;
				if(fDist > 1.1F)
				{
					// Bad brush.
					return false;
				}
			}
		}
	}

	return true;
}

// Returns true if brush crosses plane.
int CQBrush::ClassifyBrush(CPlane* pPlane)
{
	CFace* pFace;
	int nSide, nFront=0, nBack=0, nOn=0, nBoth=0;

	m_pFaceList->First();
	for(pFace = m_pFaceList->GetCurItem(); pFace != NULL; pFace = m_pFaceList->GetNextItem())
	{
		nSide = pFace->ClassifyWinding(pPlane);
		switch(nSide)
		{
		case PSIDE_FRONT:
			nFront++;
			break;
		case PSIDE_BACK:
			nBack++;
			break;
		case PSIDE_ON:
			nOn++;
			break;
		case PSIDE_BOTH:
			nBoth++;
			break;
		}
	}

	if(nBoth > 0)
		return PSIDE_BOTH;

	if(nBack == 0)
		return PSIDE_FRONT;
	
	return PSIDE_BACK;
}

// Called the create a brush from a list of planes.
CQBrush* CQBrush::CreateBrushFromPlaneList(CLinkedList<CQBrushPlane>& PlaneListA)
{
	CQBrush* pBrush;
	CQBrushPlane* pPlaneA, *pPlaneB;
	CFace* pFaceToSplit, *pFrag;
	CLinkedList<CQBrushPlane> PlaneListB;
	
	// Make a new brush.
	pBrush = (CQBrush*)NewBaseObject(OBJECT_CLASS_BRUSH);//new CQBrush();

	// Create a working copy of the list.
	PlaneListB.MakeWrapperList(PlaneListA);

	PlaneListA.First();
	for(pPlaneA = PlaneListA.GetCurItem(); pPlaneA; pPlaneA = PlaneListA.GetNextItem())
	{
		// Create a base face.
		pFaceToSplit = CFace::BaseFaceFromPlane(&pPlaneA->m_Plane);
		if(pFaceToSplit == NULL)
		{
			Warning("CQBrush: CreateBrushFromPlaneList(): degenerate face plane ignored.\n");
			continue;
		}

		pFrag = NULL;
		PlaneListB.First();
		for(pPlaneB = PlaneListB.GetCurItem(); pPlaneB; pPlaneB = PlaneListB.GetNextItem())
		{
			// Don't split w/ the same face.
			if(pPlaneB == pPlaneA)
				continue;

			// Split and return back side.
			pFrag = pFaceToSplit->ChopFaceByPlane(&pPlaneB->m_Plane, false);
			delete pFaceToSplit;
			pFaceToSplit = pFrag;

			if(pFrag == NULL)
				break;
		}

		if(pFrag == NULL)
			Warning("CQBrush::CreateBrushFromPlaneList(): face clipped out.\n");
		else
		{
			pFrag->SetTextureName(pPlaneA->m_sTexture, false);
			pFrag->SetTexScales(pPlaneA->m_vScales, false);
			pFrag->SetTexShifts(pPlaneA->m_vShifts, false);
			pFrag->SetRotation(pPlaneA->m_fRotate, false);
			pBrush->m_pFaceList->AddItem(pFrag);
			if(pPlaneA->m_bUseFlags)
			{
				pFrag->m_nSurface = pPlaneA->m_nSurface;
				pFrag->m_nContents = pPlaneA->m_nContents;
				pFrag->m_nValue = pPlaneA->m_nValue;
			}
			else
			{
				CTexture* pTex = GetTexAPI()->GetTextureFromName(pPlaneA->m_sTexture);
				if(pTex)
				{
					pFrag->m_nSurface = pTex->m_nSurface;
					pFrag->m_nContents = pTex->m_nContents;
					pFrag->m_nValue = pTex->m_nValue;
				}
				else
				{
					pFrag->m_nSurface = 0;
					pFrag->m_nContents = 0;
					pFrag->m_nValue = 0;
				}
			}

			pFrag->m_pParent = pBrush;
			pFrag->m_bTexInit = false;
			pFrag->RebuildTexture();
		}
	}

	if(pBrush->m_pFaceList->GetCount() == 0)
	{
		Warning("CQBrush::CreateBrushFromPlaneList: no faces emitted.\n");
		delete pBrush;
		return NULL;
	}

	pBrush->BuildRange();
	pBrush->ComputeOrigin();

	return pBrush;
}

// Splits a brush by a plane.
void CQBrush::SplitBrushByPlane(CPlane* pPlane, CQBrush** pFront, CQBrush** pBack)
{
	int nSide;
	CFace* pFace;
	CLinkedList<CQBrushPlane> m_FrontList, m_BackList;
	CQBrushPlane* pBrushPlane;

	*pFront = NULL;
	*pBack  = NULL;

	m_pFaceList->First();
	for(pFace = m_pFaceList->GetCurItem(); pFace; pFace = m_pFaceList->GetNextItem())
	{
		nSide = pFace->ClassifyWinding(pPlane);
		
		if(nSide == PSIDE_FRONT || nSide == PSIDE_BOTH)
		{
			pBrushPlane = new CQBrushPlane(*(pFace->GetPlanePtr()));
			pBrushPlane->m_sTexture = pFace->GetTextureName();
			// Add to front side.
			m_FrontList.AddItem(pBrushPlane);
		}
		if(nSide == PSIDE_BACK || nSide == PSIDE_BOTH)
		{
			pBrushPlane = new CQBrushPlane(*(pFace->GetPlanePtr()));
			pBrushPlane->m_sTexture = pFace->GetTextureName();
			m_BackList.AddItem(pBrushPlane);
		}
		if(nSide == PSIDE_ON)
			Warning("SplitBrushByPlane(): PSIDE_ON!!\n");
	}

	// If we didn't split the brush, then stop here.
	if(m_FrontList.GetCount() == 0 || m_BackList.GetCount() == 0)
	{
		Warning("CQBrush::SplitBrushByPlane: plane does not split brush.\n");
		return;
	}

	// Add last plane to brushes.
	CQBrushPlane BrushPlane(*pPlane);
	
	// Use current texture for addition.
	BrushPlane.m_sTexture = GetTexAPI()->GetCurTextureName();

	m_BackList.AddItem( new CQBrushPlane(BrushPlane));
	BrushPlane.m_Plane.Flip();
	m_FrontList.AddItem(new CQBrushPlane(BrushPlane));	// Add this plane.
	
	// Build a brush out of it.
	*pFront = CQBrush::CreateBrushFromPlaneList(m_FrontList);
	*pBack  = CQBrush::CreateBrushFromPlaneList(m_BackList);
}

// Hollows the brush via the specified depth.
CObjectGroup* CQBrush::HollowBrush(float fDepth)
{
	bool bOut=false;

	// Make a duplicate of this brush.
	CLinkedList<CQBrushPlane> m_PlaneList;
	
	// If we're negative, we wan't to hollow outwards.
	if(fDepth < 0.0f)
		bOut = true;
	
	vec3_t vPoint;

	// Copy planes from this one.
	CFace* pFace;
	CQBrushPlane* pPlane;

	this->m_pFaceList->First();
	for(pFace = this->m_pFaceList->GetCurItem(); pFace != NULL; pFace = this->m_pFaceList->GetNextItem())
	{
		pPlane = new CQBrushPlane(*(pFace->GetPlanePtr()));
		pPlane->m_sTexture = pFace->GetTextureName();

		// Get the point.
		// We always scale inwards!
		VectorScale(pPlane->m_Plane.m_vNormal, pPlane->m_Plane.m_vDist-fDepth, vPoint);
		// Get the new distance.
		pPlane->m_Plane.m_vDist = DotProduct(vPoint, pPlane->m_Plane.m_vNormal);

		m_PlaneList.AddItem(pPlane);
	}

	CQBrush* pSubBrush = CQBrush::CreateBrushFromPlaneList(m_PlaneList);
	if(pSubBrush == NULL)
	{
		Warning("CQBrush::HollowBrush(): CreateBrushFromPlaneList() returned null!!\n");
		return false;
	}

	CObjectGroup* pGroup;

	if(bOut)
		pGroup = pSubBrush->CSGSubtract(this);
	else
		pGroup = this->CSGSubtract(pSubBrush);

	delete pSubBrush;
	return pGroup;
}

bool CQBrush::ObjectsIntersect(CBaseObject* pObject)
{
	// Source is a group?
	int nClass = pObject->GetObjectClass();
	int nFront=0, nBack=0, nOn=0, nBoth=0;

	if(nClass == OBJECT_CLASS_GROUP)
	{
		CObjectGroup* pGroup = (CObjectGroup*)pObject;
		return pGroup->ObjectsIntersect(this);
	}
	else if(nClass == OBJECT_CLASS_BRUSH)
	{
		int nBCount=0;
		CFace* pFace;
		CQBrush* pBrush = (CQBrush*)pObject;
		
		BuildRange();
		pBrush->BuildRange();
		if(!RangesIntersect(m_vMins, m_vMaxs, pBrush->m_vMins, pBrush->m_vMaxs))
			return false;

		// If it's on the front of any face, then it can't touch.
		m_pFaceList->First();
		for(pFace = m_pFaceList->GetCurItem(); pFace != NULL; pFace = m_pFaceList->GetNextItem())
		{
			pFace->ComputePlane();
			nClass = pBrush->ClassifyBrush(pFace->GetPlanePtr());
			switch(nClass)
			{
			case PSIDE_FRONT:
				nFront++;
				break;
			case PSIDE_BACK:
				nBack++;
				break;
			case PSIDE_BOTH:
				nBoth++;
				break;
			case PSIDE_ON:
				nOn++;
				break;
			}
		}
	}

	// No front faces.
	return nFront == 0;
}

// Subtracts argument brush from this brush.
// Returns a group of brushes.
CObjectGroup* CQBrush::CSGSubtract(CBaseObject* pObject)
{
	CObjectGroup* pGroup;
	CQBrush* pBrush, *pSplitBrush, *pFront, *pBack;
	CFace* pFace;
	int nSide;

	if(pObject->GetObjectClass() == OBJECT_CLASS_GROUP)
	{
		return CObjectGroup::CarveWithGroup(this, (CObjectGroup*)pObject);
	}
	else if(pObject->GetObjectClass() != OBJECT_CLASS_BRUSH)
	{
		Warning("CQBrush:CSGSubtract(): source carver is not of supported class type.");
		return NULL;
	}

	pBrush = (CQBrush*)pObject;

	if(!RangesIntersect(pBrush->m_vMins, pBrush->m_vMaxs, this->m_vMins, this->m_vMaxs))
	{
		Warning("CQBrush::CSGSubtract(): target brush does not intersect the source.\n");
		return NULL;
	}

	// Make a group.
	pGroup = new CObjectGroup();

	pSplitBrush = this;
	pBrush->m_pFaceList->First();
	for(pFace = pBrush->m_pFaceList->GetCurItem(); pFace; pFace = pBrush->m_pFaceList->GetNextItem())
	{
		nSide = pSplitBrush->ClassifyBrush(pFace->GetPlanePtr());
		if(nSide == PSIDE_BOTH)
		{
			// Split the pBrush.
			pSplitBrush->SplitBrushByPlane(pFace->GetPlanePtr(), &pFront, &pBack);
			// Free this fragment if it's not this pBrush.
			if(pSplitBrush != this)
				delete pSplitBrush;

			// Emit front?
			if(pFront != NULL)
				pGroup->AddObject(pFront);
			pSplitBrush = pBack;
			
			// End?
			if(pBack == NULL)
				break;
		}
		else if(nSide == PSIDE_FRONT)
			Warning("CQBrush::CSGSubtract(): pBrush is PSIDE_FRONT, but wasn't emitted.\n");
	}

	return pGroup;
}

// Make hotspots.
void CQBrush::GenerateHotSpots(CHotSpotManager* pHotSpotManager, CMapView* pView, CBaseHotSpotEventResponder* pResp, bool bHandles, bool bSel)
{
	if(bHandles)	// Doing center of brush for dragging.
	{
		if(pResp == NULL)
		{
			Warning("CQBrush::GenerateHotSpots(): missing responder!\n");
			return;
		}

		ComputeOrigin();

		CHotSpot* pSpot = new CHotSpot();
		pSpot->m_nClass = OBJECT_CLASS_BRUSH;
		pSpot->m_fMX = m_vOrigin[VI(VEC_X, pView)];
		pSpot->m_fMY = m_vOrigin[VI(VEC_Y, pView)];
		pSpot->ComputeWindowXY(pView);
		pSpot->m_nDrawFlags = HSDF_CROSS|HSDF_HD_COLOR;
		if(bSel)
			pSpot->m_nObjectRenderFlags = RF_SELECTED;
		pSpot->m_nCursor = TC_IDC_CROSS;
		pSpot->m_nDragRestrictions = HSDR_NONE;
		pSpot->m_pData = this;
		pSpot->m_pEventResponder = pResp;
		pHotSpotManager->AddHotSpot(pSpot);

		return;
	}

	CFace* pFace;

	m_pFaceList->First();
	for(pFace = m_pFaceList->GetCurItem(); pFace != NULL; pFace = m_pFaceList->GetNextItem())
	{
		pFace->GenerateHotSpots(pHotSpotManager, pView);
	}
}
