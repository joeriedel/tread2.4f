#if !defined(AFX_SCRIPTDIALOG_H__35A05104_BEEA_11D3_BD51_00A0CC582962__INCLUDED_)
#define AFX_SCRIPTDIALOG_H__35A05104_BEEA_11D3_BD51_00A0CC582962__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ScriptDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CScriptDialog dialog

class CScriptDialog : public CDialog
{
// Construction
public:
	CScriptDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CScriptDialog)
	enum { IDD = IDD_DIALOG_SCRIPTS };
	CListCtrl	m_lcScripts;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CScriptDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CScriptDialog)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCRIPTDIALOG_H__35A05104_BEEA_11D3_BD51_00A0CC582962__INCLUDED_)
