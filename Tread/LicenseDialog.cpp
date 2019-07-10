// LicenseDialog.cpp : implementation file
//

#include "stdafx.h"
#include "tread3d.h"
#include "LicenseDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLicenseDialog dialog


CLicenseDialog::CLicenseDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CLicenseDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLicenseDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_bHide = false;
}

void CLicenseDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLicenseDialog)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLicenseDialog, CDialog)
	//{{AFX_MSG_MAP(CLicenseDialog)
	ON_BN_CLICKED(IDC_BUTTON_CANCEL, OnButtonCancel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLicenseDialog message handlers

void CLicenseDialog::OnButtonCancel() 
{
	OnCancel();	
}

void CLicenseDialog::HideCancelButton(bool bHide)
{
	m_bHide = bHide;	
}

BOOL CLicenseDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if(m_bHide)
		GetDlgItem(IDC_BUTTON_CANCEL)->ShowWindow(false);
	else
		GetDlgItem(IDC_BUTTON_CANCEL)->ShowWindow(true);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
