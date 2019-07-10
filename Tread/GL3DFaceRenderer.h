// GL3DFACERENDERER.H
// Copyright © 1999 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#ifndef GL3DFACERENDERER_H
#define GL3DFACERENDERER_H

#include "Face.h"
#include "GLRenderPackage.h"

class CGL3DWindowRenderer;

class CGL3DFaceRenderer
{
private:

	CGL3DWindowRenderer* m_pWR;
	WOGLFUNCTIONTABLE* m_pGLFT;

public:

	CGL3DFaceRenderer(CGL3DWindowRenderer* pWR, WOGLFUNCTIONTABLE* pGLFT);
	~CGL3DFaceRenderer();

	void RenderFace(CFace* pFace, CBaseObject* pParent, int nDrawFlags, int nFlags=0);

};

#endif