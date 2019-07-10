#if !defined(AFX_MODELDIALOG_H__699419A1_BD60_11D3_BD51_00A0CC582962__INCLUDED_)
#define AFX_MODELDIALOG_H__699419A1_BD60_11D3_BD51_00A0CC582962__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ModelDialog.h : header file
//

#include "FilterEdit.h"

/////////////////////////////////////////////////////////////////////////////
// CModelDialog dialog

class CModelDialog : public CDialog
{
// Construction
public:
	CModelDialog(CWnd* pParent = NULL);   // standard constructor
	CString GetCategory();
	CString GetName();
	int GetType();

// Dialog Data
	//{{AFX_DATA(CModelDialog)
	enum { IDD = IDD_DIALOG_MODEL };
	CFilterEdit	m_edName;
	CFilterEdit	m_edCategory;
	CButton	m_btCreate;
	CString	m_sCategory;
	CString	m_sName;
	int		m_nType;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CModelDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CModelDialog)
	afx_msg void OnButtonCreate();
	afx_msg void OnButtonCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeEditCategory();
	afx_msg void OnChangeEditName();
	afx_msg void OnRadioBrush();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MODELDIALOG_H__699419A1_BD60_11D3_BD51_00A0CC582962__INCLUDED_)
