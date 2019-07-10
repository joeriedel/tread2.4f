#if !defined(AFX_FILTERHOTKEYCTRL_H__93C92004_6F43_11D3_B673_004005A2D20C__INCLUDED_)
#define AFX_FILTERHOTKEYCTRL_H__93C92004_6F43_11D3_B673_004005A2D20C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FilterHotKeyCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFilterHotKeyCtrl window

class CFilterHotKeyCtrl : public CHotKeyCtrl
{
// Construction
public:
	CFilterHotKeyCtrl();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFilterHotKeyCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CFilterHotKeyCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CFilterHotKeyCtrl)
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FILTERHOTKEYCTRL_H__93C92004_6F43_11D3_B673_004005A2D20C__INCLUDED_)
