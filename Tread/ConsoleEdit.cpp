// ConsoleEdit.cpp : implementation file
//

#include "stdafx.h"
#include "tread3d.h"
#include "ConsoleEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CConsoleEdit

CConsoleEdit::CConsoleEdit()
{
}

CConsoleEdit::~CConsoleEdit()
{
}


BEGIN_MESSAGE_MAP(CConsoleEdit, CEdit)
	//{{AFX_MSG_MAP(CConsoleEdit)
	ON_CONTROL_REFLECT(EN_MAXTEXT, OnMaxtext)
	ON_WM_SETCURSOR()
	ON_WM_CHAR()
	ON_WM_RBUTTONDOWN()
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConsoleEdit message handlers

void CConsoleEdit::OnMaxtext() 
{
	// TODO: Add your control notification handler code here
	Clear();
}

BOOL CConsoleEdit::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	HCURSOR cursor;
	cursor = AfxGetApp()->LoadCursor(IDC_CURSOR_CONSOLE2);
	if(cursor == NULL)
		return CEdit::OnSetCursor(pWnd, nHitTest, message);

	SetCursor(cursor);
	
	return true;
}

void CConsoleEdit::Clear()
{
	SetSel(0, -1, true);
	ReplaceSel("");
	SetSel(0);
}

void CConsoleEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
}

void CConsoleEdit::OnRButtonDown(UINT nFlags, CPoint point) 
{
}

void CConsoleEdit::OnLButtonDown(UINT nFlags, CPoint point) 
{
}
