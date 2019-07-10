// UNDOAPI.CPP
// Copyright © 1999 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#include "stdafx.h"
#include "MapObject.h"
#include "Geometry.h"
#include "UndoAPI.h"
#include "Tread3DDoc.h"
#include "TreadSys.h"
#include "RenderAPI.h"

CUndoAPI::CUndoAPI()
{
	m_pStackPos = NULL;
	m_bEOL = false;
}

// Clears the undo stack.
void CUndoAPI::ClearUndoStack(CTread3DDoc* pDocument)
{
	while(m_pStackPos != NULL && m_UndoList.GetItem(LL_HEAD) != m_pStackPos)
		PurgePP(pDocument, 1);
	while(m_UndoList.IsEmpty() == false)
		PurgeBF(pDocument, 1);

	m_pStackPos = NULL;
	m_bEOL = false;
}

void CUndoAPI::Rotate(UNDOITEM* pItem, vec3_t vRot, vec3_t vOrigin)
{
	UNDOREF* pRef;
	CBaseTransformableObject* pTrans;

	pItem->RefList.First();
	for(pRef = pItem->RefList.GetCurItem(); pRef != NULL; pRef = pItem->RefList.GetNextItem())
	{
		pTrans = (CBaseTransformableObject*)pRef->pObj->GetInterface(I_TRANSFORMABLE);
		if(pTrans != NULL)
			pTrans->Rotate(vOrigin, vRot);
	}
}

void CUndoAPI::PurgeLastItem(CTread3DDoc* pDocument)
{
	PurgeBF(pDocument, 1);	
}

void CUndoAPI::PurgePP(CTread3DDoc* pDocument, int nCount)
{
	int i;
	UNDOITEM* pItem;

	for(i = 0; i < nCount; i++)
	{
		m_UndoList.SetPosition(m_pStackPos);

		pItem = m_UndoList.GetPrevItem();
		if(pItem == NULL)
			return;
		
		m_UndoList.RemoveItem(pItem);

		if(pItem->nFlags&UAF_TAIL)
			PurgePP(pDocument, pItem->nCount);

		// Delete?
		if(pItem->nFlags&UAF_DELETE)
		{
			/*UNDOREF* pRef;
			pItem->RefList.First();
			for(;;)
			{
				pRef = pItem->RefList.RemoveItem(LL_HEAD);
				if(pRef == NULL)
					break;
				if(pRef->pObj != NULL)
					delete pRef->pObj;
				delete pRef;
			}*/
		}
		if(pItem->nFlags&(UAF_GROUP) && (pItem->nFlags&(UAF_HEAD|UAF_TAIL)) == 0)
		{
			//pDocument->RemoveObjectFromMap((CBaseObject*)pItem->pData);
			delete (CBaseObject*)pItem->pData;
		}
		if(pItem->nFlags&(UAF_HOLLOW|UAF_CARVE) && (pItem->nFlags&(UAF_HEAD|UAF_TAIL)) == 0)
		{
			//pDocument->RemoveObjectFromMap((CBaseObject*)pItem->pData2);
			delete (CBaseObject*)pItem->pData2;
		}

/*
		if(m_pStackPos == pItem)
			m_pStackPos = m_UndoList.GetItem(LL_HEAD);
*/

		delete pItem;
	}

}

void CUndoAPI::PurgeBF(CTread3DDoc* pDocument, int nCount)
{
	int i;

	for(i = 0; i < nCount; i++)
	{
		UNDOITEM* pItem = m_UndoList.RemoveItem(LL_TAIL);
		if(pItem == NULL)
			return;

		if(pItem->nFlags&UAF_TAIL)
			PurgeBF(pDocument, pItem->nCount);
		
		// Delete?
		if(pItem->nFlags&UAF_DELETE)
		{
			UNDOREF* pRef;
			pItem->RefList.First();
			for(;;)
			{
				pRef = pItem->RefList.RemoveItem(LL_HEAD);
				if(pRef == NULL)
					break;
				if(pRef->pObj != NULL)
					delete pRef->pObj;
				delete pRef;
			}
		}
		if(pItem->nFlags&(UAF_GROUP) && (pItem->nFlags&(UAF_HEAD|UAF_TAIL)) == 0)
		{
			//pDocument->RemoveObjectFromMap((CBaseObject*)pItem->pData);
			delete (CBaseObject*)pItem->pData;
		}
		if(pItem->nFlags&(UAF_HOLLOW|UAF_CARVE) && (pItem->nFlags&(UAF_HEAD|UAF_TAIL)) == 0)
		{
			//pDocument->RemoveObjectFromMap((CBaseObject*)pItem->pData2);
			delete (CBaseObject*)pItem->pData2;
		}

		if(m_pStackPos == pItem)
			m_pStackPos = m_UndoList.GetItem(LL_TAIL);
		delete pItem;
	}
}

