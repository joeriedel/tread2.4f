#include "stdafx.h"
#include "CtrlStatusBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCtrlStatusBar

IMPLEMENT_DYNAMIC(CCtrlStatusBar, CStatusBar)

BEGIN_MESSAGE_MAP(CCtrlStatusBar, CStatusBar)
	//{{AFX_MSG_MAP(CCJToolBar)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCtrlStatusBar message handlers

CCtrlStatusBar::CCtrlStatusBar()
{
}

CCtrlStatusBar::~CCtrlStatusBar()
{
	m_ctrlList.DestroyList();
}

//////////////////////////////////////////////////////////////////////
// Inserts a control into the toolbar at the given button id.
// pClass	- runtime class of the control.
// strTitle - window title ( if any ) of the control.
// pRect	- size of the control to be inserted.
// nID		- resource id of the button where the control is to be placed.
// dwStyle  - style flags for the control

CWnd* CCtrlStatusBar::InsertControl(CRuntimeClass * pClass, CString strTitle, CRect & pRect, UINT nID, DWORD dwStyle)
{
	dwStyle |= WS_CHILD | WS_VISIBLE;
	CSBCtrl* pCtrl = NULL;
	
	CRect rect = pRect;
	BOOL bCreate = FALSE;
	
	// make sure the id is valid
	ASSERT( CommandToIndex( nID ) >= 0 );
	//SetButtonInfo( CommandToIndex( nID ), nID, TBBS_SEPARATOR, pRect.Width());
	
	CString strClass( pClass->m_lpszClassName );
	
	if(strClass == _T("CProgressCtrl"))
	{
		pCtrl = (CSBCtrl*)new CProgressCtrl();
		bCreate = ((CProgressCtrl*)pCtrl)->Create(dwStyle, rect, this, nID);
	}
	else if(strClass == _T("CComboBox"))
	{
		pCtrl = (CSBCtrl*)new CComboBox();
		bCreate = ((CComboBox*)pCtrl)->Create(dwStyle, rect, this, nID);
	}
	else if(strClass == _T("CEdit"))
	{
		pCtrl = (CSBCtrl*)new CEdit();
		bCreate = ((CEdit*)pCtrl)->Create(dwStyle, rect, this, nID);
	}
	else if(strClass == _T("CButton"))
	{
		pCtrl = (CSBCtrl*)new CButton();
		bCreate = ((CButton*)pCtrl)->Create(strTitle,dwStyle, rect, this, nID);
	}
	
	if(pCtrl == NULL)
		return NULL;

	if(bCreate == false)
	{
		delete pCtrl;
		return NULL;
	}

	GetItemRect(CommandToIndex(nID), &pRect);
	
	pCtrl->SetWindowPos(0, pRect.left, pRect.top, 0, 0,
		SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOSIZE | SWP_NOCOPYBITS );
	
	pCtrl->SetFont(GetFont());
	pCtrl->ShowWindow(SW_SHOW);
	
	// we have to remember this control, so we can delete it later
	m_ctrlList.AddItem(pCtrl);
	
	return pCtrl;
}