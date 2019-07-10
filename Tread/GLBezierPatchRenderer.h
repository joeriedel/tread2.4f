// GLBEZIERPATCHRENDERER.H
// Copyright © 1999 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#ifndef GLBEZIERPATCHRENDERER_H
#define GLBEZIERPATCHRENDERER_H

#include "BezierPatch.h"
#include "GLRenderPackage.h"

class CGLFaceRenderer;
class CGLWindowRenderer;

class CGLBezierPatchRenderer
{
private:

	CGLFaceRenderer* m_pFR;

public:

	CGLBezierPatchRenderer(CGLWindowRenderer* pWR, WOGLFUNCTIONTABLE* pFT);
	~CGLBezierPatchRenderer();

	void RenderBezierPatch(CBezierPatch* pPatch, CMapView* pView, int nDrawFlags, int nFlags=0);

};


#endif