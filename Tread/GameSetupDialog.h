#if !defined(AFX_GAMESETUPDIALOG_H__BC59C121_83BE_11D3_B673_004005A2D20C__INCLUDED_)
#define AFX_GAMESETUPDIALOG_H__BC59C121_83BE_11D3_B673_004005A2D20C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GameSetupDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CGameSetupDialog dialog
#include "DriveTreeCtrl.h"

#include "gameapi.h"

class CGameSetupDialog : public CDialog
{
private:
	CImageList m_imgList1;
	CImageList m_imgList2;
	CString m_sPath;
	CString m_sFile;
	bool m_bSlash;
	bool m_bFile;

	CLinkedList<GAMEPAKFILE>* m_pPakList;
	CString m_sGame;
	int m_nMode;
	int m_nSubMode;
	//int m_nItem;

// Construction
public:
	CGameSetupDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CGameSetupDialog)
	enum { IDD = IDD_DIALOG_GAMESETUP };
	CListBox	m_lbFiles;
	CButton	m_btRemove;
	CButton	m_btAdd;
	CListCtrl	m_lcPaths;
	CListBox	m_lbGames;
	CDriveTreeCtrl	m_tcPath;
	CTabCtrl	m_tabMode;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGameSetupDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CGameSetupDialog)
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnButtonAdd();
	afx_msg void OnButtonRemove();
	afx_msg void OnSelchangeListGames();
	afx_msg void OnSelchangeTabMode(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangedTreeDirectory(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangeListDir();
	afx_msg void OnClickListPaths(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonDone();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GAMESETUPDIALOG_H__BC59C121_83BE_11D3_B673_004005A2D20C__INCLUDED_)
