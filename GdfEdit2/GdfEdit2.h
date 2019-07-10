// GdfEdit2.h : main header file for the GDFEDIT2 application
//

#if !defined(AFX_GDFEDIT2_H__E6785D44_7D06_11D3_B673_004005A2D20C__INCLUDED_)
#define AFX_GDFEDIT2_H__E6785D44_7D06_11D3_B673_004005A2D20C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CGdfEdit2App:
// See GdfEdit2.cpp for the implementation of this class
//

class CGdfEdit2App : public CWinApp
{
private:
	CStatusBar*  m_pStatusBar;
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CGdfEdit2App();
	void SetPaneText(CString sText);
	void AttachStatusBar(CStatusBar* pStatusBar);


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGdfEdit2App)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CGdfEdit2App)
	afx_msg void OnAppAbout();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GDFEDIT2_H__E6785D44_7D06_11D3_B673_004005A2D20C__INCLUDED_)
