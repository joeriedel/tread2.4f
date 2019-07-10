// GameSetupDialog.cpp : implementation file
//

#include "stdafx.h"
#include "tread3d.h"
#include "GameSetupDialog.h"
//#include "TreadSys.h"
#include "GameApi.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGameSetupDialog dialog

#define TEXTURE_WIDTH 400
#define PATH_WIDTH 400
#define SECTION_WIDTH 150

CGameSetupDialog::CGameSetupDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CGameSetupDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGameSetupDialog)
	//}}AFX_DATA_INIT

	m_bSlash = false;
	m_bFile = false;
	m_pPakList = NULL;
	m_nMode = 0;
}


void CGameSetupDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGameSetupDialog)
	DDX_Control(pDX, IDC_LIST_DIR, m_lbFiles);
	DDX_Control(pDX, IDC_BUTTON_REMOVE, m_btRemove);
	DDX_Control(pDX, IDC_BUTTON_ADD, m_btAdd);
	DDX_Control(pDX, IDC_LIST_PATHS, m_lcPaths);
	DDX_Control(pDX, IDC_LIST_GAMES, m_lbGames);
	DDX_Control(pDX, IDC_TREE_DIRECTORY, m_tcPath);
	DDX_Control(pDX, IDC_TAB_MODE, m_tabMode);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGameSetupDialog, CDialog)
	//{{AFX_MSG_MAP(CGameSetupDialog)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON_ADD, OnButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_REMOVE, OnButtonRemove)
	ON_LBN_SELCHANGE(IDC_LIST_GAMES, OnSelchangeListGames)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_MODE, OnSelchangeTabMode)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_DIRECTORY, OnSelchangedTreeDirectory)
	ON_LBN_SELCHANGE(IDC_LIST_DIR, OnSelchangeListDir)
	ON_NOTIFY(NM_CLICK, IDC_LIST_PATHS, OnClickListPaths)
	ON_BN_CLICKED(IDC_BUTTON_DONE, OnButtonDone)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGameSetupDialog message handlers

BOOL CGameSetupDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if(!m_imgList1.Create(IDB_BITMAP_PAK, 16, 0, RGB(255, 0, 255)))
		return false;
	m_lcPaths.SetImageList(&m_imgList1, LVSIL_SMALL);

	m_lcPaths.InsertColumn(0, "", LVCFMT_LEFT, TEXTURE_WIDTH);
	m_lcPaths.InsertColumn(1, "", LVCFMT_LEFT, 0);

	if(!m_imgList2.Create(IDB_BITMAP_DRIVES, 16, 0, RGB(255, 0, 255)))
		return false;
	m_tcPath.SetImageList(&m_imgList2, TVSIL_NORMAL);

	m_tabMode.InsertItem(0, "Texture Files");
	m_tabMode.InsertItem(1, "Build Options");

	m_tcPath.InitTree();

	CGameDefinition* pGame = NULL;
	CGameDefinition* pCurrent = NULL;
	CLinkedList<CGameDefinition>* pGameList = NULL;
	CString sBuffer;
	int n;

	if(!GetMapManager()->IsMap())
		GetGameAPI()->PurgeAndReset();

	pGameList = GetGameAPI()->GetGameDefinitionList();
	if(pGameList == NULL)
	{
		Error("Invalid pointer to Game Definition List");
		return false;
	}

	pGameList->First();
	for(pGame = pGameList->GetCurItem(); pGame != NULL; pGame = pGameList->GetNextItem())
	{
		pCurrent = GetGameAPI()->GetCurrentGame();
		if(pCurrent != NULL)
		{
			if((pGame->GetGameName() == pCurrent->GetGameName()) && (pCurrent != NULL))
			{
				sBuffer.Format("%s : Disabled", pGame->GetGameName());
				n = m_lbGames.AddString(sBuffer);
				m_lbGames.SetItemData(n, NULL);
			}
			else
			{
				n = m_lbGames.AddString(pGame->GetGameName());
				m_lbGames.SetItemData(n, (ULONG)pGame);
			}
		}
		else
		{
			n = m_lbGames.AddString(pGame->GetGameName());
			m_lbGames.SetItemData(n, (ULONG)pGame);
		}
	}

	m_btAdd.EnableWindow(false);
	m_btRemove.EnableWindow(false);
	m_tcPath.EnableWindow(false);
	m_lbFiles.EnableWindow(false);
	m_lcPaths.EnableWindow(false);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CGameSetupDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	return hbr;
}

