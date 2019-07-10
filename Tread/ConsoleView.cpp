// ConsoleView.cpp : implementation file
//

#include "stdafx.h"
#include "Tread3D.h"
#include "ConsoleView.h"
#include "TreadSys.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CConsoleView

IMPLEMENT_DYNCREATE(CConsoleView, CRichEditView)

CConsoleView::CConsoleView()
{
}

CConsoleView::~CConsoleView()
{
}


BEGIN_MESSAGE_MAP(CConsoleView, CRichEditView)
	//{{AFX_MSG_MAP(CConsoleView)
	ON_WM_DROPFILES()
	ON_WM_RBUTTONDOWN()
	ON_WM_SETCURSOR()
	ON_WM_CREATE()
	ON_COMMAND(ID_CONSOLE_CLEAR, OnConsoleClear)
	ON_COMMAND(ID_CONSOLE_DUMP, OnConsoleDump)
	ON_COMMAND(ID_CONSOLE_INFO, OnConsoleInfo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConsoleView drawing

/////////////////////////////////////////////////////////////////////////////
// CConsoleView diagnostics

#ifdef _DEBUG
void CConsoleView::AssertValid() const
{
	CRichEditView::AssertValid();
}

void CConsoleView::Dump(CDumpContext& dc) const
{
	CRichEditView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CConsoleView message handlers

void CConsoleView::ScrollToLine(int nLine)
{
	SetCapture();
	BeginWaitCursor();

	long nLength = GetRichEditCtrl().GetTextLength();
	bool bLine = false;
	CString sBuffer;
	char sArray[10];
	int n = 0;
	bLine = FindLine(nLine, n, 1000);

	if(bLine)
	{
		SetFocus();
		GetRichEditCtrl().SetSel(n, n);
				
		CTread3DApp* pApp = (CTread3DApp*)AfxGetApp();
		itoa(GetRichEditCtrl().LineFromChar(n) + 1, sArray, 10);
		sBuffer = "Line : ";
		sBuffer += sArray;
	}

	ReleaseCapture();
	EndWaitCursor();
}

bool CConsoleView::FindLine(int nLine, int &nPos, int nGap)
{
	for(nPos; nPos < (GetRichEditCtrl().GetTextLength() + nGap); nPos += nGap)
	{
		if((GetRichEditCtrl().LineFromChar(nPos) + 1) == nLine)
			return true;
		else if((GetRichEditCtrl().LineFromChar(nPos) + 1) > nLine)
		{
			if(FindLine(nLine, (nPos -= nGap), nGap / 2))
				return true;
			break;
		}
	}

	return false;
}

void CConsoleView::AppendText(const CString& sInput, UINT nModifier)
{
	CRichEditCtrl& rich = GetRichEditCtrl();
	int nLength  = GetTextLength();
	
	rich.SetSel( -1, -1 );		// move insertion point to end of text
	rich.ReplaceSel(sInput, false);
	rich.SetSel( nLength, -1 ); // select the inserted text

	chFormat.dwEffects |= nModifier;
	SetCharFormat( chFormat );
	rich.SetSel( -1, -1 ); // again, move insertion point to end of text
	
	chFormat.dwEffects &= !nModifier;
	SetCharFormat( chFormat );
}

void CConsoleView::ResetContent()
{
	CRichEditCtrl& rich = GetRichEditCtrl();
	rich.SetSel( 0, -1 );		// move insertion point to end of text
	rich.ReplaceSel("", false);
}

BOOL CConsoleView::OnDrop(COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point) 
{
	return false;
}

DROPEFFECT CConsoleView::OnDragOver(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point) 
{
	//return CRichEditView::OnDragOver(pDataObject, dwKeyState, point);
	return DROPEFFECT_NONE;
}

DROPEFFECT CConsoleView::OnDragEnter(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point) 
{
	//return CRichEditView::OnDragEnter(pDataObject, dwKeyState, point);
	return DROPEFFECT_NONE;
}

void CConsoleView::OnDropFiles(HDROP hDropInfo) 
{
}

void CConsoleView::OnRButtonDown(UINT nFlags, CPoint point) 
{
	CMenu commandMenu;
	ClientToScreen(&point);

	commandMenu.LoadMenu(IDR_MENU_CONSOLE);
	CMenu *popup = commandMenu.GetSubMenu(0);
	popup->TrackPopupMenu(TPM_RIGHTALIGN | TPM_RIGHTBUTTON, 
							point.x, point.y, this);
	
	CRichEditView::OnRButtonDown(nFlags, point);
}

BOOL CConsoleView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	HCURSOR cursor;
	cursor = AfxGetApp()->LoadCursor(IDC_CURSOR_CONSOLE);
	if(cursor == NULL)
		return CView::OnSetCursor(pWnd, nHitTest, message);

	SetCursor(cursor);
	
	return TRUE;
}

void CConsoleView::SetFontType(CString sFont)
{
	strcpy(chFormat.szFaceName, (LPCSTR)sFont);
	SetCharFormat(chFormat);
}

void CConsoleView::SetFontColor(COLORREF clText)
{
	chFormat.crTextColor = clText;
	SetCharFormat(chFormat);
}

void CConsoleView::SetFontSize(int nSize)
{
	chFormat.yHeight = nSize;
	SetCharFormat(chFormat);
}

void CConsoleView::ResetFont()
{
	chFormat.crTextColor = DF_COLOR;
	strcpy(chFormat.szFaceName, DF_FONT);
	chFormat.yHeight = DF_SIZE;

	SetCharFormat(chFormat);
}

void CConsoleView::SetFont(CString sFont, int nSize, COLORREF clFont)
{
	if(!sFont.IsEmpty())
		strcpy(chFormat.szFaceName, (LPCSTR)sFont);

	if(nSize > 100)
		chFormat.yHeight = nSize;

	if(clFont != 4294967295)
		chFormat.crTextColor = clFont;

	SetCharFormat(chFormat);
}

int CConsoleView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CRichEditView::OnCreate(lpCreateStruct) == -1)
		return -1;

	GetRichEditCtrl().SetReadOnly(true);
	GetRichEditCtrl().SetBackgroundColor(false, RGB(220, 220, 220));
	GetRichEditCtrl().GetSelectionCharFormat(chFormat);
	GetRichEditCtrl().GetParaFormat(prFormat);

	chFormat.dwEffects &= ~CFE_AUTOCOLOR;
	chFormat.dwEffects &= ~CFE_BOLD;
	chFormat.dwMask |= CFM_COLOR ;
	chFormat.dwMask |= CFM_BACKCOLOR;
	chFormat.dwMask |= CFM_KERNING;
	
	strcpy(chFormat.szFaceName, DF_FONT);
	chFormat.yHeight = DF_SIZE;

	chFormat.crTextColor = DF_COLOR;
	chFormat.crBackColor = RGB(220, 220, 220);

	SetCharFormat(chFormat);
	SetParaFormat(prFormat);
	
	return 0;
}

