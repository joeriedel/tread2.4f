// Tread3D.h : main header file for the TREAD3D application
//

#if !defined(AFX_TREAD3D_H__E08CCCE5_1023_11D3_9084_004005A2D20C__INCLUDED_)
#define AFX_TREAD3D_H__E08CCCE5_1023_11D3_9084_004005A2D20C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols
#include <afxsock.h>

/////////////////////////////////////////////////////////////////////////////
// CTread3DApp:
// See Tread3D.cpp for the implementation of this class
//

//the following three defines specify the count of each type

//string definitions
#define S_TREAD_PATH	0
#define S_USER_NAME		1
#define S_3D_PAK		2
#define S_2D_PAK		3
//-----------------------
#define STRING_MAX		4

//integer definitions
#define I_BACKPLANE		0
#define I_GAMMA			1
#define I_MAXMAP		2
#define I_CURVESUBDEPTH	3
#define I_TEX_MEMORY	4
//-----------------------
#define MAX_INT			5

//bool definitions
#define B_FIRST_RUN		0
#define B_VISGROUP_APPLY 1
#define B_DEBUG			2
#define B_SMARTNAME		3
#define B_SELECTIONMASK	4
#define B_FACEAUTOAPPLY	5
#define B_NOSPLASH		6
#define B_LOAD_PLUGINS	7
#define B_REGISTER		8
#define B_IE4			9
#define B_ANIMATECOLORS	10
//-----------------------
#define BOOL_MAX		11

class CTread3DApp : public CWinApp
{
private:
	bool VerifyKey(CString &sUser, CString sKey);
	int VerifyKeyFile();

public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CTread3DApp();

	CString m_pString[STRING_MAX];
	int m_pInt[MAX_INT];
	bool m_pBool[BOOL_MAX];

	void LoadSettings();
	void SaveSettings();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTread3DApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CTread3DApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void ShowTipAtStartup(void);
	void ShowTipOfTheDay(void);

	WSADATA lpwsaData;
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TREAD3D_H__E08CCCE5_1023_11D3_9084_004005A2D20C__INCLUDED_)
