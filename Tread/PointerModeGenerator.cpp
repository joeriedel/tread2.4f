// POINTERMODEGENERATOR.CPP
// Copyright © 1999 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#include "stdafx.h"
#include "MapObject.h"
#include "HotSpotManager.h"
#include "PointerModeGenerator.h"
#include "MapView.h"
#include "Tread3DDoc.h"
#include "TreadSys.h"
#include "Brush.h"
#include "Geometry.h"
#include "Anchor.h"

#define DF_SIZE_MINX	0x00000001
#define DF_SIZE_MAXX	0x00000002
#define DF_SIZE_MINY	0x00000004
#define DF_SIZE_MAXY	0x00000008
#define DF_SIZE_CENX	0x00000010
#define DF_SIZE_CENY	0x00000020

#define SENSATIVITY	3

CPointerModeGenerator::CPointerModeGenerator()
{
	m_nMode = MODE_POINTER;
	m_nSubmode = ALL_MODES;
}

void CPointerModeGenerator::BuildBrushHandles(CLinkedList<CBaseObject>* pList, CMapView* pView, CHotSpotManager* pHotSpotManager, bool bSel)
{
	CBaseObject* pObj;
	
	pList->First();
	for(pObj = pList->GetCurItem(); pObj != NULL; pObj = pList->GetNextItem())
	{
		if(pObj->GetObjectInterfaces()&I_VISGROUPREF)
		{
			if(((CVisgroupRefObject*)pObj->GetInterface(I_VISGROUPREF))->IsVisible() == false)
				continue;
		}

		if(pObj->GetObjectClass() == OBJECT_CLASS_GROUP)
			BuildBrushHandles(((CObjectGroup*)pObj)->GetObjectList(), pView, pHotSpotManager, bSel);
		else
		if(pObj->GetObjectClass() == OBJECT_CLASS_LENTITY)
			BuildBrushHandles(((CEntity*)pObj)->GetObjectList(), pView, pHotSpotManager, bSel);
		else
		if(pObj->GetObjectClass() == OBJECT_CLASS_BRUSH)
			((CQBrush*)pObj)->GenerateHotSpots(pHotSpotManager, pView, this, true, bSel);
	}
}

