#if !defined(AFX_BUILDDIALOG_H__37285844_98E5_11D3_BD51_00A0CC582962__INCLUDED_)
#define AFX_BUILDDIALOG_H__37285844_98E5_11D3_BD51_00A0CC582962__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BuildDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBuildDialog dialog

class CBuildDialog : public CDialog
{
private:
	bool m_bMode;	//TRUE = FLAGS, FALSE = OPTIONS
	CImageList m_imgList;
	int m_nValue;
	void ClearFlags();
	int m_nLastPos;

// Construction
public:
	CBuildDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CBuildDialog)
	enum { IDD = IDD_DIALOG_BUILD };
	CListBox	m_lbOption;
	CEdit	m_edCommand;
	CListCtrl	m_lcSettings;
	CButton	m_btBuild;
	CString	m_sCommand;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBuildDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CBuildDialog)
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonBuild();
	afx_msg void OnClickListSelection(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonCancel();
	afx_msg void OnSelchangeListOptoin();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BUILDDIALOG_H__37285844_98E5_11D3_BD51_00A0CC582962__INCLUDED_)
