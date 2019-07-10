#if !defined(AFX_TEXTUREBAR_H__0F54C9E2_6CE2_11D3_B673_004005A2D20C__INCLUDED_)
#define AFX_TEXTUREBAR_H__0F54C9E2_6CE2_11D3_B673_004005A2D20C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TextureBar.h : header file
//

#include <cj60lib.h>
/////////////////////////////////////////////////////////////////////////////
// CTextureBar dialog

class CTextureBar : public CDialogBar
{
// Construction
public:
	CTextureBar(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTextureBar)
	enum { IDD = IDD_DIALOGBAR_TEXTURE };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTextureBar)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTextureBar)
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnDestroy();
	virtual BOOL OnInitDialog();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TEXTUREBAR_H__0F54C9E2_6CE2_11D3_B673_004005A2D20C__INCLUDED_)
