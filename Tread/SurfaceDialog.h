#if !defined(AFX_SURFACEDIALOG_H__63310301_91DE_11D3_BD51_00A0CC582962__INCLUDED_)
#define AFX_SURFACEDIALOG_H__63310301_91DE_11D3_BD51_00A0CC582962__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SurfaceDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSurfaceDialog dialog

#include "TexListBox.h"

#define IDD_DIALOG_SURFACE 194

class CSurfaceDialog : public CDialog
{
private:
	int m_nStep;
	int m_nCBSelect;
	CImageList m_imgList;
	int m_nSurface;
	int m_nContents;
	bool m_bFace;
	bool m_bTexLock;

// Construction
public:
	CSurfaceDialog(CWnd* pParent = NULL);   // standard constructor
	void TextureLockUpdate();
	void SaveFaceValues();
	void LoadFaceValues(bool bFace);
	void EnableControls(bool bEnable = true);
	
// Dialog Data
	//{{AFX_DATA(CSurfaceDialog)
	enum { IDD = IDD_DIALOG_SURFACE };
	CEdit	m_edValue;
	CButton	m_btAuto;
	CButton	m_btMask;
	CButton	m_btReset;
	CButton	m_btGrab;
	CStatic	m_icTexLock;
	CStatic	m_stTexLock;
	CButton	m_btApply;
	CTexListBox	m_lbPreview;
	CListCtrl	m_lcProperties;
	int		m_nRotate;
	int		m_nScaleX;
	int		m_nShiftX;
	int		m_nScaleY;
	int		m_nShiftY;
	BOOL	m_bMask;
	CString	m_sTextureName;
	int		m_nValue;
	BOOL	m_bAutoApply;
	//}}AFX_DATA

	CEdit	m_edControl[5];
	CSpinButtonCtrl	m_spAdjust[5];


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSurfaceDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSurfaceDialog)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnButtonGrabtexture();
	afx_msg void OnButtonReset();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnButtonApply();
	afx_msg void OnCheckAutoapply();
	afx_msg void OnCheckMask();
	afx_msg void OnClickListProperrties(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnChangeEditValue();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SURFACEDIALOG_H__63310301_91DE_11D3_BD51_00A0CC582962__INCLUDED_)
