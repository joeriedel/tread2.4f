#if !defined(AFX_REPORTDIALOG_H__DF979281_7FE4_11D3_B673_004005A2D20C__INCLUDED_)
#define AFX_REPORTDIALOG_H__DF979281_7FE4_11D3_B673_004005A2D20C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ReportDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CReportDialog dialog

class CReportDialog : public CDialog
{
// Construction
public:
	CReportDialog(CWnd* pParent = NULL);   // standard constructor
	void SetCount(unsigned int nCount);
	void SetClasses(unsigned int nClasses);
	void SetEntity(unsigned int nEntity);
	void SetMessage(CString sMessage);
	void SetSeconds(int nSeconds);
	void SetUnique(int nUnique);

// Dialog Data
	//{{AFX_DATA(CReportDialog)
	enum { IDD = IDD_DIALOG_REPORT };
	UINT	m_nClasses;
	UINT	m_nCount;
	UINT	m_nEntity;
	CString	m_sMessage;
	UINT	m_nSeconds;
	UINT	m_nUnique;
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CReportDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CReportDialog)
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REPORTDIALOG_H__DF979281_7FE4_11D3_B673_004005A2D20C__INCLUDED_)
