// PLUGINAPI.CPP
// Copyright © 2000 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#include "stdafx.h"
#include "TreadSys.h"
#include "Tread3D.h"
#include "Tread3DDoc.h"
#include "PluginAPI.h"
#include "FUtils.h"
#include "szString.h"
#include "Geometry.h"
#include "GameAPI.h"
#include "Visgroup.h"
#include "TexAPI.h"
#include "ColorTable.h"
#include "RenderAPI.h"
#include "MainFrm.h"
#ifdef _WIN32
#include "ViewControl.h"
#endif

#define T3DAFXSETUP()	AFX_MANAGE_STATE(AfxGetStaticModuleState())

CTreadPluginAPI m_PluginAPI;
CTreadPluginAPI* GetPluginAPI() { return &m_PluginAPI; }

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

typedef CTreadPlugin* (*T_PLUGINHOOK) (void);
typedef void (*T_REGISTERFUNCTABLE) (T3DEXPORTTABLE*);

// Plugin container, holds the DLL handle.

CTreadPluginHolder::CTreadPluginHolder()
{
#ifdef _WIN32
	m_hDllInstance = NULL;
#endif
	m_pPlugin = NULL;
}

CTreadPluginHolder::~CTreadPluginHolder()
{
	if(m_pPlugin != NULL)
	{
		m_pPlugin->ShutdownPlugin();
		delete m_pPlugin;
	}
	
#ifdef _WIN32
	if(m_hDllInstance != NULL)
		FreeLibrary(m_hDllInstance);
	m_hDllInstance = NULL;
#endif

	m_pPlugin = NULL;
	
}

TPHMENUITEM CTreadPluginAPI::AddMenuItem(CString sItem, CTreadPlugin* pPlugin)
{
	// Add one to the list.
	TPMENUITEM* pItem = new TPMENUITEM();
	pItem->sTitle = sItem;
	pItem->sPluginName = pPlugin->GetPluginName();
	pItem->nChecked = 0;
	pItem->bEnabled = true;

	m_MenuList.AddItem(pItem);

	Debug("sysMenu: '%s' added item '%s' to plugin menu\n", pPlugin->GetPluginName(), sItem);

	return m_MenuList.GetCount();
}

void CTreadPluginAPI::RemoveMenuItem(TPHMENUITEM hItem, CTreadPlugin* pPlugin)
{
	if(hItem >= LL_HEAD)
		return;

	Debug("sysMenu: '%s' removed item %d from plugin menu\n", pPlugin->GetPluginName(), hItem);

	m_MenuList.DeleteItem(hItem-1);
}

#ifdef _WIN32
void CTreadPluginAPI::UpdateCommandUI(int nMenuID, CCmdUI* pCmdUI)
{
	TPHMENUITEM hItem = nMenuID-ID_PLUGIN_MIN;

	if(hItem >= LL_HEAD)
		return;

	TPMENUITEM* pItem = m_MenuList.GetItem(hItem);
	if(pItem == NULL)
		return;

	pCmdUI->Enable(pItem->bEnabled);
	pCmdUI->SetCheck(pItem->nChecked);
}
#endif

void CTreadPluginAPI::SetMenuItemState(TPHMENUITEM hItem, int nChecked, bool bEnabled, CTreadPlugin* pPlugin)
{
	if(hItem >= LL_HEAD)
		return;
	TPMENUITEM* pItem = m_MenuList.GetItem(hItem-1);
	if(pItem == NULL)
		return;

	pItem->nChecked = nChecked;
	pItem->bEnabled = bEnabled;

	Debug("sysMenu: '%s' set menu state: %d, %d, %d\n", pPlugin->GetPluginName(), hItem, nChecked, bEnabled);
}

void CTreadPluginAPI::ChangeMenuItem(TPHMENUITEM hItem, CString sNewTitle, CTreadPlugin* pPlugin)
{
	if(hItem >= LL_HEAD)
		return;
	TPMENUITEM* pItem = m_MenuList.GetItem(hItem-1);
	if(pItem == NULL)
		return;

	Debug("sysMenu: '%s' changed item %d to '%s'\n", pPlugin->GetPluginName(), hItem, sNewTitle);

	pItem->sTitle = sNewTitle;
}

