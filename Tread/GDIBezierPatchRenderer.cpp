// GDIBEZIERPATCHRENDERER.CPP
// Copyright © 1999 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#include "stdafx.h"
#include "RenderAPI.h"
#include "MapView.h"
#include "GDIWindingRenderer.h"
#include "GDIBezierPatchRenderer.h"
#include "Face.h"
#include "BezierPatch.h"

CGDIBezierPatchRenderer::CGDIBezierPatchRenderer(CGDIWindowRenderer* pWR)
{
	m_pFR = new CGDIWindingRenderer(pWR);
}

CGDIBezierPatchRenderer::~CGDIBezierPatchRenderer()
{
	if(m_pFR != NULL)
		delete m_pFR;
	m_pFR = NULL;
}

void CGDIBezierPatchRenderer::RenderBezierPatch(CMapView* pView, CDC* pDC, CBezierPatch* pPatch, int nDrawFlags, int nFlags)
{
	CFace* pFace;
	CBaseObject* pParent;

	if(pPatch->m_pGroup != NULL)
		pParent = pPatch->m_pGroup;
	else
		pParent = pPatch;

	pPatch->m_pFaceList->First();
	for(pFace = pPatch->m_pFaceList->GetCurItem(); pFace; pFace = pPatch->m_pFaceList->GetNextItem())
		m_pFR->RenderWinding(pView, pDC, pFace, pParent, nDrawFlags, nFlags);

}
