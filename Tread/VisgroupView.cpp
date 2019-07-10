// VisgroupView.cpp : implementation file
//

#include "stdafx.h"
#include "Tread3D.h"
#include "ObjectDialog.h"
#include "treadsys.h"
#include "viewControl.h"
#include "Visgroup.h"
#include "StringDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CVisgroupView

IMPLEMENT_DYNCREATE(CVisgroupView, CTreeView)

CVisgroupView::CVisgroupView()
{
	//m_hCategory = NULL;
	m_hSelect = NULL;
	m_hDrag = NULL;

	m_pImgDrag = NULL;
	m_nDelayInterval =500;
	m_nScrollInterval = 200;
	m_nScrollMargin = 10;
	m_bDragging = false;
	nTimer = -1;
}

CVisgroupView::~CVisgroupView()
{
}

BEGIN_MESSAGE_MAP(CVisgroupView, CTreeView)
	//{{AFX_MSG_MAP(CVisgroupView)
	ON_WM_RBUTTONDOWN()
	ON_WM_CREATE()
	ON_COMMAND(ID_VISGROUP_ADDCAT, OnVisgroupAddcat)
	ON_NOTIFY_REFLECT(TVN_BEGINLABELEDIT, OnBeginlabeledit)
	ON_NOTIFY_REFLECT(TVN_ENDLABELEDIT, OnEndlabeledit)
	ON_COMMAND(ID_VISGROUP_HIDEALL, OnVisgroupHideall)
	ON_COMMAND(ID_VISGROUP_SHOWALL, OnVisgroupShowall)
	ON_NOTIFY_REFLECT(TVN_ITEMEXPANDING, OnItemexpanding)
	ON_NOTIFY_REFLECT(TVN_BEGINDRAG, OnBegindrag)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_TIMER()
	ON_WM_LBUTTONDOWN()
	ON_COMMAND(IDM_VISGROUP_DELETE, OnVisgroupDelete)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVisgroupView drawing

/////////////////////////////////////////////////////////////////////////////
// CVisgroupView diagnostics

#ifdef _DEBUG
void CVisgroupView::AssertValid() const
{
	CTreeView::AssertValid();
}

void CVisgroupView::Dump(CDumpContext& dc) const
{
	CTreeView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CVisgroupView message handlers

void CVisgroupView::OnInitialUpdate() 
{
	CTreeView::OnInitialUpdate();

	m_ImgList.Create(IDB_VISGROUP, 16, 0, RGB(255, 0, 255));
	m_TreeCtrl->SetImageList(&m_ImgList, TVSIL_NORMAL);
	m_TreeCtrl->InsertItem("Visgroups", 4, 4, TVI_ROOT, TVI_LAST);
}

void CVisgroupView::OnRButtonDown(UINT nFlags, CPoint point) 
{
	CTreeView::OnRButtonDown(nFlags, point);

	CMenu commandMenu;
	ClientToScreen(&point);

	commandMenu.LoadMenu(IDR_VISGROUP_POPUP);
	CMenu *popup = commandMenu.GetSubMenu(0);
	popup->TrackPopupMenu(TPM_RIGHTALIGN | TPM_RIGHTBUTTON, 
							point.x, point.y, this);
}

HTREEITEM CVisgroupView::InsertItem(CString sGroup, CString sItem, bool bVisible)
{
	HTREEITEM hItem;
	HTREEITEM hGroup;

	if(sGroup.IsEmpty())
		return NULL;

	hGroup = FindVisgroup(sGroup, TVI_ROOT);
	if(hGroup == NULL)
	{
		hGroup = m_TreeCtrl->InsertItem(sGroup, 1, 1, TVI_ROOT, TVI_LAST);
		if(hGroup == NULL)
			return NULL;
	}

	if(!sItem.IsEmpty())
	{
		if(bVisible)
			hItem = m_TreeCtrl->InsertItem(sItem, 3, 3, hGroup, TVI_LAST);
		else
			hItem = m_TreeCtrl->InsertItem(sItem, 2, 2, hGroup, TVI_LAST);
		m_TreeCtrl->SetCheck(hItem, bVisible);
		m_TreeCtrl->SortChildren(hGroup);
	}
	
	return hItem;
}

HTREEITEM CVisgroupView::InsertItem(HTREEITEM hGroup, CString sItem, bool bVisible)
{
	HTREEITEM hItem;
	
	if(!sItem.IsEmpty())
	{
		if(bVisible)
			hItem = m_TreeCtrl->InsertItem(sItem, 3, 3, hGroup, TVI_LAST);
		else
			hItem = m_TreeCtrl->InsertItem(sItem, 2, 2, hGroup, TVI_LAST);
		m_TreeCtrl->SetCheck(hItem, bVisible);
		m_TreeCtrl->SortChildren(hGroup);
	}
	
	return hItem;
}

BOOL CVisgroupView::PreCreateWindow(CREATESTRUCT& cs) 
{
	cs.style |=  TVS_HASLINES | TVS_HASBUTTONS
			| TVS_EDITLABELS | TVS_LINESATROOT;
	cs.style &= ~TVS_DISABLEDRAGDROP;

	return CTreeView::PreCreateWindow(cs);
}

int CVisgroupView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CTreeView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	m_TreeCtrl = &GetTreeCtrl();
	RegisterView(VISGROUP_VIEW, this);

	return 0;
}

