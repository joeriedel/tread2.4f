// BrowserDialog.cpp : implementation file
//

#include "stdafx.h"
#include "tread3d.h"
#include "BrowserDialog.h"
#include "TreadSys.h"
#include "ColorCycleController.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBrowserDialog dialog

int g_nSize[] = {32, 64, 128, 256};

CBrowserDialog::CBrowserDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CBrowserDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBrowserDialog)
	m_sFilter = _T("");
	m_sTexname = _T("");
	//}}AFX_DATA_INIT

	m_bInit = false;
	bNewPos = false;
}

void CBrowserDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBrowserDialog)
	DDX_Control(pDX, IDC_LIST_TEXTURE, m_lbPreview);
	DDX_Control(pDX, IDC_BUTTON_FILTER, m_btFilter);
	DDX_Control(pDX, IDC_TEXTURESIZELIST, m_cbSize);
	DDX_Control(pDX, IDC_BUTTON_DONE, m_btDone);
	DDX_Control(pDX, IDC_TEXTURELISTBOX, m_lbTexture);
	DDX_Text(pDX, IDC_EDIT_FILTER, m_sFilter);
	DDX_Text(pDX, IDC_STATIC_TEXNAME, m_sTexname);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CBrowserDialog, CDialog)
	//{{AFX_MSG_MAP(CBrowserDialog)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON_DONE, OnButtonDone)
	ON_CBN_SELCHANGE(IDC_TEXTURESIZELIST, OnSelchangeTexturesizelist)
	ON_LBN_SELCHANGE(IDC_TEXTURELISTBOX, OnSelchangeTexturelistbox)
	ON_LBN_DBLCLK(IDC_TEXTURELISTBOX, OnDblclkTexturelistbox)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_BUTTON_FILTER, OnButtonFilter)
	ON_BN_CLICKED(IDC_BUTTON_RESET, OnButtonReset)
	ON_EN_CHANGE(IDC_EDIT_FILTER, OnChangeEditFilter)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBrowserDialog message handlers

BOOL CBrowserDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CRect rcClient1;
	CRect rcClient2;

	//get dialog client rect
	GetClientRect(&m_rcClient);
	m_rcSize = m_rcClient;
	CalcWindowRect(m_rcSize);
	ClientToScreen(&m_rcClient);
	
	m_cbSize.SetCurSel(1);

	CTexture** pList;
	int nCount;
	pList = GetTexAPI()->GetFullPakTexList(nCount);

	m_lbTexture.Initialize();
	if(!m_lbTexture.SetTexturePointer(pList, nCount))
		return false;

	m_lbPreview.Initialize();
	m_lbPreview.SetMaxItemSize(256, 256);
	
	int n = 0;
	CTexture* pTexture = GetTexAPI()->GetCurTexture();
	if(pTexture != NULL)
	{
		pTexture->Load();
		n = m_lbTexture.SelectString(-1, pTexture->m_sName);
		m_lbTexture.SetCurSel(n);
		m_sTexname.Format("%s @ %dx%d", pTexture->m_sName, pTexture->m_nWidth, pTexture->m_nHeight);
		UpdateData(false);
		m_lbPreview.SetTexturePointer(pTexture);
	}
	else
		m_lbTexture.SetCurSel(0);
		
	m_lbPreview.GetClientRect(&rcClient1);
	m_lbTexture.GetClientRect(&rcClient2);
	
	m_ptAnchor.y = 1;
	m_ptAnchor.x = rcClient1.right + 10;

	m_ptMin.y = m_ptAnchor.y + rcClient2.bottom;
	m_ptMin.x = m_ptAnchor.x + rcClient2.right;

	m_bInit = true;

	m_cbSize.SetCurSel(m_nPos);
	m_lbTexture.SetMaxItemSize(g_nSize[m_nPos], g_nSize[m_nPos]);
	m_lbTexture.SetFocus();

	m_lbTexture.Update(m_sFilter);

	UpdateData(false);

	m_lbTexture.SetCurSel(n);
	
	GetColorCycleController()->StartAnimating(false);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CBrowserDialog::OnSize(UINT nType, int cx, int cy) 
{
	if(m_bInit)
	{
		if((cx >= m_ptMin.x) && (cy >= m_ptMin.y))
		{
			CDialog::OnSize(nType, cx, cy);
			m_lbTexture.MoveWindow(m_ptAnchor.x, m_ptAnchor.y, cx - m_ptAnchor.x, cy - m_ptAnchor.y);
			
			GetClientRect(&m_rcClient);
			m_rcSize = m_rcClient;
			CalcWindowRect(m_rcSize);
			ClientToScreen(&m_rcClient);
		}
	}
}