void CUndoAPI::SetSize(UNDOITEM* pItem, vec3_t vOldSize, vec3_t vNewSize, vec3_t vOrigin)
{
	vec3_t vScale;

	// Make sure we don't have a 0 somewhere.
	int i;
	for(i = 0; i < 3; i++)
	{
		if(vOldSize[i] < 1.0F && vOldSize[i] > -1.0F)
			vOldSize[i] = 1.0F;
	}

	VectorDivide(vNewSize, vOldSize, vScale);
	Scale(pItem, vScale, vOrigin);
}

void CUndoAPI::Translate(UNDOITEM* pItem, vec3_t vVec)
{
	UNDOREF* pRef;
	CBaseTransformableObject* pTrans;

	pItem->RefList.First();
	for(pRef = pItem->RefList.GetCurItem(); pRef != NULL; pRef = pItem->RefList.GetNextItem())
	{
		pTrans = (CBaseTransformableObject*)pRef->pObj->GetInterface(I_TRANSFORMABLE);
		if(pTrans != NULL)
			pTrans->Translate(vVec);
	}
}

void CUndoAPI::SetOrigin(UNDOITEM* pItem, vec3_t vOldOrigin, vec3_t vNewOrigin)
{
	// Compute difference from origins.
	vec3_t vDiff;
	VectorSubtract(vNewOrigin, vOldOrigin, vDiff);
	Translate(pItem, vDiff);
}

void CUndoAPI::Scale(UNDOITEM* pItem, vec3_t vScale, vec3_t vOrigin)
{
	UNDOREF* pRef;
	CBaseTransformableObject* pTrans;

	pItem->RefList.First();
	for(pRef = pItem->RefList.GetCurItem(); pRef != NULL; pRef = pItem->RefList.GetNextItem())
	{
		pTrans = (CBaseTransformableObject*)pRef->pObj->GetInterface(I_TRANSFORMABLE);
		if(pTrans != NULL)
			pTrans->Scale(vScale, vOrigin);
	}
}

// Delete all items until we hit m_pStackPos.
void CUndoAPI::PurgeFromPos(CTread3DDoc* pDocument)
{
	while(m_UndoList.GetItem(LL_HEAD) != m_pStackPos)
		PurgePP(pDocument, 1);
}

// Add's an undo item.
void CUndoAPI::AddUndoItem(UNDOITEM* pItem, CTread3DDoc* pDocument)
{
	if(m_bEOL)
		ClearUndoStack(pDocument);
	else if(m_pStackPos != NULL)
		PurgeFromPos(pDocument);

	m_UndoList.AddToHead(pItem);
	if(m_UndoList.GetCount() > 50)
		PurgeLastItem(pDocument);
	m_pStackPos = pItem;

	// What does the item specify?
	switch(pItem->nFlags&~(UAF_HEAD|UAF_TAIL))
	{
	case UAF_TRANSLATE:
	case UAF_SIZE:
	case UAF_ROTATE:
		pItem->nURF=URF_CLEARSEL|URF_ADDTOSEL|URF_BUILDSELBOX|URF_BUILDHOTSPOTS;
		break;
	case UAF_DELETE:
	case UAF_CREATE:
		pItem->nURF=URF_BUILDSELBOX|URF_BUILDHOTSPOTS;
	}
}

UNDOITEM* CUndoAPI::CreateUndoItem(CLinkedList<CBaseObject>* pList)
{
	UNDOITEM* pItem = new UNDOITEM;
	UNDOREF* pRef;
	CBaseObject* pObj;

	pList->First();
	for(pObj = pList->GetCurItem(); pObj != NULL; pObj = pList->GetNextItem())
	{
		pRef = new UNDOREF();
		pRef->pObj = pObj;
		pItem->RefList.AddItem(pRef);
	}

	return pItem;
}

CString CUndoAPI::GetUndoString()
{
	if(m_bEOL)
		return"";
	UNDOITEM* pItem = m_UndoList.GetItem(LL_HEAD);
	if(pItem == NULL)
		return "";
	switch(pItem->nFlags&~(UAF_HEAD|UAF_TAIL))
	{
	case UAF_DELETE: return "Undo Delete";
	case UAF_CREATE: return "Undo Create";
	case UAF_CARVE: return "Undo Carve";
	case UAF_ROTATE: return "Undo Rotate";
	case UAF_TRANSLATE: return "Undo Drag";
	case UAF_SIZE: return "Undo Size";
	case UAF_HOLLOW: "Undo Hollow";
	}

	return "";
}

