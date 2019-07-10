#if !defined(AFX_POPUPDIALOG_H__B5F2CD5C_0F66_4301_B9DB_82F1C92CAC87__INCLUDED_)
#define AFX_POPUPDIALOG_H__B5F2CD5C_0F66_4301_B9DB_82F1C92CAC87__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PopupDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPopupDialog dialog

#include "CharListBox.h"

class CGdfEdit2View;

class CPopupDialog : public CDialog
{
private:
	CRect m_rcWindow;
	CString m_sSelection;
	CString m_sMatch;
	CGdfEdit2View* m_pView;

// Construction
public:
	CPopupDialog(CWnd* pParent = NULL);   // standard constructor
	void SetWindowPosition(CPoint ptPos);
	void SetFindString(CString sMatch);
	void AddToFindString(CString sMatch);
	CString GetSelectedString();

	void AddStringItem(CString sItem);
	void AddStringList(CString pList[], int nLegnth);
	void AddStringList(char pList[], int nLength);
	
	CGdfEdit2View* GetParentWindow();
	void SetParentWindow(CGdfEdit2View* pView);

// Dialog Data
	//{{AFX_DATA(CPopupDialog)
	enum { IDD = IDD_DIALOG_POPUP };
	CCharListBox	m_lbItems;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPopupDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPopupDialog)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnKillfocusListPopup();
	afx_msg void OnSetFocus(CWnd* pNewWnd);
	afx_msg void OnButtonCapture();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_POPUPDIALOG_H__B5F2CD5C_0F66_4301_B9DB_82F1C92CAC87__INCLUDED_)