void CGameSetupDialog::OnButtonAdd() 
{
	CString sBuffer;

	if(m_nMode == 0)
	{
		int nImage, n;

		if(m_bFile)
		{
			if(m_bSlash)
				sBuffer.Format("%s%s", m_sPath, m_sFile);
			else
				sBuffer.Format("%s\\%s", m_sPath, m_sFile);

			nImage = 1;
		}
		else
		{
			sBuffer = m_sPath;
			if(m_bSlash)
			{
				sBuffer.SetAt(m_sPath.GetLength() - 1, ' ');
				sBuffer.TrimRight();
			}

			nImage = 0;
		}

		LVFINDINFO findInfo;
		findInfo.flags = LVFI_STRING;
		findInfo.psz = (LPCSTR)sBuffer;
		n = m_lcPaths.FindItem(&findInfo, -1);
		if(n != -1)
		{
			Error("Duplicate file or directory.");
			return;
		}

		GAMEPAKFILE* pPak;
		pPak = new GAMEPAKFILE;
		if(pPak == NULL)
			return;

		pPak->bDir = !m_bFile;
		pPak->nHandle = 0;
		pPak->sFilename = sBuffer;
		char szString[256];
		GetFileTitle(sBuffer, szString, 256);
		pPak->sTitle = szString;

		pPak->prev = pPak->next = NULL;

		n = m_lcPaths.InsertItem(0, sBuffer, nImage);
		m_lcPaths.SetItemData(n, (ULONG)pPak);
		m_pPakList->AddItem(pPak);
	}
	else
	{
		if(m_nSubMode == PATH_TYPE_DIR)
		{
			sBuffer = m_sPath;
		}
		else
		{
			if(m_bSlash)
				sBuffer.Format("%s%s", m_sPath, m_sFile);
			else
				sBuffer.Format("%s\\%s", m_sPath, m_sFile);
		}

		POSITION pos;
		int nItem;

		pos = m_lcPaths.GetFirstSelectedItemPosition();
		if(pos == NULL)
			return;

		while(pos != NULL)
		{
			nItem = m_lcPaths.GetNextSelectedItem(pos);
			DEFMODULE* pModule = (DEFMODULE*)m_lcPaths.GetItemData(nItem);
			pModule->sPath = sBuffer;
			m_lcPaths.SetItemText(nItem, 1, sBuffer);
		}
	}
}

void CGameSetupDialog::OnButtonRemove() 
{
	if(m_nMode == 0)
	{
		POSITION pos;
		int nItem;

		pos = m_lcPaths.GetFirstSelectedItemPosition();
		if(pos == NULL)
			return;

		while(pos != NULL)
		{
			nItem = m_lcPaths.GetNextSelectedItem(pos);
			GAMEPAKFILE* pPak = (GAMEPAKFILE*)m_lcPaths.GetItemData(nItem);
			if(pPak == NULL)
				Error("Item is NULL in CGameSetupDialog::OnButtonRemove");
			m_pPakList->DeleteItem(pPak);
			m_lcPaths.DeleteItem(nItem);
		}
	}
	else
	{
		POSITION pos;
		int nItem;

		pos = m_lcPaths.GetFirstSelectedItemPosition();
		if(pos == NULL)
			return;

		while(pos != NULL)
		{
			nItem = m_lcPaths.GetNextSelectedItem(pos);
			DEFMODULE* pModule = (DEFMODULE*)m_lcPaths.GetItemData(nItem);
			pModule->sPath = "";
			m_lcPaths.SetItemText(nItem, 1, "");
		}
	}

	m_btRemove.EnableWindow(false);
}

void CGameSetupDialog::OnSelchangeListGames() 
{
	CString sBuffer;
	int nTemp = m_lbGames.GetCurSel();
	if(nTemp == LB_ERR)
		return;

	CGameDefinition* pDef = (CGameDefinition*)m_lbGames.GetItemData(nTemp);
	if(pDef == NULL)
	{
		m_tcPath.EnableWindow(false);
		m_lbFiles.EnableWindow(false);
		m_lcPaths.EnableWindow(false);
		return;
	}

	if(m_nMode == 0)
	{
		m_tabMode.EnableWindow(true);
		m_tcPath.EnableWindow(true);
		m_lbFiles.EnableWindow(true);
		m_lcPaths.EnableWindow(true);

		m_btAdd.EnableWindow(false);
		m_btRemove.EnableWindow(false);

		m_btAdd.SetWindowText("");

		m_lcPaths.DeleteAllItems();

		m_pPakList = pDef->GetPakFileList();
		if(m_pPakList == NULL)
			return;

		GAMEPAKFILE* pPak;
		int nImage;
		int n;

		m_pPakList->First();
		for(pPak = m_pPakList->GetCurItem(); pPak != NULL; pPak = m_pPakList->GetNextItem())
		{
			sBuffer = pPak->sFilename;
			if(pPak->bDir)
				nImage = 0;
			else
				nImage = 1;

			n = m_lcPaths.InsertItem(0, sBuffer, nImage);
			m_lcPaths.SetItemData(n, (ULONG)pPak);
		}
	}
	else
	{
		m_tabMode.EnableWindow(true);
		m_lcPaths.EnableWindow(true);

		m_btAdd.EnableWindow(false);
		m_btRemove.EnableWindow(false);
		m_tcPath.EnableWindow(false);
		m_lbFiles.EnableWindow(false);
		
		m_lcPaths.DeleteAllItems();

		CLinkedList<DEFMODULE>* pList = pDef->GetDefBuild()->pModuleList;
		DEFMODULE* pModule;
		int n = 0, k;

		pList->First();
		for(pModule = pList->GetCurItem(); pModule != NULL; pModule = pList->GetNextItem())
		{
			if(pModule->sSetupTitle.IsEmpty())
				continue;

			k = m_lcPaths.InsertItem(n, pModule->sSetupTitle, 2);
			m_lcPaths.SetItemData(k, (ULONG)pModule);
			m_lcPaths.SetItemText(k, 1, pModule->sPath);
			n++;
		}
	}
}

