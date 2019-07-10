// FlagsDialog.cpp : implementation file
//

#include "stdafx.h"
#include "tread3d.h"
#include "FlagsDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFlagsDialog dialog


CFlagsDialog::CFlagsDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CFlagsDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFlagsDialog)
	m_nFlags = 0;
	//}}AFX_DATA_INIT

	m_nCount = 1;
}


void CFlagsDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFlagsDialog)
	DDX_Control(pDX, IDC_LIST_FLAGS, m_lcFlags);
	DDX_Text(pDX, IDC_EDIT_FLAGS, m_nFlags);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFlagsDialog, CDialog)
	//{{AFX_MSG_MAP(CFlagsDialog)
	ON_BN_CLICKED(IDC_BUTTON_DONE, OnButtonDone)
	ON_NOTIFY(NM_CLICK, IDC_LIST_FLAGS, OnClickListFlags)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFlagsDialog message handlers

BOOL CFlagsDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_lcFlags.InsertColumn(0 , "", LVCFMT_LEFT, 125, -1);

	m_imgList.Create(IDB_BITMAP_CHECK2, 16, 0, RGB(255, 0, 255));
	m_lcFlags.SetImageList(&m_imgList, LVSIL_STATE);
	
	ENTITYFLAGOPTIONDEFINITION* pFlag;
	int n;
	m_pFlagList->First();
	for(pFlag = m_pFlagList->GetCurItem(); pFlag != NULL; pFlag = m_pFlagList->GetNextItem())
	{
		n = m_lcFlags.InsertItem(m_nCount, pFlag->sString, 0);
		m_lcFlags.SetItemData(n, pFlag->nValue);

		if(m_nFlags & pFlag->nValue)
			m_lcFlags.SetCheck(n, true);
		else
			m_lcFlags.SetCheck(n, false);

		m_nCount++;
	}

	UpdateData(false);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CFlagsDialog::OnButtonDone() 
{
	// TODO: Add your control notification handler code here
	OnOK();
}

void CFlagsDialog::AddFlag(CString sName, int nValue)
{
	int n;
	n = m_lcFlags.InsertItem(m_nCount, sName);
	m_lcFlags.SetItemData(n, nValue);

	if(m_nFlags & nValue)
		m_lcFlags.SetCheck(n, true);
	else
		m_lcFlags.SetCheck(n, false);

	m_nCount++;
}

CString CFlagsDialog::GetFlags()
{
	CString sBuffer;
	sBuffer.Format("%d", m_nFlags);
	return sBuffer;
}

void CFlagsDialog::SetFlags(int nFlags)
{
	m_nFlags = nFlags;
}

void CFlagsDialog::SetFlags(CString sFlags)
{
	m_nFlags = atoi((LPCSTR)sFlags);
}

void CFlagsDialog::OnClickListFlags(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NMLISTVIEW* pNMItem = (NMLISTVIEW*)pNMHDR;
	LVHITTESTINFO hitTest;
	int nPos;

	hitTest.pt = pNMItem->ptAction;
	m_lcFlags.SubItemHitTest(&hitTest);
	nPos = hitTest.iItem;

	if(nPos == -1)
		return;

	int nValue = m_lcFlags.GetItemData(nPos);
	if(!m_lcFlags.GetCheck(nPos))	//selecting options
	{
		m_nFlags |= nValue;
		m_lcFlags.SetCheck(nPos, true);
	}
	else		//unselecting options
	{
		m_nFlags &= ~nValue;
		m_lcFlags.SetCheck(nPos, false);
	}

	UpdateData(false);
	
	*pResult = 0;
}
