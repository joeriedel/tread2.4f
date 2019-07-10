//////////////////////////////////////////////////////////////////////
// HOOKSYS.H														//
// Copyright © 2000 Joe Riedel, Nick Randal.						//
// Author: Joe Riedel.												//
//////////////////////////////////////////////////////////////////////
#ifndef HOOKSYS_H
#define HOOKSYS_H

#include "LinkedList.h"
#include "MathLib.h"

// Defines all exported function types, and automatically exports
// the hook procedure that links the routines to the Tread System.

class CMapView;
class CMapManager;
class CMode;
class CTread3DDoc;
class CBaseObject;
class CVisgroupAPI;
class CTreadMapAPI;
class CGameAPI;
class CTexAPI;
class CColorTable;
class CTreadPluginAPI;
struct UNDOREF;
struct GDATA;

typedef void (*TF_DEBUG) (CString, ...);
typedef void (*TF_WARNING) (CString, bool);
typedef void (*TF_ERROR) (CString, bool);
typedef void (*TF_FATAL) (CString, bool);

typedef CString (*TF_GETREGSTRING) (UINT);
typedef int (*TF_GETREGINT) (UINT);
typedef bool (*TF_GETREGBOOL) (bool);
typedef void (*TF_SETREFSTRING) (UINT, CString);
typedef void (*TF_SETREGINT) (UINT, int);
typedef void (*TF_SETREGBOOL) (UINT, bool);

typedef void (*TF_TREADPOPUPMENU) (UINT, int, int, CWnd*, bool);
typedef void (*TF_TREADSETCURSOR) (CMapView*, int);

typedef CMapManager* (*TF_GETMAPMANAGER) (void);
typedef CLinkedList<CBaseObject>* (*TF_GETGLOBALCLIPBOARD)(void);

typedef void (*TF_CONSOLE) (CString, ...);
typedef void (*TF_SETCONSOLE) (CString, int, COLORREF);
typedef void (*TF_RESETCONSOLE) (void);
typedef void (*TF_CLEARCONSOLE) (void);

#ifdef _WIN32
typedef CView* (*TF_GETVIEW) (UINT nCode);
typedef void (*TF_ENABLEVIEW) (UINT nCode, bool bEnable);
typedef void (*TF_ENABLEALLVIEWS) (bool bEnable = true);
#endif

typedef CVisgroupAPI* (*TF_GETVISGROUPAPI) (void);
typedef CTreadMapAPI* (*TF_GETTREADMAPAPI) (void);
typedef CGameAPI* (*TF_GETGAMEAPI) (void);
typedef CTexAPI* (*TF_GETTEXAPI) (void);
typedef CMode* (*TF_GETMODEMANAGER) (void);
typedef CTreadPluginAPI* (*TF_GETPLUGINAPI) ();
typedef CBaseObject* (*TF_NEWBASEOBJECT) (int);

typedef void (*TF_PRGSETPOS)(int);
typedef void (*TF_PRGSETRANGE)(long, long);
typedef void (*TF_PRGSTEPIT)();
typedef void (*TF_PRGSETSTEP)(int);

typedef CColorTable* (*TF_GETCOLORTABLE) ();

typedef void (*TF_G_UNDO)(CLinkedList<UNDOREF>*, CTread3DDoc*, int);
typedef void (*TF_G_REDO)(CLinkedList<UNDOREF>*, CTread3DDoc*, int);
typedef void (*TF_G_PREDELETE)(CLinkedList<CBaseObject>*, CTread3DDoc*);
typedef CBaseObject* (*TF_G_FINDOBJECT)(CLinkedList<CBaseObject>*, int);
typedef void (*TF_G_SPLITOBJECTLIST)(CPlane*, CLinkedList<CBaseObject>*, CLinkedList<CBaseObject>*, CLinkedList<CBaseObject>*);
typedef void (*TF_G_TRANSLATE)(vec3_t, GDATA*);
typedef void (*TF_G_SCALE)(vec3_t, GDATA*);
typedef void (*TF_G_SETORIGIN)(vec3_t, GDATA*);
typedef void (*TF_G_REFLECT)(int, GDATA*);
typedef void (*TF_G_ROTATE)(vec3_t, GDATA*);
typedef void (*TF_G_BUILD)(GDATA*);
typedef float (*TF_G_SNAPFLOAT)(vec_t, int);
typedef float (*TF_G_SNAPFLOATDELTA)(vec_t, vec_t, int);
typedef void (*TF_G_SNAPVECTOR)(vec3_t, vec3_t, int, vec_t*);
typedef void (*TF_G_SNAPVECTORDELTA)(vec3_t vPoint, vec3_t vLast, vec3_t vOut, int nGridSize, vec_t* pTrans);
typedef void (*TF_G_SNAP)(GDATA* pGD, int nGridSize, vec_t* pTrans);
typedef void (*TF_G_SNAPDELTA)(GDATA* pGD, vec3_t vLast, int nGridSize, vec_t* pTrans);
typedef void (*TF_G_SETSIZE)(vec3_t vSize, GDATA* pGD);
typedef void (*TF_G_ADDTOMAP)(CLinkedList<CBaseObject>* pList, CTread3DDoc* pDocument);
typedef void (*TF_G_ADDTOSELECTION)(CLinkedList<CBaseObject>* pList, CTread3DDoc* pDocument);
typedef void (*TF_G_COPYLIST)(CLinkedList<CBaseObject>* pSrc, CLinkedList<CBaseObject>* pDst);
typedef void (*TF_G_TEXTURELIST)(CLinkedList<CBaseObject>* pList, CString sTexture);
typedef void (*TF_G_TRANSFERLIST)(CLinkedList<CBaseObject>* pSrc, CLinkedList<CBaseObject>* pDst);
typedef int (*TF_G_GETINTERFACECOUNT)(CLinkedList<CBaseObject>* pList, int nInterface);
typedef int (*TF_G_GETCLASSCOUNT)(CLinkedList<CBaseObject>* pList, int nClass);
typedef bool (*TF_G_WRITEOBJECTLIST)(int nVersion, CFile& hFile, CLinkedList<CBaseObject>* pList);
typedef bool (*TF_G_READOBJECTLIST)(int nNumObjects, int nVersion, CFile& hFile, CLinkedList<CBaseObject>* pList, CLinkedList<CBaseObject>* pUIList);
typedef void (*TF_G_INITAFTERLOAD)(CLinkedList<CBaseObject>* pList, CTread3DDoc* pDocument);
typedef void (*TF_G_INITFROMSELECTION)(CLinkedList<CBaseObject>* pList, CTread3DDoc* pDocument, CMapView* pView, vec3_t vMins, vec3_t vMaxs, vec3_t vOrigin);

