// GEOMETRY.CPP
// Copyright © 1999 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#include "stdafx.h"
#include "MapObject.h"
#include "Tread3DDoc.h"
#include "Geometry.h"
#include "Brush.h"
#include "Entity.h"
#include "Face.h"
#include "Winding.h"
#include "Camera.h"
#include "UndoAPI.h"

_T_EXPORT int gnRecurseCount=0;

void G_InitFromSelection(CLinkedList<CBaseObject>* pList, CTread3DDoc* pDocument, CMapView* pView, vec3_t vMins, vec3_t vMaxs, vec3_t vOrigin)
{
	CBaseObject* pObj;

	for(pObj = pList->ResetPos(); pObj != NULL; pObj = pList->GetNextItem())
		pObj->InitFromSelection(pDocument, pView, vMins, vMaxs, vOrigin);
}

void G_Redo(CLinkedList<UNDOREF>* pList, CTread3DDoc* pDocument, int nFlags)
{
	UNDOREF* pRef;
	pList->First();
	for(pRef = pList->GetCurItem(); pRef != NULL; pRef = pList->GetNextItem())
		if(pRef->pObj)
			pRef->pObj->Redo(nFlags, pDocument);
}

void G_Undo(CLinkedList<UNDOREF>* pList, CTread3DDoc* pDocument, int nFlags)
{
	UNDOREF* pRef;
	pList->First();
	for(pRef = pList->GetCurItem(); pRef != NULL; pRef = pList->GetNextItem())
		if(pRef->pObj)
			pRef->pObj->Undo(nFlags, pDocument);
}

void G_PreDelete(CLinkedList<CBaseObject>* pList, CTread3DDoc* pDocument)
{
	CBaseObject* pObj;
	pList->First();
	for(pObj = pList->GetCurItem(); pObj != NULL; pObj = pList->GetNextItem())
		pObj->PreDelete(pDocument);
}

CBaseObject* G_FindObject(CLinkedList<CBaseObject>* pList, int nClassType)
{
	CBaseObject* pObj;

	pList->First();
	for(pObj = pList->GetCurItem(); pObj != NULL; pObj = pList->GetNextItem())
		if(pObj->GetObjectClass()&nClassType)
			return pObj;

	return NULL;
}

void G_SplitObjectList(CPlane* pPlane, CLinkedList<CBaseObject>* pSrcList, CLinkedList<CBaseObject>* pFrontList, CLinkedList<CBaseObject>* pBackList)
{
	CBaseObject* pObj;
	CQBrush* pBrush, *pFront, *pBack;
	int nSide;

	pSrcList->First();
	for(pObj = pSrcList->GetCurItem(); pObj != NULL; pObj = pSrcList->GetNextItem())
	{
		if(pObj == NULL)
			break;

		if(pObj->GetObjectClass() != OBJECT_CLASS_BRUSH)
			continue;

		pBrush = (CQBrush*)pObj;

		nSide = pBrush->ClassifyBrush(pPlane);
		switch(nSide)
		{
		case PSIDE_BOTH:

			pBrush->SplitBrushByPlane(pPlane, &pFront, &pBack);
			if(pFront != NULL)
				pFrontList->AddItem(pFront);
			if(pBack != NULL)
				pBackList->AddItem(pBack);
			break;

		case PSIDE_FRONT:

			pFrontList->AddItem(pBrush->Clone());
			break;

		case PSIDE_BACK:

			pBackList->AddItem(pBrush->Clone());
			break;
		}
	}
}

void G_InitAfterLoad(CLinkedList<CBaseObject>* pList, CTread3DDoc* pDocument)
{
	CBaseObject* pObj;

	pList->First();
	for(pObj = pList->GetCurItem(); pObj != NULL; pObj = pList->GetNextItem())
		pObj->InitAfterLoad(pDocument);
}

