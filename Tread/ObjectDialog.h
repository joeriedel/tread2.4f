#if !defined(AFX_OBJECTDIALOG_H__36A0350B_2A22_11D3_9084_004005A2D20C__INCLUDED_)
#define AFX_OBJECTDIALOG_H__36A0350B_2A22_11D3_9084_004005A2D20C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ObjectDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CObjectDialog dialog

class CObjectDialog : public CDialog
{
// Construction
public:
	CObjectDialog(CWnd* pParent = NULL);   // standard constructor
	void SetLine1(CString sLine);
	void SetLine2(CString sLine);
	void AddItem1(CString sItem);
	void AddItem2(CString sItem);
	CString GetItem1();
	CString GetItem2();

// Dialog Data
	//{{AFX_DATA(CObjectDialog)
	enum { IDD = IDD_DIALOG_OBJECT };
	CComboBox	m_cbItem2;
	CComboBox	m_cbItem1;
	CString	m_sLine1;
	CString	m_sLine2;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CObjectDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CObjectDialog)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OBJECTDIALOG_H__36A0350B_2A22_11D3_9084_004005A2D20C__INCLUDED_)
