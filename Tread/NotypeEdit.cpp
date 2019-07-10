// NotypeEdit.cpp : implementation file
//

#include "stdafx.h"
#include "Tread3D.h"
#include "NotypeEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNotypeEdit

CNotypeEdit::CNotypeEdit()
{
}

CNotypeEdit::~CNotypeEdit()
{
}


BEGIN_MESSAGE_MAP(CNotypeEdit, CEdit)
	//{{AFX_MSG_MAP(CNotypeEdit)
	ON_WM_CHAR()
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNotypeEdit message handlers

void CNotypeEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	//CEdit::OnChar(nChar, nRepCnt, nFlags);
}

BOOL CNotypeEdit::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	// TODO: Add your message handler code here and/or call default
	HCURSOR cursor;
	cursor = AfxGetApp()->LoadCursor(IDC_CURSOR_CONSOLE2);
	if(cursor == NULL)
		return CEdit::OnSetCursor(pWnd, nHitTest, message);

	SetCursor(cursor);
	
	return TRUE;
}

void CNotypeEdit::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	//CEdit::OnLButtonDown(nFlags, point);
}
