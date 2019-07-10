// TreeBar.cpp : implementation file
//

#include "stdafx.h"
#include "GdfEdit2.h"
#include "TreeBar.h"

#include "ConsoleView.h"
#include "ChildFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTreeBar dialog


CTreeBar::CTreeBar(CWnd* pParent /*=NULL*/)
{
	//{{AFX_DATA_INIT(CTreeBar)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	Head.pSub = NULL;
	Head.pNext = NULL;
	m_bWarning = true;
	m_bError = true;
}

void CTreeBar::DoDataExchange(CDataExchange* pDX)
{
	CDialogBar::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTreeBar)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTreeBar, CDialogBar)
	//{{AFX_MSG_MAP(CTreeBar)
	ON_WM_CREATE()
	ON_WM_CTLCOLOR()
	ON_WM_DESTROY()
	ON_NOTIFY(NM_RCLICK, IDC_TREE_SCRIPT, OnRclickTreeScript)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTreeBar message handlers

int CTreeBar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialogBar::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	
	return 0;
}

HBRUSH CTreeBar::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialogBar::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	
	// TODO: Return a different brush if the default is not desired
	return hbr;
}

void CTreeBar::OnDestroy() 
{
	CDialogBar::OnDestroy();
	
	// TODO: Add your message handler code here
	DestroyNode(Head.pSub);

	if(m_ctTree != NULL)
	{
		delete m_ctTree;
		m_ctTree = NULL;
	}
}

bool CTreeBar::Initialize() 
{
	m_ctTree = new CTreeCtrl();
	m_ctTree->SubclassDlgItem(IDC_TREE_SCRIPT, this);
	m_imgList.Create(IDB_BITMAP_CLASS, 16, 0, RGB(255, 0, 255));
	m_ctTree->SetImageList(&m_imgList, TVSIL_NORMAL);
	return true;
}

void CTreeBar::ResetContent()
{
	m_ctTree->DeleteAllItems();
	DestroyTree();
	m_nClasses = 0;
	m_nEntities = 0;
}

void CTreeBar::DestroyTree()
{
	if(Head.pSub != NULL)
	{
		DestroyNode(Head.pSub);
		Head.pSub = NULL;
	}
}

