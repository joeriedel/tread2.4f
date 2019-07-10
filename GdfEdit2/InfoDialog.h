#if !defined(AFX_INFODIALOG_H__72437121_4837_11D3_9085_004005A2D20C__INCLUDED_)
#define AFX_INFODIALOG_H__72437121_4837_11D3_9085_004005A2D20C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InfoDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CInfoDialog dialog

class CInfoDialog : public CDialog
{
// Construction
public:
	CInfoDialog(CWnd* pParent = NULL);   // standard constructor
	void SetCount(unsigned int nCount);
	void SetClasses(unsigned int nClasses);
	void SetEntity(unsigned int nEntity);
	void SetMessage(CString sMessage);
	void SetSeconds(int nSeconds);
	void SetUnique(int nUnique);

// Dialog Data
	//{{AFX_DATA(CInfoDialog)
	enum { IDD = IDD_DIALOG_FINISH };
	UINT	m_nClasses;
	UINT	m_nCount;
	UINT	m_nEntity;
	CString	m_sMessage;
	UINT	m_nSeconds;
	UINT	m_nUnique;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInfoDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CInfoDialog)
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnDestroy();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INFODIALOG_H__72437121_4837_11D3_9085_004005A2D20C__INCLUDED_)
