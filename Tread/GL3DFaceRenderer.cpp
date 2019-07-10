// GL3DFACERENDERER.CPP
// Copyright © 1999 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#include "stdafx.h"
#include "Face.h"
#include "GLRenderPackage.h"
#include "GL3DWindowRenderer.h"
#include "GL3DFaceRenderer.h"
#include "TreadSys.h"
#include "ColorTable.h"

CGL3DFaceRenderer::CGL3DFaceRenderer(CGL3DWindowRenderer* pWR, WOGLFUNCTIONTABLE* pGLFT)
{
	m_pWR = pWR;
	m_pGLFT = pGLFT;
}

CGL3DFaceRenderer::~CGL3DFaceRenderer()
{
}

void CGL3DFaceRenderer::RenderFace(CFace* pFace, CBaseObject* pParent, int nDrawFlags, int nFlags)
{
	int i;
	vec_t* vV, *vT;
	CTexture* pTex = pFace->GetTexturePtr();

	nDrawFlags |= pFace->m_nObjectRenderFlags;

	// Do nothing in line & pick.
	if((nDrawFlags & (RF_LINE|RF_PICKMODE))==(RF_LINE|RF_PICKMODE))
		return;

	// If there is no texture, then switch to a color real fast.
	if(pTex == NULL && (nDrawFlags & RF_TEXTURED))
		nDrawFlags |= RF_SOLID;

	if((nDrawFlags&RF_PICKMODE)==0)
	{
		if((nDrawFlags&(RF_SHADED|RF_TRANSPARENT|RF_SELECTED)) == RF_SHADED)
		{
			// What color do we use?
			switch(pFace->m_nPlaneType)
			{
			case NF_FACING_X:
				m_pGLFT->glColor4f(1.0f, 1.0f, 1.0f, GL3D_BASE_ALPHA);
				break;
			case NF_FACING_Y:
				m_pGLFT->glColor4f(0.85f, 0.85f, 0.85f, GL3D_BASE_ALPHA);
				break;
			case NF_FACING_Z:
				m_pGLFT->glColor4f(0.70f, 0.70f, 0.70f, GL3D_BASE_ALPHA);
				break;
			case NF_FACING_ANY:
				m_pGLFT->glColor4f(0.78f, 0.78f, 0.78f, GL3D_BASE_ALPHA);
				break;
			}
			m_pGLFT->glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		}
		else if((nDrawFlags&(RF_SELECTED)) == 0)
			m_pGLFT->glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

		// Set the bias.
		if(nDrawFlags & RF_SELECTED)
			m_pWR->SetBias(true, nDrawFlags);
		else
			m_pWR->SetBias(false, nDrawFlags);

		// Bind the texture.
		m_pWR->BindTexture(pTex, nDrawFlags);
	}
	else
	{
		// Load the parent name, or this.
		if(pParent == NULL)
			m_pGLFT->glLoadName((unsigned int)pFace);
		else
			m_pGLFT->glLoadName((unsigned int)pParent);
	}

	

	// Solid or texture.
	if(nDrawFlags & (RF_TEXTURED|RF_SOLID|RF_PICKMODE))
	{
		m_pGLFT->glBegin(GL_POLYGON);
		for(i = 0; i < pFace->m_nNumPoints; i++)
		{
			vV = pFace->m_vPoints[i];
			if(pTex != NULL && (nDrawFlags & (RF_SOLID|RF_LINE|RF_PICKMODE)) == 0)
			{
				vT = pFace->m_vTexCoords[i];
				m_pGLFT->glTexCoord2f(vT[0]/(float)pTex->m_nWidth, vT[1]/(float)pTex->m_nHeight);
			}
			m_pGLFT->glVertex3fv(vV);
		}
		m_pGLFT->glEnd();
	}
	else
	{
		m_pGLFT->glBegin(GL_LINE_LOOP);
		for(i = 0; i < pFace->m_nNumPoints; i++)
		{
			vV = pFace->m_vPoints[i];
			m_pGLFT->glVertex3fv(vV);
		}
		m_pGLFT->glEnd();
	}

	// If selected and we're not hiding the selection outline.
	if((nDrawFlags&RF_SELECTED)==RF_SELECTED && (nDrawFlags&RF_NO3DSELOUTLINE)==0)
	{
		m_pGLFT->glDisable(GL_TEXTURE_2D);
		m_pGLFT->glDepthFunc(GL_ALWAYS);
		m_pGLFT->glColor3fv(GetColorTable()->GetFloatV(CL_HOTSPOT_BOX));
		m_pGLFT->glBegin(GL_LINE_LOOP);
		for(i = 0; i < pFace->m_nNumPoints; i++)
		{
			vV = pFace->m_vPoints[i];
			m_pGLFT->glVertex3fv(vV);
		}
		m_pGLFT->glEnd();
		m_pWR->SetBias(false, 0);
		//m_pGLFT->glColor4f(1.0F, 1.0F, 1.0F, GL3D_BASE_ALPHA);
		m_pGLFT->glDepthFunc(GL_LESS);
		if(nDrawFlags&RF_TEXTURED)
			m_pGLFT->glEnable(GL_TEXTURE_2D);
	}
}