void CVisgroupView::OnVisgroupAddcat() 
{
	CString sGroup;
	char szBuffer[4];
	HTREEITEM hFound;

	for(int n = 1; n < 101; n++)
	{
		itoa(n, szBuffer, 10);
		sGroup = "Category";
		sGroup += szBuffer;

		hFound = FindVisgroup(sGroup, TVI_ROOT);
		if(hFound != NULL)
			continue;
		else
			break;
	}

	if(n == 100)
		return;
	
	InsertItem(sGroup, "");
}

void CVisgroupView::OnBeginlabeledit(NMHDR* pNMHDR, LRESULT* pResult) 
{
	TV_DISPINFO* pTVDispInfo = (TV_DISPINFO*)pNMHDR;
	HTREEITEM hItem = pTVDispInfo->item.hItem;
	
	if(hItem == TVI_ROOT)
	{
		*pResult = 1;
		return;
	}
	
	m_hSelect = hItem;
	GetMapManager()->EnableAccelerator(false);
	*pResult = 0;
}

void CVisgroupView::OnEndlabeledit(NMHDR* pNMHDR, LRESULT* pResult) 
{
	TV_DISPINFO* pDispInfo = (TV_DISPINFO*)pNMHDR;
	//enable the mainframe accelerator
	GetMapManager()->EnableAccelerator(true);

	HTREEITEM hItem = pDispInfo->item.hItem;
	if(hItem == NULL)
		return;

	//check if the string null
	if(pDispInfo->item.pszText == NULL)
		return;

	if(strcmp(pDispInfo->item.pszText, "") == 0)
		return;

	CString sText = pDispInfo->item.pszText;
	//check if the string exceeds the maximum length
	if(sText.GetLength() > 50)
	{
		MessageBeep(MB_OK);
		Warning("VISGROUP: Name exceeds maximum length of 50 characters.\n");
		return;
	}

	HTREEITEM hDup;
	HTREEITEM hParent = m_TreeCtrl->GetParentItem(hItem);
	if(hParent == NULL)
		hParent = TVI_ROOT;

	hDup = FindVisgroup(sText, hParent);
	if(hDup != NULL)
	{
		if(hParent == TVI_ROOT)
		{
			MessageBeep(MB_OK);
			Warning("VISGROUP: Category with same name found. Cannot rename.\n");
			return;
		}
		
		int nReply = MessageBox("Another visgroup with the same name was found. Would you like to merge them?",
							"Duplicate Visgroup", MB_OKCANCEL);
		if(nReply == IDOK)
		{
			CVisgroup* pVisSrc = (CVisgroup*)m_TreeCtrl->GetItemData(hItem);
			CVisgroup* pVisDest = (CVisgroup*)m_TreeCtrl->GetItemData(hDup);
			GetVisgroupAPI()->CombineVisgroups(pVisDest, pVisSrc);
			m_TreeCtrl->DeleteItem(hItem);		//delete the old item
		}

		return;
	}
	
	//set the tree items text and the visgroups text
	if(hParent == TVI_ROOT)
	{
		m_TreeCtrl->SetItemText(hItem, sText);
		CVisgroup* pVisgroup;
		HTREEITEM hChild = m_TreeCtrl->GetChildItem(hItem);
		if(hChild == NULL)
			return;
		
		for(hChild =  m_TreeCtrl->GetChildItem(hItem); hChild != NULL; hChild = m_TreeCtrl->GetNextSiblingItem(hChild))
		{
			pVisgroup = (CVisgroup*)m_TreeCtrl->GetItemData(hChild);
			pVisgroup->SetCategory(sText);
		}
	}
	else
	{
		CVisgroup* pVisgroup = (CVisgroup*)m_TreeCtrl->GetItemData(hItem);
		pVisgroup->SetName(sText);
		m_TreeCtrl->SetItemText(hItem, sText);
	}

	GetMapManager()->GetDocPtr()->UpdateWindows();
	*pResult = 0;
}

