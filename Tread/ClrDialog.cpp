// ClrDialog.cpp : implementation file
//

#include "stdafx.h"
#include "tread3d.h"
#include "ClrDialog.h"
#include "TreadSys.h"
#include "ColorTable.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CClrDialog dialog


CClrDialog::CClrDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CClrDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CClrDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_bDraw = true;
	m_penDraw = NULL;
	m_pDC = NULL;
	m_clText = 0;
}


void CClrDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CClrDialog)
	DDX_Control(pDX, IDC_STATIC_DISPLAY, m_stDisplay);
	DDX_Control(pDX, IDC_LIST_ITEMS, m_lbItems);
	DDX_Control(pDX, IDC_BUTTON_COLOR, m_btColor);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CClrDialog, CDialog)
	//{{AFX_MSG_MAP(CClrDialog)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, OnButtonClose)
	ON_BN_CLICKED(IDC_BUTTON_COLOR, OnButtonColor)
	ON_BN_CLICKED(IDC_BUTTON_RESET, OnButtonReset)
	ON_LBN_SELCHANGE(IDC_LIST_ITEMS, OnSelchangeListItems)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CClrDialog message handlers

void CClrDialog::OnDestroy() 
{
	CDialog::OnDestroy();
	
	if(m_pDC != NULL)
		m_stDisplay.ReleaseDC(m_pDC);
}

BOOL CClrDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CWinApp* pApp = AfxGetApp();
	HICON icButton;

	icButton = pApp->LoadIcon(IDI_ICON_PALETTE);
	m_btColor.SetIcon(icButton);

	m_btColor.EnableWindow(false);

	int nCount = GetColorTable()->GetTableCount();
	for(int n = 0; n < nCount; n++)
		m_lbItems.AddString(GetColorTable()->GetColorItemString(n));

	m_pDC = m_stDisplay.GetDC();
	if(m_pDC == NULL)
	{
		Error("Cannot get DC");
		return false;
	}
	
	m_stDisplay.GetClientRect(m_rcDisplay);

	m_rcBox.right = m_rcDisplay.right - 20;
	m_rcBox.left = m_rcDisplay.left + 20;
	m_rcBox.top = m_rcDisplay.top + 10;
	m_rcBox.bottom = m_rcDisplay.bottom - 10;

	InvalidateRect(m_rcDisplay, false);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CClrDialog::OnButtonClose() 
{
	OnOK();
}

void CClrDialog::OnButtonColor() 
{
	int nTemp;
	nTemp = m_lbItems.GetCurSel();
	if(nTemp == -1)
	{
		m_btColor.EnableWindow(false);
		return;
	}

	COLORREF nColor;
	CColorDialog dlgTemp;

	dlgTemp.m_cc.Flags = CC_FULLOPEN | CC_RGBINIT;
	dlgTemp.m_cc.rgbResult = GetColorTable()->GetColor(nTemp);
	
	if(dlgTemp.DoModal() == IDOK)
	{
		m_penDraw = NULL;
		nColor = dlgTemp.GetColor();
		GetColorTable()->SetColor(nTemp, nColor);
		m_penDraw = GetColorTable()->GetPen(nTemp);
		m_clText = GetColorTable()->GetColor(nTemp);
		InvalidateRect(m_rcDisplay, false);
	}
}

void CClrDialog::OnButtonReset() 
{
	m_penDraw = NULL;

	GetColorTable()->Default();

	int nTemp;
	nTemp = m_lbItems.GetCurSel();
	if(nTemp == -1)
		m_penDraw = NULL;
	else
	{
		m_penDraw = GetColorTable()->GetSolidPen(nTemp);
		m_clText = GetColorTable()->GetColor(nTemp);
	}

	m_btColor.EnableWindow(true);
	InvalidateRect(m_rcDisplay, false);
}

void CClrDialog::OnSelchangeListItems() 
{
	CColorTable* pTable = GetColorTable();

	int nTemp;
	nTemp = m_lbItems.GetCurSel();
	if(nTemp == -1)
		m_penDraw = NULL;
	else
	{
		m_penDraw = GetColorTable()->GetPen(nTemp);
		m_clText = GetColorTable()->GetColor(nTemp);
	}

	m_btColor.EnableWindow(true);

	InvalidateRect(m_rcDisplay, false);
}

void CClrDialog::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	m_pDC->FillSolidRect(m_rcDisplay, RGB(0, 0, 0));

	if(m_penDraw != NULL)
	{
		CPen* pOldPen = m_pDC->SelectObject(m_penDraw);

		m_pDC->MoveTo(m_rcBox.right, m_rcBox.top);
		m_pDC->LineTo(m_rcBox.left, m_rcBox.top);
		m_pDC->LineTo(m_rcBox.left, m_rcBox.bottom);
		m_pDC->LineTo(m_rcBox.right, m_rcBox.bottom);
		m_pDC->LineTo(m_rcBox.right, m_rcBox.top);

		m_pDC->SetTextColor(m_clText);
		m_pDC->TextOut(55, 25, "Sample");
		
		m_pDC->SelectObject(pOldPen);
	}
}
