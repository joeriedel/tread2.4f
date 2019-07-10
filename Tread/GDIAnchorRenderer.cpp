// GDICAMERARENDERER.CPP
// Copyright © 1999 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#include "stdafx.h"
#include "RenderAPI.h"
#include "MapView.h"
#include "GDIAnchorRenderer.h"
#include "Anchor.h"
#include "ColorTable.h"

#define ANCHOR_SIZE		4
#define ANCHOR_SIZE2	8
#define ANCHOR_LINE		10

CGDIAnchorRenderer::CGDIAnchorRenderer()
{
}

CGDIAnchorRenderer::~CGDIAnchorRenderer()
{
}

void CGDIAnchorRenderer::RenderAnchor(CMapView* pView, CDC* pDC, CAnchor* pAnchor, int nDrawFlags, int nFlags)
{
	int nX, nY;
	CPen* pOldPen;
	CBrush* pOldBrush;
	CRect rcBox;
	CRect rcBox2;
	

	/*if(nDrawFlags&RF_SELECTED)
	{
		pOldPen = pDC->SelectObject(GetColorTable()->GetPen(CL_ANCHOR_SEL));
		pOldBrush = pDC->SelectObject(GetColorTable()->GetBrush(CL_ANCHOR_SEL));
	}
	else*/
	{
		pOldPen = pDC->SelectObject(GetColorTable()->GetPen(CL_ANCHOR2));
		pOldBrush = pDC->SelectObject(GetColorTable()->GetBrush(CL_ANCHOR));
	}

	nX = (int)MP_TO_WX(pAnchor->GetOrigin(), pView);
	nY = (int)MP_TO_WY(pAnchor->GetOrigin(), pView);

	rcBox.SetRect(nX - ANCHOR_SIZE, nY - ANCHOR_SIZE, nX + ANCHOR_SIZE, nY + ANCHOR_SIZE);
	rcBox2.SetRect(nX - ANCHOR_SIZE2, nY - ANCHOR_SIZE2, nX + ANCHOR_SIZE2, nY + ANCHOR_SIZE2);

	//pDC->DrawRect(rcBox);
	//pDC->DrawRect(rcBox2);

	// Draw the 4 sides.
	pDC->MoveTo(rcBox.left, rcBox.top);
	pDC->LineTo(rcBox.right, rcBox.top);
	pDC->LineTo(rcBox.right, rcBox.bottom);
	pDC->LineTo(rcBox.left, rcBox.bottom);
	pDC->LineTo(rcBox.left, rcBox.top);

	// Draw the 4 sides.
	pDC->MoveTo(rcBox2.left, rcBox2.top);
	pDC->LineTo(rcBox2.right, rcBox2.top);
	pDC->LineTo(rcBox2.right, rcBox2.bottom);
	pDC->LineTo(rcBox2.left, rcBox2.bottom);
	pDC->LineTo(rcBox2.left, rcBox2.top);

	pDC->SelectObject(GetColorTable()->GetPen(CL_ANCHOR));

	// Draw cross.
	pDC->MoveTo(nX - ANCHOR_LINE, nY);
	pDC->LineTo(nX + ANCHOR_LINE, nY);
	pDC->MoveTo(nX, nY - ANCHOR_LINE);
	pDC->LineTo(nX, nY + ANCHOR_LINE);


	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldBrush);
}
