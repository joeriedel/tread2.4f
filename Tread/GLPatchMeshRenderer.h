// GLPATCHMESHRENDERER.H
// Copyright © 2000 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#ifndef GLPATCHMESHRENDERER_H
#define GLPATCHMESHRENDERER_H

#include "patchmesh.h"
#include "GLRenderPackage.h"

class CGLPatchMeshRenderer
{
private:
	WOGLFUNCTIONTABLE* m_pGLFT;

public:

	CGLPatchMeshRenderer(WOGLFUNCTIONTABLE* m_pGLFT);
	~CGLPatchMeshRenderer();

	void RenderPatchMesh(CPatchMesh* pMesh, CMapView* pView, int nDrawFlags);
};

#endif