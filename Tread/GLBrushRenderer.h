// GLBRUSHRENDERER.H
// Copyright © 1999 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#ifndef GLBRUSHRENDERER_H
#define GLBRUSHRENDERER_H

#include "Brush.h"
#include "GLRenderPackage.h"

class CGLFaceRenderer;
class CGLWindowRenderer;

class CGLBrushRenderer
{
private:

	CGLFaceRenderer* m_pFR;

public:

	CGLBrushRenderer(CGLWindowRenderer* pWR, WOGLFUNCTIONTABLE* pFT);
	~CGLBrushRenderer();

	void RenderBrush(CQBrush* pBrush, CMapView* pView, int nDrawFlags, int nFlags=0);

};

#endif