void G_Reflect(int nAxis, GDATA* pGD)
{
	CBaseObject* pObject;

	pGD->m_pList->First();
	for(pObject = pGD->m_pList->GetCurItem(); pObject != NULL; pObject = pGD->m_pList->GetNextItem())
	{
		if(pObject->GetObjectInterfaces()&I_TRANSFORMABLE)
		{
			CBaseTransformableObject* pT = (CBaseTransformableObject*)pObject->GetInterface(I_TRANSFORMABLE);
			pT->Reflect(nAxis, pGD->m_vOrigin);
		}
	}
}

bool G_ReadObjectList(int nNumObjects, int nVersion, CFile& hFile, CLinkedList<CBaseObject>* pList, CLinkedList<CBaseObject>* pUIList)
{
	int i;
	int nClassType;
	CBaseObject* pObject;

	gnRecurseCount++;

	for(i = 0; i < nNumObjects; i++)
	{
		if(gnRecurseCount==1)
			PrgStepIt();

		nClassType = CBaseStorableObject::ReadObjectClass(hFile);
		pObject = NewBaseObject(nClassType);
		if(pObject == NULL)
		{
			CString s;
			s.Format("7X0: Unknown class type 0x%x\n", nClassType);
			Debug(s);
			gnRecurseCount--;
			return false;
		}

		if(!pObject->ReadObject(hFile, nVersion))
		{
			gnRecurseCount--;
			return false;
		}

		if(pUIList != NULL)
		{
			if(pObject->GetObjectInterfaces()&I_UI)
				pUIList->AddItem(pObject);
			else
				pList->AddItem(pObject);
		}
		else
			pList->AddItem(pObject);
	}

	gnRecurseCount--;
	return true;
}

bool G_WriteObjectList(int nVersion, CFile& hFile, CLinkedList<CBaseObject>* pList)
{
	CBaseObject* pObject;
	pList->First();

	gnRecurseCount++;

	for(pObject = pList->GetCurItem(); pObject != NULL; pObject = pList->GetNextItem())
	{
		if(gnRecurseCount==1)
			PrgStepIt();

		if(pObject->GetObjectInterfaces()&I_STORABLE)
		{
			if(!pObject->WriteObjectClass(hFile))
				return false;
			if(!pObject->WriteObject(hFile, nVersion))
				return false;
		}
	}

	gnRecurseCount--;

	return true;
}

int G_GetInterfaceCount(CLinkedList<CBaseObject>* pList, int nInterface)
{
	int nCount=0;
	CBaseObject* pObject;

	pList->First();
	for(pObject = pList->GetCurItem(); pObject != NULL; pObject = pList->GetNextItem())
		if(pObject->GetObjectInterfaces()&nInterface)
			nCount++;

	return nCount;
}

int G_GetClassCount(CLinkedList<CBaseObject>* pList, int nClass)
{
	int nCount=0;
	CBaseObject* pObject;

	pList->First();
	for(pObject = pList->GetCurItem(); pObject != NULL; pObject = pList->GetNextItem())
	{
		// Do groups
		if(pObject->GetObjectClass()&OBJECT_CLASS_GROUP)
			nCount += G_GetClassCount(((CObjectGroup*)pObject)->GetObjectList(), nClass);
		else if(pObject->GetObjectClass()&nClass)
			nCount++;
	}

	return nCount;
}

// General purpose geometry.
void G_CopyList(CLinkedList<CBaseObject>* pSrc, CLinkedList<CBaseObject>* pDst)
{
	CBaseObject* pObj;

	pSrc->First();
	for(pObj = pSrc->GetCurItem(); pObj != NULL; pObj = pSrc->GetNextItem())
	{
		// Skip UI objects.
		/*if(pObj->GetObjectInterfaces()&I_UI)
			continue;*/

		pDst->AddItem(pObj->Clone());
	}
}

void G_TransferList(CLinkedList<CBaseObject>* pSrc, CLinkedList<CBaseObject>* pDst)
{
	CBaseObject* pObject;

	pSrc->First();
	for(;;)
	{
		pObject = pSrc->RemoveItem(LL_HEAD);
		if(pObject == NULL)
			break;
		pDst->AddItem(pObject);
	}
}

