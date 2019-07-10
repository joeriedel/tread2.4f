// GDICLIPTOOLRENDERER.CPP
// Copyright © 1999 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#include "stdafx.h"
#include "GDIWindowRenderer.h"
#include "GDIClipToolRenderer.h"
#include "RenderAPI.h"
#include "ColorTable.h"
#include "MapView.h"

CGDIClipToolRenderer::CGDIClipToolRenderer()
{
}

CGDIClipToolRenderer::~CGDIClipToolRenderer()
{
}

void CGDIClipToolRenderer::RenderClipTool(CMapView* pView, CDC* pDC, CTread3DDoc* pDocument, CClipTool* pClipTool, int nDrawFlags, int nFlags)
{
	if(pClipTool->m_bValid == false)
		return;

	// Simply redraw the front objects over the currently drawn set.
	GetRenderAPI()->DrawObjectList(pView, pDocument, pDC, pView->GetWindowRenderer(), &pClipTool->m_BackList, nDrawFlags, 0);

	// Now draw the clip-line.
	CPen* pOldPen = pDC->SelectObject(GetColorTable()->GetPen(CL_CLIP));

	pDC->MoveTo((int)MX_TO_WX(pClipTool->m_vPts[0][VI(VEC_X, pView)], pView),
		(int)MY_TO_WY(pClipTool->m_vPts[0][VI(VEC_Y, pView)], pView));
	pDC->LineTo((int)MX_TO_WX(pClipTool->m_vPts[1][VI(VEC_X, pView)], pView),
		(int)MY_TO_WY(pClipTool->m_vPts[1][VI(VEC_Y, pView)], pView));

	pDC->SelectObject(pOldPen);
}
