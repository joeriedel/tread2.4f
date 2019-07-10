#if !defined(AFX_CONSOLEDIALOG_H__7DA6ADA5_1670_11D3_9084_004005A2D20C__INCLUDED_)
#define AFX_CONSOLEDIALOG_H__7DA6ADA5_1670_11D3_9084_004005A2D20C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ConsoleDialog.h : header file
//

//#include "NotypeEdit.h"
#include "ConsoleEdit.h"
/////////////////////////////////////////////////////////////////////////////
// CConsoleDialog dialog

class CConsoleDialog : public CDialog
{
// Construction
private:
	CPoint m_ptAnchor;
	bool bInit;

public:
	CConsoleDialog(CWnd* pParent = NULL);   // standard constructor
	void AppendText(CString sInput);

// Dialog Data
	//{{AFX_DATA(CConsoleDialog)
	//enum { IDD = IDD_DIALOG_CONSOLE };
	CButton	m_btSave;
	CButton	m_btClear;
	CConsoleEdit	m_ctConsole;
	BOOL	m_bOntop;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CConsoleDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CConsoleDialog)
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnButtonClear();
	afx_msg void OnButtonSave();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONSOLEDIALOG_H__7DA6ADA5_1670_11D3_9084_004005A2D20C__INCLUDED_)
