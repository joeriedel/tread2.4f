// ConeDialog.cpp : implementation file
//

#include "stdafx.h"
#include "shapes.h"
#include "ConeDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CConeDialog dialog


CConeDialog::CConeDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CConeDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CConeDialog)
	m_nSize = 128;
	m_nAxis = 0;
	m_nLength = 256;
	m_nNumSides = 6;
	//}}AFX_DATA_INIT
}


void CConeDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConeDialog)
	DDX_Text(pDX, IDC_CONE_SIZE_EDIT, m_nSize);
	DDV_MinMaxInt(pDX, m_nSize, 1, 2048);
	DDX_Radio(pDX, IDC_CONE_X, m_nAxis);
	DDX_Text(pDX, IDC_LENGTH_EDIT, m_nLength);
	DDV_MinMaxInt(pDX, m_nLength, 1, 2048);
	DDX_Text(pDX, IDC_NUMBER_OF_SIDES_EDIT, m_nNumSides);
	DDV_MinMaxInt(pDX, m_nNumSides, 3, 256);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CConeDialog, CDialog)
	//{{AFX_MSG_MAP(CConeDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConeDialog message handlers

void CConeDialog::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData();

	CDialog::OnOK();
}
