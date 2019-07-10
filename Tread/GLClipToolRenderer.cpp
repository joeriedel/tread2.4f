// GLCLIPTOOLRENDERER.CPP
// Copyright © 1999 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#include "stdafx.h"
#include "GLRenderPackage.h"
#include "GLWindowRenderer.h"
#include "GLClipToolRenderer.h"
#include "MapView.h"
#include "ColorTable.h"
#include "GameAPI.h"
#include "TreadSys.h"
#include "Tread3D.h"

CGLClipToolRenderer::CGLClipToolRenderer(CGLWindowRenderer* pWR, WOGLFUNCTIONTABLE* pFT)
{
	m_pGLFT = pFT;
	m_pWR = pWR;
}

CGLClipToolRenderer::~CGLClipToolRenderer()
{
}

void CGLClipToolRenderer::RenderClipTool(CClipTool* pClipTool, CDC* pDC, CMapView* pView, CTread3DDoc* pDocument, int nDrawFlags, int nFlags)
{
	if(pClipTool->m_bValid == false)
		return;

	// Simply redraw the front objects over the currently drawn set.
	GetRenderAPI()->DrawObjectList(pView, pDocument, pDC, pView->GetWindowRenderer(), &pClipTool->m_BackList, nDrawFlags, 0);

	// Now draw the clip-line.
	m_pGLFT->glColor3fv(GetColorTable()->GetFloatV(CL_CLIP));

	m_pGLFT->glBegin(GL_LINES);

	m_pGLFT->glVertex2i((int)pClipTool->m_vPts[0][VI(VEC_X, pView)],
		(int)pClipTool->m_vPts[0][VI(VEC_Y, pView)]);
	m_pGLFT->glVertex2i((int)pClipTool->m_vPts[1][VI(VEC_X, pView)],
		(int)pClipTool->m_vPts[1][VI(VEC_Y, pView)]);

	m_pGLFT->glEnd();

	m_pGLFT->glColor3f(1.0F, 1.0F, 1.0F);
}
