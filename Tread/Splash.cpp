// CG: This file was added by the Splash Screen component.
// Splash.cpp : implementation file
//

#include "stdafx.h"  // e. g. stdafx.h
#include "resource.h"  // e.g. resource.h

#include "Splash.h"  // e.g. splash.h
#include "TreadSys.h"
#include "LinkedList.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
//   Splash Screen class

BOOL CSplashWnd::c_bShowSplashWnd;
CSplashWnd* CSplashWnd::c_pSplashWnd;
CSplashWnd::CSplashWnd()
{
	m_clLine1 = RGB(255, 255, 255);
	m_clLine2 = RGB(255, 255, 255);

/*#ifdef _DEBUG
	m_sLine1 = "Testing...";
	m_sLine2 = "Hello World";
#endif*/
}

CSplashWnd::~CSplashWnd()
{
	// Clear the static window pointer.
	ASSERT(c_pSplashWnd == this);
	c_pSplashWnd = NULL;
}

BEGIN_MESSAGE_MAP(CSplashWnd, CWnd)
	//{{AFX_MSG_MAP(CSplashWnd)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CSplashWnd::EnableSplashScreen(BOOL bEnable /*= TRUE*/)
{
	c_bShowSplashWnd = bEnable;
}

void CSplashWnd::ShowSplashScreen(CWnd* pParentWnd /*= NULL*/)
{
	if (!c_bShowSplashWnd || c_pSplashWnd != NULL)
		return;

	// Allocate a new splash screen, and create the window.
	c_pSplashWnd = new CSplashWnd;
	if (!c_pSplashWnd->Create(pParentWnd))
		delete c_pSplashWnd;
	else
	{
		c_pSplashWnd->ShowWindow(SW_SHOW);
		c_pSplashWnd->UpdateWindow();
	}
}

BOOL CSplashWnd::PreTranslateAppMessage(MSG* pMsg)
{
	if (c_pSplashWnd == NULL)
		return FALSE;

	// If we get a keyboard or mouse message, hide the splash screen.
	if (/*pMsg->message == WM_KEYDOWN ||
	    pMsg->message == WM_SYSKEYDOWN ||*/
	    pMsg->message == WM_LBUTTONDOWN ||
	    pMsg->message == WM_RBUTTONDOWN ||
	    pMsg->message == WM_MBUTTONDOWN ||
	    pMsg->message == WM_NCLBUTTONDOWN ||
	    pMsg->message == WM_NCRBUTTONDOWN ||
	    pMsg->message == WM_NCMBUTTONDOWN)
	{
		c_pSplashWnd->HideSplashScreen();
		return TRUE;	// message handled here
	}

	return FALSE;	// message not handled
}

BOOL CSplashWnd::Create(CWnd* pParentWnd /*= NULL*/)
{
	if (!m_Bitmap.LoadBitmap(IDB_SPLASH))
		return FALSE;

	BITMAP bm;
	m_Bitmap.GetBitmap(&bm);

	m_ftText.CreateFont(10, 0, 0, 0, 500,
						false, false, false,
						ANSI_CHARSET,
						OUT_DEFAULT_PRECIS,
						CLIP_DEFAULT_PRECIS,
						DEFAULT_QUALITY,
						DEFAULT_PITCH | FF_SWISS,
						"MS Sans Serif");

	RECT rect = {0, 0, bm.bmWidth, bm.bmHeight};
	this->CreateTransparent("Loading Tread...", rect, IDB_SPLASH_MASK, IDB_SPLASH);
	return TRUE;
}

void CSplashWnd::HideSplashScreen()
{
	// Destroy the window, and update the mainframe.
	DestroyWindow();
	if(AfxGetMainWnd() != NULL)
		AfxGetMainWnd()->UpdateWindow();
}

void CSplashWnd::PostNcDestroy()
{
	// Free the C++ class.
	delete this;
}

int CSplashWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// Center the window.
	CenterWindow();
	//SetWindowText("Tread3D Startup...");

	// Set a timer to destroy the splash screen.
	SetTimer(1, 4000, NULL);

	return 0;
}

void CSplashWnd::OnPaint()
{
	CPaintDC dc(this);
	CDC dcImage;
	BITMAP bm;
	CBitmap* pOldBitmap;

	if (!dcImage.CreateCompatibleDC(&dc))
		return;
	
	m_Bitmap.GetBitmap(&bm);

	// Paint the image.
	pOldBitmap = dcImage.SelectObject(&m_Bitmap);
	dc.BitBlt(0, 0, bm.bmWidth, bm.bmHeight, &dcImage, 0, 0, SRCCOPY);
	dcImage.SelectObject(pOldBitmap);

	RenderTextLines(dc);
}

#define START_X	40
#define START_Y	94
#define SIZE_X	400//260
#define SIZE_Y	110
#define CHAR_HEIGHT 14

void CSplashWnd::StaticPrintLine(CString sInput)
{
	if(c_pSplashWnd == NULL)
		return;
	c_pSplashWnd->PrintLine(sInput);
}

#define MAX_LINES	10
void CSplashWnd::PrintLine(CString sInput)
{
	TEXTLINE* pTextLine = new TEXTLINE;
	pTextLine->sString = sInput;
	TextList.AddItem(pTextLine, LL_HEAD);
	if(TextList.GetCount() > MAX_LINES)
		TextList.DeleteItem(LL_TAIL);

	RECT rcRedraw = {START_X, START_Y, START_X+SIZE_X, START_Y+SIZE_Y};
	RedrawWindow(&rcRedraw, NULL, RDW_INVALIDATE|RDW_UPDATENOW);
}

void CSplashWnd::RenderTextLines(CPaintDC& dc)
{
	// Start at the bottom and work up.
	int x = START_X;
	int y = START_Y+SIZE_Y-CHAR_HEIGHT;
	TEXTLINE* pTextLine;

	CFont* pOldFont = dc.SelectObject(&m_ftText);
	dc.SetBkMode(TRANSPARENT);
	dc.SetTextColor(RGB(255, 255, 255));
	
	TextList.First();
	while(y >= START_Y)
	{
		// Print this line.
		pTextLine = TextList.GetCurItem();
		if(pTextLine == NULL)
			break;

		dc.TextOut(x, y, pTextLine->sString);
		y-=CHAR_HEIGHT;
		TextList.GetNextItem();
	}
	
	dc.SelectObject(pOldFont);
}

void CSplashWnd::OnTimer(UINT nIDEvent)
{
	// Destroy the splash screen window.
	HideSplashScreen();
}

void CSplashWnd::SetTextLine1(CString sInput)
{
	if(c_pSplashWnd)
		c_pSplashWnd->SetTextL1(sInput);
}

void CSplashWnd::SetTextLine2(CString sInput)
{
	if(c_pSplashWnd)
		c_pSplashWnd->SetTextL2(sInput);
}

void CSplashWnd::SetTextL1(CString &sInput)
{
	m_sLine1 = sInput;
	CRect rcText(8, 150, 220 + 8, 150 + 15);
	RedrawWindow(rcText, NULL, RDW_INVALIDATE|RDW_UPDATENOW);
}

void CSplashWnd::SetTextL2(CString &sInput)
{
	m_sLine2 = sInput;
	CRect rcText(8, 165, 220 + 8, 165 + 20);
	RedrawWindow(rcText, NULL, RDW_INVALIDATE|RDW_UPDATENOW);
}

void CSplashWnd::SetColorLine1(COLORREF clInput)
{
	if(!c_pSplashWnd)
		return;

	//c_pSplashWnd->m_clLine1 = clInput;
}

void CSplashWnd::SetColorLine2(COLORREF clInput)
{
	if(!c_pSplashWnd)
		return;

	//c_pSplashWnd->m_clLine2 = clInput;
}