bool CTreeBar::AddEntity(CString sKey, CString sSmart, CString sBase, CString sGame, int nLine)
{
	entry_t* pNode = NULL;	//new node
	entry_t* pTemp = NULL;	//temporary pointer
	CString sBuffer;

	//if all of these are empty then something is wrong
	if(sKey.IsEmpty() && sBase.IsEmpty() && sSmart.IsEmpty())
	{
		return false;
	}

	if(sKey.GetLength() > 32)
	{
		sBuffer.Format("String exceeds max length by (%d) characters : '%s'", sKey.GetLength() - 32, sKey);
		Warning(sBuffer, nLine);
	}
	if(sSmart.GetLength() > 32)
	{
		sBuffer.Format("String exceeds max length by (%d) characters : '%s'", sSmart.GetLength() - 32, sSmart);
		Warning(sBuffer, nLine);
	}
	if(sBase.GetLength() > 32)
	{
		sBuffer.Format("String exceeds max length by (%d) characters : '%s'", sBase.GetLength() - 32, sBase);
		Warning(sBuffer, nLine);
	}

	if(Head.pSub == NULL)	//no nodes in the tree
	{
		pNode = new entry_t;
		if(pNode == NULL)
		{
			AfxMessageBox("Memory allocation failure", MB_OK, 0);
			return false;
		}

		if(sBase.IsEmpty())	//base class
		{
			pNode->sKey = sKey;
			pNode->nLine = nLine;
			pNode->pNext = NULL;
			pNode->pSub = NULL;
			Head.pSub = pNode;
			m_nClasses++;
		}
		else //ther cannot have been a base class if the tree is empty
		{
			sBuffer.Format("Previously undefined base class : '%s'", sBase);
			Warning(sBuffer, nLine);
			delete pNode;
			return false;
		}
	}
	else	//the list contains some entries
	{
		if(sBase.IsEmpty())	//base class
		{
			//find the end of the current list
			for(pTemp = Head.pSub; pTemp->pNext != NULL; pTemp = pTemp->pNext)
			{
				if(pTemp->sKey == sKey)
				{
					sBuffer.Format("Duplicate base class : '%s'", sBase);
					Warning(sBuffer, nLine);
					return false;
				}
			}

			pNode = new entry_t;
			if(pNode == NULL)
			{
				AfxMessageBox("Memory allocation failure", MB_OK, 0);
				return false;
			}

			pNode->sKey = sKey;
			pNode->nLine = nLine;
			pNode->pNext = NULL;
			pNode->pSub = NULL;
			
			pTemp->pNext = pNode;	//attach at end
			m_nClasses++;
		}
		else if(sSmart.IsEmpty())	//derived base
		{
			//find the base class node if it exists
			MatchString(Head.pSub, pTemp, sBase);

			if(pTemp == NULL)
			{
				sBuffer.Format("Previously undefined base class : '%s'", sBase);
				Warning(sBuffer, nLine);
				return false;
			}
			
			pNode = new entry_t;
			if(pNode == NULL)
			{
				AfxMessageBox("Memory allocation failure", MB_OK, 0);
				return false;
			}

			pNode->sKey = sKey;
			pNode->nLine = nLine;
			pNode->pNext = NULL;
			pNode->pSub = NULL;
			m_nClasses++;

			if(pTemp->pSub == NULL)	//first entry
				pTemp->pSub = pNode;
			else	//other entries exist so find the end and attach
			{
				//find the end of the current list
				for(pTemp = pTemp->pSub; pTemp->pNext != NULL; pTemp = pTemp->pNext)
				{
					if(pTemp->sKey == sKey)
					{
						sBuffer.Format("Duplicate base class : '%s'", sBase);
						Warning(sBuffer, nLine);
						return false;
					}
				}
				
				//attach the node to the end
				pTemp->pNext = pNode;
			}
		}
		else	//entity
		{
			//find the base class node if it exists
			MatchString(Head.pSub, pTemp, sBase);

			if(pTemp == NULL)
			{
				sBuffer.Format("Previously undefined base class : '%s'", sBase);
				Warning(sBuffer, nLine);
				return false;
			}
			
			pNode = new entry_t;
			if(pNode == NULL)
			{
				AfxMessageBox("Memory allocation failure", MB_OK, 0);
				return false;
			}

			pNode->sKey = sKey;
			pNode->sName = sSmart;
			pNode->nLine = nLine;
			pNode->pNext = NULL;
			pNode->pSub = NULL;
			m_nEntities++;

			if(pTemp->pSub == NULL)	//first entry
				pTemp->pSub = pNode;
			else	//other entries exist so find the end and attach
			{
				//find the end of the current list
				pTemp = pTemp->pSub;
				if(pTemp->sName == sSmart)
				{
						//Warning("\"" + sSmart + "\" - Duplicate smart name", nLine);
						sBuffer.Format("Duplicate smart name for : '%s'", sSmart);
						Warning(sBuffer, nLine);
						return false;
				}

				for(; pTemp->pNext != NULL; pTemp = pTemp->pNext)
				{
					if(pTemp->sName == sSmart)
					{
						sBuffer.Format("Duplicate smart name for : '%s'", sSmart);
						Warning(sBuffer, nLine);
						return false;
					}
				}

				if(pTemp->sName == sSmart)
				{
						sBuffer.Format("Duplicate smart name for : '%s'", sSmart);
						Warning(sBuffer, nLine);
						return false;
				}
				
				//attach the node to the end
				pTemp->pNext = pNode;
			}
		}
	}
	
	return true;
}

bool CTreeBar::AddGame(CString sGame, int nLine)
{
	entry_t* pNode = NULL;	//new node
	entry_t* pTemp = NULL;	//temporary pointer
	CString sBuffer;

	if(!sGame.IsEmpty())
	{
		if(sGame.GetLength() > 32)
		{
			sBuffer.Format("String exceeds max length by (%d) characters : '%s'", sGame.GetLength() - 32, sGame);
			Warning(sBuffer, nLine);
		}

		Head.sGame = sGame;
		Head.nLine = nLine;
		return true;
	}
	else
		return false;
}

