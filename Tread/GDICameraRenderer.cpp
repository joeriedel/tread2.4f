// GDICAMERARENDERER.CPP
// Copyright © 1999 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#include "stdafx.h"
#include "RenderAPI.h"
#include "MapView.h"
#include "GDICameraRenderer.h"
#include "Camera.h"
#include "ColorTable.h"

#define CAMERA_SIZE		3

CGDICameraRenderer::CGDICameraRenderer()
{
}

CGDICameraRenderer::~CGDICameraRenderer()
{
}

void CGDICameraRenderer::RenderCamera(CMapView* pView, CDC* pDC, CCamera* pCamera, int nDrawFlags, int nFlags)
{
	int nX1, nX2, nY1, nY2;
	CBrush* pOldBrush;
	CPen* pOldPen;

	if(nDrawFlags&RF_SELECTED)
	{
		pOldPen = pDC->SelectObject(GetColorTable()->GetPen(CL_CAMERA_SEL));
		pOldBrush = pDC->SelectObject(GetColorTable()->GetBrush(CL_CAMERA_SEL));
	}
	else
	{
		pOldPen = pDC->SelectObject(GetColorTable()->GetPen(CL_CAMERA_LINE));
		pOldBrush = pDC->SelectObject(GetColorTable()->GetBrush(CL_CAMERA_BASE));
	}

	nX1 = (int)MP_TO_WX(pCamera->GetLook(), pView);
	nY1 = (int)MP_TO_WY(pCamera->GetLook(), pView);
	nX2 = (int)MP_TO_WX(pCamera->GetOrigin(), pView);
	nY2 = (int)MP_TO_WY(pCamera->GetOrigin(), pView);

	pDC->MoveTo(nX1, nY1);
	pDC->LineTo(nX2, nY2);

	// Draw the base.
	pDC->Ellipse(nX2-CAMERA_SIZE, nY2-CAMERA_SIZE, nX2+CAMERA_SIZE, nY2+CAMERA_SIZE);

	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldBrush);
}
