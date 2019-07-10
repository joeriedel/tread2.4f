// GDIFACERENDERER.CPP
// Copyright © 1999 Joe Riedel, Nick Randal.
// Author: Joe Riedel
#include "stdafx.h"
#include "RenderAPI.h"
#include "GDIWindowRenderer.h"
#include "GDIFaceRenderer.h"
#include "ColorTable.h"
#include "Face.h"
#include "MapView.h"

CGDIFaceRenderer::CGDIFaceRenderer(CGDIWindowRenderer* pWR) 
{ m_pWR = pWR; }
CGDIFaceRenderer::~CGDIFaceRenderer() 
{}

void CGDIFaceRenderer::RenderFace(CMapView* pView, CDC* pDC, CFace* pFace, int nDrawFlags, int nFlags)
{
	CPen* pOldPen;
	vec_t* pA, *pE;
	int nX1, nY1, nX2, nY2;


	if(nDrawFlags & RF_SELECTED)
		pOldPen = pDC->SelectObject(GetColorTable()->GetPen(CL_LINE_SEL));
	else if(nDrawFlags & RF_SPECIAL_A)	// Draw owned.
		pOldPen = pDC->SelectObject(GetColorTable()->GetPen(CL_OWNED));
	else
		pOldPen = pDC->SelectObject(GetColorTable()->GetPen(CL_LINE));

	// Draw the points.
	pA = pFace->m_vPoints[0];
	pE = pFace->m_vPoints[pFace->m_nNumPoints-1];
		
	// Get first set.
	nX2 = (int)MP_TO_WX(pE, pView);
	nY2 = (int)MP_TO_WY(pE, pView);

	for(;;)
	{
		nX1 = (int)MP_TO_WX(pA, pView);
		nY1 = (int)MP_TO_WY(pA, pView);

		pDC->MoveTo(nX1, nY1);
		pDC->LineTo(nX2, nY2);

		nX2 = nX1; nY2 = nY1;
		pA+=3;

		if(pA > pE)
			break;
	}

	pDC->SelectObject(pOldPen);
}