void CTreeBar::FillTreeCtrl()
{
	if(m_ctTree == NULL)
		return;

	HTREEITEM hGame = m_ctTree->InsertItem(Head.sGame, 0, 0, TVI_ROOT, TVI_LAST);
	m_ctTree->SetItemData(hGame, Head.nLine);
	TraceNode(Head.pSub, hGame);
}

//Inorder recursion to fill the tree control with the data in the tree
void CTreeBar::TraceNode(entry_t* pNode, HTREEITEM &hItem)
{
	if(pNode == NULL)
		return;

	HTREEITEM hTemp;

	if(pNode->sName.IsEmpty())
	{
		hTemp = m_ctTree->InsertItem(pNode->sKey, 1, 1, hItem, TVI_LAST);
		m_ctTree->SetItemData(hTemp, pNode->nLine);
	}
	else
	{
		hTemp = m_ctTree->InsertItem(pNode->sKey, 2, 2, hItem, TVI_LAST);
		m_ctTree->SetItemData(hTemp, pNode->nLine);
	}

	//trace the sub list to the end if it exists
	if(pNode->pSub != NULL)
		TraceNode(pNode->pSub, hTemp);

	if(pNode->pNext != NULL)	//trace the current list to the end
		TraceNode(pNode->pNext, hItem);
}

//destroy the list
void CTreeBar::DestroyNode(entry_t* pNode)
{
	if(pNode == NULL)
		return;

	if(pNode->pNext != NULL)	//trace the current list to the end
		DestroyNode(pNode->pNext);

	//trace the sub list to the end if it exists
	if(pNode->pSub != NULL)
		DestroyNode(pNode->pSub);
	
	//after the last node has been found the nodes can be deleted through
	//the recursion unwinding
	delete pNode;
}

//Find a match for sFind in the tree and return a pointer to the node if found
void CTreeBar::MatchString(entry_t* pNode, entry_t* &pFound, CString &sFind)
{
	if(pNode == NULL)
	{
		pFound = NULL;
		return;
	}

	if(sFind == pNode->sKey)
	{
		pFound = pNode;
		return;
	}
	
	if(pNode->pNext != NULL)
	{
		MatchString(pNode->pNext, pFound, sFind);
		if(pFound != NULL)
			return;
	}

	if(pNode->pSub != NULL)
	{
		MatchString(pNode->pSub, pFound, sFind);
		if(pFound != NULL)
			return;
	}
}

void CTreeBar::Warning(CString sMessage, int nLine)
{
	if(!m_bWarning)
		return;

	CString sBuffer;
	sBuffer.Format("  (%d)  WARNING >> %s", nLine, sMessage);
	m_pOwnerFrame->GetConsole()->AddMessage(sBuffer, WARNING_MSG, nLine);
}

void CTreeBar::Error(CString sMessage, int nLine)
{
	if(!m_bError)
		return;

	CString sBuffer;
	sBuffer.Format("  (%d)  ERROR >> %s", nLine, sMessage);
	m_pOwnerFrame->GetConsole()->AddMessage(sBuffer, ERROR_MSG, nLine);
}

void CTreeBar::SetMessageLevel(bool bWarn, bool bErr)
{
	m_bWarning = bWarn;
	m_bError = bErr;
}

unsigned int CTreeBar::GetEntities()
{
	return m_nEntities;
}

unsigned int CTreeBar::GetClasses()
{
	return m_nClasses;
}

void CTreeBar::SetOwnerFrame(CChildFrame* pOwnerFrame)
{
	m_pOwnerFrame = pOwnerFrame;
}

CString CTreeBar::GetGameName()
{
	return Head.sGame;
}

bool CTreeBar::GetCurItem(entry_t* &pEntry)
{
	CString sTemp;
	HTREEITEM hItem;

	hItem = m_ctTree->GetSelectedItem();
	if(m_ctTree->GetRootItem() == hItem)
	{
		pEntry = &Head;
		return true;
	}
	
	sTemp = m_ctTree->GetItemText(hItem);

	MatchString(Head.pSub, pEntry, sTemp);
	if(pEntry == NULL)
		return false;

	return true;
}

void CTreeBar::OnRclickTreeScript(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	HTREEITEM hSelect = m_ctTree->GetSelectedItem();
	if(hSelect != NULL)
		m_pOwnerFrame->GetTextView()->ScrollToLine(m_ctTree->GetItemData(hSelect));
	*pResult = 0;
}
