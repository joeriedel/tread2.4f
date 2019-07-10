// DEFAULTACTIONRESPONDER.CPP
// Copyright © 2000 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#include "stdafx.h"
#include "RenderAPI.h"
#include "Tread3DDoc.h"
#include "EventManager.h"
#include "HotSpotManager.h"
#include "MapObject.h"
#include "DefaultActionResponder.h"
#include "Geometry.h"
#include "TreadSys.h"
#include "Brush.h"
#include "MapView.h"
#include "UndoAPI.h"
#include "GameAPI.h"
#include "Anchor.h"

#ifdef _WIN32
#include "AnchorDialog.h"
#include "ViewControl.h"
#endif

// If an action responder does not handle an action and returns ARF_DEFAULT
// then this routine handles it.
#define LOC_MINS	0
#define LOC_MAXS	1
#define LOC_CENTER	2
#define LOC_HORZ	3
#define LOC_VERT	4

float CDefaultActionResponder::GetObjectSpace(CBaseTransformableObject* pTA, CBaseTransformableObject* pTB, int nAxis)
{
	// On the given axis, figure out how far these objects are seperated.
	// We don't need to deal with orientation.
	pTA->BuildRange();
	pTB->BuildRange();

	return (pTB->m_vMins[nAxis] - pTA->m_vMaxs[nAxis]);
}

void  CDefaultActionResponder::SpaceObjects(CBaseTransformableObject* pTA, CBaseTransformableObject* pTB, int nAxis, float fSpace)
{
	// Set the object spacing.
	vec3_t vTrans;

	VectorClear(vTrans);
	vTrans[nAxis] = (fSpace - GetObjectSpace(pTA, pTB, nAxis));
	pTB->Translate(vTrans);
}