CString CTreadPluginAPI::GetMenuItemString(TPHMENUITEM hItem, CTreadPlugin* pPlugin)
{
	if(hItem >= LL_HEAD)
		return "";
	TPMENUITEM* pItem = m_MenuList.GetItem(hItem-1);
	if(pItem == NULL)
		return "";
	return pItem->sTitle;
}

void CTreadPluginAPI::UpdatePluginMenu()
{
	// Call nick here.
	GetMapManager()->GetMainFramePtr()->BuildPluginMenu(true);
}

bool CTreadPluginAPI::LoadPlugin(CString sDllPath)
{
#ifdef _WIN32
	HINSTANCE hInstance;
	
	Debug("pluginAPI: loading: " + sDllPath + "\n");
	
	hInstance = LoadLibrary(sDllPath);
	if(hInstance == NULL)
	{
		Debug("pluginAPI: unable to load dll\n");
		return false;	// Couldn't open.
	}

	T_PLUGINHOOK PluginHook = (T_PLUGINHOOK)GetProcAddress(hInstance, "T3D_CreatePluginInstance");
	if(PluginHook == NULL)
	{
		Debug("pluginAPI: dll has no entry point\n");
		FreeLibrary(hInstance);
		return false;
	}

	// Create the object.
	CTreadPlugin* pPlugin = PluginHook();
	if(pPlugin == NULL)
	{
		Debug("pluginAPI: T3D_CreatePluginInstance failed\n");
		FreeLibrary(hInstance);
		return false;
	}

	if(pPlugin->GetAPIVersion() != TP_API_VERSION)
	{
		Debug("pluginAPI: Inconsistent API version\n");
		delete pPlugin;
		//FreeLibrary(hInstance);
		return false;
	}

	T_REGISTERFUNCTABLE RegisterTable = (T_REGISTERFUNCTABLE)GetProcAddress(hInstance, "T3D_RegisterFunctionTable");
	if(RegisterTable == NULL)
	{
		Debug("pluginAPI: dll has no table registration\n");
		delete pPlugin;
		//FreeLibrary(hInstance);
		return false;
	}

	// Make a table and register it.
	{
		T3DEXPORTTABLE Table;

		Table.Debug = Debug;
		Table.Warning = Warning;
		Table.Error = Error;
		Table.Fatal = Fatal;
		Table.GetGlobalClipboard = GetGlobalClipboard;
		Table.GetMapManager = GetMapManager;
		Table.Console = Console;
		Table.SetConsole = SetConsole;
		Table.ResetConsole = ResetConsole;
		Table.ClearConsole = ClearConsole;
		Table.GetGlobalClipboard = GetGlobalClipboard;
		Table.GetMapManager = GetMapManager;
		Table.GetModeManager = GetModeManager;
		Table.GetView = GetView;
		Table.EnableView = EnableView;
		Table.EnableAllViews = EnableAllViews;
		Table.GetVisgroupAPI = GetVisgroupAPI;
		Table.GetTreadMapAPI = GetTreadMapAPI;
		Table.GetGameAPI = GetGameAPI;
		Table.GetTexAPI = GetTexAPI;
		Table.GetColorTable = GetColorTable;
		Table.GetPluginAPI = GetPluginAPI;
		Table.NewBaseObject = NewBaseObject;
		Table.PrgSetPos = PrgSetPos;
		Table.PrgSetRange = PrgSetRange;
		Table.PrgStepIt = PrgStepIt;
		Table.PrgSetStep = PrgSetStep;
		Table.G_Undo = G_Undo;
		Table.G_Redo = G_Redo;
		Table.G_PreDelete = G_PreDelete;
		Table.G_FindObject = G_FindObject;
		Table.G_SplitObjectList = G_SplitObjectList;
		Table.G_Translate = G_Translate;
		Table.G_Scale = G_Scale;
		Table.G_SetOrigin = G_SetOrigin;
		Table.G_Reflect = G_Reflect;
		Table.G_Rotate = G_Rotate;
		Table.G_Build = G_Build;
		Table.G_SnapFloat = G_SnapFloat;
		Table.G_SnapFloatDelta = G_SnapFloatDelta;
		Table.G_SnapVector = G_SnapVector;
		Table.G_SnapVectorDelta = G_SnapVectorDelta;
		Table.G_Snap = G_Snap;
		Table.G_SnapDelta = G_SnapDelta;
		Table.G_SetSize = G_SetSize;
		Table.G_AddToMap = G_AddToMap;
		Table.G_AddToSelection = G_AddToSelection;
		Table.G_CopyList = G_CopyList;
		Table.G_TextureList = G_TextureList;
		Table.G_TransferList = G_TransferList;
		Table.G_GetInterfaceCount = G_GetInterfaceCount;
		Table.G_GetClassCount = G_GetClassCount;
		Table.G_WriteObjectList = G_WriteObjectList;
		Table.G_ReadObjectList = G_ReadObjectList;
		Table.G_InitAfterLoad = G_InitAfterLoad;
		Table.gViewOr = &gViewOr;
		Table.gViewIn = &gViewIn;
		Table.g2DIndex = &g2DIndex;

		RegisterTable(&Table);
	}

	//FreeLibrary(hInstance);
	// This is pretty horrible, at least I think it is.
	// The system does not call FreeLibrary() on any loaded plugins
	// when it shuts down. This shouldn't kill the system because it should
	// release those mapped dll's when the program closes.
	//
	// I'm doing this because calling free on it, well, crashes the system.
	
	hInstance = NULL;

	// Add it to the list.
	CTreadPluginHolder* pHolder = new CTreadPluginHolder();

	// Add to list.
	m_PluginList.AddItem(pHolder);

	// Try to initialize it.
	CString sName;
	sName = pPlugin->GetPluginName();

	Debug("pluginAPI: initing " + sName + "...\n");
	PrintSplashLine("initing " + sName + "...");

	int nCode = pPlugin->InitializePlugin();
	if(nCode != TPF_OK)
	{
		Debug("pluginAPI: failed with code %d", nCode);
		PrintSplashLine("failed to load!");
		pHolder->m_pPlugin = NULL;
		m_PluginList.DeleteItem(pHolder);
		delete pPlugin;
		//FreeLibrary(hInstance);
		return false;
	}

	pHolder->m_pPlugin = pPlugin;
	pHolder->m_hDllInstance = hInstance;
#endif

	return true;
}
	