void CGameSetupDialog::OnSelchangeTabMode(NMHDR* pNMHDR, LRESULT* pResult) 
{
	m_nMode = m_tabMode.GetCurSel();

	if(m_nMode == 0)
	{
		m_lcPaths.SetColumnWidth(0, TEXTURE_WIDTH);
		m_lcPaths.SetColumnWidth(1, 0);
		m_btAdd.SetWindowText("");
		m_btRemove.SetWindowText("Remove Path");
	}
	else
	{
		m_btAdd.SetWindowText("Set Path");
		m_btRemove.SetWindowText("Clear Path");

		m_lcPaths.SetColumnWidth(0, SECTION_WIDTH);
		m_lcPaths.SetColumnWidth(1, PATH_WIDTH);
	}
	

	OnSelchangeListGames();
	
	*pResult = 0;
}

void CGameSetupDialog::OnSelchangedTreeDirectory(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	CString sBuffer;

	m_sPath = m_tcPath.GetSelectedPath(pNMHDR, pResult);
	if(m_sPath.IsEmpty())
		return;

	sBuffer = m_sPath;
	if(sBuffer.GetAt(sBuffer.GetLength() - 1) == '\\')
	{
		sBuffer += "*.*";
		m_bSlash = true;
	}
	else
	{
		sBuffer += "\\*.*";
		m_bSlash = false;
	}

	m_lbFiles.ResetContent();

	HANDLE hFind;
	WIN32_FIND_DATA fd;

	hFind = ::FindFirstFile((LPCSTR)sBuffer, &fd);
	if(hFind != INVALID_HANDLE_VALUE)
	{
		fd.dwFileAttributes &= FILE_ATTRIBUTE_DIRECTORY;
		if(fd.dwFileAttributes != FILE_ATTRIBUTE_DIRECTORY)
			m_lbFiles.AddString(fd.cFileName);
					
		while(::FindNextFile(hFind, &fd))
		{
			//Blank out all attributes accept directory
			//if it exists then handle the sub directory
			fd.dwFileAttributes &= FILE_ATTRIBUTE_DIRECTORY;
			if(fd.dwFileAttributes != FILE_ATTRIBUTE_DIRECTORY)
				m_lbFiles.AddString(fd.cFileName);
		}

		::CloseHandle(hFind);
	}
	
	m_bFile = false;

	if(m_nMode == 0)
	{
		m_btAdd.SetWindowText("&Add Directory");
		m_btAdd.EnableWindow(true);
	}
	else
	{
		if(m_nSubMode == PATH_TYPE_DIR)
			m_btAdd.EnableWindow(true);
		else
		{
			m_lbFiles.ShowWindow(true);
			m_lbFiles.EnableWindow(true);
		}
	}

	*pResult = 0;
}

void CGameSetupDialog::OnSelchangeListDir() 
{
	int nTemp = m_lbFiles.GetCurSel();
	if(nTemp == LB_ERR)
		return;
	
	m_bFile = true;

	m_lbFiles.GetText(nTemp, m_sFile);
	m_btAdd.EnableWindow(true);

	if(m_nMode == 0)
	{
		m_btAdd.SetWindowText("&Add File");
	}
}

void CGameSetupDialog::OnClickListPaths(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NMLISTVIEW* pNMItem = (NMLISTVIEW*)pNMHDR;

	//get the hit test information to find out where the user clicked
	LVHITTESTINFO hitTest;
	int nItem;
	hitTest.pt = pNMItem->ptAction;
	m_lcPaths.SubItemHitTest(&hitTest);
	nItem = hitTest.iItem;

	//if the item is invalid then return
	if(nItem == -1)
		return;

	LVITEM ItemState;
	ItemState.mask = LVIF_STATE;
	ItemState.iItem = nItem;
	ItemState.iSubItem = 0;
	ItemState.state = -1;
	ItemState.stateMask = LVIS_SELECTED;

	m_lcPaths.SetItemState(nItem, &ItemState);

	//enable the remove button
	m_btRemove.EnableWindow(true);

	if(m_nMode == 0)	//if the mode is pak list
	{//DO NOTHING
	}
	else				//if the mode is game paths
	{
		//enable tree control with directory structure
		m_tcPath.EnableWindow(true);

		//get a pointer to the defmodule assigned to the itme
		DEFMODULE* pModule = (DEFMODULE*)m_lcPaths.GetItemData(nItem);
		//save the path type and enable / disable file list accordingly
		m_nSubMode = pModule->nPathType;

		if(m_nSubMode == PATH_TYPE_DIR)
			m_lbFiles.ShowWindow(false);
		else
		{
			m_lbFiles.ShowWindow(true);
			m_lbFiles.EnableWindow(true);
		}
	}

	*pResult = 0;
}

void CGameSetupDialog::OnButtonDone() 
{
	GetGameAPI()->SaveRegSettings();
	OnOK();
}