void CPointerModeGenerator::GenerateHotSpots(CHotSpotManager* pHotSpotManager, CTread3DDoc* pDocument, CMapView* pView)
{
	// Let the anchors make their hotspots.
	CBaseObject* pObj;
	CLinkedList<CBaseObject>* pList = pDocument->GetUIObjectList();
	pList->First();
	for(pObj = pList->GetCurItem(); pObj != NULL; pObj = pList->GetNextItem())
	{
		if(pObj->GetObjectClass() != OBJECT_CLASS_ANCHOR)
			continue;

		((CAnchor*)pObj)->GenerateHotSpots(pHotSpotManager, pView);

	}

	// Let the brushes make thier hotspots.
	if(pDocument->GetBrushHandles())
	{
		BuildBrushHandles(pDocument->GetObjectList(), pView, pHotSpotManager, false);
		BuildBrushHandles(pDocument->GetSelectedObjectList(), pView, pHotSpotManager, true);
	}

	// Attach hotspots to the bounding box.
	if(pDocument->GetSelectedObjectList()->IsEmpty())
		return;

	// Nothing but one entity?
	if(pDocument->GetSelectedObjectCount(OBJECT_CLASS_ALL&~OBJECT_CLASS_ENTITY) == 0)
	{
		//if(pDocument->GetSelectedObjectCount(OBJECT_CLASS_ENTITY) == 1)
		return;
	}

	int nDrawFlags = ((pDocument->GetModePtr()->GetSubmode()&POINTER_ROTATE)==0) ? HSDF_SQUARE : HSDF_ROUND;
	
	// Do corners.
	// MINX/MINY
	CHotSpot* pSpot = new CHotSpot();
	pSpot->m_fMX = pDocument->m_vSelMins[VI(VEC_X, pView)];
	pSpot->m_fMY = pDocument->m_vSelMins[VI(VEC_Y, pView)];
	pSpot->m_nDragRestrictions = HSDR_NONE;
	pSpot->m_nDrawFlags = nDrawFlags;
	pSpot->m_pEventResponder = this;
	pSpot->m_nDataFlags = DF_SIZE_MINX|DF_SIZE_MINY;
	if(VO(VT(pView), VEC_X) > 0)
		pSpot->m_nCursor = TC_IDC_SIZENESW;
	else
		pSpot->m_nCursor = TC_IDC_SIZENWSE;
	if(nDrawFlags == HSDF_ROUND)	// Crosshair.
		pSpot->m_nCursor = TC_IDC_CROSS;
	pHotSpotManager->AddHotSpot(pSpot);
	// MAXX/MINY
	pSpot = new CHotSpot();
	pSpot->m_fMX = pDocument->m_vSelMaxs[VI(VEC_X, pView)];
	pSpot->m_fMY = pDocument->m_vSelMins[VI(VEC_Y, pView)];
	pSpot->m_nDragRestrictions = HSDR_NONE;
	pSpot->m_nDrawFlags = nDrawFlags;
	pSpot->m_pEventResponder = this;
	pSpot->m_nDataFlags = DF_SIZE_MAXX|DF_SIZE_MINY;
	if(VO(VT(pView), VEC_X) > 0)
		pSpot->m_nCursor = TC_IDC_SIZENWSE;
	else
		pSpot->m_nCursor = TC_IDC_SIZENESW;
	if(nDrawFlags == HSDF_ROUND)	// Crosshair.
		pSpot->m_nCursor = TC_IDC_CROSS;
	pHotSpotManager->AddHotSpot(pSpot);
	// MAXX/MAXY
	pSpot = new CHotSpot();
	pSpot->m_fMX = pDocument->m_vSelMaxs[VI(VEC_X, pView)];
	pSpot->m_fMY = pDocument->m_vSelMaxs[VI(VEC_Y, pView)];
	pSpot->m_nDragRestrictions = HSDR_NONE;
	pSpot->m_nDrawFlags = nDrawFlags;
	pSpot->m_pEventResponder = this;
	pSpot->m_nDataFlags = DF_SIZE_MAXX|DF_SIZE_MAXY;
	if(VO(VT(pView), VEC_X) > 0)
		pSpot->m_nCursor = TC_IDC_SIZENESW;
	else
		pSpot->m_nCursor = TC_IDC_SIZENWSE;
	if(nDrawFlags == HSDF_ROUND)	// Crosshair.
		pSpot->m_nCursor = TC_IDC_CROSS;
	pHotSpotManager->AddHotSpot(pSpot);
	// MINX/MAXY
	pSpot = new CHotSpot();
	pSpot->m_fMX = pDocument->m_vSelMins[VI(VEC_X, pView)];
	pSpot->m_fMY = pDocument->m_vSelMaxs[VI(VEC_Y, pView)];
	pSpot->m_nDragRestrictions = HSDR_NONE;
	pSpot->m_nDrawFlags = nDrawFlags;
	pSpot->m_pEventResponder = this;
	pSpot->m_nDataFlags = DF_SIZE_MINX|DF_SIZE_MAXY;
	if(VO(VT(pView), VEC_X) > 0)
		pSpot->m_nCursor = TC_IDC_SIZENWSE;
	else
		pSpot->m_nCursor = TC_IDC_SIZENESW;
	if(nDrawFlags == HSDF_ROUND)	// Crosshair.
		pSpot->m_nCursor = TC_IDC_CROSS;
	pHotSpotManager->AddHotSpot(pSpot);

	if((pDocument->GetModePtr()->GetSubmode()&POINTER_ROTATE)==0)
	{
		// Sides.
		// CENX/MINY
		pSpot = new CHotSpot();
		pSpot->m_fMX = pDocument->m_vSelCenter[VI(VEC_X, pView)];
		pSpot->m_fMY = pDocument->m_vSelMins[VI(VEC_Y, pView)];
		pSpot->m_nDragRestrictions = HSDR_VERT;
		pSpot->m_nDrawFlags = HSDF_ROUND;
		pSpot->m_pEventResponder = this;
		pSpot->m_nDataFlags = DF_SIZE_CENX|DF_SIZE_MINY;
		pSpot->m_nCursor = TC_IDC_SIZENS;
		pHotSpotManager->AddHotSpot(pSpot);
		// CENX/MAXY
		pSpot = new CHotSpot();
		pSpot->m_fMX = pDocument->m_vSelCenter[VI(VEC_X, pView)];
		pSpot->m_fMY = pDocument->m_vSelMaxs[VI(VEC_Y, pView)];
		pSpot->m_nDragRestrictions = HSDR_VERT;
		pSpot->m_nDrawFlags = HSDF_ROUND;
		pSpot->m_pEventResponder = this;
		pSpot->m_nDataFlags = DF_SIZE_CENX|DF_SIZE_MAXY;
		pSpot->m_nCursor = TC_IDC_SIZENS;
		pHotSpotManager->AddHotSpot(pSpot);
		// MINX/CENY
		pSpot = new CHotSpot();
		pSpot->m_fMX = pDocument->m_vSelMins[VI(VEC_X, pView)];
		pSpot->m_fMY = pDocument->m_vSelCenter[VI(VEC_Y, pView)];
		pSpot->m_nDragRestrictions = HSDR_HORZ;
		pSpot->m_nDrawFlags = HSDF_ROUND;
		pSpot->m_pEventResponder = this;
		pSpot->m_nDataFlags = DF_SIZE_MINX|DF_SIZE_CENY;
		pSpot->m_nCursor = TC_IDC_SIZEWE;
		pHotSpotManager->AddHotSpot(pSpot);
		// MAXX/CENY
		pSpot = new CHotSpot();
		pSpot->m_fMX = pDocument->m_vSelMaxs[VI(VEC_X, pView)];
		pSpot->m_fMY = pDocument->m_vSelCenter[VI(VEC_Y, pView)];
		pSpot->m_nDragRestrictions = HSDR_HORZ;
		pSpot->m_nDrawFlags = HSDF_ROUND;
		pSpot->m_pEventResponder = this;
		pSpot->m_nDataFlags = DF_SIZE_MAXX|DF_SIZE_CENY;
		pSpot->m_nCursor = TC_IDC_SIZEWE;
		pHotSpotManager->AddHotSpot(pSpot);
	}

}

