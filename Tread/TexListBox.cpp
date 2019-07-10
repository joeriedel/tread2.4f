// TexListBox.cpp : implementation file
//

#include "stdafx.h"
#include "tread3d.h"
#include "TexListBox.h"
#include "texapi.h"
#include "Tread3DDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define CLIST_BORDER_SIZE		3
#define CLIST_CAPTION_SIZE		14
#define CLIST_TOTAL_HEIGHT		((CLIST_BORDER_SIZE<<1) + CLIST_CAPTION_SIZE)
#define CLIST_TOTAL_WIDTH		((CLIST_BORDER_SIZE<<1))

/////////////////////////////////////////////////////////////////////////////
// CTexListBox

CTexListBox::CTexListBox()
{
	m_pTexList = NULL;
	m_pTexture = NULL;

	m_nMaxWidth = m_nMaxHeight = 64;
	m_nCount = 0;
}

CTexListBox::~CTexListBox()
{
}

BEGIN_MESSAGE_MAP(CTexListBox, CListBox)
	//{{AFX_MSG_MAP(CTexListBox)
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTexListBox message handlers

void CTexListBox::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	int x, y, nWidth, nHeight, rech, recw;
	CFont* pOldFont;
	CDC* pDC;
	CTexture* pTex;

	pDC = CDC::FromHandle(lpDrawItemStruct->hDC);	//Device context
	pTex = (CTexture*)lpDrawItemStruct->itemData; //texture item
	if(pTex == NULL)
	{
		pOldFont = pDC->SelectObject(&ftText);
		pDC->SetTextColor(RGB(255, 255, 255));
		pDC->TextOut(20, 10, "No matching texture(s)");
		pDC->SelectObject(pOldFont);
		pDC->FillSolidRect(&m_rcClient, RGB(0, 0, 0));
		return;
	}

	if(m_pTexture == NULL)
	{
		CRect rcItem, rcDraw;

		rcItem = lpDrawItemStruct->rcItem;
		rcDraw =  rcItem;

		if(m_pTexture == NULL)
		{
			// Has current focus.
			if(lpDrawItemStruct->itemAction & ODA_FOCUS)
			{
				// Focus rect
				if(lpDrawItemStruct->itemState & ODS_FOCUS)
				{
					pDC->Draw3dRect(&rcDraw, RGB(200, 200, 0), RGB(160, 160, 0));
					InflateRect(&rcDraw, -1, -1);

					pDC->Draw3dRect(&rcDraw, RGB(200, 200, 0), RGB(160, 160, 0));
					InflateRect(&rcDraw, -1, -1);

					pDC->Draw3dRect(&rcDraw, RGB(200, 200, 0), RGB(160, 160, 0));
				}
				else
				{
					pDC->Draw3dRect(&rcDraw, RGB(0, 0, 0), RGB(0, 0, 0));
					InflateRect(&rcDraw, -1, -1);

					pDC->Draw3dRect(&rcDraw, RGB(0, 0, 0), RGB(0, 0, 0));
					InflateRect(&rcDraw, -1, -1);

					pDC->Draw3dRect(&rcDraw, RGB(0, 0, 0), RGB(0, 0, 0));
				}
				
				return;
			}
		}

		pTex->Load();	//load the texture

		rcDraw = rcItem;
		InflateRect(&rcDraw, -CLIST_BORDER_SIZE, -CLIST_BORDER_SIZE);

		// Determine the width/height of the object.
		nWidth = (pTex->m_nWidth > m_nMaxWidth) ? m_nMaxWidth : pTex->m_nWidth;
		nHeight = (pTex->m_nHeight > m_nMaxHeight) ? m_nMaxHeight : pTex->m_nHeight;
		recw = rcDraw.right - rcDraw.left;
		rech = rcDraw.bottom - rcDraw.top - CLIST_CAPTION_SIZE;

		// Find the center for the texture.
		x = rcDraw.left + (recw >> 1) - (nWidth >> 1);
		y = rcDraw.top  + (rech >> 1) - (nHeight >> 1);
		
		bih.bmiHeader.biWidth = pTex->m_nSclWidth;
		bih.bmiHeader.biHeight = -pTex->m_nSclHeight;
				
		// Display the bitmap.
		StretchDIBits(pDC->m_hDC, x, y, nWidth, nHeight, 
				0, 0, pTex->m_nSclWidth, pTex->m_nSclHeight, pTex->m_pData, 
				(BITMAPINFO*)&bih, DIB_RGB_COLORS, SRCCOPY);

		if(m_nMaxHeight == 32)
			return;

		
		// Below the texture itself.
		rcDraw = rcItem;
		InflateRect(&rcDraw, -CLIST_BORDER_SIZE, -CLIST_BORDER_SIZE);

		// Move to position.
		rcDraw.top = rcDraw.bottom - CLIST_CAPTION_SIZE;
		pDC->FillSolidRect(&rcDraw, RGB(0, 90, 180));

		// Locate text position.
		x = rcDraw.left + 1;
		y = rcDraw.top + 1;

		// Write the text.
		pOldFont = pDC->SelectObject(&ftText);
		pDC->SetTextColor(RGB(255, 255, 255));
		pDC->SetBkColor(RGB(0, 90, 180));
		pDC->TextOut(x, y, pTex->m_sName);
		pDC->SelectObject(pOldFont);
	}
	else if(m_pTexture != NULL)
	{
		//int nWidth, nHeight;
		CRect rcClient;

		pTex->Load();
		GetClientRect(&rcClient);

		nWidth = rcClient.right - rcClient.left;
		nHeight = rcClient.bottom - rcClient.top;

		pDC->FillSolidRect(&rcClient, RGB(0, 0, 0));

		// Draw the texture info.
		nWidth = (pTex->m_nWidth > m_nMaxWidth) ? m_nMaxWidth : pTex->m_nWidth;
		nHeight = (pTex->m_nHeight > m_nMaxHeight) ? m_nMaxHeight : pTex->m_nHeight;
		bih.bmiHeader.biWidth = pTex->m_nSclWidth;
		bih.bmiHeader.biHeight = -pTex->m_nSclHeight;
					
		// Display the bitmap.
		StretchDIBits(pDC->m_hDC, 0, 0, nWidth, nHeight, 
			0, 0, pTex->m_nSclWidth, pTex->m_nSclHeight, pTex->m_pData, 
			(BITMAPINFO*)&bih, DIB_RGB_COLORS, SRCCOPY);
	}
	else
		pDC->FillSolidRect(&m_rcClient, RGB(0, 0, 0));
}

