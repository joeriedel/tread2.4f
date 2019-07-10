// FolderDialog.h: interface for the CFolderDialog class.
//	$Copyright © 1998, Kenneth M. Reed, All Rights Reserved. $
//	$Header: FolderDialog.h  Revision:1.11  Mon Apr 06 12:04:50 1998  KenReed $

#if !defined(AFX_FOLDERDIALOG_H__56F34711_AACC_11D2_9B53_004005A2D20C__INCLUDED_)
#define AFX_FOLDERDIALOG_H__56F34711_AACC_11D2_9B53_004005A2D20C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FolderDialog.h : header file
//
//#include <shlobj.h>
#include "CursorListBox.h"
#include "drivetreectrl.h"

class CFolderDialog : public CDialog
{
private:
	__int64 m_nSize;
	CImageList m_imgDrives;
	void FollowSub(CString path, __int64 &size, int &file, int &dir, WIN32_FIND_DATA &fd);
// Construction
public:
	CFolderDialog(CWnd* pParent = NULL);   // standard constructor
		

// Dialog Data
	//{{AFX_DATA(CFolderDialog)
	enum { IDD = IDD_DIALOG_FILEVIEW };
	CSpinButtonCtrl	m_spSize;
	CSpinButtonCtrl	m_spIndex;
	CProgressCtrl	m_prgUsed;
	CDriveTreeCtrl	m_ctTree;
	CString	m_sPath;
	CString	m_sDir;
	CString	m_sFile;
	CString	m_sSubFile;
	CString	m_sTotal;
	CString	m_sPercent;
	CString	m_sSize;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFolderDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFolderDialog)
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnDestroy();
	virtual BOOL OnInitDialog();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnSelchangedTreeFileview(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FOLDERDIALOG_H__56F34711_AACC_11D2_9B53_004005A2D20C__INCLUDED_)