void CPointerModeGenerator::OnHotSpotDragRotate(CWindowEventManager* pEventManager, CHotSpotManager* pHotSpotManager, CHotSpot* pHotSpot, CMapView* pView, CTread3DDoc* pDocument, int nMouseX, int nMouseY, int nButtons)
{
	float fX, fY;

	fX = WX_TO_MX(nMouseX, pView);
	fY = WY_TO_MY(nMouseY, pView);

	// Same?
	if(m_fX == fX && m_fY == fY)
		return;

	m_bMoved = true;

	// Compute change in vectors.
	vec3_t vOld, vNew;

	vNew[VI(VEC_X, pView)] = fX;
	vNew[VI(VEC_Y, pView)] = fY;
	vNew[VI(VEC_Z, pView)] = 0.0F;
	VectorSubtract(vNew, pDocument->m_vSelCenter, vNew);
	VectorNormalize(vNew);

	float fNDX = vNew[VI(VEC_X, pView)];
	float fNDY = vNew[VI(VEC_Y, pView)];

	vOld[VI(VEC_X, pView)] = m_fX;
	vOld[VI(VEC_Y, pView)] = m_fY;
	vOld[VI(VEC_Z, pView)] = 0.0F;
	VectorSubtract(vOld, pDocument->m_vSelCenter, vOld);
	VectorNormalize(vOld);
	
	float fODX = vOld[VI(VEC_X, pView)];
	float fODY = vOld[VI(VEC_Y, pView)];

	// Get angle of change.
	float fNewAngle = atan(fNDY / fNDX);
	float fOldAngle = atan(fODY / fODX);

	// Flip?
	if(fNDX > 0.0F)
		fNewAngle += PI;
	if(fODX > 0.0F)
		fOldAngle += PI;

	float fAngle = fNewAngle - fOldAngle;

	// For some reason, the front view is flipped.
	// I honestly don't know why at this point.
	if(pView->m_nView == VIEW_FRONT)
		fAngle = -fAngle;

	// Snap to 15 degree inc?
	if((nButtons&(EF_SHIFT|EF_CONTROL))==0)
	{
		int nDeg = (fAngle/PI*180.0F);

		// Don't do less than 15.
		if(abs((float)nDeg) < 15)
			return;
		
		// Get rid of the fraction.
		nDeg = nDeg/15*15;

		// Convert back to the angle.
		fAngle = ((float)nDeg)/180.0F*PI;
	}

	// Rotate around the Z of the view.
	vec3_t vRot = {0.0F, 0.0F, 0.0F};
	vRot[VI(VEC_Z, pView)] = fAngle;
	m_vTotalRot[VI(VEC_Z, pView)] += fAngle;

	GDATA GData;
	GData.m_pList = pDocument->GetSelectedObjectList();
	VectorCopy(pDocument->m_vSelCenter, GData.m_vOrigin);
	G_Rotate(vRot, &GData);

	m_fX = fX;
	m_fY = fY;

	int nFlags;
	
	if(pDocument->GetUpdateAllViews())
		nFlags = FLAT_VIEWS;
	else
		nFlags = pView->m_nView;

	if(pDocument->GetGlobalRenderFlags()&RF_REALTIME3D)
		nFlags |= VIEW_3D;
	pDocument->UpdateWindows(nFlags);
}

