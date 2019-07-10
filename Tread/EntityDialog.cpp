// EntityDialog.cpp : implementation file
//

#include "stdafx.h"
#include "tread3d.h"
#include "EntityDialog.h"
#include "TreadSys.h"
#include "GameApi.h"
#include "AngleDialog.h"
#include "FlagsDialog.h"
#include "OptionsDialog.h"
#include "StringDialog.h"
#include "EntityKeyDialog.h"
#include "ColorCycleController.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEntityDialog dialog


CEntityDialog::CEntityDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CEntityDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEntityDialog)
	m_sEntity = _T("");
	//}}AFX_DATA_INIT

	m_bLink = false;
	m_bSmart = true;
	m_hSelection = NULL;
	m_hGroup = NULL;
	m_pEntity = NULL;
	m_bWorldspawn = false;
}


void CEntityDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEntityDialog)
	DDX_Control(pDX, IDC_TREE_ENTITY, m_tcEntity);
	DDX_Control(pDX, IDC_LIST_PROPERTIES, m_lcProps);
	DDX_Text(pDX, IDC_STATIC_ENTITY, m_sEntity);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CEntityDialog, CDialog)
	//{{AFX_MSG_MAP(CEntityDialog)
	ON_NOTIFY(NM_CLICK, IDC_LIST_PROPERTIES, OnClickListProperties)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_PROPERTIES, OnDblclkListProperties)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_ENTITY, OnSelchangedTreeEntity)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_PROPERTIES, OnRclickListProperties)
	ON_BN_CLICKED(IDC_BUTTON_ADDKEY, OnButtonAddkey)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEntityDialog message handlers

BOOL CEntityDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();

	if(m_pEntity == NULL)
		return false;

	// Kill the radioactive thing while this is up.
	// wayne fix.
	GetColorCycleController()->StartAnimating(false);

	//find out if the smart name should be displayed
	m_bSmart = GetRegBool(B_SMARTNAME);
	
	//init the list control
	if(!m_imgList.Create(IDB_ENTITY, 16, 0, RGB(255, 0, 255)))
	{
		Error("Entity Dialog : Could not load imagelist");
		return false;
	}
	m_lcProps.SetImageList(&m_imgList, LVSIL_SMALL);
	m_lcProps.InsertColumn(0 , "Property", LVCFMT_CENTER, 140, 1);
	m_lcProps.InsertColumn(1 , "Value", LVCFMT_CENTER, 160, 2);

	//init the tree control
	if(!m_imgList2.Create(IDB_VISGROUP, 16, 0, RGB(255, 0, 255)))
	{
		Error("Entity Dialog : Could not load imagelist");
		return false;
	}
	m_tcEntity.SetImageList(&m_imgList2, TVSIL_NORMAL);
	m_tcEntity.SetIndent(2);

	//find out if the entity we received is point or solid
	if(m_pEntity->GetEntityDefinition()->bOwner)
		m_bLink = true;
	else
		m_bLink = false;

	if(m_bWorldspawn)
	{
		HTREEITEM hItem;
		CGameDefinition* pGame;

		pGame = GetGameAPI()->GetCurrentGame();
		if(pGame == NULL)
			return false;

		m_tcEntity.InsertItem(pGame->GetGameName(), 4, 4, TVI_ROOT, TVI_LAST);

		ENTITYDEFINITION* pDefinition = m_pEntity->GetEntityDefinition();
		if(pDefinition == NULL)
			return false;
		
		if(m_bSmart)
			hItem = InsertItem("Map", pDefinition->sSmartName);
		else
			hItem = InsertItem("Map", pDefinition->sName);

		if(hItem == NULL)
		{
			Error("Worldspawn entity is NULL.");
			return false;
		}
	
		m_tcEntity.SetItemData(hItem, (ULONG)pDefinition);
		m_tcEntity.SetCheck(hItem, true);
		m_tcEntity.SelectItem(hItem);
		m_hSelection = hItem;
		LoadProperties();
	}
	else
	{
		//load the entity list
		LoadEntityList();
	}

	m_lcProps.SetHoverTime(500);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CEntityDialog::OnClickListProperties(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NMLISTVIEW* pNMItem = (NMLISTVIEW*)pNMHDR;
	LVHITTESTINFO hitTest;

	hitTest.pt = pNMItem->ptAction;
	m_lcProps.SubItemHitTest(&hitTest);

	if(hitTest.iItem == -1)
		return;

	m_lcProps.SetHotItem(hitTest.iItem);

	*pResult = 0;
}

