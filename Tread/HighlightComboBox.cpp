// HighlightComboBox.cpp : implementation file
//

#include "stdafx.h"
#include "tread3d.h"
#include "HighlightComboBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHighlightComboBox

CHighlightComboBox::CHighlightComboBox()
{
	m_bFound = false;
}

CHighlightComboBox::~CHighlightComboBox()
{
}


BEGIN_MESSAGE_MAP(CHighlightComboBox, CComboBox)
	//{{AFX_MSG_MAP(CHighlightComboBox)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHighlightComboBox message handlers

void CHighlightComboBox::SetHighlightColor(COLORREF clHighlight)
{
	m_clHighlight = clHighlight;
}

void CHighlightComboBox::SetHighlight(bool bHighlight)
{
	m_bFound = bHighlight;
	Invalidate();
}

HBRUSH CHighlightComboBox::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CComboBox::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	if(m_bFound)
		pDC->SetTextColor(m_clHighlight);
	else
		pDC->SetTextColor(0);
	
	// TODO: Return a different brush if the default is not desired
	return hbr;
}
