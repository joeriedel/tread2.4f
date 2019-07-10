// PopupDialog.cpp : implementation file
//

#include "stdafx.h"
#include "GdfEdit2.h"
#include "PopupDialog.h"
#include "GdfEdit2View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPopupDialog dialog


CPopupDialog::CPopupDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CPopupDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPopupDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CPopupDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPopupDialog)
	DDX_Control(pDX, IDC_LIST_POPUP, m_lbItems);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPopupDialog, CDialog)
	//{{AFX_MSG_MAP(CPopupDialog)
	ON_WM_CREATE()
	ON_LBN_KILLFOCUS(IDC_LIST_POPUP, OnKillfocusListPopup)
	ON_WM_SETFOCUS()
	ON_BN_CLICKED(IDC_BUTTON_CAPTURE, OnButtonCapture)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPopupDialog message handlers

int CPopupDialog::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	GetClientRect(&m_rcWindow);
	m_lbItems.SetParentWindow(this);
	
	return 0;
}

void CPopupDialog::SetWindowPosition(CPoint ptPos)
{
	CDialog::SetWindowPos(NULL, 
						ptPos.x, 
						ptPos.y, 
						ptPos.x + (m_rcWindow.right - m_rcWindow.left), 
						ptPos.y + (m_rcWindow.bottom - m_rcWindow.top), SWP_NOSIZE | SWP_SHOWWINDOW);
}

void CPopupDialog::SetFindString(CString sMatch)
{
	m_sMatch = sMatch;
	//m_lbItems.SelectString(-1, m_sMatch);
	m_lbItems.SetUserString(sMatch);
}

void CPopupDialog::AddToFindString(CString sMatch)
{
	m_sMatch += sMatch;
	m_lbItems.SelectString(-1, m_sMatch);
}

CString CPopupDialog::GetSelectedString()
{
	return m_sSelection;
}

void CPopupDialog::AddStringItem(CString sItem)
{
	m_lbItems.AddString(sItem);
}

void CPopupDialog::AddStringList(CString pList[], int nLength)
{
	EnableWindow(false);

	CString* pString;
	int nItem;
	for(int n = 0; n < nLength; n++)
	{
		pString = new CString(pList[n]);
		nItem = m_lbItems.AddString(pList[n]);
		m_lbItems.SetItemData(nItem, (ULONG)pString);
	}

	EnableWindow(true);
}

void CPopupDialog::AddStringList(char pList[], int nLength)
{
	CString* pString;
	int nItem;
	for(int n = 0; n < nLength; n++)
	{
		pString = new CString(pList[n]);
		nItem = m_lbItems.AddString((LPCSTR)pList[n]);
		m_lbItems.SetItemData(nItem, (ULONG)pString);
	}
}

void CPopupDialog::OnKillfocusListPopup() 
{
	// TODO: Add your control notification handler code here
	ShowWindow(false);
}

CGdfEdit2View* CPopupDialog::GetParentWindow()
{
	return m_pView;
}

void CPopupDialog::SetParentWindow(CGdfEdit2View* pView)
{
	m_pView = pView;
}

void CPopupDialog::OnSetFocus(CWnd* pNewWnd) 
{
	CDialog::OnSetFocus(pNewWnd);
	
	m_lbItems.SetFocus();
	
}

void CPopupDialog::OnButtonCapture() 
{
	m_lbItems.CompleteText();
}
