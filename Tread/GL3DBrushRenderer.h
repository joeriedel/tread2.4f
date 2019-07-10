// GL3DBRUSHRENDERER.H
// Copyright © 1999 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#ifndef GL3DBRUSHRENDERER_H
#define GL3DBRUSHRENDERER_H

#include "Brush.h"
#include "GLRenderPackage.h"

class CGL3DFaceRenderer;
class CGL3DWindowRenderer;

class CGL3DBrushRenderer
{
private:

	CGL3DFaceRenderer* m_pFR;
	WOGLFUNCTIONTABLE* m_pGLFT;
	CBaseWindowRenderer* m_pWR;

	void RenderFaceOutLine(CFace* pFace, int nDrawFlags, int nFlags=0);

public:

	CGL3DBrushRenderer(CGL3DWindowRenderer* pWR, WOGLFUNCTIONTABLE* pFT);
	~CGL3DBrushRenderer();

	void RenderBrush(CQBrush* pBrush, int nDrawFlags, int nFlags=0);

};

#endif