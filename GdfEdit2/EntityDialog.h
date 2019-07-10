#if !defined(AFX_ENTITYDIALOG_H__EB6BEC62_801A_11D3_B673_004005A2D20C__INCLUDED_)
#define AFX_ENTITYDIALOG_H__EB6BEC62_801A_11D3_B673_004005A2D20C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EntityDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CEntityDialog dialog

class CEntityDialog : public CDialog
{
// Construction
private:
	CString sBase;
public:
	CEntityDialog(CWnd* pParent = NULL);   // standard constructor
	void SetBase(CString sName);
	CString GetString();

// Dialog Data
	//{{AFX_DATA(CEntityDialog)
	enum { IDD = IDD_DIALOG_ENTITY };
	CListBox	m_ltSelect;
	CString	m_sDescribe;
	//}}AFX_DATA

	CString	m_sName[4];
	CString m_sParam[4];
	CButton m_btDefault[4];
	CString m_sEntity;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEntityDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CEntityDialog)
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

#endif // !defined(AFX_ENTITYDIALOG_H__EB6BEC62_801A_11D3_B673_004005A2D20C__INCLUDED_)
