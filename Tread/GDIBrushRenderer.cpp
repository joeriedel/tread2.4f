// GDIBRUSHRENDERER.CPP
// Copyright © 1999 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#include "stdafx.h"
#include "RenderAPI.h"
#include "MapView.h"
#include "GDIWindingRenderer.h"
#include "GDIBrushRenderer.h"
#include "Face.h"
#include "Brush.h"

CGDIBrushRenderer::CGDIBrushRenderer(CGDIWindowRenderer* pWR)
{
	m_pFR = new CGDIWindingRenderer(pWR);
}

CGDIBrushRenderer::~CGDIBrushRenderer()
{
	if(m_pFR != NULL)
		delete m_pFR;
	m_pFR = NULL;
}

void CGDIBrushRenderer::RenderBrush(CMapView* pView, CDC* pDC, CQBrush* pBrush, int nDrawFlags, int nFlags)
{
	CFace* pFace;

	pBrush->m_pFaceList->First();
	for(pFace = pBrush->m_pFaceList->GetCurItem(); pFace; pFace = pBrush->m_pFaceList->GetNextItem())
		m_pFR->RenderWinding(pView, pDC, pFace, pBrush, nDrawFlags, nFlags);

}
