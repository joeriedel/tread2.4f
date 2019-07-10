// GDIENTITYRENDERER.H
// Copyright © 1999 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#ifndef GDIENTITYRENDERER_H
#define GDIENTITYRENDERER_H

#include "Entity.h"

class CGDIWindowRenderer;

class CGDIEntityRenderer
{
private:

	CGDIWindowRenderer* m_pWR;
	CFont EntityFont;

public:

	CGDIEntityRenderer(CGDIWindowRenderer* pWR);
	~CGDIEntityRenderer() {}

	void RenderEntity(CMapView* pView, CTread3DDoc* pDocument, CDC* pDC, CEntity* pEntity, int nDrawFlags, int nFlags);

};

#endif