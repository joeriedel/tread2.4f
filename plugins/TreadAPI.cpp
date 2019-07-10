//////////////////////////////////////////////////////////////////////
// TREADAPI.CPP														//
// Copyright © 2000 Joe Riedel, Nick Randal.						//
// Author: Joe Riedel.												//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "HookSys.h"
#include "TreadAPI.h"

// This module declares storage for our function pointers.
void (*Debug)(CString sMessage, ...);
void (*Warning) (CString sMessage, bool bPopup);
void (*Error)(CString sMessage, bool bConsole);
void (*Fatal)(CString sMessage, bool bConsole);
void (*TreadSetCursor) (CMapView* pView, int nCursor);
void (*Console)(CString sMessage, ...);
void (*SetConsole)(CString sFont, int nSize, COLORREF nColor);
void (*ResetConsole)();
void (*ClearConsole)();
CView* (*GetView) (UINT nCode);
void (*EnableView) (UINT nCode, bool bEnable);
void (*EnableAllViews) (bool bEnable);
CMode* (*GetModeManager) ();
CString (*GetRegString) (UINT nCode);
int (*GetRegInt) (UINT nCode);
bool (*GetRegBool) (UINT nCode);
void (*SetRegString) (UINT nCode, CString sData);
void (*SetRegInt) (UINT nCode, int nData);
void (*SetRegBool) (UINT nCode, bool bData);
void (*TreadPopupMenu) (UINT nMenuId, int nXPos, int nYPos, CWnd* pWnd, bool bTranslate);
CLinkedList<CBaseObject>* (*GetGlobalClipboard) ();
CTexAPI* (*GetTexAPI)();
CMapManager* (*GetMapManager) ();
CGameAPI* (*GetGameAPI) ();
CTreadMapAPI* (*GetTreadMapAPI) ();
CVisgroupAPI* (*GetVisgroupAPI) ();
CRenderAPI* (*GetRenderAPI) ();
CTreadPluginAPI* (*GetPluginAP) ();
CBaseObject* (*NewBaseObject) (int nClassType);
CColorTable* (*GetColorTable) ();
CTreadPluginAPI* (*GetPluginAPI)();
void (*PrgSetPos)(int nPos);
void (*PrgSetRange)(long nMin, long nMax);
void (*PrgStepIt)();
void (*PrgSetStep)(int nStep);
void (*G_Undo)(CLinkedList<UNDOREF>* pList, CTread3DDoc* pDocument, int nFlags);
void (*G_Redo)(CLinkedList<UNDOREF>* pList, CTread3DDoc* pDocument, int nFlags);
void (*G_PreDelete)(CLinkedList<CBaseObject>* pList, CTread3DDoc* pDocument);
CBaseObject* (*G_FindObject)(CLinkedList<CBaseObject>* pList, int nClassType);
void (*G_SplitObjectList)(CPlane* pPlane, CLinkedList<CBaseObject>* pSrcList, CLinkedList<CBaseObject>* pFrontList, CLinkedList<CBaseObject>* pDstList);
void (*G_InitAsModel)(GDATA* pGD, CTread3DDoc* pDocument);
void (*G_Translate)(vec3_t vTrans, GDATA* pGD);
void (*G_Scale)(vec3_t vScale, GDATA* pGD);
void (*G_SetOrigin)(vec3_t vOrigin, GDATA* pGD);
void (*G_Reflect)(int nAxis, GDATA* pGD);
void (*G_Rotate)(vec3_t vRot, GDATA* pGD);
void (*G_Build)(GDATA* pGD);
float (*G_SnapFloat)(vec_t fPoint, int nGridSize);
float (*G_SnapFloatDelta)(vec_t fPoint, vec_t fLast, int nGridSize);
void (*G_SnapVector)(vec3_t vPoint, vec3_t vOut, int nGridSize, vec_t* pTrans);
void (*G_SnapVectorDelta)(vec3_t vPoint, vec3_t vLast, vec3_t vOut, int nGridSize, vec_t* pTrans);
void (*G_Snap)(GDATA* pGD, int nGridSize, vec_t* pTrans);
void (*G_SnapDelta)(GDATA* pGD, vec3_t vLast, int nGridSize, vec_t* pTrans);
void (*G_SetSize)(vec3_t vSize, GDATA* pGD);
void (*G_AddToMap)(CLinkedList<CBaseObject>* pList, CTread3DDoc* pDocument);
void (*G_AddToSelection)(CLinkedList<CBaseObject>* pList, CTread3DDoc* pDocument);
void (*G_CopyList)(CLinkedList<CBaseObject>* pSrc, CLinkedList<CBaseObject>* pDst);
void (*G_TextureList)(CLinkedList<CBaseObject>* pList, CString sTexture);
void (*G_TransferList)(CLinkedList<CBaseObject>* pSrc, CLinkedList<CBaseObject>* pDst);
int (*G_GetInterfaceCount)(CLinkedList<CBaseObject>* pList, int nInterface);
int (*G_GetClassCount)(CLinkedList<CBaseObject>* pList, int nClass);
bool (*G_WriteObjectList)(int nVersion, CFile& hFile, CLinkedList<CBaseObject>* pList);
bool (*G_ReadObjectList)(int nNumObjects, int nVersion, CFile& hFile, CLinkedList<CBaseObject>* pList, CLinkedList<CBaseObject>* pUIList);
void (*G_InitAfterLoad)(GDATA* pData, CTread3DDoc* pDocument);
void (*G_InitFromSelection)(CLinkedList<CBaseObject>* pList, CTread3DDoc* pDocument, CMapView* pView, vec3_t vMins, vec3_t vMaxs, vec3_t vOrigin);

GVIEWOR* gViewOr=NULL;
GVIEWIN* gViewIn=NULL;
G2DINDEX* g2DIndex=NULL;
