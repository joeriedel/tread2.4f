// CG: This file was added by the Splash Screen component.

#ifndef _SPLASH_SCRN_
#define _SPLASH_SCRN_

// Splash.h : header file
//
#include "LinkedList.h"
#include "TransparentWnd.h"

/////////////////////////////////////////////////////////////////////////////
//   Splash Screen class
struct TEXTLINE
{
	TEXTLINE* prev, *next;
	CString sString;
};


class CSplashWnd : public CTransparentWnd//public CWnd
{
// Construction
protected:
	CSplashWnd();

// Attributes:
public:
	static void SetTextLine1(CString sInput);
	static void SetTextLine2(CString sInput);
	static void SetColorLine1(COLORREF clInput);
	static void SetColorLine2(COLORREF clInput);
	static void StaticPrintLine(CString sInput);

private:
	
	CBitmap m_Bitmap;
	
// Operations
public:
	static void EnableSplashScreen(BOOL bEnable = TRUE);
	static void ShowSplashScreen(CWnd* pParentWnd = NULL);
	static BOOL PreTranslateAppMessage(MSG* pMsg);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSplashWnd)
	//}}AFX_VIRTUAL

// Implementation
public:
	~CSplashWnd();
	virtual void PostNcDestroy();

protected:
	void SetTextL1(CString &sInput);
	void SetTextL2(CString &sInput);

	BOOL Create(CWnd* pParentWnd = NULL);
	void HideSplashScreen();
	static BOOL c_bShowSplashWnd;
	static CSplashWnd* c_pSplashWnd;

	CString m_sLine1;
	CString m_sLine2;
	COLORREF m_clLine1;
	COLORREF m_clLine2;

	CLinkedList<TEXTLINE> TextList;
	void RenderTextLines(CPaintDC&);
	void PrintLine(CString sInput);

	CFont m_ftText;

// Generated message map functions
protected:
	//{{AFX_MSG(CSplashWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


#endif
