// GL3DBRUSHRENDERER.CPP
// Copyright © 1999 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#include "stdafx.h"
#include "../TreadAPI.h"
#include "GL3DFaceRenderer.h"
#include "GL3DBrushRenderer.h"

CGL3DBrushRenderer::CGL3DBrushRenderer()
{
}

CGL3DBrushRenderer::~CGL3DBrushRenderer()
{
}

void CGL3DBrushRenderer::RenderFaceOutLine(CFace* pFace, int nDrawFlags)
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

void CGL3DBrushRenderer::RenderBrush(CQBrush* pBrush, int nDrawFlags, bool bDrawLight)
{
	CFace* pFace;

	// Do nothing in line & pick.
	if((nDrawFlags & (RF_LINE|RF_PICKMODE))==(RF_LINE|RF_PICKMODE))
		return;

	pBrush->m_pFaceList->First();
	for(pFace = pBrush->m_pFaceList->GetCurItem(); pFace != NULL; pFace = pBrush->m_pFaceList->GetNextItem())
		m_FR.RenderLitFace((CLitFace*)pFace, NULL, nDrawFlags|RF_NO3DSELOUTLINE, bDrawLight, m_pWR, m_pGLFT);

	if((nDrawFlags&RF_SELECTED)==RF_SELECTED && (nDrawFlags&(RF_NO3DSELOUTLINE|RF_PICKMODE))==0)
	{
		m_pGLFT->glDisable(GL_TEXTURE_2D);
		m_pGLFT->glDepthFunc(GL_ALWAYS);
		m_pGLFT->glColor3fv(GetColorTable()->GetFloatV(CL_HOTSPOT_BOX));

		pBrush->m_pFaceList->First();
		for(pFace = pBrush->m_pFaceList->GetCurItem(); pFace != NULL; pFace = pBrush->m_pFaceList->GetNextItem())
			RenderFaceOutLine(pFace, nDrawFlags);

		m_pGLFT->glDepthFunc(GL_LESS);
		if(nDrawFlags&RF_TEXTURED)
			m_pGLFT->glEnable(GL_TEXTURE_2D);
	}
}