void G_AddToSelection(CLinkedList<CBaseObject>* pList, CTread3DDoc* pDocument)
{
	CBaseObject* pObject;

	pList->First();
	for(;;)
	{
		pObject = pList->RemoveItem(LL_HEAD);
		if(pObject == NULL)
			break;
		pDocument->AddObjectToSelection(pObject);
	}
}

void G_AddToMap(CLinkedList<CBaseObject>* pList, CTread3DDoc* pDocument)
{
	CBaseObject* pObject;

	pList->First();
	for(;;)
	{
		pObject = pList->RemoveItem(LL_HEAD);
		if(pObject == NULL)
			break;
		pDocument->AddObjectToMap(pObject);
	}
}

void G_SetSize(vec3_t vNewSize, GDATA* pGD)
{
	vec3_t vScale;
	vec3_t vSize;

	VectorSubtract(pGD->m_vMaxs, pGD->m_vMins, vSize);

	// Make sure we don't have a 0 somewhere.
	int i;
	for(i = 0; i < 3; i++)
	{
		if(vSize[i] < 1.0F && vSize[i] > -1.0F)
			vSize[i] = 1.0F;
	}

	VectorDivide(vNewSize, vSize, vScale);
	G_Scale(vScale, pGD);
}
	
#define SWITCH_FACTOR	0.50F
float G_SnapFloat(vec_t fPoint, int nGridSize)
{
	int nPoint = (int)fPoint;
	int nMax, nMin;
	int nMaxDiff, nMinDiff;
	bool s=false;

	if(nPoint < 0)
	{
		nPoint = -nPoint;
		s = true;
	}

	// What's it closes to?
	nMin = (nPoint/nGridSize) * nGridSize;
	nMax = nMin + nGridSize;
	nMinDiff = abs((float)nPoint-nMin);
	nMaxDiff = abs((float)nPoint-nMax);

	if(nMinDiff < nMaxDiff)
		nPoint = nMin;
	else
		nPoint = nMax;

	if(s)
		nPoint = -nPoint;

	return (float)nPoint;
}

float G_SnapFloatDelta(vec_t fPoint, vec_t fLast, int nGridSize)
{
	// Ensure locked point.
	fLast = G_SnapFloat(fLast, nGridSize);

	int nSwitch = (SWITCH_FACTOR * nGridSize);
	int nDiff = fPoint - fLast;

	if(nDiff >= nSwitch)
	{
		fLast += nGridSize;
		if((fPoint - fLast) >= nGridSize)
			fLast += ((int)(fPoint - fLast)) / nGridSize * nGridSize;
	}
	else if(nDiff <= -nSwitch)
	{
		fLast -= nGridSize;
		if((fLast - fPoint) >= nGridSize)
			fLast -= ((int)(fLast - fPoint)) / nGridSize * nGridSize;
	}
	
	return fLast;
}

void G_SnapVector(vec3_t vPoint, vec3_t vOut, int nGridSize, vec_t* pTrans)
{
	vec3_t vTemp;
	vTemp[0] = G_SnapFloat(vPoint[0], nGridSize);
	vTemp[1] = G_SnapFloat(vPoint[1], nGridSize);
	vTemp[2] = G_SnapFloat(vPoint[2], nGridSize);

	if(pTrans != NULL)
		VectorSubtract(vTemp, vPoint, pTrans);
	VectorCopy(vTemp, vOut);
}

void G_SnapVectorDelta(vec3_t vPoint, vec3_t vLast, vec3_t vOut, int nGridSize, vec_t* pTrans)
{
	vec3_t vTemp;
	vTemp[0] = G_SnapFloatDelta(vPoint[0], vLast[0], nGridSize);
	vTemp[1] = G_SnapFloatDelta(vPoint[1], vLast[1], nGridSize);
	vTemp[2] = G_SnapFloatDelta(vPoint[2], vLast[2], nGridSize);

	if(pTrans != NULL)
		VectorSubtract(vTemp, vPoint, pTrans);
	VectorCopy(vTemp, vOut);
}

