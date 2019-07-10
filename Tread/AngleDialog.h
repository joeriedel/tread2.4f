#if !defined(AFX_ANGLEDIALOG_H__216872E1_8E1F_11D3_B08D_00A0CC582962__INCLUDED_)
#define AFX_ANGLEDIALOG_H__216872E1_8E1F_11D3_B08D_00A0CC582962__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AngleDialog.h : header file
//

#include "FilterEdit.h"
/////////////////////////////////////////////////////////////////////////////
// CAngleDialog dialog

class CAngleDialog : public CDialog
{
// Construction
private:
	CRect m_rcClient;
	CPoint m_ptCenter;
	CPoint m_ptAngle;
	CRect m_rcBound;
	int m_nDegree;
	int m_nLast;
	bool m_bDrag;
	CFont m_ftText;
	CPen m_pnBlue;
	CPen m_pnBlack;
	//CString m_sDegree;

	void CalcAngle();
	void UpdateDialog();

public:
	CAngleDialog(CWnd* pParent = NULL);   // standard constructor
	void SetAngle(int nAngle);
	CString GetAngle();
	void SetAngle(CString sAngle);

// Dialog Data
	//{{AFX_DATA(CAngleDialog)
	enum { IDD = IDD_ENTITY_ANGLE };
	CFilterEdit	m_edAngle;
	CComboBox	m_cbDegree;
	CString	m_sDegree;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAngleDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAngleDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonDone();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnSelchangeComboAngle();
	afx_msg void OnChangeEditAngle();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ANGLEDIALOG_H__216872E1_8E1F_11D3_B08D_00A0CC582962__INCLUDED_)
