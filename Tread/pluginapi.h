// PLUGINAPI.H
// Copyright © 2000 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#ifndef PLUGINAPI_H
#define PLUGINAPI_H

#include "LinkedList.h"
#include "TreadSys.h"
#include "Tread3DDoc.h"
#include "PakFile.h"
#include "RenderAPI.h"
#include "TreadMapAPI.h"

// $sb "pluginapi.h block1"
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

// $eb

CTreadPluginAPI* GetPluginAPI();

#endif