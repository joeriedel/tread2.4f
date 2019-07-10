// KeyDialog.cpp : implementation file
//

#include "stdafx.h"
#include "GdfEdit2.h"
#include "KeyDialog.h"
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CKeyDialog dialog


CKeyDialog::CKeyDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CKeyDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CKeyDialog)
	m_sDescribe = _T("");
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CKeyDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CKeyDialog)
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


BEGIN_MESSAGE_MAP(CKeyDialog, CDialog)
	//{{AFX_MSG_MAP(CKeyDialog)
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
// CKeyDialog message handlers

HBRUSH CKeyDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	
	// TODO: Return a different brush if the default is not desired
	return hbr;
}

BOOL CKeyDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_ltSelect.AddString("Attribute");
	m_ltSelect.AddString("Key");
	m_ltSelect.AddString("Flags");
	m_ltSelect.AddString("Options");

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

void CKeyDialog::OnSelchangeListSelect() 
{
	// TODO: Add your control notification handler code here
	switch(m_ltSelect.GetCurSel())
	{
	case 0:
		m_sDescribe = "An attribute specifies a pre defined entity property i.e.(size).";

		m_sName[0] = "Attribute name";
		m_sName[1] = "Initial value";
		m_sName[2].Empty();
		m_sName[3].Empty();

		m_sParam[0].Empty();
		m_sParam[1].Empty();
		m_sParam[2].Empty();
		m_sParam[3].Empty();
		break;
	case 1:
		m_sDescribe = "A key specifies the type and the name of a property for an entity.";

		m_sName[0] = "Key type";
		m_sName[1] = "Key name";
		m_sName[2] = "Smart name";
		m_sName[3] = "Initial value";

		m_sParam[0].Empty();
		m_sParam[1].Empty();
		m_sParam[2].Empty();
		m_sParam[3].Empty();
		break;
	case 2:
		m_sDescribe = "A flag set defines a group of additive properties.";

		m_sName[0] = "Key name";
		m_sName[1] = "Smart name";
		m_sName[2] = "Flag count";
		m_sName[3].Empty();

		m_sParam[0].Empty();
		m_sParam[1].Empty();
		m_sParam[2].Empty();
		m_sParam[3].Empty();
		break;
	case 3:
		m_sDescribe = "An option set defines a group of options for an entity property.";

		m_sName[0] = "Key name";
		m_sName[1] = "Smart name";
		m_sName[2] = "Flag count";
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

void CKeyDialog::OnButtonFinish() 
{
	// TODO: Add your control notification handler code here
	UpdateData(true);

	char pBuffer[10];
	CString sBuffer;
	int nCount;
	int nFlag;
	int n;
	
	switch(m_ltSelect.GetCurSel())
	{
	case 0:
		m_sKey.Format("\tdef_att : \"%s\" : \"%s\"", m_sParam[0], m_sParam[1]);
		break;
	case 1:
		m_sKey.Format("\tdef_key : %s : \"%s\" : \"%s\" : \"%s\"", m_sParam[0], m_sParam[1], m_sParam[2], m_sParam[3]);
		break;
	case 2:
		nCount = atoi((LPCSTR)m_sParam[2]);

		if(nCount < 0)
			nCount = 0;
		else if(nCount > 32)
			nCount = 32;

		for(n = 0; n < nCount; n++)
		{
			nFlag = (int)pow(2, n);
			itoa(nFlag, pBuffer, 10);
			sBuffer += "\t\tdef_flg : \"\" : ";
			sBuffer += pBuffer;
			sBuffer += " : false\n";
		}
			
		m_sKey.Format("\t$DefFlags [ \"%s\" : \"%s\" ]\n\t{\n%s\t}\n", m_sParam[0], m_sParam[1], sBuffer);
		break;
	case 3:
		nCount = atoi((LPCSTR)m_sParam[2]);

		if(nCount < 0)
			nCount = 0;
		else if(nCount > 16)
			nCount = 16;

		for(n = 0; n < nCount; n++)
		{
			itoa(n, pBuffer, 10);
			sBuffer += "\t\tdef_flg : \"\" : ";
			sBuffer += pBuffer;
			sBuffer += " : false\n";
		}
				
		m_sKey.Format("\t$DefOptions [ \"%s\" : \"%s\" ]\n\t{\n%s\t}\n", m_sParam[0], m_sParam[1], sBuffer);
		break;
	}
	OnOK();
}

CString CKeyDialog::GetString()
{
	return m_sKey;
}

void CKeyDialog::OnButtonDefualt1() 
{
	// TODO: Add your control notification handler code here
	int nTemp = m_ltSelect.GetCurSel();
	if(nTemp == LB_ERR)
		return;

	UpdateData(true);

	switch(nTemp)
	{
	case 0:		//entity
		m_sParam[0] = "size";
		break;
	case 1:		//base entity
		m_sParam[0] = "string";
		break;
	case 2:		//derived entity
		m_sParam[0] = "";
		break;
	case 3:		//global
		m_sParam[0] = "";
		break;
	}

	UpdateData(false);
}

void CKeyDialog::OnButtonDefualt2() 
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
		m_sParam[1] = "";
		break;
	case 3:		//global
		m_sParam[1] = "";
		break;
	}

	UpdateData(false);
}

void CKeyDialog::OnButtonDefualt3() 
{
	// TODO: Add your control notification handler code here
	int nTemp = m_ltSelect.GetCurSel();
	if(nTemp == LB_ERR)
		return;

	UpdateData(true);

	switch(nTemp)
	{
	case 0:		//entity
		m_sParam[2] = "";
		break;
	case 1:		//base entity
		m_sParam[2] = "";
		break;
	case 2:		//derived entity
		m_sParam[2] = "1";
		break;
	case 3:		//global
		m_sParam[2] = "1";
		break;
	}

	UpdateData(false);
}

void CKeyDialog::OnButtonDefualt4() 
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
		m_sParam[3] = "0";
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
