//{{AFX_INCLUDES()
#include "htmlviewer.h"
//}}AFX_INCLUDES
#if !defined(AFX_QUICKHELPDIALOG_H__EFE8C0C7_BF7A_11D3_BD51_00A0CC582962__INCLUDED_)
#define AFX_QUICKHELPDIALOG_H__EFE8C0C7_BF7A_11D3_BD51_00A0CC582962__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// QuickHelpDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CQuickHelpDialog dialog

#define IDD_DIALOG_QUICKHELP            196

class CQuickHelpDialog : public CDialog
{
private:
	CString m_sFile;

	CPoint m_ptMin;
	CPoint m_ptAnchor;
	CRect m_rcClient;
	CRect m_rcSize;
	bool m_bInit;
	CRect m_rcNewSize;
//	WINDOWPLACEMENT m_wndPlace;

// Construction
public:
	CQuickHelpDialog(CWnd* pParent = NULL);   // standard constructor
	void SetHtmlFile(CString sFile);

// Dialog Data
	//{{AFX_DATA(CQuickHelpDialog)
	enum { IDD = IDD_DIALOG_QUICKHELP };
	CHtmlViewer	m_HtmlViewer;
	CButton m_btBack;
	CButton m_btForward;
	CButton m_btHome;
	CButton m_btClose;
	CButton m_btSite;
	CProgressCtrl m_pgDownload;
	CString m_sStatus;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CQuickHelpDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CQuickHelpDialog)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDownloadBeginExplorer();
	afx_msg void OnDownloadCompleteExplorer();
	afx_msg void OnProgressChangeExplorer(long Progress, long ProgressMax);
	afx_msg void OnBeforeNavigate2Explorer1(LPDISPATCH pDisp, VARIANT FAR* URL, VARIANT FAR* Flags, VARIANT FAR* TargetFrameName, VARIANT FAR* PostData, VARIANT FAR* Headers, BOOL FAR* Cancel);
	virtual BOOL OnInitDialog();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnButtonBack();
	afx_msg void OnButtonForward();
	afx_msg void OnButtonHome();
	afx_msg void OnButtonClose();
	afx_msg void OnButtonSite();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_QUICKHELPDIALOG_H__EFE8C0C7_BF7A_11D3_BD51_00A0CC582962__INCLUDED_)