bool CTreadPluginAPI::Initialize()
{
	CString sFilename;
	CFileSearch FileSearch;

	m_n2DPreDrawFlags = m_n2DPostDrawFlags = 0;
	m_n3DPreDrawFlags = m_n3DPostDrawFlags = 0;

	Debug("pluginAPI: init...\n");
	Debug("pluginAPI: searching: %s\n", GetRegString(S_TREAD_PATH) + "\\plugins\\*.dll");

	// It's ok if we can't find any.
	// Must do *.* on recursive searches.
	if(!FileSearch.BeginFileSearch(GetRegString(S_TREAD_PATH) + "\\plugins", "*.*", true))
	{
		PrintSplashLine("no plugins loaded");
		Debug("pluginAPI: no plugin directory, loading aborted\n");
		return true;
	}

	for(;;)
	{
		FileSearch.GetNextFile(sFilename);
		if(sFilename == "")
			break;

		// Is it a .dll?
		if(!CompareExt(sFilename, ".dll"))
			continue;

		// Try to load it.
		LoadPlugin(GetRegString(S_TREAD_PATH) + "\\plugins\\" + sFilename);
	}

	Debug("pluginAPI: %d plugin(s) loaded\n", m_PluginList.GetCount());
	CString sFmt;
	sFmt.Format("pluginAPI: %d plugin(s) loaded", m_PluginList.GetCount());
	PrintSplashLine(sFmt);

	return true;
}

bool CTreadPluginAPI::Shutdown()
{
	m_MenuList.DestroyList();
	m_PluginList.DestroyList();
	return true;
}

void CTreadPluginAPI::BindPreDrawClasses(CTreadPlugin* pPlugin, int nClasses, int nType)
{
	// See if we can find it.
	CTreadPluginWrapper* pWrapper = FindPlugin(&m_PreDrawList, pPlugin);
	if(pWrapper == NULL)
		return;
	
	if(nType&PDT_2D)
		m_n2DPreDrawFlags |= nClasses;
	if(nType&PDT_3D)
		m_n3DPreDrawFlags |= nClasses;

	pWrapper->m_nTypeFlags |= nType;
	pWrapper->m_nDrawFlags |= nClasses;

	Debug("...'%s' bound draw class flags: %d\n", pPlugin->GetPluginName(), nClasses);
}

