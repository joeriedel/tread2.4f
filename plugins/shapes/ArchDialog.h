#if !defined(AFX_ARCHDIALOG_H__04310D22_206B_11D4_8587_00A0CC627234__INCLUDED_)
#define AFX_ARCHDIALOG_H__04310D22_206B_11D4_8587_00A0CC627234__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ArchDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CArchDialog dialog

class CArchDialog : public CDialog
{
// Construction
public:
	CArchDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CArchDialog)
	enum { IDD = IDD_ARCH_DIALOG };
	int		m_nDepth;
	int		m_nNumSections;
	int		m_nSize;
	int		m_nWidth;
	int		m_nAxis;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CArchDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CArchDialog)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ARCHDIALOG_H__04310D22_206B_11D4_8587_00A0CC627234__INCLUDED_)
