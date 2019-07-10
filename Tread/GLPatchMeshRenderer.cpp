// GLPATCHMESHRENDERER.CPP
// Copyright © 2000 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#include "stdafx.h"
#include "GLPatchMeshRenderer.h"
#include "GLWindowRenderer.h"
#include "ColorTable.h"
#include "MapView.h"

CGLPatchMeshRenderer::CGLPatchMeshRenderer(WOGLFUNCTIONTABLE* pGLFT)
{
	m_pGLFT = pGLFT;
}

CGLPatchMeshRenderer::~CGLPatchMeshRenderer()
{
}

void CGLPatchMeshRenderer::RenderPatchMesh(CPatchMesh* pMesh, CMapView* pView, int nDrawFlags)
{
	if(nDrawFlags&RF_SELECTED)
		m_pGLFT->glColor3fv(GetColorTable()->GetFloatV(CL_LINE_SEL));
	else
		m_pGLFT->glColor3fv(GetColorTable()->GetFloatV(CL_LINE));

	// Get the face list.
	// Note: this forces the mesh to rebuild if needed.
	CLinkedList<CFace>* pList = pMesh->GetFaceList();
	CFace* pFace;
	vec3_t* pV;
	
	m_pGLFT->glBegin(GL_QUADS);
	for(pFace = pList->ResetPos(); pFace != NULL; pFace = pList->GetNextItem())
	{
		if(pFace->GetNumPoints() != 4)
		{
			Warning("CGLPatchMeshRenderer::RenderPatchMesh(): numpts != 4!\n");
			continue;
		}

		pV = pFace->GetPoints();
		
		m_pGLFT->glVertex2f((*pV)[VI(VEC_X, pView)], (*pV)[VI(VEC_Y, pView)]);
		pV++;
		m_pGLFT->glVertex2f((*pV)[VI(VEC_X, pView)], (*pV)[VI(VEC_Y, pView)]);
		pV++;
		m_pGLFT->glVertex2f((*pV)[VI(VEC_X, pView)], (*pV)[VI(VEC_Y, pView)]);
		pV++;
		m_pGLFT->glVertex2f((*pV)[VI(VEC_X, pView)], (*pV)[VI(VEC_Y, pView)]);
	}
	m_pGLFT->glEnd();
}