void CTreadPluginAPI::UnbindPreDrawClasses(CTreadPlugin* pPlugin, int nClasses, int nType)
{
	// See if we can find it.
	CTreadPluginWrapper* pWrapper = FindPlugin(&m_PreDrawList, pPlugin);
	if(pWrapper == NULL)
		return;
	
	pWrapper->m_nTypeFlags &= ~nType;
	pWrapper->m_nDrawFlags &= ~nClasses;

	if(nType&PDT_2D)
		m_n2DPreDrawFlags = UpdateDrawFlags(&m_PreDrawList);
	if(nType&PDT_3D)
		m_n3DPreDrawFlags = UpdateDrawFlags(&m_PreDrawList);

	Debug("...'%s' un-bound draw class flags: %d\n", pPlugin->GetPluginName(), nClasses);

}

void CTreadPluginAPI::BindPostDrawClasses(CTreadPlugin* pPlugin, int nClasses, int nType)
{
	// See if we can find it.
	CTreadPluginWrapper* pWrapper = FindPlugin(&m_PostDrawList, pPlugin);
	if(pWrapper == NULL)
		return;
	
	if(nType&PDT_2D)
		m_n2DPostDrawFlags |= nClasses;
	if(nType&PDT_3D)
		m_n3DPostDrawFlags |= nClasses;

	pWrapper->m_nTypeFlags |= nType;
	pWrapper->m_nDrawFlags |= nClasses;

	Debug("...'%s' bound draw class flags: %d\n", pPlugin->GetPluginName(), nClasses);

}

void CTreadPluginAPI::UnbindPostDrawClasses(CTreadPlugin* pPlugin, int nClasses, int nType)
{
	// See if we can find it.
	CTreadPluginWrapper* pWrapper = FindPlugin(&m_PostDrawList, pPlugin);
	if(pWrapper == NULL)
		return;
	
	pWrapper->m_nTypeFlags &= ~nType;
	pWrapper->m_nDrawFlags &= ~nClasses;

	if(nType&PDT_2D)
		m_n2DPostDrawFlags = UpdateDrawFlags(&m_PreDrawList);
	if(nType&PDT_3D)
		m_n3DPostDrawFlags = UpdateDrawFlags(&m_PreDrawList);


	Debug("...'%s' un-bound draw class flags: %d\n", pPlugin->GetPluginName(), nClasses);
}

int CTreadPluginAPI::UpdateDrawFlags(CLinkedList<CTreadPluginWrapper>* pList)
{
	int nFlags=0;
	CTreadPluginWrapper* pWrapper;

	pList->First();
	for(pWrapper = pList->GetCurItem(); pWrapper != NULL; pWrapper = pList->GetNextItem())
		nFlags |= pWrapper->m_nDrawFlags;

	return nFlags;
}

void CTreadPluginAPI::InvokePlugin(TPHMENUITEM nID, CTread3DDoc* pDocument)
{
	TPMENUITEM* pItem;
	CString sMatchName;
	CTreadPluginHolder* pHolder;
	
	int nInvokeCount=0;
	
	m_MenuList.First();
	for(pItem = m_MenuList.GetCurItem(); pItem != NULL; pItem = m_MenuList.GetNextItem())
	{
		if(pItem->nID == nID)
		{
			sMatchName = pItem->sPluginName;
			break;
		}
	}

	if(pItem == NULL)
	{
		Debug("sysInvoke: unable to locate plugin with id: %d\n", nID);
		return;
	}

	Debug("sysInvoke: attempting to invoke: '%s'\n", sMatchName);

	m_PluginList.First();
	for(pHolder = m_PluginList.GetCurItem(); pHolder != NULL; pHolder = m_PluginList.GetNextItem())
	{
		if(pHolder->GetPlugin()->GetPluginName() == sMatchName)
		{
			nInvokeCount++;
#ifdef _WIN32
			pHolder->GetPlugin()->InvokePlugin(pDocument, nID-ID_PLUGIN_MIN+1);
#else
			pHolder->GetPlugin()->InvokePlugin(pDocument, nID+1);
#endif
		}
	}

	Debug("sysInvoke: invoked %d plugin(s)\n", nInvokeCount);
}

