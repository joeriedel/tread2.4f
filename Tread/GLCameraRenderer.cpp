// GLCAMERARENDERER.CPP
// Copyright © 1999 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#include "stdafx.h"
#include "Camera.h"
#include "GLRenderPackage.h"
#include "GLWindowRenderer.h"
#include "GLCameraRenderer.h"
#include "TreadSys.h"
#include "ColorTable.h"
#include "RenderAPI.h"
#include "MapView.h"

#define CAMERA_SIZE		3

CGLCameraRenderer::CGLCameraRenderer(CGLWindowRenderer* pWR, WOGLFUNCTIONTABLE* pGLFT)
{
	m_pWR = pWR;
	m_pGLFT = pGLFT;
}

CGLCameraRenderer::~CGLCameraRenderer()
{
}

void CGLCameraRenderer::RenderCamera(CCamera* pCamera, CMapView* pView, int nDrawFlags, int nFlags)
{
	int nX[2], nY[2];


	if(nDrawFlags&RF_SELECTED)
		m_pGLFT->glColor3fv(GetColorTable()->GetFloatV(CL_CAMERA_SEL));
	else
		m_pGLFT->glColor3fv(GetColorTable()->GetFloatV(CL_CAMERA_LINE));

	m_pGLFT->glBegin(GL_LINES);
	m_pGLFT->glVertex2i(pCamera->m_vOrigin[VI(VEC_X, pView)], pCamera->m_vOrigin[VI(VEC_Y, pView)]);
	m_pGLFT->glVertex2i(pCamera->m_vLook[VI(VEC_X, pView)], pCamera->m_vLook[VI(VEC_Y, pView)]);
	m_pGLFT->glEnd();

	nX[0] = WX_TO_MX(MP_TO_WX(pCamera->m_vOrigin, pView) - CAMERA_SIZE, pView);
	nX[1] = WX_TO_MX(MP_TO_WX(pCamera->m_vOrigin, pView) + CAMERA_SIZE, pView);
	nY[0] = WY_TO_MY(MP_TO_WY(pCamera->m_vOrigin, pView) - CAMERA_SIZE, pView);
	nY[1] = WY_TO_MY(MP_TO_WY(pCamera->m_vOrigin, pView) + CAMERA_SIZE, pView);

	// Draw a filled polygon as the base.
	if((nDrawFlags&RF_SELECTED)==0)
		m_pGLFT->glColor3fv(GetColorTable()->GetFloatV(CL_CAMERA_BASE));
	m_pGLFT->glBegin(GL_QUADS);
	m_pGLFT->glVertex2i(nX[0], nY[0]);
	m_pGLFT->glVertex2i(nX[1], nY[0]);
	m_pGLFT->glVertex2i(nX[1], nY[1]);
	m_pGLFT->glVertex2i(nX[0], nY[1]);
	m_pGLFT->glEnd();
}
