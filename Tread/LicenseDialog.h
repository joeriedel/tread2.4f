#if !defined(AFX_LICENSEDIALOG_H__5F8A86C1_2119_11D4_8E26_00A0CC582962__INCLUDED_)
#define AFX_LICENSEDIALOG_H__5F8A86C1_2119_11D4_8E26_00A0CC582962__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LicenseDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CLicenseDialog dialog

class CLicenseDialog : public CDialog
{
// Construction
private:
	bool m_bHide;

public:
	CLicenseDialog(CWnd* pParent = NULL);   // standard constructor
	void HideCancelButton(bool bHide);

// Dialog Data
	//{{AFX_DATA(CLicenseDialog)
	enum { IDD = IDD_DIALOG_AGREEMENT };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLicenseDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CLicenseDialog)
	afx_msg void OnButtonCancel();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LICENSEDIALOG_H__5F8A86C1_2119_11D4_8E26_00A0CC582962__INCLUDED_)
