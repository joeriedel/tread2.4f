// TextureView.cpp : implementation file
//

#include "stdafx.h"
#include "tread3d.h"
#include "viewControl.h"
#include "BrowserDialog.h"
//#include "DialogControl.h"
#include "TexAPI.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTextureView

IMPLEMENT_DYNCREATE(CTextureView, CFormView)

CTextureView::CTextureView()
	: CFormView(CTextureView::IDD)
{
	//{{AFX_DATA_INIT(CTextureView)
	m_sName = _T("");
	//}}AFX_DATA_INIT
	m_rcBrowse.SetRectEmpty();
	m_nPos = 1;
	m_sFilter = _T("");
}

CTextureView::~CTextureView()
{
}

void CTextureView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTextureView)
	DDX_Control(pDX, IDC_COMBO_MRU, m_cbMru);
	DDX_Control(pDX, IDC_LIST_PREVIEW, m_lbPreview);
	DDX_Control(pDX, IDC_BUTTON_BROWSE, m_btBrowse);
	DDX_Control(pDX, IDC_COMBO_TEXTURE, m_cbTexture);
	DDX_Text(pDX, IDC_STATIC_NAME, m_sName);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTextureView, CFormView)
	//{{AFX_MSG_MAP(CTextureView)
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_BUTTON_BROWSE, OnButtonBrowse)
	ON_CBN_SELCHANGE(IDC_COMBO_TEXTURE, OnSelchangeComboTexture)
	ON_CBN_SELCHANGE(IDC_COMBO_MRU, OnSelchangeComboMru)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTextureView diagnostics

#ifdef _DEBUG
void CTextureView::AssertValid() const
{
	CFormView::AssertValid();
}

void CTextureView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTextureView message handlers

void CTextureView::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();

	m_cbTexture.Initialize();
	m_lbPreview.Initialize();
	m_cbMru.Initialize();
	m_lbPreview.SetMaxItemSize(64, 64);

	m_lbPreview.EnableWindow(false);
	m_btBrowse.EnableWindow(false);
	m_cbTexture.EnableWindow(false);
}

DROPEFFECT CTextureView::OnDragOver(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CFormView::OnDragOver(pDataObject, dwKeyState, point);
}

int CTextureView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CFormView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	RegisterView(TEXTURE_VIEW, this);
	
	return 0;
}

void CTextureView::UpdateSelection()
{
	CTexture* pTexture = GetTexAPI()->GetCurTexture();

	int n = m_cbTexture.SelectString(-1, pTexture->m_sName);
	m_cbTexture.SetCurSel(n);
	OnSelchangeComboTexture();
}

void CTextureView::LoadList()
{
	CTexture** pList;
	int nCount;
	pList = GetTexAPI()->GetFullPakTexList(nCount);
	if(m_cbTexture.SetTexturePointer(pList, nCount))
	{
		m_btBrowse.EnableWindow(true);
		m_cbTexture.EnableWindow(true);
		m_cbMru.EnableWindow(true);
		m_lbPreview.EnableWindow(true);
		CTexture* pTex = GetTexAPI()->GetCurTexture();
		if(pTex != NULL)
		{
			m_lbPreview.SetTexturePointer(pTex);
			int nSelect = m_cbTexture.SelectString(-1, pTex->m_sName);
			m_cbTexture.SetCurSel(nSelect);
			LoadMRU(GetMapManager()->GetDocPtr()->GetTextureMRU());

			m_sName = pTex->m_sName;
		}
		else
		{
			m_cbTexture.SetCurSel(0);
		}
	}
	else
	{
		m_btBrowse.EnableWindow(false);
		m_cbTexture.EnableWindow(false);
		m_cbMru.EnableWindow(false);
		m_lbPreview.EnableWindow(false);
	}

	UpdateData(false);
}

void CTextureView::LoadMRU(CLinkedList<TEXMRUREF>* pList)
{
	if(!m_cbMru.SetTexturePointer(pList))
		m_cbMru.EnableWindow(false);
	else
	{
		m_cbMru.SetCurSel(0);
		m_cbMru.EnableWindow(true);
	}
}

void CTextureView::UnloadList()
{
	m_cbTexture.ResetList();
	m_cbMru.ResetList();
	m_lbPreview.ResetList();
}

void CTextureView::OnButtonBrowse() 
{
	CBrowserDialog dlgBrowse;
	//setup the dialog
	dlgBrowse.SetFilterString(m_sFilter);
	dlgBrowse.SetSizePosition(m_nPos);
	dlgBrowse.SetWindowPos(m_rcBrowse);
	//show the dialog
	dlgBrowse.DoModal();
	//get info from the dialog
	dlgBrowse.GetWindowPos(m_rcBrowse);
	m_sFilter = dlgBrowse.GetFilterString();
	m_nPos = dlgBrowse.GetSizePosition();

	CTexture* pTexture = GetTexAPI()->GetCurTexture();
	if(pTexture != NULL)
	{
		m_cbMru.EnableWindow(true);
		int n = m_cbTexture.SelectString(-1, pTexture->m_sName);
		m_cbTexture.SetCurSel(n);
		OnSelchangeComboTexture();
		m_lbPreview.SetTexturePointer(pTexture);
	}
}

void CTextureView::OnSelchangeComboTexture()
{
	// TODO: Add your control notification handler code here
	int nSelect = m_cbTexture.GetCurSel();	
	if(nSelect == CB_ERR)
		return;

	
	CTexture* pTex = (CTexture*)m_cbTexture.GetItemData(nSelect);
	GetTexAPI()->SetCurTexture(pTex);
	m_lbPreview.SetTexturePointer(pTex);

	m_sName = pTex->m_sName;

	int nItem = m_cbMru.SelectString(-1, pTex->m_sName);

	if(nItem == -1)
	{
		m_cbMru.EnableWindow(true);
		GetMapManager()->GetDocPtr()->AddTextureToMRU(pTex);
	}

	UpdateData(false);
}

void CTextureView::OnSelchangeComboMru() 
{
	// TODO: Add your control notification handler code here
	int nSelect = m_cbMru.GetCurSel();	
	if(nSelect == CB_ERR)
		return;

	CTexture* pTex = (CTexture*)m_cbMru.GetItemData(nSelect);
	GetTexAPI()->SetCurTexture(pTex);
	m_lbPreview.SetTexturePointer(pTex);
	m_cbTexture.SelectString(-1, pTex->m_sName);

	m_sName = pTex->m_sName;
	UpdateData(false);
}
