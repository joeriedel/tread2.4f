#if !defined(AFX_MAPDIALOG_H__699419A2_BD60_11D3_BD51_00A0CC582962__INCLUDED_)
#define AFX_MAPDIALOG_H__699419A2_BD60_11D3_BD51_00A0CC582962__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MapDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMapDialog dialog

class CMapDialog : public CDialog
{
// Construction
public:
	CMapDialog(CWnd* pParent = NULL);   // standard constructor
	void SetStrings(CString sAuthor, CString sComments);
	CString GetAuthor();
	CString GetComment();

// Dialog Data
	//{{AFX_DATA(CMapDialog)
	enum { IDD = IDD_DIALOG_MAP };
	CString	m_sAuthor;
	CString	m_sComments;
	CString	m_sBrush;
	CString	m_sPointEntity;
	CString	m_sSolidEntity;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMapDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMapDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonSave();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAPDIALOG_H__699419A2_BD60_11D3_BD51_00A0CC582962__INCLUDED_)
