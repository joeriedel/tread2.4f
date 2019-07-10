#if !defined(AFX_HIGHLIGHTCOMBOBOX_H__8F815221_BA0B_11D3_BD51_00A0CC582962__INCLUDED_)
#define AFX_HIGHLIGHTCOMBOBOX_H__8F815221_BA0B_11D3_BD51_00A0CC582962__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HighlightComboBox.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CHighlightComboBox window

class CHighlightComboBox : public CComboBox
{
// Construction
private:
	COLORREF m_clHighlight;
	bool m_bFound;

public:
	CHighlightComboBox();
	void SetHighlightColor(COLORREF clHighlight);
	void SetHighlight(bool bHighlight);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHighlightComboBox)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CHighlightComboBox();

	// Generated message map functions
protected:
	//{{AFX_MSG(CHighlightComboBox)
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HIGHLIGHTCOMBOBOX_H__8F815221_BA0B_11D3_BD51_00A0CC582962__INCLUDED_)
