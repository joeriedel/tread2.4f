// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__E08CCCE9_1023_11D3_9084_004005A2D20C__INCLUDED_)
#define AFX_MAINFRM_H__E08CCCE9_1023_11D3_9084_004005A2D20C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <cj60lib.h>
#include "BrowserDialog.h"
#include "CtrlStatusBar.h"

// $sb "mainfrm.h block1"

class CPopupMenu : public CMenu
{
public:
	CPopupMenu* next;
	CPopupMenu* prev;
	CString m_sHandle;
};

class CMainFrame : public CCJMDIFrameWnd
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();
	bool InitializeDockingWindows();
	void DockControlBarLeftOf(CToolBar* Bar,CToolBar* LeftOf);

public:	//Interface flags
	bool m_bUpdateMenu;
	bool m_bMapOpen;
	bool m_bEntitySelect;
	bool m_bBrushSelect;
	bool m_bBuildMap;
	bool m_bAccel;
	bool m_bUpdatePointer;
	bool m_bUpdateClip;
	void SetComboGrid(int nGrid);
	void SetComboZoom(float fZoom);
	void BuildPluginMenu(bool bDelete = false);

private:
	
	CLinkedList<CPopupMenu> m_MenuList;

	CCJTabCtrlBar m_wndWorkspace;
	CCJTabCtrlBar m_wndTexture;
	CImageList m_TabImages;
	CImageList m_TabImages2;

	CCtrlStatusBar m_wndStatusBar;
	CCJToolBar m_wndToolBar;
	CCJToolBar m_wndModeBar;
	CCJToolBar m_wndViewBar;
	CCJToolBar m_wndObjectBar;
	CCJToolBar m_wndAlignBar;

	CCoolMenuManager	m_menuManager;
	CDockState m_dockState;

	ACCEL* m_pAccelTable;
	int m_nAccelCount;
	int m_nPointerImage;
	int m_nClipImage;
	bool m_bModifier;
	bool m_bFaceDialog;

	CComboBox* m_pGridBox;
	CComboBox* m_pZoomBox;
	CProgressCtrl* m_pProgress;

	bool UpdateTreadMenu();
	CString VtoS(int nKey);
	CPopupMenu* GetPopupMenu(CString sName);
	
// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	
	
