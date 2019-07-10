// GLCAMERARENDERER.H
// Copyright © 1999 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#ifndef GLANCHORRENDERER_H
#define GLANCHORRENDERER_H

#include "Anchor.h"
#include "GLRenderPackage.h"

class CGLWindowRenderer;

class CGLAnchorRenderer
{
private:

	CGLWindowRenderer* m_pWR;
	WOGLFUNCTIONTABLE* m_pGLFT;

public:

	CGLAnchorRenderer(CGLWindowRenderer* pWR, WOGLFUNCTIONTABLE* pGLFT);
	~CGLAnchorRenderer();

	void RenderAnchor(CAnchor* pAnchor, CMapView* pView, int nDrawFlags, int nFlags=0);

};

#endif