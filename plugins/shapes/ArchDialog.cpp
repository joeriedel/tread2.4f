// ArchDialog.cpp : implementation file
//

#include "stdafx.h"
#include "shapes.h"
#include "ArchDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CArchDialog dialog


CArchDialog::CArchDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CArchDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CArchDialog)
	m_nDepth = 64;
	m_nNumSections = 6;
	m_nSize = 256;
	m_nWidth = 32;
	m_nAxis = 0;
	//}}AFX_DATA_INIT
}


void CArchDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CArchDialog)
	DDX_Text(pDX, IDC_DEPTH_EDIT, m_nDepth);
	DDV_MinMaxInt(pDX, m_nDepth, 1, 2048);
	DDX_Text(pDX, IDC_NUMBER_OF_SECTIONS_EDIT, m_nNumSections);
	DDV_MinMaxInt(pDX, m_nNumSections, 3, 1000);
	DDX_Text(pDX, IDC_SIZE_EDIT, m_nSize);
	DDV_MinMaxInt(pDX, m_nSize, 1, 2048);
	DDX_Text(pDX, IDC_WIDTH_EDIT, m_nWidth);
	DDV_MinMaxInt(pDX, m_nWidth, 1, 2048);
	DDX_Radio(pDX, IDC_ARCH_X, m_nAxis);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CArchDialog, CDialog)
	//{{AFX_MSG_MAP(CArchDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CArchDialog message handlers

void CArchDialog::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData();

	CDialog::OnOK();
}
