// InfoDialog.cpp : implementation file
//

#include "stdafx.h"
//#include "GdfEdit.h"
#include "InfoDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInfoDialog dialog


CInfoDialog::CInfoDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CInfoDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CInfoDialog)
	m_nClasses = 0;
	m_nCount = 0;
	m_nEntity = 0;
	m_sMessage = _T("");
	m_nSeconds = 0;
	m_nUnique = 0;
	//}}AFX_DATA_INIT
}


void CInfoDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInfoDialog)
	DDX_Text(pDX, IDC_EDIT_CLASSES, m_nClasses);
	DDX_Text(pDX, IDC_EDIT_COUNT, m_nCount);
	DDX_Text(pDX, IDC_EDIT_ENTITIES, m_nEntity);
	DDX_Text(pDX, IDC_STATIC_MESSAGE, m_sMessage);
	DDX_Text(pDX, IDC_EDIT_SECONDS, m_nSeconds);
	DDX_Text(pDX, IDC_EDIT_UNIQUE, m_nUnique);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CInfoDialog, CDialog)
	//{{AFX_MSG_MAP(CInfoDialog)
	ON_WM_CTLCOLOR()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInfoDialog message handlers

HBRUSH CInfoDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	
	// TODO: Return a different brush if the default is not desired
	return hbr;
}

void CInfoDialog::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	
}

BOOL CInfoDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	UpdateData(false);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CInfoDialog::SetCount(unsigned int nCount)
{
	m_nCount = nCount;
}

void CInfoDialog::SetClasses(unsigned int nClasses)
{
	m_nClasses = nClasses;
}

void CInfoDialog::SetEntity(unsigned int nEntity)
{
	m_nEntity = nEntity;
}

void CInfoDialog::SetMessage(CString sMessage)
{
	m_sMessage = sMessage;
}

void CInfoDialog::SetSeconds(int nSeconds)
{
	m_nSeconds = nSeconds;
}

void CInfoDialog::SetUnique(int nUnique)
{
	m_nUnique = nUnique;
}