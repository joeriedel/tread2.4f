#if !defined(AFX_REGISTERDIALOG_H__E5779885_885E_11D3_B673_00A0CC582962__INCLUDED_)
#define AFX_REGISTERDIALOG_H__E5779885_885E_11D3_B673_00A0CC582962__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RegisterDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRegisterDialog dialog

#include "ClientSocket.h"
#include "FilterEdit.h"

class CRegisterDialog : public CDialog
{
private:
	CClientSocket* m_pClient;
	bool m_bRegister;

	void CreateKeyFile(CString sKey);
	CString m_sAddress;
	int m_nPort;

// Construction
public:
	CRegisterDialog(CWnd* pParent = NULL);   // standard constructor
	void ProcessPendingRead();
	CMsg* ReadMsg(CClientSocket* pSocket);
	void SendMsg(CClientSocket* pSocket, CMsg* pMsg);

	bool IsRegistered() { return m_bRegister; }
	
// Dialog Data
	//{{AFX_DATA(CRegisterDialog)
	enum { IDD = IDD_DIALOG_REGISTER };
	CFilterEdit	m_edKey;
	CListBox	m_lbMessage;
	CProgressCtrl	m_pgConnect;
	CButton	m_btDone;
	CButton	m_btConnect;
	CString	m_sKey;
	CString	m_sName;
	CString	m_sEmail;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRegisterDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CRegisterDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeEditKey();
	afx_msg void OnButtonConnect();
	afx_msg void OnButtonDone();
	afx_msg void OnDestroy();
	afx_msg void OnChangeEditName();
	afx_msg void OnChangeEditEmail();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REGISTERDIALOG_H__E5779885_885E_11D3_B673_00A0CC582962__INCLUDED_)
