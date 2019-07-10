// ConsoleDialog.cpp : implementation file
//

#include "stdafx.h"
#include "Tread3D.h"
#include "DialogControl.h"
#include "treadsys.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CConsoleDialog dialog


CConsoleDialog::CConsoleDialog(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_DIALOG_CONSOLE, pParent)
{
	//{{AFX_DATA_INIT(CConsoleDialog)
	m_bOntop = FALSE;
	//}}AFX_DATA_INIT
	bInit = false;
}


void CConsoleDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConsoleDialog)
	DDX_Control(pDX, IDC_BUTTON_SAVE, m_btSave);
	DDX_Control(pDX, IDC_BUTTON_CLEAR, m_btClear);
	DDX_Control(pDX, IDC_EDIT_CONSOLE, m_ctConsole);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CConsoleDialog, CDialog)
	//{{AFX_MSG_MAP(CConsoleDialog)
	ON_WM_CTLCOLOR()
	ON_WM_CLOSE()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, OnButtonClear)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, OnButtonSave)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConsoleDialog message handlers

HBRUSH CConsoleDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	switch(nCtlColor)
	{
	case CTLCOLOR_STATIC:
		break;
	case CTLCOLOR_LISTBOX:
		break;
	case CTLCOLOR_EDIT:
		pDC->SetTextColor(RGB(255, 255, 255));
		pDC->SetBkColor(RGB(0, 0, 0));
		HBRUSH hbr = (HBRUSH)GetStockObject(BLACK_BRUSH);
		return hbr;
	}
	
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	return hbr;
}

BOOL CConsoleDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	AppendText("Debug Window Initialized\n");

	CWinApp* pApp = AfxGetApp();
	HICON icButton;

	icButton = pApp->LoadIcon(IDI_ICON_CLEAR);
	m_btClear.SetIcon(icButton);

	CRect rcButton;
	m_btClear.GetClientRect(rcButton);

	m_ptAnchor.x = 0;
	m_ptAnchor.y = rcButton.bottom + 3;

	bInit = true;
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CConsoleDialog::OnClose() 
{
	ShowWindow(SW_HIDE);
	//CDialog::OnClose();
}

void CConsoleDialog::AppendText(CString sInput)
{
	char s1[3], s2[3];
	s1[0] = 10;
	s1[1] = 0;
	s2[0] = 13;
	s2[1] = 10;
	s2[2] = 0;

	sInput.Replace((LPCTSTR)s1, (LPCTSTR)s2);

	m_ctConsole.SetSel( -1, -1 );		// move insertion point to end of text
	m_ctConsole.ReplaceSel(sInput, false);
	m_ctConsole.SetSel( -1, -1 ); // again, move insertion point to end of text
}

void CConsoleDialog::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);

	CRect rcEdit(m_ptAnchor.x, m_ptAnchor.y, cx, cy);
	if(bInit)
		m_ctConsole.MoveWindow(rcEdit);
}

void CConsoleDialog::OnButtonClear() 
{
	m_ctConsole.SetWindowText("");
	m_ctConsole.SetFocus();
}

void CConsoleDialog::OnButtonSave() 
{
	CString sBuffer;
	m_ctConsole.GetWindowText(sBuffer);

	CFile hFile;
	if(hFile.Open(GetRegString(S_TREAD_PATH) + "\\debug.txt", CFile::modeWrite | CFile::modeCreate))
	{
		hFile.Write((LPCSTR)sBuffer, sBuffer.GetLength());
		hFile.Close();
		Debug("Wrote: debug.txt\n");
	}
	else
		Debug("Save debug information: Failed.\n");
}

int CConsoleDialog::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	RegisterDialog(this, DLG_DEBUG);
		
	return 0;
}
