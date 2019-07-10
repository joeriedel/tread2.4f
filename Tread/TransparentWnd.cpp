//********************************************************************************
//* TransparentWindow.CPP
//*
//* A transparent window class.
//*
//* Based on the idea of Jason Wylie
//*
//* (C) 1998 by Franz Polzer
//*
//* Visit me at:	stud1.tuwien.ac.at/~e9225140
//* Write to me:	e9225140@student.tuwien.ac.at
//********************************************************************************

#include "stdafx.h"
#include "TransparentWnd.h"

#include <assert.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//********************************************************************************
//* Constructor
//********************************************************************************

CTransparentWnd::CTransparentWnd()
{
}


//********************************************************************************
//* Destructor
//********************************************************************************

CTransparentWnd::~CTransparentWnd()
{
}


BEGIN_MESSAGE_MAP(CTransparentWnd, CWnd)
	//{{AFX_MSG_MAP(CTransparentWnd)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


//********************************************************************************
//* CreateTransparent()
//*
//* Creates the main application window transparent
//********************************************************************************

void CTransparentWnd::CreateTransparent(LPCTSTR pTitle, RECT &rect, unsigned short MaskID, unsigned short BitmapID)
{
	CreateEx(	/*WS_EX_TOPMOST*/0,
						AfxRegisterWndClass(0),
						pTitle,
						WS_POPUP,
						rect,
						NULL,
						NULL,
						NULL );
	SetupRegion(GetWindowDC(), MaskID);
	m_BitmapID = BitmapID;
}


//********************************************************************************
//* SetupRegion()
//*
//* Set the Window Region for transparancy outside the mask region
//********************************************************************************

void CTransparentWnd::SetupRegion(CDC *pDC, unsigned short MaskID)
{
	CDC					memDC;
	CBitmap			cBitmap;
	CBitmap*		pOldMemBmp = NULL;
	COLORREF		col;
	CRect				cRect;
	int					x, y;
	CRgn				wndRgn, rgnTemp;

	GetWindowRect(&cRect);

	cBitmap.LoadBitmap(MaskID);
	memDC.CreateCompatibleDC(pDC);
	pOldMemBmp = memDC.SelectObject(&cBitmap);

	wndRgn.CreateRectRgn(0, 0, cRect.Width(), cRect.Height());
	for(x=0; x<=cRect.Width(); x++)
	{
		for(y=0; y<=cRect.Height(); y++)
		{
			col = memDC.GetPixel(x, y);
			if(col == 0)
			{
				rgnTemp.CreateRectRgn(x, y, x+1, y+1);
				wndRgn.CombineRgn(&wndRgn, &rgnTemp, RGN_XOR);
				rgnTemp.DeleteObject();	
			}
		}
	}
	if (pOldMemBmp) memDC.SelectObject(pOldMemBmp);
	SetWindowRgn((HRGN)wndRgn, TRUE);
}


//********************************************************************************
//* CTransparentWnd message handlers
//********************************************************************************

void CTransparentWnd::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CWnd::OnLButtonDown(nFlags, point);
	PostMessage(WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x,point.y));
}

BOOL CTransparentWnd::OnEraseBkgnd(CDC* pDC) 
{
	CRect	rect;
	GetWindowRect(&rect);

	CDC memDC;
	CBitmap			cBitmap;
	CBitmap*		pOldMemBmp = NULL;

	cBitmap.LoadBitmap(m_BitmapID);
	memDC.CreateCompatibleDC(pDC);
	pOldMemBmp = memDC.SelectObject(&cBitmap);
	pDC->BitBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, SRCCOPY);

	if (pOldMemBmp) memDC.SelectObject( pOldMemBmp );
	
	return TRUE;
}

void CTransparentWnd::OnPaint()
{
	CPaintDC dc(this);

	// Add your drawing code here!
}