/*HTREEITEM CVisgroupView::FindItem(CString &sItem, HTREEITEM hItem)
{
	if(hItem == NULL)
		return NULL;

	if(m_TreeCtrl->ItemHasChildren(hItem))
		FindItem(sItem, m_TreeCtrl->GetChildItem(hItem));

	if(m_TreeCtrl->GetItemText(hItem) == sItem)
		return hItem;
	else
		return FindItem(sItem, m_TreeCtrl->GetNextItem(hItem, TVGN_NEXT));
}*/

void CVisgroupView::OnVisgroupHideall() 
{
	HTREEITEM hParent = m_TreeCtrl->GetChildItem(TVI_ROOT);
	if(hParent == NULL)
		return;

	for(; hParent != NULL; hParent = m_TreeCtrl->GetNextItem(hParent, TVGN_NEXT))
	{
		Debug("\nParent: " + m_TreeCtrl->GetItemText(hParent));
		HTREEITEM hChild = m_TreeCtrl->GetChildItem(hParent);
		if(hChild == NULL)
			continue;

		for(; hChild != NULL; hChild = m_TreeCtrl->GetNextItem(hChild, TVGN_NEXT))
		{
			//Debug("\nChild: " + m_TreeCtrl->GetItemText(hChild));
			m_TreeCtrl->SetCheck(hChild, false);
			m_TreeCtrl->SetItemImage(hChild, 2, 2);
			CVisgroup* pVisgroup = (CVisgroup*)m_TreeCtrl->GetItemData(hChild);
			pVisgroup->SetVisible(false);
		}
	}

	GetMapManager()->GetDocPtr()->BuildHotSpots();
	GetMapManager()->GetDocPtr()->UpdateWindows();
}

void CVisgroupView::OnVisgroupShowall() 
{
	HTREEITEM hParent = m_TreeCtrl->GetChildItem(TVI_ROOT);
	if(hParent == NULL)
		return;

	for(; hParent != NULL; hParent = m_TreeCtrl->GetNextItem(hParent, TVGN_NEXT))
	{
		HTREEITEM hChild = m_TreeCtrl->GetChildItem(hParent);
		if(hChild == NULL)
			continue;

		for(; hChild != NULL; hChild = m_TreeCtrl->GetNextItem(hChild, TVGN_NEXT))
		{
			m_TreeCtrl->SetCheck(hChild, true);
			m_TreeCtrl->SetItemImage(hChild, 3, 3);
			CVisgroup* pVisgroup = (CVisgroup*)m_TreeCtrl->GetItemData(hChild);
			pVisgroup->SetVisible(true);
		}
	}

	GetMapManager()->GetDocPtr()->BuildHotSpots();
	GetMapManager()->GetDocPtr()->UpdateWindows();
}

HTREEITEM CVisgroupView::FindCategory(CString &sItem)
{
	if(sItem.IsEmpty())
		return NULL;

	HTREEITEM hItem = m_TreeCtrl->GetChildItem(TVI_ROOT);
	if(hItem == NULL)
		return NULL;

	for(; hItem != NULL; hItem = m_TreeCtrl->GetNextSiblingItem(hItem))
	{
		if(sItem == m_TreeCtrl->GetItemText(hItem))
			return hItem;
	}

	return NULL;
}

HTREEITEM CVisgroupView::FindVisgroup(CString &sItem, HTREEITEM hParent)
{
	if((hParent == NULL) || sItem.IsEmpty())
		return NULL;

	HTREEITEM hItem = m_TreeCtrl->GetChildItem(hParent);
	if(hItem == NULL)
		return NULL;

	for(; hItem != NULL; hItem = m_TreeCtrl->GetNextSiblingItem(hItem))
	{
		if(sItem == m_TreeCtrl->GetItemText(hItem))
			return hItem;
	}

	return NULL;
}

void CVisgroupView::ResetContent()
{
	m_TreeCtrl->DeleteAllItems();
}

