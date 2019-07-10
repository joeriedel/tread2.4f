// GDICLIPTOOLRENDERER.H
// Copyright © 1999 Joe Riedel.
// Author: Joe Riedel.
#ifndef GDICLIPTOOLRENDERER_H
#define GDICLIPTOOLRENDERER_H

#include "ClipTool.h"

class CGDIWindowRenderer;
class CGDIClipToolRenderer
{

public:

	CGDIClipToolRenderer();
	~CGDIClipToolRenderer();

	void RenderClipTool(CMapView* pView, CDC* pDC, CTread3DDoc* pDocument, CClipTool* pClipTool, int nDrawFlags, int nFlags=0);
};


#endif