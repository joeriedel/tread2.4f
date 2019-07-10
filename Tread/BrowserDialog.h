#if !defined(AFX_BROWSERDIALOG_H__6AF73BA1_765D_11D3_B673_004005A2D20C__INCLUDED_)
#define AFX_BROWSERDIALOG_H__6AF73BA1_765D_11D3_B673_004005A2D20C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BrowserDialog.h : header file
//

//#include "TextureListBox.h"
#include "TexListBox.h"
#include "GameApi.h"
#include "TexApi.h"

/////////////////////////////////////////////////////////////////////////////
// CBrowserDialog dialog

class CBrowserDialog : public CDialog
{
private:
	CPoint m_ptMin;
	bool m_bInit;
	CPoint m_ptAnchor;
	CRect m_rcClient;
	CRect m_rcSize;
	bool bNewPos;
	CRect m_rcNewSize;
	WINDOWPLACEMENT m_wndPlace;
	int m_nPos;

	// Construction
public:
	CBrowserDialog(CWnd* pParent = NULL);   // standard constructor
	void GetWindowPos(CRect &rcTemp);
	void SetWindowPos(CRect rcPos);

	int GetSizePosition();
	void SetSizePosition(int nPos);
	CString GetFilterString();
	void SetFilterString(CString sFilter);

// Dialog Data
	//{{AFX_DATA(CBrowserDialog)
	enum { IDD = IDD_DIALOG_TEXTUREBROWSER };
	
	CButton	m_btFilter;
	CComboBox	m_cbSize;
	CButton	m_btDone;

	CTexListBox	m_lbTexture;
	CTexListBox	m_lbPreview;

	CString	m_sFilter;
	CString	m_sTexname;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBrowserDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CBrowserDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnButtonDone();
	afx_msg void OnSelchangeTexturesizelist();
	afx_msg void OnSelchangeTexturelistbox();
	afx_msg void OnDblclkTexturelistbox();
	afx_msg void OnShowWindow(bool bShow, UINT nStatus);
	afx_msg void OnButtonFilter();
	afx_msg void OnButtonReset();
	afx_msg void OnChangeEditFilter();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BROWSERDIALOG_H__6AF73BA1_765D_11D3_B673_004005A2D20C__INCLUDED_)