void CEntityDialog::OnDblclkListProperties(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NMLISTVIEW* pNMItem = (NMLISTVIEW*)pNMHDR;

	LVHITTESTINFO hitTest;
	CString sBuffer;
	CString sItem;
	hitTest.pt = pNMItem->ptAction;
	m_lcProps.SubItemHitTest(&hitTest);

	if(hitTest.iItem == -1)
		return;

	m_lcProps.SetItemState(-1, LVIS_SELECTED | LVIS_FOCUSED, 1);
	m_lcProps.SetHotItem(hitTest.iItem);

	ENTITYKEY* pKey = (ENTITYKEY*)m_lcProps.GetItemData(hitTest.iItem);
	ENTITYKEYDEFINITION* pKeyDef;
	
	
	
	if(pKey == NULL)
		return;

	if(pKey->pKeyDef == NULL)
	{
		CEntityKeyDialog keyDlg;
		keyDlg.SetKeyValues(pKey->sName, pKey->sValue);
		keyDlg.DoModal();

		pKey->sName = keyDlg.GetKeyName();
		pKey->sValue = keyDlg.GetKeyValue();

		m_lcProps.SetItemText(hitTest.iItem, 0, pKey->sName);
	}
	else
	{
		pKeyDef = pKey->pKeyDef;
		sItem = m_lcProps.GetItemText(hitTest.iItem, 1);

		if(pKeyDef->nType == EKD_TYPE_ANGLE)
		{
			CAngleDialog angleDlg;
			angleDlg.SetAngle(sItem);
			angleDlg.DoModal();
			pKey->sValue = angleDlg.GetAngle();
		}
		else if(pKeyDef->nType == EKD_TYPE_FLAGS)
		{
			if(pKeyDef->pFlagDef->bType)
			{
				CFlagsDialog flagsDlg;
				flagsDlg.SetFlags(sItem);
				flagsDlg.SetFlagsList(pKeyDef->pFlagDef->pFlagList);
				flagsDlg.DoModal();
				pKey->sValue = flagsDlg.GetFlags();
			}
			else
			{
				COptionsDialog optionsDlg;
				optionsDlg.SetOption(sItem);
				optionsDlg.SetOptionsList(pKeyDef->pFlagDef->pFlagList);
				optionsDlg.DoModal();
				pKey->sValue = optionsDlg.GetOption();
			}
		}
		else if(pKeyDef->nType == EKD_TYPE_VECTOR)
		{
			CStringDialog stringDlg;
			stringDlg.SetInput(sItem);
			stringDlg.SetFilter(FILTER_CHAR | FILTER_PUNCTUATION);
			stringDlg.DoModal();
			pKey->sValue = stringDlg.GetInput();
		}
		else if(pKeyDef->nType == EKD_TYPE_STRING)
		{
			CStringDialog stringDlg;
			stringDlg.SetInput(sItem);
			stringDlg.DoModal();
			pKey->sValue = stringDlg.GetInput();
		}
		else if(pKeyDef->nType == EKD_TYPE_INTEGER)
		{
			CStringDialog stringDlg;
			stringDlg.SetInput(sItem);
			stringDlg.SetFilter(FILTER_CHAR | FILTER_PUNCTUATION);
			stringDlg.DoModal();
			pKey->sValue = stringDlg.GetInput();
		}
		else if(pKeyDef->nType == EKD_TYPE_Q2COLOR)
		{
			float fRed, fGreen, fBlue;
			int nCount;
			CColorDialog colorDlg;

			nCount = sItem.Find(' ');
			sBuffer = sItem.Left(nCount);
			fRed = atof(sBuffer);
			sItem.Delete(0, nCount + 1);

			nCount = sItem.Find(' ');
			sBuffer = sItem.Left(nCount);
			fGreen = atof(sBuffer);
			sItem.Delete(0, nCount + 1);

			nCount = sItem.Find(' ');
			if(nCount == -1)
				nCount = sItem.GetLength();
			sBuffer = sItem.Left(nCount);
			fBlue = atof(sBuffer);

			colorDlg.m_cc.Flags = CC_FULLOPEN | CC_RGBINIT;
			colorDlg.m_cc.rgbResult = RGB((int)(fRed * 255), (int)(fGreen * 255), (int)(fBlue* 255));
			colorDlg.DoModal();
			UINT nColor = colorDlg.GetColor();
			float r, g, b;
			b = ((float)((nColor & 0xFF0000) >> 16)) / 255;
			g = ((float)((nColor & 0xFF00) >> 8)) / 255;
			r = ((float)(nColor & 0xFF)) / 255;

			sBuffer.Format("%f %f %f", r, g, b);
			pKey->sValue = sBuffer;
		}
		else if(pKeyDef->nType == EKD_TYPE_HLCOLOR)
		{
			int nRed, nGreen, nBlue, nCount;
			CColorDialog colorDlg;
			
			nCount = sItem.Find(' ');
			sBuffer = sItem.Left(nCount);
			nRed = atoi(sBuffer);
			sItem.Delete(0, nCount + 1);

			nCount = sItem.Find(' ');
			sBuffer = sItem.Left(nCount);
			nGreen = atoi(sBuffer);
			sItem.Delete(0, nCount + 1);

			nCount = sItem.Find(' ');
			if(nCount == -1)
				nCount = sItem.GetLength();
			sBuffer = sItem.Left(nCount);
			nBlue = atoi(sBuffer);

			colorDlg.m_cc.Flags = CC_FULLOPEN | CC_RGBINIT;
			colorDlg.m_cc.rgbResult = RGB(nRed, nGreen, nBlue);
			colorDlg.DoModal();
			UINT nColor = colorDlg.GetColor();
			sBuffer.Format("%u %u %u 128", nColor & 0xFF, (nColor & 0xFF00) >> 8, (nColor & 0xFF0000) >> 16);
			pKey->sValue = sBuffer;
		}
		else
		{
			Error("Invalid Key Type");
			return;
		}
	}

	//save the value to the list
	m_lcProps.SetItemText(hitTest.iItem, 1, pKey->sValue);

	*pResult = 0;
}

