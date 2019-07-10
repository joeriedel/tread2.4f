// TexComboBox.cpp : implementation file
//

#include "stdafx.h"
#include "tread3d.h"
#include "TexComboBox.h"
#include "TexAPI.h"
#include "Tread3DDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define CTEXCOMBO_SPACING		(10)
#define CTEXCOMBO_ITEM_HEIGHT	(64 + CTEXCOMBO_SPACING)
#define COLOR_ITEM_HEIGHT   20

/////////////////////////////////////////////////////////////////////////////
// CTexComboBox

CTexComboBox::CTexComboBox()
{
	m_pTexture = NULL;
	m_pTexList = NULL;
	m_pTexMru = NULL;
	m_nCount = NULL;
}

CTexComboBox::~CTexComboBox()
{
}


BEGIN_MESSAGE_MAP(CTexComboBox, CComboBox)
	//{{AFX_MSG_MAP(CTexComboBox)
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTexComboBox message handlers

int CTexComboBox::CompareItem(LPCOMPAREITEMSTRUCT lpCompareItemStruct) 
{
	// return -1 = item 1 sorts before item 2
	// return 0 = item 1 and item 2 sort the same
	// return 1 = item 1 sorts after item 2
	int i;
	CTexture* pTex1, *pTex2;

	pTex1 = (CTexture*)lpCompareItemStruct->itemData1;
	pTex2 = (CTexture*)lpCompareItemStruct->itemData2;

	if((pTex1 == NULL) || (pTex2 == NULL))
		return -1;

	i = strcmp(pTex1->m_sName, pTex2->m_sName);
	(i < 0) ? i = -1 : (i > 0) ? i = 1 : i = 0;

	return i;
}

void CTexComboBox::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	int nItem, nWidth, nHeight;
	CString sInfo;
	CString sName;
	COLORREF textcolor;
	CTexture* pTexture;
	CRect rcItem;
	CFont* pOldFont;
	CDC* pDC;
	
	pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
	
	pTexture = (CTexture*)lpDrawItemStruct->itemData; // RGB in item data
	if(pTexture == NULL)
		return;

	rcItem = lpDrawItemStruct->rcItem;
	
	// Just in combo edit area?
	if(lpDrawItemStruct->itemState & ODS_COMBOBOXEDIT)
	{
		// Draw in edit box.
		nItem = GetCurSel();

		pOldFont = pDC->SelectObject(&m_ftText);
		// Draw the encompasing rect.
		pDC->FillSolidRect(&rcItem, GetSysColor(COLOR_HIGHLIGHT));
		if(nItem != CB_ERR)
		{
			sName = pTexture->m_sName;
			pDC->SetTextColor(RGB(255, 255, 255));
			pDC->TextOut(5, 5, sName);
		}
		else
		{
			pDC->SetTextColor(RGB(255, 255, 255));
			pDC->TextOut(5, 5, "No Item");
			//EnableWindow(false);
		}

		pDC->SelectObject(pOldFont);
		return;
	}

	// Don't draw if there's nothing there.
	if(GetCount() == 0)
		return;
	
	pTexture->Load();

	// Has current focus.
	if((lpDrawItemStruct->itemAction & ODA_FOCUS) && (lpDrawItemStruct->itemState & ODS_FOCUS))
	{
		// Draw the encompasing rect.
		textcolor = RGB(0, 0, 0);
		pDC->FillSolidRect(&rcItem, RGB(0, 180, 180));
	}
	else
	{
		textcolor = RGB(200, 200, 0);
		pDC->FillSolidRect(&rcItem, RGB(0, 0, 0));
	}

	// Draw the full texture.
	if(pTexture != NULL)
	{
		nWidth = (pTexture->m_nWidth > 64) ? 64 : pTexture->m_nWidth;
		nHeight = (pTexture->m_nHeight > 64) ? 64 : pTexture->m_nHeight;
		
		sInfo.Format("%dx%d", pTexture->m_nWidth, pTexture->m_nHeight);
		sName = pTexture->m_sName;

		m_bih.bmiHeader.biWidth = pTexture->m_nSclWidth;
		m_bih.bmiHeader.biHeight = -pTexture->m_nSclHeight;
		
		
		// Display the bitmap.
		StretchDIBits(pDC->m_hDC, rcItem.left+5, rcItem.top+5, nWidth, nHeight, 
			0, 0, pTexture->m_nSclWidth, pTexture->m_nSclHeight, pTexture->m_pData, 
			(BITMAPINFO*)&m_bih, DIB_RGB_COLORS, SRCCOPY);

		pOldFont = pDC->SelectObject(&m_ftText);

		pDC->SetTextColor(textcolor);
		pDC->TextOut(rcItem.left + 75, rcItem.top+5, sInfo);
		//pDC->TextOut(rcItem.left + 75, rcItem.top+5, sName);
		//pDC->TextOut(rcItem.left + 75, rcItem.top+20, sInfo);
			
		pDC->SelectObject(pOldFont);
	}
	else
		Error("CTextureComboBox::DrawItem(): Texture is NULL.");
}
	
