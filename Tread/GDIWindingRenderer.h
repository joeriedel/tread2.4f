// GDIWINDINGRENDERER.H
// Copyright © 1999 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#ifndef GDIWINDINGRENDERER_H
#define GDIWINDINGRENDERER_H

#include "Winding.h"

class CGDIWindowRenderer;
class CGDIWindingRenderer
{

private:

	CGDIWindowRenderer* m_pWR;

public:

	CGDIWindingRenderer(CGDIWindowRenderer* pWR);
	~CGDIWindingRenderer();

	void RenderWinding(CMapView* pView, CDC* pDC, CWinding* pWinding, CBaseObject* pParent, int nDrawFlags, int nFlags=0);
};

#endif