// ConsoleView.cpp : implementation file
//

#include "stdafx.h"
#include "GdfEdit2.h"
#include "ConsoleView.h"
#include "childFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CConsoleView

IMPLEMENT_DYNCREATE(CConsoleView, CListView)

CConsoleView::CConsoleView()
{
	m_nLast = 0;
	m_nWarning = 0;
	m_nError = 0;
}

CConsoleView::~CConsoleView()
{
}

BEGIN_MESSAGE_MAP(CConsoleView, CListView)
	//{{AFX_MSG_MAP(CConsoleView)
	ON_WM_CREATE()
	ON_NOTIFY_REFLECT(NM_DBLCLK, OnDblclk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConsoleView drawing

void CConsoleView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CConsoleView diagnostics

#ifdef _DEBUG
void CConsoleView::AssertValid() const
{
	CListView::AssertValid();
}

void CConsoleView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CConsoleView message handlers

void CConsoleView::OnInitialUpdate() 
{
	CListView::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
	//m_ListCtrl->ModifyStyle(LVS_TYPEMASK, LVS_LIST | LVS_ALIGNLEFT | LVS_NOLABELWRAP | LVS_SHOWSELALWAYS);

	if(!m_imgList.Create(IDB_BITMAP_ERROR, 16, 0, RGB(255, 0, 255)))
	{
		return;
	}
	m_imgList.SetOverlayImage(3, 1);
	
	m_ListCtrl->SetImageList(&m_imgList, LVSIL_SMALL);
	m_ListCtrl->InsertColumn(0, "", LVCFMT_LEFT, 600, -1);
}

int CConsoleView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	
	if (CListView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	m_ListCtrl = &GetListCtrl();
	m_ListCtrl->ModifyStyle(LVS_TYPEMASK, LVS_REPORT | LVS_NOCOLUMNHEADER);
	
	return 0;
}

void CConsoleView::OnDblclk(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	NMLISTVIEW* pListView = (NMLISTVIEW*)pNMHDR;

	if(pListView->iItem == -1)
		return;

	int nItem = m_ListCtrl->GetItemData(pListView->iItem);
	if(nItem == -1)
		return;

	LVITEM item;
	//m_ListCtrl->GetItemState(pListView->iItem, &item);
	memset(&item, 0, sizeof(LVITEM));
	//item.mask = LVIF_STATE;
	item.iItem = pListView->iItem;
	//item.iImage = 3;
	item.stateMask = LVIS_OVERLAYMASK ;
	item.state = INDEXTOOVERLAYMASK(1);
	m_ListCtrl->SetItemState(pListView->iItem, &item);
	
	//m_ListCtrl->SetOverlayImage();
	
	
	//m_ListCtrl->SetItemState(pListView->iItem, &item);

	//m_ListCtrl->SetItemState(pListView->iItem, 3, LVIS_STATEIMAGEMASK);
	//m_ListCtrl->SetItemState(pListView->iItem, 1, LVIS_FOCUSED);
	//m_ListCtrl->SetItemState(pListView->iItem, 0, LVIS_SELECTED);
	//m_ListCtrl->SetCheck(pListView->iItem, false);

	m_pOwnerFrame->GetTextView()->ScrollToLine(nItem, true);
	
	*pResult = 0;
}

void CConsoleView::SetOwnerFrame(CChildFrame* pOwnerFrame)
{
	m_pOwnerFrame = pOwnerFrame;
}

void CConsoleView::AddMessage(CString &sMessage, UINT nType, int nLine)
{
	int n;

	switch(nType)
	{
	case INFO_MSG:
		break;
	case WARNING_MSG:
		m_nWarning++;
		break;
	case ERROR_MSG:
		m_nError++;
		break;
	}

	n = m_ListCtrl->InsertItem(m_nLast, sMessage, nType - 1);
	m_ListCtrl->SetItemData(n, nLine);
	m_nLast++;
}

void CConsoleView::AddMessage(char* pMessage, UINT nType, int nLine)
{
	int n;

	switch(nType)
	{
	case INFO_MSG:
		break;
	case WARNING_MSG:
		m_nWarning++;
		break;
	case ERROR_MSG:
		m_nError++;
		break;
	}

	n = m_ListCtrl->InsertItem(m_nLast, pMessage, nType - 1);
	m_ListCtrl->SetItemData(n, nLine);
	m_nLast++;
}

void CConsoleView::ResetConsole()
{
	m_ListCtrl->DeleteAllItems();
	m_nLast = 0;
	m_nWarning = 0;
	m_nError = 0;
}

void CConsoleView::PrintResults()
{
	CString sBuffer;
	sBuffer.Format("Verfication finished with  >>  %d error(s), %d warning(s)", m_nError, m_nWarning);
	AddMessage(sBuffer, INFO_MSG, -1);
}

int CConsoleView::GetErrors()
{
	return m_nError;
}

int CConsoleView::GetWarnings()
{
	return m_nWarning;
}
