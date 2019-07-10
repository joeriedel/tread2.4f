// GL3DBEZIERPATCHRENDERER.CPP
// Copyright © 1999 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#include "stdafx.h"
#include "BezierPatch.h"
#include "GLRenderPackage.h"
#include "GL3DWindowRenderer.h"
#include "GL3DFaceRenderer.h"
#include "GL3DBezierPatchRenderer.h"

CGL3DBezierPatchRenderer::CGL3DBezierPatchRenderer(CGL3DWindowRenderer* pWR, WOGLFUNCTIONTABLE* pFT)
{
	m_pFR = new CGL3DFaceRenderer(pWR, pFT);
}

CGL3DBezierPatchRenderer::~CGL3DBezierPatchRenderer()
{
	delete m_pFR;
}

void CGL3DBezierPatchRenderer::RenderBezierPatch(CBezierPatch* pCurve, int nDrawFlags, int nFlags)
{
	CFace* pFace;

	pCurve->m_pFaceList->First();
	for(pFace = pCurve->m_pFaceList->GetCurItem(); pFace != NULL; pFace = pCurve->m_pFaceList->GetNextItem())
		m_pFR->RenderFace(pFace, pCurve, nDrawFlags, nFlags);
}