void CVisgroupView::OnItemexpanding(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	HTREEITEM hNewItem;
	
	hNewItem = pNMTreeView->itemNew.hItem;

	if(hNewItem != NULL)
	{
		if(pNMTreeView->action == 1)
			m_TreeCtrl->SetItemImage(hNewItem, 1, 1);
		else if(pNMTreeView->action == 2)
		{
			if(m_TreeCtrl->ItemHasChildren(hNewItem))
				m_TreeCtrl->SetItemImage(hNewItem, 0, 0);
			else
				m_TreeCtrl->SetItemImage(hNewItem, 1, 1);
		}
	}

	*pResult = 0;
}

void CVisgroupView::LoadVisgroup()
{
	CVisgroupAPI* pVisgroup = GetVisgroupAPI();
	if(pVisgroup == NULL)
		return;

	m_TreeCtrl->DeleteAllItems();
	m_TreeCtrl->InsertItem("Visgroups", 4, 4, TVI_ROOT, TVI_LAST);

	CLinkedList<CVisgroup>* pList = pVisgroup->GetVisgroupList();
	CVisgroup* pVis = NULL;
	pList->First();
	HTREEITEM hItem;

	for(pVis = pList->GetCurItem(); pVis != NULL; pVis = pList->GetNextItem())
	{
		hItem = InsertItem(pVis->GetCategory(), pVis->GetName(), pVis->GetVisible());
		m_TreeCtrl->SetItemData(hItem, (ULONG)pVis);
	}

	//m_TreeCtrl->SortChildren(NULL);
}

void CVisgroupView::UpdateVisgroup()
{
	CVisgroupAPI* pVisgroup = GetVisgroupAPI();
	if(pVisgroup == NULL)
		return;

	//m_TreeCtrl->DeleteAllItems();
	//m_TreeCtrl->InsertItem("Visgroups", 4, 4, TVI_ROOT, TVI_LAST);

	CLinkedList<CVisgroup>* pList = pVisgroup->GetVisgroupList();
	CVisgroup* pVis = NULL;
	pList->First();
	HTREEITEM hItem;
	HTREEITEM hCat;

	for(pVis = pList->GetCurItem(); pVis != NULL; pVis = pList->GetNextItem())
	{
		hCat = FindCategory(pVis->GetCategory());
		if(hCat == NULL)
		{
			hItem = InsertItem(pVis->GetCategory(), pVis->GetName(), pVis->GetVisible());
			m_TreeCtrl->SetItemData(hItem, (ULONG)pVis);
		}
		else
		{
			hItem = FindVisgroup(pVis->GetName(), hCat);
			if(hItem == NULL)
			{
				hItem = InsertItem(hCat, pVis->GetName(), pVis->GetVisible());
				m_TreeCtrl->SetItemData(hItem, (ULONG)pVis);
			}
		}
	}

	//m_TreeCtrl->SortChildren(NULL);
}

void CVisgroupView::OnBegindrag(NMHDR* pNMHDR, LRESULT* pResult) 
{
	*pResult = 0;
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	HTREEITEM hItem = pNMTreeView->itemNew.hItem;
	int img, imgs;

	if(hItem == NULL)
		return;

	if(m_TreeCtrl->GetParentItem(hItem) == NULL)
		return;
	
	m_TreeCtrl->GetItemImage(hItem, img, imgs);
	if((img != 2) && (img != 3))
		return;
	
	
	m_pImgDrag = m_TreeCtrl->CreateDragImage (hItem);
	if (m_pImgDrag != NULL)		 // Just to be sure
	{
		CRect rcItem;
        // Compute the coordinates of the “hot spot”—the location of the
        // cursor relative to the upper left corner of the item rectangle.
        m_TreeCtrl->GetItemRect (hItem, rcItem, true);
        CPoint point (pNMTreeView->ptDrag.x, pNMTreeView->ptDrag.y);
        CPoint hotSpot = point;
        hotSpot.x -= rcItem.left;
        hotSpot.y -= rcItem.top;

        // Convert the client coordinates in “point” to coordinates relative
        // to the upper left corner of the control window.
        CPoint client (0, 0);
        ClientToScreen (&client);
        GetWindowRect (rcItem);
        point.x += client.x - rcItem.left;
        point.y += client.y - rcItem.top;

        // Capture the mouse and begin dragging.
        SetCapture ();
        m_pImgDrag->BeginDrag (0, hotSpot);
        m_pImgDrag->DragEnter (this, point);
        m_hDrag = hItem;
        m_bDragging = true;
    }
}