void CPointerModeGenerator::OnHotSpotDragObject(CWindowEventManager* pEventManager, CHotSpotManager* pHotSpotManager, CHotSpot* pHotSpot, CMapView* pView, CTread3DDoc* pDocument, int nMouseX, int nMouseY, int nButtons)
{
	if(abs((float)nMouseX-m_nX) < SENSATIVITY)
		if(abs((float)nMouseY-m_nY) < SENSATIVITY)
			return;

	if(!m_bSnap)
	{
		// Lock?
		if(pDocument->GetGridSnap())
		{
			m_fX = G_SnapFloat(m_fX, pDocument->GetGridSize());
			m_fY = G_SnapFloat(m_fY, pDocument->GetGridSize());

			// Snap the selections.
			if(pDocument->GetSelectedObjectCount(OBJECT_CLASS_ALL&~OBJECT_CLASS_ENTITY) > 0)
			{
				GDATA GData;
				GData.m_pList = pDocument->GetSelectedObjectList();
				VectorCopy(pDocument->m_vSelMins, GData.m_vMins);
				G_Snap(&GData, pDocument->GetGridSize(), m_vMoveTotal);
				m_bMoved = !VectorIsZero(m_vMoveTotal);
			}
			else
			{
				// Translate and snap each entity.
				CBaseObject* pObject;
				CEntity* pEntity;
				CLinkedList<CBaseObject>* pList = pDocument->GetSelectedObjectList();
				pList->First();

				for(pObject = pList->GetCurItem(); pObject != NULL; pObject = pList->GetNextItem())
				{
					pEntity = (CEntity*)pObject;
					G_SnapVector(pEntity->m_vOrigin, pEntity->m_vOrigin, pDocument->GetGridSize(), m_vMoveTotal);
				}

				// For undo.
				m_bMoved = false;//!VectorIsZero(vMoveTotal);
			}
		}

		m_bSnap = true;
	}
	else
	{
		// Drag the selections.
		float fX = WX_TO_MX((float)nMouseX, pView);
		float fY = WY_TO_MY((float)nMouseY, pView);

		// Snap?
		if(pDocument->GetGridSnap())
		{
			fX = G_SnapFloatDelta(fX, m_fX, pDocument->GetGridSize());
			fY = G_SnapFloatDelta(fY, m_fY, pDocument->GetGridSize());
		}

		// Change?
		float fDX = fX - m_fX;
		float fDY = fY - m_fY;

		if(fDX == 0.0F && fDY == 0.0F)
			return;

		m_nX = nMouseX;
		m_nY = nMouseY;
		m_bMoved = true;

		// Translate the selections.
		vec3_t vTrans;

		vTrans[VI(VEC_X, pView)] = fDX;
		vTrans[VI(VEC_Y, pView)] = fDY;
		vTrans[VI(VEC_Z, pView)] = 0.0F;

		VectorAdd(vTrans, m_vMoveTotal, m_vMoveTotal);

		if(pDocument->GetSelectedObjectCount(OBJECT_CLASS_ALL&~OBJECT_CLASS_ENTITY) > 0)
		{
			GDATA GData;
			GData.m_pList = pDocument->GetSelectedObjectList();
			G_Translate(vTrans, &GData);
		}
		else
		{
			// Translate and snap each entity.
			CBaseObject* pObject;
			CEntity* pEntity;
			CLinkedList<CBaseObject>* pList = pDocument->GetSelectedObjectList();
			pList->First();

			for(pObject = pList->GetCurItem(); pObject != NULL; pObject = pList->GetNextItem())
			{
				pEntity = (CEntity*)pObject;
				pEntity->Translate(vTrans);

				// Snap?
				if(pDocument->GetGridSnap())
					G_SnapVector(pEntity->m_vOrigin, pEntity->m_vOrigin, pDocument->GetGridSize());
			}
		}

		m_fX = fX;
		m_fY = fY;
	}

	// Update.
	int nFlags;

	if(pDocument->GetUpdateAllViews())
		nFlags = FLAT_VIEWS;
	else
		nFlags = pView->m_nView;

	if(pDocument->GetGlobalRenderFlags()&RF_REALTIME3D)
		nFlags |= VIEW_3D;
	pDocument->UpdateWindows(nFlags);
}