void CEntityDialog::LoadProperties()
{
	ENTITYDEFINITION* pDef;
	ENTITYKEYDEFINITION* pKeyDef;
	ENTITYKEY* pKey;
	CLinkedList<ENTITYKEY>* pKeyList;
	CString sBuffer;
	int i = 0;
	int n;

	m_lcProps.DeleteAllItems();
	pDef = (ENTITYDEFINITION*)m_tcEntity.GetItemData(m_hSelection);
	if(pDef == NULL)
		return;
	
	if(m_pEntity == NULL)
		return;

	pKeyList = m_pEntity->GetKeyList();
	if(pKeyList != NULL)
	{
		pKeyList->First();
		
		for(pKey = pKeyList->GetCurItem(); pKey != NULL; pKey = pKeyList->GetNextItem())
		{
			pKeyDef = pKey->pKeyDef;
			if(pKeyDef != NULL)
			{
				if(pKeyDef->sSmartName == "$CONTENTS_77X")
					continue;
				if(pKeyDef->sSmartName == "$SURFACE_77X")
					continue;

				n = m_lcProps.InsertItem(i, pKeyDef->sSmartName, pKeyDef->nType - 1);
				m_lcProps.SetItemData(n, (ULONG)pKey);
				m_lcProps.SetItemText(n, 1, pKey->sValue);
			}
			else
			{
				n = m_lcProps.InsertItem(i, pKey->sName, 1);
				m_lcProps.SetItemData(n, (ULONG)pKey);
				m_lcProps.SetItemText(n, 1, pKey->sValue);
			}

			i++;
		}
	}

	if(m_bSmart)
		m_sEntity.Format("%s\n%d Key(s)", pDef->sName, i);
	else
		m_sEntity.Format("%s\n%d Key(s)", pDef->sSmartName, i);
	UpdateData(false);
}

void CEntityDialog::SetEntity(CEntity* pEntity, bool bWorldspawn)
{
	m_bWorldspawn = bWorldspawn;

	m_pEntity = (CEntity*)pEntity->Clone();
	if(m_pEntity == NULL)
		return;
}

CEntity* CEntityDialog::GetEntity()
{
	return m_pEntity;
}

