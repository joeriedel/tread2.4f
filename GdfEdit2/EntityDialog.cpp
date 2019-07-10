// EntityDialog.cpp : implementation file
//

#include "stdafx.h"
#include "GdfEdit2.h"
#include "EntityDialog.h"

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
	m_sDescribe = _T("");
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CEntityDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEntityDialog)
	
	DDX_Control(pDX, IDC_LIST_SELECT, m_ltSelect);
	DDX_Text(pDX, IDC_STATIC_DESCRIPTION, m_sDescribe);
	//}}AFX_DATA_MAP

	DDX_Text(pDX, IDC_STATIC_NAME1, m_sName[0]);
	DDX_Text(pDX, IDC_STATIC_NAME2, m_sName[1]);
	DDX_Text(pDX, IDC_STATIC_NAME3, m_sName[2]);
	DDX_Text(pDX, IDC_STATIC_NAME4, m_sName[3]);

	DDX_Text(pDX, IDC_EDIT_PARAM1, m_sParam[0]);
	DDX_Text(pDX, IDC_EDIT_PARAM2, m_sParam[1]);
	DDX_Text(pDX, IDC_EDIT_PARAM3, m_sParam[2]);
	DDX_Text(pDX, IDC_EDIT_PARAM4, m_sParam[3]);
	
	DDX_Control(pDX, IDC_BUTTON_DEFUALT4, m_btDefault[3]);
	DDX_Control(pDX, IDC_BUTTON_DEFUALT3, m_btDefault[2]);
	DDX_Control(pDX, IDC_BUTTON_DEFUALT2, m_btDefault[1]);
	DDX_Control(pDX, IDC_BUTTON_DEFUALT1, m_btDefault[0]);
}

BEGIN_MESSAGE_MAP(CEntityDialog, CDialog)
	//{{AFX_MSG_MAP(CEntityDialog)
	ON_WM_CTLCOLOR()
	ON_LBN_SELCHANGE(IDC_LIST_SELECT, OnSelchangeListSelect)
	ON_BN_CLICKED(IDC_BUTTON_FINISH, OnButtonFinish)
	ON_BN_CLICKED(IDC_BUTTON_DEFUALT1, OnButtonDefualt1)
	ON_BN_CLICKED(IDC_BUTTON_DEFUALT2, OnButtonDefualt2)
	ON_BN_CLICKED(IDC_BUTTON_DEFUALT3, OnButtonDefualt3)
	ON_BN_CLICKED(IDC_BUTTON_DEFUALT4, OnButtonDefualt4)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEntityDialog message handlers

HBRUSH CEntityDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	
	// TODO: Return a different brush if the default is not desired
	return hbr;
}

