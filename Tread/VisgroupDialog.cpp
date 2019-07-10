// VisgroupDialog.cpp : implementation file
//

#include "stdafx.h"
#include "tread3d.h"
#include "VisgroupDialog.h"
#include "Visgroup.h"
#include "TreadSys.h"
#include "ViewControl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CVisgroupDialog dialog


CVisgroupDialog::CVisgroupDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CVisgroupDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CVisgroupDialog)
	m_sCategory = _T("");
	m_sVisgroup = _T("");
	//}}AFX_DATA_INIT

	m_bFound1 = false;
	m_bFound2 = false;
}


void CVisgroupDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CVisgroupDialog)
	DDX_Control(pDX, IDC_COMBOBOX_VISGROUP, m_cbVisgroup);
	DDX_Control(pDX, IDC_COMBOBOX_CATEGORY, m_cbCategory);
	DDX_Control(pDX, IDC_BUTTON_OK, m_btAdd);
	DDX_CBString(pDX, IDC_COMBOBOX_CATEGORY, m_sCategory);
	DDX_CBString(pDX, IDC_COMBOBOX_VISGROUP, m_sVisgroup);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CVisgroupDialog, CDialog)
	//{{AFX_MSG_MAP(CVisgroupDialog)
	ON_CBN_EDITCHANGE(IDC_COMBOBOX_CATEGORY, OnEditchangeComboboxCategory)
	ON_CBN_SELCHANGE(IDC_COMBOBOX_CATEGORY, OnSelchangeComboboxCategory)
	ON_CBN_EDITCHANGE(IDC_COMBOBOX_VISGROUP, OnEditchangeComboboxVisgroup)
	ON_CBN_SELCHANGE(IDC_COMBOBOX_VISGROUP, OnSelchangeComboboxVisgroup)
	ON_BN_CLICKED(IDC_BUTTON_OK, OnButtonOk)
	ON_BN_CLICKED(IDC_BUTTON_CANCEL, OnButtonCancel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVisgroupDialog message handlers

BOOL CVisgroupDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_btAdd.EnableWindow(false);
	m_btAdd.SetWindowText("Waiting...");
	m_cbVisgroup.EnableWindow(false);

	m_cbVisgroup.SetHighlightColor(RGB(0,0,255));
	m_cbCategory.SetHighlightColor(RGB(0,0,255));

	CVisgroupAPI* pVisgroup = GetVisgroupAPI();
	if(pVisgroup == NULL)
		return false;

	CLinkedList<CVisgroup>* pList = pVisgroup->GetVisgroupList();
	CVisgroup* pVis = NULL;
	pList->First();
	int nItem;

	for(pVis = pList->GetCurItem(); pVis != NULL; pVis = pList->GetNextItem())
	{
		nItem = m_cbCategory.FindStringExact(-1, pVis->GetCategory());
		if(nItem == -1)
			nItem = m_cbCategory.AddString(pVis->GetCategory());
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CVisgroupDialog::OnEditchangeComboboxCategory() 
{
	// TODO: Add your control notification handler code here
	UpdateData(true);

	if(m_sCategory.IsEmpty())
	{
		m_btAdd.EnableWindow(false);
		m_btAdd.SetWindowText("Waiting...");
		m_bFound1 = false;
		m_cbCategory.SetHighlight(false);
		m_cbVisgroup.ResetContent();
		m_cbVisgroup.EnableWindow(false);
		return;
	}
	
	m_cbVisgroup.EnableWindow(true);
	if(m_sVisgroup.IsEmpty())
		m_btAdd.EnableWindow(false);
	else
		m_btAdd.EnableWindow(true);

	m_cbVisgroup.ResetContent();
	int nItem = m_cbCategory.FindStringExact(-1, m_sCategory);
	if(nItem == -1)
	{
		m_bFound1 = false;
		m_cbCategory.SetHighlight(false);
		m_btAdd.SetWindowText("Create");
		return;
	}

	m_bFound1 = true;
	m_cbCategory.SetHighlight(true);
	if(m_bFound2)
		m_btAdd.SetWindowText("Add");
		
	CVisgroupAPI* pVisgroup = GetVisgroupAPI();
	if(pVisgroup == NULL)
		return;

	CLinkedList<CVisgroup>* pList = pVisgroup->GetVisgroupList();
	CVisgroup* pVis = NULL;
	pList->First();
	
	for(pVis = pList->GetCurItem(); pVis != NULL; pVis = pList->GetNextItem())
	{
		if(pVis->GetCategory() == m_sCategory)
		{
			nItem = m_cbVisgroup.AddString(pVis->GetName());
			//m_cbVisgroup.SetItemData(nItem, (ULONG)pVis);
		}
	}
}

void CVisgroupDialog::OnSelchangeComboboxCategory() 
{
	// TODO: Add your control notification handler code here
	int nSelect = m_cbCategory.GetCurSel();
	if(nSelect == -1)
		return;

	m_cbVisgroup.EnableWindow(true);
	m_cbCategory.GetLBText(nSelect, m_sCategory);
	m_cbCategory.SetHighlight(true);
	m_bFound1 = true;
	if(m_bFound2)
		m_btAdd.SetWindowText("Add");
	else
		m_btAdd.SetWindowText("Create");

	CVisgroupAPI* pVisgroup = GetVisgroupAPI();
	if(pVisgroup == NULL)
		return;

	m_cbVisgroup.ResetContent();
	CLinkedList<CVisgroup>* pList = pVisgroup->GetVisgroupList();
	CVisgroup* pVis = NULL;
	int nItem;
	pList->First();
	
	for(pVis = pList->GetCurItem(); pVis != NULL; pVis = pList->GetNextItem())
	{
		if(pVis->GetCategory() == m_sCategory)
		{
			nItem = m_cbVisgroup.AddString(pVis->GetName());
			//m_cbVisgroup.SetItemData(nItem, (ULONG)pVis);
		}
	}

	if(m_sCategory.IsEmpty() || m_sVisgroup.IsEmpty())
		m_btAdd.EnableWindow(false);
	else
		m_btAdd.EnableWindow(true);
}

void CVisgroupDialog::OnEditchangeComboboxVisgroup() 
{
	// TODO: Add your control notification handler code here
	UpdateData(true);

	if(m_sVisgroup.IsEmpty())
	{
		m_btAdd.EnableWindow(false);
		m_btAdd.SetWindowText("Waiting...");
		m_bFound2 = false;
		m_cbVisgroup.SetHighlight(false);
		//m_cbVisgroup.ResetContent();
		//m_cbVisgroup.EnableWindow(false);
		return;
	}
	
	//m_cbVisgroup.EnableWindow(true);
	if(m_sCategory.IsEmpty())
		m_btAdd.EnableWindow(false);
	else
		m_btAdd.EnableWindow(true);

	//m_cbVisgroup.ResetContent();
	int nItem = m_cbVisgroup.FindStringExact(-1, m_sVisgroup);
	if(nItem == -1)
	{
		m_bFound2 = false;
		m_cbVisgroup.SetHighlight(false);
		m_btAdd.SetWindowText("Create");
		return;
	}

	m_bFound2 = true;
	m_cbVisgroup.SetHighlight(true);
	if(m_bFound1)
		m_btAdd.SetWindowText("Add");
}

void CVisgroupDialog::OnSelchangeComboboxVisgroup() 
{
	// TODO: Add your control notification handler code here
	int nSelect = m_cbVisgroup.GetCurSel();
	if(nSelect == -1)
		return;

	m_cbVisgroup.GetLBText(nSelect, m_sVisgroup);

	m_bFound2 = true;
	m_cbVisgroup.SetHighlight(true);

	if(m_bFound1)
		m_btAdd.SetWindowText("Add");
	else
		m_btAdd.SetWindowText("Create");

	if(m_sCategory.IsEmpty() || m_sVisgroup.IsEmpty())
		m_btAdd.EnableWindow(false);
	else
		m_btAdd.EnableWindow(true);
}

void CVisgroupDialog::OnButtonOk() 
{
	UpdateData(true);
	GetMapManager()->GetDocPtr()->SetSelectedObjectsVisgroup(m_sVisgroup, m_sCategory);
	CVisgroupView* pVisgroupView = (CVisgroupView*)GetView(VISGROUP_VIEW);
	pVisgroupView->UpdateVisgroup();
	OnOK();
}

void CVisgroupDialog::OnButtonCancel() 
{
	// TODO: Add your control notification handler code here
	OnCancel();
}