void CVisgroupView::OnMouseMove(UINT nFlags, CPoint point) 
{
	CTreeView::OnMouseMove(nFlags, point);

	if(m_bDragging && (m_pImgDrag != NULL))
	{
		KillTimer(1);

		m_pImgDrag->DragMove(point);
		HTREEITEM hItem = HighlightDropTarget(point);

		//::SetCursor(hItem == NULL ? AfxGetApp()->LoadStandardCursor(IDC_NO) : (HCURSOR)::GetClassLong(m_hWnd, GCL_HCURSOR));

		CRect rcClient;
		GetClientRect(rcClient);
		int cy = rcClient.Height();
		if((point.y >= 0 && point.y <= m_nScrollMargin) || (point.y >= cy - m_nScrollMargin && point.y <= cy)
			|| (hItem != NULL && m_TreeCtrl->ItemHasChildren(hItem) && !IsItemExpanded(hItem)))
			nTimer = SetTimer(1, m_nDelayInterval, NULL);
	}
	else
	{
		m_bDragging = false;
		KillTimer(nTimer);
	}
}

void CVisgroupView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CTreeView::OnLButtonUp(nFlags, point);

	if(m_bDragging && (m_pImgDrag != NULL))
	{
		KillTimer(nTimer);

		m_pImgDrag->DragLeave(this);
		m_pImgDrag->EndDrag();

		ReleaseCapture();

		m_bDragging = false;
		m_TreeCtrl->SelectDropTarget(NULL);
		delete m_pImgDrag;
		m_pImgDrag = NULL;

		int img, imgs;
		UINT nFlags;
		HTREEITEM hItem = m_TreeCtrl->HitTest(point, &nFlags);
		if(hItem == NULL)
			return;

		m_TreeCtrl->GetItemImage(hItem, img, imgs);
		if((img != 0) && (img != 1))
			return;

		if(hItem == m_hDrag)
		{
			MessageBeep(MB_OK);
			Warning("VISGROUP: An item cannot be dropped on itself.\n");
			return;
		}
		else if(hItem == m_TreeCtrl->GetParentItem(m_hDrag))
		{
			MessageBeep(MB_OK);
			Warning("VISGROUP: An item cannot be dropped on it's parent.\n");
			return;
		}

		//Get the category name
		CString sCategory;
		sCategory = m_TreeCtrl->GetItemText(hItem);

		//before the new item gets moved, check for a duplicate
		HTREEITEM hDup = FindVisgroup(m_TreeCtrl->GetItemText(m_hDrag), hItem);
		if(hDup != NULL)
		{
			int nReply = MessageBox("Another visgroup with the same name was found. Would you like to merge them?",
								"Duplicate Visgroup", MB_OKCANCEL);
			if(nReply == IDOK)
			{
				//merge the two visgroups
				CVisgroup* pVisSrc = (CVisgroup*)m_TreeCtrl->GetItemData(m_hDrag);
				CVisgroup* pVisDest = (CVisgroup*)m_TreeCtrl->GetItemData(hDup);
				GetVisgroupAPI()->CombineVisgroups(pVisDest, pVisSrc);
				m_TreeCtrl->DeleteItem(m_hDrag);//delete the old item
			}
		}
		else
		{
			CVisgroup* pVisgroup = (CVisgroup*)m_TreeCtrl->GetItemData(m_hDrag);
			pVisgroup->SetCategory(m_TreeCtrl->GetItemText(hItem));
			MoveTree(hItem, m_hDrag);
		}

		m_hSelect = m_hDrag;
		m_hDrag = NULL;
	}
	else
	{
		m_bDragging = false;
		KillTimer(nTimer);
	}

	GetMapManager()->GetDocPtr()->UpdateWindows();
}

