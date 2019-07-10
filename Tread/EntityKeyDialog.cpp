// EntityKeyDialog.cpp : implementation file
//

#include "stdafx.h"
#include "tread3d.h"
#include "EntityKeyDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEntityKeyDialog dialog


CEntityKeyDialog::CEntityKeyDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CEntityKeyDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEntityKeyDialog)
	m_sName = _T("");
	m_sValue = _T("");
	//}}AFX_DATA_INIT

	m_nType = -1;
}


void CEntityKeyDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEntityKeyDialog)
	DDX_Control(pDX, IDC_BUTTON_OK, m_btOK);
	DDX_Control(pDX, IDC_EDIT_VALUE, m_edValue);
	DDX_Control(pDX, IDC_EDIT_NAME, m_edName);
	DDX_Text(pDX, IDC_EDIT_NAME, m_sName);
	DDX_Text(pDX, IDC_EDIT_VALUE, m_sValue);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEntityKeyDialog, CDialog)
	//{{AFX_MSG_MAP(CEntityKeyDialog)
	ON_EN_CHANGE(IDC_EDIT_NAME, OnChangeEditName)
	ON_BN_CLICKED(IDC_BUTTON_OK, OnButtonOk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEntityKeyDialog message handlers

BOOL CEntityKeyDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_btOK.EnableWindow(false);

	if(m_sName.IsEmpty())
	{
		m_edValue.EnableWindow(false);
		m_btOK.EnableWindow(false);
	}
	else
	{
		m_edValue.EnableWindow();
		m_btOK.EnableWindow();
	}

	UpdateData(false);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CEntityKeyDialog::OnChangeEditName() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	UpdateData();

	if(m_sName.IsEmpty())
		m_edValue.EnableWindow(false);
	else
	{
		m_edValue.EnableWindow();
		m_btOK.EnableWindow();
	}
}

CString CEntityKeyDialog::GetKeyName()
{
	return m_sName;
}

CString CEntityKeyDialog::GetKeyValue()
{
	return m_sValue;
}

void CEntityKeyDialog::OnButtonOk() 
{
	UpdateData();
	OnOK();
}

void CEntityKeyDialog::SetKeyValues(CString sName, CString sValue)
{
	m_sName = sName;
	m_sValue = sValue;
}
