// GLFACERENDERER.H
// Copyright © 1999 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#ifndef GLFACERENDERER_H
#define GLFACERENDERER_H

#include "Face.h"
#include "GLRenderPackage.h"

class CGLWindowRenderer;

class CGLFaceRenderer
{
private:

	CGLWindowRenderer* m_pWR;
	WOGLFUNCTIONTABLE* m_pGLFT;

public:

	CGLFaceRenderer(CGLWindowRenderer* pWR, WOGLFUNCTIONTABLE* pGLFT);
	~CGLFaceRenderer();

	void RenderFace(CFace* pFace, CBaseObject* pParent, CMapView* pView, int nDrawFlags, int nFlags=0);

};

#endif