bool CTreadPluginAPI::PostEvent(int nEvent, int nFlags, CTread3DDoc* pDocument, void* pParam, void* pParam2, void* pParam3, void* pParam4)
{
	int nMode, nSubmode;

	if(GetModeManager() != NULL)
	{
		nMode = GetModeManager()->GetMode();
		nSubmode = GetModeManager()->GetSubmode();
	}
	else
		nMode = nSubmode = 0;

	int nRecvCount=0;

	// Supress the SELCHANGE debug message: we get tons of them.
	if(nEvent != PEF_SELCHANGE)
	{
		Debug("sysPostEvent: event-code: %d, flags: %d\n", nEvent, nFlags);
		Debug("... processing\n");
	}

	// Loop through the plugins and trigger the event.
	CTreadPluginWrapper* pWrapper;
	m_EventList.First();
	for(pWrapper = m_EventList.GetCurItem(); pWrapper != NULL; pWrapper = m_EventList.GetNextItem())
	{
		if(pWrapper->m_nFlags&nEvent)
		{
			nRecvCount++;
			pWrapper->GetPlugin()->OnEvent(nEvent, nFlags, nMode, nSubmode, pDocument, pParam, pParam2, pParam3, pParam4);
		}
	}

	if(nEvent != PEF_SELCHANGE)
		Debug("... %d reciever(s)\n", nRecvCount);

	return true;
}

CTreadPluginWrapper* CTreadPluginAPI::FindPlugin(CLinkedList<CTreadPluginWrapper>* pList, CTreadPlugin* pPlugin)
{
	CTreadPluginWrapper* pW;
	
	pList->First();
	for(pW = pList->GetCurItem(); pW != NULL; pW = pList->GetNextItem())
	{
		if(pW->GetPlugin() == pPlugin)
			return pW;
	}

	return NULL;
}

void CTreadPluginAPI::BindEvent(CTreadPlugin* pPlugin, int nEventFlags)
{
	// See if we can find it.
	CTreadPluginWrapper* pWrapper = FindPlugin(&m_EventList, pPlugin);

	// Make a wrapper for this plugin?
	if(pWrapper == NULL)
	{
		pWrapper = new CTreadPluginWrapper();
		pWrapper->m_pPlugin = pPlugin;
		m_EventList.AddItem(pWrapper);
	}

	Debug("... '%s' bound event-flags: %d\n", pPlugin->GetPluginName(), nEventFlags);

	pWrapper->m_nFlags |= nEventFlags;
}

void CTreadPluginAPI::UnbindEvent(CTreadPlugin* pPlugin, int nEventFlags)
{
	// Do we have it?
	CTreadPluginWrapper* pWrapper = FindPlugin(&m_EventList, pPlugin);

	Debug("... '%s' unbound event-flags: %d\n", pPlugin->GetPluginName(), nEventFlags);

	if(pWrapper == NULL)
	{
		Debug(".... no event bindings found!\n");
		return;
	}

	pWrapper->m_nFlags &= ~nEventFlags;

	// No more events are bound?
	if(pWrapper->m_nFlags == 0)
		m_EventList.DeleteItem(pWrapper);
}

void CTreadPluginAPI::Get2DRenderPackageList(CString sNames[MAX_RENDER_PACKAGES], CString sDescriptions[MAX_RENDER_PACKAGES], int& nNumPaks)
{
	int i, nNum;
	CString sRPNames[256];
	CString sRPDesc[256];
	CTreadPluginWrapper* pW;

	nNumPaks = 0;

	m_RPAKList.First();
	for(pW = m_RPAKList.GetCurItem(); pW != NULL; pW = m_RPAKList.GetNextItem())
	{
		nNum = 0;
		pW->GetPlugin()->Get2DRenderPackageList(sRPNames, sRPDesc, nNum);

		Debug("... '%s' exported %d 2d render package(s)\n", pW->GetPlugin()->GetPluginName(), nNum);

		if(nNum == 0)
			continue;

		if(nNum + nNumPaks > 256)
		{
			Warning("CTreadPluginAPI::Get2DRenderPackageList(): MAX_RENDER_PACKAGES!!\n");
			break;
		}

		// Copy them in.
		for(i = 0; i < nNum; i++)
		{
			sNames[i+nNumPaks] = sRPNames[i];
			sDescriptions[i+nNumPaks] = sRPDesc[i];
		}

		nNumPaks += nNum;
		if(nNumPaks == MAX_RENDER_PACKAGES)
			break;
	}
}

