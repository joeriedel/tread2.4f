// ObjectView.cpp : implementation file
//

#include "stdafx.h"
#include "Tread3D.h"
#include "TreadSys.h"
#include "viewControl.h"
#include "GameApi.h"
#include "Models.h"
#include "DialogControl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CObjectView

IMPLEMENT_DYNCREATE(CObjectView, CTreeView)

CObjectView::CObjectView()
{
	m_hGroup = NULL;
	m_hSelection = NULL;

	m_nMode = LIST_NONE;

	m_bLoading = false;
}

CObjectView::~CObjectView()
{
}


BEGIN_MESSAGE_MAP(CObjectView, CTreeView)
	//{{AFX_MSG_MAP(CObjectView)
	ON_WM_CREATE()
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, OnSelchanged)
	ON_NOTIFY_REFLECT(NM_RCLICK, OnRclick)
	ON_NOTIFY_REFLECT(TVN_ITEMEXPANDING, OnItemexpanding)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CObjectView drawing

/////////////////////////////////////////////////////////////////////////////
// CObjectView diagnostics

#ifdef _DEBUG
void CObjectView::AssertValid() const
{
	CTreeView::AssertValid();
}

void CObjectView::Dump(CDumpContext& dc) const
{
	CTreeView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CObjectView message handlers

BOOL CObjectView::PreCreateWindow(CREATESTRUCT& cs) 
{
	cs.style |=  TVS_HASLINES | TVS_HASBUTTONS | TVS_DISABLEDRAGDROP
			| TVS_LINESATROOT;
	return CTreeView::PreCreateWindow(cs);
}

int CObjectView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CTreeView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	m_TreeCtrl = &GetTreeCtrl();

	RegisterView(OBJECT_VIEW, this);
	
	return 0;
}

void CObjectView::OnInitialUpdate() 
{
	CTreeView::OnInitialUpdate();
	
	imgList.Create(IDB_VISGROUP, 16, 0, RGB(255, 0, 255));
	m_TreeCtrl->SetImageList(&imgList, TVSIL_NORMAL);
	m_TreeCtrl->SetIndent(2);
}

void CObjectView::InsertGroup(CString sGroup)
{
	m_hGroup = m_TreeCtrl->InsertItem(sGroup, 1, 1, TVI_ROOT, TVI_LAST);
}

HTREEITEM CObjectView::InsertItem(CString sItem)
{
	if(m_TreeCtrl->GetRootItem() == m_hGroup)
		return NULL;

	if(m_TreeCtrl->GetParentItem(m_hGroup) != NULL)
		return NULL;

	if(m_hGroup != NULL)
		return m_TreeCtrl->InsertItem(sItem, 2, 5, m_hGroup, TVI_LAST);

	return NULL;
}

HTREEITEM CObjectView::InsertItem(CString sGroup, CString sItem)
{
	if(sGroup.IsEmpty())
		return NULL;

	m_hGroup = FindCategory(sGroup);
	if(m_hGroup == NULL)
	{
		m_hGroup = m_TreeCtrl->InsertItem(sGroup, 1, 1, TVI_ROOT, TVI_LAST);
		if(m_hGroup == NULL)
			return NULL;
	}
	
	return m_TreeCtrl->InsertItem(sItem, 2, 5, m_hGroup, TVI_LAST);
}

void CObjectView::OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	HTREEITEM hOldItem;
	HTREEITEM hNewItem;

	hOldItem = pNMTreeView->itemOld.hItem;
	hNewItem = pNMTreeView->itemNew.hItem;

	//make sure the selection is an item and not a parent
	if(m_TreeCtrl->GetParentItem(hNewItem) != NULL)
	{
		//uncheck a previous selection if it exists
		if(m_hSelection != NULL)
		{
			m_TreeCtrl->SetCheck(m_hSelection, false);
			m_TreeCtrl->SetItemImage(m_hSelection, 2, 2);
		}

		//select the new item
		m_TreeCtrl->SetCheck(hNewItem, true);
		m_TreeCtrl->SetItemImage(hNewItem, 5, 5);
		m_hSelection = hNewItem;	//store the new selection
		//store the parent item
		m_hGroup = m_TreeCtrl->GetParentItem(m_hSelection);

		if(!m_bLoading)
		{
			if(m_nMode == LIST_ENTITY)
			{
				ENTITYDEFINITION* pEntity = (ENTITYDEFINITION*)m_TreeCtrl->GetItemData(m_hSelection);
				GetGameAPI()->GetCurrentGame()->SetCurrentPointEntity(pEntity);
			}
			else if(m_nMode == LIST_BRUSH)
			{
				MODEL* pModel = (MODEL*)m_TreeCtrl->GetItemData(m_hSelection);
				GetGameAPI()->GetCurrentGame()->SetCurrentModel(pModel);
			}
		}
	}

	*pResult = 0;
}

