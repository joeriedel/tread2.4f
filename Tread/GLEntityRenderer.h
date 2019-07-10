// GLENTITYRENDERER.H
// Copyright © 1999 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#ifndef GLENTITYRENDERER_H
#define GLENTITYRENDERER_H

#include "Entity.h"
#include "GLRenderPackage.h"

class CGLFaceRenderer;
class CGLWindowRenderer;

class CGLEntityRenderer
{
private:

	CGLWindowRenderer* m_pWR;
	WOGLFUNCTIONTABLE* m_pGLFT;
	
public:

	CGLEntityRenderer(CGLWindowRenderer* pWR, WOGLFUNCTIONTABLE* pFT);
	~CGLEntityRenderer();

	void Initialize(CMapView* pView, CDC* pDC);
	void RenderEntity(CEntity* pEntity, CMapView* pView, int nDrawFlags, int nFlags=0);

};

#endif