#if !defined(AFX_KEYDIALOG_H__EB6BEC63_801A_11D3_B673_004005A2D20C__INCLUDED_)
#define AFX_KEYDIALOG_H__EB6BEC63_801A_11D3_B673_004005A2D20C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// KeyDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CKeyDialog dialog

class CKeyDialog : public CDialog
{
// Construction
private:
	CString sBase;
public:
	CKeyDialog(CWnd* pParent = NULL);   // standard constructor
	CString GetString();

// Dialog Data
	//{{AFX_DATA(CKeyDialog)
	enum { IDD = IDD_DIALOG_MODIFIER };
	CListBox	m_ltSelect;
	CString	m_sDescribe;
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	CString	m_sName[4];
	CString m_sParam[4];
	CButton m_btDefault[4];
	CString m_sKey;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CKeyDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CKeyDialog)
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeListSelect();
	afx_msg void OnButtonFinish();
	afx_msg void OnButtonDefualt1();
	afx_msg void OnButtonDefualt2();
	afx_msg void OnButtonDefualt3();
	afx_msg void OnButtonDefualt4();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_KEYDIALOG_H__EB6BEC63_801A_11D3_B673_004005A2D20C__INCLUDED_)