UNDOITEM* CUndoAPI::GetRedoItem()
{
	if(m_pStackPos == NULL)
		return NULL;

	if(m_bEOL)
		return m_pStackPos;

	// Redo Item is always one forward from m_pStackPos.
	m_UndoList.SetPosition(m_pStackPos);
	return m_UndoList.GetPrevItem();
}

CString CUndoAPI::GetRedoString()
{
	UNDOITEM* pItem = GetRedoItem();
	if(pItem == NULL)
		return "";
	switch(pItem->nFlags&~(UAF_HEAD|UAF_TAIL))
	{
	case UAF_DELETE: return "Redo Delete";
	case UAF_CREATE: return "Redo Create";
	case UAF_CARVE: return "Redo Carve";
	case UAF_ROTATE: return "Redo Rotate";
	case UAF_TRANSLATE: return "Redo Drag";
	case UAF_SIZE: return "Redo Size";
	case UAF_HOLLOW: "Redo Hollow";
	}

	return "";
}

bool CUndoAPI::CanRedo()
{
	return GetRedoItem() != NULL;
}

bool CUndoAPI::CanUndo()
{
	return m_pStackPos != NULL && !m_bEOL;
}

int CUndoAPI::PerformUndoRedo(UNDOITEM* pItem, CTread3DDoc* pDocument, bool bUndo)
{
	// What does the item specify?
	switch(pItem->nFlags&~(UAF_HEAD|UAF_TAIL|UAF_GMEM))
	{
	case UAF_TRANSLATE:
		{
			if(bUndo)
			{
				vec3_t vVec = {-pItem->vUVals[0], -pItem->vUVals[1], -pItem->vUVals[2]};
				Translate(pItem, vVec);
			}
			else
				Translate(pItem, pItem->vUVals);

			pDocument->ClearSelection();
			AddItemToSelection(pItem, pDocument);

		} break;
	case UAF_SIZE:
		{
			if(bUndo)
			{
				// Move back to old origin.
				SetOrigin(pItem, pItem->vROrigin, pItem->vUOrigin);
				SetSize(pItem, pItem->vRVals, pItem->vUVals, pItem->vUOrigin);
			}
			else
			{
				// Move to new origin.
				SetOrigin(pItem, pItem->vUOrigin, pItem->vROrigin);
				SetSize(pItem, pItem->vUVals, pItem->vRVals, pItem->vROrigin);
			}

			pDocument->ClearSelection();
			AddItemToSelection(pItem, pDocument);

		} break;
	case UAF_DELETE:
		{
			if(bUndo)
			{
				// Replace the item into the map.
				pDocument->ClearSelection();
				AddItemToSelection(pItem, pDocument);
			}
			else
				// Remove the item from the map.
				RemoveItemFromMap(pItem, pDocument);

		} break;
	case UAF_CREATE:
		{
			if(bUndo)
				// Remove the item.
				RemoveItemFromMap(pItem, pDocument);
			else
			{
				// Replace.
				pDocument->ClearSelection();
				AddItemToSelection(pItem, pDocument);
			}

		} break;
	case UAF_ROTATE:
		{
			if(bUndo)
			{
				// Rotate negative.
				vec3_t vRot = { -pItem->vUVals[0], -pItem->vUVals[1], -pItem->vUVals[2] };
				Rotate(pItem, vRot, pItem->vUOrigin);
			}
			else
				Rotate(pItem, pItem->vUVals, pItem->vUOrigin);

			pDocument->ClearSelection();
			AddItemToSelection(pItem, pDocument);

		} break;

	case UAF_GROUP:
		{
			if(bUndo)
			{
				CObjectGroup* pGroup;	
				CBaseObject* pGrpObj;
				CLinkedList<CBaseObject>* pUngroupList = new CLinkedList<CBaseObject>();

				pDocument->ClearSelection();

				// Dismantle this group.
				pGroup = (CObjectGroup*)pItem->pData;
				for(;;)
				{
					pGroup->GetObjectList()->First();
					pGrpObj = pGroup->GetObjectList()->GetCurItem();
					if(pGrpObj == NULL)
						break;
					pGroup->RemoveObject((CBaseGroupableObject*)pGrpObj->GetInterface(I_GROUPABLE));
					pUngroupList->AddItem(pGrpObj);
				}

				pDocument->RemoveObjectFromMap(pGroup);
				// Transfer the others to the selected list.
				G_AddToSelection(pUngroupList, pDocument);
				// Delete the old list.
				delete pUngroupList;

				return URF_BUILDSELBOX|URF_BUILDHOTSPOTS;
			}
			else
			{
				CObjectGroup* pGroup = (CObjectGroup*)pItem->pData;
				UNDOREF* pRef;
				CLinkedList<UNDOREF>* pList = &pItem->RefList;

				pList->First();
				for(;;)
				{
					pRef = pList->GetCurItem();
					if(pRef == NULL)
						break;

					pDocument->RemoveObjectFromMap(pRef->pObj);

					if(pRef->pObj->GetObjectInterfaces()&I_GROUPABLE)
						pGroup->AddObject((CBaseGroupableObject*)pRef->pObj->GetInterface(I_GROUPABLE));

					pList->GetNextItem();
				}

				// Add the group.
				pGroup->BuildRange();
				pDocument->ClearSelection();				
				pDocument->AddObjectToSelection(pGroup);
				
				return URF_BUILDSELBOX|URF_BUILDHOTSPOTS;
			}

		} break;

		case UAF_UNGROUP:
		{
			if(bUndo)
			{
				if(pItem->nFlags&UAF_TAIL)	// Illegal.
					return URF_BUILDSELBOX|URF_BUILDHOTSPOTS;

				// Group?
				if(pItem->nFlags&UAF_HEAD)
					return URF_CLEARSEL;

				CObjectGroup* pGroup = (CObjectGroup*)pItem->pData;
				UNDOREF* pRef;
				CLinkedList<UNDOREF>* pList = &pItem->RefList;

				pList->First();
				for(;;)
				{
					pRef = pList->GetCurItem();
					if(pRef == NULL)
						break;

					pDocument->RemoveObjectFromMap(pRef->pObj);

					if(pRef->pObj->GetObjectInterfaces()&I_GROUPABLE)
						pGroup->AddObject((CBaseGroupableObject*)pRef->pObj->GetInterface(I_GROUPABLE));

					pList->GetNextItem();
				}

				// Add the group.
				pGroup->BuildRange();
				pDocument->AddObjectToSelection(pGroup);
				
				return URF_OK;
			}
			else
			{
				// Group?
				if(pItem->nFlags&UAF_HEAD)	// Illegal.
					return URF_BUILDSELBOX|URF_BUILDHOTSPOTS;
				if(pItem->nFlags&UAF_TAIL)
					return URF_CLEARSEL;

				CObjectGroup* pGroup;	
				CBaseObject* pGrpObj;
				CLinkedList<CBaseObject>* pUngroupList = new CLinkedList<CBaseObject>();

				// Dismantle this group.
				pGroup = (CObjectGroup*)pItem->pData;
				for(;;)
				{
					pGroup->GetObjectList()->First();
					pGrpObj = pGroup->GetObjectList()->GetCurItem();
					if(pGrpObj == NULL)
						break;
					pGroup->RemoveObject((CBaseGroupableObject*)pGrpObj->GetInterface(I_GROUPABLE));
					pUngroupList->AddItem(pGrpObj);
				}

				pDocument->RemoveObjectFromMap(pGroup);
				// Transfer the others to the selected list.
				G_AddToSelection(pUngroupList, pDocument);
				// Delete the old list.
				delete pUngroupList;

				return URF_OK;
			}

		} break;

		case UAF_HOLLOW:
		case UAF_CARVE:
		{		
			if(bUndo)
			{
				if(pItem->nFlags&UAF_HEAD)
					return URF_CLEARSEL;
				if(pItem->nFlags&UAF_TAIL)
					return URF_BUILDSELBOX|URF_BUILDHOTSPOTS;

				// Swap the objects.
				pDocument->RemoveObjectFromMap((CBaseObject*)pItem->pData2);
				pDocument->AddObjectToSelection((CBaseObject*)pItem->pData);
				return URF_OK;
			}
			else
			{
				if(pItem->nFlags&UAF_TAIL)
					return URF_CLEARSEL;
				if(pItem->nFlags&UAF_HEAD)
					return URF_BUILDSELBOX|URF_BUILDHOTSPOTS;

				// Swap.
				pDocument->RemoveObjectFromMap((CBaseObject*)pItem->pData);
				pDocument->AddObjectToSelection((CBaseObject*)pItem->pData2);
				return URF_OK;
			}
			
		} break;
	}

	return URF_BUILDSELBOX|URF_BUILDHOTSPOTS;
}