BOOL CEntityDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_ltSelect.AddString("Entity");
	m_ltSelect.AddString("Base Class");
	m_ltSelect.AddString("Derived Class");
	m_ltSelect.AddString("Global");

	GetDlgItem(IDC_BUTTON_FINISH)->EnableWindow(false);

	GetDlgItem(IDC_STATIC_NAME1)->ShowWindow(false);
	GetDlgItem(IDC_STATIC_NAME2)->ShowWindow(false);
	GetDlgItem(IDC_STATIC_NAME3)->ShowWindow(false);
	GetDlgItem(IDC_STATIC_NAME4)->ShowWindow(false);

	GetDlgItem(IDC_EDIT_PARAM1)->ShowWindow(false);
	GetDlgItem(IDC_EDIT_PARAM2)->ShowWindow(false);
	GetDlgItem(IDC_EDIT_PARAM3)->ShowWindow(false);
	GetDlgItem(IDC_EDIT_PARAM4)->ShowWindow(false);

	m_btDefault[0].ShowWindow(false);
	m_btDefault[1].ShowWindow(false);
	m_btDefault[2].ShowWindow(false);
	m_btDefault[3].ShowWindow(false);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CEntityDialog::OnSelchangeListSelect() 
{
	// TODO: Add your control notification handler code here
	switch(m_ltSelect.GetCurSel())
	{
	case 0:
		m_sDescribe = "Define entity statement.\nThis is used to define individual entities.";

		m_sName[0] = "KEY";
		m_sName[1] = "Smart name";
		m_sName[2] = "Base class";
		m_sName[3].Empty();

		m_sParam[0].Empty();
		m_sParam[1].Empty();
		m_sParam[3].Empty();

		if(sBase.IsEmpty())
			m_sParam[2] = "base_";
		else
			m_sParam[2] = sBase;
		break;
	case 1:
		m_sDescribe = "Define base entity class.\n";

		m_sName[0] = "Class name";
		m_sName[1].Empty();
		m_sName[2].Empty();
		m_sName[3].Empty();

		m_sParam[0] = "base_";
		m_sParam[1].Empty();
		m_sParam[2].Empty();
		m_sParam[3].Empty();
		break;
	case 2:
		m_sDescribe = "Define derived entity class.\n";

		m_sName[0] = "Class name";
		m_sName[1] = "Category (optional)";
		m_sName[2] = "Base class";
		m_sName[3].Empty();

		m_sParam[0] = "base_";
		m_sParam[1].Empty();
		m_sParam[3].Empty();

		if(sBase.IsEmpty())
			m_sParam[2] = "base_";
		else
			m_sParam[2] = sBase;
		
		break;
	case 3:
		m_sDescribe = "Define global parameters. Use this to setup the global parameters.";

		m_sName[0] = "Game name";
		m_sName[1] = "Author name";
		m_sName[2] = "Version";
		m_sName[3].Empty();

		m_sParam[0].Empty();
		m_sParam[1].Empty();
		m_sParam[2].Empty();
		m_sParam[3].Empty();
		break;
	}

	if(m_sName[0] == "")
	{
		GetDlgItem(IDC_STATIC_NAME1)->ShowWindow(false);
		GetDlgItem(IDC_EDIT_PARAM1)->ShowWindow(false);
		m_btDefault[0].ShowWindow(false);
	}
	else
	{
		GetDlgItem(IDC_STATIC_NAME1)->ShowWindow(true);
		GetDlgItem(IDC_EDIT_PARAM1)->ShowWindow(true);
		m_btDefault[0].ShowWindow(true);
		GetDlgItem(IDC_EDIT_PARAM1)->SetFocus();
	}

	if(m_sName[1] == "")
	{
		GetDlgItem(IDC_STATIC_NAME2)->ShowWindow(false);
		GetDlgItem(IDC_EDIT_PARAM2)->ShowWindow(false);
		m_btDefault[1].ShowWindow(false);
	}
	else
	{
		GetDlgItem(IDC_STATIC_NAME2)->ShowWindow(true);
		GetDlgItem(IDC_EDIT_PARAM2)->ShowWindow(true);
		m_btDefault[1].ShowWindow(true);
	}

	if(m_sName[2] == "")
	{
		GetDlgItem(IDC_STATIC_NAME3)->ShowWindow(false);
		GetDlgItem(IDC_EDIT_PARAM3)->ShowWindow(false);
		m_btDefault[2].ShowWindow(false);
	}
	else
	{
		GetDlgItem(IDC_STATIC_NAME3)->ShowWindow(true);
		GetDlgItem(IDC_EDIT_PARAM3)->ShowWindow(true);
		m_btDefault[2].ShowWindow(true);
	}
	
	if(m_sName[3] == "")
	{
		GetDlgItem(IDC_STATIC_NAME4)->ShowWindow(false);
		GetDlgItem(IDC_EDIT_PARAM4)->ShowWindow(false);
		m_btDefault[3].ShowWindow(false);
	}
	else
	{
		GetDlgItem(IDC_STATIC_NAME4)->ShowWindow(true);
		GetDlgItem(IDC_EDIT_PARAM4)->ShowWindow(true);
		m_btDefault[3].ShowWindow(true);
	}

	GetDlgItem(IDC_BUTTON_FINISH)->EnableWindow(true);

	UpdateData(false);
}

