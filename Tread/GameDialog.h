#if !defined(AFX_GAMEDIALOG_H__19F392A1_7089_11D3_B673_004005A2D20C__INCLUDED_)
#define AFX_GAMEDIALOG_H__19F392A1_7089_11D3_B673_004005A2D20C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GameDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CGameDialog dialog

class CGameDialog : public CDialog
{
private:
	CString m_sGame;
// Construction
public:
	CGameDialog(CWnd* pParent = NULL);   // standard constructor
	void AddGameString(CString sGame);
	CString GetGameSelection();

// Dialog Data
	//{{AFX_DATA(CGameDialog)
	enum { IDD = IDD_DIALOG_GAMESELECT };
	CListBox	m_lbGames;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGameDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CGameDialog)
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeListGames();
	afx_msg void OnButtonSelect();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnDblclkListGames();
	afx_msg void OnSelcancelListGames();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GAMEDIALOG_H__19F392A1_7089_11D3_B673_004005A2D20C__INCLUDED_)