void CPointerModeGenerator::OnHotSpotDragNormal(CWindowEventManager* pEventManager, CHotSpotManager* pHotSpotManager, CHotSpot* pHotSpot, CMapView* pView, CTread3DDoc* pDocument, int nMouseX, int nMouseY, int nButtons)
{
	float fOldX, fOldY;
	vec3_t vMins, vMaxs, vSize, vNewSize;//, vOrigin;
	
	fOldX = m_fX; fOldY = m_fY;
	VectorCopy(pDocument->m_vSelMins, vMins);
	VectorCopy(pDocument->m_vSelMaxs, vMaxs);
	VectorSubtract(vMaxs, vMins, vSize);

	if(pDocument->GetGridSnap())
	{
		if(m_bSnap)
		{
			// Snap first.
			m_fX = G_SnapFloat(WX_TO_MX(nMouseX, pView), pDocument->GetGridSize());
			m_fY = G_SnapFloat(WY_TO_MY(nMouseY, pView), pDocument->GetGridSize());
			m_bSnap = false;
		}

		// Snap to new position.
		m_fX = G_SnapFloatDelta(WX_TO_MX(nMouseX, pView), m_fX, pDocument->GetGridSize());
		m_fY = G_SnapFloatDelta(WY_TO_MY(nMouseY, pView), m_fY, pDocument->GetGridSize());
	}
	else
	{
		m_fX = WX_TO_MX(nMouseX, pView);
		m_fY = WY_TO_MY(nMouseY, pView);
	}

	if(m_fX == fOldX && m_fY == fOldY)
		return;

	m_bMoved = true;

	// Size it.
	if(pHotSpot->m_nDataFlags&DF_SIZE_MINX)
		pDocument->m_vSelMins[VI(VEC_X, pView)] = m_fX;
	if(pHotSpot->m_nDataFlags&DF_SIZE_MAXX)
		pDocument->m_vSelMaxs[VI(VEC_X, pView)] = m_fX;
	if(pHotSpot->m_nDataFlags&DF_SIZE_MINY)
		pDocument->m_vSelMins[VI(VEC_Y, pView)] = m_fY;
	if(pHotSpot->m_nDataFlags&DF_SIZE_MAXY)
		pDocument->m_vSelMaxs[VI(VEC_Y, pView)] = m_fY;

	// Ensure right size.
	VectorSubtract(pDocument->m_vSelMaxs, pDocument->m_vSelMins, vNewSize);
	if(vNewSize[VI(VEC_X, pView)] < 1.0F)
	{
		// Clamp.
		pDocument->m_vSelMins[VI(VEC_X, pView)] = vMins[VI(VEC_X, pView)];
		pDocument->m_vSelMaxs[VI(VEC_X, pView)] = vMaxs[VI(VEC_X, pView)];
		m_fX = fOldX;
	}
	
	if(vNewSize[VI(VEC_Y, pView)] < 1.0F)
	{
		// Clamp.
		pDocument->m_vSelMins[VI(VEC_Y, pView)] = vMins[VI(VEC_Y, pView)];
		pDocument->m_vSelMaxs[VI(VEC_Y, pView)] = vMaxs[VI(VEC_Y, pView)];
		m_fY = fOldY;
	}

	// Compute new size.
	VectorSubtract(pDocument->m_vSelMaxs, pDocument->m_vSelMins, vNewSize);

	// Same size?
	if(vNewSize[0] == vSize[0] && vNewSize[1] == vSize[1] && vNewSize[2] == vSize[2])
	{
		pDocument->UpdateWindows(pView->m_nView);
		return;
	}

	// Move the hotspot to new location.
	pHotSpot->m_fMX = m_fX;
	pHotSpot->m_fMY = m_fY;

	// Compute origin.
	GDATA GData;
	GData.m_pList = pDocument->GetSelectedObjectList();
	GData.m_nFlags = GF_NONE;
	VectorCopy(vMins, GData.m_vMins);
	VectorCopy(vMaxs, GData.m_vMaxs);
	ComputeRangeOrigin(vMins, vMaxs, GData.m_vOrigin);

	// Size it.
	G_SetSize(vNewSize, &GData);

	// Place it.
	ComputeRangeOrigin(pDocument->m_vSelMins, pDocument->m_vSelMaxs, pDocument->m_vSelCenter);
	G_SetOrigin(pDocument->m_vSelCenter, &GData);

	int nFlags;

	if(pDocument->GetUpdateAllViews())
		nFlags = FLAT_VIEWS;
	else
		nFlags = pView->m_nView;

	if(pDocument->GetGlobalRenderFlags()&RF_REALTIME3D)
		nFlags |= VIEW_3D;
	pDocument->UpdateWindows(nFlags);
}

