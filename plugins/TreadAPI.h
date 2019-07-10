//////////////////////////////////////////////////////////////////////
// TREADAPI.H														//
// Copyright © 2000 Joe Riedel, Nick Randal. All Rights Reserved.	//
// Author: Joe Riedel.												//
//////////////////////////////////////////////////////////////////////
// This code is supplied WITHOUT WARRANTY OF ANY KIND. So if		//
// something goes wrong, it isn't our fault.						//
//////////////////////////////////////////////////////////////////////
//
// This file contains all the Tread definitions you will need to create
// a plugin. Just about all the guts of Tread are exposed here, at least,
// the definitions are.
//
// FORWARD:
//
// It is my hope that the community will be able to take this and make
// some cool stuff. I expect to see some stuff from David Hyde! One cool
// idea of something is a plugin that actually draws the entities
// .md2 model in the 3D view and wireframe, like Qoole does for Quake.
// Hopefully we'll see GenSurf built in sometime too. These aren't 
// demands, they're just ideas. I've tried to make the plugin system
// as versatile and unlimited as possible, but with certain intentional
// hinderences. Like you can't bypass our license verification with
// a plugin. Here's what has been exposed:
//
// 1) Most of the MFC interface classes have been exposed so you
// can make calls to them when necessary.
//
// 2) The majority of the API's (GameAPI, TexAPI, VisgroupAPI,
// TargetAPI, UndoAPI) are all callable. This is where you will
// draw the majority of your functionality, at least when getting
// info from Tread.
//
// 3) The rendering system is fully expandable by plugins. You can
// override the drawing of buit-in objects. You can create new
// objects and draw them in existing renderers. Or you can create
// entirely new renderers.
//
// 4) A plethora of objects used internally by Tread. The majority
// of Tread works on virtual base classes, hence its expandability.
// Most of the system was designed to be a black-box. For the most
// part you should be able to derrive off the base classes to add
// your own stuff.
//
// 5) The map file system is fully expandable. You can create your
// own objects and save them with the .map. Tread will, however, become
// dependant on the plugin to load the object as well. You can make your
// own map reader/writer's to.
//
// SUPPORT:
//
// Officially we do not support ANYTHING created by the user community.
// If you make it you must make it explicitly clear that we are not the
// ones to come crawling to if it doesn't work. 
//
// We also do not provide "official" support of people developing plugins
// for Tread. That aside, you can always ask us questions, so feel free
// to email us about it. I'd love to work with people answering questions,
// etc, etc.
//
// SOURCE-CODE:
//
// You are not in anyway required to release source code to your plugins.
// You retain full rights to code you write. You have no right to the code
// we released for this API. We may request from time to time to release
// your plugin with selling versions of Tread. You are not entitled to any
// monetary compensation if we do. You, however, retain the right to
// decline the request to release it with Tread.
//
// If you do not plan to sell your plugin then releasing the source code 
// to it will greatly benefit the community. It would be my hope that 
// people start a site that contains Tread plugin source and current plugins 
// and what they do.
//
// CONTACT:
//
// jmriedel@planetquake.com -- That's me: Joe Riedel.
// nrandal@planetquake.com -- That's Nick Randal.
// sarnold@planetquake.com -- That Sean Arnold, our webmaster.
//
// DOCUMENTATION:
//
// Unfortunately the only documentation I plan to provide is what you see
// in this documented source code. Anyone who figures stuff out and would
// like to start documenting things is alright with me, as long as you
// make it clear that it's not official. You can always email Nick or I
// regarding clarification on things. I may release some example plugins.
//
// Look through this header for brief comments on how things works.
//
// EXPLANATION OF SUPPLIED FILES.
//
// The TreadAPI header file defines all the exported Tread definitions
// needed by the plugin. This is the only header you need to include
// to use the definitions. The TreadAPI.cpp file defines some extern
// storage. Sneak a peak if you want.
//
// HookSys.cpp exports a function that Tread hooks and uses to setup
// some global function calls and parameters, such as console routines
// and others. You need to simply include HookSys.h and HookSys.cpp in
// your application.
//
// Mathlib.cpp contains various defintions and function for manipulating
// vectors and quaternions. These are provided so you don't have to
// write your own, unless of course you want to.
//
// LinkedList.h is a template linked list that I think is one of the
// best things we ever did this time around. Alot of stability problems
// in the old versions had to do with directly manipulating list pointers.
// This template class allows you to make a linked list class that
// contains any type of object with prev,next pointers. There is a 
// special base class that you should derrive off of called CLLObject.
// There are exceptions to this, however, in which case you should just
// make prev,next pointers.
//
// wogl.h contains stripped down OGL definitions that Tread uses,
// incase you want to extend the OGL render package.
//
// Tokenizer.cpp is a white space file parsing class. It's very usefull
// and is used throughout Tread. I've decided to supply it to you as
// full source so modifications to it would be allowed. However, be
// carefull that if you change it you make a new class because there
// may be cases where Tread passes a CTokenizer object to you.
//
// SzString.cpp is a small null terminated string library. It works
// on CStrings as well thanx to the LPCTSTR operator.
//
// FUtils.cpp is a file utility library. It provides routines to read
// and write strings etc. It also gives you a class called CFileSearch
// wich is most usefull and will do recursive file searching up directory
// chains. The full source for that is provided as well.
//
#ifndef TREADSYS_H
#define TREADSYS_H

//////////////////////////////////////////////////////////////////////
// This header defines some global flags and system function calls	//
//////////////////////////////////////////////////////////////////////

#include "LinkedList.h"
#include "HookSys.h"
#include "MathLib.h"
#include "Tokenizer.h"
#include "wogl.h"

#define _T_EXPORT
#define T3DPLUGINAPI	extern "C" __declspec (dllexport)
#define T3DAFXSETUP()	AFX_MANAGE_STATE(AfxGetStaticModuleState())

class CMode;
class CMapFrame;
class CMapView;
class CConsoleView;
class CTread3DDoc;
class CMainFrame;
class CBaseObject;
class CRenderAPI;
class CTexture;
class CMode;
class CEntity;
class CVisgroupAPI;
class CVisgroup;
class CUndoAPI;
class CTargetAPI;
class CWindowEventManager;
class CBrowserDialog;
class CTexPalette;
class CPakFile;
class CBaseRenderPackage;
class CBaseTreadMapFile;
class CBaseWindowRenderer;

struct RUNSTATE;
struct TREAD2XHEADERINFO;

#define MAX_TRANSPARENTS		256
#define MAX_RENDER_PACKAGES		256

//////////////////////////////////////////////////////////////////////
// PLUGINAPI.H														//
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
// The plugin system allows users to extend Tread, hopefully	//
// indefinently, well except for certain things...				//
//////////////////////////////////////////////////////////////////

#define TPF_ERRROR		0x00000000
#define TPF_OK			0x00000001

#define TP_API_VERSION		3

// Plugin event flags. Plugins can bind these events.
// Not all of these are events, some of them are 
// functionality options that we are making Tread
// aware of. Otherwise they won't be called.
#define PEF_SELCHANGE		0x00000001
#define PEF_MAPCHANGE		0x00000002
#define PEF_GAMECHANGE		0x00000004		// The current game has changed.
#define PEF_ENTITYUPDATE	0x00000008		// Called to reparse entity data.

#define PFF_RPAKCREATOR		0x00000001		// Render package creator.
#define PFF_TPAKREADER		0x00000002		// Texture package reader.
#define PFF_TDIRREADER		0x00000004		// Texture directory reader.
#define PFF_PREDRAWOBJECT	0x00000008		// Only to ovveride built-in drawing.
#define PFF_POSTDRAWOBJECT	0x00000010		// Used to draw new objects.
#define PFF_OBJECTCREATOR	0x00000020		// It creates a CBaseObject.
#define PFF_MAPREADER		0x00000040		// It reads a map file.
#define PFF_MAPWRITER		0x00000080		// It writes a map file.

#define PCF_PRE				0x00000001
#define PCF_POST			0x00000002

#define PDT_2D				0x00000001
#define PDT_3D				0x00000002

typedef unsigned int TPHMENUITEM;

// Defines a plugin object.
class CTreadPlugin
{
public:

	CTreadPlugin() {};
	virtual ~CTreadPlugin() {};

	virtual int GetAPIVersion() = 0;
	virtual int InitializePlugin() = 0;
	virtual int ShutdownPlugin() = 0;
	virtual CString GetPluginName() = 0;

	virtual int InvokePlugin(CTread3DDoc* pDocument, TPHMENUITEM nMenuID) = 0;
	virtual void OnEvent(int nEventFlags, int nFlags, int nMode, int nSubmode, CTread3DDoc* pDocument, void* pParam=NULL, void* pParam2=NULL, void* pParam3=NULL, void* pParam4=NULL) {}
	virtual CPakFile* OpenTextureDirectory(CString sFilename) { return NULL; }
	virtual CPakFile* OpenTexturePackage(CString sFilename) { return NULL; }
	virtual bool ClaimPackageName(CString sPakName) { return false; }
	virtual CBaseRenderPackage* CreateRenderPackage(CString sPakName) { return NULL; }
	virtual CBaseObject* CreateBaseObject(int nObjectClass) { return NULL; }

	virtual CBaseTreadMapFile* GetMapReader(CString sTag, int nMagic, CTread3DDoc* pDocument) { return NULL; }
	virtual CBaseTreadMapFile* GetMapWriter(TREAD2XHEADERINFO* pHeader, CTread3DDoc* pDocument) { return NULL; }

	// Get the rendering lists from this plugin.
	virtual void Get2DRenderPackageList(CString sNames[MAX_RENDER_PACKAGES], CString psDescriptions[MAX_RENDER_PACKAGES], int& nNumPaks) { nNumPaks = 0; }
	virtual void Get3DRenderPackageList(CString sNames[MAX_RENDER_PACKAGES], CString psDescriptions[MAX_RENDER_PACKAGES], int& nNumPaks) { nNumPaks = 0; }

	// Draws an object.
	virtual bool DrawObject(CMapView* pView, CTread3DDoc* pDocument, CBaseWindowRenderer* pWindowRenderer, CDC* pDC, void* pContext, CBaseObject* pObject, int nObjectClass, int nObjectInterfaces, int nDrawFlags, int nFlags, int nPAKUID, int nStage, int nType) { return false; }
};

// Plugin container, holds the DLL handle.
class CTreadPluginAPI;

class _T_EXPORT CTreadPluginHolder : public CLLObject
{
	friend class CTreadPluginAPI;

private:

	CTreadPlugin* m_pPlugin;
#ifdef _WIN32
	HINSTANCE m_hDllInstance;
#endif

public:

	CTreadPluginHolder();
	~CTreadPluginHolder();

	CTreadPlugin* GetPlugin() { return m_pPlugin; }

};

class _T_EXPORT CTreadPluginWrapper : public CLLObject
{
	friend class CTreadPluginAPI;

private:

	int m_nFlags;
	int m_nDrawFlags;
	int m_nTypeFlags;
	CTreadPlugin* m_pPlugin;

public:

	CTreadPluginWrapper()
	{ m_nFlags = m_nDrawFlags = m_nTypeFlags = 0; }

	CTreadPlugin* GetPlugin() { return m_pPlugin; }
};

struct TPMENUITEM
{
	TPMENUITEM* prev, *next;

	TPHMENUITEM nID;
	bool bEnabled;
	int nChecked;
	CString sTitle;
	CString sPluginName;
};

// The API system.
class _T_EXPORT CTreadPluginAPI
{
private:

	CLinkedList<CTreadPluginHolder> m_PluginList;
	CLinkedList<CTreadPluginWrapper> m_PreDrawList;
	CLinkedList<CTreadPluginWrapper> m_PostDrawList;
	CLinkedList<CTreadPluginWrapper> m_RPAKList;
	CLinkedList<CTreadPluginWrapper> m_TPAKList;
	CLinkedList<CTreadPluginWrapper> m_TDIRList;
	CLinkedList<CTreadPluginWrapper> m_EventList;
	CLinkedList<CTreadPluginWrapper> m_ObjCreatorList;
	CLinkedList<CTreadPluginWrapper> m_MapReaderList;
	CLinkedList<CTreadPluginWrapper> m_MapWriterList;
	CLinkedList<TPMENUITEM> m_MenuList;

	bool LoadPlugin(CString sDllPath);
	CTreadPluginWrapper* FindPlugin(CLinkedList<CTreadPluginWrapper>* pList, CTreadPlugin* pPlugin);
	int UpdateDrawFlags(CLinkedList<CTreadPluginWrapper>* pList);

public:

	bool Initialize();
	bool Shutdown();

	int m_n2DPreDrawFlags;
	int m_n2DPostDrawFlags;
	int m_n3DPreDrawFlags;
	int m_n3DPostDrawFlags;
	
	TPHMENUITEM AddMenuItem(CString sItem, CTreadPlugin* pPlugin);
	void RemoveMenuItem(TPHMENUITEM hItem, CTreadPlugin* pPlugin);
	void ChangeMenuItem(TPHMENUITEM hItem, CString sNewTitle, CTreadPlugin* pPlugin);
	void SetMenuItemState(TPHMENUITEM hItem, int nChecked, bool bEnabled, CTreadPlugin* pPlugin);
	CString GetMenuItemString(TPHMENUITEM hItem, CTreadPlugin* pPlugin);
	void UpdatePluginMenu();

#ifdef _WIN32
	void UpdateCommandUI(int nMenuID, CCmdUI* pCmdUI);
#endif

	bool PostEvent(int nEvent, int nFlags, CTread3DDoc* pDocument, void* pParam=NULL, void* pParam2=NULL, void* pParam3=NULL, void* pParam4=NULL);
	void InvokePlugin(TPHMENUITEM nID, CTread3DDoc* pDocument);
	CLinkedList<TPMENUITEM>* GetPluginMenuList() { return &m_MenuList; }

	void BindEvent(CTreadPlugin* pPlugin, int nEventFlags);
	void UnbindEvent(CTreadPlugin* pPlugin, int nEventFlags);
	void BindFunction(CTreadPlugin* pPlugin, int nFunction);
	void UnbindFunction(CTreadPlugin* pPlugin, int nFunction);
	void BindPreDrawClasses(CTreadPlugin* pPlugin, int nClasses, int nType);
	void UnbindPreDrawClasses(CTreadPlugin* pPlugin, int nClasses, int nType);
	void BindPostDrawClasses(CTreadPlugin* pPlugin, int nClasses, int nType);
	void UnbindPostDrawClasses(CTreadPlugin* pPlugin, int nClasses, int nType);

	CPakFile* OpenTextureDirectory(CString sType);
	CPakFile* OpenTexturePackage(CString sFilename);
	CBaseRenderPackage* CreateRenderPackage(CString sPakName);
	CBaseObject* CreateBaseObject(int nObjectClass);

	CBaseTreadMapFile* GetMapReader(CString sTag, int nMagic, CTread3DDoc* pDocument);
	CBaseTreadMapFile* GetMapWriter(TREAD2XHEADERINFO* pHeader, CTread3DDoc* pDocument);

	void Get2DRenderPackageList(CString sNames[MAX_RENDER_PACKAGES], CString sDescriptions[MAX_RENDER_PACKAGES], int& nNumPaks);
	void Get3DRenderPackageList(CString sNames[MAX_RENDER_PACKAGES], CString sDescriptions[MAX_RENDER_PACKAGES], int& nNumPaks);

	// Draws an object.
	bool PreDrawObject(CMapView* pView, CTread3DDoc* pDocument, CBaseWindowRenderer* pWindowRenderer, CDC* pDC, void* pContext, CBaseObject* pObject, int nObjectClass, int nObjectInterfaces, int nDrawFlags, int nFlags, int nType, int nPAKUID);
	bool PostDrawObject(CMapView* pView, CTread3DDoc* pDocument, CBaseWindowRenderer* pWindowRenderer, CDC* pDC, void* pContext, CBaseObject* pObject, int nObjectClass, int nObjectInterfaces, int nDrawFlags, int nFlags, int nType, int nPAKUID);	
};

extern CTreadPluginAPI* (*GetPluginAPI)();

//
// You must export a function of this type, that creates your plugin object:
//
// CTreadPlugin* T3D_CreatePluginInstance();
//

//////////////////////////////////////////////////////////////////////
// TREADSYS.H														//
//////////////////////////////////////////////////////////////////////

/*
Nick : 9/8/99
Added CMode handling.
*/
/*
Joe : 10/10/99
Move flags to hex.
Modes are now bit flags so event responders can bind to 
multiple events.
*/


#ifdef _WIN32
#pragma warning(disable : 4244)     // truncate from double to float
#pragma warning(disable : 4800)		// forcing int to bool.
#endif



//#define DIALOGBAR_MAX 1	//max dialog bar entries

//view types
#define VIEW_TOP		0x00000001
#define VIEW_SIDE		0x00000002
#define VIEW_FRONT		0x00000004
#define VIEW_GL			0x00000008
#define VIEW_3D			0x00000008

#define FLAT_VIEWS			(VIEW_TOP|VIEW_SIDE|VIEW_FRONT)
#define ALL_VIEWS			(VIEW_3D|FLAT_VIEWS)

#define ALL_MODE_FLAGS	0xFFFFFFFF

//MODE FLAGS
#define DEFAULT_MODE	0x00000001
#define DEFAULT_SUBMODE 0x00000000
#define DEFAULT_FLAG	0x00000004

// These are bitflags so that event responders can
// mask and attach to multiple mode events.
#define MODE_POINTER	0x00000001
#define MODE_CAMERA		0x00000002
#define MODE_ZOOM		0x00000004
#define MODE_PAINT		0x00000008
#define MODE_ENTITY		0x00000010
#define MODE_BRUSH		0x00000020
#define MODE_FACE		0x00000040
#define MODE_VERTEX		0x00000080
#define MODE_CLIP		0x00000100

//SUBMODE FLAGS

#define SUB_NONE		0x00000000

//SUBMODE POINTER FLAGS
#define POINTER_MASK_SUBMODE	0x00000003	//MASK
#define POINTER_BRUSH	0x00000001			//INDEX VALUES
#define POINTER_ENTITY	0x00000002
#define POINTER_ALL		0x00000003
#define POINTER_ROTATE	0x00000004

//SUBMODE CLIP FLAGS
#define CLIP_MASK_SUBMODE		0x00000001	//MASK
#define CLIP_CLIP		0x00000000			//INDEX VALUES
#define CLIP_SPLIT		0x00000001


//FLAGS
#define FLAG_TEXLOCK		0x00000001
#define FLAG_BUILDING		0x00000002


// Console routines.
extern void (*Debug)(CString sMessage, ...);
extern void (*Warning) (CString sMessage, bool bPopup = false);
extern void (*Error)(CString sMessage, bool bConsole = true);
extern void (*Fatal)(CString sMessage, bool bConsole = false);

// Registry calls.
extern CString (*GetRegString) (UINT nCode);
extern int (*GetRegInt) (UINT nCode);
extern bool (*GetRegBool) (UINT nCode);

extern void (*SetRegString) (UINT nCode, CString sData);
extern void (*SetRegInt) (UINT nCode, int nData);
extern void (*SetRegBool) (UINT nCode, bool bData);

// Tracks a popup menu.
extern void (*TreadPopupMenu) (UINT nMenuId, int nXPos, int nYPos, CWnd* pWnd, bool bTranslate=true);

// Get's the current clipboard.
extern CLinkedList<CBaseObject>* (*GetGlobalClipboard) ();


// Tread Cursors.
#define TC_DEFAULT			-1
#define TC_IDC_ARROW		0
#define TC_IDC_IBEAM		1
#define TC_IDC_WAIT			2
#define TC_IDC_CROSS		3
#define TC_IDC_UPARROW		4
#define TC_IDC_SIZEALL		5
#define TC_IDC_SIZENWSE		6
#define TC_IDC_SIZENESW		7
#define TC_IDC_SIZEWE		8
#define TC_IDC_SIZENS		9
#define TC_IDC_ZOOM			10
#define TC_NUMCURSORS		11


// Set's the mouse cursor.
extern void (*TreadSetCursor) (CMapView* pView, int nCursor);

// Console routines.
extern void (*Console)(CString sMessage, ...);
extern void (*SetConsole)(CString sFont, int nSize, COLORREF nColor);
extern void (*ResetConsole)();
extern void (*ClearConsole)();

// Get's the global mode manager.
extern CMode* (*GetModeManager) ();

// Status bar progess control.
extern void (*PrgSetPos)(int nPos);
extern void (*PrgSetRange)(long nMin, long nMax);
extern void (*PrgStepIt)();
extern void (*PrgSetStep)(int nStep);

//
// The CMapManager class maintains the current list of maps. It's main advantage
// is to be able to get the documents and mode manager of that class by calling:
//
// GetModeManager()->GetDocPtr() which retrieves the current document.
//
// It's heavily used on Nicks side to maintain the interface.
//
class _T_EXPORT CMapManager
{
public:
	CMapManager();
	CMapManager(int nMax);
	~CMapManager();

	int RegisterMap(const CMapFrame* pWnd);
	void UnregisterMap(int nHandle);
	CMapFrame* GetCurrentMap();
	bool SetCurrentMap(int nHandle);

#ifdef _WIN32
	void SetProgressPtr(CProgressCtrl* pProgress) { m_pProgress = pProgress; }
#endif


	bool IsMap();
	int GetMapCount();

	CDC* GetPaneDC(unsigned int nPane);
	void ReleasePaneDC(unsigned int nPane, CDC* pDC);
	CRect GetPaneRect(unsigned int nPane);
	CMode* GetModePtr();
	CTread3DDoc* GetDocPtr();
	CMainFrame* GetMainFramePtr();

	void RedrawAllMaps(int nFlags=ALL_VIEWS, bool bFast=true);
	void RedrawAllMapsSelections(int nFlags=ALL_VIEWS, bool bFast=true);

	void AddGlobalRenderFlagsToAllMaps(int nFlags);
	void RemoveGlobalRenderFlagsFromAllMaps(int nFlags);

	void UpdateInterface(CTread3DDoc* pDocument = NULL);
	void RegisterMainFrame(CMainFrame* pMainFrame);
	
#ifdef _WIN32
	
	void EnableAccelerator(bool bEnable = true);
	CWinApp* GetMainApp() { return m_pWinApp; }
#endif

	bool ObjectsSelected();
	void CycleColorIfSelected();

private:
	CMapView* GetMapPane(unsigned int nPane);
	CTread3DDoc* m_pDoc;

#ifdef _WIN32
	CConsoleView* GetConsole();
	CProgressCtrl* m_pProgress;
#endif

	CMainFrame* m_pMainFrame;
	CMapFrame** m_pWndTable;

	
	int m_nMax;
	int m_nCurrent;
	CMode *m_pMode;

	CRect m_rcView[4];
	
#ifdef _WIN32
	CWinApp* m_pWinApp;
#endif
};

extern CMapManager* (*GetMapManager) ();

//////////////////////////////////////////////////////////////////////
// VIEWCONTROL.H													//
//////////////////////////////////////////////////////////////////////
//-----------------------
#define VISGROUP_VIEW	0
#define ANCHOR_VIEW		1
#define OBJECT_VIEW		2
#define TEXTURE_VIEW	3

