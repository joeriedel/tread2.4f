#if !defined(AFX_STRINGDIALOG_H__216872E4_8E1F_11D3_B08D_00A0CC582962__INCLUDED_)
#define AFX_STRINGDIALOG_H__216872E4_8E1F_11D3_B08D_00A0CC582962__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StringDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CStringDialog dialog
#include "FilterEdit.h"

class CStringDialog : public CDialog
{
// Construction
public:
	CStringDialog(CWnd* pParent = NULL);   // standard constructor
	void SetInput(int nInput);
	void SetInput(CString sInput);

	CString GetInput();

	void SetFilter(int nFilter);

// Dialog Data
	//{{AFX_DATA(CStringDialog)
	enum { IDD = IDD_ENTITY_STRING };
	CFilterEdit	m_edFilter;
	CString	m_sInput;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStringDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CStringDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonDone();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STRINGDIALOG_H__216872E4_8E1F_11D3_B08D_00A0CC582962__INCLUDED_)