void CUndoAPI::AddItemToMap(UNDOITEM* pItem, CTread3DDoc* pDocument)
{
	UNDOREF* pRef;
	pItem->RefList.First();
	pRef = pItem->RefList.GetCurItem();
	if(pRef != NULL)
	{
		for(;;)
		{
			pDocument->AddObjectToMap(pRef->pObj);
			pRef = pItem->RefList.GetNextItem();
			if(pRef == NULL)
				break;
		}
	}
}

void CUndoAPI::RemoveItemFromMap(UNDOITEM* pItem, CTread3DDoc* pDocument)
{
	UNDOREF* pRef;
	pItem->RefList.First();
	pRef = pItem->RefList.GetCurItem();
	if(pRef != NULL)
	{
		for(;;)
		{
			pDocument->RemoveObjectFromMap(pRef->pObj);
			pRef = pItem->RefList.GetNextItem();
			if(pRef == NULL)
				break;
		}
	}
}

void CUndoAPI::AddItemToSelection(UNDOITEM* pItem, CTread3DDoc* pDocument)
{
	UNDOREF* pRef;
	pItem->RefList.First();
	pRef = pItem->RefList.GetCurItem();
	if(pRef != NULL)
	{
		for(;;)
		{
			pDocument->AddObjectToSelection(pRef->pObj);
			pRef = pItem->RefList.GetNextItem();
			if(pRef == NULL)
				break;
		}
	}
}

