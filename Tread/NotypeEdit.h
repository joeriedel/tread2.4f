#if !defined(AFX_NOTYPEEDIT_H__B8C38DA4_19A8_11D3_9084_004005A2D20C__INCLUDED_)
#define AFX_NOTYPEEDIT_H__B8C38DA4_19A8_11D3_9084_004005A2D20C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NotypeEdit.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CNotypeEdit window

class CNotypeEdit : public CEdit
{
// Construction
public:
	CNotypeEdit();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNotypeEdit)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CNotypeEdit();

	// Generated message map functions
protected:
	//{{AFX_MSG(CNotypeEdit)
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NOTYPEEDIT_H__B8C38DA4_19A8_11D3_9084_004005A2D20C__INCLUDED_)
