// GLCAMERARENDERER.H
// Copyright © 1999 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#ifndef GLCAMERARENDERER_H
#define GLCAMERARENDERER_H

#include "Camera.h"
#include "GLRenderPackage.h"

class CGLWindowRenderer;

class CGLCameraRenderer
{
private:

	CGLWindowRenderer* m_pWR;
	WOGLFUNCTIONTABLE* m_pGLFT;

public:

	CGLCameraRenderer(CGLWindowRenderer* pWR, WOGLFUNCTIONTABLE* pGLFT);
	~CGLCameraRenderer();

	void RenderCamera(CCamera* pCamera, CMapView* pView, int nDrawFlags, int nFlags=0);

};

#endif