void CPointerModeGenerator::OnHotSpotDrag(CWindowEventManager* pEventManager, CHotSpotManager* pHotSpotManager, CHotSpot* pHotSpot, CMapView* pView, CTread3DDoc* pDocument, int nMouseX, int nMouseY, int nButtons, int nFlags)
{
	if(pHotSpot->m_nClass == OBJECT_CLASS_BRUSH)
		OnHotSpotDragObject(pEventManager, pHotSpotManager, pHotSpot, pView, pDocument, nMouseX, nMouseY, nButtons);
	else if((pDocument->GetModePtr()->GetSubmode()&POINTER_ROTATE)==0)
		OnHotSpotDragNormal(pEventManager, pHotSpotManager, pHotSpot, pView, pDocument, nMouseX, nMouseY, nButtons);
	else
		OnHotSpotDragRotate(pEventManager, pHotSpotManager, pHotSpot, pView, pDocument, nMouseX, nMouseY, nButtons);
}

void CPointerModeGenerator::OnHotSpotMouseUp(CWindowEventManager* pEventManager, CHotSpotManager* pHotSpotManager, CHotSpot* pHotSpot, CMapView* pView, CTread3DDoc* pDocument, int nMouseX, int nMouseY, int nButtons, int nFlags)
{
	pEventManager->EnableAutoScroll(NULL, false);
	pDocument->RemoveGlobalRenderFlags(RF_NOHOTSPOTS|RF_NOSELBOX|RF_FAST|RF_NOSELBOXSIZE);
	pDocument->BuildSelectionBBox();

	// Undo anything?
	if(m_bMoved)
	{
		UNDOITEM* pItem = CUndoAPI::CreateUndoItem(pDocument->GetSelectedObjectList());
		if(pHotSpot->m_nClass == OBJECT_CLASS_BRUSH)
		{
			pItem->nFlags = UAF_TRANSLATE;
			VectorCopy(m_vMoveTotal, pItem->vUVals);
		}
		else if((pDocument->GetModePtr()->GetSubmode()&POINTER_ROTATE)==0)
		{
			pItem->nFlags = UAF_SIZE;
			VectorCopy(m_vSize, pItem->vUVals);
			VectorCopy(m_vOrigin, pItem->vUOrigin);
			VectorCopy(pDocument->m_vSelSize, pItem->vRVals);
			VectorCopy(pDocument->m_vSelCenter, pItem->vROrigin);
		}
		else
		{
			pItem->nFlags = UAF_ROTATE;
			VectorCopy(m_vTotalRot, pItem->vUVals);
			VectorCopy(m_vOrigin, pItem->vUOrigin);
		}

		pDocument->GetUndoAPI()->AddUndoItem(pItem, pDocument);
	}

	pDocument->BuildHotSpots();
	pDocument->UpdateWindows();
}

