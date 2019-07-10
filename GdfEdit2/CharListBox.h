#if !defined(AFX_CHARLISTBOX_H__69B0BCF9_DB52_494A_8F73_459FB3DE740F__INCLUDED_)
#define AFX_CHARLISTBOX_H__69B0BCF9_DB52_494A_8F73_459FB3DE740F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CharListBox.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCharListBox window
class CPopupDialog;

class CCharListBox : public CListBox
{
private:
	CFont m_ftText;
	CFont m_ftBoldText;
	CPopupDialog* m_pDialog;
	CString m_sMatch;

	int IndexOfMatch(CString &sParam1, CString &sParam2);

// Construction
public:
	CCharListBox();
	CPopupDialog* GetParentWindow();
	void SetParentWindow(CPopupDialog* pDialog);
	void SetUserString(CString sMatch);
	void CompleteText();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCharListBox)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual int CompareItem(LPCOMPAREITEMSTRUCT lpCompareItemStruct);
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	virtual void DeleteItem(LPDELETEITEMSTRUCT lpDeleteItemStruct);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCharListBox();

	// Generated message map functions
protected:
	//{{AFX_MSG(CCharListBox)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHARLISTBOX_H__69B0BCF9_DB52_494A_8F73_459FB3DE740F__INCLUDED_)
