// GLCLIPTOOLRENDERER.H
// Copyright © 1999 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#ifndef GLCLIPTOOLRENDERER_H
#define GLCLIPTOOLRENDERER_H

#include "ClipTool.h"
#include "GLRenderPackage.h"

class CGLWindowRenderer;

class CGLClipToolRenderer
{
private:

	CGLWindowRenderer* m_pWR;
	WOGLFUNCTIONTABLE* m_pGLFT;

public:

	CGLClipToolRenderer(CGLWindowRenderer* pWR, WOGLFUNCTIONTABLE* pFT);
	~CGLClipToolRenderer();

	void RenderClipTool(CClipTool* pClipTool, CDC* pDC, CMapView* pView, CTread3DDoc* pDocument, int nDrawFlags, int nFlags=0);

};

#endif