extern CView* (*GetView) (UINT nCode);
extern void (*EnableView) (UINT nCode, bool bEnable = true);
extern void (*EnableAllViews) (bool bEnable = true);

//////////////////////////////////////////////////////////////////////
// MODE.H															//
//////////////////////////////////////////////////////////////////////
// Defined mode controller.
struct _T_EXPORT MODE_FLAGS;
struct _T_EXPORT MODE_FLAGS
{
	int nMode;
	int nSubmode;

	MODE_FLAGS* next;
	MODE_FLAGS* prev;
};

class CTread3DDoc;

#define MODES 9

class _T_EXPORT CMode
{
public:
	CMode();
	~CMode();

	void Push();
	void Pop();
	void ClearStack();
	bool IsStackEmpty();

	void FlipLast();
	//void FlipLastMode();
	//void FlipLastSubmode();
	
	void Set(int nMode, int nSubmode, bool bSet = true);
	void SetMode(int nFlag);
	void SetSubmode(int nFlag);
	void SetSubmode(int nFlag, int nMask);
	void AddSubmode(int nFlag);
	void SubtractSubmode(int nFlag);

	void RefreshMode();

	int GetMode();
	int GetSubmode();
	int GetSubmode(int nMask);
	int GetSubmodeFor(int nMode);
	int GetSubmodeFor(int nMode, int nMask);

	bool IsMode(int nMode);
	bool IsSubmode(int nSubmode) ;

	void Default();
	void DefaultMode();
	void DefaultSubmode();

	void ClearFlags();
	void SetFlags(int nFlags);
	void SetFlag(int nFlag, bool bValue);
	void AddFlags(int nFlags);
	void SubtractFlags(int nFlags);
	void ToggleFlag(int nFlags);
	int GetFlags();
	bool IsFlag(int nFlags);

	void SetDocument(CTread3DDoc* pDocument) { m_pDocument = pDocument; }

private:
	int m_nMode;
	int m_nSubmode[MODES];
	int m_nFlags;

	int m_nLastMode;
	int m_nLastSubmode;

	CTread3DDoc* m_pDocument;
	CLinkedList<MODE_FLAGS> m_modeList;
};

//////////////////////////////////////////////////////////////////////
// TEXCOMBOBOX.H													//
//////////////////////////////////////////////////////////////////////
struct TEXMRUREF;
class CTexture;

class _T_EXPORT CComboBox;
class _T_EXPORT CTexComboBox : public CComboBox
{
	struct BITMAPDIB24
	{
		BITMAPINFOHEADER bmiHeader;
		RGBQUAD bmiColors;
	};

private:
	CFont m_ftText;
	BYTE* m_bits;
	HBITMAP m_bmhandle;
	BITMAPDIB24 m_bih;
	CTexture* m_pTexture;
	CTexture** m_pTexList;
	CLinkedList<TEXMRUREF>* m_pTexMru;
	int m_nCount;
	CRect m_rcClient;

// Construction
public:
	CTexComboBox();

// Attributes
public:

// Operations
public:
	void Initialize();

	bool SetTexturePointer(CTexture* pTexture);
	bool SetTexturePointer(CTexture** pTexList, int nCount);
	bool SetTexturePointer(CLinkedList<TEXMRUREF>* pTexMru);

	void Update();
	
	void ResetList();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTexComboBox)
	public:
	virtual int CompareItem(LPCOMPAREITEMSTRUCT lpCompareItemStruct);
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTexComboBox();

	// Generated message map functions
protected:
	//{{AFX_MSG(CTexComboBox)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////
// TEXLISTBOX.H														//
//////////////////////////////////////////////////////////////////////
class _T_EXPORT CTexListBox : public CListBox
{
	struct BITMAPDIB24
	{
		BITMAPINFOHEADER bmiHeader;
		RGBQUAD bmiColors;
	};
private:
	CFont ftText;
	BITMAPDIB24 bih;
	int	m_nMaxWidth, m_nMaxHeight;
	CTexture* m_pTexture;
	CTexture** m_pTexList;
	CLinkedList<TEXMRUREF>* m_pTexMru;
	int m_nCount;
	CRect m_rcClient;

	// Construction
public:
	CTexListBox();

// Attributes
public:
	void Initialize();
	void SetMaxItemSize(int width, int height);

	bool SetTexturePointer(CTexture* pTexture);
	bool SetTexturePointer(CTexture** pTexList, int nCount);
	bool SetTexturePointer(CLinkedList<TEXMRUREF>* pTexMru);

	CTexture* GetTexturePointer();

	void Update();
	void Update(CString sFilter);

	void ResetList();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTexListBox)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	virtual int CompareItem(LPCOMPAREITEMSTRUCT lpCompareItemStruct);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTexListBox();

	// Generated message map functions
protected:
	//{{AFX_MSG(CTexListBox)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////
// VISGROUPVIEW.H													//
//////////////////////////////////////////////////////////////////////
class _T_EXPORT CVisgroupView : public CTreeView
{
protected:
	CVisgroupView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CVisgroupView)

// Attributes
public:
	void LoadVisgroup();
	void UpdateVisgroup();
	void ResetContent();

	HTREEITEM InsertItem(CString sGroup, CString sItem, bool bVisible = true);
	HTREEITEM InsertItem(HTREEITEM hGroup, CString sItem, bool bVisible = true);

	HTREEITEM FindCategory(CString &sItem);
	HTREEITEM FindVisgroup(CString &sItem, HTREEITEM hParent);

	bool GetSelection(CString &sItem, CString &sGroup);