void CVisgroupView::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	CTreeView::OnTimer(nIDEvent);

	if(m_bDragging == false)
	{
		KillTimer(nTimer);
		return;
	}

	// Reset the timer.
    nTimer = SetTimer (1, m_nScrollInterval, NULL);

    // Get the current cursor position and window height.
    DWORD dwPos = ::GetMessagePos ();
    CPoint point (LOWORD (dwPos), HIWORD (dwPos));
    ScreenToClient (&point);
    CRect rect;
    GetClientRect (rect);
    int cy = rect.Height ();
    // Scroll the window if the cursor is near the top or bottom.

    if (point.y >= 0 && point.y <= m_nScrollMargin)
	{
        HTREEITEM hFirstVisible = m_TreeCtrl->GetFirstVisibleItem();
        m_pImgDrag->DragShowNolock (FALSE);
        SendMessage (WM_VSCROLL, MAKEWPARAM (SB_LINEUP, 0), NULL);
        m_pImgDrag->DragShowNolock (TRUE);

        // Kill the timer if the window did not scroll, or redraw the
        // drop target highlight if the window did scroll.

        if (m_TreeCtrl->GetFirstVisibleItem() == hFirstVisible)
            KillTimer (nTimer);
        else
		{
            HighlightDropTarget(point);
            return;
        }
    }
    else if (point.y >= cy - m_nScrollMargin && point.y <= cy)
	{
        HTREEITEM hFirstVisible = m_TreeCtrl->GetFirstVisibleItem ();
        m_pImgDrag->DragShowNolock (FALSE);
        SendMessage (WM_VSCROLL, MAKEWPARAM (SB_LINEDOWN, 0), NULL);
        m_pImgDrag->DragShowNolock (TRUE);

        // Kill the timer if the window did not scroll, or redraw the
        // drop target highlight if the window did scroll.

		if (m_TreeCtrl->GetFirstVisibleItem() == hFirstVisible)
            KillTimer (nTimer);
        else 
		{
            HighlightDropTarget (point);
            return;
        }
    }

    // If the cursor is hovering over a collapsed item, expand the tree.
    UINT nFlags;
    HTREEITEM hItem = m_TreeCtrl->HitTest (point, &nFlags);

    if (hItem != NULL && m_TreeCtrl->ItemHasChildren (hItem) && !IsItemExpanded (hItem))
	{
        m_pImgDrag->DragShowNolock (FALSE);
        m_TreeCtrl->Expand (hItem, TVE_EXPAND);
        m_pImgDrag->DragShowNolock (TRUE);
        KillTimer (1);

        return;
    }
}

bool CVisgroupView::IsChildOf(HTREEITEM hItem1, HTREEITEM hItem2)
{
    HTREEITEM hParent = hItem1;
    while ((hParent = m_TreeCtrl->GetParentItem (hParent)) != NULL)
	{
        if (hParent == hItem2)
            return TRUE;
    }
    return FALSE;
}

void CVisgroupView::MoveTree(HTREEITEM hDest, HTREEITEM hSrc)
{
	int nImage, nSelectedImage;
    m_TreeCtrl->GetItemImage (hSrc, nImage, nSelectedImage);
    CString string = m_TreeCtrl->GetItemText (hSrc);
    // Create an exact copy of the item at the destination.
    HTREEITEM hNewItem = m_TreeCtrl->InsertItem (string, nImage, nSelectedImage, hDest);
    // If the item has subitems, copy them, too.
    // Select the newly added item.
    m_TreeCtrl->SelectItem (hNewItem);
	m_TreeCtrl->SetItemData(hNewItem, m_TreeCtrl->GetItemData(hSrc));
	m_hSelect = hNewItem;

    m_TreeCtrl->DeleteItem (hSrc);
}

void CVisgroupView::CopyTree(HTREEITEM hDest, HTREEITEM hSrc)
{
    // Get the attributes of item to be copied.
    int nImage, nSelectedImage;
    m_TreeCtrl->GetItemImage (hSrc, nImage, nSelectedImage);
    CString string = m_TreeCtrl->GetItemText (hSrc);
    // Create an exact copy of the item at the destination.
    HTREEITEM hNewItem = m_TreeCtrl->InsertItem (string, nImage, nSelectedImage, hDest);
    // If the item has subitems, copy them, too.
    if (m_TreeCtrl->ItemHasChildren (hSrc))
        CopyChildren (hNewItem, hSrc);
    // Select the newly added item.
    m_TreeCtrl->SelectItem (hNewItem);
}

