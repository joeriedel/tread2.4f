// GDIWINDINGRENDERER.CPP
// Copyright © 1999 Joe Riedel, Nick Randal.
// Author: Joe Riedel
#include "stdafx.h"
#include "RenderAPI.h"
#include "GDIWindowRenderer.h"
#include "GDIWindingRenderer.h"
#include "ColorTable.h"
#include "Winding.h"
#include "MapView.h"
#include "TreadSys.h"

CGDIWindingRenderer::CGDIWindingRenderer(CGDIWindowRenderer* pWR) 
{ m_pWR = pWR; }
CGDIWindingRenderer::~CGDIWindingRenderer() 
{}

void CGDIWindingRenderer::RenderWinding(CMapView* pView, CDC* pDC, CWinding* pWinding, CBaseObject* pParent, int nDrawFlags, int nFlags)
{
	CPen* pOldPen;
	vec_t* pA, *pE, *pB;
	int nX1, nY1, nX2, nY2;
	GDIWINDOWLINE* pLine;
	bool bLog = (nDrawFlags&RF_FAST)==0;

	nDrawFlags |= pWinding->m_nObjectRenderFlags;

	if(nDrawFlags & RF_SELECTED)
		pOldPen = pDC->SelectObject(GetColorTable()->GetPen(CL_LINE_SEL));
	else 
	if(nDrawFlags & RF_OWNED)	// Draw owned.
		pOldPen = pDC->SelectObject(GetColorTable()->GetPen(CL_OWNED));
	else
		pOldPen = pDC->SelectObject(GetColorTable()->GetPen(CL_LINE));

	// Draw the points.
	pA = pWinding->m_vPoints[0];
	pB = pWinding->m_vPoints[pWinding->m_nNumPoints-1];
	pE = pB;
	
	// Get first set.
	nX2 = (int)MP_TO_WX(pE, pView);
	nY2 = (int)MP_TO_WY(pE, pView);

	//pDC->MoveTo(nX2, nY2);

	for(;;)
	{
		nX1 = (int)MP_TO_WX(pA, pView);
		nY1 = (int)MP_TO_WY(pA, pView);
		
		//if(!m_pWR->ClipLine(pView, pA, pB))
		{
			// Store.
			if(bLog)
			{
				pLine = m_pWR->GetWindowLine();
				pLine->m_fX1 = (float)nX1;
				pLine->m_fX2 = (float)nX2;
				pLine->m_fY1 = (float)nY1;
				pLine->m_fY2 = (float)nY2;
				pLine->m_bBuild = true;
				if(pParent != NULL)
					pLine->m_pObject = pParent;
				else
					pLine->m_pObject = pWinding;
			}

			pDC->MoveTo(nX1, nY1);
			pDC->LineTo(nX2, nY2);
		}

		nX2 = nX1; nY2 = nY1;
		pB = pA;
		pA+=3;
		
		if(pA > pE)
			break;
	}

	pDC->SelectObject(pOldPen);
}

