// GL3DFACERENDERER.H
// Copyright © 1999 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#ifndef GL3DFACERENDERER_H
#define GL3DFACERENDERER_H

#include "../TreadAPI.h"
#include "LitFace.h"

class CGL3DFaceRenderer
{
private:

public:

	CGL3DFaceRenderer();
	~CGL3DFaceRenderer();

	void RenderLitFace(CLitFace* pFace, CBaseObject* pParent, int nDrawFlags, bool bDrawLight, CGL3DWindowRenderer* pWR, WOGLFUNCTIONTABLE* pGLFT);

};

#endif