// GDIFACERENDERER.H
// Copyright © 1999 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#ifndef GDIFACERENDERER_H
#define GDIFACERENDERER_H

#include "Face.h"

class CGDIWindowRenderer;
class CGDIFaceRenderer
{

private:

	CGDIWindowRenderer* m_pWR;

public:

	CGDIFaceRenderer(CGDIWindowRenderer* pWR);
	~CGDIFaceRenderer();

	void RenderFace(CMapView* pView, CDC* pDC, CFace* pFace, int nDrawFlags, int nFlags=0);
};

#endif