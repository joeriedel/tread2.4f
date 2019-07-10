// GL3DBRUSHRENDERER.H
// Copyright © 1999 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#ifndef GL3DBRUSHRENDERER_H
#define GL3DBRUSHRENDERER_H

#include "../TreadAPI.h"
#include "GL3DFaceRenderer.h"

class CGL3DBrushRenderer
{
private:

	CGL3DFaceRenderer m_FR;

	void RenderFaceOutLine(CFace* pFace, int nDrawFlags);

public:

	WOGLFUNCTIONTABLE* m_pGLFT;
	CGL3DWindowRenderer* m_pWR;

	CGL3DBrushRenderer();
	~CGL3DBrushRenderer();

	void RenderBrush(CQBrush* pBrush, int nDrawFlags, bool bDrawLight);

};

#endif