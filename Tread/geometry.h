// GEOMETRY.CPP
// Copyright © 1999 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#ifndef GEOMETRY_H
#define GEOMETRY_H

#include "MathLib.h"
#include "MapObject.h"
#include "TreadMap2X.h"
#include "TExport.h"

extern _T_EXPORT int gnRecurseCount;

// $sb "geometry.h block1"

#define GF_NONE			0x00000000
#define GF_ORIGIN		0x00000001	// Build the origin.
#define GF_RANGE		0x00000002	// Build the range.

struct GDATA
{
	int m_nFlags;
	vec3_t m_vMins, m_vMaxs, m_vOrigin;
	CLinkedList<CBaseObject>* m_pList;
};

struct UNDOREF;
class CTread3DDoc;

void G_Undo(CLinkedList<UNDOREF>* pList, CTread3DDoc* pDocument, int nFlags);
void G_Redo(CLinkedList<UNDOREF>* pList, CTread3DDoc* pDocument, int nFlags);
void G_PreDelete(CLinkedList<CBaseObject>* pList, CTread3DDoc* pDocument);
CBaseObject* G_FindObject(CLinkedList<CBaseObject>* pList, int nClassType);
void G_SplitObjectList(CPlane* pPlane, CLinkedList<CBaseObject>* pSrcList, CLinkedList<CBaseObject>* pFrontList, CLinkedList<CBaseObject>* pDstList);
void G_InitFromSelection(CLinkedList<CBaseObject>* pList, CTread3DDoc* pDocument, CMapView* pView, vec3_t vMins, vec3_t vMaxs, vec3_t vOrigin);
void G_Translate(vec3_t vTrans, GDATA* pGD);
void G_Scale(vec3_t vScale, GDATA* pGD);
void G_SetOrigin(vec3_t vOrigin, GDATA* pGD);
void G_Reflect(int nAxis, GDATA* pGD);
void G_Rotate(vec3_t vRot, GDATA* pGD);
void G_Build(GDATA* pGD);
float G_SnapFloat(vec_t fPoint, int nGridSize);
float G_SnapFloatDelta(vec_t fPoint, vec_t fLast, int nGridSize);
void G_SnapVector(vec3_t vPoint, vec3_t vOut, int nGridSize, vec_t* pTrans=NULL);
void G_SnapVectorDelta(vec3_t vPoint, vec3_t vLast, vec3_t vOut, int nGridSize, vec_t* pTrans=NULL);
void G_Snap(GDATA* pGD, int nGridSize, vec_t* pTrans=NULL);
void G_SnapDelta(GDATA* pGD, vec3_t vLast, int nGridSize, vec_t* pTrans=NULL);
void G_SetSize(vec3_t vSize, GDATA* pGD);
void G_AddToMap(CLinkedList<CBaseObject>* pList, CTread3DDoc* pDocument);
void G_AddToSelection(CLinkedList<CBaseObject>* pList, CTread3DDoc* pDocument);
void G_CopyList(CLinkedList<CBaseObject>* pSrc, CLinkedList<CBaseObject>* pDst);
void G_TextureList(CLinkedList<CBaseObject>* pList, CString sTexture);
void G_TransferList(CLinkedList<CBaseObject>* pSrc, CLinkedList<CBaseObject>* pDst);
int G_GetInterfaceCount(CLinkedList<CBaseObject>* pList, int nInterface=I_ALL);
int G_GetClassCount(CLinkedList<CBaseObject>* pList, int nClass=OBJECT_CLASS_ALL);
bool G_WriteObjectList(int nVersion, CFile& hFile, CLinkedList<CBaseObject>* pList);
bool G_ReadObjectList(int nNumObjects, int nVersion, CFile& hFile, CLinkedList<CBaseObject>* pList, CLinkedList<CBaseObject>* pUIList);
void G_InitAfterLoad(CLinkedList<CBaseObject>* pList, CTread3DDoc* pDocument);

// $eb

#endif