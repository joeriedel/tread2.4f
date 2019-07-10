#if !defined(AFX_ANCHORDIALOG_H__40A84F42_1E0C_11D4_8E26_00A0CC582962__INCLUDED_)
#define AFX_ANCHORDIALOG_H__40A84F42_1E0C_11D4_8E26_00A0CC582962__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AnchorDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAnchorDialog dialog

class CAnchorDialog : public CDialog
{
// Construction
public:
	CAnchorDialog(CWnd* pParent = NULL);   // standard constructor
	CString GetAnchorName();

// Dialog Data
	//{{AFX_DATA(CAnchorDialog)
	enum { IDD = IDD_DIALOG_ANCHOR };
	CString	m_sAnchor;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAnchorDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAnchorDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonSet();
	afx_msg void OnChangeEditAnchor();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ANCHORDIALOG_H__40A84F42_1E0C_11D4_8E26_00A0CC582962__INCLUDED_)