void CConsoleView::OnConsoleClear() 
{
	int nRet = MessageBox("Are you sure you want to clear the console?", "Confirmation", MB_YESNO | MB_ICONQUESTION);

	switch(nRet)
	{
	case IDYES:
		CRichEditCtrl& rich = GetRichEditCtrl();
		rich.SetSel( 0, -1 );		// move insertion point to end of text
		rich.ReplaceSel("", false);
		break;
	};
}

void CConsoleView::OnConsoleDump() 
{
	CString sBuffer;
	CRichEditCtrl& rich = GetRichEditCtrl();
	rich.SetSel( 0, -1 );		// move insertion point to end of text
	sBuffer = rich.GetSelText();
	rich.SetSel(-1, -1);

	CFile hFile;
	if(hFile.Open(GetRegString(S_TREAD_PATH) + "\\console.doc", CFile::modeWrite | CFile::modeCreate))
	{
		hFile.Write((LPCSTR)sBuffer, sBuffer.GetLength());
		hFile.Close();
		Debug("Wrote: console.doc\n");
	}
	else
		Debug("Console dump: Failed.\n");
}

void CConsoleView::OnConsoleInfo() 
{
	SetFontSize(220);
	SetFontColor(RGB(0, 0, 0));
	SetFontType("Times New Roman");
	AppendText("----------------------------------------\n", CFM_BOLD | CFM_STRIKEOUT);

	SetFontColor(RGB(0, 100, 50));
	AppendText("Author: Joe Riedel, Nick Randal\n");
	AppendText("Very special thanks to Sean Arnold and Wayne Smith\n\n");
	
	SetFontColor(RGB(0, 0, 255));
	AppendText("Visit our website at: http://www.planetquake.com/tread\n", CFM_UNDERLINE);

	SetFontSize(200);
	SetFontColor(RGB(0, 0, 0));
	AppendText("----------------------------------------\n", CFM_BOLD | CFM_STRIKEOUT);
	AppendText("Copyright © 2000 Joe Riedel and Nick Randal, All Rights Reserved.\n");
	
	SetFontColor(RGB(0, 0, 0));
	AppendText("----------------------------------------\n", CFM_BOLD | CFM_STRIKEOUT);
	AppendText("This program is protected by federal and international copyright law.\n");
	AppendText("Distribution or replication of Tread program files without\n");
	AppendText("express written permission from Joe Riedel and Nicholas Randal\n");
	AppendText("is forbidden. This version of Tread may *not* be used for commercial purposes of any kind.\n");
	AppendText("Violators will be prosecuted to the full extent of the law.\n");
	
	SetFontColor(RGB(0, 0, 0));
	AppendText("----------------------------------------\n", CFM_BOLD | CFM_STRIKEOUT);
	ResetFont();
}
