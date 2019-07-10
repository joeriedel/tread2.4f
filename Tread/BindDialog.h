#if !defined(AFX_BINDDIALOG_H__3DE76E21_6D1E_11D3_B673_004005A2D20C__INCLUDED_)
#define AFX_BINDDIALOG_H__3DE76E21_6D1E_11D3_B673_004005A2D20C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BindDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBindDialog dialog
#include "SelListCtrl.h"
#include "FilterHotKeyCtrl.h"

class CBindDialog : public CDialog
{
// Construction
private:
	//void InsertItem(CString sKey, CString sText, UINT nID, int nItem);
	void InsertItem(UINT nID, CString sText, CString sDefault, int nItem);
	bool VtoS(short nKey, CString &sTemp);
	int GetOldKeys(int nID, CString &sBuffer);
	
	int m_nPos;
	int m_nCount;
	int m_nOldCount;
	ACCEL* m_pTable;
	ACCEL* m_pOldTable;

public:
	CBindDialog(CWnd* pParent = NULL);   // standard constructor

	void SetTableHandle(ACCEL* pHandle, int nCount);
	int GetTableCount();
	ACCEL* GetTableRef();

// Dialog Data
	//{{AFX_DATA(CBindDialog)
	enum { IDD = IDD_DIALOG_KEYS };
	CButton	m_btUnbindAll;
	CButton	m_ctClear;
	CButton	m_ctSet;
	CSelListCtrl m_ctList;
	CFilterHotKeyCtrl	m_ctHotkey;
	CString	m_sOutput;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBindDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CBindDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnClickListKeys(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonSetkey();
	afx_msg void OnButtonClearkey();
	afx_msg void OnButtonUnbindall();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BINDDIALOG_H__3DE76E21_6D1E_11D3_B673_004005A2D20C__INCLUDED_)
