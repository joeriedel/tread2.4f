// CharListBox.cpp : implementation file
//

#include "stdafx.h"
#include "GdfEdit2.h"
#include "CharListBox.h"
#include "PopupDialog.h"
#include "GdfEdit2View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCharListBox

CCharListBox::CCharListBox()
{
}

CCharListBox::~CCharListBox()
{
}


BEGIN_MESSAGE_MAP(CCharListBox, CListBox)
	//{{AFX_MSG_MAP(CCharListBox)
	ON_WM_CREATE()
	ON_WM_CHAR()
	ON_WM_SYSKEYDOWN()
	ON_WM_LBUTTONDBLCLK()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCharListBox message handlers

int CCharListBox::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CListBox::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	m_ftText.CreateFont(10, 0, 0, 0, 200,
						0, 0, 0, DEFAULT_CHARSET,
						OUT_CHARACTER_PRECIS,
						CLIP_CHARACTER_PRECIS,
						DEFAULT_QUALITY,
						DEFAULT_PITCH | FF_MODERN,
						"System");

	m_ftBoldText.CreateFont(16, 0, 0, 0, FW_BOLD,
						0, 0, 0, DEFAULT_CHARSET,
						OUT_CHARACTER_PRECIS,
						CLIP_CHARACTER_PRECIS,
						DEFAULT_QUALITY,
						DEFAULT_PITCH | FF_MODERN,
						"System");

	return 0;
}

void CCharListBox::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	CDC* pDC;
	int x, y;
	CString sBuffer, sLeft, sRight;
	sBuffer = *(CString*)lpDrawItemStruct->itemData;
	

	x = lpDrawItemStruct->rcItem.left + 2;
	y = lpDrawItemStruct->rcItem.top + 1;

	pDC = CDC::FromHandle(lpDrawItemStruct->hDC);	//Device context

	if(lpDrawItemStruct->itemAction & ODA_FOCUS)
	{
		// Focus rect
		if(lpDrawItemStruct->itemState & ODS_FOCUS)
		{
			int nIndex = IndexOfMatch(sBuffer, m_sMatch);
			int nLength = sBuffer.GetLength();

			sLeft = sBuffer.Left(nIndex);
			CSize size = pDC->GetTextExtent(sBuffer, nIndex);
			sRight = sBuffer.Right(nLength - nIndex);

			pDC->FillSolidRect(&lpDrawItemStruct->rcItem, RGB(230, 230, 230));

			pDC->SetTextColor(RGB(0,0,210));
			pDC->TextOut(x,y, sLeft);

			pDC->SetTextColor(RGB(120,120,120));
			pDC->TextOut(size.cx + x,y, sRight);
		}
		else
		{
			pDC->FillSolidRect(&lpDrawItemStruct->rcItem, RGB(255, 255, 255));
			pDC->SetTextColor(RGB(150,150,150));
			pDC->TextOut(x,y, sBuffer);
		}
	}
	else
	{
		pDC->FillSolidRect(&lpDrawItemStruct->rcItem, RGB(255, 255, 255));
		pDC->SetTextColor(RGB(150,150,150));
		pDC->TextOut(x,y, sBuffer);
	}
}

int CCharListBox::CompareItem(LPCOMPAREITEMSTRUCT lpCompareItemStruct) 
{
	int i;
	CString* pString1, *pString2;

	pString1 = (CString*)lpCompareItemStruct->itemData1;
	pString2 = (CString*)lpCompareItemStruct->itemData2;

	if((pString1 == NULL) || (pString2 == NULL))
		return -1;

	i = pString1->Compare(*pString2);
	(i < 0) ? i = -1 : (i > 0) ? i = 1 : i = 0;

	return i;
	
	return 0;
}

void CCharListBox::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
{
	lpMeasureItemStruct->itemWidth = 100;
	lpMeasureItemStruct->itemHeight = 14;
}

void CCharListBox::DeleteItem(LPDELETEITEMSTRUCT lpDeleteItemStruct) 
{
	CString* pString = (CString*)lpDeleteItemStruct->itemData;
	if(pString != NULL)
		delete pString;
	
	CListBox::DeleteItem(lpDeleteItemStruct);
}

CPopupDialog* CCharListBox::GetParentWindow()
{
	return m_pDialog;
}

void CCharListBox::SetParentWindow(CPopupDialog* pDialog)
{
	m_pDialog = pDialog;
}

void CCharListBox::SetUserString(CString sMatch)
{
	m_sMatch = sMatch;
	SelectString(-1, m_sMatch);
}

void CCharListBox::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	CListBox::OnChar(nChar, nRepCnt, nFlags);

	int nLength;

	switch(nChar)
	{
	case VK_BACK:
		nLength = m_sMatch.GetLength();
		if(nLength > 1)
		{
			m_sMatch.Delete(nLength-1, 1);
			SelectString(-1, m_sMatch);
			GetParentWindow()->GetParentWindow()->RemoveTextAndMove(1);
		}
		else
		{
			GetParentWindow()->GetParentWindow()->RemoveTextAndMove(1);
			GetParentWindow()->ShowWindow(false);
		}
		break;
	case VK_DELETE:
		break;
	case VK_SPACE:
		GetParentWindow()->GetParentWindow()->InsertTextAndMove(nChar);
		GetParentWindow()->ShowWindow(false);
		break;
	default:
		if((nChar > 33) && (nChar < 125))
		{
			m_sMatch += nChar;
			SelectString(-1, m_sMatch);
			GetParentWindow()->GetParentWindow()->InsertTextAndMove(nChar);

			int nIndex = FindStringExact(-1, m_sMatch);
			if(nIndex != -1)
				GetParentWindow()->ShowWindow(false);
		}
	}
}

int CCharListBox::IndexOfMatch(CString &sParam1, CString &sParam2)
{
	int nLength, nLength2, n;
	nLength = sParam1.GetLength();
	nLength2 = sParam2.GetLength();

	for(n = 0; (n < nLength) && (n < nLength2); n++)
	{
		if(sParam1.GetAt(n) == sParam2.GetAt(n))
			continue;
		else
			break;
	}

	return n;
}

void CCharListBox::OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	MessageBeep(MB_OK);
	
	CListBox::OnSysKeyDown(nChar, nRepCnt, nFlags);
}

void CCharListBox::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	CListBox::OnLButtonDblClk(nFlags, point);

	int nLength = m_sMatch.GetLength();
	GetParentWindow()->GetParentWindow()->RemoveText(nLength);

	int nItem = GetCurSel();
	if(nItem == -1)
		return;

	CString sBuffer;
	GetText(nItem, sBuffer);
	GetParentWindow()->GetParentWindow()->InsertText(sBuffer);

	GetParentWindow()->ShowWindow(false);
}

void CCharListBox::CompleteText()
{
	int nLength = m_sMatch.GetLength();
	GetParentWindow()->GetParentWindow()->RemoveText(nLength);

	int nItem = GetCurSel();
	if(nItem == -1)
		return;

	CString sBuffer;
	GetText(nItem, sBuffer);
	GetParentWindow()->GetParentWindow()->InsertText(sBuffer);

	GetParentWindow()->ShowWindow(false);
}
