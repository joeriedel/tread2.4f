// GL3DFACERENDERER.CPP
// Copyright © 1999 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#include "stdafx.h"
#include "../TreadAPI.h"
#include "LitFace.h"
#include "GL3DFaceRenderer.h"

CGL3DFaceRenderer::CGL3DFaceRenderer()
{
}

CGL3DFaceRenderer::~CGL3DFaceRenderer()
{
}

void CGL3DFaceRenderer::RenderLitFace(CLitFace* pFace, CBaseObject* pParent, int nDrawFlags, bool bDrawLight, CGL3DWindowRenderer* pWR, WOGLFUNCTIONTABLE* pGLFT)
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
		if(!bDrawLight && (nDrawFlags&(RF_SHADED|RF_TRANSPARENT|RF_SELECTED)) == RF_SHADED)
		{
			// What color do we use?
			switch(pFace->m_nPlaneType)
			{
			case NF_FACING_X:
				pGLFT->glColor4f(1.0f, 1.0f, 1.0f, GL3D_BASE_ALPHA);
				break;
			case NF_FACING_Y:
				pGLFT->glColor4f(0.85f, 0.85f, 0.85f, GL3D_BASE_ALPHA);
				break;
			case NF_FACING_Z:
				pGLFT->glColor4f(0.70f, 0.70f, 0.70f, GL3D_BASE_ALPHA);
				break;
			case NF_FACING_ANY:
				pGLFT->glColor4f(0.78f, 0.78f, 0.78f, GL3D_BASE_ALPHA);
				break;
			}
			pGLFT->glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		}
		else if((nDrawFlags&(RF_SELECTED)) == 0)
			pGLFT->glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

		// Set the bias.
		if(nDrawFlags & RF_SELECTED)
			pWR->SetBias(true, nDrawFlags);
		else
			pWR->SetBias(false, nDrawFlags);

		// Bind the texture.
		pWR->BindTexture(pTex, nDrawFlags);
	}
	else
	{
		// Load the parent name, or this.
		if(pParent == NULL)
			pGLFT->glLoadName((unsigned int)pFace);
		else
			pGLFT->glLoadName((unsigned int)pParent);
	}

	// Solid or texture.
	if(nDrawFlags & (RF_TEXTURED|RF_SOLID|RF_PICKMODE))
	{
		pGLFT->glBegin(GL_POLYGON);
		for(i = 0; i < pFace->m_nNumPoints; i++)
		{
			vV = pFace->m_vPoints[i];
			if(pTex != NULL && (nDrawFlags & (RF_SOLID|RF_LINE|RF_PICKMODE)) == 0)
			{
				vT = pFace->m_vTexCoords[i];
				pGLFT->glTexCoord2f(vT[0]/(float)pTex->m_nWidth, vT[1]/(float)pTex->m_nHeight);
			}
			pGLFT->glVertex3fv(vV);
		}
		pGLFT->glEnd();
	}
	else
	{
		pGLFT->glBegin(GL_LINE_LOOP);
		for(i = 0; i < pFace->m_nNumPoints; i++)
		{
			vV = pFace->m_vPoints[i];
			pGLFT->glVertex3fv(vV);
		}
		pGLFT->glEnd();
	}

	// Blend lightmaps?
	if(bDrawLight)
	{
		// Draw the lightmaps.
		CLightmap* pLightmap;
		CLinkedList<CLightmap>* pList = pFace->GetLightmaps();
		if(pList->IsEmpty())
			return;

		pWR->SetBias(false, RF_TEXTURED);
		pGLFT->glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
		if((nDrawFlags&RF_TEXTURED)==0)
			pGLFT->glEnable(GL_TEXTURE_2D);
		pGLFT->glEnable(GL_BLEND);
		pGLFT->glEnable(GL_POLYGON_OFFSET_FILL);
		pGLFT->glBlendFunc(GL_DST_COLOR, GL_ZERO);
		pGLFT->glDepthFunc(GL_LEQUAL);
		pGLFT->glDepthMask(false);
		pGLFT->glPolygonOffset(-0.10f, -4.0f);

		pList->First();
		for(pLightmap = pList->GetCurItem(); pLightmap != NULL; pLightmap = pList->GetNextItem())
		{
			// Bind it.
			if(pLightmap->m_pTexture->m_bModified)
			{
				pWR->DetachTexture(pLightmap->m_pTexture);
				pLightmap->m_pTexture->m_bModified = false;
			}

			pWR->BindTexture(pLightmap->m_pTexture, RF_TEXTURED, true);
			pGLFT->glBegin(GL_POLYGON);
			for(i = 0; i < pLightmap->m_nNumFragPts; i++)
			{
				pGLFT->glTexCoord2fv(pLightmap->m_pFragTexs[i]);
				pGLFT->glVertex3fv(pLightmap->m_pFragPts[i]);
			}
			pGLFT->glEnd();

		}

		if((nDrawFlags&RF_TEXTURED)==0)
			pGLFT->glDisable(GL_TEXTURE_2D);
		pGLFT->glDisable(GL_BLEND);
		pGLFT->glDisable(GL_POLYGON_OFFSET_FILL);
		pGLFT->glDepthFunc(GL_LESS);
		pGLFT->glDepthMask(true);

	}
}
