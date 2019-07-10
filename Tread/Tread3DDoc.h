// Tread3DDoc.h : interface of the CTread3DDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_TREAD3DDOC_H__E08CCCED_1023_11D3_9084_004005A2D20C__INCLUDED_)
#define AFX_TREAD3DDOC_H__E08CCCED_1023_11D3_9084_004005A2D20C__INCLUDED_

#if _WIN32
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#endif

#include "LinkedList.h"
#include "MapObject.h"
#include "Visgroup.h"
#include "TreadSys.h"
#include "Mode.h"
#include "EventManager.h"
#include "TreadMapAPI.h"
#include "Entity.h"
#include "TargetAPI.h"
#include "UndoAPI.h"

class CRenderAPI;
class CMapView;

// $sb "tread3ddoc.h block1"
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

// $eb

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TREAD3DDOC_H__E08CCCED_1023_11D3_9084_004005A2D20C__INCLUDED_)