void CBrowserDialog::OnButtonDone() 
{
	GetMapManager()->CycleColorIfSelected();
	OnOK();
}

void CBrowserDialog::OnSelchangeTexturesizelist() 
{
	int nTemp = m_cbSize.GetCurSel();
	if(nTemp != -1)
	{
		m_lbTexture.SetMaxItemSize(g_nSize[nTemp], g_nSize[nTemp]);
		m_lbTexture.SetCurSel(m_lbTexture.GetCurSel());
		m_lbTexture.SetFocus();
		m_nPos = nTemp;
	}
}

void CBrowserDialog::OnSelchangeTexturelistbox() 
{
	int i;
	CTexture* pTexture;

	i = m_lbTexture.GetCurSel();
	pTexture = (CTexture*)m_lbTexture.GetItemData(i);
	if(pTexture == NULL)
	{
		Error("Texture is NULL");
		return;
	}
	GetTexAPI()->SetCurTexture(pTexture);
	m_lbPreview.SetTexturePointer(pTexture);

	m_sTexname.Format("%s @ %dx%d", pTexture->m_sName, pTexture->m_nWidth, pTexture->m_nHeight);
	UpdateData(false);
}

void CBrowserDialog::OnDblclkTexturelistbox() 
{
	int i;
	CTexture* pTexture;

	i = m_lbTexture.GetCurSel();
	pTexture = (CTexture*)m_lbTexture.GetItemData(i);
	if(pTexture == NULL)
	{
		Error("Texture is NULL");
		return;
	}
	GetTexAPI()->SetCurTexture(pTexture);

	GetMapManager()->CycleColorIfSelected();
	OnOK();
}

void CBrowserDialog::GetWindowPos(CRect &m_rcTemp)
{
	m_rcTemp.left = m_rcClient.left + m_rcSize.left;
	m_rcTemp.top = m_rcClient.top + m_rcSize.top;
	m_rcTemp.right = m_rcSize.right - m_rcSize.left;
	m_rcTemp.bottom = m_rcSize.bottom - m_rcSize.top;
}

void CBrowserDialog::SetWindowPos(CRect rcPos)
{
	if(rcPos.bottom <= 0)
		return;

	m_rcNewSize = rcPos;
	bNewPos = true;
}

void CBrowserDialog::OnShowWindow(bool bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	if(bShow & bNewPos)
		CDialog::SetWindowPos(NULL, 
						m_rcNewSize.left, 
						m_rcNewSize.top, 
						m_rcNewSize.right, 
						m_rcNewSize.bottom, 
						0);
}

void CBrowserDialog::OnButtonFilter() 
{
	BeginWaitCursor();
	SetCapture();

	UpdateData(true);
	m_lbTexture.Update(m_sFilter);

	ReleaseCapture();
	EndWaitCursor();
}

void CBrowserDialog::OnButtonReset() 
{
	BeginWaitCursor();
	SetCapture();

	m_sFilter.Empty();
	UpdateData(false);
	m_lbTexture.Update();

	ReleaseCapture();
	EndWaitCursor();
}

void CBrowserDialog::OnChangeEditFilter() 
{
	UpdateData(true);
	if(m_sFilter.IsEmpty())
		m_btFilter.EnableWindow(false);
	else
		m_btFilter.EnableWindow(true);
}

int CBrowserDialog::GetSizePosition()
{
	return m_nPos;
}

void CBrowserDialog::SetSizePosition(int nPos)
{
	m_nPos = nPos;
}

CString CBrowserDialog::GetFilterString()
{
	return m_sFilter;
}

void CBrowserDialog::SetFilterString(CString sFilter)
{
	m_sFilter = sFilter;
}
