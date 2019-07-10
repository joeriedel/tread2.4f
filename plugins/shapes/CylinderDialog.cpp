// CylinderDialog.cpp : implementation file
//

#include "stdafx.h"
#include "shapes.h"
#include "CylinderDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCylinderDialog dialog


CCylinderDialog::CCylinderDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CCylinderDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCylinderDialog)
	m_nAxis = 0;
	m_nDepth = 256;
	m_nNumSides = 6;
	m_nSize = 128;
	//}}AFX_DATA_INIT
}


void CCylinderDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCylinderDialog)
	DDX_Radio(pDX, IDC_CYLINDER_X, m_nAxis);
	DDX_Text(pDX, IDC_DEPTH_EDIT, m_nDepth);
	DDV_MinMaxInt(pDX, m_nDepth, 1, 2048);
	DDX_Text(pDX, IDC_NUMBER_OF_SIDES_EDIT, m_nNumSides);
	DDV_MinMaxInt(pDX, m_nNumSides, 3, 256);
	DDX_Text(pDX, IDC_SIZE_EDIT, m_nSize);
	DDV_MinMaxInt(pDX, m_nSize, 1, 2048);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCylinderDialog, CDialog)
	//{{AFX_MSG_MAP(CCylinderDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCylinderDialog message handlers

void CCylinderDialog::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData();

	CDialog::OnOK();
}
