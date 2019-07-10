// ScriptDialog.cpp : implementation file
//

#include "stdafx.h"
#include "tread3d.h"
#include "ScriptDialog.h"
#include "TreadSys.h"
#include "GameAPI.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CScriptDialog dialog


CScriptDialog::CScriptDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CScriptDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CScriptDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CScriptDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CScriptDialog)
	DDX_Control(pDX, IDC_LIST_SCRIPTS, m_lcScripts);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CScriptDialog, CDialog)
	//{{AFX_MSG_MAP(CScriptDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScriptDialog message handlers

BOOL CScriptDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_lcScripts.InsertColumn(0, "Game", LVCFMT_LEFT, 100);
	m_lcScripts.InsertColumn(1, "Script Author", LVCFMT_LEFT, 100);
	m_lcScripts.InsertColumn(2, "Version", LVCFMT_LEFT, 100);

	CGameDefinition* pGame = NULL;
	CLinkedList<CGameDefinition>* pGameList = NULL;
	CString sBuffer;
	int n = 0;
	int nItem;

	pGameList = GetGameAPI()->GetGameDefinitionList();
	if(pGameList == NULL)
	{
		Error("Invalid pointer to Game Definition List");
		return false;
	}

	pGameList->First();
	for(pGame = pGameList->GetCurItem(); pGame != NULL; pGame = pGameList->GetNextItem())
	{
		nItem = m_lcScripts.InsertItem(n, pGame->GetGameName());
		m_lcScripts.SetItemText(nItem, 1, pGame->GetAuthorString());
		m_lcScripts.SetItemText(nItem, 2, pGame->GetVersionString());
		n++;
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
