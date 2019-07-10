#if !defined(AFX_CONSOLEVIEW_H__417ADD66_15A3_11D3_9084_004005A2D20C__INCLUDED_)
#define AFX_CONSOLEVIEW_H__417ADD66_15A3_11D3_9084_004005A2D20C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ConsoleView.h : header file
//
#include <afxrich.h>

/////////////////////////////////////////////////////////////////////////////
// CConsoleView view

#define DF_FONT "Arial"
#define DF_SIZE 170
#define DF_COLOR 0

class CConsoleView : public CRichEditView
{
protected:
	CConsoleView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CConsoleView)

// Attributes
public:
	void ScrollToLine(int nLine);
	void AppendText( const CString& sInput, UINT nModifier = 0);
	void ResetContent();

	void SetFontType(CString sFont);
	void SetFontColor(COLORREF clText);
	void SetFontSize(int nSize);
	void ResetFont();
	void SetFont(CString sFont, int nSize, COLORREF clFont);

private:
	bool FindLine(int nLine, int &nPos, int nGap);

	CHARFORMAT2 chFormat;
	PARAFORMAT2 prFormat;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CConsoleView)
	public:
	virtual BOOL OnDrop(COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point);
	virtual DROPEFFECT OnDragOver(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);
	virtual DROPEFFECT OnDragEnter(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);
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
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnConsoleClear();
	afx_msg void OnConsoleDump();
	afx_msg void OnConsoleInfo();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONSOLEVIEW_H__417ADD66_15A3_11D3_9084_004005A2D20C__INCLUDED_)
