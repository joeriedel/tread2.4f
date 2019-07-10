// GLFACERENDERER.CPP
// Copyright © 1999 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#include "stdafx.h"
#include "Face.h"
#include "GLRenderPackage.h"
#include "GLWindowRenderer.h"
#include "GLFaceRenderer.h"
#include "TreadSys.h"
#include "ColorTable.h"
#include "RenderAPI.h"
#include "MapView.h"

CGLFaceRenderer::CGLFaceRenderer(CGLWindowRenderer* pWR, WOGLFUNCTIONTABLE* pGLFT)
{
	m_pWR = pWR;
	m_pGLFT = pGLFT;
}

CGLFaceRenderer::~CGLFaceRenderer()
{
}

void CGLFaceRenderer::RenderFace(CFace* pFace, CBaseObject* pParent, CMapView* pView, int nDrawFlags, int nFlags)
{
	int i;
	int nX1, nX2, nY1, nY2;
	vec_t* pA, *pB;
	bool bLog = (nDrawFlags&RF_FAST)==0;
	GDIWINDOWLINE* pLine;

	nDrawFlags |= pFace->m_nObjectRenderFlags;

	if(nDrawFlags&RF_SELECTED)
		m_pGLFT->glColor3fv(GetColorTable()->GetFloatV(CL_LINE_SEL));
	else
	if(nDrawFlags&RF_OWNED)
		m_pGLFT->glColor3fv(GetColorTable()->GetFloatV(CL_OWNED));
	else
		m_pGLFT->glColor3fv(GetColorTable()->GetFloatV(CL_LINE));


	pB = pFace->m_vPoints[pFace->m_nNumPoints-1];
	m_pGLFT->glBegin(GL_LINES);

	for(i = 0; i < pFace->m_nNumPoints; i++)
	{
		pA = pFace->m_vPoints[i];
		if(m_pWR->ClipLine(pView, pA, pB))
		{
			pB = pA;
			continue;
		}

		// Store.
		if(bLog)
		{
			nX1 = (int)MX_TO_WX(pA[VI(VEC_X, pView)], pView);
			nX2 = (int)MX_TO_WX(pB[VI(VEC_X, pView)], pView);
			nY1 = (int)MY_TO_WY(pA[VI(VEC_Y, pView)], pView);
			nY2 = (int)MY_TO_WY(pB[VI(VEC_Y, pView)], pView);

			pLine = m_pWR->GetWindowLine();
			pLine->m_fX1 = (float)nX1;
			pLine->m_fX2 = (float)nX2;
			pLine->m_fY1 = (float)nY1;
			pLine->m_fY2 = (float)nY2;
			pLine->m_nFlags = 0;
			pLine->m_bBuild = true;
			if(pParent != NULL)
				pLine->m_pObject = pParent;
			else
				pLine->m_pObject = pFace;
		}

		m_pGLFT->glVertex2i((int)pA[VI(VEC_X, pView)], (int)pA[VI(VEC_Y, pView)]);
		m_pGLFT->glVertex2i((int)pB[VI(VEC_X, pView)], (int)pB[VI(VEC_Y, pView)]);
		pB = pA;
	}

	m_pGLFT->glEnd();
		
}