HTREEITEM CEntityDialog::InsertItem(CString sGroup, CString sItem)
{
	if(sGroup.IsEmpty())
		return NULL;

	m_hGroup = FindCategory(sGroup);
	if(m_hGroup == NULL)
	{
		m_hGroup = m_tcEntity.InsertItem(sGroup, 1, 0, TVI_ROOT, TVI_LAST);
		if(m_hGroup == NULL)
			return NULL;
	}
	
	return m_tcEntity.InsertItem(sItem, 2, 5, m_hGroup, TVI_LAST);
}

void CEntityDialog::LoadEntityList()
{
	HTREEITEM hItem;
	CGameDefinition* pGame;
	CLinkedList<ENTITYDEFINITION>* pList;
	CString sCat;

	pGame = GetGameAPI()->GetCurrentGame();
	if(pGame == NULL)
		return;

	m_tcEntity.DeleteAllItems();
	m_tcEntity.InsertItem(pGame->GetGameName(), 4, 4, TVI_ROOT, TVI_LAST);

	pList = pGame->GetEntityDefinitionList();
	if(pList == NULL)
		return;

	pList->First();

	ENTITYDEFINITION* pDef = m_pEntity->GetEntityDefinition();
	if(pDef == NULL)
	{
		return;
	}

	ENTITYDEFINITION* pNode = pList->GetCurItem();
	for(; pNode != NULL; pNode = pList->GetNextItem())
	{
		if(!pNode->bVirtual)
		{
			if(pNode->bOwner != m_bLink)
				continue;

			if(m_bSmart)
			{
				sCat = pNode->sCategory;
				if(sCat.IsEmpty())
					sCat = "GLOBAL";
								
				hItem = InsertItem(sCat, pNode->sSmartName);
				m_tcEntity.SetItemData(hItem, (ULONG)pNode);
				if((pDef->sSmartName == pNode->sSmartName) && (pNode->sCategory == pDef->sCategory))
				{
					m_tcEntity.SetCheck(hItem, true);
					m_tcEntity.SelectItem(hItem);
					m_hSelection = hItem;
					LoadProperties();
				}
			}
			else
			{
				if(pNode->pBaseClass != NULL)
					sCat = pNode->pBaseClass->sName;
				else
					sCat = "GLOBAL";
				hItem = InsertItem(sCat, pNode->sName);
				m_tcEntity.SetItemData(hItem, (ULONG)pNode);
				if(pDef->sName == pNode->sName)
				{
					//if(pNode->pBaseClass != NULL)
					//	if(pNode->pBaseClass->sName == pDef->pBaseClass->sName)

					m_tcEntity.SetCheck(hItem, true);
					m_tcEntity.SelectItem(hItem);
					m_hSelection = hItem;
					LoadProperties();					
				}
			}
		}
	}
}

HTREEITEM CEntityDialog::FindCategory(CString &sItem)
{
	if(sItem.IsEmpty())
		return NULL;

	HTREEITEM hItem = m_tcEntity.GetChildItem(TVI_ROOT);
	if(hItem == NULL)
		return NULL;

	for(; hItem != NULL; hItem = m_tcEntity.GetNextSiblingItem(hItem))
	{
		if(sItem == m_tcEntity.GetItemText(hItem))
			return hItem;
	}

	return NULL;
}

void CEntityDialog::OnSelchangedTreeEntity(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	HTREEITEM hOldItem;
	HTREEITEM hNewItem;

	hOldItem = pNMTreeView->itemOld.hItem;
	hNewItem = m_tcEntity.GetSelectedItem();

	//make sure the selection is an item and not a parent
	if(m_tcEntity.GetParentItem(hNewItem) != NULL)
	{
		//uncheck a previous selection if it exists
		if(m_hSelection != NULL)
		{
			m_tcEntity.SetCheck(m_hSelection, false);
			m_tcEntity.SetItemImage(m_hSelection, 2, 2);
		}

		//select the new item
		m_tcEntity.SetCheck(hNewItem, true);
		m_tcEntity.SetItemImage(hNewItem, 5, 5);
		m_hSelection = hNewItem;	//store the new selection
		//store the parent item
		m_hGroup = m_tcEntity.GetParentItem(m_hSelection);
		ENTITYDEFINITION* pDef = (ENTITYDEFINITION*)m_tcEntity.GetItemData(m_hSelection);

		if(m_pEntity != NULL)
		{
			if(m_pEntity->GetEntityDefinition() != pDef)
			{
				delete m_pEntity;
				m_pEntity = (CEntity*)NewBaseObject(OBJECT_CLASS_ENTITY);
				m_pEntity->InitFromDefinition(pDef);
			}
		}
		else
		{
			m_pEntity = (CEntity*)NewBaseObject(OBJECT_CLASS_ENTITY);
			m_pEntity->InitFromDefinition(pDef);
		}


		LoadProperties();
	}

	*pResult = 0;
}

