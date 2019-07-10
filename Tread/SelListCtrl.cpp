// SelListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "TreadSys.h"
#include "SelListCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSelListCtrl

CSelListCtrl::CSelListCtrl()
{
}

CSelListCtrl::~CSelListCtrl()
{
}


BEGIN_MESSAGE_MAP(CSelListCtrl, CListCtrl)
	//{{AFX_MSG_MAP(CSelListCtrl)
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSelListCtrl message handlers

void CSelListCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	UINT uFlags = 0;
	nItem = HitTest(point, &uFlags);

	CListCtrl::OnLButtonDown(nFlags, point);
}

int CSelListCtrl::GetSelection()
{
	return nItem;
}