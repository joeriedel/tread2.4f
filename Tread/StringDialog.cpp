// StringDialog.cpp : implementation file
//

#include "stdafx.h"
#include "tread3d.h"
#include "StringDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStringDialog dialog


CStringDialog::CStringDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CStringDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CStringDialog)
	m_sInput = _T("");
	//}}AFX_DATA_INIT
}


void CStringDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStringDialog)
	DDX_Control(pDX, IDC_EDIT_INPUT, m_edFilter);
	DDX_Text(pDX, IDC_EDIT_INPUT, m_sInput);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CStringDialog, CDialog)
	//{{AFX_MSG_MAP(CStringDialog)
	ON_BN_CLICKED(IDC_BUTTON_DONE, OnButtonDone)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStringDialog message handlers

BOOL CStringDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	UpdateData(false);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CStringDialog::OnButtonDone() 
{
	UpdateData(true);
	OnOK();
}

void CStringDialog::SetInput(int nInput)
{
	m_sInput.Format("%d", nInput);
}

void CStringDialog::SetInput(CString sInput)
{
	m_sInput = sInput;
}

CString CStringDialog::GetInput()
{
	return m_sInput;
}

void CStringDialog::SetFilter(int nFilter)
{
	m_edFilter.SetFilter(nFilter);
}
