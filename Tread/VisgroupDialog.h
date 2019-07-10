#if !defined(AFX_VISGROUPDIALOG_H__BBE1DB81_B7D3_11D3_BD51_00A0CC582962__INCLUDED_)
#define AFX_VISGROUPDIALOG_H__BBE1DB81_B7D3_11D3_BD51_00A0CC582962__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// VisgroupDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CVisgroupDialog dialog

#include "HighlightComboBox.h"

class CVisgroupDialog : public CDialog
{
private:
	bool m_bFound1;
	bool m_bFound2;

// Construction
public:
	CVisgroupDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CVisgroupDialog)
	enum { IDD = IDD_DIALOG_VISGROUP };
	CHighlightComboBox	m_cbVisgroup;
	CHighlightComboBox	m_cbCategory;
	CButton	m_btAdd;
	CString	m_sCategory;
	CString	m_sVisgroup;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVisgroupDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CVisgroupDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnEditchangeComboboxCategory();
	afx_msg void OnSelchangeComboboxCategory();
	afx_msg void OnEditchangeComboboxVisgroup();
	afx_msg void OnSelchangeComboboxVisgroup();
	afx_msg void OnButtonOk();
	afx_msg void OnButtonCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VISGROUPDIALOG_H__BBE1DB81_B7D3_11D3_BD51_00A0CC582962__INCLUDED_)
