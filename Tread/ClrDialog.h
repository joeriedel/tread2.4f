#if !defined(AFX_CLRDIALOG_H__1B7A77A2_7003_11D3_B673_004005A2D20C__INCLUDED_)
#define AFX_CLRDIALOG_H__1B7A77A2_7003_11D3_B673_004005A2D20C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ClrDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CClrDialog dialog

class CClrDialog : public CDialog
{
private:
	bool m_bDraw;
	CRect m_rcDisplay;
	CRect m_rcBox;
	CPen* m_penDraw;
	CDC* m_pDC;
	COLORREF m_clText;

// Construction
public:
	CClrDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CClrDialog)
	enum { IDD = IDD_DIALOG_COLOR };
	CStatic	m_stDisplay;
	CListBox	m_lbItems;
	CButton	m_btColor;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CClrDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CClrDialog)
	afx_msg void OnDestroy();
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonClose();
	afx_msg void OnButtonColor();
	afx_msg void OnButtonReset();
	afx_msg void OnSelchangeListItems();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CLRDIALOG_H__1B7A77A2_7003_11D3_B673_004005A2D20C__INCLUDED_)
