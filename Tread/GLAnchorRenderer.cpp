// GLCAMERARENDERER.CPP
// Copyright © 1999 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#include "stdafx.h"
#include "Anchor.h"
#include "GLRenderPackage.h"
#include "GLWindowRenderer.h"
#include "GLAnchorRenderer.h"
#include "TreadSys.h"
#include "ColorTable.h"
#include "RenderAPI.h"
#include "MapView.h"

#define ANCHOR_SIZE		4
#define ANCHOR_SIZE2	8
#define ANCHOR_LINE		10

CGLAnchorRenderer::CGLAnchorRenderer(CGLWindowRenderer* pWR, WOGLFUNCTIONTABLE* pGLFT)
{
	m_pWR = pWR;
	m_pGLFT = pGLFT;
}

CGLAnchorRenderer::~CGLAnchorRenderer()
{
}

void CGLAnchorRenderer::RenderAnchor(CAnchor* pAnchor, CMapView* pView, int nDrawFlags, int nFlags)
{
	int nXO, nYO;
	int nX[2], nY[2];
	

	/*if(nDrawFlags&RF_SELECTED)
		m_pGLFT->glColor3fv(GetColorTable()->GetFloatV(CL_ANCHOR_SEL));
	else*/

	nXO = (int)MP_TO_WX(pAnchor->GetOrigin(), pView);
	nYO = (int)MP_TO_WY(pAnchor->GetOrigin(), pView);

	m_pGLFT->glColor3fv(GetColorTable()->GetFloatV(CL_ANCHOR2));

	nX[0] = WX_TO_MX(MP_TO_WX(pAnchor->m_vOrigin, pView) - ANCHOR_SIZE, pView);
	nX[1] = WX_TO_MX(MP_TO_WX(pAnchor->m_vOrigin, pView) + ANCHOR_SIZE, pView);
	nY[0] = WY_TO_MY(MP_TO_WY(pAnchor->m_vOrigin, pView) - ANCHOR_SIZE, pView);
	nY[1] = WY_TO_MY(MP_TO_WY(pAnchor->m_vOrigin, pView) + ANCHOR_SIZE, pView);

	m_pGLFT->glBegin(GL_LINE_LOOP);
	m_pGLFT->glVertex2i(nX[0], nY[0]);
	m_pGLFT->glVertex2i(nX[1], nY[0]);
	m_pGLFT->glVertex2i(nX[1], nY[1]);
	m_pGLFT->glVertex2i(nX[0], nY[1]);
	m_pGLFT->glEnd();

	nX[0] = WX_TO_MX(MP_TO_WX(pAnchor->m_vOrigin, pView) - ANCHOR_SIZE2, pView);
	nX[1] = WX_TO_MX(MP_TO_WX(pAnchor->m_vOrigin, pView) + ANCHOR_SIZE2, pView);
	nY[0] = WY_TO_MY(MP_TO_WY(pAnchor->m_vOrigin, pView) - ANCHOR_SIZE2, pView);
	nY[1] = WY_TO_MY(MP_TO_WY(pAnchor->m_vOrigin, pView) + ANCHOR_SIZE2, pView);

	m_pGLFT->glBegin(GL_LINE_LOOP);
	m_pGLFT->glVertex2i(nX[0], nY[0]);
	m_pGLFT->glVertex2i(nX[1], nY[0]);
	m_pGLFT->glVertex2i(nX[1], nY[1]);
	m_pGLFT->glVertex2i(nX[0], nY[1]);
	m_pGLFT->glEnd();

	m_pGLFT->glColor3fv(GetColorTable()->GetFloatV(CL_ANCHOR));

	// Draw cross.
	m_pGLFT->glBegin(GL_LINES);
	m_pGLFT->glVertex2i(WX_TO_MX(nXO, pView), WY_TO_MY(nYO-ANCHOR_SIZE2, pView));
	m_pGLFT->glVertex2i(WX_TO_MX(nXO, pView), WY_TO_MY(nYO+ANCHOR_SIZE2, pView));
	m_pGLFT->glEnd();
	m_pGLFT->glBegin(GL_LINES);
	m_pGLFT->glVertex2i(WX_TO_MX(nXO-ANCHOR_SIZE2, pView), WY_TO_MY(nYO, pView));
	m_pGLFT->glVertex2i(WX_TO_MX(nXO+ANCHOR_SIZE2, pView), WY_TO_MY(nYO, pView));
	m_pGLFT->glEnd();

}