void CTreadPluginAPI::Get3DRenderPackageList(CString sNames[MAX_RENDER_PACKAGES], CString sDescriptions[MAX_RENDER_PACKAGES], int& nNumPaks)
{
		int i, nNum;
	CString sRPNames[256];
	CString sRPDesc[256];
	CTreadPluginWrapper* pW;

	nNumPaks = 0;

	m_RPAKList.First();
	for(pW = m_RPAKList.GetCurItem(); pW != NULL; pW = m_RPAKList.GetNextItem())
	{
		nNum = 0;
		pW->GetPlugin()->Get3DRenderPackageList(sRPNames, sRPDesc, nNum);

		Debug("... '%s' exported %d 3d render package(s)\n", pW->GetPlugin()->GetPluginName(), nNum);

		if(nNum == 0)
			continue;

		if(nNum + nNumPaks > 256)
		{
			Warning("CTreadPluginAPI::Get3DRenderPackageList(): MAX_RENDER_PACKAGES!!\n");
			break;
		}

		// Copy them in.
		for(i = 0; i < nNum; i++)
		{
			sNames[i+nNumPaks] = sRPNames[i];
			sDescriptions[i+nNumPaks] = sRPDesc[i];
		}

		nNumPaks += nNum;
		if(nNumPaks == MAX_RENDER_PACKAGES)
			break;
	}

}

void CTreadPluginAPI::BindFunction(CTreadPlugin* pPlugin, int nFunction)
{
	CTreadPluginWrapper* pW;

	Debug("... '%s' is binding:\n", pPlugin->GetPluginName());

	if(nFunction&PFF_RPAKCREATOR)
	{
		Debug(".... RPAKCREATOR\n");
		pW = new CTreadPluginWrapper();
		pW->m_pPlugin = pPlugin;
		m_RPAKList.AddItem(pW);
	}

	if(nFunction&PFF_TPAKREADER)
	{
		Debug(".... TPAKREADER\n");
		pW = new CTreadPluginWrapper();
		pW->m_pPlugin = pPlugin;
		m_TPAKList.AddItem(pW);
	}

	if(nFunction&PFF_TDIRREADER)
	{
		Debug(".... TDIRREADER\n");
		pW = new CTreadPluginWrapper();
		pW->m_pPlugin = pPlugin;
		m_TDIRList.AddItem(pW);
	}

	if(nFunction&PFF_PREDRAWOBJECT)
	{
		Debug(".... PREDRAWOBJECT\n");
		pW = new CTreadPluginWrapper();
		pW->m_pPlugin = pPlugin;
		m_PreDrawList.AddItem(pW);
	}
	
	if(nFunction&PFF_POSTDRAWOBJECT)
	{
		Debug(".... POSTDRAWOBJECT\n");
		pW = new CTreadPluginWrapper();
		pW->m_pPlugin = pPlugin;
		m_PostDrawList.AddItem(pW);
	}

	if(nFunction&PFF_OBJECTCREATOR)
	{
		Debug(".... OBJECTCREATOR\n");
		pW = new CTreadPluginWrapper();
		pW->m_pPlugin = pPlugin;
		m_ObjCreatorList.AddItem(pW);
	}

	if(nFunction&PFF_MAPREADER)
	{
		Debug(".... MAPREADER\n");
		pW = new CTreadPluginWrapper();
		pW->m_pPlugin = pPlugin;
		m_MapReaderList.AddItem(pW);
	}

	if(nFunction&PFF_MAPWRITER)
	{
		Debug(".... MAPWRITER\n");
		pW = new CTreadPluginWrapper();
		pW->m_pPlugin = pPlugin;
		m_MapWriterList.AddItem(pW);
	}
}