void CPointerModeGenerator::OnHotSpotMouseDown(CWindowEventManager* pEventManager, CHotSpotManager* pHotSpotManager, CHotSpot* pHotSpot, CMapView* pView, CTread3DDoc* pDocument, int nMouseX, int nMouseY, int nButtons, int nFlags)
{
	// Drag the center of the object.
	if(pHotSpot->m_nClass == OBJECT_CLASS_BRUSH)
	{
		// Does this object have a head group?
		CBaseGroupableObject* pObj = (CBaseGroupableObject*)pHotSpot->m_pData;
		CObjectGroup* pGrp = pObj->GetHeadGroup();
		if(pGrp != NULL)
			pObj = pGrp;

		if(pDocument->GetSelectedObjectList()->InList(pObj))
		{
			if((nButtons&EF_CONTROL)==EF_CONTROL)
				pDocument->AddObjectToMap(pObj);
		}
		else
		{
			if((nButtons&EF_CONTROL)==0)
				pDocument->ClearSelection();
			pDocument->AddObjectToSelection(pObj);
		}

		// We're a dragging.
		m_bSnap = false;
		m_bMoved = false;

		m_nX = nMouseX;
		m_nY = nMouseY;
		m_fX = WX_TO_MX(m_nX, pView);
		m_fY = WY_TO_MY(m_nY, pView);
		pDocument->BuildSelectionBBox();
		VectorClear(m_vMoveTotal);
		pEventManager->EnableAutoScroll(pHotSpotManager);
		pDocument->AddGlobalRenderFlags(RF_NOHOTSPOTS|RF_NOSELBOX|RF_FAST);
		pDocument->UpdateWindows(pView->m_nView);
		pDocument->CycleColorIfSelected();

		return;
	}

	m_bSnap = true;
	m_bMoved = false;
	VectorCopy(pDocument->m_vSelCenter, m_vOrigin);
	VectorCopy(pDocument->m_vSelSize, m_vSize);
	VectorClear(m_vTotalRot);
	m_fX = WX_TO_MX(nMouseX, pView);
	m_fY = WY_TO_MY(nMouseY, pView);
	pEventManager->EnableAutoScroll(pHotSpotManager);
	pDocument->AddGlobalRenderFlags(RF_NOHOTSPOTS|RF_FAST);
	pDocument->UpdateWindows(pView->m_nView);
}

void CPointerModeGenerator::OnHotSpotMouseEnter(CWindowEventManager* pEventManager, CHotSpotManager* pHotSpotManager, CHotSpot* pHotSpot, CMapView* pView, CTread3DDoc* pDocument, int nMouseX, int nMouseY, int nButtons, int nFlags) 
{
}

void CPointerModeGenerator::OnHotSpotMouseLeave(CWindowEventManager* pEventManager, CHotSpotManager* pHotSpotManager, CHotSpot* pHotSpot, CMapView* pView, CTread3DDoc* pDocument, int nMouseX, int nMouseY, int nButtons, int nFlags) 
{
}
