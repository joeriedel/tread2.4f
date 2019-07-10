// GameDialog.cpp : implementation file
//

#include "stdafx.h"
#include "tread3d.h"
#include "TreadSys.h"
#include "GameDialog.h"
#include "GameApi.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGameDialog dialog


CGameDialog::CGameDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CGameDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGameDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

void CGameDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGameDialog)
	DDX_Control(pDX, IDC_LIST_GAMES, m_lbGames);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGameDialog, CDialog)
	//{{AFX_MSG_MAP(CGameDialog)
	ON_WM_CTLCOLOR()
	ON_LBN_SELCHANGE(IDC_LIST_GAMES, OnSelchangeListGames)
	ON_BN_CLICKED(IDC_BUTTON_SELECT, OnButtonSelect)
	ON_WM_SETCURSOR()
	ON_LBN_DBLCLK(IDC_LIST_GAMES, OnDblclkListGames)
	ON_LBN_SELCANCEL(IDC_LIST_GAMES, OnSelcancelListGames)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGameDialog message handlers

HBRUSH CGameDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr;

	switch(nCtlColor)
	{
	case CTLCOLOR_LISTBOX:
		pDC->SetTextColor(RGB(50, 50, 200));
		pDC->SetBkColor(RGB(196, 196, 196));
		hbr = (HBRUSH)GetStockObject(LTGRAY_BRUSH);
		return hbr;
	}
	
	hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	return hbr;
}

BOOL CGameDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	GetDlgItem(IDC_BUTTON_SELECT)->EnableWindow(false);
	
	CGameDefinition* pGame = NULL;
	CLinkedList<CGameDefinition>* pList = NULL;

	pList = GetGameAPI()->GetGameDefinitionList();
	if(pList == NULL)
	{
		Error("Invalid pointer to Game Definition List");
		return false;
	}

	pList->First();
	for(pGame = pList->GetCurItem(); pGame != NULL; pGame = pList->GetNextItem())
		AddGameString(pGame->GetGameName());


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CGameDialog::OnSelchangeListGames() 
{
	GetDlgItem(IDC_BUTTON_SELECT)->EnableWindow(true);

	int nTemp;
	nTemp = m_lbGames.GetCurSel();
	if(nTemp == -1)
		return;
	else
		m_lbGames.GetText(nTemp, m_sGame);
}

void CGameDialog::AddGameString(CString sGame)
{
	m_lbGames.AddString(sGame);
}

CString CGameDialog::GetGameSelection()
{
	return m_sGame;
}

void CGameDialog::OnButtonSelect() 
{
	OnOK();
}

BOOL CGameDialog::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	HCURSOR cursor;
	cursor = AfxGetApp()->LoadCursor(IDC_BLUE_ARROW);
	if(cursor == NULL)
		return CDialog::OnSetCursor(pWnd, nHitTest, message);

	SetCursor(cursor);
	
	return true;
}

void CGameDialog::OnDblclkListGames() 
{
	int nTemp;
	nTemp = m_lbGames.GetCurSel();
	if(nTemp == -1)
		return;
	
	m_lbGames.GetText(nTemp, m_sGame);
	OnOK();
}

void CGameDialog::OnSelcancelListGames() 
{
	GetDlgItem(IDC_BUTTON_SELECT)->EnableWindow(false);
}
