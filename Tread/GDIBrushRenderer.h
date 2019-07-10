// GDIBRUSHRENDERER.H
// Copyright © 1999 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#ifndef GDIBRUSHRENDERER_H
#define GDIBRUSHRENDERER_H

#include "Brush.h"

class CGDIWindingRenderer;
class CGDIWindowRenderer;
class CGDIBrushRenderer
{
	CGDIWindingRenderer* m_pFR;

public:
	
	CGDIBrushRenderer(CGDIWindowRenderer* pWR);
	~CGDIBrushRenderer();
	
	void RenderBrush(CMapView* pView, CDC* pDC, CQBrush* pBrush, int nDrawFlags, int nFlags);
};

#endif