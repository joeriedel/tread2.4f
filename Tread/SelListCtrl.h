#if !defined(AFX_SELLISTCTRL_H__B39D4515_E36B_11D2_9084_004005A2D20C__INCLUDED_)
#define AFX_SELLISTCTRL_H__B39D4515_E36B_11D2_9084_004005A2D20C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SelListCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSelListCtrl window

class CSelListCtrl : public CListCtrl
{
private:
	int nItem;
// Construction
public:
	CSelListCtrl();

// Attributes
public:

// Operations
public:
	int GetSelection();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSelListCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSelListCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CSelListCtrl)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SELLISTCTRL_H__B39D4515_E36B_11D2_9084_004005A2D20C__INCLUDED_)
