// FilterEdit.cpp : implementation file
//

#include "stdafx.h"
#include "tread3d.h"
#include "FilterEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFilterEdit

CFilterEdit::CFilterEdit()
{
	m_nFilter = 0;
}

CFilterEdit::~CFilterEdit()
{
}


BEGIN_MESSAGE_MAP(CFilterEdit, CEdit)
	//{{AFX_MSG_MAP(CFilterEdit)
	ON_WM_CHAR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFilterEdit message handlers

void CFilterEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	
	if(m_nFilter & FILTER_CHAR)
	{
		if((nChar >= 65) && (nChar <= 90))
			return;
		if((nChar >= 97) && (nChar <= 122))
			return;
	}

	if(m_nFilter & FILTER_NUMBER)
	{
		if((nChar >= 48) && (nChar <= 57))
			return;
	}

	if(m_nFilter & FILTER_SPACE)
		if(nChar == 32)
			return;

	if(m_nFilter & FILTER_DASH)
		if(nChar == 45)
			return;

	if(m_nFilter & FILTER_PUNCTUATION)
	{
		if((nChar >= 33) && (nChar <= 44))
			return;
		if((nChar >= 58) && (nChar <= 64))
			return;
		if((nChar >= 91) && (nChar <= 96))
			return;
		switch(nChar)
		{
		case 46:
		case 47:
			return;
		}
	}

	if(m_nFilter & FILTER_BACKSPACE)
	{
		if(nChar == VK_BACK)
			return;
	}

	CEdit::OnChar(nChar, nRepCnt, nFlags);
}

void CFilterEdit::SetFilter(int nFilter)
{
	m_nFilter = nFilter;
}
