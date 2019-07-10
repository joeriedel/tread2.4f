#if !defined(AFX_STARTUPDIALOG_H__4ADF04A1_2135_11D4_8587_00A0CC627234__INCLUDED_)
#define AFX_STARTUPDIALOG_H__4ADF04A1_2135_11D4_8587_00A0CC627234__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StartupDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CStartupDialog dialog

class CStartupDialog : public CDialog
{
// Construction
public:
	CStartupDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CStartupDialog)
	enum { IDD = IDD_STARTUP_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStartupDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CStartupDialog)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STARTUPDIALOG_H__4ADF04A1_2135_11D4_8587_00A0CC627234__INCLUDED_)