int CUndoAPI::Redo(CTread3DDoc* pDocument)
{
	UNDOITEM* pRedoItem = GetRedoItem();
	if(pRedoItem == NULL)
		return URF_FAIL;

	// For Undo.
	m_bEOL = false;
	m_pStackPos = pRedoItem;
	m_UndoList.SetPosition(pRedoItem);

	// Apply the undo to the list.
	if((pRedoItem->nFlags&(UAF_HEAD|UAF_TAIL)) == 0)
		G_Redo(&m_pStackPos->RefList, pDocument, m_pStackPos->nFlags);

	int nURF = PerformUndoRedo(m_pStackPos, pDocument, false);
	if(nURF == URF_FAIL)
		return URF_FAIL;

	if(nURF&URF_CLEARSEL)
		pDocument->ClearSelection();
	if(nURF&URF_ADDTOSEL)
		AddItemToSelection(m_pStackPos, pDocument);
	if(nURF&URF_BUILDSELBOX)
	{
		pDocument->RemoveGlobalRenderFlags(RF_NOSELBOX);
		pDocument->BuildSelectionBBox();
	}
	if(nURF&URF_BUILDHOTSPOTS)
		pDocument->BuildHotSpots();

	// Is this a tail/count?
	if(pRedoItem->nFlags&UAF_TAIL)
	{
		int i;
		for(i = 0; i < pRedoItem->nCount; i++)
			Redo(pDocument);	// Perform undo procedure.
	}

	return URF_OK;
}

int CUndoAPI::Undo(CTread3DDoc* pDocument)
{
	UNDOITEM* pCurItem = m_pStackPos;
	if(m_pStackPos == NULL || m_bEOL)
		return URF_FAIL;

	m_UndoList.SetPosition(m_pStackPos);

	// Apply the undo to the list.
	if((pCurItem->nFlags&(UAF_HEAD|UAF_TAIL)) == 0)
		G_Undo(&m_pStackPos->RefList, pDocument, m_pStackPos->nFlags);

	int nURF = PerformUndoRedo(m_pStackPos, pDocument, true);
	if(nURF == URF_FAIL)
		return URF_FAIL;

	if(nURF&URF_CLEARSEL)
		pDocument->ClearSelection();
	if(nURF&URF_ADDTOSEL)
		AddItemToSelection(m_pStackPos, pDocument);
	if(nURF&URF_BUILDSELBOX)
	{
		pDocument->RemoveGlobalRenderFlags(RF_NOSELBOX);
		pDocument->BuildSelectionBBox();
	}
	if(nURF&URF_BUILDHOTSPOTS)
		pDocument->BuildHotSpots();

	UNDOITEM* pItem = m_UndoList.GetNextItem();
	if(pItem != NULL)
		m_pStackPos = pItem;
	else
		m_bEOL = true;

	// Is this a head/count?
	if(pCurItem->nFlags&UAF_HEAD)
	{
		int i;
		for(i = 0; i < pCurItem->nCount; i++)
			Undo(pDocument);	// Perform undo procedure.
	}

	return URF_OK;
}