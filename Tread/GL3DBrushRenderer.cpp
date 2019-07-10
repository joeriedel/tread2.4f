// GL3DBRUSHRENDERER.CPP
// Copyright © 1999 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#include "stdafx.h"
#include "GLRenderPackage.h"
#include "GL3DWindowRenderer.h"
#include "GL3DFaceRenderer.h"
#include "GL3DBrushRenderer.h"
#include "ColorTable.h"

CGL3DBrushRenderer::CGL3DBrushRenderer(CGL3DWindowRenderer* pWR, WOGLFUNCTIONTABLE* pFT)
{
	m_pGLFT = pFT;
	m_pFR = new CGL3DFaceRenderer(pWR, pFT);
	m_pWR = pWR;
}

CGL3DBrushRenderer::~CGL3DBrushRenderer()
{
	delete m_pFR;
}

void CGL3DBrushRenderer::RenderFaceOutLine(CFace* pFace, int nDrawFlags, int nFlags)
{
	int i;
	vec_t* vV;

	m_pGLFT->glBegin(GL_LINE_LOOP);
	for(i = 0; i < pFace->m_nNumPoints; i++)
	{
		vV = pFace->m_vPoints[i];
		m_pGLFT->glVertex3fv(vV);
	}
	m_pGLFT->glEnd();
}

void CGL3DBrushRenderer::RenderBrush(CQBrush* pBrush, int nDrawFlags, int nFlags)
{
	CFace* pFace;

	// Do nothing in line & pick.
	if((nDrawFlags & (RF_LINE|RF_PICKMODE))==(RF_LINE|RF_PICKMODE))
		return;

	pBrush->m_pFaceList->First();
	for(pFace = pBrush->m_pFaceList->GetCurItem(); pFace != NULL; pFace = pBrush->m_pFaceList->GetNextItem())
		m_pFR->RenderFace(pFace, NULL, nDrawFlags|RF_NO3DSELOUTLINE, nFlags);

	if((nDrawFlags&RF_SELECTED)==RF_SELECTED && (nDrawFlags&(RF_NO3DSELOUTLINE|RF_PICKMODE))==0)
	{
		m_pGLFT->glDisable(GL_TEXTURE_2D);
		m_pGLFT->glDepthFunc(GL_ALWAYS);
		m_pGLFT->glColor3fv(GetColorTable()->GetFloatV(CL_HOTSPOT_BOX));

		pBrush->m_pFaceList->First();
		for(pFace = pBrush->m_pFaceList->GetCurItem(); pFace != NULL; pFace = pBrush->m_pFaceList->GetNextItem())
			RenderFaceOutLine(pFace, nDrawFlags, nFlags);

		//((CGL3DWindowRenderer*)m_pWR)->SetBias(((CGL3DWindowRenderer*)m_pWR)->GetBias(), 0);
		//m_pGLFT->glColor4f(1.0F, 1.0F, 1.0F, GL3D_BASE_ALPHA);
		m_pGLFT->glDepthFunc(GL_LESS);
		if(nDrawFlags&RF_TEXTURED)
			m_pGLFT->glEnable(GL_TEXTURE_2D);
	}
}