void CTexListBox::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
{
	lpMeasureItemStruct->itemWidth  = m_nMaxWidth + CLIST_TOTAL_WIDTH;
	lpMeasureItemStruct->itemHeight = m_nMaxHeight + CLIST_TOTAL_HEIGHT;
}

void CTexListBox::Initialize()
{
	SetMaxItemSize(64, 64);

	ftText.CreateFont(12, 0, 0, 0, 170,
						false, false, false,
						ANSI_CHARSET,
						OUT_DEFAULT_PRECIS,
						CLIP_DEFAULT_PRECIS,
						DEFAULT_QUALITY,
						DEFAULT_PITCH | FF_SWISS,
						"Arial");

	bih.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bih.bmiHeader.biPlanes = 1;
	bih.bmiHeader.biBitCount = 24;
	bih.bmiHeader.biCompression = BI_RGB;
	bih.bmiHeader.biSizeImage = 0;

	GetClientRect(&m_rcClient);
}

void CTexListBox::SetMaxItemSize(int nWidth, int nHeight)
{
	m_nMaxWidth = nWidth;
	m_nMaxHeight = nHeight;

	SetItemHeight(0, m_nMaxWidth + CLIST_TOTAL_HEIGHT);
	SetColumnWidth(m_nMaxHeight + CLIST_TOTAL_WIDTH);
	RedrawWindow();
}

bool CTexListBox::SetTexturePointer(CTexture* pTexture)
{
	int n;

	ResetContent();

	m_pTexList = NULL;
	m_pTexMru = NULL;
	m_pTexture = pTexture;
	
	if(m_pTexture == NULL)
	{
		EnableWindow(false);
		return false;
	}

	m_nCount = 1;
	
	n = AddString(m_pTexture->m_sName);
	SetItemData(n, (ULONG)m_pTexture);
	
	EnableWindow(true);
	return true;
}

bool CTexListBox::SetTexturePointer(CTexture** pTexList, int nCount)
{
	ResetContent();

	m_pTexMru = NULL;
	m_pTexture = NULL;
	m_pTexList = pTexList;
	m_nCount = nCount;

	if((m_pTexList == NULL) || (m_nCount <= 0))
	{
		EnableWindow(false);
		return false;
	}

	int n;
	for(int i = 0; i < nCount; i++)
	{
		n = AddString(m_pTexList[i]->m_sName);
		SetItemData(n, (ULONG)m_pTexList[i]);
	}

	EnableWindow(true);
	return true;
}

bool CTexListBox::SetTexturePointer(CLinkedList<TEXMRUREF>* pTexMru)
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
		EnableWindow(false);
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

	EnableWindow(true);
	return true;
}

void CTexListBox::Update()
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

		EnableWindow(true);
	}
	else
		EnableWindow(false);
}

void CTexListBox::Update(CString sFilter)
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
			if(m_pTexList[i]->m_sName.Find(sFilter) != -1)
			{
				n = AddString((LPCTSTR)m_pTexList[i]);
				SetItemData(n, (ULONG)m_pTexList[i]);
			}
		}

		EnableWindow(true);
	}
	else if(m_pTexMru != NULL)
	{
		m_pTexMru->First();
		for(pRef = m_pTexMru->GetCurItem(); pRef != NULL; pRef = m_pTexMru->GetNextItem())
		{
			if(pRef->sName.Find(sFilter) != -1)
			{
				n = AddString((LPCTSTR)pRef->sName);
				SetItemData(n, (ULONG)pRef->pTex);
				k++;
			}
		}

		m_nCount = k;
		EnableWindow(true);
	}
	else
		EnableWindow(false);
}

BOOL CTexListBox::OnEraseBkgnd(CDC* pDC) 
{
	//CRect rcItem;
	//GetClientRect(&rcItem);
	pDC->FillSolidRect(&m_rcClient, RGB(0, 0, 0));
	return true;
}

int CTexListBox::CompareItem(LPCOMPAREITEMSTRUCT lpCompareItemStruct) 
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

void CTexListBox::ResetList()
{
	ResetContent();
}

void CTexListBox::OnSize(UINT nType, int cx, int cy) 
{
	CListBox::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	GetClientRect(&m_rcClient);
}

CTexture* CTexListBox::GetTexturePointer()
{
	if(m_pTexture != NULL)
		return m_pTexture;

	if(m_pTexList != NULL)
		return m_pTexList[0];

	if(m_pTexMru != NULL)
	{
		TEXMRUREF* pRef = m_pTexMru->GetCurItem();
		if(pRef != NULL)
			return pRef->pTex;
	}

	return NULL;
}