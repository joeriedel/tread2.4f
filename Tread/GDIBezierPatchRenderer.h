// GDIBEZIERPATCHRENDERER.H
// Copyright © 1999 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#ifndef GDIBEZIERPATCHRENDERER_H
#define GDIBEZIERPATCHRENDERER_H

#include "BezierPatch.h"

class CGDIWindingRenderer;
class CGDIWindowRenderer;
class CGDIBezierPatchRenderer
{
	CGDIWindingRenderer* m_pFR;

public:
	
	CGDIBezierPatchRenderer(CGDIWindowRenderer* pWR);
	~CGDIBezierPatchRenderer();
	
	void RenderBezierPatch(CMapView* pView, CDC* pDC, CBezierPatch* pPatch, int nDrawFlags, int nFlags);
};

#endif