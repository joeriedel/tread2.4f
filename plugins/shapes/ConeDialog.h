#if !defined(AFX_CONEDIALOG_H__04310D23_206B_11D4_8587_00A0CC627234__INCLUDED_)
#define AFX_CONEDIALOG_H__04310D23_206B_11D4_8587_00A0CC627234__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ConeDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CConeDialog dialog

class CConeDialog : public CDialog
{
// Construction
public:
	CConeDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CConeDialog)
	enum { IDD = IDD_CONE_DIALOG };
	int		m_nSize;
	int		m_nAxis;
	int		m_nLength;
	int		m_nNumSides;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CConeDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CConeDialog)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONEDIALOG_H__04310D23_206B_11D4_8587_00A0CC627234__INCLUDED_)