void CTexComboBox::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
{
	lpMeasureItemStruct->itemHeight = CTEXCOMBO_ITEM_HEIGHT;
}

BOOL CTexComboBox::OnEraseBkgnd(CDC* pDC) 
{
	CRect rcItem;
	GetClientRect(&rcItem);
	pDC->FillSolidRect(&rcItem, RGB(0, 0, 0));
	return true;
}

void CTexComboBox::Initialize()
{
	m_ftText.CreateFont(14, 0, 0, 0, 200,
						false, false, false,
						ANSI_CHARSET,
						OUT_DEFAULT_PRECIS,
						CLIP_DEFAULT_PRECIS,
						DEFAULT_QUALITY,
						DEFAULT_PITCH | FF_SWISS,
						"Arial");

	m_bih.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	m_bih.bmiHeader.biPlanes = 1;
	m_bih.bmiHeader.biBitCount = 24;
	m_bih.bmiHeader.biCompression = BI_RGB;
	m_bih.bmiHeader.biSizeImage = 0;
}

bool CTexComboBox::SetTexturePointer(CTexture* pTexture)
{
	int n;

	ResetContent();

	m_pTexList = NULL;
	m_pTexMru = NULL;
	m_pTexture = pTexture;
	
	if(m_pTexture == NULL)
	{
		//EnableWindow(false);
		return false;
	}

	m_nCount = 1;
	
	n = AddString(m_pTexture->m_sName);
	SetItemData(n, (ULONG)m_pTexture);
	
	//SetCurSel(0);
	return true;
}

bool CTexComboBox::SetTexturePointer(CTexture** pTexList, int nCount)
{
	ResetContent();

	m_pTexMru = NULL;
	m_pTexture = NULL;
	m_pTexList = pTexList;
	m_nCount = nCount;

	if((m_pTexList == NULL) || (m_nCount <= 0))
	{
		//EnableWindow(false);
		return false;
	}

	int n;
	for(int i = 0; i < nCount; i++)
	{
		n = AddString(m_pTexList[i]->m_sName);
		SetItemData(n, (ULONG)m_pTexList[i]);
	}

	//SetCurSel(0);

	//EnableWindow(true);
	return true;
}

bool CTexComboBox::SetTexturePointer(CLinkedList<TEXMRUREF>* pTexMru)
{
	int n;
	int k = 0;
	TEXMRUREF* pRef;

	ResetContent();

	m_pTexList = NULL;
	m_pTexture = NULL;
	m_pTexMru = pTexMru;

	if((m_pTexMru == NULL) || (m_pTexMru->IsEmpty()))
	{
		//EnableWindow(false);
		return false;
	}

	m_pTexMru->First();
	for(pRef = m_pTexMru->GetCurItem(); pRef != NULL; pRef = m_pTexMru->GetNextItem())
	{
		n = AddString(pRef->sName);
		SetItemData(n, (ULONG)pRef->pTex);
		k++;
	}

	m_nCount = k;

	//SetCurSel(0);

	//EnableWindow(true);
	return true;
}

void CTexComboBox::Update()
{
	int n;
	int k = 0;
	TEXMRUREF* pRef;

	ResetContent();

	if(m_pTexture != NULL)
	{
		n = AddString(m_pTexture->m_sName);
		SetItemData(n, (ULONG)m_pTexture);
		
		EnableWindow(true);
	}
	else if(m_pTexList != NULL)
	{
		for(int i = 0; i < m_nCount; i++)
		{
			n = AddString(m_pTexList[i]->m_sName);
			SetItemData(n, (ULONG)m_pTexList[i]);
		}

		EnableWindow(true);
	}
	else if(m_pTexMru != NULL)
	{
		m_pTexMru->First();
		for(pRef = m_pTexMru->GetCurItem(); pRef != NULL; pRef = m_pTexMru->GetNextItem())
		{
			n = AddString(pRef->sName);
			SetItemData(n, (ULONG)pRef->pTex);
			k++;
		}

		m_nCount = k;

		//EnableWindow(true);
	}
	//else
		//EnableWindow(false);
}

void CTexComboBox::ResetList()
{
	ResetContent();
	EnableWindow(false);
	SetCurSel(-1);
	//GetTexAPI()->SetCurTexture(NULL);
}