// Generated message map functions
protected:
	afx_msg void OnInitMenu(CMenu* pMenu);
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnClose();
	afx_msg void OnUpdateViewWorkspace(CCmdUI* pCmdUI);
	afx_msg void OnViewWorkspace();
	afx_msg void OnModePointer();
	afx_msg void OnUpdateModePointer(CCmdUI* pCmdUI);
	afx_msg void OnModeCamera();
	afx_msg void OnUpdateModeCamera(CCmdUI* pCmdUI);
	afx_msg void OnModeZoom();
	afx_msg void OnUpdateModeZoom(CCmdUI* pCmdUI);
	afx_msg void OnModePaint();
	afx_msg void OnUpdateModePaint(CCmdUI* pCmdUI);
	afx_msg void OnModeBrush();
	afx_msg void OnUpdateModeBrush(CCmdUI* pCmdUI);
	afx_msg void OnModeFace();
	afx_msg void OnUpdateModeFace(CCmdUI* pCmdUI);
	afx_msg void OnModeVertex();
	afx_msg void OnUpdateModeVertex(CCmdUI* pCmdUI);
	afx_msg void OnModeClip();
	afx_msg void OnUpdateModeClip(CCmdUI* pCmdUI);
	afx_msg void OnModeTexlock();
	afx_msg void OnUpdateModeTexlock(CCmdUI* pCmdUI);
	afx_msg void OnRunmap();
	afx_msg void OnUpdateRunmap(CCmdUI* pCmdUI);
	afx_msg void OnModeEntity();
	afx_msg void OnUpdateModeEntity(CCmdUI* pCmdUI);
	afx_msg void OnOptionsKeybinding();
	afx_msg void OnOptionsDebugwindow();
	afx_msg void OnOptionsColoroptions();
	afx_msg void OnObjectLinkEntity();
	afx_msg void OnObjectProperties();
	afx_msg void OnSelchangeGridCombo();
	afx_msg void OnSelchangeZoomCombo();
	afx_msg void OnUpdateGridCombo(CCmdUI* pCmdUI);
	afx_msg void OnUpdateZoomCombo(CCmdUI* pCmdUI);
	afx_msg void OnOptionsGamesetup();
	afx_msg void OnObjectCarve();
	afx_msg void OnUpdateObjectCarve(CCmdUI* pCmdUI);
	afx_msg void OnObjectFlipHorizontal();
	afx_msg void OnUpdateObjectFlipHorizontal(CCmdUI* pCmdUI);
	afx_msg void OnObjectFlipVertical();
	afx_msg void OnUpdateObjectFlipVertical(CCmdUI* pCmdUI);
	afx_msg void OnObjectGroup();
	afx_msg void OnUpdateObjectGroup(CCmdUI* pCmdUI);
	afx_msg void OnObjectHollow();
	afx_msg void OnUpdateObjectHollow(CCmdUI* pCmdUI);
	afx_msg void OnUpdateObjectLinkEntity(CCmdUI* pCmdUI);
	afx_msg void OnUpdateObjectProperties(CCmdUI* pCmdUI);
	afx_msg void OnObjectRotate180Cw();
	afx_msg void OnUpdateObjectRotate180Cw(CCmdUI* pCmdUI);
	afx_msg void OnObjectRotate90Ccw();
	afx_msg void OnUpdateObjectRotate90Ccw(CCmdUI* pCmdUI);
	afx_msg void OnObjectRotate90Cw();
	afx_msg void OnUpdateObjectRotate90Cw(CCmdUI* pCmdUI);
	afx_msg void OnObjectUngroup();
	afx_msg void OnUpdateObjectUngroup(CCmdUI* pCmdUI);
	afx_msg void OnObjectUnlinkEntity();
	afx_msg void OnUpdateObjectUnlinkEntity(CCmdUI* pCmdUI);
	afx_msg void OnViewFrontview();
	afx_msg void OnUpdateViewFrontview(CCmdUI* pCmdUI);
	afx_msg void OnViewSideview();
	afx_msg void OnUpdateViewSideview(CCmdUI* pCmdUI);
	afx_msg void OnViewTopview();
	afx_msg void OnUpdateViewTopview(CCmdUI* pCmdUI);
	afx_msg void OnViewGdi();
	afx_msg void OnUpdateViewGdi(CCmdUI* pCmdUI);
	afx_msg void OnViewOpengl();
	afx_msg void OnUpdateViewOpengl(CCmdUI* pCmdUI);
	afx_msg void On3dLine();
	afx_msg void On3dSolid();
	afx_msg void On3dTextured();
	afx_msg void OnUpdate3dLine(CCmdUI* pCmdUI);
	afx_msg void OnUpdate3dSolid(CCmdUI* pCmdUI);
	afx_msg void OnUpdate3dTextured(CCmdUI* pCmdUI);
	afx_msg void OnMapNoentityfill();
	afx_msg void OnMapNoselboxsize();
	afx_msg void On3dRealtime();
	afx_msg void OnUpdateMapNoentityfill(CCmdUI* pCmdUI);
	afx_msg void OnUpdateMapNoselboxsize(CCmdUI* pCmdUI);
	afx_msg void OnUpdate3dRealtime(CCmdUI* pCmdUI);
	afx_msg void On3dNoselboxoutline();
	afx_msg void OnMapNoentnames();
	afx_msg void On3dNobilinear();
	afx_msg void OnUpdate3dNobilinear(CCmdUI* pCmdUI);
	afx_msg void OnUpdate3dNoselboxoutline(CCmdUI* pCmdUI);
	afx_msg void OnUpdateMapNoentnames(CCmdUI* pCmdUI);
	afx_msg void On3dNodraw();
	afx_msg void OnUpdate3dNodraw(CCmdUI* pCmdUI);
	afx_msg void OnEditCopy();
	afx_msg void OnEditCut();
	afx_msg void OnEditDelete();
	afx_msg void OnEditPaste();
	afx_msg void OnEditUndo();
	afx_msg void OnUpdateEditCopy(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditCut(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditDelete(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditPaste(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditUndo(CCmdUI* pCmdUI);
	afx_msg void OnAddvisgroup();
	afx_msg void OnUpdateAddvisgroup(CCmdUI* pCmdUI);
	afx_msg void OnTreadSetup();
	afx_msg void OnViewWorkspace2();
	afx_msg void OnUpdateViewWorkspace2(CCmdUI* pCmdUI);
	afx_msg void OnToolbarModes();
	afx_msg void OnToolbarObjects();
	afx_msg void OnToolbarView();
	afx_msg void OnUpdateToolbarModes(CCmdUI* pCmdUI);
	afx_msg void OnUpdateToolbarObjects(CCmdUI* pCmdUI);
	afx_msg void OnUpdateToolbarView(CCmdUI* pCmdUI);
	afx_msg void OnViewOpenglsoft();
	afx_msg void OnUpdateViewOpenglsoft(CCmdUI* pCmdUI);
	afx_msg void OnEntityPath();
	afx_msg void OnEntityPathloop();
	afx_msg void OnEntityTargetpath();
	afx_msg void OnUpdateEntityPath(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEntityPathloop(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEntityTargetpath(CCmdUI* pCmdUI);
	afx_msg void OnViewCenter();
	afx_msg void OnUpdateViewCenter(CCmdUI* pCmdUI);
	afx_msg void OnClearMemory();
	afx_msg void OnUpdateClearMemory(CCmdUI* pCmdUI);
	afx_msg void OnUpdateMapProperties(CCmdUI* pCmdUI);
	afx_msg void OnWorldspawn();
	afx_msg void OnUpdateWorldspawn(CCmdUI* pCmdUI);
	afx_msg void OnUpdateBuildModel(CCmdUI* pCmdUI);
	afx_msg void OnScriptInformation();
	afx_msg void OnQuickHelp();
	afx_msg void OnClipAction();
	afx_msg void On3dviewTransparentwater();
	afx_msg void OnUpdate3dviewTransparentwater(CCmdUI* pCmdUI);
	afx_msg void OnEditRedo();
	afx_msg void OnUpdateEditRedo(CCmdUI* pCmdUI);
	afx_msg void OnViewSmartnames();
	afx_msg void OnUpdateViewSmartnames(CCmdUI* pCmdUI);
	afx_msg void OnEntityDeletepath();
	afx_msg void OnUpdateEntityDeletepath(CCmdUI* pCmdUI);
	afx_msg void OnFileImport();
	afx_msg void OnFileExport();
	afx_msg void OnUpdateFileExport(CCmdUI* pCmdUI);
	afx_msg void On3dDrawentities();
	afx_msg void OnUpdate3dDrawentities(CCmdUI* pCmdUI);
	afx_msg void On3dShaded();
	afx_msg void OnUpdate3dShaded(CCmdUI* pCmdUI);
	afx_msg void OnDropAnchor();
	afx_msg void OnGridSnap();
	afx_msg void OnUpdateGridSnap(CCmdUI* pCmdUI);
	afx_msg void OnUpdatePluginMenu(CCmdUI* pCmdUI);
	afx_msg void OnViewBrushHandles();
	afx_msg void OnUpdateViewBrushHandles(CCmdUI* pCmdUI);
	afx_msg void OnViewAnchors();
	afx_msg void OnUpdateViewAnchors(CCmdUI* pCmdUI);
	afx_msg void OnMapDrawpaths();
	afx_msg void OnUpdateMapDrawpaths(CCmdUI* pCmdUI);
	afx_msg void OnViewUpdateallviews();
	afx_msg void OnUpdateViewUpdateallviews(CCmdUI* pCmdUI);
	afx_msg void OnToolbarAlign();
	afx_msg void OnUpdateToolbarAlign(CCmdUI* pCmdUI);
	afx_msg void OnGridLarger();
	afx_msg void OnUpdateGridLarger(CCmdUI* pCmdUI);
	afx_msg void OnUpdateGridSmaller(CCmdUI* pCmdUI);
	afx_msg void OnGridSmaller();
	afx_msg void OnAlignEdgeBottom();
	afx_msg void OnUpdateAlignEdgeBottom(CCmdUI* pCmdUI);
	afx_msg void OnAlignEdgeLeft();
	afx_msg void OnUpdateAlignEdgeLeft(CCmdUI* pCmdUI);
	afx_msg void OnUpdateAlignEdgeRight(CCmdUI* pCmdUI);
	afx_msg void OnAlignEdgeRight();
	afx_msg void OnAlignEdgeTop();
	afx_msg void OnUpdateAlignEdgeTop(CCmdUI* pCmdUI);
	afx_msg void OnAlignSizeBoth();
	afx_msg void OnUpdateAlignSizeBoth(CCmdUI* pCmdUI);
	afx_msg void OnAlignSizeHorizontal();
	afx_msg void OnUpdateAlignSizeHorizontal(CCmdUI* pCmdUI);
	afx_msg void OnAlignSizeVertical();
	afx_msg void OnUpdateAlignSizeVertical(CCmdUI* pCmdUI);
	afx_msg void OnAlignSpaceHorizontal();
	afx_msg void OnUpdateAlignSpaceHorizontal(CCmdUI* pCmdUI);
	afx_msg void OnAlignSpaceVertical();
	afx_msg void OnUpdateAlignSpaceVertical(CCmdUI* pCmdUI);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnViewAnimatedSelection();
	afx_msg void OnUpdateViewAnimatedSelection(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
// $eb
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__E08CCCE9_1023_11D3_9084_004005A2D20C__INCLUDED_)
