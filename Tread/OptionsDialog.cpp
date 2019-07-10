// OptionsDialog.cpp : implementation file
//

#include "stdafx.h"
#include "tread3d.h"
#include "OptionsDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COptionsDialog dialog


COptionsDialog::COptionsDialog(CWnd* pParent /*=NULL*/)
	: CDialog(COptionsDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(COptionsDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_nOption = -1;
}


void COptionsDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COptionsDialog)
	DDX_Control(pDX, IDC_COMBO_OPTIONS, m_cbOptions);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COptionsDialog, CDialog)
	//{{AFX_MSG_MAP(COptionsDialog)
	ON_BN_CLICKED(IDC_BUTTON_DONE, OnButtonDone)
	ON_CBN_SELCHANGE(IDC_COMBO_OPTIONS, OnSelchangeComboOptions)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COptionsDialog message handlers

BOOL COptionsDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	ENTITYFLAGOPTIONDEFINITION* pOption;
	int n;
	m_pOptionList->First();
	for(pOption = m_pOptionList->GetCurItem(); pOption != NULL; pOption = m_pOptionList->GetNextItem())
	{
		n = m_cbOptions.AddString(pOption->sString);
		m_cbOptions.SetItemData(n, pOption->nValue);

		if(m_nOption == pOption->nValue)
			m_cbOptions.SetCurSel(n);

		m_nCount++;
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void COptionsDialog::OnButtonDone() 
{
	OnOK();
}

CString COptionsDialog::GetOption()
{
	CString sBuffer;
	sBuffer.Format("%d", m_nOption);
	return sBuffer;
}

void COptionsDialog::SetOption(int nOption)
{
	m_nOption = nOption;
}

void COptionsDialog::SetOption(CString sOption)
{
	m_nOption = atoi((LPCSTR)sOption);
}

void COptionsDialog::OnSelchangeComboOptions() 
{
	int nSelect = m_cbOptions.GetCurSel();
	if(nSelect == -1)
		return;

	m_nOption = m_cbOptions.GetItemData(nSelect);
}
