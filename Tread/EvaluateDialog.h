#if !defined(AFX_EVALUATEDIALOG_H__E5779884_885E_11D3_B673_00A0CC582962__INCLUDED_)
#define AFX_EVALUATEDIALOG_H__E5779884_885E_11D3_B673_00A0CC582962__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EvaluateDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CEvaluateDialog dialog
#include <cj60lib.h>

class CEvaluateDialog : public CDialog
{
private:
	bool Evaluate();
// Construction
public:
	CEvaluateDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CEvaluateDialog)
	enum { IDD = IDD_DIALOG_LICENSE };
	CButton	m_btRegister;
	CButton	m_btEvaluate;
	CHyperLink	m_stUrl;
	CString	m_sDays;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEvaluateDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CEvaluateDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonEvaluate();
	afx_msg void OnButtonRegister();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EVALUATEDIALOG_H__E5779884_885E_11D3_B673_00A0CC582962__INCLUDED_)