void CObjectView::LoadEntityList()
{
	if(m_nMode == LIST_ENTITY)
		return;

	HTREEITEM hItem;
	bool bSmart = GetRegBool(B_SMARTNAME);
	CGameDefinition* pGame;
	CLinkedList<ENTITYDEFINITION>* pList;
	CString sCat;

	m_bLoading = true;

	//get the current game and the current point entity
	pGame = GetGameAPI()->GetCurrentGame();
	if(pGame == NULL)
		return;
	ENTITYDEFINITION* pCurrent = pGame->GetCurrentPointEntity();
	HTREEITEM hSelItem;

	//delete all the items in the list and re add the root node
	m_TreeCtrl->DeleteAllItems();
	sCat.Format("%s (Entities)", pGame->GetGameName());
	m_TreeCtrl->InsertItem(sCat, 4, 4, TVI_ROOT, TVI_LAST);

	//acquire the entity definition list
	pList = pGame->GetEntityDefinitionList();
	if(pList == NULL)
		return;

	//set the mode before entering the loop or OnSelChanged will mess up
	m_nMode = LIST_ENTITY;
	
	pList->First();
	for(ENTITYDEFINITION* pNode = pList->GetCurItem(); pNode != NULL; pNode = pList->GetNextItem())
	{
		if(!pNode->bVirtual && !pNode->bOwner)
		{
			if(bSmart)
			{
				sCat = pNode->sCategory;
				if(sCat.IsEmpty())
					sCat = "GLOBAL";
								
				hItem = InsertItem(sCat, pNode->sSmartName);
			}
			else
			{
				if(pNode->pBaseClass != NULL)
					sCat = pNode->pBaseClass->sName;
				else
					sCat = "GLOBAL";
				hItem = InsertItem(sCat, pNode->sName);
			}
			m_TreeCtrl->SetItemData(hItem, (ULONG)pNode);
			
			if(pCurrent == pNode)
				hSelItem = hItem;
		}
	}

	m_TreeCtrl->SetCheck(hSelItem, true);
	m_TreeCtrl->SelectItem(hSelItem);
	m_bLoading = false;
}

void CObjectView::LoadBrushList()
{
	if(m_nMode == LIST_BRUSH)
		return;

	HTREEITEM hItem;
	CGameDefinition* pGame;
	CLinkedList<MODEL>* pList;
	CString sBuffer;

	m_bLoading = true;

	//get the current game and teh current model
	pGame = GetGameAPI()->GetCurrentGame();
	if(pGame == NULL)
		return;
	MODEL* pCurrent = pGame->GetCurrentModel();
	HTREEITEM hSelItem;

	m_TreeCtrl->DeleteAllItems();
	sBuffer.Format("%s (Brushes/Models)", pGame->GetGameName());
	m_TreeCtrl->InsertItem(sBuffer, 4, 4, TVI_ROOT, TVI_LAST);

	pList = pGame->GetModelList();
	if(pList == NULL)
		return;

	//set the mode before entering the loop or OnSelChanged will mess up
	m_nMode = LIST_BRUSH;
	
	pList->First();
	for(MODEL* pNode = pList->GetCurItem(); pNode != NULL; pNode = pList->GetNextItem())
	{
		hItem = InsertItem(pNode->m_Header.m_sCategory, pNode->m_Header.m_sName);
		m_TreeCtrl->SetItemData(hItem, (ULONG)pNode);
			
		if(pCurrent == pNode)
			hSelItem = hItem;
	}

	m_TreeCtrl->SetCheck(hSelItem, true);
	m_TreeCtrl->SelectItem(hSelItem);
	m_bLoading = false;
}

bool CObjectView::EntityListLoaded()
{
	if(m_nMode == LIST_ENTITY)
		return true;
	else
		return false;
}

bool CObjectView::BrushListLoaded()
{
	if(m_nMode == LIST_BRUSH)
		return true;
	else
		return false;
}


HTREEITEM CObjectView::FindCategory(CString &sItem)
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

HTREEITEM CObjectView::FindItem(CString &sItem, HTREEITEM hParent)
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

bool CObjectView::GetSelection(CString &sItem, CString &sGroup)
{
	if((m_hSelection == NULL) || (m_hGroup == NULL))
		return false;

	sItem = m_TreeCtrl->GetItemText(m_hSelection);
	sGroup = m_TreeCtrl->GetItemText(m_hGroup);

	return true;
}

void CObjectView::ResetContent()
{
	m_TreeCtrl->DeleteAllItems();
	m_nMode = LIST_NONE;
}

void CObjectView::OnRclick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	if((m_nMode == LIST_ENTITY) || (m_nMode == LIST_NONE))
		return;

	ShowDialog(DLG_PREVIEW, true);
	*pResult = 0;
}

void CObjectView::OnItemexpanding(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;

	HTREEITEM hOldItem;
	HTREEITEM hNewItem;
	
	hOldItem = pNMTreeView->itemOld.hItem;
	hNewItem = pNMTreeView->itemNew.hItem;

	if(hNewItem != NULL)
	{
		if(pNMTreeView->action == 1)
			m_TreeCtrl->SetItemImage(hNewItem, 1, 1);
		else if(pNMTreeView->action == 2)
			m_TreeCtrl->SetItemImage(hNewItem, 0, 0);
	}

	*pResult = 0;
}
