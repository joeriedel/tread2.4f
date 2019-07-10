// ChildFrm.h : interface of the CMapFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHILDFRM_H__E08CCCEB_1023_11D3_9084_004005A2D20C__INCLUDED_)
#define AFX_CHILDFRM_H__E08CCCEB_1023_11D3_9084_004005A2D20C__INCLUDED_

#if macintosh
#include "mac_childfrm.h"
#else

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MapView.h"
#include "ConsoleView.h"
#include "Tread3DDoc.h"
#include "TExport.h"

class _T_EXPORT CMDIChildWnd;
// $sb "childfrm.h block1"

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
// $eb
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif
#endif // !defined(AFX_CHILDFRM_H__E08CCCEB_1023_11D3_9084_004005A2D20C__INCLUDED_)