void G_Snap(GDATA* pGD, int nGridSize, vec_t* pTrans)
{
	// Snaps the mins to the grid.
	vec3_t vMins, vDiff;

	G_SnapVector(pGD->m_vMins, vMins, nGridSize);
	VectorSubtract(vMins, pGD->m_vMins, vDiff);
	G_Translate(vDiff, pGD);

	if(pTrans != NULL)
		VectorCopy(vDiff, pTrans);
}

void G_SnapDelta(GDATA* pGD, vec3_t vLast, int nGridSize, vec_t* pTrans)
{
	// Snaps the mins to the grid.
	vec3_t vMins, vDiff;

	G_SnapVectorDelta(pGD->m_vMins, vLast, vMins, nGridSize);
	VectorSubtract(vMins, pGD->m_vMins, vDiff);
	G_Translate(vDiff, pGD);

	if(pTrans != NULL)
		VectorCopy(vDiff, pTrans);
}

void G_TextureList(CLinkedList<CBaseObject>* pList, CString sTexture)
{
	CBaseObject* pObject;
	CBaseTextureableObject* pTex;

	pList->First();
	for(pObject = pList->GetCurItem(); pObject != NULL; pObject = pList->GetNextItem())
	{
		pTex = (CBaseTextureableObject*)pObject->GetInterface(I_TEXTUREABLE);
		if(pTex != NULL)
			pTex->SetTextureName(sTexture);
	}
}

void G_Translate(vec3_t vTrans, GDATA* pGD)
{
	CBaseObject* pObject;
	CBaseTransformableObject* pTrans;

	pGD->m_pList->First();
	for(pObject = pGD->m_pList->GetCurItem(); pObject != NULL; pObject = pGD->m_pList->GetNextItem())
	{
		pTrans = (CBaseTransformableObject*)pObject->GetInterface(I_TRANSFORMABLE);
		if(pTrans != NULL)
			pTrans->Translate(vTrans);
	}
}

void G_Scale(vec3_t vScale, GDATA* pGD)
{
	CBaseObject* pObject;
	CBaseTransformableObject* pTrans;

	pGD->m_pList->First();
	for(pObject = pGD->m_pList->GetCurItem(); pObject != NULL; pObject = pGD->m_pList->GetNextItem())
	{
		pTrans = (CBaseTransformableObject*)pObject->GetInterface(I_TRANSFORMABLE);
		if(pTrans != NULL)
			pTrans->Scale(vScale, pGD->m_vOrigin);
	}
}

void G_SetOrigin(vec3_t vOrigin, GDATA* pGD)
{
	// Compute difference from origins.
	vec3_t vDiff;
	VectorSubtract(vOrigin, pGD->m_vOrigin, vDiff);
	G_Translate(vDiff, pGD);
}

void G_Rotate(vec3_t vRot, GDATA* pGD)
{
	CBaseObject* pObject;
	CBaseTransformableObject* pTrans;

	pGD->m_pList->First();
	for(pObject = pGD->m_pList->GetCurItem(); pObject != NULL; pObject = pGD->m_pList->GetNextItem())
	{
		pTrans = (CBaseTransformableObject*)pObject->GetInterface(I_TRANSFORMABLE);
		if(pTrans != NULL)
			pTrans->Rotate(pGD->m_vOrigin, vRot);
	}
}

void G_Build(GDATA* pGD)
{
	CBaseObject* pObject;
	CBaseTransformableObject* pTrans;

	// Build range?
	if(pGD->m_nFlags&GF_RANGE)
	{
		InitializeRange(pGD->m_vMins, pGD->m_vMaxs);
		pGD->m_pList->First();
		for(pObject = pGD->m_pList->GetCurItem(); pObject != NULL; pObject = pGD->m_pList->GetNextItem())
		{
			pTrans = (CBaseTransformableObject*)pObject->GetInterface(I_TRANSFORMABLE);
			if(pTrans != NULL)
				pTrans->InsertIntoRange(pGD->m_vMins, pGD->m_vMaxs);
		}
	}

	// Build origin?
	if(pGD->m_nFlags&GF_ORIGIN)
		ComputeRangeOrigin(pGD->m_vMins, pGD->m_vMaxs, pGD->m_vOrigin);
}