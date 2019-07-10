// StartupDialog.cpp : implementation file
//

#include "stdafx.h"
#include "tread3d.h"
#include "StartupDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStartupDialog dialog


CStartupDialog::CStartupDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CStartupDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CStartupDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CStartupDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStartupDialog)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CStartupDialog, CDialog)
	//{{AFX_MSG_MAP(CStartupDialog)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStartupDialog message handlers
