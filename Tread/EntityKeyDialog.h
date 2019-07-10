#if !defined(AFX_ENTITYKEYDIALOG_H__CA7F19C1_7BA3_11D4_81AC_00A0CC582962__INCLUDED_)
#define AFX_ENTITYKEYDIALOG_H__CA7F19C1_7BA3_11D4_81AC_00A0CC582962__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EntityKeyDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CEntityKeyDialog dialog

class CEntityKeyDialog : public CDialog
{
// Construction
public:
	CEntityKeyDialog(CWnd* pParent = NULL);   // standard constructor

	CString GetKeyName();
	CString GetKeyValue();
	void SetKeyValues(CString sName, CString sValue);

private:
	UINT m_nType;

public:

// Dialog Data
	//{{AFX_DATA(CEntityKeyDialog)
	enum { IDD = IDD_DIALOG_ENTITYKEY };
	CButton	m_btOK;
	CEdit	m_edValue;
	CEdit	m_edName;
	CString	m_sName;
	CString	m_sValue;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEntityKeyDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CEntityKeyDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeEditName();
	afx_msg void OnButtonOk();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ENTITYKEYDIALOG_H__CA7F19C1_7BA3_11D4_81AC_00A0CC582962__INCLUDED_)
