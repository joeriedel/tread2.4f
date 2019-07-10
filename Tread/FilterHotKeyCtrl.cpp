// FilterHotKeyCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "tread3d.h"
#include "FilterHotKeyCtrl.h"
#include "TreadSys.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFilterHotKeyCtrl

CFilterHotKeyCtrl::CFilterHotKeyCtrl()
{
}

CFilterHotKeyCtrl::~CFilterHotKeyCtrl()
{
}


BEGIN_MESSAGE_MAP(CFilterHotKeyCtrl, CHotKeyCtrl)
	//{{AFX_MSG_MAP(CFilterHotKeyCtrl)
	ON_WM_KEYDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFilterHotKeyCtrl message handlers

void CFilterHotKeyCtrl::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	if(((nChar >= 65) && (nChar <= 90)) || ((nChar >= 48) && (nChar <= 57)))
	{
		CHotKeyCtrl::OnKeyDown(nChar, nRepCnt, nFlags);
		
		return;
	}
	
	switch(nChar)
	{
	case 113:
	case 114:
	case 115:
	case 116:
	case 117:
	case 118:
	case 119:
	case 120:
	case 121:
	case 122:
	case 123:
	case 36:
	case 35:
	case 33:
	case 34:
	case 45:
	case 37:
	case 39:
	case 38:
	case 40:
		break;
	case 219:
	case 220:
	case 221:
	case 186:
	case 222:
	case 188:
	case 190:
	case 191:
		//nChar &= ~0x80;
		//nChar = 'A';
		break;
	default:
		return;
	}
	
	//CHotKeyCtrl::SetHotKey(nChar, 0);
	CHotKeyCtrl::OnKeyDown(nChar, nRepCnt, nFlags);
}
