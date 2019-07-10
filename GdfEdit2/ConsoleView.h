#if !defined(AFX_CONSOLEVIEW_H__E6785D5A_7D06_11D3_B673_004005A2D20C__INCLUDED_)
#define AFX_CONSOLEVIEW_H__E6785D5A_7D06_11D3_B673_004005A2D20C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ConsoleView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CConsoleView view


#include <afxcview.h>

class CChildFrame;

#define INFO_MSG	1
#define WARNING_MSG	2
#define ERROR_MSG	3

class CConsoleView : public CListView
{
protected:
	CConsoleView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CConsoleView)

	CImageList m_imgList;
	CChildFrame* m_pOwnerFrame;
	CListCtrl* m_ListCtrl;
	int m_nLast;
	int m_nWarning;
	int m_nError;

// Attributes
public:
	void SetOwnerFrame(CChildFrame* pOwnerFrame);
	void AddMessage(CString &sMessage, UINT nType = INFO_MSG, int nLine = -1);
	void AddMessage(char* pMessage, UINT nType = INFO_MSG, int nLine = -1);
	void ResetConsole();
	void PrintResults();

	int GetErrors();
	int GetWarnings();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CConsoleView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CConsoleView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CConsoleView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDblclk(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONSOLEVIEW_H__E6785D5A_7D06_11D3_B673_004005A2D20C__INCLUDED_)
