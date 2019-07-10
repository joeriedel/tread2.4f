// AnchorDialog.cpp : implementation file
//

#include "stdafx.h"
#include "tread3d.h"
#include "AnchorDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAnchorDialog dialog


CAnchorDialog::CAnchorDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CAnchorDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAnchorDialog)
	m_sAnchor = _T("");
	//}}AFX_DATA_INIT
}


void CAnchorDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAnchorDialog)
	DDX_Text(pDX, IDC_EDIT_ANCHOR, m_sAnchor);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAnchorDialog, CDialog)
	//{{AFX_MSG_MAP(CAnchorDialog)
	ON_BN_CLICKED(IDC_BUTTON_SET, OnButtonSet)
	ON_EN_CHANGE(IDC_EDIT_ANCHOR, OnChangeEditAnchor)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAnchorDialog message handlers

BOOL CAnchorDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	GetDlgItem(IDC_BUTTON_SET)->EnableWindow(false);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CAnchorDialog::OnButtonSet() 
{
	OnOK();
}

void CAnchorDialog::OnChangeEditAnchor() 
{
	UpdateData(true);

	if(m_sAnchor.IsEmpty())
		GetDlgItem(IDC_BUTTON_SET)->EnableWindow(false);
	else
		GetDlgItem(IDC_BUTTON_SET)->EnableWindow(true);
}

CString CAnchorDialog::GetAnchorName()
{
	return m_sAnchor;
}