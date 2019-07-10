// ObjectDialog.cpp : implementation file
//

#include "stdafx.h"
#include "tread3d.h"
#include "ObjectDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CObjectDialog dialog


CObjectDialog::CObjectDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CObjectDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CObjectDialog)
	m_sLine1 = _T("");
	m_sLine2 = _T("");
	//}}AFX_DATA_INIT
}


void CObjectDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CObjectDialog)
	DDX_Control(pDX, IDC_COMBO_ITEM2, m_cbItem2);
	DDX_Control(pDX, IDC_COMBO_ITEM1, m_cbItem1);
	DDX_Text(pDX, IDC_STATIC_LINE1, m_sLine1);
	DDX_Text(pDX, IDC_STATIC_LINE2, m_sLine2);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CObjectDialog, CDialog)
	//{{AFX_MSG_MAP(CObjectDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CObjectDialog message handlers

BOOL CObjectDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CObjectDialog::SetLine1(CString sLine)
{
	m_sLine1 = sLine;
}

void CObjectDialog::SetLine2(CString sLine)
{
	m_sLine2 = sLine;
}

void CObjectDialog::AddItem1(CString sItem)
{
	m_cbItem1.AddString(sItem);
}

void CObjectDialog::AddItem2(CString sItem)
{
	m_cbItem2.AddString(sItem);
}

CString CObjectDialog::GetItem1()
{
	CString sBuffer;
	return sBuffer;
}

CString CObjectDialog::GetItem2()
{
	CString sBuffer;
	return sBuffer;
}
