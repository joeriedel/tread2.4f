#if !defined(AFX_HOLLOWDIALOG_H__FC1AF5E3_B32A_11D3_BD51_00A0CC582962__INCLUDED_)
#define AFX_HOLLOWDIALOG_H__FC1AF5E3_B32A_11D3_BD51_00A0CC582962__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HollowDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CHollowDialog dialog
#include "FilterEdit.h"

class CHollowDialog : public CDialog
{
// Construction
public:
	CHollowDialog(CWnd* pParent = NULL);   // standard constructor
	void SetUnit(float fUnit);
	float GetUnit();

// Dialog Data
	//{{AFX_DATA(CHollowDialog)
	enum { IDD = IDD_DIALOG_HOLLOW };
	CFilterEdit	m_edUnit;
	CButton	m_btHollow;
	float	m_fUnit;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHollowDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CHollowDialog)
	afx_msg void OnButtonHollow();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HOLLOWDIALOG_H__FC1AF5E3_B32A_11D3_BD51_00A0CC582962__INCLUDED_)