int CDefaultActionResponder::OnAction(int nAction, void* pParam, CModeChangeEventManager* pEventManager, CMode* pModeManager, CTread3DDoc* pDocument)
{
	// Select based on the action invoked.
	switch( nAction )
	{
	case EF_ACTION_SIZEBOTH:
		{
			pEventManager->InvokeAction(EF_ACTION_SIZEHORZ, (void*)1, pDocument);
			pEventManager->InvokeAction(EF_ACTION_SIZEVERT, NULL, pDocument);
		} break;
	case EF_ACTION_SIZEHORZ:
	case EF_ACTION_SIZEVERT:
		{
			CMapView* pView = pDocument->GetCurView();
			if(pView == NULL)
			{
				Error("There is no active view!\n");
				return ARF_HANDLED;
			}

			int nAxis = (nAction==EF_ACTION_SIZEHORZ)?VI(VEC_X, pView):VI(VEC_Y, pView);
		
			// Get distance between first two objects.
			CLinkedList<CBaseObject>* pList = pDocument->GetSelectedObjectList();
			if(pDocument->GetSelectedInterfaceCount(I_TRANSFORMABLE) < 2)
			{
				Error("Unable to perform horizontal/vertical size adjustment. You must have at least 3 objects selected.");
				return ARF_HANDLED;
			}

			CBaseObject* pO;
			CBaseTransformableObject* pT=NULL;
			vec_t fToSize, fSize;
			vec3_t vScale;

			for(pO = pList->ResetPos(); pO != NULL; pO = pList->GetNextItem())
			{
				pT = (CBaseTransformableObject*)pO->GetInterface(I_TRANSFORMABLE);
				if(pT != NULL)
					break;
			}
			
			if(pT == NULL)
			{
				Error("CDefaultActionResponder::OnAction: EF_ACTION_SIZEVERT/HORZ there are not two I_TRANSFORMABLE objects selected");
				return ARF_HANDLED;
			}

			pT->BuildRange();
			fToSize = pT->m_vMaxs[nAxis] - pT->m_vMins[nAxis];

			// Starting at second object, space all others.
			CBaseObject* pPrev, *pNext;
			pList->SetPosition(pO);
			
			pPrev = pO;
			for(pNext = pList->GetNextItem(); pNext != NULL; pNext = pList->GetNextItem())
			{
				pT = (CBaseTransformableObject*)pNext->GetInterface(I_TRANSFORMABLE);
				if(pT == NULL)
					continue;

				pT->BuildRange();
				pT->ComputeOrigin();

				VectorClear(vScale);

				vScale[0] = vScale[1] = vScale[2] = 1.0f;
				fSize = pT->m_vMaxs[nAxis] - pT->m_vMins[nAxis];
				if(fSize == 0)
					vScale[nAxis] = fToSize;
				else
					vScale[nAxis] = fToSize / fSize;

				pT->Scale(vScale, pT->m_vOrigin);

				pPrev = pNext;
			}

			// Redraw views.
			if(pParam == NULL)
			{
				pDocument->BuildSelectionBBox();
				pDocument->BuildHotSpots();
				pDocument->UpdateWindows();
			}

		} break;
	case EF_ACTION_CLOSEHORZ:
	case EF_ACTION_CLOSEVERT:
		{
			CMapView* pView = pDocument->GetCurView();
			if(pView == NULL)
			{
				Error("There is no active view!\n");
				return ARF_HANDLED;
			}

			int nAxis = (nAction==EF_ACTION_CLOSEHORZ)?VI(VEC_X, pView):VI(VEC_Y, pView);

			// Get distance between first two objects.
			CLinkedList<CBaseObject>* pList = pDocument->GetSelectedObjectList();
			if(pDocument->GetSelectedInterfaceCount(I_TRANSFORMABLE) < 2)
			{
				Error("Unable to perform horizontal/vertical space adjustment. You must have at least 3 objects selected.");
				return ARF_HANDLED;
			}

			CBaseObject* pO;
			CBaseTransformableObject* pT=NULL;

			for(pO = pList->ResetPos(); pO != NULL; pO = pList->GetNextItem())
			{
				pT = (CBaseTransformableObject*)pO->GetInterface(I_TRANSFORMABLE);
				if(pT != NULL)
					break;
			}
			
			if(pT == NULL)
			{
				Error("CDefaultActionResponder::OnAction: EF_ACTION_CLOSEVERT/HORZ there are not two I_TRANSFORMABLE objects selected");
				return ARF_HANDLED;
			}

			// Starting at second object, space all others.
			CBaseObject* pPrev, *pNext;
			pList->SetPosition(pO);
			
			pPrev = pO;
			for(pNext = pList->GetNextItem(); pNext != NULL; pNext = pList->GetNextItem())
			{
				pT = (CBaseTransformableObject*)pNext->GetInterface(I_TRANSFORMABLE);
				if(pT == NULL)
					continue;

				SpaceObjects((CBaseTransformableObject*)pPrev->GetInterface(I_TRANSFORMABLE),
					pT, nAxis, 0.0f);

				pPrev = pNext;
			}

			// Redraw views.
			pDocument->BuildSelectionBBox();
			pDocument->BuildHotSpots();
			pDocument->UpdateWindows();

		} break;

	case EF_ACTION_SPACEHORZ:
	case EF_ACTION_SPACEVERT:
		{	
			//
			// NOTE: this action only really half works. The center objects get flipped if the order things
			// are selected in negates the orientation of the view. This can easily be remedied by the user
			// simply by performing a "flip" of the objects that got transposed. usually it only happens
			// in the front view.
			//
			// it should be fixed in code eventually though.
			//
			CMapView* pView = pDocument->GetCurView();
			if(pView == NULL)
			{
				Error("There is no active view!\n");
				return ARF_HANDLED;
			}

			int nAxis = (nAction==EF_ACTION_SPACEHORZ)?VI(VEC_X, pView):VI(VEC_Y, pView);

			// Get distance between first two objects.
			CLinkedList<CBaseObject>* pList = pDocument->GetSelectedObjectList();
			if(pDocument->GetSelectedInterfaceCount(I_TRANSFORMABLE) < 3)
			{
				Error("Unable to perform horizontal/vertical space adjustment. You must have at least 3 objects selected.");
				return ARF_HANDLED;
			}

			float fTrCount=0;
			CBaseObject* pO;
			CBaseTransformableObject* pT1=NULL, *pT2=NULL, *pT;

			// Find the end brackets.
			float min=999999;
			for(pO = pList->ResetPos(); pO; pO = pList->GetNextItem())
			{
				pT = IC_TRANSFORMABLE(pO);
				if(!pT)
					continue;
				pT->BuildRange();
				if(pT->m_vMins[nAxis] < min)
				{
					pT1 = pT;
					min = pT->m_vMins[nAxis];
				}
				fTrCount += 1.0f;
			}
			
			if(fTrCount < 3.0f)
			{
				Error("CDefaultActionResponder::OnAction: EF_ACTION_SPACEVERT/HORZ: must have at least 3 objects selected!\n");
				return ARF_HANDLED;
			}

			float max = -999999;
			for(pO = pList->ResetPos(); pO; pO = pList->GetNextItem())
			{
				pT = IC_TRANSFORMABLE(pO);
				if(!pT)
					continue;
				pT->BuildRange();
				if(pT != pT1 && pT->m_vMaxs[nAxis] > max)
				{
					pT2 = pT;
					max = pT->m_vMaxs[nAxis];
				}
			}

			// Get the inside distance.
			float dist = (pT2->m_vMins[nAxis] - pT1->m_vMaxs[nAxis]);
			if(dist < 1.0f)
			{
				Error("CDefaultActionResponder::OnAction: EF_ACTION_SPACEVERT/HORZ: there is no space between the endcap objects!\n");
				return ARF_HANDLED;
			}

			float occ=0.0f;
			// Add up the size of the objects between.

			for(pO = pList->ResetPos(); pO; pO = pList->GetNextItem())
			{
				pT = IC_TRANSFORMABLE(pO);
				if(!pT)
					continue;
				if(pT == pT1 || pT == pT2)
					continue;

				pT->BuildRange();
				occ += (pT->m_vMaxs[nAxis]-pT->m_vMins[nAxis]);

			}

			// Get the space amount.
			float fSpace = (dist-occ)/(fTrCount-1);
			float fPos = pT1->m_vMaxs[nAxis]+fSpace;
			vec3_t vTrans;

			for(pO = pList->ResetPos(); pO; pO = pList->GetNextItem())
			{
				pT = IC_TRANSFORMABLE(pO);
				if(!pT)
					continue;
				if(pT == pT1 || pT == pT2)
					continue;

				// This one goes to fPos.
				pT->BuildRange();
				VectorClear(vTrans);
				vTrans[nAxis] = fPos-pT->m_vMins[nAxis];
				pT->Translate(vTrans);
				pT->BuildRange();

				// Move it along!
				fPos = pT->m_vMaxs[nAxis]+fSpace;
			}
			
			// Redraw views.
			pDocument->BuildSelectionBBox();
			pDocument->BuildHotSpots();
			pDocument->UpdateWindows();

		} break;

	case EF_ACTION_ALIGNLEFT:
	case EF_ACTION_ALIGNRIGHT:
	case EF_ACTION_ALIGNTOP:
	case EF_ACTION_ALIGNBOTTOM:
	case EF_ACTION_ALIGNHORZCEN:
	case EF_ACTION_ALIGNVERTCEN:
		{
			int nAxis=VEC_X, n2DAxis;
			int nLoc=LOC_MINS;
			vec3_t vVec, vTrans, vDiff;

			CMapView* pView = pDocument->GetCurView();
			if(pView == NULL)
			{
				Warning("CDefaultActionResponder::OnAction(): EF_ACTION_ALIGN, GetCurView() returned NULL!\n");
				return ARF_HANDLED;
			}

			// Set axis.
			switch(nAction)
			{
			case EF_ACTION_ALIGNLEFT:
			case EF_ACTION_ALIGNRIGHT:
			case EF_ACTION_ALIGNHORZCEN:
				n2DAxis = VEC_X;
				nAxis = VI(VEC_X, pView);
				break;
			case EF_ACTION_ALIGNTOP:
			case EF_ACTION_ALIGNBOTTOM:
			case EF_ACTION_ALIGNVERTCEN:
				n2DAxis = VEC_Y;
				nAxis = VI(VEC_Y, pView);
				break;
			}

			// Go through each object and set new position.
			CBaseObject* pObj;
			CBaseTransformableObject* pT;
			CLinkedList<CBaseObject>* pList = pDocument->GetSelectedObjectList();

			// Get first item.
			pObj = pList->ResetPos();
			if(pObj == NULL)
				return ARF_HANDLED;
			for(;;)
			{
				pT = (CBaseTransformableObject*)pObj->GetInterface(I_TRANSFORMABLE);
				if(pT != NULL)
					break;

				pObj = pList->GetNextItem();
				if(pObj == NULL)
					return ARF_HANDLED;
			}

			pT->BuildRange();

			// Get the place to align to.
			if(nAction == EF_ACTION_ALIGNHORZCEN || nAction == EF_ACTION_ALIGNVERTCEN)
			{
				nLoc = LOC_CENTER;
				VectorCopy(pT->m_vOrigin, vVec);
			}
			else if((VMO(n2DAxis, pView) > 0 && (nAction == EF_ACTION_ALIGNLEFT || nAction == EF_ACTION_ALIGNTOP)) || (VMO(n2DAxis, pView) < 0 && (nAction == EF_ACTION_ALIGNRIGHT || nAction == EF_ACTION_ALIGNBOTTOM)))
			{
				nLoc = LOC_MINS;
				// Align via mins.
				VectorCopy(pT->m_vMins, vVec);					
			}
			else
			{
				nLoc = LOC_MAXS;
				// Align via maxs.
				VectorCopy(pT->m_vMaxs, vVec);
			}

			for(; pObj != NULL; pObj = pList->GetNextItem())
			{
				pT = (CBaseTransformableObject*)pObj->GetInterface(I_TRANSFORMABLE);
				if(pT == NULL)
					continue;
				
				pT->BuildRange();
				VectorSubtract(vVec, ((nLoc==LOC_MAXS) ? pT->m_vMaxs : (nLoc==LOC_MINS) ? pT->m_vMins : pT->m_vOrigin), vDiff);
				VectorClear(vTrans);
				vTrans[nAxis] = vDiff[nAxis];

				pT->Translate(vTrans);
			}

			pDocument->BuildSelectionBBox();
			pDocument->BuildHotSpots();
			pDocument->UpdateWindows();

		} break;

	case EF_ACTION_DROPANCHOR:
		{
			int nX, nY;
			vec3_t vOrigin;
			CMapView* pView, *pOther;

			pDocument->RetrieveXY(&nX, &nY, (void**)&pView);
			pOther = pDocument->GetOtherViewType(pView->m_nView);

			vOrigin[VI(VEC_X, pView)] = WX_TO_MX(nX, pView);
			vOrigin[VI(VEC_Y, pView)] = WY_TO_MY(nY, pView);
			vOrigin[VI(VEC_Z, pView)] = pOther->m_vMapCenter[V2I(VU(pView), pOther)];

			// Make an anchor.
			CAnchor* pAnchor = (CAnchor*)NewBaseObject(OBJECT_CLASS_ANCHOR);
			VectorCopy(vOrigin, pAnchor->GetOrigin());

#ifdef _WIN32
			CAnchorDialog dlgTemp;
			dlgTemp.DoModal();

			pAnchor->SetName(dlgTemp.GetAnchorName());
#endif

			pDocument->AddObjectToMap(pAnchor);
			pDocument->BuildHotSpots();
			pDocument->UpdateWindows(FLAT_VIEWS);
#ifdef _WIN32
			((CAnchorView*)GetView(ANCHOR_VIEW))->LoadAnchorList();
#endif
		} break;

	case EF_ACTION_PAINT:

		pDocument->SetSelectedObjectsTexture(GetTexAPI()->GetCurTextureName());
		pDocument->UpdateWindows();
		break;

	case EF_ACTION_UNLINK:
		{
#ifdef _WIN32
			// Find every LENTITY, and get the attached objects and add them directly to the map.
			int nYesNo = MessageBox(NULL, "Are you sure you want to unlink the selected entities? This action cannot be undone!", "Hold on there stranger", MB_YESNO);
			if(nYesNo == IDNO)
				break;
#endif

			CEntity* pEntity;
			CBaseObject* pObject;
			CLinkedList<CBaseObject>* pEntList;
			CLinkedList<CBaseObject>* pList = pDocument->GetSelectedObjectList();

			pList->First();
			for(;;)
			{
				pObject = pList->GetCurItem();
				if(pObject == NULL)
					break;
				if(pObject->GetObjectClass() != OBJECT_CLASS_LENTITY)
				{
					pList->GetNextItem();
					continue;
				}

				pEntity = (CEntity*)pObject;
				pEntList = pEntity->GetObjectList();
				pEntList->First();

				for(;;)
				{
					pObject = pEntList->GetItem(LL_HEAD);
					if(pObject == NULL)
						break;
					pEntity->RemoveObject((CBaseGroupableObject*)pObject);
					pDocument->AddObjectToMap(pObject);
				}

				pList->DeleteItem(pEntity);
			}
		}

		pDocument->BuildHotSpots();
		pDocument->UpdateWindows(FLAT_VIEWS);
		break;

	case EF_ACTION_LINK:
	{
		// Only linkable objects.
		if(pDocument->GetSelectedObjectCount(OBJECT_CLASS_ALL) != pDocument->GetSelectedInterfaceCount(I_LINK|I_GROUPABLE))
		{
			Error("Unable to perform link because there are un-linkable objects selected.");
			return ARF_HANDLED;
		}

		CEntity* pEntity;
		// Create a new entity.
		if(pParam != NULL)
		{
			pEntity = (CEntity*)pParam;
		}
		else
		{
			CEntity* pEntity = GetGameAPI()->GetCurrentGame()->CreateEntityFromDefinition(GetGameAPI()->GetCurrentGame()->GetDefSolidString());
			if(pEntity == NULL)
			{
				Error("Unable to create '" + GetGameAPI()->GetCurrentGame()->GetDefSolidString() + "' entity.");
				return ARF_HANDLED;
			}
		}

		// Transfer them:
		//
		CBaseObject* pObject;
		CLinkedList<CBaseObject>* pList = pDocument->GetSelectedObjectList();
		pList->First();
		for(;;)
		{
			pObject = pList->RemoveItem(LL_HEAD);
			if(pObject == NULL)
				break;

			pEntity->AddObject((CBaseGroupableObject*)pObject->GetInterface(I_GROUPABLE));
		}

		/* Should be done by entity.
		pEntity->m_nObjectRenderFlags = RF_NODRAW;	// Don't draw it.
		pEntity->m_nInterfaceRenderFlags = RF_OWNED;	// Draw the object list as owned.
		*/

		pDocument->AddObjectToSelection(pEntity);
		pDocument->UpdateObjectCounts();
		pDocument->BuildSelectionBBox();
		pDocument->BuildHotSpots();
		pDocument->UpdateWindows(FLAT_VIEWS);

	} break;

	case EF_ACTION_UNDO:

		pDocument->GetUndoAPI()->Undo(pDocument);
		pDocument->AdjustViews();		
		pDocument->UpdateWindows();
		break;

	case EF_ACTION_REDO:

		pDocument->GetUndoAPI()->Redo(pDocument);
		pDocument->AdjustViews();
		pDocument->UpdateWindows();
		break;

	case EF_ACTION_UNDOSTRING:

		Warning("UNDOSTRING: " + pDocument->GetUndoAPI()->GetUndoString() + "\n");
		break;

	case EF_ACTION_REDOSTRING:
		
		Warning("REDOSTRING: " + pDocument->GetUndoAPI()->GetRedoString() + "\n");
		break;

	case EF_ACTION_ROT90CW:
	case EF_ACTION_ROT90CCW:
	case EF_ACTION_ROT180:
		{
			int nView = *((int*)pParam);
			vec3_t vRot = {0.0F, 0.0F, 0.0F};
			
			vRot[VMI(nView, VEC_Z)] = (nAction == EF_ACTION_ROT90CW) ? (PI/2) 
				: (nAction == EF_ACTION_ROT90CCW) ? -(PI/2) 
				: PI;

			if(nView == VIEW_TOP)
				vRot[VMI(nView, VEC_Z)] = -vRot[VMI(nView, VEC_Z)];

			GDATA GData;
			GData.m_pList = pDocument->GetSelectedObjectList();
			VectorCopy(pDocument->m_vSelCenter, GData.m_vOrigin);
			G_Rotate(vRot, &GData);
			pDocument->BuildSelectionBBox();
			pDocument->BuildHotSpots();
			pDocument->UpdateWindows();
		};

	case EF_ACTION_CENTER:

		pDocument->CenterViewsOnSelection();
		pDocument->UpdateWindows(FLAT_VIEWS);
		break;

	case EF_ACTION_CUT:

		pDocument->Cut();
		pDocument->AddGlobalRenderFlags(RF_NOSELBOX|RF_NOSELBOXSIZE);
		pDocument->BuildHotSpots();
		pDocument->UpdateWindows();
		break;

	case EF_ACTION_COPY:

		pDocument->Copy();
		break;

	case EF_ACTION_PASTE:

		pDocument->Paste();
		pDocument->RemoveGlobalRenderFlags(RF_NOSELBOX|RF_NOSELBOXSIZE);
		pDocument->BuildSelectionBBox();
		pDocument->BuildHotSpots();
		pDocument->AdjustViews();
		pDocument->UpdateWindows();
		break;

	case EF_ACTION_DELETEPATHS:
		{
			CBaseObject* pObj;
			CBaseTargetObject* pT;
			CLinkedList<CBaseObject>* pList = pDocument->GetSelectedObjectList();

			pList->First();
			for(pObj = pList->GetCurItem(); pObj != NULL; pObj = pList->GetNextItem())
			{
				pT = (CBaseTargetObject*)pObj->GetInterface(I_TARGET);
				if(pT == NULL)
					continue;
				pT->SetTargetName("", pDocument);
			}
		} 

		pDocument->UpdateWindows();
		break;

	case EF_ACTION_MAKEPATHS:
	case EF_ACTION_MAKEPATHSL:
		{
			CString sName;
			CBaseObject* pObj1, *pObj2;
			CBaseTargetObject* pT1, *pT2;
			CLinkedList<CBaseObject>* pList = pDocument->GetSelectedObjectList();

			if(pList->GetCount() != pDocument->GetSelectedInterfaceCount(I_TARGET))
			{
				Error("MakeTargets cannot be performed because some selected objects are not targetable");
				return ARF_HANDLED;
			}

			pList->First();
			pObj1 = pList->GetCurItem();
			pObj2 = pList->GetNextItem();
			
			for(; pObj2 != NULL; pObj1 = pObj2, pObj2 = pList->GetNextItem())
			{
				sName = pDocument->GetTargetAPI()->GenerateTargetName();
				pT1 = (CBaseTargetObject*)pObj1->GetInterface(I_TARGET);
				pT2 = (CBaseTargetObject*)pObj2->GetInterface(I_TARGET);

				pT1->SetTargetName(sName, pDocument);
				pT2->SetTargetableName(sName, pDocument);
				pDocument->GetTargetAPI()->SetTargetBinding(sName, pObj2);
			}

			if(nAction == EF_ACTION_MAKEPATHSL)
			{
				// Link head to tail.
				pObj2 = pList->GetItem(LL_HEAD);
				pObj1 = pList->GetItem(LL_TAIL);

				sName = pDocument->GetTargetAPI()->GenerateTargetName();
				pT1 = (CBaseTargetObject*)pObj1->GetInterface(I_TARGET);
				pT2 = (CBaseTargetObject*)pObj2->GetInterface(I_TARGET);

				pT1->SetTargetName(sName, pDocument);
				pT2->SetTargetableName(sName, pDocument);
				pDocument->GetTargetAPI()->SetTargetBinding(sName, pObj2);
			}
		}

		pDocument->UpdateWindows();

		break;

		// Makes first object target all others.
	case EF_ACTION_MAKETARGETS:
		{
			CString sName;
			CBaseObject* pObj1, *pObj2;
			CBaseTargetObject* pT1, *pT2;
			CLinkedList<CBaseObject>* pList = pDocument->GetSelectedObjectList();

			if(pList->GetCount() != pDocument->GetSelectedInterfaceCount(I_TARGET))
			{
				Error("MakeTargets cannot be performed because some selected objects are not targetable");
				return ARF_HANDLED;
			}

			pList->First();
			pObj1 = pList->GetCurItem();
			pObj2 = pList->GetNextItem();
			
			pT1 = (CBaseTargetObject*)pObj1->GetInterface(I_TARGET);
			sName = pDocument->GetTargetAPI()->GenerateTargetName();
			pT1->SetTargetName(sName, pDocument);

			for(; pObj2 != NULL; pObj2 = pList->GetNextItem())
			{
				pT2 = (CBaseTargetObject*)pObj2->GetInterface(I_TARGET);
				pT2->SetTargetableName(sName, pDocument);
				pDocument->GetTargetAPI()->SetTargetBinding(sName, pObj2);
			}
		}

		pDocument->UpdateWindows();
		break;

	case EF_ACTION_DELETE:
		{
			UNDOITEM* pItem = CUndoAPI::CreateUndoItem(pDocument->GetSelectedObjectList());
			pItem->nFlags = UAF_DELETE;
			pDocument->GetUndoAPI()->AddUndoItem(pItem, pDocument);

			if(pDocument->DeleteSelection())
			{
				pDocument->BuildHotSpots();
				pDocument->UpdateWindows();
			}

			break;
		}
	case EF_ACTION_FLIPVERT:
		{
			if(pDocument->GetSelectedInterfaceCount(I_TRANSFORMABLE) == 0)
			{
				Warning("CPointerModeResponder: EF_ACTION_FLIPVERT invoked without transformable objects.\n");
				break;
			}

			// Setup the flip.
			GDATA gd;

			gd.m_pList = pDocument->GetSelectedObjectList();
			VectorCopy(pDocument->m_vSelCenter, gd.m_vOrigin);
			G_Reflect(VI(VEC_Y, pDocument->GetCurView()), &gd);

			pDocument->UpdateWindows();
			break;
		}

	case EF_ACTION_FLIPHORZ:
		{
			if(pDocument->GetSelectedInterfaceCount(I_TRANSFORMABLE) == 0)
			{
				Warning("CPointerModeResponder: EF_ACTION_FLIPHORZ invoked without transformable objects.\n");
				break;
			}

			// Setup the flip.
			GDATA gd;

			gd.m_pList = pDocument->GetSelectedObjectList();
			VectorCopy(pDocument->m_vSelCenter, gd.m_vOrigin);
			G_Reflect(VI(VEC_X, pDocument->GetCurView()), &gd);

			pDocument->UpdateWindows();
			break;
		}	

	case EF_ACTION_GROUP:
		{
			if(pDocument->GetSelectedInterfaceCount(I_GROUPABLE) == 0)
			{
				Warning("CPointerModeResponder: EF_ACTION_GROUP invoked without groupable objects.\n");
				break;
			}

			UNDOITEM* pItem = CUndoAPI::CreateUndoItem(pDocument->GetSelectedObjectList());
			pItem->nFlags = UAF_GROUP;

			CObjectGroup* pGroup = new CObjectGroup();
			CBaseObject* pObject;
			CLinkedList<CBaseObject>* pList = pDocument->GetSelectedObjectList();

			pList->First();
			for(;;)
			{
				pObject = pList->GetCurItem();
				if(pObject == NULL)
					break;

				if(pObject->GetObjectInterfaces()&I_GROUPABLE)
				{
					pList->RemoveItem(LL_CURRENT);
					pGroup->AddObject((CBaseGroupableObject*)pObject->GetInterface(I_GROUPABLE));

				}
				else
					pList->GetNextItem();
			}

			// Add the group.
			pGroup->BuildRange();
			pDocument->AddObjectToSelection(pGroup);
			
			pItem->pData = pGroup;
			pDocument->GetUndoAPI()->AddUndoItem(pItem, pDocument);

			break;
		}

	case EF_ACTION_UNGROUP:
		{
			// Groups?
			if(pDocument->GetSelectedInterfaceCount(I_GROUP) == 0)
			{
				Warning("CPointerModeResponder: EF_ACTION_UNGROUP invoked without groups.\n");
				break;
			}

			CObjectGroup* pGroup;
			CBaseObject* pObject, *pGrpObj;
			CLinkedList<CBaseObject>* pList = pDocument->GetSelectedObjectList();
			CLinkedList<CBaseObject>* pUngroupList = new CLinkedList<CBaseObject>();

			UNDOREF* pRef;
			UNDOITEM* pHead, *pTail;
			UNDOITEM* pItem = new UNDOITEM();
			pItem->nFlags = UAF_UNGROUP|UAF_TAIL;
			pItem->nCount = pDocument->GetSelectedInterfaceCount(I_GROUP)+1; // +1 For tail
			pDocument->GetUndoAPI()->AddUndoItem(pItem, pDocument);
			pTail = pItem;

			pList->First();
			for(;;)
			{
				pObject = pList->GetCurItem();
				if(pObject == NULL)
					break;

				if((pObject->GetObjectInterfaces()&I_GROUP)==0)
				{
					pList->GetNextItem();
					continue;
				}

				// Dismantle this group.
				pGroup = (CObjectGroup*)pObject->GetInterface(I_GROUP);
				pItem = new UNDOITEM();
				pItem->nFlags = UAF_UNGROUP|UAF_GMEM;
				pItem->pData = pGroup;

				for(;;)
				{
					pGroup->GetObjectList()->First();
					pGrpObj = pGroup->GetObjectList()->GetCurItem();
					if(pGrpObj == NULL)
						break;
					pGroup->RemoveObject((CBaseGroupableObject*)pGrpObj->GetInterface(I_GROUPABLE));
					pUngroupList->AddItem(pGrpObj);

					pRef = new UNDOREF();
					pRef->pObj = pGrpObj;
					pItem->RefList.AddItem(pRef);
				}

				pDocument->GetUndoAPI()->AddUndoItem(pItem, pDocument);

				pList->RemoveItem(LL_CURRENT);
			}

			// Transfer the others to the selected list.
			pDocument->UpdateObjectCounts();
			G_AddToSelection(pUngroupList, pDocument);
			
			// Delete the old list.
			delete pUngroupList;

			pHead = new UNDOITEM();
			pHead->nFlags = UAF_UNGROUP|UAF_HEAD;
			pHead->nCount = pTail->nCount;
			pDocument->GetUndoAPI()->AddUndoItem(pHead, pDocument);
			
			break;
		}

	case EF_ACTION_HOLLOW:
		{
			// Hollow all selected objects.
			if(pDocument->GetSelectedInterfaceCount(I_HOLLOW)==0)
			{
				Warning("CPointerModeResponder::OnAction(): EF_ACTION_HOLLOW invoked without hollowable objects!!\n");
				break;
			}

			CBaseObject* pObject;
			CLinkedList<CBaseObject>* pList = pDocument->GetSelectedObjectList();
			CLinkedList<CBaseObject>* pOutList = new CLinkedList<CBaseObject>();

			UNDOITEM* pTail;
			UNDOITEM*pItem = new UNDOITEM();
			pItem->nFlags = UAF_HOLLOW|UAF_TAIL;
			pItem->nCount = pList->GetCount()+1;
			pTail = pItem;
			pDocument->GetUndoAPI()->AddUndoItem(pTail, pDocument);

			pList->First();
			for(;;)
			{
				pObject = pList->GetCurItem();
				if(pObject == NULL)
					break;

				if((pObject->GetObjectInterfaces()&I_HOLLOW)==0)
				{
					pList->GetNextItem();
					continue;
				}

				// Remove this object.
				pList->RemoveItem(LL_CURRENT);

				// New item for it.
				pItem = new UNDOITEM();
				pItem->nFlags = UAF_HOLLOW|UAF_GMEM;
				pItem->pData = pObject;

				// What type of object is it?
				switch(pObject->GetObjectClass())
				{
				case OBJECT_CLASS_BRUSH:
					{
						CQBrush* pBrush = (CQBrush*)pObject;

						// Hollow the brush.
						CObjectGroup* pGroup = pBrush->HollowBrush(pDocument->GetHollowDepth());
						if(pGroup == NULL)
						{
							Warning("HollowBrush() returned NULL!!\n");
							delete pItem;
							pItem = NULL;
						}
						else
						{
							pItem->pData2 = pGroup;
							pOutList->AddItem(pGroup);
							//delete pBrush; // Stored on undo stack.
						}

						break;
					}
				default:

					Warning("Unknown I_HOLLOW object class: \n", pObject->GetObjectClass());
					delete pItem;
					pItem = NULL;
					break;
				}
				
				if(pItem != NULL)
					pDocument->GetUndoAPI()->AddUndoItem(pItem, pDocument);
			}

			// Copy the list over.
			G_TransferList(pOutList, pList);
			delete pOutList;

			pItem = new UNDOITEM();
			pItem->nFlags = UAF_HOLLOW|UAF_HEAD;
			pItem->nCount = pTail->nCount;
			pDocument->GetUndoAPI()->AddUndoItem(pItem, pDocument);

			pDocument->UpdateWindows();

			break;
		}

	case EF_ACTION_CARVE:
		{
			// Get the selected object list.
			CLinkedList<CBaseObject>* pList = new CLinkedList<CBaseObject>();
			CLinkedList<CBaseObject>* pOutList = new CLinkedList<CBaseObject>();
			CLinkedList<CBaseObject>* pSelectedList = pDocument->GetSelectedObjectList();
			CObjectGroup* pOutGroup = NULL;
			CBaseObject* pObject;
			CBaseObject* pCarve;

			UNDOITEM* pItem;
			UNDOITEM*pTail = new UNDOITEM();
			pTail->nFlags = UAF_CARVE|UAF_TAIL;
			pTail->nCount = 1; // For tail.
			pDocument->GetUndoAPI()->AddUndoItem(pTail, pDocument);

			// Transfer the list over.
			G_TransferList(pDocument->GetObjectList(), pList);

			pSelectedList->First();
			for(pCarve = pSelectedList->GetCurItem(); pCarve != NULL; pCarve = pSelectedList->GetNextItem())
			{
				// Transformable and carvable?
				if((pCarve->GetObjectInterfaces()&I_CARVE)==0)
					continue;

				pList->First();
				for(;;)
				{
					pObject = pList->RemoveItem(LL_HEAD);
					if(pObject == NULL)
						break;

					// Is this object carvable?
					if((pObject->GetObjectInterfaces()&I_CARVEABLE)==0)
					{
						pDocument->GetObjectList()->AddItem(pObject);	// Moves right back into the map.
						continue;
					}

					// Get the carve interface.
					CBaseCarveableObject* pC = (CBaseCarveableObject*)pObject->GetInterface(I_CARVEABLE);
					if(pC == NULL)
					{
						Warning("Object interface I_CARVEABLE is null!!\n");
						pDocument->GetObjectList()->AddItem(pObject);
						continue;
					}
					else 
					if((pC->GetCarveClasses()&pCarve->GetObjectClass())==0)
					{
						pOutList->AddItem(pObject);
						continue;
					}

					if(pC->ObjectsIntersect(pCarve))
					{
						CObjectGroup* pGroup = pC->CSGSubtract(pCarve);
						if(pGroup == NULL)
						{
							pDocument->AddObjectToMap(pObject);
							continue;
						}

						//delete pObject;
						
						// NOTE: Doing undo at this point will cause the undo operation to do the
						// following sorts of iterations:
						//
						// Normal steps:
						// Carve object A producing B
						// Carve object B producing C
						// etc.
						// When undoing this we will get a stack that does this:
						// 
						// replace C with B.
						// replace B with A.
						//
						// As you can see, the only objects that really need to be
						// stored are A and C. However, it is more difficult to figure this
						// out programmatically, so all intermediate carve stages are saved.
						// You could let the user step backwards through the carve interactions
						// for no good reason. :þ
						//
						// This does take more memory for carving operations since the 
						// intravenous carve stages are stored, and not just the end product.
						// However, this shouldn't make a lot of difference on smaller carve sets
						// since only the "carved" objects are stacked. In case someone's 
						// interested they could implement a slower, more wasteful method by
						// simply storing the whole map instead of just the touched objects.
						// This would be easier then doing it this way, albiet not much.
						//
						// I'm joking about the more wasteful implementation.
						//
						// If the undo setup here is not sufficient because of severly wasted
						// memory, something of this extreme would have to be done:
						//
						// Each time an object is carved, try to find it in the undo stack,
						// and replace the resulting operand.
						//
						// I'm just too lazy to do that right now.
						pItem = new UNDOITEM();
						pItem->nFlags = UAF_CARVE|UAF_GMEM;
						pItem->pData = pObject;
						pItem->pData2 = pGroup;
						pTail->nCount++;
						pDocument->GetUndoAPI()->AddUndoItem(pItem, pDocument);

						pOutList->AddItem(pGroup);
					}
					else
						pOutList->AddItem(pObject);
				}
				
				G_TransferList(pOutList, pList);
			}

			G_TransferList(pList, pDocument->GetObjectList());

			delete pOutList;
			delete pList;

			// Add head item.
			pItem = new UNDOITEM();
			pItem->nFlags = UAF_HEAD|UAF_CARVE;
			pItem->nCount = pTail->nCount;
			pDocument->GetUndoAPI()->AddUndoItem(pItem, pDocument);

			// case EF_ACTION_CARVE
			pDocument->UpdateWindows();
			break;
		}

	default:
		return ARF_NOTHANDLED;		// We couldn't take it.
	}

	return ARF_HANDLED;
}