private:
	HTREEITEM HighlightDropTarget(CPoint point);
	void CopyChildren (HTREEITEM hDest, HTREEITEM hSrc);
    void CopyTree (HTREEITEM hDest, HTREEITEM hSrc);
    void MoveTree (HTREEITEM hDest, HTREEITEM hSrc);
    bool IsChildOf (HTREEITEM hItem1, HTREEITEM hItem2);
	bool IsItemExpanded(HTREEITEM hItem);

	int m_nDelayInterval;
	int m_nScrollMargin;
	int m_nScrollInterval;
	
	bool m_bDragging;
	UINT nTimer;

	CTreeCtrl* m_TreeCtrl;
	CImageList m_ImgList;
	CImageList* m_pImgDrag;
	
	HTREEITEM m_hSelect;
	HTREEITEM m_hDrag;
		
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVisgroupView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CVisgroupView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CVisgroupView)
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnVisgroupAddcat();
	afx_msg void OnBeginlabeledit(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEndlabeledit(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnVisgroupHideall();
	afx_msg void OnVisgroupShowall();
	afx_msg void OnItemexpanding(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBegindrag(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnVisgroupDelete();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
//////////////////////////////////////////////////////////////////////
// OBJECTVIEW.H														//
//////////////////////////////////////////////////////////////////////
class _T_EXPORT CObjectView : public CTreeView
{
protected:
	CObjectView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CObjectView)

// Attributes
public:
	void InsertGroup(CString sGroup);
	HTREEITEM InsertItem(CString sItem);
	HTREEITEM InsertItem(CString sGroup, CString sItem);

	void LoadEntityList();
	void LoadBrushList();
	bool EntityListLoaded();
	bool BrushListLoaded();

	HTREEITEM FindCategory(CString &sItem);
	HTREEITEM FindItem(CString &sItem, HTREEITEM hParent);
	HTREEITEM FindItem(CString &sItem);

	bool GetSelection(CString &sItem, CString &sGroup);

	void ResetContent();

private:
	CImageList imgList;
	CTreeCtrl* m_TreeCtrl;

	HTREEITEM m_hGroup;
	HTREEITEM m_hSelection;

	int m_nMode;
	bool m_bLoading;
	//bool m_bBrushLoaded;
	//bool m_bEntityLoaded;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CObjectView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CObjectView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CObjectView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRclick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemexpanding(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////
// TARGETAPI.H														//
//////////////////////////////////////////////////////////////////////
//
// This is Treads built in Targetting system (i.e. the system that draws
// lines between objects to represent targets). It works simply by
// associating an object with it's name. To setup this association you
// call: SetTargetBinding("TargetName", pObject);. You can associate
// more than one object with the same name.
//
// Tread will automatically draw the targets of any object that returns
// the I_TARGETABLE interface. This means that it must derrive from
// CBaseTargetObject. Tread looks up the name that the object targets
// and asks the TargetAPI to return a list of objects that have bound
// to that name via SetTargetBinding(). This returns a TTARGET struct
// that contains a list of TREF's. Each TREF references an object.
//
// To unbind an object from it's name call:
// SetTargetBinding("", pObject);.
//
struct TREF
{
	TREF* prev, *next;
	CBaseObject* pObject;
};

struct TTARGET
{
	TTARGET* prev, *next;

	CString sName;
	CLinkedList<TREF> TRefList;
};

class _T_EXPORT CTargetAPI
{
	CLinkedList<TTARGET> m_TargetList;

	TREF* FindTRef(TTARGET* pTarget, CBaseObject* pObj);

public:

	// "" == Remove binding.
	void SetTargetBinding(CString sTarget, CBaseObject* pObj);
	TTARGET* FindTargetSet(CString sName);
	CString GenerateTargetName();
};


//////////////////////////////////////////////////////////////////////
// UNDOAPI.H														//
//////////////////////////////////////////////////////////////////////
//
// Treads UndoAPI is the class that maintains a list of operations
// done on geometry. This list can then be used to "undo" that set
// of operations.
//
// Unfortunately this API is not currently expandable as far as the types
// of operations it will keep track of. This may change in the future.
//
// The UAF_ flags define the operations that Tread will undo/redo. The
// system works simply by storing the operation done, and the list of
// operands effected by the operation. This is stored via the UNDOITEM
// structure.
//
// If you have a list of items that are affected by an action, there
// is a convenient function provided that will produce an UNDOITEM struct
// for you: CUndoAPI::CreateUndoItem().
//
// It would be too exhaustive for me to attempt to explain this whole
// system without source, so I'll probably have to give some sample
// code at some point that uses this.
//

#define UAF_GMEM		0x00000002	// Group member
#define UAF_HEAD		0x00000004	// Head items are always empty.
#define UAF_TAIL		0x00000008
#define UAF_TRANSLATE	0x00000010
#define UAF_ROTATE		0x00000020
#define UAF_CARVE		0x00000040
#define UAF_DELETE		0x00000080
#define UAF_CREATE		0x00000100
#define UAF_SIZE		0x00000200
#define UAF_GROUP		0x00000400
#define UAF_UNGROUP		0x00000800
#define UAF_HOLLOW		0x00001000
#define UAF_LINK		0x00002000
#define UAF_UNLINK		0x00004000

struct UNDOREF
{
	UNDOREF* prev, *next;
	CBaseObject* pObj;
};

struct UNDOITEM
{
	UNDOITEM* prev, *next;

	int nFlags;
	int nCount;
	int nURF;
	vec3_t vUVals;
	vec3_t vUOrigin;
	vec3_t vRVals;
	vec3_t vROrigin;
	void* pData;
	void* pData2;
	CLinkedList<UNDOREF> RefList;
};

#define URF_FAIL			-1
#define URF_OK				0x00000000
#define URF_CLEARSEL		0x00000001
#define URF_ADDTOSEL		0x00000002
#define URF_BUILDSELBOX		0x00000004
#define URF_BUILDHOTSPOTS	0x00000008	

class _T_EXPORT CUndoAPI
{
private:

	bool m_bEOL;
	UNDOITEM* m_pStackPos;
	CLinkedList<UNDOITEM> m_UndoList;
	void AddItemToSelection(UNDOITEM* pItem, CTread3DDoc* pDocument);
	void AddItemToMap(UNDOITEM* pItem, CTread3DDoc* pDocument);
	void RemoveItemFromMap(UNDOITEM* pItem, CTread3DDoc* pDocument);
	void PurgeLastItem(CTread3DDoc* pDocument);
	void PurgeFromPos(CTread3DDoc* pDocument);
	void PurgePP(CTread3DDoc* pDocument, int nCount);
	void PurgeBF(CTread3DDoc* pDocument, int nCount);
	void Translate(UNDOITEM* pItem, vec3_t vVec);
	void SetSize(UNDOITEM* pItem, vec3_t vOldSize, vec3_t vNewSize, vec3_t vOrigin);
	void Scale(UNDOITEM* pItem, vec3_t vScale, vec3_t vOrigin);
	void SetOrigin(UNDOITEM* pItem, vec3_t vOldOrigin, vec3_t vNewOrigin);
	void Rotate(UNDOITEM* pItem, vec3_t vRot, vec3_t vOrigin);
	UNDOITEM* GetRedoItem();
	int PerformUndoRedo(UNDOITEM* pItem, CTread3DDoc* pDocument, bool bUndo = true);

public:

	CUndoAPI();

	void ClearUndoStack(CTread3DDoc* pDocument);
	void AddUndoItem(UNDOITEM* pItem, CTread3DDoc* pDocument);
	int Undo(CTread3DDoc* pDocument);
	int Redo(CTread3DDoc* pDocument);
	bool CanUndo();
	bool CanRedo();
	CString GetUndoString();
	CString GetRedoString();

	static UNDOITEM* CreateUndoItem(CLinkedList<CBaseObject>* pList);
};

//////////////////////////////////////////////////////////////////////
// MAPOBJECT.H														//
//////////////////////////////////////////////////////////////////////
//
// This defines the base classes at which the heart of Tread really lies.
// It works with these base classes, allowing all sorts of crazy derrivations
// to take place without any internal rewritting.
//
#define OBJECT_CLASS_UNKNOWN			-1
#define OBJECT_CLASS_ENTITY				0x00000001
#define OBJECT_CLASS_BRUSH				0x00000002
#define OBJECT_CLASS_FACE				0x00000004
#define OBJECT_CLASS_WINDING			0x00000008
#define OBJECT_CLASS_PATCHMESH			0x00000010
#define OBJECT_CLASS_GROUP				0x00000020
#define OBJECT_CLASS_BASE				0x00000040
#define OBJECT_CLASS_CAMERA				0x00000080
#define OBJECT_CLASS_HOTSPOT			0x00000100
#define OBJECT_CLASS_CLIPTOOL			0x00000200
#define OBJECT_CLASS_LENTITY			0x00000400
#define OBJECT_CLASS_ANCHOR				0x00000800

// We reserve object id's up to 0x00800000
// Please do not use any numbers up to and
// including this define.
#define OBJECT_CLASS_RESERVED_LAST		0x00800000
#define OBJECT_CLASS_RESERVED_MASK		0x00FFFFFF
#define OBJECT_CLASS_USER_MASK			0xFF000000

// These bits are free for use.
#define OBJECT_CLASS_USER1				0x01000000
#define OBJECT_CLASS_USER2				0x02000000
#define OBJECT_CLASS_USER3				0x04000000
#define OBJECT_CLASS_USER4				0x08000000
#define OBJECT_CLASS_USER5				0x10000000
#define OBJECT_CLASS_USER6				0x20000000
#define OBJECT_CLASS_USER7				0x40000000
#define OBJECT_CLASS_USER8				0x80000000

#define OBJECT_CLASS_ALL				0xFFFFFFFF

#define I_NONE			0x00000000
#define I_BASE			0x00000001
#define I_STORABLE		0x00000002
#define I_TRANSFORMABLE	0x00000004
#define I_TEXTUREABLE	0x00000008
#define I_GROUPABLE		0x00000010
#define I_CHECKABLE		0x00000020
#define I_VISGROUPREF	0x00000040
#define I_GROUP			0x00000080
#define I_HSPOTGEN		0x00000100
#define I_UI			0x00000200	// User Interface Object
#define I_CARVE			0x00000400	// Can carve with it.
#define I_HOLLOW		0x00000800	// Can be hollowed.
#define I_CARVEABLE		0x00001000	// Can be carved.
#define I_TEXADJUST		0x00002000	// Texture can be adjusted via face dialog.
#define I_TARGET		0x00004000	// Target.
#define I_LINK			0x00008000	// Linkable.
#define I_UNLINK		0x00010000	// Unlinkable.
#define I_FGROUP		0x00020000	

#define I_RESERVED_LAST 0x00800000
#define I_RESERVED_MASK	0x00FFFFFF
#define I_USER_MASK		0xFF000000
#define I_USER1			0x01000000
#define I_USER2			0x02000000
#define I_USER3			0x04000000
#define I_USER4			0x08000000
#define I_USER5			0x10000000
#define I_USER6			0x20000000
#define I_USER7			0x40000000
#define I_USER8			0x80000000

#define I_ALL			0xFFFFFFFF

// These macro's ease the casting of object interfaces.

#define ICT_GEN(a, i)	((a->GetObjectInterfaces(i)&i)==i)
#define  IC_GEN(a, i)	(a->GetInterface(i))

#define ICT_BASE(a)					ICT_GEN(a, I_BASE)
#define  IC_BASE(a)					 IC_GEN(a, I_BASE)
#define ICT_STORABLE(a)				ICT_GEN(a, I_STORABLE)
#define  IC_STORABLE(a)				(CBaseStorableObject*)IC_GEN(a, I_STORABLE)
#define ICT_TRANSFORMABLE(a)		ICT_GEN(a, I_TRANSFORMABLE)
#define  IC_TRANSFORMABLE(a)		(CBaseTransformableObject*)IC_GEN(a, I_TRANSFORMABLE)
#define ICT_TEXTUREABLE(a)			ICT_GEN(a, I_TEXTUREABLE)
#define  IC_TEXTUREABLE(a)			(CBaseTextureableObject*)IC_GEN(a, I_TEXTUREABLE)
#define ICT_GROUPABLE(a)			ICT_GEN(a, I_GROUPABLE)
#define  IC_GROUPABLE(a)			(CBaseGroupableObject)IC_GEN(a, I_GROUPABLE)
#define ICT_CHECKABLE(a)			ICT_GEN(a, I_CHECKABLE)
#define  IC_CHECKABLE(a)			(CBaseCheckableObject*)IC_GEN(a, I_CHECKABLE)
#define ICT_VISGROUPREF(a)			ICT_GEN(a, I_VISGROUPREF)
#define  IC_VISGROUPREF(a)			(CVisgroupRef*)IC_GEN(a, I_VISGROUPREF)
#define ICT_GROUP(a)				ICT_GEN(a, I_GROUP)
#define  IC_GROUP(a)				(CObjectGroup*)IC_GEN(a, I_GROUP)
#define ICT_HSPOTGEN(a)				ICT_GEN(a, I_HSPOTGEN)
#define  IC_HSPOTGEN(a)				(CBaseHotSpotGenerator*)IC_GEN(a, I_HSPOTGEN)
#define ICT_UI(a)					ICT_GEN(a, I_UI)
#define ICT_CARVE(a)				ICT_GEN(a, I_CARVE)
#define ICT_HOLLOW(a)				ICT_GEN(a, I_HOLLOW)
#define ICT_CARVEABLE(a)			ICT_GEN(a, I_CARVEABLE)
#define  IC_CARVEABLE(a)			(CBaseCarveableObject*)IC_GEN(a, I_CARVEABLE)
#define ICT_TEXADJUST(a)			ICT_GEN(a, I_TEXADJUST)
#define ICT_TARGET(a)				ICT_GEN(a, I_TARGET)
#define  IC_TARGET(a)				(CBaseTargetObject*)IC_GEN(a, I_TARGET)
#define ICT_LINK(a)					ICT_GEN(a, I_LINK)
#define ICT_UNLINK(a)				ICT_GEN(a, I_UNLINK)
#define ICT_FGROUP(a)				ICT_GEN(a, I_FGROUP)

class CTread3DDoc;

// A storable object.
class _T_EXPORT CBaseStorableObject
{
public:

	CBaseStorableObject() {}
	virtual ~CBaseStorableObject() {}

	virtual bool ReadObject(CFile& hFile, int nVersion) = 0;
	virtual bool WriteObject(CFile& hFile, int nVersion) = 0;

	// If a higher object wants to be read/written it should
	// extend this to return the I_STORABLE interface.
	virtual int GetObjectInterfaces() = 0;
	virtual int GetObjectClass() = 0;

	// Writes object id.
	bool WriteObjectClass(CFile& hFile)
	{
		int nId;
		
		nId = GetObjectClass();
		hFile.Write(&nId, 4);	

		return true;
	}

	// Read's an object class.
	static int ReadObjectClass(CFile& hFile)
	{
		int nId;
		hFile.Read(&nId, 4);
		return nId;
	}

};

// A Base Object.
class _T_EXPORT CBaseObject : public CLLObject, public CBaseStorableObject
{
public:

	int m_nObjectRenderFlags;
	int m_nClassRenderFlags;
	int m_nInterfaceRenderFlags;
	int m_nViewTypeFilters;

	CBaseObject() : CLLObject(), CBaseStorableObject()
	{ 
		m_nObjectRenderFlags = 0; 
		m_nClassRenderFlags = 0;
		m_nInterfaceRenderFlags = 0;
		m_nViewTypeFilters = 0;
	}

	CBaseObject(const CBaseObject& ObjectToCopy) : CLLObject(ObjectToCopy), CBaseStorableObject()
	{
		m_nObjectRenderFlags = ObjectToCopy.m_nObjectRenderFlags;
		m_nClassRenderFlags = ObjectToCopy.m_nClassRenderFlags;
		m_nInterfaceRenderFlags = ObjectToCopy.m_nInterfaceRenderFlags;
		m_nViewTypeFilters = ObjectToCopy.m_nViewTypeFilters;
	}

	virtual ~CBaseObject()
	{ 
	}
	
	// Called before object is deleted.
	virtual void PreDelete(CTread3DDoc* pDocument) {}
	// Called when an action is undone on the object.
	virtual void Undo(int nFlags, CTread3DDoc* pDocument) {}
	// Called when an action is redone on the object.
	virtual void Redo(int nFlags, CTread3DDoc* pDocument) {}

	// Get's the object class.
	virtual int GetObjectClass() { return OBJECT_CLASS_BASE; }

	virtual CBaseObject* Clone();

#define IALF_OK			0x00000000
#define IALF_REMOVE		0x00000001
#define IALF_REMOVEQ	0x00000002	// Quite remove.
#define IALF_FATAL		0x00000004
#define IALF_NOENTDEF	0x00000008
#define IALF_NOKEYDEF	0x00000010
#define IALF_NOTEXTURES	0x00000020
#define IALF_DELETE		0x00000040
#define IALF_DELETEQ	0x00000080
#define IALF_RESTART	0x00000100
#define IALF_END		0x00000200
#define IALF_ALL		0xFFFFFFFF

	virtual int InitAfterLoad(CTread3DDoc* pDocument) { return IALF_OK; }
	virtual int PostInitAfterLoad(CTread3DDoc* pDocument, int nCode) { return IALF_OK; }

	virtual void InitFromSelection(CTread3DDoc* pDocument, CMapView* pView, vec3_t vMins, vec3_t vMaxs, vec3_t vOrigin) {}

	// If an higher object wants to be read/written it should
	// extend this to return the I_STORABLE interface.
	// This defines what interface classes this object can be
	// cast to.
	virtual int GetObjectInterfaces(){ return I_BASE; }
	virtual void* GetInterface(int nInterface);

	// Storable Object.
	virtual bool ReadObject(CFile& hFile, int nVersion) { return false; }
	virtual bool WriteObject(CFile& hFile, int nVersion) { return false; }

};

// A groupable object is an object that can be stored in 
// a CObjectGroup class.

class _T_EXPORT CObjectGroup;
class _T_EXPORT CBaseGroupableObject : public CBaseObject
{
protected:

	CObjectGroup* m_pGroup;

public:
	
	CBaseGroupableObject() : CBaseObject() { m_pGroup = NULL; }
	CBaseGroupableObject(const CBaseGroupableObject&  ObjectToCopy) : CBaseObject(ObjectToCopy)
	{ m_pGroup = ObjectToCopy.m_pGroup; }
	virtual ~CBaseGroupableObject() {}

	virtual CBaseObject* Clone() { return new CBaseGroupableObject(*this); }

	CObjectGroup* GetParentGroup() { return m_pGroup; }
	CObjectGroup* GetHeadGroup();
	virtual int GetObjectInterfaces() { return I_BASE | I_GROUPABLE; }
	friend class CObjectGroup;
};

// A carvable object.
class _T_EXPORT CObjectGroup;
class _T_EXPORT CBaseCarveableObject
{
public:

	CBaseCarveableObject() {}
	virtual ~CBaseCarveableObject() {}

	virtual CObjectGroup* CSGSubtract(CBaseObject* pObject) { return NULL; }
	virtual bool ObjectsIntersect(CBaseObject* pObject) { return false; }
	virtual int GetCarveClasses() const { return 0; }
};

// Target object.
// I_TARGETABLE
class _T_EXPORT CBaseTargetObject
{
public:

	CBaseTargetObject() {}
	virtual ~CBaseTargetObject() {}

	virtual CString GetTargetableName() = 0;
	virtual void SetTargetableName(CString sName, CTread3DDoc* pDocument) = 0;
	virtual CString GetTargetName() = 0;
	virtual void SetTargetName(CString sName, CTread3DDoc* pDocument) = 0;

};


// An object that can be transformed.
class _T_EXPORT CBaseTransformableObject
{
public:

	// Data
	vec3_t m_vOrigin;
	vec3_t m_vMins, m_vMaxs;

	// Methods.
	CBaseTransformableObject()
	{
		m_vOrigin[0] = 0.0F;
		m_vOrigin[1] = 0.0F;
		m_vOrigin[2] = 0.0F;
	}

	CBaseTransformableObject(const CBaseTransformableObject& ObjectToCopy)
	{ VectorCopy(ObjectToCopy.m_vOrigin, m_vOrigin); }

	virtual ~CBaseTransformableObject() {}

	virtual void SetOrigin(vec3_t vOrigin) {}
	virtual void Scale(vec3_t vScale) {}	// Scales the objects size.
	virtual void Scale(vec3_t vScale, vec3_t vOrigin) {}
	virtual void Translate(vec3_t vTranslation) {}
	virtual void Rotate(vec3_t vRot) {}
	virtual void Rotate(vec3_t vOrigin, vec3_t vRot) {}
	virtual void InsertIntoRange(vec3_t vMins, vec3_t vMaxs) {}
	virtual void Reflect(int nAxis, vec3_t vFlipOrigin) {}
	virtual void ComputeOrigin() {}
	virtual void BuildRange() {}
	virtual bool InRange(vec3_t vMins, vec3_t vMaxs) { return false; }
	virtual bool In2DRange(vec3_t vMins, vec3_t vMaxs, int nView) { return false; }
};


// Defines a textureable object.
class _T_EXPORT CBaseTextureableObject
{
protected:

	CString	m_sTexture;
	CTexture* m_pTexture;
	vec_t	  m_vRotation;
	vec_t	  m_vTexShifts[2];
	vec_t	  m_vTexScales[2];
	
	// Writes the texture info.
	bool WriteTextureInfo(CFile& hFile, int nVersion);

	// Reads the texture info.
	bool ReadTextureInfo(CFile& hFile, int nVersion);

	// Builds the texture reference.
	bool AttachTexture();

	// Detaches the texture.
	bool DetachTexture();

public:

	int	m_nSurface;
	int m_nContents;
	int m_nValue;

	CBaseTextureableObject()
	{
		m_sTexture = "";
		m_pTexture = NULL;
		m_vRotation = 0.0F;
		m_vTexShifts[0] = 0.0F; m_vTexShifts[1] = 0.0F;
		m_vTexScales[0] = 1.0F; m_vTexScales[1] = 1.0F;
		m_nSurface = 0;
		m_nContents = 0;
		m_nValue = 0;
	}

	CBaseTextureableObject(const CBaseTextureableObject& ObjectToCopy)
	{
		m_sTexture = ObjectToCopy.m_sTexture;
		m_pTexture = ObjectToCopy.m_pTexture;
		m_vRotation = ObjectToCopy.m_vRotation;
		m_vTexShifts[0] = ObjectToCopy.m_vTexShifts[0];
		m_vTexShifts[1] = ObjectToCopy.m_vTexShifts[1];
		m_vTexScales[0] = ObjectToCopy.m_vTexScales[0];
		m_vTexScales[1] = ObjectToCopy.m_vTexScales[1];
		m_nSurface = ObjectToCopy.m_nSurface;
		m_nContents = ObjectToCopy.m_nContents;
		m_nValue = ObjectToCopy.m_nValue;
	}
	
	virtual ~CBaseTextureableObject() {}

	void SetRotation(vec_t vRotation, bool bRebuild=true)
	{ 
		m_vRotation = vRotation; 
		if(bRebuild)
			RebuildTexture();
	}

	vec_t GetRotation() { return m_vRotation; }

	void SetTexShifts(vec_t vTexShifts[2], bool bRebuild=true)
	{
		m_vTexShifts[0] = vTexShifts[0];
		m_vTexShifts[1] = vTexShifts[1];
		if(bRebuild)
			RebuildTexture();
	}

	void GetTexShifts(vec_t vTexShifts[2])
	{
		vTexShifts[0] = m_vTexShifts[0];
		vTexShifts[1] = m_vTexShifts[1];
	}

	void SetTexScales(vec_t vTexScales[2], bool bRebuild=true)
	{
		m_vTexScales[0] = vTexScales[0];
		m_vTexScales[1] = vTexScales[1];
		if(bRebuild)
			RebuildTexture();
	}
	
	void GetTexScales(vec_t vTexScales[2])
	{
		vTexScales[0] = m_vTexScales[0];
		vTexScales[1] = m_vTexScales[1];
	}

	CString GetTextureName() { return m_sTexture; }
	void SetTextureName(const char* szName, bool bEvent=true);
	virtual CTexture* GetTexturePtr() { AttachTexture(); return m_pTexture; }

	// Events.
	virtual void OnTextureChange() {}
	virtual void OnAttributeChange() {}
	virtual void RebuildTexture() {}
};

// An object that can have integrity checked.
#define CHECK_OK	0x00000000
class _T_EXPORT CBaseCheckableObject
{
public:
	
	CBaseCheckableObject() {}
	virtual ~CBaseCheckableObject() {}

	int CheckObject() { return CHECK_OK; }
	CString GetCheckMessage(int nCode) { return "CBaseCheckable: No Error Message Defined."; }
};

// A VisgroupRef is an object that references a visgroup.
class _T_EXPORT CVisgroup;
class _T_EXPORT CVisgroupRef : public CLLObject
{
private:

	CVisgroup* m_pVisgroup;
	CString m_sName, m_sCategory;
	CTread3DDoc* m_pDocument;

public:

	CVisgroupRef();
	CVisgroupRef(const CVisgroupRef& VisgroupRefToCopy);
	virtual ~CVisgroupRef();
	
	bool Read(CFile& hFile);
	bool Write(CFile& hFile);

	void SetVisgroup(CVisgroup* pVisgroup) { m_pVisgroup = pVisgroup; }
	void SetVisgroup(CString sName, CString sCategory);
	CString GetName();
	CString GetCategory();
	bool IsVisible();
	void BindToDocument(CTread3DDoc* pDocument);
};

class _T_EXPORT CVisgroupRefObject
{
private:

	CVisgroupRef m_VisgroupRef;

public:

	CVisgroupRefObject() {}
	virtual ~CVisgroupRefObject() {}

	bool Read(CFile& hFile) { return m_VisgroupRef.Read(hFile); }
	bool Write(CFile& hFile) { return m_VisgroupRef.Write(hFile); }

	void SetVisgroup(CVisgroup* pVisgroup) { m_VisgroupRef.SetVisgroup(pVisgroup); }
	void SetVisgroup(CString sName, CString sCategory) { m_VisgroupRef.SetVisgroup(sName, sCategory); }
	CString GetName() { return m_VisgroupRef.GetName(); }
	CString GetCategory() { return m_VisgroupRef.GetCategory(); }
	bool IsVisible() { return m_VisgroupRef.IsVisible(); }
	void BindToDocument(CTread3DDoc* pDocument) { m_VisgroupRef.BindToDocument(pDocument); }
};

// A group of map objects.
class _T_EXPORT CObjectGroup : public CBaseGroupableObject, public CVisgroupRefObject, 
	public CBaseTransformableObject, public CBaseTextureableObject,
	public CBaseCarveableObject
{
protected:


	CLinkedList<CBaseObject>* m_pObjectList;

public:

	CObjectGroup();
	CObjectGroup(const CObjectGroup& ObjectToCopy);
	virtual ~CObjectGroup();

	// Adds/Removes an object.
	virtual bool AddObject(CBaseGroupableObject* pObject);
	virtual bool RemoveObject(CBaseGroupableObject* pObject);

	CLinkedList<CBaseObject>* GetObjectList() { return m_pObjectList; }

	// From CBaseObject.
	virtual int GetObjectClass() { return OBJECT_CLASS_GROUP; }
	virtual int GetObjectInterfaces() { return I_CARVE | I_CARVEABLE | I_GROUPABLE | I_GROUP | I_VISGROUPREF | I_TRANSFORMABLE | I_TEXTUREABLE | I_STORABLE; }
	virtual void* GetInterface(int nInterface);

	virtual void SetOrigin(vec3_t vOrigin);
	virtual void Scale(vec3_t vScale);
	virtual void Scale(vec3_t vScale, vec3_t vOrigin);
	virtual void Translate(vec3_t vTranslation);
	virtual void Rotate(vec3_t vRot);
	virtual void Rotate(vec3_t vOrigin, vec3_t vRot);
	virtual void InsertIntoRange(vec3_t vMins, vec3_t vMaxs);
	virtual void ComputeOrigin();
	virtual void BuildRange();
	virtual void Reflect(int nAxis, vec3_t vFlipOrigin);
	virtual bool In2DRange(vec3_t vMins, vec3_t vMaxs, int nView);

	virtual CObjectGroup* CSGSubtract(CBaseObject* pObject);
	virtual bool ObjectsIntersect(CBaseObject* pObject);
	virtual int GetCarveClasses() const { return OBJECT_CLASS_BRUSH|OBJECT_CLASS_GROUP; }

	virtual CBaseObject* Clone() { return new CObjectGroup(*this); }

	// Events.
	virtual void OnTextureChange();

	// Storable Object.
	virtual bool ReadObject(CFile& hFile, int nVersion);
	virtual bool WriteObject(CFile& hFile, int nVersion);
	virtual int InitAfterLoad(CTread3DDoc* pDocument);

	static CObjectGroup* CarveWithGroup(CBaseCarveableObject* pCarveTarget, CObjectGroup* pCarveGroup);
};

extern CBaseObject* (*NewBaseObject) (int nClassType);

//////////////////////////////////////////////////////////////////////
// VISGROUP.H														//
//////////////////////////////////////////////////////////////////////
// Defines visgroup api system.										

class _T_EXPORT CVisgroup : virtual public CLLObject
{
private:

	CString m_sName;
	CString m_sCategory;
	bool	m_bVisible, m_bPending;

public:

	CVisgroup()
	{
		m_sName = "";
		m_sCategory = "";
		m_bVisible = true;
	}

	CVisgroup(CString sName, CString sCategory, bool bVisible = true)
	{
		m_sName = sName;
		m_sCategory = sCategory;
		m_bVisible = bVisible;
	}

	bool Read(CFile& hFile, int nVersion);
	bool Write(CFile& hFile, int nVersion);

	CString GetName() const { return m_sName; }
	CString GetCategory() const { return m_sCategory; }

	void SetName(CString sName) { m_sName = sName; }
	void SetCategory(CString sCategory) { m_sCategory = sCategory; }
	void SetVisible(bool bVisible = true) { m_bVisible = bVisible; }
	void SetPending(bool bVisible = true) { m_bPending = bVisible; }
	void ApplyPending() { m_bVisible = m_bPending; }
	bool GetVisible() const { return m_bVisible; }
	bool GetPending() const { return m_bPending; }
	
	friend class CVisgroupAPI;
};

// The VisgroupAPI.
// Maintains a list of visgroups, and the references that reference the groups.
class CVisgroupAPI
{
private:

	CLinkedList<CVisgroup> m_VisgroupList;
	CLinkedList<CVisgroupRef> m_VisgroupRefList;

	void UpdateReferences(CString sName, CString sCategory, CVisgroup* pNewVisgroup);

public:

	CVisgroupAPI()
	{
		m_VisgroupRefList.SetAutoDelete(false);
	}

	void AddVisgroup(CVisgroup* pVisgroup);
	CVisgroup* AddVisgroup(CString sName, CString sCategory);
	bool DeleteVisgroup(CString sName, CString sCategory);
	bool DeleteVisgroup(CVisgroup* pVisgroup);
	bool CombineVisgroups(CVisgroup* pDstGroup, CVisgroup* pSrcGroup);
	CVisgroup* FindVisgroup(CString sName, CString sCategory);

	// Add's a reference visgroup.
	void AddVisgroupRef(CVisgroupRef* pVisgroupRef);
	void DeleteVisgroupRef(CVisgroupRef* pVisgroupRef);
	CLinkedList<CVisgroup>* GetVisgroupList() { return &m_VisgroupList; }

	// Reads/Writes all visgroups.
	bool WriteVisgroups(CFile& hFile, int nVersion);
	bool ReadVisgroups(CFile& hFile, int nVersion);
};

extern CVisgroupAPI* (*GetVisgroupAPI) ();

//////////////////////////////////////////////////////////////////////
// TREADMAPAPI.H													//
//////////////////////////////////////////////////////////////////////
// Treads map system.
#define T2XHF_NONE			0x00000000
#define T2XHF_BRUSH			0x00000001
#define T2XHF_MAPFILE		0x00000002
#define T2XHF_MODEL			0x00000004
#define T2XHF_TRD2FILE		0x00000008
#define T2XHF_IGNORE		0x00000010
#define T2XHF_NOAUTOSIZE	0x00000020
#define T2XHF_RTOBJECT		0x00000040

struct RUNSTATE;

struct _T_EXPORT TREAD2XHEADERINFO
{
	int m_nVersion;
	int m_nFlags;
	int m_nValue;
	int m_nClass;	// Only for T2XHF_OBJX

	int m_nRenderFlags;
	int m_nGridSize;
	int m_nCurveSubDepth;
	bool m_bGridSnap;
	bool m_bBrushHandles;
	bool m_bDrawTargets;
	bool m_bUpdateAllViews;
	int m_nViewTypes[4];
	float m_fScale[4];
	float m_fX[4];
	float m_fY[4];
	float m_fHollowDepth;

	CString m_sGame;
	CString m_sAuthor;
	CString m_sComments;
	CString m_sCategory;
	CString m_sName;

	int m_nNumStates;
	RUNSTATE* m_pRunStates;

	TREAD2XHEADERINFO()
	{
		m_nVersion = 0;
		m_nFlags = T2XHF_NONE;
		m_nClass = -1;
		m_sGame="";
		m_sAuthor="";
		m_sComments="";
		m_sCategory=""; // For models.
		m_sName="";		// For models.
	}
};

class CTread3DDoc;
class _T_EXPORT CBaseTreadMapFile
{
public:

	CBaseTreadMapFile() {}
	virtual ~CBaseTreadMapFile() {}

	virtual bool WriteHeader(CFile& hFile, TREAD2XHEADERINFO* pHeader, CTread3DDoc* pDocument) { return true; }
	virtual bool ReadHeader(CFile& hFile, TREAD2XHEADERINFO* pHeader, CTread3DDoc* pDocument) { return true; }
	virtual bool WriteMap(CFile& hFile, TREAD2XHEADERINFO* pHeader, CTread3DDoc* pDocument, CLinkedList<CBaseObject>* pObjectList, CLinkedList<CBaseObject>* pSelectedObjectList, CLinkedList<CBaseObject>* pUIObjectList) { return true; }
	virtual bool ReadMap(CFile& hFile, TREAD2XHEADERINFO* pHeader, CTread3DDoc* pDocument, CLinkedList<CBaseObject>* pObjectList, CLinkedList<CBaseObject>* pUIObjectList) { return true; }

};


class _T_EXPORT CTreadMapAPI
{
	
private:

	CBaseTreadMapFile* GetFileReader(CString sTag, int nMagic, CTread3DDoc* pDocument);
	CBaseTreadMapFile* GetFileWriter(TREAD2XHEADERINFO* pHeader, CTread3DDoc* pDocument);

public:
	
	CTreadMapAPI();
	~CTreadMapAPI();

	bool WriteMap(CFile& hFile, TREAD2XHEADERINFO* pHeader, CTread3DDoc* pDocument, CLinkedList<CBaseObject>* pObjectList, CLinkedList<CBaseObject>* pSelectedObjectList, CLinkedList<CBaseObject>* pUIObjectList);
	bool ReadMap(CFile& hFile, TREAD2XHEADERINFO* pHeader, CTread3DDoc* pDocument, CLinkedList<CBaseObject>* pObjectList, CLinkedList<CBaseObject>* pUIObjectList);

};
extern CTreadMapAPI* (*GetTreadMapAPI) ();

//////////////////////////////////////////////////////////////////////
// EVENTMANAGER.H													//
//////////////////////////////////////////////////////////////////////
/**********************************************************************************
** Treads Event System has been heavily redone since version 1.x. The new system **
** operates on a per object basis. There are two distinctions, and two sets of   **
** events that are trapped and handled. The first set are mode change events.    **
** Mode change events must be handled to alter various viewing flags, and build  **
** hotspots based on the current mode. For example, when switching from pointer  **
** mode, to vertex mode, hotspots should be generated for the vertices of the    **
** selected objects, not the global selection area. This would be done by        **
** attaching a mode change responder to the vertex mode, and then doing the      **
** appropriate procedures there.                                                 **
**                                                                               **
** The other set of tracked events are window events. Things such as the mouse   **
** moving in a view, or clicking. The window event manager ties in with the      **
** hotspot event manager. Hotspots are setup via the mode event responders. The  **
** window event manager passes events to even responders based on the current    **
** mode. These events can be usurped by the hotspot manager, if the event is     **
** occuring over a hotspot.                                                      **
**                                                                               **
**********************************************************************************/

class CWindowEventManager;
class CMapView;
class CMapFrame;
class CTread3DDoc;

#define ALL_MODES	-1

#ifdef _WIN32
#define EF_LBUTTON		MK_LBUTTON
#define EF_RBUTTON		MK_RBUTTON
#define EF_SHIFT		MK_SHIFT
#define EF_CONTROL		MK_CONTROL
#define EF_ALT			MK_ALT
#else
#define EF_LBUTTON		0x00000001
#define EF_RBUTTON		0x00000002
#define EF_SHIFT		0x00000004
#define EF_CONTROL		0x00000008
#define EF_ALT			0x00000010
#endif

// Define events that can happen via menus (i.e. Carving)
#define EF_ACTION_CARVE			0x00000001
#define EF_ACTION_HOLLOW		0x00000002
#define EF_ACTION_GROUP			0x00000003
#define EF_ACTION_UNGROUP		0x00000004
#define EF_ACTION_FLIPVERT		0x00000005
#define EF_ACTION_FLIPHORZ		0x00000006
#define EF_ACTION_CLIP			0x00000007
#define EF_ACTION_DELETE		0x00000008
#define EF_ACTION_MAKETARGETS	0x00000009
#define EF_ACTION_MAKEPATHS		0x0000000A
#define EF_ACTION_MAKEPATHSL	0x0000000B
#define EF_ACTION_LINK			0x0000000C
#define EF_ACTION_UNLINK		0x0000000D
#define EF_ACTION_UNDO			0x0000000E
#define EF_ACTION_CUT			0x0000000F
#define EF_ACTION_COPY			0x00000010
#define EF_ACTION_PASTE			0x00000011
#define EF_ACTION_CENTER		0x00000012
#define EF_ACTION_ROT90CW		0x00000013
#define EF_ACTION_ROT90CCW		0x00000014
#define EF_ACTION_ROT180		0x00000015
#define EF_ACTION_REDO			0x00000016	// Hehe, last minute idea.
#define EF_ACTION_UNDOSTRING	0x00000017  // Get Undo String.
#define EF_ACTION_REDOSTRING	0x00000018	// Get Redo String.
#define EF_ACTION_PAINT			0x00000019
#define EF_ACTION_DELETEPATHS	0x0000001A
#define EF_ACTION_DROPANCHOR	0x0000001B
#define EF_ACTION_ALIGNLEFT		0x0000001C
#define EF_ACTION_ALIGNRIGHT	0x0000001D
#define EF_ACTION_ALIGNTOP		0x0000001E
#define EF_ACTION_ALIGNBOTTOM	0x0000001F
#define EF_ACTION_ALIGNHORZCEN	0x00000020
#define EF_ACTION_ALIGNVERTCEN	0x00000021
#define EF_ACTION_SPACEHORZ		0x00000022
#define EF_ACTION_SPACEVERT		0x00000023
#define EF_ACTION_CLOSEHORZ		0x00000024
#define EF_ACTION_CLOSEVERT		0x00000025
#define EF_ACTION_SIZEHORZ		0x00000026
#define EF_ACTION_SIZEVERT		0x00000027
#define EF_ACTION_SIZEBOTH		0x00000028

class _T_EXPORT CBaseWindowEventResponder : public CLLObject
{
public:

	CBaseWindowEventResponder() {}
	virtual ~CBaseWindowEventResponder() {}

	int m_nMode;
	int m_nSubmode;

	virtual void OnMouseMove(CBaseWindowEventResponder* pParent, CWindowEventManager* pEventManager, CMapView* pView, CTread3DDoc* pDocument, int nMouseX, int nMouseY, int nButtons) {}
	virtual void OnMouseDown(CBaseWindowEventResponder* pParent, CWindowEventManager* pEventManager, CMapView* pView, CTread3DDoc* pDocument, int nMouseX, int nMouseY, int nButtons) {}
	virtual void OnMouseUp(CBaseWindowEventResponder* pParent, CWindowEventManager* pEventManager, CMapView* pView, CTread3DDoc* pDocument, int nMouseX, int nMouseY, int nButtons) {}
	virtual void OnKeyDown(CBaseWindowEventResponder* pParent, CWindowEventManager* pEventManager, CMapView* pView, CTread3DDoc* pDocument, int nKeyCode) {}
};

// NOTE: The event manager will delete all the trapper objects
// that get added. There is no need to delete them yourself.
class _T_EXPORT CWindowEventManager
{
private:

	bool m_bAutoScroll;
	bool m_bTimerActive;
	int m_nMouseX, m_nMouseY, m_nButtons;
	CBaseWindowEventResponder* m_pAutoScrollResponder;
	CBaseWindowEventResponder* m_pGlobalResponder;
	CLinkedList<CBaseWindowEventResponder>* m_pResponderList;

public:

	CWindowEventManager();
	~CWindowEventManager();


	void EnableAutoScroll(CBaseWindowEventResponder* pResp, bool bAuto=true);

	void ClearResponderList() { m_pResponderList->DestroyList(); }
	void SetGlobalResponder(CBaseWindowEventResponder* pResponder);
	void AddEventResponder(CBaseWindowEventResponder* pResponder);

	void OnTimer(CMapView* pView, CTread3DDoc* pDocument, int nCode);
	void OnMouseMove(CMapView* pView, CTread3DDoc* pDocument, int nMouseX, int nMouseY, int nButtons);
	void OnMouseDown(CMapView* pView, CTread3DDoc* pDocument, int nMouseX, int nMouseY, int nButtons);
	void OnMouseUp(CMapView* pView, CTread3DDoc* pDocument, int nMouseX, int nMouseY, int nButtons);
	void OnKeyDown(CMapView* pView, CTread3DDoc* pDocument, int nKeyCode);
};

// Action responder flags.
#define ARF_DEFAULT			0x00000000
#define ARF_HANDLED			0x00000001
#define ARF_IGNORE			0x00000002
#define ARF_NOTHANDLED		0x00000003

// Responds to a specific mode change.
class CMode;
class CModeChangeEventManager;
class _T_EXPORT CBaseModeChangeResponder : public CLLObject
{
public:

	CBaseModeChangeResponder() {}
	virtual ~CBaseModeChangeResponder() {}

	int m_nMode;
	int m_nSubmode;

	virtual int OnAction(int nAction, void* pParam, CModeChangeEventManager* pEventManager, CMode* pModeManager, CTread3DDoc* pDocument) { return ARF_DEFAULT; }
	virtual void OnModeChange(CModeChangeEventManager* pEventManager, CMode* pModeManager, CTread3DDoc* pDocument, int nOldMode, int nOldSubmode, int nNewMode, int nNewSubmode) {}
};

// Responds to mode changes.
// All responders will be deleted with the manager.
class _T_EXPORT CModeChangeEventManager
{
private:
	CLinkedList<CBaseModeChangeResponder>* m_pResponderList;

public:

	CModeChangeEventManager();
	~CModeChangeEventManager();
 
	void InvokeAction(int nAction, void* pParam, CTread3DDoc* pDocument);
	void AddEventResponder(CBaseModeChangeResponder* pReponder);
	void OnModeChange(CMode* pModeManager, CTread3DDoc* pDocument, int nOldMode, int nOldSubmode, int nNewMode, int nNewSubmode);
};

//////////////////////////////////////////////////////////////////////
// DEFAULTACTIONRESPONDER.H											//

class _T_EXPORT CDefaultActionResponder
{
private:

	static float GetObjectSpace(CBaseTransformableObject* pTA, CBaseTransformableObject* pTB, int nAxis);
	static void  SpaceObjects(CBaseTransformableObject* pTA, CBaseTransformableObject* pTB, int nAxis, float fSpace);

public:

	static int OnAction(int nAction, void* pParam, CModeChangeEventManager* pEventManager, CMode* pModeManager, CTread3DDoc* pDocument);
};


//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// HOTSPOTMANAGER.H													//
//////////////////////////////////////////////////////////////////////
// Hotspot system.
#define HSDF_NODRAW			0x00000001
#define HSDF_ROUND			0x00000002
#define HSDF_SQUARE			0x00000004
#define HSDF_DIAMOND		0x00000008
#define HSDF_CROSS			0x00000010
#define HSDF_LINE			0x00000020
#define HSDF_SEL			0x00000040
#define HSDF_HD_COLOR		0x00000080

#define HSDR_VERT		0x00000001
#define HSDR_HORZ		0x00000002
#define HSDR_NONE		(HSDR_VERT|HSDR_HORZ)

class CBaseHotSpotEventResponder;
class _T_EXPORT CHotSpot : public CBaseObject
{
public:
	
	CHotSpot();
	virtual ~CHotSpot();

	virtual CBaseObject* Clone() { return new CHotSpot(*this); }

	void CopyToLXY();
	void ComputeMapXY(CMapView* pView);
	void ComputeWindowXY(CMapView* pView);

	int m_nClass;
	int m_nCode;
	int m_nCursor;
	int m_nDrawFlags;
	int m_nDragRestrictions;
	int m_nFlags;

	float m_fMX, m_fMY;	// Map X/Y.
	float m_fLX, m_fLY;
	int m_nWX, m_nWY;	// Window X/Y.

	int m_nDataFlags;
	void* m_pData;
	vec3_t m_vPt;
	CHotSpot* m_pLineSpot;
	CBaseObject* m_pObject;

	CBaseHotSpotEventResponder* m_pEventResponder;
};

class CHotSpotManager;
// Generates hotspots.
class _T_EXPORT CBaseHotSpotGenerator
{
public:

	CBaseHotSpotGenerator() {}
	virtual ~CBaseHotSpotGenerator() {}
	
	virtual void GenerateHotSpots(CHotSpotManager* pHotSpotManager, CTread3DDoc* pDocument, CMapView* pView) {}
};

class _T_EXPORT CBaseHotSpotModeGenerator : public CLLObject
{

public:
	
	CBaseHotSpotModeGenerator() {}
	virtual ~CBaseHotSpotModeGenerator() {}

	int m_nMode;
	int m_nSubmode;

	virtual void GenerateHotSpots(CHotSpotManager* pHotSpotManager, CTread3DDoc* pDocument, CMapView* pView) {}
};


#define TS_NONE		0x00000000
#define TS_TRACKING	0x00000001
#define TS_DRAGGING	0x00000002
#define TS_BBOX		0x00000003

#define ISF_ALL		0xFFFFFFFF
#define ISF_NONE	0x00000000
#define ISF_WINX	0x00000001
#define ISF_WINY	0x00000002
#define ISF_PTX		0x00000004
#define ISF_PTY		0x00000008
#define	ISF_PTZ		0x00000010
#define ISF_CLASS	0x00000020
#define ISF_MASK	0x00000040
#define ISF_MASK1	0x00000080
#define ISF_MASK2	0x00000100
#define ISF_MASK3	0x00000200
#define ISF_MASK4	0x00000400
#define ISF_MASKS	(ISF_MASK1|ISF_MASK2|ISF_MASK3|ISF_MASK4)
#define ISF_CODE	0x00000800

struct HSPOTREF
{
	HSPOTREF* prev, *next;
	CHotSpot* pSpot;
};

class CRenderAPI;
class _T_EXPORT CHotSpotManager : public CBaseWindowEventResponder
{
private:

	int m_nMouseX, m_nMouseY, m_nButtons;
	int m_nOldX, m_nOldY;
	int m_nTrackState;
	int m_nOldRenderFlags;
	bool m_bGetFromSave;
	bool m_bMulti;
	CHotSpot* m_pTrackSpot, *m_pIdentSpot;
	CLinkedList<CHotSpot>* m_pHotSpotList;
	CLinkedList<HSPOTREF>* m_pStoreList;
	CLinkedList<HSPOTREF>* m_pSelList;
	CLinkedList<CBaseHotSpotModeGenerator>* m_pGeneratorList;

	bool OverSpot(CHotSpot* pSpot, int nMouseX, int nMouseY);
	bool FindTrackSpot(CWindowEventManager* pManager, CMapView* pView, CTread3DDoc* pDocument, int nMouseX, int nMouseY, int nButtons);

public:

	friend class CRenderAPI;

	CHotSpotManager();
	virtual ~CHotSpotManager();

	void ClearGeneratorList() { m_pGeneratorList->DestroyList(); }
	void AddModeGenerator(CBaseHotSpotModeGenerator* pHG);

	void AddHotSpot(CHotSpot* pSpot);
	void DeleteHotSpots();

	void SetTrackSpot(CHotSpot* pTrackSpot);
	void SetDragSpot(CHotSpot* pDragSpot, int nMouseX, int nMouseY, int nButtons, CTread3DDoc* pDocument, CMapView* pView, CWindowEventManager* pEventManager);

	// Returns true if spot found.
	bool Track(CWindowEventManager* pEventManager, CMapView* pView, CTread3DDoc* pDocument, int nMouseX, int nMouseY, int nButtons);

	void BuildHotSpots(CTread3DDoc* pDocument, CMapView* pView);

	bool EnableBBoxSelection(bool bEnable = true);
	bool EnableMultipleSelection(bool bEnable = true);

	void ClearSelectedHotSpots();
	void AddHotSpotToSelection(CHotSpot* pSpot);
	void RemoveHotSpotFromSelection(CHotSpot* pSpot);

	void InitIdenticalSearch();
	void SaveIdenticalSearch(CHotSpot* pHotSpot, CMapView* pView, int nFlags);
	void RestoreIdenticalSearch();
	CHotSpot* GetNextIdenticalHotSpot(CHotSpot* pHotSpot, CMapView* pView, int nFlags);

	bool BeginBBoxSelection(CBaseWindowEventResponder* pParent, CWindowEventManager* pEventManager, CMapView* pView, CTread3DDoc* pDocument, int nMouseX, int nMouseY, int nButtons);

	virtual void OnMouseMove(CBaseWindowEventResponder* pParent, CWindowEventManager* pEventManager, CMapView* pView, CTread3DDoc* pDocument, int nMouseX, int nMouseY, int nButtons);
	virtual void OnMouseDown(CBaseWindowEventResponder* pParent, CWindowEventManager* pEventManager, CMapView* pView, CTread3DDoc* pDocument, int nMouseX, int nMouseY, int nButtons);
	virtual void OnMouseUp(CBaseWindowEventResponder* pParent, CWindowEventManager* pEventManager, CMapView* pView, CTread3DDoc* pDocument, int nMouseX, int nMouseY, int nButtons);
	virtual void OnKeyDown(CBaseWindowEventResponder* pParent, CWindowEventManager* pEventManager, CMapView* pView, CTread3DDoc* pDocument, int nKeyCode);
};


#define HEF_NONE	0x00000000
#define HEF_GROUP	0x00000001
#define HEF_HEAD	0x00000002
#define HEF_TAIL	0x00000004
#define HEF_MULTI	0x00000008

// A hotspot event trapper.
class _T_EXPORT CBaseHotSpotEventResponder
{
public:
	
	CBaseHotSpotEventResponder() {}
	virtual ~CBaseHotSpotEventResponder() {}

	virtual void OnHotSpotDrag(CWindowEventManager* pEventManager, CHotSpotManager* pHotSpotManager, CHotSpot* pHotSpot, CMapView* pView, CTread3DDoc* pDocument, int nMouseX, int nMouseY, int nButtons, int nFlags) {}
	virtual void OnHotSpotMouseUp(CWindowEventManager* pEventManager, CHotSpotManager* pHotSpotManager, CHotSpot* pHotSpot, CMapView* pView, CTread3DDoc* pDocument, int nMouseX, int nMouseY, int nButtons, int nFlags) {}
	virtual void OnHotSpotMouseDown(CWindowEventManager* pEventManager, CHotSpotManager* pHotSpotManager, CHotSpot* pHotSpot, CMapView* pView, CTread3DDoc* pDocument, int nMouseX, int nMouseY, int nButtons, int nFlags) {}
	virtual void OnHotSpotMouseEnter(CWindowEventManager* pEventManager, CHotSpotManager* pHotSpotManager, CHotSpot* pHotSpot, CMapView* pView, CTread3DDoc* pDocument, int nMouseX, int nMouseY, int nButtons, int nFlags) {}
	virtual void OnHotSpotMouseLeave(CWindowEventManager* pEventManager, CHotSpotManager* pHotSpotManager, CHotSpot* pHotSpot, CMapView* pView, CTread3DDoc* pDocument, int nMouseX, int nMouseY, int nButtons, int nFlags) {}
};


//////////////////////////////////////////////////////////////////////
// MODELS.H															//
//////////////////////////////////////////////////////////////////////
// Models are trd2 files.
struct MODEL
{
	MODEL* prev, *next;
	TREAD2XHEADERINFO m_Header;
	CLinkedList<CBaseObject>* m_pObjectList;
	vec3_t vMins, vMaxs, vOrigin;

	MODEL();
	~MODEL();
};

//////////////////////////////////////////////////////////////////////
// Tread Texturing System.											//
//////////////////////////////////////////////////////////////////////

// Defines a texture.
class CTexReader;
class CTexPalette;

// Defines a texture.
class _T_EXPORT CTexture
{

public:

	// Linked list links.
	CTexture* prev, *next;

	// Constructor.
	CTexture();
	
	// Destructor.
	virtual ~CTexture();

	CString m_sName;						// Texture Display Name.
	CString m_sRealName;					// The real texture name.
	CString m_sFilename;					// The filename.
	int m_nUserFlags;						// Pak defined flags.
	int m_nRenderFlags;						// Flags that get rendered onto the object.
	int m_nExtraFlags;						// More flags.
	int m_nUniqueID;						// Unique Identifier.
	int m_nBits;							// 8, 24, 32 == Alpha bit color.
	int m_nHandle;							// Handle (for Reader).
	int m_nWidth, m_nHeight;				// Width/Height.
	int m_nSclWidth, m_nSclHeight;			// Scaled height (for non-pwr of 2).
	int m_nContents, m_nSurface, m_nValue;	// Q2 Surface/Contents.
	int m_nSize;							// Size of data block.
	unsigned char* m_pData;					// Image data.
	vec3_t m_vColor;						// For line/solid mode.
	bool m_bInfo;							// true if info is loaded.
	bool m_bApplyGamma;						// true if we need to apply gamma.
	CTexReader* m_pReader;					// The Reader.
	CTexPalette* m_pPalette;				// Palette to display with.

	virtual bool Load();

	// Loads only info.
	virtual bool LoadInfo();

	// Free's any memory.
	virtual bool Free();

	// Free's the texture data.
	virtual bool FreeData();

	// Converts a texture to 24bit color.
	virtual bool To24BitColor();
	virtual bool ApplyGamma();

};

class _T_EXPORT CTexPalette
{
private:

	unsigned char* m_pPalData;

public:

	CTexPalette(unsigned char* pPalData)
	{
		m_pPalData = pPalData;
	}

	~CTexPalette()
	{
		if(m_pPalData)
			delete[] m_pPalData;
		m_pPalData = NULL;
	}

	unsigned char* GetPalData() { return m_pPalData; }
};

//////////////////////////////////////////////////////////////////
// Description: Base Class For Pak File loading/reading.		//
//////////////////////////////////////////////////////////////////
class _T_EXPORT CPakFile
{

public:

	// Constructor.
	CPakFile() {}
	// Destructor.
	virtual ~CPakFile() {}

	// Get the filename.
	virtual const char* GetFilename() = 0;

	// Derived class types should return TRUE on the file
	// type codes.
	/*static*/ virtual bool ClaimFile(const char* szFilename) { return false; }

	// Attaches a file to this object & initializes the reader.
	virtual bool OpenAndInitialize(const char* szFileName) = 0;

	// Get's the list of texture's in the pak.
	virtual CTexture** GetTextureList(int& nNumTextures) const  = 0;

	// Seeks to the file's position.
	virtual bool SeekToFile(CTexture* pTex) = 0;

	// Get's a files offset.
	virtual int GetFileOffset(CTexture* pTex) { return 0; }

	// Get's the file object.
	virtual void* GetFileObject(CTexture* pTex, int nContext=0) = 0;
	virtual void ReleaseFileObject(void* pFile, int nContext=0) {}

	// Gets a packages palette.
	virtual CTexPalette* GetPalette() { return NULL; }

	// Closes the pak file.
	virtual bool Close() = 0;

	virtual bool BuildPak() { return true; }
	virtual bool PakBuildsComplete() { return true; }

	// Free's the list of textures.
	// NOTE: Do NOT override.
	void FreeTextureList(CTexture** pTexList) const
	{
		if(pTexList != NULL)
			delete[] pTexList;
	}
};


class _T_EXPORT CTexReader
{
private:

	CPakFile* m_pPakFile;

public:

	CTexReader() { m_pPakFile = NULL; }
	virtual ~CTexReader() {}

	void SetPakFile(CPakFile* pPakFile) { m_pPakFile = pPakFile; }
	CPakFile* GetPakFile() { return m_pPakFile; }
	void* GetFileObject(CTexture* pTex, int nContext=0)
	{
		if(m_pPakFile == NULL)
			return NULL;

		return m_pPakFile->GetFileObject(pTex, nContext);
	}
	void ReleaseFileObject(void* pFile, int nContext=0)
	{
		if(m_pPakFile == NULL)
			return;

		m_pPakFile->ReleaseFileObject(pFile, nContext);
	}
	bool SeekToFile(CTexture* pTex)
	{
		if(m_pPakFile == NULL)
			return false;

		return m_pPakFile->SeekToFile(pTex);
	}
	int GetFileOffset(CTexture* pTex)
	{
		if(m_pPakFile == NULL)
			return 0;

		return m_pPakFile->GetFileOffset(pTex);
	}

	// Returns the RF_ flags based on surface flags.
	virtual int TranslateSurfaceToRenderFlags(int nFlags) { return 0; }

	// Loads the texture.
	// After the call pTex object should be a 24 bit color image.
	virtual bool LoadTexture(CTexture* pTex) = 0;

	// Get's the texture information.
	virtual bool LoadTextureInfo(CTexture* pTex) = 0;

	// Called to free a texture.
	virtual bool FreeTexture(CTexture* pTex);
	virtual bool FreeTextureData(CTexture* pTex);

};

// Parses a texture directory.
class _T_EXPORT CTexDirectory : public CPakFile
{
public:

	CTexDirectory() : CPakFile() {}
	virtual ~CTexDirectory() {}

	// Get the filename.
	const char* GetFilename() { return "Texture Directory"; }

	// Attaches a file to this object & initializes the reader.
	virtual bool OpenAndInitialize(const char* szFileName) = 0;

	// Get's the list of texture's in the pak.
	virtual CTexture** GetTextureList(int& nNumTextures) const = 0;

	// Get's the file object.
	virtual void* GetFileObject(CTexture* pTex, int nContext) = 0;
	virtual void ReleaseFileObject(void* pFile, int nContext) = 0;

	// Closes the pak file.
	virtual bool Close() = 0;

	virtual bool BuildPak() { return true; }
	
	// Seeks to the file's position.
	bool SeekToFile(CTexture* pTex) { return true; }

	// Get the file's position
	int GetFileOffset(CTexture* pTex) { return 0; }
	
	// Gets a packages palette.
	virtual CTexPalette* GetPalette() const { return NULL; }
};

// Loads a pak file.
#define MAX_OPEN_PAKS	64

class _T_EXPORT CPakLoader
{
private:

	CPakFile* m_pPakFiles[MAX_OPEN_PAKS];
	CPakFile* OpenAndAssignPak(const char* szFilename, bool bDir = false);
	int FindAvailablePak();
	CPakFile* GetPakLoader(const char* szFilename);
	CPakFile* GetDirLoader(const char* szFilename);

public:

	CPakLoader();
	virtual ~CPakLoader();

	// Get's a pakfile index.
	int GetPakIndex(CPakFile* pPak);
	CPakFile* GetPakFromIndex(int nPakIndex);

	// Opens a pak file.
	CPakFile* OpenPak(const char* szFilename, bool bDir = false);
	bool ClosePak(CPakFile* pPakFile);
	bool CloseAllPaks();
};


/*
12/15/99 - Joe Riedel
	- Added directory loading
*/

// Provides interface to pak classes and routines.
#define BAD_PAK_HANDLE		-1

#define MAX_TEXTURE_MEM		((1024*1000)*96)	// 96 Megs.

class _T_EXPORT CTexAPI
{
private:

	char m_szFilter[256];
	int m_nNextID;
	int m_nNumTexs, m_nFilterNumTexs;
	CTexture* m_pCurTex;
	CTexture** m_ppTexList;
	CTexture** m_ppFilterTexList;
	CPakLoader m_PakLoader;
	bool m_bPaksToList[MAX_OPEN_PAKS];

	int m_nUsedMem;
	CLinkedList<CTexture> m_LoadedTextureList;

	void FreePakTexList();
	
public:

	CTexAPI()
	{ 
		m_pCurTex = NULL;
		m_nNextID = 1;
		m_nNumTexs = m_nFilterNumTexs = 0;
		m_ppTexList = m_ppFilterTexList = NULL; 
		m_szFilter[0] = NULL;
		memset(m_bPaksToList, 0, sizeof(m_bPaksToList));
		m_LoadedTextureList.SetAutoDelete(false);
	}

	virtual ~CTexAPI()
	{
		FreePakTexList();
		if(m_ppTexList != NULL)
			delete m_ppTexList;
		m_PakLoader.CloseAllPaks();
	}

	// Openstex pak.
	int OpenPak(const char* szFilename, bool bDir = false);
	bool BuildOpenPaks();
	bool ClosePak(int nPakHandle);
	bool CloseAllPaks(void);
	const char* GetPakFilename(int nPakHandle);

	void AssignID(CTexture* pTexture);

	// Get's a paks texture list.
	bool AddPakToList(int nPakHandle);
	bool RemovePakToList(int nPakHandle);
	void SetNameFilter(const char* szFilter);
	bool BuildFilterTexList();
	CTexture* GetTextureFromName(CString sName);
	CTexture** GetFilterTexList(int& nNumTextures);
	CTexture** GetFullPakTexList(int& nNumTextures);
	CPakLoader* GetPakLoader() { return &m_PakLoader; }

	// Set's/Get's current texture.
	void SetCurTexture(CTexture* pTexture);
	CTexture* GetCurTexture();
	CString GetCurTextureName();
	
	// Called after the game has loaded.
	void PostGameLoad();

	// Called when a texture requests to load itself.
	void PreTextureLoad(CTexture* pTexture);
	// Called when the texture sucessfully loaded.
	void PostTextureLoad(CTexture* pTexture);
	// Free's all loaded textures.
	void FreeAllLoadedTextures();

};


// Get's the texAPI.
extern CTexAPI* (*GetTexAPI)();

//////////////////////////////////////////////////////////////////////
// WINDING.H														//
//////////////////////////////////////////////////////////////////////

#define WINDING_STORABLE_OBJECT_ID	0xE0000000
#define MAX_WINDING_POINTS			256

#define PSIDE_FRONT	1
#define PSIDE_BACK	2
#define PSIDE_ON	4
#define PSIDE_BOTH	8

class _T_EXPORT CWinding :    public CBaseGroupableObject, 
					public CBaseTransformableObject,
					public CBaseHotSpotEventResponder,
					public CBaseHotSpotGenerator

{

public:

	friend class CGDIWindingRenderer;

protected:

	CPlane m_Plane;
	int		m_nPlaneType;
	int		m_nNumPoints;
	vec3_t*	m_vPoints;
	float m_fX, m_fY;
	
	static vec_t m_sfDX;
	static vec_t m_sfDY;

	void ComputePlane();
	void FreePoints();

public:

	CWinding();
	CWinding(int nNumPoints);
	CWinding(int nNumPoints, vec3_t* vPoints);
	CWinding(const CWinding& WindingToCopy);
	virtual ~CWinding();

	// CWinding functions.
	CPlane* GetPlanePtr() { return &m_Plane; }
	// Makes a copy of the points array passed.
	virtual void CopyPoints(int nNumPoints, vec3_t* vPoints);
	virtual void SplitWindingByPlane(CPlane* pPlane, CWinding** pFront, CWinding** pBack);
	virtual CWinding* ChopWindingByPlane(CPlane* pPlane, bool bReturnFront = true);
	virtual void Flip();

	int GetNumPoints() const { return m_nNumPoints; }
	vec3_t* GetPoints() const { return m_vPoints; }

	bool WindingCrossesPlane(CPlane* pPlane);
	int  ClassifyWinding(CPlane* pPlane);
	int  ClassifyWinding(CPlane* pPlane, float fEpsilon);
	int  BiasClassifyWinding(CPlane* pPlane, float fFront);
	bool CheckWinding();
	static CWinding* BaseWindingFromPlane(CPlane* pPlane);

	virtual CBaseObject* Clone() { return new CWinding(*this); }

	// CBaseObject
	virtual int GetObjectClass() { return OBJECT_CLASS_WINDING; }
	virtual int GetObjectInterfaces() { return I_GROUPABLE | I_STORABLE | I_TRANSFORMABLE | I_CHECKABLE; }
	virtual void* GetInterface(int nInterface);

	// CBaseStorableObject.
	virtual bool ReadObject(CFile& hFile, int nVersion);
	virtual bool WriteObject(CFile& hFile, int nVersion);

	// CBaseTransformableObject
	virtual void Scale(vec3_t vScale);
	virtual void Scale(vec3_t vScale, vec3_t vOrigin);
	virtual void SetOrigin(vec3_t vOrigin);
	virtual void Translate(vec3_t vTranslation);
	virtual void Rotate(vec3_t vRot);
	virtual void Rotate(vec3_t vOrigin, vec3_t vRot);
	virtual void InsertIntoRange(vec3_t vMins, vec3_t vMaxs);
	virtual void ComputeOrigin();
	virtual void Reflect(int nAxis, vec3_t vOrigin);
	virtual bool In2DRange(vec3_t vMins, vec3_t vMaxs, int nView);

	// Hotspot.
	virtual void GenerateHotSpots(CHotSpotManager* pHotSpotManager, CMapView* pView);
	virtual void OnHotSpotDrag(CWindowEventManager* pEventManager, CHotSpotManager* pHotSpotManager, CHotSpot* pHotSpot, CMapView* pView, CTread3DDoc* pDocument, int nMouseX, int nMouseY, int nButtons, int nFlags);
	virtual void OnHotSpotMouseDown(CWindowEventManager* pEventManager, CHotSpotManager* pHotSpotManager, CHotSpot* pHotSpot, CMapView* pView, CTread3DDoc* pDocument, int nMouseX, int nMouseY, int nButtons, int nFlags);
	virtual void OnHotSpotMouseUp(CWindowEventManager* pEventManager, CHotSpotManager* pHotSpotManager, CHotSpot* pHotSpot, CMapView* pView, CTread3DDoc* pDocument, int nMouseX, int nMouseY, int nButtons, int nFlags);
};


//////////////////////////////////////////////////////////////////////
// FACE.H															//
//////////////////////////////////////////////////////////////////////
class _T_EXPORT CFace :   public CWinding, 
				public CBaseTextureableObject,
				public CVisgroupRefObject
{
public:

	friend class CGL3DFaceRenderer;
	friend class CGLFaceRenderer;
	friend class CPreviewDialog;
	friend class CGL3DBrushRenderer;
	friend class CQBrush;
	friend class CTreadMap1X;

protected:

	bool m_bTexInit;
	int m_nNumTexCoords;
	vec_t** m_vTexCoords;

	void GenerateTextureCoords();
	void FreeTexCoords();
	void CopyTexCoords(int nNumTexCoords, vec_t** vTexCoords);
	bool AllocTexCoords(int nNumTexCoords);

	// CWinding.
	virtual void SplitWindingByPlane(CPlane* pPlane, CWinding** pFront, CWinding** pBack)
	{ CWinding::SplitWindingByPlane(pPlane, pFront, pBack); }
	
	virtual CWinding* ChopWindingByPlane(CPlane* pPlane, bool bReturnFront = true)
	{ return CWinding::ChopWindingByPlane(pPlane, bReturnFront); }
	
public:

	CFace();
	CFace(int nNumPoints);
	CFace(int nNumPoints, vec3_t* vPoints);
	CFace(const CFace& FaceToCopy);
	CFace(const CWinding& WindingToCopy);
	virtual ~CFace();

	CBaseObject* m_pParent;

	static void TextureAxisFromPlane(CPlane *pPlane, vec3_t vXV, vec3_t vYV);
	static void MakeTVecs(vec3_t vUV[2], vec3_t vOut[2], vec_t vScales[2], vec_t fRot);
	static void ProjectTexture(vec3_t vUV[2], vec_t vShifts[2], int nNumCoords, vec3_t* vCoords, vec_t** vTexCoords);


	virtual vec_t** GetTextureCoords() { return m_vTexCoords; }
	virtual void CopyWinding(const CWinding& WindingToCopy);
	virtual void CopyPoints(int nNumPoints, vec3_t* vPoints);
	virtual void CopyPoints(int nNumPoints, vec3_t* vPoints, vec_t** vTexs);

	// CWinding
	virtual void Flip();
	virtual void SplitFaceByPlane(CPlane* pPlane, CFace** pFront, CFace** pBack);
	virtual CFace* ChopFaceByPlane(CPlane* pPlane, bool bReturnFront = true);
	static CFace* BaseFaceFromPlane(CPlane* pPlane);

	// CBaseObject.
	virtual int GetObjectClass() { return OBJECT_CLASS_FACE; }
	virtual int GetObjectInterfaces() { return CWinding::GetObjectInterfaces() | I_TEXTUREABLE | I_VISGROUPREF; }
	virtual void* GetInterface(int nInterface);

	// CBaseStorableObject.
	virtual bool ReadObject(CFile& hFile, int nVersion);
	virtual bool WriteObject(CFile& hFile, int nVersion);

	virtual CBaseObject* Clone();

	// CBaseTexturableObject
	// Return True if object supports texturing manipulations.
	virtual bool SupportsTextureManipulations() { return true; }

	virtual void Scale(vec3_t vScale);
	virtual void Scale(vec3_t vScale, vec3_t vOrigin);
	virtual void SetOrigin(vec3_t vOrigin);
	virtual void Translate(vec3_t vTranslation);
	virtual void Rotate(vec3_t vRot);
	virtual void Rotate(vec3_t vOrigin, vec3_t vRot);
	virtual void Reflect(int nAxis, vec3_t vOrigin);

	// Events.
	virtual void OnTextureChange();
	virtual void OnAttributeChange();
	virtual void RebuildTexture();
	
	virtual void OnHotSpotMouseUp(CWindowEventManager* pEventManager, CHotSpotManager* pHotSpotManager, CHotSpot* pHotSpot, CMapView* pView, CTread3DDoc* pDocument, int nMouseX, int nMouseY, int nButtons, int nFlags);
};

//////////////////////////////////////////////////////////////////////
// BRUSH.H															//
//////////////////////////////////////////////////////////////////////
class _T_EXPORT CQBrushPlane : public CLLObject, public CVisgroupRefObject
{
public:

	CQBrushPlane()
	{
		m_bUseFlags = false;
	}

	CQBrushPlane(const CPlane& PlaneToCopy)
	{
		m_Plane.m_vDist = PlaneToCopy.m_vDist;
		VectorCopy(PlaneToCopy.m_vNormal, m_Plane.m_vNormal);
		m_vShifts[0] = 0.0F; m_vShifts[1] = 0.0F;
		m_vScales[0] = 0.0F; m_vScales[1] = 0.0F;
		m_fRotate = 0.0F;
		m_bUseFlags = false;
	}

	CPlane m_Plane;
	CString m_sTexture;
	vec2_t m_vShifts;
	vec2_t m_vScales;
	vec_t m_fRotate;

	bool m_bUseFlags;
	int m_nSurface, m_nContents, m_nValue;

};


// A Quake Style Brush.
class _T_EXPORT CQBrush : public CBaseGroupableObject, 
				public CBaseTransformableObject, 
				public CBaseTextureableObject,
				public CBaseCheckableObject,
				public CVisgroupRefObject,
				public CBaseHotSpotGenerator,
				public CBaseCarveableObject
{

public:

	friend class CGDIBrushRenderer;
	friend class CGL3DBrushRenderer;

public:

	CQBrush();
	CQBrush(const CQBrush& BrushToCopy);
	virtual ~CQBrush();
	
	CLinkedList<CFace>* m_pFaceList;

	// Make hotspots.
	virtual void GenerateHotSpots(CHotSpotManager* pHotSpotManager, CMapView* pView, CBaseHotSpotEventResponder* pResp=NULL, bool bHandles=false, bool bSel=false);

	// Brush functions.
	void SplitBrushByPlane(CPlane* pPlane, CQBrush** pFront, CQBrush** pBack);
	CObjectGroup* HollowBrush(float fDepth);
	int ClassifyBrush(CPlane* pPlane);
	bool CheckBrush();

	static CQBrush* CreateBrushFromPlaneList(CLinkedList<CQBrushPlane>& PlaneList);

	virtual int InitAfterLoad(CTread3DDoc* pDocument);

	// CBaseObject
	virtual int GetObjectClass() { return OBJECT_CLASS_BRUSH; }
	virtual int GetObjectInterfaces() { return I_GROUPABLE | I_STORABLE | I_TRANSFORMABLE 
												| I_TEXTUREABLE | I_CHECKABLE | I_VISGROUPREF | I_CARVE | I_HOLLOW | I_CARVEABLE | I_LINK; }
	virtual void* GetInterface(int nInterface);

	virtual CBaseObject* Clone() { return new CQBrush(*this); }

	// CBaseStorableObject
	virtual bool ReadObject(CFile& hFile, int nVersion);
	virtual bool WriteObject(CFile& hFile, int nVersion);

	// CBaseTransformableObject
	virtual void Scale(vec3_t vScale);	// Scales the objects size.
	virtual void Scale(vec3_t vScale, vec3_t vOrigin);
	virtual void Translate(vec3_t vTranslation);
	virtual void Rotate(vec3_t vRot);
	virtual void Rotate(vec3_t vOrigin, vec3_t vRot);
	virtual void InsertIntoRange(vec3_t vMins, vec3_t vMaxs);
	virtual void Reflect(int nAxis, vec3_t vFlipOrigin);
	virtual void ComputeOrigin();
	virtual void BuildRange();
	virtual bool In2DRange(vec3_t vMins, vec3_t vMaxs, int nView);

	// Carvable.
	virtual bool ObjectsIntersect(CBaseObject* pObject);
	virtual CObjectGroup* CSGSubtract(CBaseObject* pObject);
	virtual int GetCarveClasses() const { return OBJECT_CLASS_BRUSH|OBJECT_CLASS_GROUP; }

	// CBaseTextureableObject
	// Events.
	virtual void OnTextureChange();
	virtual void OnAttributeChange();
	virtual void RebuildTexture();
};

//////////////////////////////////////////////////////////////////////
// ENTITY.H															//
//////////////////////////////////////////////////////////////////////

#define ENTITY_STORABLE_OBJECT_ID	0xF0000000

struct ENTITYKEY;
struct _T_EXPORT ENTITYKEY
{
	ENTITYKEY* prev, *next;
	
	ENTITYKEY()
	{
		pKeyDef = NULL;
		sName = sValue = "";
	}

	ENTITYKEY(struct ENTITYKEYDEFINITION* pKeyDefToCopy);

	ENTITYKEY(const ENTITYKEY& KeyToCopy)
	{
		sName = KeyToCopy.sName;
		sValue = KeyToCopy.sValue;
		pKeyDef = KeyToCopy.pKeyDef;
		prev = next = NULL;
	}

	
	struct ENTITYKEYDEFINITION* pKeyDef;	// Definition of this key.
	CString sName, sValue;
};

// Render classes.
class CGDIEntityRenderer;
// End Render Classes.

// Defines an entity.
class _T_EXPORT CEntity : public CObjectGroup, public CBaseTargetObject
{

	friend class CGDIEntityRenderer;

protected:

	CString m_sClassName;
	struct ENTITYDEFINITION* m_pEntDef;
	CLinkedList<struct ENTITYKEY>* m_pKeyList;
	bool WriteKey(ENTITYKEY* pKey, CFile& hFile, int nVersion);
	ENTITYKEY* ReadKey(CFile& hFile, int nVersion);
	int m_nMiscFlags[4];

public:

	CEntity();
	CEntity(const CEntity& EntToCopy);
	virtual ~CEntity();

	virtual bool InitFromDefinition(ENTITYDEFINITION* pEntDef);

	CString GetClassName() const { return m_sClassName; }

	bool DeleteKey(CString sName);
	ENTITYKEY* GetKey(CString sName);
	CString GetKeyValue(CString sName);
	CLinkedList<ENTITYKEY>* GetKeyList() const { return m_pKeyList; }
	ENTITYDEFINITION* GetEntityDefinition() { return m_pEntDef; }
	virtual bool SetClassName(CString sClass);

	virtual bool IsOwner();

	virtual void PreDelete(CTread3DDoc* pDocument);

	virtual CBaseObject* Clone() { return new CEntity(*this); }

	// Changes one entity it an duplicate of the parameter,
	// and maintains original brushes.
	virtual void MorphEntity(CEntity* pEntity);

	// From CBaseGroupableObject
	virtual int GetObjectClass();
	virtual int GetObjectInterfaces();
	virtual void* GetInterface(int nInterface);

	virtual bool AddObject(CBaseGroupableObject* pObject);
	virtual bool RemoveObject(CBaseGroupableObject* pObject);

	// From CBaseTargetObject
	virtual CString GetTargetableName();
	virtual void SetTargetableName(CString sName, CTread3DDoc* pDocument);
	virtual CString GetTargetName();
	virtual void SetTargetName(CString sName, CTread3DDoc* pDocument);

	// From CBaseTransformableObject
	virtual void SetOrigin(vec3_t vOrigin) { VectorCopy(vOrigin, m_vOrigin); }
	virtual void Translate(vec3_t vTranslation);
	virtual void Rotate(vec3_t vOrigin, vec3_t vRot);
	virtual void InsertIntoRange(vec3_t vMins, vec3_t vMaxs);
	virtual void Scale(vec3_t vScale, vec3_t vOrigin);
	virtual void Reflect(int nAxis, vec3_t vFlipOrigin);
	virtual void BuildRange();
	virtual bool In2DRange(vec3_t vMins, vec3_t vMaxs, int nView);

	// From CBaseStorableObject.
	virtual bool ReadObject(CFile& hFile, int nVersion);
	virtual bool WriteObject(CFile& hFile, int nVersion);
	virtual int InitAfterLoad(CTread3DDoc* pDocument);
	virtual int PostInitAfterLoad(CTread3DDoc* pDocument, int nCode);
};


//////////////////////////////////////////////////////////////////////
// CAMERA.H															//
//////////////////////////////////////////////////////////////////////


#define CF_ORIGIN	0x00000001
#define CF_LOOK		0x00000002

class CMapView;
class _T_EXPORT CCamera : public CBaseObject, public CBaseHotSpotGenerator, public CBaseHotSpotEventResponder
{
private:

	int m_nView;
	CMapView* m_pView;
	vec3_t m_vOrigin, m_vLook;
	CCamera* m_pOldCam;

public:

	CCamera();

	vec_t* GetOrigin() { return m_vOrigin; }
	vec_t* GetLook() { return m_vLook; }
	CMapView* GetView() { return m_pView; }
	void SetView(CMapView* pView, CTread3DDoc* pDocument);

	// Make hotspots.
	virtual void GenerateHotSpots(CHotSpotManager* pHotSpotManager, CMapView* pView);
	virtual void OnHotSpotDrag(CWindowEventManager* pEventManager, CHotSpotManager* pHotSpotManager, CHotSpot* pHotSpot, CMapView* pView, CTread3DDoc* pDocument, int nMouseX, int nMouseY, int nButtons, int nFlags);
	virtual void OnHotSpotMouseUp(CWindowEventManager* pEventManager, CHotSpotManager* pHotSpotManager, CHotSpot* pHotSpot, CMapView* pView, CTread3DDoc* pDocument, int nMouseX, int nMouseY, int nButtons, int nFlags);
	virtual void OnHotSpotMouseDown(CWindowEventManager* pEventManager, CHotSpotManager* pHotSpotManager, CHotSpot* pHotSpot, CMapView* pView, CTread3DDoc* pDocument, int nMouseX, int nMouseY, int nButtons, int nFlags);
	virtual void OnHotSpotMouseEnter(CWindowEventManager* pEventManager, CHotSpotManager* pHotSpotManager, CHotSpot* pHotSpot, CMapView* pView, CTread3DDoc* pDocument, int nMouseX, int nMouseY, int nButtons, int nFlags);
	virtual void OnHotSpotMouseLeave(CWindowEventManager* pEventManager, CHotSpotManager* pHotSpotManager, CHotSpot* pHotSpot, CMapView* pView, CTread3DDoc* pDocument, int nMouseX, int nMouseY, int nButtons, int nFlags);

	virtual bool ReadObject(CFile& hFile, int nVersion);
	virtual bool WriteObject(CFile& hFile, int nVersion);

	// Get's the object class.
	virtual int GetObjectClass() { return OBJECT_CLASS_CAMERA; }

	virtual CBaseObject* Clone() { return new CCamera(*this); }

	// If an higher object wants to be read/written it should
	// extend this to return the I_STORABLE interface.
	virtual int GetObjectInterfaces() { return I_BASE|I_UI|I_STORABLE; }

	virtual int InitAfterLoad(CTread3DDoc* pDocument);

	friend class CGDICameraRenderer;
	friend class CGLCameraRenderer;
	friend class CTreadMap1X;
	friend class CCameraModeWinResponder;
};


//////////////////////////////////////////////////////////////////////
// CCLIPTOOL.H														//
//////////////////////////////////////////////////////////////////////

class _T_EXPORT CClipTool : public CBaseObject, public CBaseHotSpotGenerator, public CBaseHotSpotEventResponder
{
	friend class CClipToolModeWinResponder;
	friend class CGDIClipToolRenderer;
	friend class CGLClipToolRenderer;

private:

	bool m_bValid;
	vec3_t m_vPts[2];
	float m_fX, m_fY;
	CPlane m_Plane;
	int m_nViewType;
	CHotSpot* m_pDragSpot;
	CLinkedList<CBaseObject> m_SrcList;
	CLinkedList<CBaseObject> m_FrontList;
	CLinkedList<CBaseObject> m_BackList;

public:

	CClipTool();

	int GetObjectClass() { return OBJECT_CLASS_CLIPTOOL; }
	int GetObjectInterfaces() { return I_UI; }

	virtual CBaseObject* Clone() { return new CClipTool(*this); }

	// Notifies us that the selection changed.
	void SelectionChanged(CTread3DDoc* pDocument);
	void DoClip(CTread3DDoc* pDocument, bool bSplit=false);

	CHotSpot* GetDragSpot() { return m_pDragSpot; }

	// Make hotspots.
	void GenerateHotSpots(CHotSpotManager* pHotSpotManager, CMapView* pView);
	void OnHotSpotDrag(CWindowEventManager* pEventManager, CHotSpotManager* pHotSpotManager, CHotSpot* pHotSpot, CMapView* pView, CTread3DDoc* pDocument, int nMouseX, int nMouseY, int nButtons, int nFlags);
	void OnHotSpotMouseUp(CWindowEventManager* pEventManager, CHotSpotManager* pHotSpotManager, CHotSpot* pHotSpot, CMapView* pView, CTread3DDoc* pDocument, int nMouseX, int nMouseY, int nButtons, int nFlags);
	void OnHotSpotMouseDown(CWindowEventManager* pEventManager, CHotSpotManager* pHotSpotManager, CHotSpot* pHotSpot, CMapView* pView, CTread3DDoc* pDocument, int nMouseX, int nMouseY, int nButtons, int nFlags);
};


//////////////////////////////////////////////////////////////////////
// GAMEAPI.H														//
//////////////////////////////////////////////////////////////////////
// The Game Definition System.										//
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
// Defines the entity scripting classes and definitions structures. //
//////////////////////////////////////////////////////////////////////

// Define possible key types.
#define EKD_TYPE_STRING			1
#define EKD_TYPE_INTEGER		2
#define EKD_TYPE_ANGLE			3
#define EKD_TYPE_Q2COLOR		4
#define EKD_TYPE_HLCOLOR		5
#define EKD_TYPE_FLAGS			6
#define EKD_TYPE_VECTOR			7

// NOTE: Because of the way the l-list and structures are set up, the auto-deconstructors
// automatically free all the memory.

// Defines an entity flag.
struct _T_EXPORT ENTITYFLAGOPTIONDEFINITION;
struct _T_EXPORT ENTITYFLAGOPTIONDEFINITION
{
	ENTITYFLAGOPTIONDEFINITION* prev, *next;

	ENTITYFLAGOPTIONDEFINITION()
	{}

	ENTITYFLAGOPTIONDEFINITION(const ENTITYFLAGOPTIONDEFINITION& FlagToCopy)
	{
		sString = FlagToCopy.sString;
		nValue = FlagToCopy.nValue;
		prev = next = NULL;
	}

	CString sString;
	int nValue;
	bool sUser;
	bool bType;
};

#define EFD_TYPE_FLAGS		true
#define EFD_TYPE_OPTIONS	false

struct _T_EXPORT ENTITYFLAGDEFINITION;
struct _T_EXPORT ENTITYFLAGDEFINITION
{
	ENTITYFLAGDEFINITION* prev, *next;

	ENTITYFLAGDEFINITION()
	{
		pFlagList = new CLinkedList<ENTITYFLAGOPTIONDEFINITION>();
		prev = next = NULL;
	}

	ENTITYFLAGDEFINITION(const ENTITYFLAGDEFINITION& FlagToCopy)
	{
		nValue = FlagToCopy.nValue;
		bType = FlagToCopy.bType;
		if(FlagToCopy.pFlagList)
			pFlagList = new CLinkedList<ENTITYFLAGOPTIONDEFINITION>(*FlagToCopy.pFlagList);
		else
			pFlagList = new CLinkedList<ENTITYFLAGOPTIONDEFINITION>();
		prev = next = NULL;
	}

	~ENTITYFLAGDEFINITION()
	{
		if(pFlagList)
			delete pFlagList;
		pFlagList = NULL;
	}

	int nValue;	// Default value of flag.
	bool bType;	// TRUE == Flags, FALSE == Options.
	CLinkedList<ENTITYFLAGOPTIONDEFINITION>* pFlagList;
};

struct _T_EXPORT ENTITYKEYDEFINITION;
struct _T_EXPORT ENTITYKEYDEFINITION
{
	ENTITYKEYDEFINITION* prev, *next;
	
	ENTITYKEYDEFINITION()
	{
		pFlagDef=NULL;
		prev = next = NULL;
		nType;
		sName = sSmartName = sValue = "";
	}

	ENTITYKEYDEFINITION(const ENTITYKEYDEFINITION& KeyDef)
	{
		if(KeyDef.pFlagDef != NULL)
			pFlagDef = new ENTITYFLAGDEFINITION(*KeyDef.pFlagDef);
		else
			pFlagDef = NULL;
		nType = KeyDef.nType;
		sName = KeyDef.sName;
		sSmartName = KeyDef.sSmartName;
		sValue = KeyDef.sValue;
		prev = next = NULL;
	}

	~ENTITYKEYDEFINITION()
	{
		if(pFlagDef)
			delete pFlagDef;
		pFlagDef = NULL;
	}

	ENTITYFLAGDEFINITION* pFlagDef;	// Flag definition. NULL == none.
	int nType;
	CString sName, sSmartName, sValue;
};

// Entity definition.
struct _T_EXPORT ENTITYDEFINITION;
struct _T_EXPORT EDBASECLASSLIST;
struct _T_EXPORT EDBASECLASSLIST
{
	EDBASECLASSLIST* prev, *next;
	ENTITYDEFINITION* pEntDef;
};

struct _T_EXPORT ENTITYDEFINITION
{
	ENTITYDEFINITION* prev, *next;
	
	ENTITYDEFINITION()
	{
		prev = next = NULL;
		sName = sSmartName = sCategory = "";
		pKeyList = new CLinkedList<ENTITYKEYDEFINITION>();
		memset(nMiscFlags, -1, sizeof(nMiscFlags));
	}

	ENTITYDEFINITION(const ENTITYDEFINITION& EntityToCopy)
	{
		bVirtual = EntityToCopy.bVirtual;
		bOwner = EntityToCopy.bOwner;
		VectorCopy(EntityToCopy.vSize, vSize);
		sName = EntityToCopy.sName;
		sSmartName = EntityToCopy.sSmartName;
		sCategory = EntityToCopy.sCategory;
		pBaseClass = EntityToCopy.pBaseClass;
		if(EntityToCopy.pKeyList)
			pKeyList = new CLinkedList<ENTITYKEYDEFINITION>(*EntityToCopy.pKeyList);
		else
			pKeyList = new CLinkedList<ENTITYKEYDEFINITION>();
		prev = next = NULL;

		memcpy(nMiscFlags, EntityToCopy.nMiscFlags, sizeof(nMiscFlags));
	}

	~ENTITYDEFINITION()
	{
		if(pKeyList)
			delete pKeyList;
		pKeyList = NULL;
	}

	bool AddKeyDefinition(ENTITYKEYDEFINITION* pKeyDef);
	ENTITYKEYDEFINITION* FindKeyDefinition(CString sKeyName);

	ENTITYDEFINITION* pBaseClass;
	CLinkedList<ENTITYKEYDEFINITION>* pKeyList;
	ENTITYKEYDEFINITION* GetKeyDefinition(const char* szName);

	bool bVirtual;
	bool bOwner;
	
	vec3_t vSize;
	CString sName;
	CString sSmartName;
	CString sCategory;

	int nMiscFlags[4];
};

//////////////////////////////////////////////////////////////////////////
// Scripted Build Structs.												//
//////////////////////////////////////////////////////////////////////////
struct _T_EXPORT RUNPROGRAMCMD
{
	RUNPROGRAMCMD* prev, *next;

	bool bWait;
	bool bConsole;
	CString sModule;
	CString sOutput;
	CString sWorkDirAlias;
	CString sWorkDir;
};

struct _T_EXPORT EXPORTMAPCMD
{
	EXPORTMAPCMD* prev, *next;
	CString sOutName;
	CString sOutput;
};

struct _T_EXPORT DEFPARAMPARAM
{
	DEFPARAMPARAM* prev, *next;

	CString sTitle;
	CString sParam;
	int nValue;
	bool bDefault;
	bool bAlways;
};

struct _T_EXPORT DEFPARAM
{
	DEFPARAM* prev, *next;

	int nValue;
	CLinkedList<DEFPARAMPARAM>* pParamList;

	DEFPARAM()
	{
		pParamList = new CLinkedList<DEFPARAMPARAM>();
	}

	~DEFPARAM()
	{
		delete pParamList;
	}

};

#define MODULE_TYPE_NOTRUNNABLE	0x00000000
#define MODULE_TYPE_RUNNABLE	0x00000001

#define PATH_TYPE_NONE			0x00000000
#define PATH_TYPE_FILE			0x00000001
#define PATH_TYPE_DIR			0x00000002

struct _T_EXPORT DEFMODULE
{
	DEFMODULE* prev, *next;
	
	DEFMODULE()
	{
		pDefParam = NULL;
		pProgramList = new CLinkedList<RUNPROGRAMCMD>();
		pExportList = new CLinkedList<EXPORTMAPCMD>();
	}

	~DEFMODULE()
	{
		if(pDefParam != NULL)
			delete pDefParam;
		delete pProgramList;
		delete pExportList;
	}

	int nType;
	int nPathType;
	CString sName;
	CString sSetupTitle;
	CString sRunTitle;
	CString sPath;
	CString sCmdLine;
	
	DEFPARAM* pDefParam;
	CLinkedList<RUNPROGRAMCMD>* pProgramList;
	CLinkedList<EXPORTMAPCMD>* pExportList;
};

struct _T_EXPORT DEFBUILD
{
	DEFBUILD()
	{
		pModuleList = new CLinkedList<DEFMODULE>();
	}

	DEFBUILD(const DEFBUILD& Copy)
	{
		pModuleList = new CLinkedList<DEFMODULE>(*Copy.pModuleList);
	}

	~DEFBUILD()
	{
		if(pModuleList)
			delete pModuleList;
		pModuleList = NULL;

	}


	CLinkedList<DEFMODULE>* pModuleList;
};

struct _T_EXPORT RUNSTATE
{
	RUNSTATE* prev, *next;

	CString sName;
	CString sValue;
	CString sCmdLine;
	int nValue;
};

class CTread3DDoc;
class CGameDefinition;

struct _T_EXPORT RUNBLOCK
{
	RUNBLOCK()
	{}

	~RUNBLOCK()
	{}

	CString sTreadFile;
	CGameDefinition* pGameDef;
	CTread3DDoc* pDocument;
};

// Predefined SCRIPT constants.
#define SCRIPT_ALWAYS		"@ALWAYS"
#define SCRIPT_ABORT_MODULE	"@ABORT"
#define SCRIPT_MAP_FILE		"@FILENAME"

//////////////////////////////////////////////////////////////////////////
// GameDefinition														//
//////////////////////////////////////////////////////////////////////////
struct	_T_EXPORT GAMEPAKFILE
{
	GAMEPAKFILE* prev, *next;

	int nHandle;
	CString sTitle;
	CString sFilename;
	bool bDir;
};

// Game API class.
class _T_EXPORT CGameAPI; // Place holder.

struct GAMECAP
{
	GAMECAP* prev, *next;

	CString sName;
	bool bEnabled;
};

// Game Definition.
class _T_EXPORT CGameDefinition : public CLLObject
{
private:

	CString m_sName;			// Game name.
	CString	m_sPalFile;			// Palette file.
	CString m_sClassNameKey;	// Name of the classname key.
	CString m_sNameKey;
	CString m_sTargetKey;
	CString m_sDefPoint;
	CString m_sDefSolid;
	CString m_sWorldspawn;
	CString m_sAuthor;
	CString m_sVersion;
	CString m_sScriptFile;
	CString m_sMapFileType;
	CString m_sTextureType;
	CString m_sGameExe;
	CString m_sGameDir;
	CString m_sGameMapDir;
	CString m_sTexDirModel;

	CLinkedList<GAMECAP> m_CapsList;
	CLinkedList<ENTITYDEFINITION> m_EntDefList;
	CLinkedList<GAMEPAKFILE> m_PakFileList;
	CLinkedList<MODEL>* m_pModelList;
	MODEL* m_pCurModel;
	ENTITYDEFINITION* m_pCurSolidEntity;
	ENTITYDEFINITION* m_pCurPointEntity;

	CTokenizer m_Tokenizer;
	DEFBUILD m_DefBuild;
	ENTITYDEFINITION m_GlobalEntDef;

#ifdef _WIN32
	CWinThread* m_pCurThread;
#endif

	// The game's palette.
	CTexPalette* m_pPalette;

	// Displays a scripting error.
	void ScriptError(ENTITYDEFINITION* pEntityDef, const char* szMessage, ...);

	// Parses $DefBuildOptions
	bool ParseDefBuild();
	DEFMODULE* ParseDefModule();
	DEFPARAM* ParseDefParams();
	
	// Parses a global defintion.
	bool ParseGlobalDefinition();
	// Parses an entity.
	ENTITYDEFINITION* ParseEntityDefinition();
	// Parses an attribute.
	bool ParseDefAttribute(ENTITYDEFINITION* pEntityDef);
	// Assigns a key type. -1 for error.
	int AssignKeyType(CString sType);
	// Verify key value based on type.
	bool VerifyKeyValue(CString sValue, int nType);
	// Verifies and gets a token.
	bool VerifyGetToken(const char* szValue);

	// Actual parsing routines.
	GAMECAP* ParseDefCap();
	ENTITYKEYDEFINITION* ParseDefKey(ENTITYDEFINITION* pEntityDef);
	ENTITYKEYDEFINITION* ParseDefOptions(CString sType, ENTITYDEFINITION* pEntityDef);
	
	// Parses ': "string"'
	bool Parse1Pair(CString& sString);
	// Parses ': "string1" : "string2"'
	bool Parse2Pair(CString& sString1, CString& sString2);
	// Parses ': "string1" : "string2" : "string3"'
	bool Parse3Pair(CString& sString1, CString& sString2, CString& sString3);
	// Parses ': "string1" : "string2" : "string3" : "string4"'
	bool Parse4Pair(CString& sString1, CString& sString2, CString& sString3, CString& sString4);

	// Allocates an entity definition.
	ENTITYDEFINITION* AllocateEntityDefinition();
	// Allocates an entity key definition.
	ENTITYKEYDEFINITION* AllocateEntityKeyDefinition();
	// Allocates a flag definition.
	ENTITYFLAGDEFINITION* AllocateEntityFlagDefinition();
	// Allocates an option definition.
	ENTITYFLAGOPTIONDEFINITION* AllocateEntityOptionDefinition();

	// Loads/Saves the games reg settings.
	bool LoadRegSettings();
	bool SaveRegSettings();

	// Loads models.
	bool LoadModels();

	// Loads all resources for the game.
	bool LoadGameResources();
	bool FreeGameResources();

	// Resolves aliases.
	//bool ResolveBuildAliases(RUNBLOCK* pRunBlock);
	
public:

	CGameDefinition() 
	{ 
		m_pCurModel = NULL;
		m_pPalette = NULL; 

#ifdef _WIN32
		m_pCurThread = NULL;
#endif

		m_pModelList = new CLinkedList<MODEL>();
	}
	~CGameDefinition()
	{
		if(m_pPalette != NULL)
			delete m_pPalette;
		delete m_pModelList;
		m_pPalette = NULL;
	}

	CTexPalette* GetPalette() { return m_pPalette; }

	CString GetGameName() { return m_sName; }
	CString GetGamePaletteName() { return m_sPalFile; }
	CString GetAuthorString() { return m_sAuthor; }
	CString GetVersionString() { return m_sVersion; }
	CString GetWorldspawnName() { return m_sWorldspawn; }
	CString GetClassNameString() { return m_sClassNameKey; }
	CString GetNameKeyString() { return m_sNameKey; }
	CString GetTargetKeyString() { return m_sTargetKey; }
	CString GetDefPointString() { return m_sDefPoint; }
	CString GetDefSolidString() { return m_sDefSolid; }
	CString GetMapFileTypeString() { return m_sMapFileType; }
	CString GetTextureTypeString() { return m_sTextureType; }
	CString GetGameExe() { return m_sGameExe; }
	void SetGameExe(CString sGameExe) { m_sGameExe = sGameExe; }
	CString GetGameDir() { return m_sGameDir; }
	void SetGameDir(CString sGameDir) { m_sGameDir = sGameDir; }
	CString GetGameMapDir() { return m_sGameMapDir; }
	void SetGameMapDir(CString sGameMapDir) { m_sGameMapDir = sGameMapDir; }

	CLinkedList<GAMEPAKFILE>* GetPakFileList() { return &m_PakFileList; }
	CLinkedList<ENTITYDEFINITION>* GetEntityDefinitionList() { return &m_EntDefList; }
	CLinkedList<MODEL>* GetModelList() { return m_pModelList; }
	CLinkedList<GAMECAP>* GetGameCapsList() { return &m_CapsList; }

	bool IsCapEnabled(CString sName);

	MODEL* GetCurrentModel() { return m_pCurModel; }
	void SetCurrentModel(MODEL* pModel) { m_pCurModel = pModel; }

	ENTITYDEFINITION* GetWorldspawnDefinition() { return &m_GlobalEntDef; }
	ENTITYDEFINITION* GetCurrentSolidEntity() { return m_pCurSolidEntity; }
	void SetCurrentSolidEntity(ENTITYDEFINITION* pEntDef) { m_pCurSolidEntity = pEntDef; }
	ENTITYDEFINITION* GetCurrentPointEntity() { return m_pCurPointEntity; }
	void SetCurrentPointEntity(ENTITYDEFINITION* pEntDef) { m_pCurPointEntity = pEntDef; }

	bool ParseGDFScript(CString sFilename);

	ENTITYDEFINITION* FindEntityDefinition(CString sClassname);
	CEntity* CreateEntityFromDefinition(CString sClassname);

	// Get's the build section. Used for programmable map compiling features.
	DEFBUILD* GetDefBuild() { return &m_DefBuild; }
	DEFMODULE* GetDefModule(CString sName);

	// Compiles/Run's a map.
	bool RunMap(RUNBLOCK* pRunBlock);

	CLinkedList<GAMEPAKFILE>* GetGamePakList() { return &m_PakFileList; }

	// Friends.
	friend class CGameAPI;

	bool SaveModel(MODEL* pModel);
};	

// The GameAPI controller.
class CBrowserDialog;
class _T_EXPORT CGameAPI
{
private:

	CGameDefinition* m_pCurGameDef;
	CLinkedList<CGameDefinition> m_GameDefinitionList;

	bool ParseGDFScript(CString sFilename);

public:

	CGameAPI();
	~CGameAPI();

	// Returns the list of gdefs.
	CLinkedList<CGameDefinition>* GetGameDefinitionList() { return &m_GameDefinitionList; }
	
	// Purges the current game and data.
	bool PurgeAndReset();

	bool SetCurrentGame(CString sName);
	CGameDefinition* GetCurrentGame() { return m_pCurGameDef; }
	// Returns the game definition with the specified name.
	CGameDefinition* GetGameDefinition(CString sName);

	// Initializes the game api.
	bool Initialize();
	bool SaveRegSettings();

};


extern CGameAPI* (*GetGameAPI) ();

//////////////////////////////////////////////////////////////////////
// General purpose routines.										//
//////////////////////////////////////////////////////////////////////

// This is the reference stuff. NOTE: we need to check this
// after the plugin header is built.
/*

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

*/

#define GF_NONE			0x00000000
#define GF_ORIGIN		0x00000001	// Build the origin.
#define GF_RANGE		0x00000002	// Build the range.

struct GDATA
{
	int m_nFlags;
	vec3_t m_vMins, m_vMaxs, m_vOrigin;
	CLinkedList<CBaseObject>* m_pList;
};

//extern int gnRecurseCount;

extern void (*G_Undo)(CLinkedList<UNDOREF>* pList, CTread3DDoc* pDocument, int nFlags);
extern void (*G_Redo)(CLinkedList<UNDOREF>* pList, CTread3DDoc* pDocument, int nFlags);
extern void (*G_PreDelete)(CLinkedList<CBaseObject>* pList, CTread3DDoc* pDocument);
extern CBaseObject* (*G_FindObject)(CLinkedList<CBaseObject>* pList, int nClassType);
extern void (*G_SplitObjectList)(CPlane* pPlane, CLinkedList<CBaseObject>* pSrcList, CLinkedList<CBaseObject>* pFrontList, CLinkedList<CBaseObject>* pDstList);
extern void (*G_InitFromSelection)(CLinkedList<CBaseObject>* pList, CTread3DDoc* pDocument, CMapView* pView, vec3_t vMins, vec3_t vMaxs, vec3_t vOrigin);
extern void (*G_Translate)(vec3_t vTrans, GDATA* pGD);
extern void (*G_Scale)(vec3_t vScale, GDATA* pGD);
extern void (*G_SetOrigin)(vec3_t vOrigin, GDATA* pGD);
extern void (*G_Reflect)(int nAxis, GDATA* pGD);
extern void (*G_Rotate)(vec3_t vRot, GDATA* pGD);
extern void (*G_Build)(GDATA* pGD);
extern float (*G_SnapFloat)(vec_t fPoint, int nGridSize);
extern float (*G_SnapFloatDelta)(vec_t fPoint, vec_t fLast, int nGridSize);
extern void (*G_SnapVector)(vec3_t vPoint, vec3_t vOut, int nGridSize, vec_t* pTrans=NULL);
extern void (*G_SnapVectorDelta)(vec3_t vPoint, vec3_t vLast, vec3_t vOut, int nGridSize, vec_t* pTrans=NULL);
extern void (*G_Snap)(GDATA* pGD, int nGridSize, vec_t* pTrans=NULL);
extern void (*G_SnapDelta)(GDATA* pGD, vec3_t vLast, int nGridSize, vec_t* pTrans=NULL);
extern void (*G_SetSize)(vec3_t vSize, GDATA* pGD);
extern void (*G_AddToMap)(CLinkedList<CBaseObject>* pList, CTread3DDoc* pDocument);
extern void (*G_AddToSelection)(CLinkedList<CBaseObject>* pList, CTread3DDoc* pDocument);
extern void (*G_CopyList)(CLinkedList<CBaseObject>* pSrc, CLinkedList<CBaseObject>* pDst);
extern void (*G_TextureList)(CLinkedList<CBaseObject>* pList, CString sTexture);
extern void (*G_TransferList)(CLinkedList<CBaseObject>* pSrc, CLinkedList<CBaseObject>* pDst);
extern int (*G_GetInterfaceCount)(CLinkedList<CBaseObject>* pList, int nInterface=I_ALL);
extern int (*G_GetClassCount)(CLinkedList<CBaseObject>* pList, int nClass=OBJECT_CLASS_ALL);
extern bool (*G_WriteObjectList)(int nVersion, CFile& hFile, CLinkedList<CBaseObject>* pList);
extern bool (*G_ReadObjectList)(int nNumObjects, int nVersion, CFile& hFile, CLinkedList<CBaseObject>* pList, CLinkedList<CBaseObject>* pUIList);
extern void (*G_InitAfterLoad)(CLinkedList<CBaseObject>* pList, CTread3DDoc* pDocument);

//////////////////////////////////////////////////////////////////////
// COLORTABLE.H														//
//////////////////////////////////////////////////////////////////////

#define CL_LINE				0
#define CL_LINE_SEL			1
#define CL_LINE_HATCH		2
#define CL_GRID				3
#define CL_GRID_ORIG		4
#define CL_HOTSPOT			5
#define CL_HOTSPOT_SEL		6
#define CL_HOTSPOT_BOX		7
#define CL_CAMERA_BASE		8
#define CL_CAMERA_LINE		9
#define CL_CAMERA_SEL		10
#define CL_CLIP				11
#define CL_ENTITY			12
#define CL_OWNED			13
#define CL_LEAK				14
#define CL_LINK				15
#define CL_FLAT_BACKGROUND	16
#define CL_3D_BACKGROUND	17
#define CL_HOTSPOT_LINE		18
#define CL_ANCHOR			19
#define CL_ANCHOR_SEL		20
#define CL_ANCHOR2			21
#define CL_BRUSH_HANDLE		22
#define CL_VIEW_SELECTED	23
#define CL_VIEW_NORMAL		24
#define CL_BBOX_SIZE		25
//----------------------------
#define CL_TABLE_LENGTH		26

//item masks
#define CI_FLOAT	1
#define CI_PENDOT	2
#define CI_PENSOLID 4
#define CI_BRUSH	8
#define CI_STRING	16

//Version number - color gets added, then change this
#define CL_VERSION 9

struct COLORITEM
{
	int m_nMask;
	float m_fColor[3];
	COLORREF m_nDefault;
#ifdef _WIN32
	CPen* m_pDot;
	CPen* m_pSolid;
	CBrush* m_pBrush;
#endif
	CString m_sName;
};

class _T_EXPORT CColorTable
{
public:
	CColorTable();
	~CColorTable();

	bool SetColor(int nID, COLORREF nColor);
	bool SetColor(int nID, float r, float g, float b);

	void Default();
	void Initialize();
	void Destroy();

#ifdef _WIN32
	CPen* GetDotPen(int nID);
	CPen* GetSolidPen(int nID);
	CPen* GetPen(int nID);
	CBrush* GetBrush(int nID);
#endif

	float* GetFloatV(int nID);
	COLORREF GetColor(int nID);

	int GetTableCount();
	CString GetColorItemString(int nID);

	void SetColorItem(int nID, COLORREF nColor);
	void SetColorItem(int nID, COLORREF nColor, int nMask);
	void SetColorItem(int nID, COLORREF nColor, int nMask, CString sName);

	void SaveTable(CString sPath);
	bool LoadTable(CString sPath);

private:
	COLORITEM m_clrTable[CL_TABLE_LENGTH];
};


extern CColorTable* (*GetColorTable) ();

//////////////////////////////////////////////////////////////////////
// RENDERAPI.H														//
//////////////////////////////////////////////////////////////////////

// Defines the view orientation array.
extern GVIEWOR* gViewOr;
extern GVIEWIN* gViewIn;
extern G2DINDEX* g2DIndex;

#define VMI(ViewType, Axis)	((*gViewIn)[ViewType][Axis])	// Maps Vector Index.
#define VO(ViewType, Axis)	((*gViewOr)[ViewType][Axis])	// Get's Vector Orientation.
#define VI(Axis, v)			(VMI(VT(v), Axis))
#define T2I(ViewType, Axis)	((*g2DIndex)[ViewType][Axis])
#define V2I(Axis, v)		(T2I(v->m_nView, Axis))
#define TU(ViewType)		((*gViewIn)[ViewType][VEC_Z])
#define VU(v)				(TU(v->m_nView))
#define VMO(Axis, v)		(VO(v->m_nView, Axis))


#define VEC_X				0
#define VEC_Y				1
#define VEC_Z				2

// Macros to index/convert coordinates.
#define VT(v) (v->m_nView)				// View Type.
#define VS(v) (v->m_fScale)				// View Scale.
#define VIS(v) (v->m_fInvScale)			// View Inverse Scale.
#define VWC_X(v) (v->m_vWinCenter[0])	// View WinCenterX.
#define VWC_Y(v) (v->m_vWinCenter[1])	// View WinCenterY.
#define VMC_X(v) (v->m_vMapCenter[0])	// View MapCenterX.
#define VMC_Y(v) (v->m_vMapCenter[1])	// View MapCenterY.
#define VMMIN_X(v) (v->m_vMapMins[0])
#define VMMAX_X(v) (v->m_vMapMaxs[0])
#define VMMIN_Y(v) (v->m_vMapMins[1])
#define VMMAX_Y(v) (v->m_vMapMaxs[1])

#define WX_TO_MX(x, v) (float)((int)(((float)x - VWC_X(v)) * VO(VT(v), VEC_X) * VIS(v) + VMC_X(v)))
#define WY_TO_MY(y, v) (float)((int)(((float)y - VWC_Y(v)) * VO(VT(v), VEC_Y) * VIS(v) + VMC_Y(v)))
#define MX_TO_WX(x, v) (float)((int)(((float)x - VMC_X(v)) * VO(VT(v), VEC_X) * VS(v) + VWC_X(v)))
#define MY_TO_WY(y, v) (float)((int)(((float)y - VMC_Y(v)) * VO(VT(v), VEC_Y) * VS(v) + VWC_Y(v)))
#define WP_TO_MX(p, v) (float)((int)(WX_TO_MX(p[VMI(VT(v), VEC_X)], v)))
#define WP_TO_MY(p, v) (float)((int)(WY_TO_MY(p[VMI(VT(v), VEC_Y)], v)))
#define MP_TO_WX(p, v) (float)((int)(MX_TO_WX(p[VMI(VT(v), VEC_X)], v)))
#define MP_TO_WY(p, v) (float)((int)(MY_TO_WY(p[VMI(VT(v), VEC_Y)], v)))

// Drawing flags.
#define RF_NONE					0x00000000
#define RF_FAST					0x00000001	// Draw fast.
#define RF_SELECTED				0x00000002	// Draw selected.
#define RF_OWNED				0x00000004	// Draw owned.
#define RF_NODRAW				0x00000008	// Don't draw the class.
#define RF_NODRAW_INTERFACES	0x00000010	// Don't draw the objects polymorphic interfaces.
#define RF_TEXTURED				0x00000020	// Texture mode.
#define RF_SOLID				0x00000040	// Solid texturing.
#define RF_LINE					0x00000080	// Line mode.
#define RF_PICKMODE				0x00000100	// Picking.
#define RF_NOHOTSPOTS			0x00000200	// Don't draw hotspots.
#define RF_REALTIME3D			0x00000400	// Don't update 3D view in realtime.
#define RF_NOSELBOX				0x00000800	// Don't draw the selection box.
#define RF_BBOXHATCHED			0x00001000	// Draw hatched bbox.
#define RF_SELBOXONTOP			0x00002000	// Force the selbox on top.
#define RF_NOSELBOXSIZE			0x00004000	// Don't show the selbox size.
#define RF_FORCESELBOX			0x00008000	// Force the selbox to be drawn
#define RF_NOENTNAMES			0x00010000	// Don't draw entity names.
#define RF_NOENTITYFILL			0x00020000	// Don't fill in entities.
#define RF_NOBILINEARFILTER		0x00040000	// Turn off bilinear filtering.
#define RF_NO3DSELBOX			0x00080000	// Turn off the 3d selbox
#define RF_NO3DSELOUTLINE		0x00100000	// Turn off 3d selection outline.
#define RF_DRAWLITTLEX			0x00200000	// Draw's a cute little X on the first selected item :)
#define RF_NO3DVIEW				0x00400000	// Disable the 3D view.
#define RF_NODRAWSEL			0x00800000	// Don't draw things selected.
#define RF_NOTRANSPARENCY		0x01000000	// Don't draw transparent object.
#define RF_NOFOG				0x02000000	// No fog
#define RF_TRANSPARENT			0x04000000	// It's transparent.
#define RF_FOG					0x08000000	// It's foggy.
#define RF_SHADED				0x10000000	// Draw angle shaded.

#define RF_ALL					0xFFFFFFFF

#define RT_TRANSPARENT			0x00000001

#define GLWINDOWRENDERER_UID		0x00000001
#define GL3DWINDOWRENDERER_UID		0x00000002
#define GDIWINDOWRENDERER_UID		0x00000004

class CHotSpot;
class CMapView;
class CBaseRenderPackage;

class _T_EXPORT CBaseWindowRenderer
{
protected:

	vec3_t m_vCamPos, m_vCamRot, m_vCamScale;

public:

	CBaseWindowRenderer() 
	{
		vec3_t v = {1.0F, 1.0F, 1.0F};
		VectorCopy(v, m_vCamScale);
	}
	virtual ~CBaseWindowRenderer() {}

	// Initializes the rendering interface.
	virtual bool Initialize(CMapView* pView, CDC* pDC, int nType) = 0;
	virtual bool Shutdown(CMapView* pView, CDC* pDC) = 0;
	virtual int GetRendererUID() = 0;

	void SetCameraPos(vec3_t vCamPos);
	void SetCameraRot(vec3_t vCamRot);
	void SetCameraScale(vec3_t vCamScale);

	virtual bool LockDevice(CMapView* pView, CTread3DDoc* pDocument, CDC* pDC, int nDrawFlags) { return true; }
	virtual bool UnlockDevice(CMapView* pView, CTread3DDoc* pDocument, CDC* pDC, int nDrawFlags) { return true; }
	virtual void OnSize(CMapView* pView, CDC* pDC)=0;
	virtual void ClearWindow(CMapView* pView, CTread3DDoc* pDocument, CDC* pDC, int nClearColor, int nFlags=0)=0;
	virtual void DrawBBox(CMapView* pView, CTread3DDoc* pDocument, CDC* pDC, vec3_t vMins, vec3_t vMaxs, int nDrawColor, int nFlags=0)=0;
	virtual void DrawBBoxSize(CMapView* pView, CTread3DDoc* pDocument, CDC* pDC, vec3_t vMins, vec3_t vMaxs, vec3_t vCenter, int nDrawColor)=0;
	virtual void DrawGrid(CMapView* pView, CTread3DDoc* pDocument, CDC* pDC, int nFlags=0) = 0;
	virtual void DrawObject(CMapView* pView, CTread3DDoc* pDocument, CDC* pDC, CBaseObject* pObject, int nObjectClass, int nObjectInterfaces, int nDrawFlags, int nFlags=0) = 0;
	virtual void DrawHotSpot(CMapView* pView, CTread3DDoc* pDocument, CDC* pDC, CHotSpot* pSpot, int nDrawFlags, int nFlags) {}
	virtual void DrawLittleX(CMapView* pView, CTread3DDoc* pDocument, CDC* pDC, vec2_t vMapPos, int nColor) {}
	virtual void EnableTransparency(bool bTransparent=true) {}

	// Draws the I_TARGET interface.
	virtual void DrawI_TARGET(CMapView* pView, CTread3DDoc* pDocument, CDC* pDC, CBaseObject* pObject, int nDrawFlags, int nType)=0;

	virtual CBaseObject* GetHitObject(CMapView* pView, CTread3DDoc* pDocument, int nClassFlags, int nMouseX, int nMouseY) = 0;
	virtual void Flush(CMapView* pView, CTread3DDoc* pDocument, CDC* pDC)=0; // Flushes the renderer. When it returns all draw commands have been completed and displayed.

	friend class CBaseRenderPackage;
};

#define RF_3D_API	0x00000001
#define RF_2D_API	0x00000002

class _T_EXPORT CBaseRenderPackage
{
public:

	CBaseRenderPackage() {}
	virtual ~CBaseRenderPackage() {}

	static bool StaticClaimPackageName(CString sName) { return false; }

	virtual int GetRenderFlags() = 0;
	virtual bool LoadPackage(CString sName) = 0;
	virtual bool UnloadPackage() = 0;
	virtual CString GetPackageName() = 0;
	virtual CBaseWindowRenderer* GetWindowRenderer(CMapView* pView, CDC* pDC, int nType) = 0;
};


class _T_EXPORT CRenderAPI
{
private:

	int m_nGlobalRenderFlags;
	int m_nClassDrawFilter;
	CBaseRenderPackage* m_p2DPackage, *m_p3DPackage;
	int m_nNumTransparents;
	int m_nNumSelectedTransparents;
	CBaseObject* m_pTransparents[MAX_TRANSPARENTS];
	CBaseObject* m_pSelectedTransparents[MAX_TRANSPARENTS];
	void ShutdownPackages(void);
	void Shutdown2DPackage(void);
	void Shutdown3DPackage(void);
	CBaseRenderPackage* CreateRenderPackage(CString sName);

	bool AddTransparent(CBaseObject* pObject);
	bool AddSelectedTransparent(CBaseObject* pObject);
	void ResetTransparents();
	void ResetSelectedTransparents();
	void DrawTransparents(CBaseObject**, int nNum, CMapView* pView, CTread3DDoc* pDocument, CDC* pDC, CBaseWindowRenderer* pWR, int nDrawFlags, int nType);

public:

	CRenderAPI()
	{
		m_nClassDrawFilter=0;
		m_p2DPackage=NULL;
		m_p3DPackage=NULL;
	}

	~CRenderAPI()
	{}

	bool Initialize();
	bool Shutdown();

	bool Load2DRenderPackage(CString nName);
	bool Load3DRenderPackage(CString nName);

	void Get2DRenderPackageList(CString sNames[MAX_RENDER_PACKAGES], CString sDescriptions[MAX_RENDER_PACKAGES], int& nNumPaks);
	void Get3DRenderPackageList(CString sNames[MAX_RENDER_PACKAGES], CString sDescriptions[MAX_RENDER_PACKAGES], int& nNumPaks);

	CString Get2DPackageName();
	CString Get3DPackageName();
	CBaseWindowRenderer* GetWindowRenderer(CMapView* pView, CDC* pDC, int nType);

	void AddDrawFilters(int nFlags);
	void RemoveDrawFilters(int nFlags);
	void AddGlobalRenderFlags(int nFlags);
	void RemoveGlobalRenderFlags(int nFlags);
	int GetGlobalRenderFlags() { return m_nGlobalRenderFlags; }
	int GetDrawFilters() { return m_nClassDrawFilter; }

	// Draws an object list.
	void DrawObjectList(CMapView* pView, CTread3DDoc* pDocument, CDC* pDC, CBaseWindowRenderer* pWR, CLinkedList<CBaseObject>* pObjectList, int nDrawFlags, int nType);

	// Renders a map.
	void RenderMap(CMapView* pView, CTread3DDoc* pDocument, CDC* pDC, CBaseWindowRenderer* pWR, int nRenderFlags, int nType);
	void RenderMapSelection(CMapView* pView, CTread3DDoc* pDocument, CDC* pDC, CBaseWindowRenderer* pWR, int nRenderFlags, int nType);

};



struct WOGLFUNCTIONTABLE
{
	void (APIENTRY *glArrayElement) (GLint i);
	void (APIENTRY *glBegin) (GLenum mode);
	void (APIENTRY *glBindTexture) (GLenum target, GLuint texture);
	void (APIENTRY *glBitmap) (GLsizei width, GLsizei height, GLfloat xorig, GLfloat yorig, GLfloat xmove, GLfloat ymove, const GLubyte *bitmap);
	void (APIENTRY *glBlendFunc) (GLenum sfactor, GLenum dfactor);
	void (APIENTRY *glCallList) (GLuint list);
	void (APIENTRY *glCallLists) (GLsizei n, GLenum type, const GLvoid *lists);
	void (APIENTRY *glClear) (GLbitfield mask);
	void (APIENTRY *glClearAccum) (GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
	void (APIENTRY *glClearColor) (GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);
	void (APIENTRY *glClearDepth) (GLclampd depth);
	void (APIENTRY *glColor3f) (GLfloat red, GLfloat green, GLfloat blue);
	void (APIENTRY *glColor3fv) (const GLfloat *v);
	void (APIENTRY *glColor4f) (GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
	void (APIENTRY *glCullFace) (GLenum mode);
	void (APIENTRY *glDeleteLists) (GLuint list, GLsizei range);
	void (APIENTRY *glDeleteTextures) (GLsizei n, const GLuint *textures);
	void (APIENTRY *glDepthFunc) (GLenum func);
	void (APIENTRY *glDepthMask) (GLboolean flag);
	void (APIENTRY *glDepthRange) (GLclampd zNear, GLclampd zFar);
	void (APIENTRY *glDisable) (GLenum cap);
	void (APIENTRY *glDisableClientState) (GLenum array);
	void (APIENTRY *glDrawArrays) (GLenum mode, GLint first, GLsizei count);
	void (APIENTRY *glDrawBuffer) (GLenum mode);
	void (APIENTRY *glDrawElements) (GLenum mode, GLsizei count, GLenum type, const GLvoid *indices);
	void (APIENTRY *glDrawPixels) (GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *pixels);
	void (APIENTRY *glEnable) (GLenum cap);
	void (APIENTRY *glEnableClientState) (GLenum array);
	void (APIENTRY *glEnd) (void);
	void (APIENTRY *glEndList) (void);
	void (APIENTRY *glFinish) (void);
	void (APIENTRY *glFlush) (void);
	void (APIENTRY *glFrontFace) (GLenum mode);
	void (APIENTRY *glFrustum) (GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar);
	GLenum (APIENTRY *glGetError) (void);
	const GLubyte * (APIENTRY *glGetString) (GLenum name);
	void (APIENTRY *glHint) (GLenum target, GLenum mode);
	void (APIENTRY *glListBase) (GLuint base);
	void (APIENTRY *glLoadIdentity) (void);
	void (APIENTRY *glMatrixMode) (GLenum mode);
	void (APIENTRY *glPushMatrix) (void);
	void (APIENTRY *glPopMatrix) (void);
	void (APIENTRY *glGetIntegerv) (GLenum pname, GLint *params);
	void (APIENTRY *glPolygonOffset) (GLfloat factor, GLfloat units);

	void (APIENTRY *glInitNames) (void);
	void (APIENTRY *glSelectBuffer) (GLsizei size, GLuint *buffer);
	void (APIENTRY *glLoadName) (GLuint name);
	void (APIENTRY *glPushName) (GLuint name);

	void (APIENTRY *glNewList) (GLuint list, GLenum mode);
	void (APIENTRY *glRasterPos2i) (GLint x, GLint y);
	GLint (APIENTRY *glRenderMode) (GLenum mode);
	void (APIENTRY *glRotatef) (GLfloat angle, GLfloat x, GLfloat y, GLfloat z);
	void (APIENTRY *glScalef) (GLfloat x, GLfloat y, GLfloat z);
	void (APIENTRY *glTexCoord2f) (GLfloat s, GLfloat t);
	void (APIENTRY *glTexCoord2fv) (const GLfloat *v);
	void (APIENTRY *glTexCoord2i) (GLint s, GLint t);
	GLboolean (APIENTRY* glIsTexture) (GLuint texture);
	void (APIENTRY *glTexEnvi) (GLenum target, GLenum pname, GLint param);
	void (APIENTRY *glTexImage2D) (GLenum target, GLint level, GLint components, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid *pixels);
	void (APIENTRY *glTexParameteri) (GLenum target, GLenum pname, GLint param);
	void (APIENTRY *glTexSubImage2D) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *pixels);
	void (APIENTRY *glTranslatef) (GLfloat x, GLfloat y, GLfloat z);
	void (APIENTRY *glVertex2i) (GLint x, GLint y);
	void (APIENTRY *glVertex2f) (GLfloat x, GLfloat y);
	void (APIENTRY *glVertex3f) (GLfloat x, GLfloat y, GLfloat z);
	void (APIENTRY *glVertex3fv) (const GLfloat *v);
	void (APIENTRY *glVertex3i) (GLint x, GLint y, GLint z);
	void (APIENTRY *glVertex3iv) (const GLint *v);
	void (APIENTRY *glVertex3s) (GLshort x, GLshort y, GLshort z);
	void (APIENTRY *glVertex3sv) (const GLshort *v);
	void (APIENTRY *glVertexPointer) (GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
	void (APIENTRY *glViewport) (GLint x, GLint y, GLsizei width, GLsizei height);
	void (APIENTRY *glPixelStorei) (GLenum pname, GLint param);
	void (APIENTRY *glPixelTransferf) (GLenum pname, GLfloat param);
	void (APIENTRY *glShadeModel) (GLenum mode);

	// Glu extensions.
	void (APIENTRY *gluPerspective) (GLdouble fovy, GLdouble aspect, GLdouble zNear, GLdouble zFar);
	void (APIENTRY *gluLookAt) (GLdouble eyeX, GLdouble eyeY, GLdouble eyeZ, GLdouble centerX, GLdouble centerY, GLdouble centerZ, GLdouble upX, GLdouble upY, GLdouble upZ);
	void (APIENTRY *gluOrtho2D) (GLdouble left, GLdouble right, GLdouble bottom, GLdouble top);
	GLint (APIENTRY *gluScaleImage) (GLenum format, GLsizei wIn, GLsizei hIn, GLenum typeIn, const void *dataIn, GLsizei wOut, GLsizei hOut, GLenum typeOut, GLvoid* dataOut);
	void (APIENTRY *gluPickMatrix) (GLdouble x, GLdouble y, GLdouble delX, GLdouble delY, GLint *viewport);

#ifdef _WIN32
	// Windows extensions.
	BOOL  (WINAPI *e_wglCopyContext)(HGLRC, HGLRC, UINT);
	HGLRC (WINAPI *e_wglCreateContext)(HDC);
	HGLRC (WINAPI *e_wglCreateLayerContext)(HDC, int);
	BOOL  (WINAPI *e_wglDeleteContext)(HGLRC);
	HGLRC (WINAPI *e_wglGetCurrentContext)(VOID);
	HDC   (WINAPI *e_wglGetCurrentDC)(VOID);
	PROC  (WINAPI *e_wglGetProcAddress)(LPCSTR);
	BOOL  (WINAPI *e_wglMakeCurrent)(HDC, HGLRC);
	BOOL  (WINAPI *e_wglShareLists)(HGLRC, HGLRC);
	BOOL  (WINAPI *e_wglUseFontBitmapsA)(HDC, DWORD, DWORD, DWORD);
	BOOL  (WINAPI *e_wglUseFontBitmapsW)(HDC, DWORD, DWORD, DWORD);
	BOOL  (WINAPI *e_SwapBuffers)(HDC);
	int   (WINAPI *e_ChoosePixelFormat)(HDC, CONST PIXELFORMATDESCRIPTOR *);
	BOOL  (WINAPI *e_SetPixelFormat)(HDC, int, CONST PIXELFORMATDESCRIPTOR *);
#endif

};



#define GWLF_NONE	0x00000000
#define GWLF_VERT	0x00000001
#define GWLF_HORZ	0x00000002
#define GWLF_RANGE	0x00000004

class CBaseObject;
struct GDIWINDOWLINE
{
	float m_fMinX, m_fMaxX;
	float m_fMinY, m_fMaxY;
	float m_fX1, m_fX2;
	float m_fY1, m_fY2;
	float m_fB, m_fSlope;
	int m_nFlags;
	bool m_bBuild;
	CBaseObject* m_pObject;

	GDIWINDOWLINE();
	void BuildLine();
	bool Hit(float fX, float fY);
};

#ifdef _WIN32
class CGDIRenderPackage;
class CGDIBrushRenderer;
class CGDIEntityRenderer;
class CGDICameraRenderer;
class CGDIClipToolRenderer;
class CGDIAnchorRenderer;

class _T_EXPORT CGDIWindowRenderer : public CBaseWindowRenderer
{
protected:

	CDC* m_pBitmapDC;
	HBITMAP m_hBitmap, m_hOldBitmap;
	CGDIBrushRenderer* m_pBR;
	CGDIEntityRenderer* m_pER;
	CGDICameraRenderer* m_pCR;
	CGDIClipToolRenderer* m_pCTR;
	CGDIAnchorRenderer* m_pAR;
	CFont BBoxSizeFont;
	CFont ViewTypeFont;

	int m_nNumLines;
	int m_nNextLine;
	GDIWINDOWLINE* m_pLines;

	bool CreateDib24(CMapView* pView, CDC* pDC);
	bool DeleteDib24();

public:

	CGDIWindowRenderer();
	virtual ~CGDIWindowRenderer();

	// Initializes the rendering interface.
	virtual bool Initialize(CMapView* pView, CDC* pDC, int nType);
	virtual bool Shutdown(CMapView* pView, CDC* pDC);
	virtual int GetRendererUID() { return GDIWINDOWRENDERER_UID; }

	virtual bool LockDevice(CMapView* pView, CTread3DDoc* pDocument, CDC* pDC, int nDrawFlags);
	virtual void OnSize(CMapView* pView, CDC* pDC);
	virtual void ClearWindow(CMapView* pView, CTread3DDoc* pDocument, CDC* pDC, int nClearColor, int nFlags=0);
	virtual void DrawBBox(CMapView* pView, CTread3DDoc* pDocument, CDC* pDC, vec3_t vMins, vec3_t vMaxs, int nDrawColor, int nFlags=0);
	virtual void DrawBBoxSize(CMapView* pView, CTread3DDoc* pDocument, CDC* pDC, vec3_t vMins, vec3_t vMaxs, vec3_t vCenter, int nDrawColor);
	virtual void DrawI_TARGET(CMapView* pView, CTread3DDoc* pDocument, CDC* pDC, CBaseObject* pObject, int nDrawFlags, int nType);
	virtual void DrawLittleX(CMapView* pView, CTread3DDoc* pDocument, CDC* pDC, vec2_t vMapPos, int nColor);

	bool ClipLine(CMapView* pView, vec_t* vPta, vec_t* vPtb);

	virtual void DrawHotSpot(CMapView* pView, CTread3DDoc* pDocument, CDC* pDC, CHotSpot* pSpot, int nDrawFlags, int nFlags);
	virtual void DrawGrid(CMapView* pView, CTread3DDoc* pDocument, CDC* pDC, int nFlags=0);
	virtual void DrawObject(CMapView* pView, CTread3DDoc* pDocument, CDC* pDC, CBaseObject* pObject, int nObjectClass, int nObjectInterfaces, int nDrawFlags, int nFlags=0);
	virtual CBaseObject* GetHitObject(CMapView* pView, CTread3DDoc* pDocument, int nClassFlags, int nMouseX, int nMouseY);
	virtual void Flush(CMapView* pView, CTread3DDoc* pDocument, CDC* pDC);
	GDIWINDOWLINE* GetWindowLine();

	friend class CGDIRenderPackage;
};

#endif


class CGL3DBrushRenderer;
class CGL3DBezierPatchRenderer;
class CGL3DEntityRenderer;

#define PICK_BUFF_SIZE	512
#define GL3D_BASE_ALPHA	0.50F

class _T_EXPORT CGL3DWindowRenderer : public CBaseWindowRenderer
{
protected:

	int m_nDownloadCount;
	
	bool m_bBias;
	bool m_bBilinear;
	int m_nBiasDrawFlags;
	int m_nTextureDrawFlags;
	int m_nNumHits;
	int m_nMouseX, m_nMouseY;
	unsigned int m_nCurName;
	unsigned int m_nPickBuff[PICK_BUFF_SIZE];
	CTexture* m_pCurTex;
	bool GeneratePixelFormat(CDC* pDC);
	bool SetPixelFormat(CDC* pDC);
	
#ifdef _WIN32
	HGLRC m_hglrc;
	// Generates the pixel format.
	int m_nPixelFormat;
#endif

	WOGLFUNCTIONTABLE* m_pGLFT;

	// Rendering objects.
	CGL3DBrushRenderer* m_pBR;
	CGL3DEntityRenderer* m_pER;

	
	void MakePowerOf2(CTexture* pTex);

public:

	CGL3DWindowRenderer(WOGLFUNCTIONTABLE* pGLFT);
	virtual ~CGL3DWindowRenderer();

	// Initializes the rendering interface.
	virtual bool Initialize(CMapView* pView, CDC* pDC, int nType);
	virtual bool Shutdown(CMapView* pView, CDC* pDC);
	virtual int GetRendererUID() { return GL3DWINDOWRENDERER_UID; }

	virtual bool LockDevice(CMapView* pView, CTread3DDoc* pDocument, CDC* pDC, int nDrawFlags);
	virtual bool UnlockDevice(CMapView* pView, CTread3DDoc* pDocument, CDC* pDC, int nDrawFlags);
	
	virtual void OnSize(CMapView* pView, CDC* pDC);
	virtual void ClearWindow(CMapView* pView, CTread3DDoc* pDocument, CDC* pDC, int nClearColor, int nFlags=0);
	virtual void DrawBBox(CMapView* pView, CTread3DDoc* pDocument, CDC* pDC, vec3_t vMins, vec3_t vMaxs, int nDrawColor, int nFlags=0);
	virtual void DrawBBoxSize(CMapView* pView, CTread3DDoc* pDocument, CDC* pDC, vec3_t vMins, vec3_t vMaxs, vec3_t vCenter, int nDrawColor){}
	virtual void DrawI_TARGET(CMapView* pView, CTread3DDoc* pDocument, CDC* pDC, CBaseObject* pObject, int nDrawFlags, int nType);

	virtual void DrawGrid(CMapView* pView, CTread3DDoc* pDocument, CDC* pDC, int nFlags=0) {}
	virtual void DrawObject(CMapView* pView, CTread3DDoc* pDocument, CDC* pDC, CBaseObject* pObject, int nObjectClass, int nObjectInterfaces, int nDrawFlags, int nFlags=0);
	virtual CBaseObject* GetHitObject(CMapView* pView, CTread3DDoc* pDocument, int nClassFlags, int nMouseX, int nMouseY);
	virtual void Flush(CMapView* pView, CTread3DDoc* pDocument, CDC* pDC);
	virtual void EnableTransparency(bool bTransparent=true);

	void SetBias(bool bBias, int nDrawFlags=0);
	bool GetBias() { return m_bBias; }
	void BindTexture(CTexture* pTexture, int nDrawFlags, bool bClamp=false);
	void DetachTexture(CTexture* pTexture);

	friend class CGLRenderPackage;
};



class CGLFaceRenderer;
class CGLBrushRenderer;
class CGLCameraRenderer;
class CGLEntityRenderer;
class CGLClipToolRenderer;
class CGLAnchorRenderer;
class CGLPatchMeshRenderer;

class _T_EXPORT CGLWindowRenderer : public CBaseWindowRenderer
{
private:

#ifdef _WIN32
	int m_nPixelFormat;
	HGLRC m_hglrc;
	// Generates the pixel format.
	
#endif

	bool GeneratePixelFormat(CDC* pDC);
	bool SetPixelFormat(CDC* pDC);
	WOGLFUNCTIONTABLE* m_pGLFT;

	CGLBrushRenderer* m_pBR;
	CGLCameraRenderer* m_pCR;
	CGLEntityRenderer* m_pER;
	CGLClipToolRenderer* m_pCTR;
	CGLAnchorRenderer* m_pAR;
	CGLPatchMeshRenderer* m_pMR;

	// We elicit some GDI routines to keep track of drawing. We do this
	// because GL's hit detection does not allow us to give the user some leeway.
	// The GDI renderer can tell if a user clicked "close" to a line. The GL pick
	// only tells if it was clicked on that exact pixel.
	int m_nNumLines;
	int m_nNextLine;
	GDIWINDOWLINE* m_pLines;

public:

	CGLWindowRenderer(WOGLFUNCTIONTABLE* pGLFT);
	virtual ~CGLWindowRenderer();

	// Initializes the rendering interface.
	virtual bool Initialize(CMapView* pView, CDC* pDC, int nType);
	virtual bool Shutdown(CMapView* pView, CDC* pDC);
	virtual int GetRendererUID() { return GLWINDOWRENDERER_UID; }

	void DrawString(int nX, int nY, CString sString);

	virtual bool LockDevice(CMapView* pView, CTread3DDoc* pDocument, CDC* pDC, int nDrawFlags);
	virtual bool UnlockDevice(CMapView* pView, CTread3DDoc* pDocument, CDC* pDC, int nDrawFlags);
	
	virtual void OnSize(CMapView* pView, CDC* pDC);
	virtual void ClearWindow(CMapView* pView, CTread3DDoc* pDocument, CDC* pDC, int nClearColor, int nFlags=0);
	virtual void DrawBBox(CMapView* pView, CTread3DDoc* pDocument, CDC* pDC, vec3_t vMins, vec3_t vMaxs, int nDrawColor, int nFlags=0);
	virtual void DrawBBoxSize(CMapView* pView, CTread3DDoc* pDocument, CDC* pDC, vec3_t vMins, vec3_t vMaxs, vec3_t vCenter, int nDrawColor);
	virtual void DrawI_TARGET(CMapView* pView, CTread3DDoc* pDocument, CDC* pDC, CBaseObject* pObject, int nDrawFlags, int nType);
	virtual void DrawLittleX(CMapView* pView, CTread3DDoc* pDocument, CDC* pDC, vec2_t vMapPos, int nColor);

	bool ClipLine(CMapView* pView, vec_t* vPta, vec_t* vPtb);

	virtual void DrawHotSpot(CMapView* pView, CTread3DDoc* pDocument, CDC* pDC, CHotSpot* pSpot, int nDrawFlags, int nFlags);
	virtual void DrawGrid(CMapView* pView, CTread3DDoc* pDocument, CDC* pDC, int nFlags=0);
	virtual void DrawObject(CMapView* pView, CTread3DDoc* pDocument, CDC* pDC, CBaseObject* pObject, int nObjectClass, int nObjectInterfaces, int nDrawFlags, int nFlags=0);
	virtual CBaseObject* GetHitObject(CMapView* pView, CTread3DDoc* pDocument, int nClassFlags, int nMouseX, int nMouseY);
	virtual void Flush(CMapView* pView, CTread3DDoc* pDocument, CDC* pDC);
	GDIWINDOWLINE* GetWindowLine();

	friend class CGLRenderPackage;
};

extern CRenderAPI* (*GetRenderAPI) ();

//////////////////////////////////////////////////////////////////////
// MAPFRAME.H														//
//////////////////////////////////////////////////////////////////////

#define VIEW_UNINITIALIZED	0
#define VIEW_INITIALIZED	1
#define VIEW_EXPANDED		2
#define VIEW_NORMAL			3

class _T_EXPORT CMapFrame : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CMapFrame)
public:
	CMapFrame();
	CMapView* GetMapPane(UINT nPane);
	CConsoleView* GetConsolePane();
	CDC* GetPaneDC(UINT nPane);
	CRect GetPaneRect(UINT nPane);

	void SetViewsZoom(float fZoom);
	void SetViewsGrid(int fGrid);

	void SetCurrentViewHandle(int nView);
	int GetCurrentViewHandle();
	CMapView* GetCurrentView();
	UINT GetCurrentViewType();
	void ExpandCurrentView();
	
// Attributes
protected:
	CSplitterWnd m_mainSplitter;
	CSplitterWnd m_subSplitter1;
	CSplitterWnd m_subSplitter2;
	//bool m_bInit;
	int m_nHandle;
	int m_nCurrentView;
	int m_nHeight;
	int m_nWidth;
	int m_nViewMode;
	int m_nTimerId;

	CConsoleView* m_pConsole;	//pointer to the console
	CMapView* m_pMapPane[4];	//array of pointers to the map panes

public:

	CTread3DDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMapFrame)
	public:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL DestroyWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMapFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// Generated message map functions
protected:
	//{{AFX_MSG(CMapFrame)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnClose();
	afx_msg void OnViewExpandcurrent();
	afx_msg void OnUpdateViewExpandcurrent(CCmdUI* pCmdUI);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////
// MAPVIEW.H														//
//////////////////////////////////////////////////////////////////////
class _T_EXPORT CMapView : public CView
{
protected:
	CMapView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CMapView)

// Attributes
public:
	void UnbindView();
	void SetView(UINT nView);
	UINT GetViewType();
	void SetParent(CMapFrame* pFrame, int nHandle);
	void KillFocus();

private:
	bool m_bKey;
	bool m_bBlock;
	
	CBaseWindowRenderer* m_pWR;

	bool m_bRedraw;
	int m_nGlobalRenderFlags;
	int m_nClassDrawFilter;
	int m_nRange;
	int m_nSmallStep;
	int m_nLargeStep;
	
	CCamera* m_pCurCam; // Used for 3D views.

	CWindowEventManager m_EventManager;
	CHotSpotManager m_HotSpotManager;

	CCamera* CreateDefaultCamera();
	CCamera* FindCameraForView();
	void SetupViewResponders();
	void AdjustCamera();
	
// Operations
public:
	CTread3DDoc* GetDocument() { return (CTread3DDoc*)m_pDocument; }
	
	int m_nView;
	bool m_bFocus;
	vec2_t m_vSize;
	vec2_t m_vWinCenter;
	vec2_t m_vMapCenter;
	vec2_t m_vMapMins, m_vMapMaxs;
	vec_t m_fScale, m_fInvScale;
	RECT m_rcClientRect;
	CString m_sView;
	HCURSOR m_hCursor;

	CMapFrame* m_pParentFrame;
	int m_nHandle;
	
	// Calculates map extents.
	void CalcMapBounds();
	void CalcWindowSizes();

	float GetScale() { return m_fScale; }
	float GetInvScale() { return m_fInvScale; }
	void SetScale(float fScale);

	void SetCursorToCenter();
	void SetMapXY(float fX, float fY);
	void SetupDefaultCamera();
	void AddDrawFilters(int nFlags);
	void RemoveDrawFilters(int nFlags);
	void AddGlobalRenderFlags(int nFlags);
	void RemoveGlobalRenderFlags(int nFlags);
	int GetGlobalRenderFlags() { return m_nGlobalRenderFlags; }
	int GetDrawFilters() { return m_nClassDrawFilter; }

	void SetCamera(CCamera* pCam);
	CCamera* GetCamera() { return m_pCurCam; }

	void RedrawWindow();
	void RedrawWindowSelections();
	CBaseWindowRenderer* GetWindowRenderer() { return m_pWR; }

	CWindowEventManager* GetWindowEventManager() { return &m_EventManager; }
	CHotSpotManager* GetHotSpotManager() { return &m_HotSpotManager; }
	
	void EnableRedraw(bool bRedraw=true) { m_bRedraw = bRedraw; }

	friend class CRenderAPI;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMapView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CMapView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CMapView)
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnVScroll(UINT nSBCode, int nPos, CScrollBar* pScrollBar);
	afx_msg void OnHScroll(UINT nSBCode, int nPos, CScrollBar* pScrollBar);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


//////////////////////////////////////////////////////////////////////
// TREAD3DDOC.H														//
//////////////////////////////////////////////////////////////////////
// Defines Map Context, MFC Document used by Tread.
struct TEXMRUREF
{
	TEXMRUREF* prev, *next;
	CString sName;
	CTexture* pTex;
};

struct SURFACEOBJREF
{
	SURFACEOBJREF* prev, *next;
	CBaseObject* pObj;
};

struct RUNSTATE;

#ifdef _WIN32
class _T_EXPORT CDocument;
class _T_EXPORT CTread3DDoc : public CDocument
#else
class CTread3DDoc
#endif
{
#ifdef _WIN32
protected: // create from serialization only
	CTread3DDoc();
	DECLARE_DYNCREATE(CTread3DDoc)
#else

public:
	CTread3DDoc();

#endif

private:

	int m_bNewMap;
	int m_bSnapToGrid;
	int m_nGridSize;
	int m_nNumSelBrushes;
	int m_nNumSelEnts;
	int m_nIALF;
	int m_nNumStates;
	int m_nCurveSubDepth;
	int m_nStoreX, m_nStoreY;
	bool m_bBrushHandles;
	bool m_bDrawTargets;
	bool m_bUpdateAllViews;
	void* m_pStoreParam;
	float m_fHollowDepth;
	CMode m_Mode;
	CEntity* m_pWorldspawn;
	CLinkedList<CBaseObject>* m_pObjectList;
	CLinkedList<CBaseObject>* m_pSelectedObjectList;
	CLinkedList<SURFACEOBJREF>* m_pSurfaceObjList;
	CLinkedList<CBaseObject>* m_pUIList;
	CLinkedList<TEXMRUREF>* m_pTextureMRU;
	RUNSTATE* m_pRunStates;	// For saving/restoring.
	CVisgroupAPI m_VisgroupAPI;
	CTargetAPI m_TargetAPI;
	CUndoAPI m_UndoAPI;
	CMapView* m_pCurView;
	
	TREAD2XHEADERINFO m_Header;

	void CreateTestMap();
	void SetupModeResponders();
	void TextureMRUIAL();
	void SetupDefaultRunStates();

	CBaseTextureableObject* GetFirstSurfaceObjectAsTextureable();
	SURFACEOBJREF* FindSurfaceObjectRef(CBaseObject* pObject);
	void UpdateSurfaceAttributes();

	int m_nClassSelectionFilter;
	int m_nGlobalRenderFlags;
	int m_nClassDrawFilter;

	CModeChangeEventManager m_EventManager;

#ifdef _WIN32
	void UpdatePluginItem(int nID, CCmdUI* pCmdUI);
	void CallPluginCommand(int nID);
#endif

	// Attributes
public:

	friend class CRenderAPI;

// Operations
public:

	vec3_t m_vSelMins, m_vSelMaxs, m_vSelCenter, m_vSelSize;

	TREAD2XHEADERINFO* GetTread2XHeader();
	int IsNewMap() { return m_bNewMap; }
	void SetScale(float fScale);
	int GetGridSize() { return m_nGridSize; }
	void SetGridSize(int nGridSize) { m_nGridSize = nGridSize; }
	bool GetGridSnap() { return m_bSnapToGrid; }
	void SetGridSnap(bool bSnap) { m_bSnapToGrid = bSnap; }
	void SetHollowDepth(float fDepth) { m_fHollowDepth = fDepth; }
	float GetHollowDepth() { return m_fHollowDepth; }
	void SetCurveSubDepth(int nSubDepth) { m_nCurveSubDepth = nSubDepth; }
	int GetCurveSubDepth() { return m_nCurveSubDepth; }
	void StoreXY(int nX, int nY, void* pParam);
	void RetrieveXY(int* pX, int* pY, void** pParam);
	void CycleColorIfSelected();

	bool GetBrushHandles() { return m_bBrushHandles; }
	void SetBrushHandles(bool bBrushHandles) { m_bBrushHandles = bBrushHandles; }
	bool GetDrawTargets() { return m_bDrawTargets; }
	void SetDrawTargets(bool bDrawTargets) { m_bDrawTargets = bDrawTargets; }
	bool GetUpdateAllViews() { return m_bUpdateAllViews; }
	void SetUpdateAllViews(bool bUpdate) { m_bUpdateAllViews = bUpdate; }

	void SetCurView(CMapView* pView) { m_pCurView = pView; }
	CMapView* GetCurView() { return m_pCurView; }
	
	TREAD2XHEADERINFO* GetHeader() { return &m_Header; }

	void Undo();
	void Cut();
	void Copy();
	void Paste();

	/////////
	// Texture manipulations: works on SurfaceObjs.
	int GetContentsFlags();
	int GetSurfaceFlags();
	int GetValueFlags();
	void SetContentsFlags(int nContents);
	void SetSurfaceFlags(int nFlags);
	void SetValueFlags(int nValue);

	void SetTexShifts(vec_t vTexShifts[2]);
	void GetTexShifts(vec_t vTexShifts[2]);
	void SetTexScales(vec_t vTexScales[2]);
	void GetTexScales(vec_t vTexScales[2]);
	vec_t GetTexRotate();
	void SetTexRotate(vec_t pRot);

	CString GetTextureName();
	void SetTextureName(const char* szName, bool bEvent=true);
	virtual CTexture* GetTexturePtr();
	/////////

	void UpdateObjectCounts();
	void AdjustViews();
	void AdjustView(CMapView* pView);
	void CenterViewOnSelection(CMapView* pView);
	void CenterViewsOnSelection();
	void CenterViewOnUIObject(CMapView* pView, CBaseObject* pObject);
	void CenterViewsOnUIObject(CBaseObject* pObject);
	void SetSelectedObjectsTexture(CString sTexture);
	void SetSelectedObjectsVisgroup(CString sName, CString sCategory);
	void CenterViews(vec3_t vPoint, int nViewFilter=ALL_VIEWS);
	bool AddObjectToMap(CBaseObject* pObject);
	bool AddObjectToSelection(CBaseObject* pObject);
	bool AddObjectToSurfaceSelection(CBaseObject* pObject);
	bool RemoveObjectFromSurfaceSelection(CBaseObject* pObject);
	bool ClearSurfaceSelection();
	bool RemoveObjectFromMap(CBaseObject* pObject);
	bool ClearSelection();
	bool DeleteSelection();
	bool DuplicateSelection();
	bool IsInterfaceSelected(int nInterface=I_ALL);
	int GetSelectedInterfaceCount(int nInterface=I_ALL);
	int GetSelectedObjectCount(int nObjectClassFilter=OBJECT_CLASS_ALL);
	void BuildSelectionBBox();
	CLinkedList<CBaseObject>* GetObjectList() { return m_pObjectList; }
	CLinkedList<CBaseObject>* GetSelectedObjectList() { return m_pSelectedObjectList; }
	CLinkedList<CBaseObject>* GetUIObjectList() { return m_pUIList; }
	CLinkedList<SURFACEOBJREF>* GetSurfaceObjList() { return m_pSurfaceObjList; }
	CVisgroupAPI* GetVisgroupAPI() { return &m_VisgroupAPI; }
	CTargetAPI* GetTargetAPI() { return &m_TargetAPI; }
	CUndoAPI* GetUndoAPI() { return &m_UndoAPI; }
	void UpdateWindows(int nUpdateFlags=ALL_VIEWS);
	void UpdateWindowsSelection(int nUpdateFlags=ALL_VIEWS);
	void SetWindowCursors(int nCursor, int nViews=FLAT_VIEWS);
	CEntity* GetWorldspawn() { return m_pWorldspawn; }
	void SetWorldspawn(CEntity* pEntity) { m_pWorldspawn = pEntity; }
	int GetSelectedBrushCount() { return m_nNumSelBrushes; }
	int GetSelectedEntityCount() { return m_nNumSelEnts; }
	void AddTextureToMRU(CTexture* pTexture);
	CLinkedList<TEXMRUREF>* GetTextureMRU() { return m_pTextureMRU; }
	TEXMRUREF* FindMRURef(CTexture* pTexture);

	bool ExportMap(CString sExportName);

	void AddDrawFilters(int nFlags);
	void RemoveDrawFilters(int nFlags);
	void AddGlobalRenderFlags(int nFlags);
	void RemoveGlobalRenderFlags(int nFlags);
	void AddClassSelectionFilters(int nFlags);
	void RemoveClassSelectionFilters(int nFlags);
	int  GetClassSelectionFilters() { return m_nClassSelectionFilter; }

	int GetGlobalRenderFlags() { return m_nGlobalRenderFlags; }
	int GetDrawFilters() { return m_nClassDrawFilter; }
	
	int GetNumRunStates() { return m_nNumStates; }
	RUNSTATE* GetRunState(int nNum);
	RUNSTATE* GetRunState(CString sName);
	bool SaveRunState(CString sName, int nValue, CString sCmdLine);
	int GetRunStateValue(CString sName);
	CString GetRunStateCmdLine(CString sName);

	// Runs the map based on the runstates.
	bool RunMap();

	void AddViewDrawFilters(int nViewFlags, int nFlags);
	void RemoveViewDrawFilters(int nViewFlags, int nFlags);
	
	void AddViewGlobalRenderFlags(int nViewFlags, int nFlags);
	void RemoveViewGlobalRenderFlags(int nViewFlags, int nFlags);

	void ToggleViewRenderFlag(int nViewFlags, int nFlag);
	void ToggleViewDrawFilter(int nViewFlags, int nFlag);

	void ToggleRenderFlag(int nFlag);
	void ToggleViewFilter(int nFlag);

	void AddIALFlags(int nFlags) { m_nIALF |= nFlags; }
	void RemoveIALFlags(int nFlags) { m_nIALF &= ~nFlags; }
	int  GetIALFlags() { return m_nIALF; }

	CMode* GetModePtr();
	void InvokeAction(int nAction, void* pParam=NULL);
	void OnModeChangeEvent(int nOldMode, int nOldSubmode, int nNewMode, int nNewSubmode);
	void BuildHotSpots(int nViews=FLAT_VIEWS);
	void RebindViews(int nType);
	void UnbindViews(int nType);

	int GetViewIndex(CMapView* pView);
	CMapView* GetViewByIndex(int idx);
	CMapView* GetOtherViewType(int nType);
	CMapView* GetViewByType(int nType);
	void SetupDefaultCameras();

#ifdef _WIN32

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTread3DDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual void OnCloseDocument();
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTread3DDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CTread3DDoc)
	afx_msg void OnBuildModel();
	afx_msg void OnMapProperties();
	afx_msg void OnUpdateBuildModel(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

#else
	virtual ~CTread3DDoc();
	virtual void OnCloseDocument();
	virtual bool OnOpenDocument(LPCTSTR lpszPathName);
	virtual bool OnSaveDocument(LPCTSTR lpszPathName);
	virtual bool OnNewDocument();
	const CString& GetPathName();
	void GetFileTitle(const char* lpszFile, char* lpszTitle, short nLength);
	void SetModifiedFlag(bool bModified=true);
#endif
};


#endif