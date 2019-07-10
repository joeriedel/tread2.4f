// GDICAMERARENDERER.H
// Copyright © 1999 Joe Riedel.
// Author: Joe Riedel.
#ifndef GDICAMERARENDERER_H
#define GDICAMERARENDERER_H

#include "Camera.h"

class CGDIWindowRenderer;
class CGDICameraRenderer
{

public:

	CGDICameraRenderer();
	~CGDICameraRenderer();

	void RenderCamera(CMapView* pView, CDC* pDC, CCamera* pCamera, int nDrawFlags, int nFlags=0);
};


#endif