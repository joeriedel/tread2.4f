// ModelDialog.cpp : implementation file
//

#include "stdafx.h"
#include "tread3d.h"
#include "ModelDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CModelDialog dialog


CModelDialog::CModelDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CModelDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CModelDialog)
	m_sCategory = _T("");
	m_sName = _T("");
	m_nType = -1;
	//}}AFX_DATA_INIT
}


void CModelDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CModelDialog)
	DDX_Control(pDX, IDC_EDIT_NAME, m_edName);
	DDX_Control(pDX, IDC_EDIT_CATEGORY, m_edCategory);
	DDX_Control(pDX, IDC_BUTTON_CREATE, m_btCreate);
	DDX_Text(pDX, IDC_EDIT_CATEGORY, m_sCategory);
	DDX_Text(pDX, IDC_EDIT_NAME, m_sName);
	DDX_Radio(pDX, IDC_RADIO_BRUSH, m_nType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CModelDialog, CDialog)
	//{{AFX_MSG_MAP(CModelDialog)
	ON_BN_CLICKED(IDC_BUTTON_CREATE, OnButtonCreate)
	ON_BN_CLICKED(IDC_BUTTON_CANCEL, OnButtonCancel)
	ON_EN_CHANGE(IDC_EDIT_CATEGORY, OnChangeEditCategory)
	ON_EN_CHANGE(IDC_EDIT_NAME, OnChangeEditName)
	ON_BN_CLICKED(IDC_RADIO_BRUSH, OnRadioBrush)
	ON_BN_CLICKED(IDC_RADIO_MODEL, OnRadioBrush)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CModelDialog message handlers

void CModelDialog::OnButtonCreate() 
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
	OnOK();
}

void CModelDialog::OnButtonCancel() 
{
	// TODO: Add your control notification handler code here
	OnCancel();
}

BOOL CModelDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_edCategory.SetFilter(FILTER_DASH | FILTER_PUNCTUATION);
	m_edCategory.SetFilter(FILTER_DASH | FILTER_PUNCTUATION);

	m_btCreate.EnableWindow(false);	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

CString CModelDialog::GetCategory()
{
	return m_sCategory;
}

CString CModelDialog::GetName()
{
	return m_sName;
}

int CModelDialog::GetType()
{
	return m_nType;
}

void CModelDialog::OnChangeEditCategory() 
{
	UpdateData(true);

	if((m_nType != -1) && (!m_sCategory.IsEmpty()) && (!m_sName.IsEmpty()))
		m_btCreate.EnableWindow(true);
	else
		m_btCreate.EnableWindow(false);	
}

void CModelDialog::OnChangeEditName() 
{
	UpdateData(true);
	
	if((m_nType != -1) && (!m_sCategory.IsEmpty()) && (!m_sName.IsEmpty()))
		m_btCreate.EnableWindow(true);
	else
		m_btCreate.EnableWindow(false);
}

void CModelDialog::OnRadioBrush() 
{
	UpdateData(true);
	
	if((m_nType != -1) && (!m_sCategory.IsEmpty()) && (!m_sName.IsEmpty()))
		m_btCreate.EnableWindow(true);
	else
		m_btCreate.EnableWindow(false);
}