void CEntityDialog::OnRclickListProperties(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NMLISTVIEW* pNMItem = (NMLISTVIEW*)pNMHDR;

	LVHITTESTINFO hitTest;
	CString sBuffer;
	CString sItem;
	CStringDialog stringDlg;
	int nItem;

	//perform a hittest to find the item the use clicked on
	hitTest.pt = pNMItem->ptAction;
	m_lcProps.SubItemHitTest(&hitTest);

	nItem = hitTest.iItem;
	if(nItem == -1) //no item was clicked on
		return;

	//change the state of the item
	LVITEM item;
	memset(&item, 0, sizeof(LVITEM));
	item.iItem = nItem;
	item.mask = LVIF_STATE;
	item.stateMask = LVIS_FOCUSED;
	item.state = -1;

	m_lcProps.SetItemState(nItem, &item);
	m_lcProps.SetHotItem(nItem);

	//get the pointer associated with the item
	ENTITYKEY* pKey = (ENTITYKEY*)m_lcProps.GetItemData(nItem);
	ENTITYKEYDEFINITION* pKeyDef;
	if(pKey == NULL)
		return;

	 if(pKey->pKeyDef == NULL)
	 {
		 stringDlg.SetFilter(0);
	 }
	 else
	 {
		pKeyDef = pKey->pKeyDef;

		//change the filter based on the key type
		if(pKeyDef->nType == EKD_TYPE_ANGLE)
			stringDlg.SetFilter(FILTER_CHAR | FILTER_PUNCTUATION);
		else if(pKeyDef->nType == EKD_TYPE_FLAGS)
			stringDlg.SetFilter(FILTER_CHAR | FILTER_PUNCTUATION);
		else if(pKeyDef->nType == EKD_TYPE_VECTOR)
			stringDlg.SetFilter(FILTER_CHAR | FILTER_PUNCTUATION);
		else if(pKeyDef->nType == EKD_TYPE_STRING)
			stringDlg.SetFilter(0);
		else if(pKeyDef->nType == EKD_TYPE_INTEGER)
			stringDlg.SetFilter(FILTER_CHAR | FILTER_PUNCTUATION);
		else if(pKeyDef->nType == EKD_TYPE_Q2COLOR)
			stringDlg.SetFilter(FILTER_CHAR | FILTER_PUNCTUATION);
		else if(pKeyDef->nType == EKD_TYPE_HLCOLOR)
			stringDlg.SetFilter(FILTER_CHAR | FILTER_PUNCTUATION);
		else
		{
			Error("Entity Dialog: Undefined key type found.");
			return;
		}
	 }

	//get the text of the item
	sItem = m_lcProps.GetItemText(nItem, 1);

	//set the string dialog memeber
	stringDlg.SetInput(sItem);
	stringDlg.DoModal();

	//get the value back from string dialog and set the items text
	pKey->sValue = stringDlg.GetInput();
	m_lcProps.SetItemText(nItem, 1, pKey->sValue);

	*pResult = 0;
}

void CEntityDialog::OnButtonAddkey() 
{
	CEntityKeyDialog dlgTemp;

	if(dlgTemp.DoModal() == IDOK)
	{
		ENTITYKEY* pKey = new ENTITYKEY();
		pKey->sName = dlgTemp.GetKeyName();
		pKey->sValue = dlgTemp.GetKeyValue();

		CLinkedList<ENTITYKEY>* pKeyList;
		pKeyList = m_pEntity->GetKeyList();
		if(pKeyList == NULL)
			return;

		pKeyList->AddItem(pKey);
		int n = m_lcProps.InsertItem(0, pKey->sName, EKD_TYPE_STRING - 1);
		m_lcProps.SetItemData(n, (ULONG)pKey);
		m_lcProps.SetItemText(n, 1, pKey->sValue);
	}
}

void CEntityDialog::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	if(GetRegBool(B_ANIMATECOLORS))
		GetMapManager()->GetDocPtr()->CycleColorIfSelected();
}
