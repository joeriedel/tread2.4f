// GL3DBEZIERPATCHRENDERER.H
// Copyright © 1999 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#ifndef GL3DBEZIERPATCHRENDERER_H
#define GL3DBEZIERPATCHRENDERER_H

#include "BezierPatch.h"
#include "GLRenderPackage.h"

class CGL3DFaceRenderer;
class CGL3DWindowRenderer;

class CGL3DBezierPatchRenderer
{
private:

	CGL3DFaceRenderer* m_pFR;

public:

	CGL3DBezierPatchRenderer(CGL3DWindowRenderer* pWR, WOGLFUNCTIONTABLE* pFT);
	~CGL3DBezierPatchRenderer();

	void RenderBezierPatch(CBezierPatch* pPatch, int nDrawFlags, int nFlags=0);

};


#endif