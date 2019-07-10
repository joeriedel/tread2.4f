// GLBEZIERPATCHRENDERER.CPP
// Copyright © 1999 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#include "stdafx.h"
#include "BezierPatch.h"
#include "GLRenderPackage.h"
#include "GLWindowRenderer.h"
#include "GLFaceRenderer.h"
#include "GLBezierPatchRenderer.h"
#include "MapView.h"

CGLBezierPatchRenderer::CGLBezierPatchRenderer(CGLWindowRenderer* pWR, WOGLFUNCTIONTABLE* pFT)
{
	m_pFR = new CGLFaceRenderer(pWR, pFT);
}

CGLBezierPatchRenderer::~CGLBezierPatchRenderer()
{
	delete m_pFR;
}

void CGLBezierPatchRenderer::RenderBezierPatch(CBezierPatch* pCurve, CMapView* pView, int nDrawFlags, int nFlags)
{
	CFace* pFace;

	pCurve->m_pFaceList->First();
	for(pFace = pCurve->m_pFaceList->GetCurItem(); pFace != NULL; pFace = pCurve->m_pFaceList->GetNextItem())
		m_pFR->RenderFace(pFace, pCurve, pView, nDrawFlags, nFlags);
}