void CVisgroupView::CopyChildren(HTREEITEM hDest, HTREEITEM hSrc)
{
    // Get the first subitem.
    HTREEITEM hItem = m_TreeCtrl->GetChildItem (hSrc);
    ASSERT (hItem != NULL);
    // Create a copy of it at the destination.
    int nImage, nSelectedImage;
    m_TreeCtrl->GetItemImage (hItem, nImage, nSelectedImage);
    CString string = m_TreeCtrl->GetItemText (hItem);
    HTREEITEM hNewItem = m_TreeCtrl->InsertItem (string, nImage, nSelectedImage, hDest);
    // If the subitem has subitems, copy them, too.
    if (m_TreeCtrl->ItemHasChildren (hItem))
        CopyChildren (hNewItem, hItem);
    // Do the same for other subitems of hSrc.
    while ((hItem = m_TreeCtrl->GetNextSiblingItem (hItem)) != NULL) {
        m_TreeCtrl->GetItemImage (hItem, nImage, nSelectedImage);
        string = m_TreeCtrl->GetItemText (hItem);
        hNewItem = m_TreeCtrl->InsertItem (string, nImage, nSelectedImage, hDest);
        if (m_TreeCtrl->ItemHasChildren (hItem))
            CopyChildren (hNewItem, hItem);
    }
}

bool CVisgroupView::IsItemExpanded(HTREEITEM hItem)
{
    return m_TreeCtrl->GetItemState (hItem, TVIS_EXPANDED) & TVIS_EXPANDED;
}

HTREEITEM CVisgroupView::HighlightDropTarget(CPoint point)
{
    // Find out which item (if any) the cursor is over.
	int img, imgs;
    UINT nFlags;
    HTREEITEM hItem = m_TreeCtrl->HitTest (point, &nFlags);

	m_TreeCtrl->GetItemImage(hItem, img, imgs);
	if((img != 0) && (img != 1))
		return hItem;

    // Highlight the item, or unhighlight all items if the cursor isn’t
    // over an item.
    m_pImgDrag->DragShowNolock (FALSE);
    m_TreeCtrl->SelectDropTarget (hItem);
    m_pImgDrag->DragShowNolock (TRUE);
    // Return the handle of the highlighted item.
    return hItem;
}

void CVisgroupView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CTreeView::OnLButtonDown(nFlags, point);

	int img, imgs;
	TVHITTESTINFO hitTest;
	hitTest.pt = point;
	m_TreeCtrl->HitTest(&hitTest);
	m_hSelect = hitTest.hItem;

	if(hitTest.hItem == TVI_ROOT)
		return;

	m_hSelect = hitTest.hItem;
	if(m_hSelect == NULL)
		return;

	m_TreeCtrl->GetItemImage(m_hSelect, img, imgs);
	if((img != 2) && (img != 3))
		return;
	
	if(hitTest.flags & TVHT_ONITEMICON)
	{
		bool bState = !m_TreeCtrl->GetCheck(m_hSelect);
		CVisgroup* pVisgroup = (CVisgroup*)m_TreeCtrl->GetItemData(m_hSelect);
		pVisgroup->SetVisible(bState);
				
		m_TreeCtrl->SetCheck(m_hSelect, bState);
		if(bState)
			m_TreeCtrl->SetItemImage(m_hSelect, 3, 3);
		else
			m_TreeCtrl->SetItemImage(m_hSelect, 2, 2);
	}

	GetMapManager()->GetDocPtr()->BuildHotSpots();
	GetMapManager()->GetDocPtr()->UpdateWindows();
}

void CVisgroupView::OnVisgroupDelete() 
{
	if(m_hSelect == NULL)
	{
		Warning("VISGROUP: No item selected for removal.\n");
		return;
	}

	if(m_TreeCtrl->ItemHasChildren(m_hSelect))
	{
		Warning("VISGROUP: Cannot remove category with existing visgroups.\n");
		return;
	}

	//do this if the item is a parent since it does not contain a valid pointer
	HTREEITEM hParent = m_TreeCtrl->GetParentItem(m_hSelect);
	if(hParent == NULL)
	{
		m_TreeCtrl->DeleteItem(m_hSelect);	//delete the item
		return;
	}

	CVisgroup* pVisgroup = (CVisgroup*)m_TreeCtrl->GetItemData(m_hSelect);
	pVisgroup->SetVisible(true);
	GetVisgroupAPI()->DeleteVisgroup(pVisgroup);
	m_TreeCtrl->DeleteItem(m_hSelect);	//delete the item
	if(!m_TreeCtrl->ItemHasChildren(hParent))
		m_TreeCtrl->SetItemImage(hParent, 1, 1);
	
	GetMapManager()->GetDocPtr()->BuildHotSpots();
	GetMapManager()->GetDocPtr()->UpdateWindows();
}
