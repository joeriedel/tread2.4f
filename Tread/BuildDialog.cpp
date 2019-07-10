// BuildDialog.cpp : implementation file
//

#include "stdafx.h"
#include "tread3d.h"
#include "BuildDialog.h"
#include "GameAPI.h"
#include "TreadSys.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

////////////////////////////////////////////////////////////////////////////
// CBuildDialog dialog


CBuildDialog::CBuildDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CBuildDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBuildDialog)
	m_sCommand = _T("");
	//}}AFX_DATA_INIT

	m_bMode = false;
	m_nValue = 0;
	m_nLastPos = -1;
}


void CBuildDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBuildDialog)
	DDX_Control(pDX, IDC_LIST_OPTOIN, m_lbOption);
	DDX_Control(pDX, IDC_EDIT_COMMAND, m_edCommand);
	DDX_Control(pDX, IDC_LIST_SELECTION, m_lcSettings);
	DDX_Control(pDX, IDC_BUTTON_BUILD, m_btBuild);
	DDX_Text(pDX, IDC_EDIT_COMMAND, m_sCommand);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBuildDialog, CDialog)
	//{{AFX_MSG_MAP(CBuildDialog)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON_BUILD, OnButtonBuild)
	ON_NOTIFY(NM_CLICK, IDC_LIST_SELECTION, OnClickListSelection)
	ON_BN_CLICKED(IDC_BUTTON_CANCEL, OnButtonCancel)
	ON_LBN_SELCHANGE(IDC_LIST_OPTOIN, OnSelchangeListOptoin)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBuildDialog message handlers

HBRUSH CBuildDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	if((nCtlColor == CTLCOLOR_LISTBOX) && (nCtlColor == CTLCOLOR_EDIT))
		pDC->SetTextColor(RGB(0, 50, 150));
	return hbr;
}

BOOL CBuildDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_lcSettings.InsertColumn(0, "", LVCFMT_LEFT, 150, -1);

	m_imgList.Create(IDB_BITMAP_CHECK, 16, 0, RGB(255, 0, 255));
	m_lcSettings.SetImageList(&m_imgList, LVSIL_STATE);
	m_lcSettings.SetTextColor(RGB(0,50,150));
	
	CLinkedList<DEFMODULE>* pList = GetGameAPI()->GetCurrentGame()->GetDefBuild()->pModuleList;
	DEFMODULE* pModule;

	int n;

	if(pList->IsEmpty())
	{
		m_btBuild.EnableWindow(false);
		m_lbOption.EnableWindow(false);
		m_sCommand = "No build utilities defined. Please check the script.";
	}
	else
	{
		pList->First();
		for(pModule = pList->GetCurItem(); pModule != NULL; pModule = pList->GetNextItem())
		{
			if(pModule->sRunTitle.IsEmpty())
				continue;

			n = m_lbOption.AddString(pModule->sRunTitle);
			m_lbOption.SetItemDataPtr(n, pModule);
		}

		m_sCommand = "Select a build option to modify.";
	}

	m_lcSettings.EnableWindow(false);
	m_edCommand.SetReadOnly(true);
	UpdateData(false);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CBuildDialog::OnButtonBuild() 
{
	if(m_nLastPos != -1)	//take care of last set of flags
	{
		DEFMODULE* pModule = (DEFMODULE*)m_lbOption.GetItemDataPtr(m_nLastPos);
		UpdateData(true);
		GetMapManager()->GetDocPtr()->SaveRunState(pModule->sName, m_nValue, m_sCommand);
	}

	OnOK();
}

void CBuildDialog::OnClickListSelection(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NMLISTVIEW* pNMItem = (NMLISTVIEW*)pNMHDR;
	LVHITTESTINFO hitTest;
	int nPos;

	hitTest.pt = pNMItem->ptAction;
	m_lcSettings.SubItemHitTest(&hitTest);
	nPos = hitTest.iItem;

	if(nPos == -1)
		return;

	int nValue = m_lcSettings.GetItemData(nPos);

	if(!m_lcSettings.GetCheck(nPos))	//selecting options
	{
		m_nValue |= nValue;
		m_lcSettings.SetCheck(nPos, true);
	}
	else		//unselecting options
	{
		m_nValue &= ~nValue;
		m_lcSettings.SetCheck(nPos, false);
	}

	*pResult = 0;
}

void CBuildDialog::ClearFlags()
{
	for(int n = 0; n < m_lcSettings.GetItemCount(); n++)
		m_lcSettings.SetCheck(n, false);
}

void CBuildDialog::OnButtonCancel() 
{
	OnCancel();
}

void CBuildDialog::OnSelchangeListOptoin() 
{
	int nSelect = m_lbOption.GetCurSel();
	if(nSelect == -1)
		return;

	DEFMODULE* pModule = (DEFMODULE*)m_lbOption.GetItemDataPtr(nSelect);
	DEFPARAM* pParam = pModule->pDefParam;

	if((pParam->pParamList == NULL) || (pParam == NULL))
		return;

	if(m_nLastPos != -1)	//take care of last set of flags
	{
		DEFMODULE* pModule = (DEFMODULE*)m_lbOption.GetItemDataPtr(m_nLastPos);
		DEFPARAM* pParam = pModule->pDefParam;
		if(pParam == NULL)
			return;

		UpdateData(true);
		GetMapManager()->GetDocPtr()->SaveRunState(pModule->sName, m_nValue, m_sCommand);
	}
	m_nLastPos = nSelect;

	//get the flags from the document
	m_nValue = GetMapManager()->GetDocPtr()->GetRunStateValue(pModule->sName);
	m_sCommand = GetMapManager()->GetDocPtr()->GetRunStateCmdLine(pModule->sName);
	
	m_lcSettings.DeleteAllItems();	//delete all previous entries

	CLinkedList<DEFPARAMPARAM>* pFlagList = pParam->pParamList;
	DEFPARAMPARAM* pItem;
	int n = 0;
	int k;

	pFlagList->First();
	for(pItem = pFlagList->GetCurItem(); pItem != NULL; pItem = pFlagList->GetNextItem())
	{
		if(pItem->sTitle.IsEmpty())
			continue;

		k = m_lcSettings.InsertItem(n, pItem->sTitle);
		m_lcSettings.SetItemData(k, pItem->nValue);

		if(pItem->nValue & m_nValue)
			m_lcSettings.SetCheck(n, true);
		else
			m_lcSettings.SetCheck(n, false);
		
		n++;
	}

	m_lcSettings.EnableWindow(true);
	m_edCommand.SetReadOnly(false);
	UpdateData(false);
}
