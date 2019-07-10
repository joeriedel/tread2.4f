// HollowDialog.cpp : implementation file
//

#include "stdafx.h"
#include "tread3d.h"
#include "HollowDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHollowDialog dialog


CHollowDialog::CHollowDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CHollowDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CHollowDialog)
	m_fUnit = 16.0f;
	//}}AFX_DATA_INIT
}


void CHollowDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHollowDialog)
	DDX_Control(pDX, IDC_EDIT_UNITS, m_edUnit);
	DDX_Control(pDX, IDC_BUTTON_HOLLOW, m_btHollow);
	DDX_Text(pDX, IDC_EDIT_UNITS, m_fUnit);
	DDV_MinMaxFloat(pDX, m_fUnit, -128.f, 128.f);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CHollowDialog, CDialog)
	//{{AFX_MSG_MAP(CHollowDialog)
	ON_BN_CLICKED(IDC_BUTTON_HOLLOW, OnButtonHollow)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHollowDialog message handlers

void CHollowDialog::OnButtonHollow() 
{
	// TODO: Add your control notification handler code here
	OnOK();
}

BOOL CHollowDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	UpdateData(false);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CHollowDialog::SetUnit(float fUnit)
{
	m_fUnit = fUnit;
}

float CHollowDialog::GetUnit()
{
	return m_fUnit;
}