void CTreadPluginAPI::UnbindFunction(CTreadPlugin* pPlugin, int nFunction)
{
	CTreadPluginWrapper* pW;

	Debug("... '%s' is unbinding:\n", pPlugin->GetPluginName());

	if(nFunction&PFF_RPAKCREATOR)
	{
		pW = FindPlugin(&m_RPAKList, pPlugin);
		if(pW)
		{
			Debug(".... RPAKCREATOR\n");
			m_RPAKList.DeleteItem(pW);
		}
	}

	if(nFunction&PFF_TPAKREADER)
	{
		pW = FindPlugin(&m_TPAKList, pPlugin);
		if(pW)
		{
			Debug(".... TPAKREADER\n");
			m_TPAKList.DeleteItem(pW);
		}
	}

	if(nFunction&PFF_TDIRREADER)
	{
		pW = FindPlugin(&m_TDIRList, pPlugin);
		if(pW)
		{
			Debug(".... TDIRREADER\n");
			m_TDIRList.DeleteItem(pW);
		}
	}

	if(nFunction&PFF_PREDRAWOBJECT)
	{
		pW = FindPlugin(&m_PreDrawList, pPlugin);
		if(pW)
		{
			Debug(".... PREDRAWOBJECT\n");
			m_PreDrawList.DeleteItem(pW);
		}
	}
	
	if(nFunction&PFF_POSTDRAWOBJECT)
	{
		pW = FindPlugin(&m_PostDrawList, pPlugin);
		if(pW)
		{
			Debug(".... POSTDRAWOBJECT\n");
			m_PostDrawList.DeleteItem(pW);
		}
	}

	if(nFunction&PFF_OBJECTCREATOR)
	{
		pW = FindPlugin(&m_ObjCreatorList, pPlugin);
		if(pW)
		{
			Debug(".... OBJECTCREATOR\n");
			m_ObjCreatorList.DeleteItem(pW);
		}
	}

	if(nFunction&PFF_MAPREADER)
	{
		pW = FindPlugin(&m_MapReaderList, pPlugin);
		if(pW)
		{
			Debug(".... MAPREADER\n");
			m_MapReaderList.DeleteItem(pW);
		}
	}

	if(nFunction&PFF_MAPWRITER)
	{
		pW = FindPlugin(&m_MapWriterList, pPlugin);
		if(pW)
		{
			Debug(".... MAPWRITER\n");
			m_MapWriterList.DeleteItem(pW);
		}
	}
}

CBaseTreadMapFile* CTreadPluginAPI::GetMapReader(CString sTag, int nMagic, CTread3DDoc* pDocument)
{
	CBaseTreadMapFile* pFR=NULL;
	CTreadPluginWrapper* pW;

	m_MapReaderList.First();
	for(pW = m_MapReaderList.GetCurItem(); pW != NULL; pW = m_MapReaderList.GetNextItem())
	{
		pFR = pW->GetPlugin()->GetMapReader(sTag, nMagic, pDocument);
		if(pFR != NULL)
		{
			Debug("... '%s' claimed map read\n", pW->GetPlugin()->GetPluginName());
			return pFR;
		}
	}

	return NULL;
}

CBaseTreadMapFile* CTreadPluginAPI::GetMapWriter(TREAD2XHEADERINFO* pHeader, CTread3DDoc* pDocument)
{
	CBaseTreadMapFile* pFR=NULL;
	CTreadPluginWrapper* pW;

	m_MapWriterList.First();
	for(pW = m_MapWriterList.GetCurItem(); pW != NULL; pW = m_MapWriterList.GetNextItem())
	{
		pFR = pW->GetPlugin()->GetMapWriter(pHeader, pDocument);
		if(pFR != NULL)
		{
			Debug("... '%s' claimed map write\n", pW->GetPlugin()->GetPluginName());
			return pFR;
		}
	}

	return NULL;
}

CPakFile* CTreadPluginAPI::OpenTextureDirectory(CString sType)
{
	// Search the list and see if any open it.
	CPakFile* pPAK;
	CTreadPluginWrapper* pW;
	m_TDIRList.First();
	for(pW = m_TDIRList.GetCurItem(); pW != NULL; pW = m_TDIRList.GetNextItem())
	{
		pPAK = pW->GetPlugin()->OpenTextureDirectory(sType);
		if(pPAK != NULL)
		{
			Debug("... '%s' claimed t-directory\n", pW->GetPlugin()->GetPluginName());
			return pPAK;
		}
	}

	return NULL;
}

