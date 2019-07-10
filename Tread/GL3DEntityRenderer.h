// GL3DENTITYRENDERER.H
// Copyright © 1999 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#ifndef GL3DENTITYRENDERER_H
#define GL3DENTITYRENDERER_H

#include "Entity.h"
#include "GL3DWindowRenderer.h"

class CGL3DEntityRenderer
{
private:

	WOGLFUNCTIONTABLE* m_pGLFT;

public:

	CGL3DEntityRenderer(WOGLFUNCTIONTABLE* pGLFT);

	void RenderEntity(CEntity* pEntity, int nDrawFlags, int nFlags=0);
};

#endif