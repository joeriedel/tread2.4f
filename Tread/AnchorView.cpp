// AnchorView.cpp : implementation file
//

#include "stdafx.h"
#include "Tread3D.h"
#include "TreadSys.h"
#include "viewControl.h"
#include "Tread3DDoc.h"
#include "ChildFrm.h"
#include "MapObject.h"
#include "Camera.h"
#include "Anchor.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAnchorView

IMPLEMENT_DYNCREATE(CAnchorView, CListView)

CAnchorView::CAnchorView()
{
	m_nItem = -1;
}

CAnchorView::~CAnchorView()
{
}

BEGIN_MESSAGE_MAP(CAnchorView, CListView)
	//{{AFX_MSG_MAP(CAnchorView)
	ON_NOTIFY_REFLECT(LVN_BEGINLABELEDIT, OnBeginlabeledit)
	ON_NOTIFY_REFLECT(LVN_ENDLABELEDIT, OnEndlabeledit)
	ON_WM_CREATE()
	ON_NOTIFY_REFLECT(NM_DBLCLK, OnDblclk)
	ON_COMMAND(ID_ANCHOR_REMOVE, OnAnchorRemove)
	ON_NOTIFY_REFLECT(NM_RCLICK, OnRclick)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAnchorView diagnostics

#ifdef _DEBUG
void CAnchorView::AssertValid() const
{
	CListView::AssertValid();
}

void CAnchorView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CAnchorView message handlers

int CAnchorView::InsertItem(CString sName)
{
	int n = m_ListCtrl->GetItemCount();
	n =	m_ListCtrl->InsertItem(n, sName, 5);

	return n;
}

int CAnchorView::InsertItem(CString sName, bool bState)
{
	int n = m_ListCtrl->GetItemCount();

	if(bState)
		n = m_ListCtrl->InsertItem(n, sName, 1);
	else
		n = m_ListCtrl->InsertItem(n, sName, 2);
	
	return n;
}

int CALLBACK CAnchorView::SortList(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	int iComp = 0;

	CString strItem1;
	CString strItem2;

	strItem1 = ((CAnchor*)lParam1)->GetName();
	strItem2 = ((CAnchor*)lParam2)->GetName();

	return strItem2.Compare(strItem1) * -1;
}

void CAnchorView::Enable(bool bEnable)
{
	EnableWindow(bEnable);
}

void CAnchorView::OnBeginlabeledit(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	GetMapManager()->EnableAccelerator(false);
	*pResult = 0;
}

void CAnchorView::OnEndlabeledit(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	
	GetMapManager()->EnableAccelerator(true);

	LVITEM hItem = pDispInfo->item;

	if(hItem.pszText == NULL)
		return;

	if(strcmp(hItem.pszText, "") == 0)
		return;

	((CAnchor*)hItem.lParam)->SetName(hItem.pszText);
	m_ListCtrl->SetItemText(hItem.iItem, 0, hItem.pszText);

	*pResult = 0;
}

void CAnchorView::OnInitialUpdate()
{
	CListView::OnInitialUpdate();
	
	m_ListCtrl->ModifyStyle(LVS_TYPEMASK, LVS_REPORT | LVS_SORTDESCENDING | LVS_EDITLABELS);
	if(!m_imgList.Create(IDB_VISGROUP, 16, 0, RGB(255, 0, 255)))
	{
		Error("Anchor Control : Could not load imagelist");
		return;
	}
	
	m_ListCtrl->SetImageList(&m_imgList, LVSIL_STATE);
	m_ListCtrl->InsertColumn(0 , "Anchor Points", LVCFMT_CENTER, 140, 1);

	Enable(false);
}

int CAnchorView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CListView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	RegisterView(ANCHOR_VIEW, this);

	m_ListCtrl = &GetListCtrl();
	
	return 0;
}

void CAnchorView::ResetContent()
{
	m_ListCtrl->DeleteAllItems();
}

void CAnchorView::LoadAnchorList()
{
	m_ListCtrl->DeleteAllItems();

	CLinkedList<CBaseObject>* pList = GetMapManager()->GetDocPtr()->GetUIObjectList();
	CBaseObject* pObject;

	pList->First();
	for(pObject = pList->GetCurItem(); pObject != NULL; pObject = pList->GetNextItem())
	{
		if(pObject->GetObjectClass() == OBJECT_CLASS_ANCHOR)
		{
			int n = InsertItem(((CAnchor*)pObject)->GetName());
			m_ListCtrl->SetItemData(n, (ULONG)pObject);
		}
	}

	m_ListCtrl->SortItems((PFNLVCOMPARE)SortList, (LPARAM)false);
}

void CAnchorView::OnDblclk(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NMLISTVIEW* pListItem = (NMLISTVIEW*)pNMHDR;

	if(pListItem->iItem == -1)
		return;

	CBaseObject* pObject = (CBaseObject*)m_ListCtrl->GetItemData(pListItem->iItem);

	CTread3DDoc* pDoc = GetMapManager()->GetDocPtr();
	pDoc->CenterViewsOnUIObject(pObject);
	pDoc->UpdateWindows(FLAT_VIEWS);

	*pResult = 0;
}

void CAnchorView::OnAnchorRemove() 
{
	if(m_nItem < 0)
		return;

	CAnchor* pAnchor = (CAnchor*)m_ListCtrl->GetItemData(m_nItem);
	m_ListCtrl->DeleteItem(m_nItem);

	CTread3DDoc* pDoc = GetMapManager()->GetDocPtr();
	CLinkedList<CBaseObject>* pList = pDoc->GetUIObjectList();
	pList->DeleteItem(pAnchor);
	pDoc->UpdateWindows(FLAT_VIEWS);
}

void CAnchorView::OnRclick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NMLISTVIEW* pListItem = (NMLISTVIEW*)pNMHDR;
	CPoint point;
	
	if(pListItem->iItem == -1)
	{
		m_nItem = -1;
		return;
	}

	m_ListCtrl->GetItemPosition(pListItem->iItem, &point);
	m_nItem = pListItem->iItem;

	CMenu commandMenu;
	ClientToScreen(&point);

	commandMenu.LoadMenu(IDR_ANCHOR_POPUP);
	CMenu *popup = commandMenu.GetSubMenu(0);
	popup->TrackPopupMenu(TPM_RIGHTALIGN | TPM_RIGHTBUTTON, 
							point.x, point.y, this);
	
	*pResult = 0;
}