void CEntityDialog::OnButtonFinish() 
{
	// TODO: Add your control notification handler code here
	UpdateData(true);

	CString sBuffer[4];
	
	switch(m_ltSelect.GetCurSel())
	{
	case 0:
		m_sEntity.Format("$DefEnt [ \"%s\" : \"%s\" ] [ \"%s\" ]\n{}\n", m_sParam[0], m_sParam[1], m_sParam[2]);
		break;
	case 1:
		m_sEntity.Format("$DefEnt [ \"%s\" : @VIRTUAL ] []\n{}\n", m_sParam[0]);
		break;
	case 2:
		if(m_sParam[1].IsEmpty() || (m_sParam[1] == "@NULL"))
			m_sEntity.Format("$DefEnt [ \"%s\" : @VIRTUAL ] [ \"%s\" ]\n{}\n", m_sParam[0], m_sParam[2]);
		else
			m_sEntity.Format("$DefEnt [ \"%s\" : @VIRTUAL : \"%s\" ] [ \"%s\" ]\n{}\n", m_sParam[0], m_sParam[1], m_sParam[2]);
		break;
	case 3:
		m_sEntity.Format("$DefGlobal [ \"%s\" ]\n{\n\tdef_author\t\t\t: \"%s\"\n\tdef_version\t\t: \"%s\"\n\tdef_solid\t\t\t: \"\"\n\tdef_point\t\t\t: \"\"\n\tdef_palette\t\t: \"\"\n\tdef_gamemaptype\t\t: \"\"\n\tdef_pakdir\t\t\t: \"\"\n\tdef_targetkey\t\t: \"\"\n\tdef_namekey\t\t: \"\"\n\tdef_textype\t\t: \"\"\n}\n", m_sParam[0], m_sParam[1], m_sParam[2]);
		break;
	}
	OnOK();
}

CString CEntityDialog::GetString()
{
	return m_sEntity;
}

void CEntityDialog::SetBase(CString sName)
{
	sBase = sName;
}

void CEntityDialog::OnButtonDefualt1() 
{
	// TODO: Add your control notification handler code here
	int nTemp = m_ltSelect.GetCurSel();
	if(nTemp == LB_ERR)
		return;

	UpdateData(true);

	switch(nTemp)
	{
	case 0:		//entity
		m_sParam[0] = "";
		break;
	case 1:		//base entity
		m_sParam[0] = "base_";
		break;
	case 2:		//derived entity
		m_sParam[0] = "base_";
		break;
	case 3:		//global
		m_sParam[0] = "";
		break;
	}

	UpdateData(false);
}

void CEntityDialog::OnButtonDefualt2() 
{
	// TODO: Add your control notification handler code here
	int nTemp = m_ltSelect.GetCurSel();
	if(nTemp == LB_ERR)
		return;

	UpdateData(true);

	switch(nTemp)
	{
	case 0:		//entity
		m_sParam[1] = "";
		break;
	case 1:		//base entity
		m_sParam[1] = "";
		break;
	case 2:		//derived entity
		m_sParam[1] = "@NULL";
		break;
	case 3:		//global
		m_sParam[1] = "Unknown";
		break;
	}

	UpdateData(false);
}

void CEntityDialog::OnButtonDefualt3() 
{
	// TODO: Add your control notification handler code here
	int nTemp = m_ltSelect.GetCurSel();
	if(nTemp == LB_ERR)
		return;

	UpdateData(true);

	switch(nTemp)
	{
	case 0:		//entity
		m_sParam[2] = "base_";
		break;
	case 1:		//base entity
		m_sParam[2] = "";
		break;
	case 2:		//derived entity
		m_sParam[2] = "base_";
		break;
	case 3:		//global
		m_sParam[2] = "1.0";
		break;
	}

	UpdateData(false);
}

void CEntityDialog::OnButtonDefualt4() 
{
	// TODO: Add your control notification handler code here
	int nTemp = m_ltSelect.GetCurSel();
	if(nTemp == LB_ERR)
		return;

	UpdateData(true);

	switch(nTemp)
	{
	case 0:		//entity
		m_sParam[3] = "";
		break;
	case 1:		//base entity
		m_sParam[3] = "";
		break;
	case 2:		//derived entity
		m_sParam[3] = "";
		break;
	case 3:		//global
		m_sParam[3] = "";
		break;
	}

	UpdateData(false);
}
