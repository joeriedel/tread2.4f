#if !defined(AFX_SETUPDIALOG_H__5DCE52A1_B34E_11D3_BD51_00A0CC582962__INCLUDED_)
#define AFX_SETUPDIALOG_H__5DCE52A1_B34E_11D3_BD51_00A0CC582962__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SetupDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSetupDialog dialog

class CSetupDialog : public CDialog
{
// Construction
public:
	CSetupDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSetupDialog)
	enum { IDD = IDD_DIALOG_SETUP };
	CSliderCtrl	m_slCurve;
	CComboBox	m_cbTexMemory;
	CSliderCtrl	m_slGamma;
	CSliderCtrl	m_slBackplane;
	CComboBox	m_cbMaxmap;
	CString	m_sBackClip;
	CString	m_sGamma;
	CString	m_sCurve;
	int		m_nSaveOption;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSetupDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSetupDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnButtonSave();
	afx_msg void OnButtonCancel();
	afx_msg void OnRadioSave();
	afx_msg void OnRadioSaveOff();
	afx_msg void OnRadioSaveReminder();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETUPDIALOG_H__5DCE52A1_B34E_11D3_BD51_00A0CC582962__INCLUDED_)
