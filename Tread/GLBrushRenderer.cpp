// GLBRUSHRENDERER.CPP
// Copyright © 1999 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#include "stdafx.h"
#include "GLRenderPackage.h"
#include "GLWindowRenderer.h"
#include "GLFaceRenderer.h"
#include "GLBrushRenderer.h"
#include "MapView.h"


CGLBrushRenderer::CGLBrushRenderer(CGLWindowRenderer* pWR, WOGLFUNCTIONTABLE* pFT)
{
	m_pFR = new CGLFaceRenderer(pWR, pFT);
}

CGLBrushRenderer::~CGLBrushRenderer()
{
	delete m_pFR;
}

void CGLBrushRenderer::RenderBrush(CQBrush* pBrush, CMapView* pView, int nDrawFlags, int nFlags)
{
	CFace* pFace;

	pBrush->m_pFaceList->First();
	for(pFace = pBrush->m_pFaceList->GetCurItem(); pFace != NULL; pFace = pBrush->m_pFaceList->GetNextItem())
		m_pFR->RenderFace(pFace, pBrush, pView, nDrawFlags, nFlags);
}