CPakFile* CTreadPluginAPI::OpenTexturePackage(CString sFilename)
{
	// Search the list and see if any open it.
	CPakFile* pPAK;
	CTreadPluginWrapper* pW;
	m_TPAKList.First();
	for(pW = m_TPAKList.GetCurItem(); pW != NULL; pW = m_TPAKList.GetNextItem())
	{
		pPAK = pW->GetPlugin()->OpenTexturePackage(sFilename);
		if(pPAK != NULL)
		{
			Debug("... '%s' claimed t-package\n", pW->GetPlugin()->GetPluginName());
			return pPAK;
		}
	}

	return NULL;
}

CBaseObject* CTreadPluginAPI::CreateBaseObject(int nObjectClass)
{
	CBaseObject* pObj;
	CTreadPluginWrapper* pW;

	m_ObjCreatorList.First();
	for(pW = m_ObjCreatorList.GetCurItem(); pW != NULL; pW = m_ObjCreatorList.GetNextItem())
	{
		pObj = pW->GetPlugin()->CreateBaseObject(nObjectClass);
		if(pObj != NULL)
			return pObj;
	}

	return NULL;
}

CBaseRenderPackage* CTreadPluginAPI::CreateRenderPackage(CString sPakName)
{
	// Search the list and see if any open it.
	CBaseRenderPackage* pPAK;
	CTreadPluginWrapper* pW;
	m_RPAKList.First();
	for(pW = m_RPAKList.GetCurItem(); pW != NULL; pW = m_RPAKList.GetNextItem())
	{
		pPAK = pW->GetPlugin()->CreateRenderPackage(sPakName);
		if(pPAK != NULL)
			return pPAK;
	}

	return NULL;
}

// Draws an object.
// Lot's of pre drawers has the ability to slow things way down.
bool CTreadPluginAPI::PreDrawObject(CMapView* pView, CTread3DDoc* pDocument, CBaseWindowRenderer* pWindowRenderer, CDC* pDC, void* pContext, CBaseObject* pObject, int nObjectClass, int nObjectInterfaces, int nDrawFlags, int nFlags, int nType, int nPAKUID)
{
	if(m_PreDrawList.IsEmpty())
		return false;

	CTreadPluginWrapper* pW;
	m_PreDrawList.First();
	for(pW = m_PreDrawList.GetCurItem(); pW != NULL; pW = m_PreDrawList.GetNextItem())
	{
		if((pW->m_nDrawFlags&nObjectClass) == 0 || (pW->m_nTypeFlags&nType) == 0)
			continue;
		if(pW->m_pPlugin->DrawObject(pView, pDocument, pWindowRenderer, pDC, pContext, pObject, nObjectClass, nObjectInterfaces, nDrawFlags, nFlags, nPAKUID, PCF_PRE, nType))
			return true;
	}

	// No one drew it.
	return false;
}

bool CTreadPluginAPI::PostDrawObject(CMapView* pView, CTread3DDoc* pDocument, CBaseWindowRenderer* pWindowRenderer, CDC* pDC, void* pContext, CBaseObject* pObject, int nObjectClass, int nObjectInterfaces, int nDrawFlags, int nFlags, int nType, int nPAKUID)
{
	if(m_PostDrawList.IsEmpty())
		return false;

	CTreadPluginWrapper* pW;
	m_PostDrawList.First();
	for(pW = m_PostDrawList.GetCurItem(); pW != NULL; pW = m_PostDrawList.GetNextItem())
	{
		if((pW->m_nDrawFlags&nObjectClass) == 0 || (pW->m_nTypeFlags&nType) == 0)
			continue;
		if(pW->m_pPlugin->DrawObject(pView, pDocument, pWindowRenderer, pDC, pContext, pObject, nObjectClass, nObjectInterfaces, nDrawFlags, nFlags, nPAKUID, PCF_POST, nType))
			return true;
	}

	// No one drew it.
	return false;
}
