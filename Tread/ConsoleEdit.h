#if !defined(AFX_CONSOLEEDIT_H__41F995A2_745E_11D3_B673_004005A2D20C__INCLUDED_)
#define AFX_CONSOLEEDIT_H__41F995A2_745E_11D3_B673_004005A2D20C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ConsoleEdit.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CConsoleEdit window

class CConsoleEdit : public CEdit
{
// Construction
public:
	CConsoleEdit();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CConsoleEdit)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CConsoleEdit();
	void Clear();

	// Generated message map functions
protected:
	//{{AFX_MSG(CConsoleEdit)
	afx_msg void OnMaxtext();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONSOLEEDIT_H__41F995A2_745E_11D3_B673_004005A2D20C__INCLUDED_)