typedef float GVIEWOR[9][2];
typedef int GVIEWIN[5][3];
typedef int G2DINDEX[5][3];

struct T3DEXPORTTABLE
{
	TF_DEBUG Debug;
	TF_WARNING Warning;
	TF_ERROR Error;
	TF_FATAL Fatal;
	TF_CONSOLE Console;
	TF_SETCONSOLE SetConsole;
	TF_RESETCONSOLE ResetConsole;
	TF_CLEARCONSOLE ClearConsole;

	TF_GETMAPMANAGER GetMapManager;
	TF_GETMODEMANAGER GetModeManager;

	TF_GETGLOBALCLIPBOARD GetGlobalClipboard;

#ifdef _WIN32
	TF_GETVIEW GetView;
	TF_ENABLEVIEW EnableView;
	TF_ENABLEALLVIEWS EnableAllViews;
#endif

	TF_GETVISGROUPAPI GetVisgroupAPI;
	TF_GETTREADMAPAPI GetTreadMapAPI;
	TF_GETGAMEAPI GetGameAPI;
	TF_GETTEXAPI GetTexAPI;
	TF_GETCOLORTABLE GetColorTable;
	TF_GETPLUGINAPI GetPluginAPI;

	TF_NEWBASEOBJECT NewBaseObject;

	TF_PRGSETPOS PrgSetPos;
	TF_PRGSETRANGE PrgSetRange;
	TF_PRGSTEPIT PrgStepIt;
	TF_PRGSETSTEP PrgSetStep;

	TF_G_UNDO	G_Undo;
	TF_G_REDO	G_Redo;
	TF_G_PREDELETE	G_PreDelete;
	TF_G_FINDOBJECT	G_FindObject;
	TF_G_SPLITOBJECTLIST	G_SplitObjectList;
	TF_G_TRANSLATE G_Translate;
	TF_G_SCALE G_Scale;
	TF_G_SETORIGIN G_SetOrigin;
	TF_G_REFLECT G_Reflect;
	TF_G_ROTATE G_Rotate;
	TF_G_BUILD G_Build;
	TF_G_SNAPFLOAT G_SnapFloat;
	TF_G_SNAPFLOATDELTA G_SnapFloatDelta;
	TF_G_SNAPVECTOR G_SnapVector;
	TF_G_SNAPVECTORDELTA G_SnapVectorDelta;
	TF_G_SNAP G_Snap;
	TF_G_SNAPDELTA G_SnapDelta;
	TF_G_SETSIZE G_SetSize;
	TF_G_ADDTOMAP G_AddToMap;
	TF_G_ADDTOSELECTION G_AddToSelection;
	TF_G_COPYLIST G_CopyList;
	TF_G_TEXTURELIST G_TextureList;
	TF_G_TRANSFERLIST G_TransferList;
	TF_G_GETINTERFACECOUNT G_GetInterfaceCount;
	TF_G_GETCLASSCOUNT G_GetClassCount;
	TF_G_WRITEOBJECTLIST G_WriteObjectList;
	TF_G_READOBJECTLIST G_ReadObjectList;
	TF_G_INITAFTERLOAD G_InitAfterLoad;
	TF_G_INITFROMSELECTION G_InitFromSelection;

	GVIEWOR* gViewOr;
	GVIEWIN* gViewIn;
	G2DINDEX* g2DIndex;
};

#endif