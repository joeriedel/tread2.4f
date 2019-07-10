// GDICAMERARENDERER.H
// Copyright © 1999 Joe Riedel.
// Author: Joe Riedel.
#ifndef GDIANCHORRENDERER_H
#define GDIANCHORRENDERER_H

#include "Anchor.h"

class CGDIWindowRenderer;
class CGDIAnchorRenderer
{

public:

	CGDIAnchorRenderer();
	~CGDIAnchorRenderer();

	void RenderAnchor(CMapView* pView, CDC* pDC, CAnchor* pAnchor, int nDrawFlags, int nFlags=0);
};


#endif