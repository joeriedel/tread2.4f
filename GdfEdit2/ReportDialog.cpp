// ReportDialog.cpp : implementation file
//

#include "stdafx.h"
#include "GdfEdit2.h"
#include "ReportDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CReportDialog dialog


CReportDialog::CReportDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CReportDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CReportDialog)
	m_nClasses = 0;
	m_nCount = 0;
	m_nEntity = 0;
	m_sMessage = _T("");
	m_nSeconds = 0;
	m_nUnique = 0;
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CReportDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CReportDialog)
	DDX_Text(pDX, IDC_EDIT_CLASSES, m_nClasses);
	DDX_Text(pDX, IDC_EDIT_COUNT, m_nCount);
	DDX_Text(pDX, IDC_EDIT_ENTITIES, m_nEntity);
	DDX_Text(pDX, IDC_STATIC_MESSAGE, m_sMessage);
	DDX_Text(pDX, IDC_EDIT_SECONDS, m_nSeconds);
	DDX_Text(pDX, IDC_EDIT_UNIQUE, m_nUnique);
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CReportDialog, CDialog)
	//{{AFX_MSG_MAP(CReportDialog)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CReportDialog message handlers

HBRUSH CReportDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	pDC->SetTextColor(RGB(0, 50, 200));
	
	// TODO: Return a different brush if the default is not desired
	return hbr;
}

BOOL CReportDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	UpdateData(false);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CReportDialog::SetCount(unsigned int nCount)
{
	m_nCount = nCount;
}

void CReportDialog::SetClasses(unsigned int nClasses)
{
	m_nClasses = nClasses;
}

void CReportDialog::SetEntity(unsigned int nEntity)
{
	m_nEntity = nEntity;
}

void CReportDialog::SetMessage(CString sMessage)
{
	m_sMessage = sMessage;
}

void CReportDialog::SetSeconds(int nSeconds)
{
	m_nSeconds = nSeconds;
}

void CReportDialog::SetUnique(int nUnique)
{
	m_nUnique = nUnique;
}