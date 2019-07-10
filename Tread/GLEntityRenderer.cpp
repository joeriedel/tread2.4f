// GLENTITYRENDERER.CPP
// Copyright © 1999 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#include "stdafx.h"
#include "GLRenderPackage.h"
#include "GLWindowRenderer.h"
#include "GLEntityRenderer.h"
#include "MapView.h"
#include "ColorTable.h"
#include "GameAPI.h"
#include "TreadSys.h"
#include "Tread3D.h"

#define ENTITYFONT_LIST_BASE	3000

CGLEntityRenderer::CGLEntityRenderer(CGLWindowRenderer* pWR, WOGLFUNCTIONTABLE* pFT)
{
	m_pGLFT = pFT;
	m_pWR = pWR;
}

CGLEntityRenderer::~CGLEntityRenderer()
{
}

void CGLEntityRenderer::Initialize(CMapView* pView, CDC* pDC)
{
#ifdef _WIN32
	CFont EntityFont;

	EntityFont.CreateFont(12, 0, 0, 0, 200, false, false, false, 
		ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, 
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, "MS Sans Serif");

	// Build first one.
	CFont* pOldFont = pDC->SelectObject(&EntityFont);
	m_pGLFT->e_wglUseFontBitmapsA(pDC->m_hDC, 0, 255, ENTITYFONT_LIST_BASE);
	pDC->SelectObject(pOldFont);
#endif
}

#define CHAR_WIDTH	5
void CGLEntityRenderer::RenderEntity(CEntity* pEntity, CMapView* pView, int nDrawFlags, int nFlags)
{
	int nDrawColor;
	vec3_t vMins, vMaxs;
	bool bLog = (nDrawFlags&RF_FAST)==0;
	GDIWINDOWLINE* pLine;
	vec3_t vSize;
	int nX1, nY1, nX2, nY2;

	if(pEntity->IsOwner() == false)
	{
		if(pEntity->GetEntityDefinition() == NULL)
		{
			vSize[0] = 32.0F;
			vSize[1] = 32.0F;
			vSize[2] = 32.0F;
		}
		else
			VectorCopy(pEntity->GetEntityDefinition()->vSize, vSize);

		if(nDrawFlags & RF_SELECTED)
			nDrawColor = CL_LINE_SEL;
		else
			nDrawColor = CL_ENTITY;

		m_pGLFT->glColor3fv(GetColorTable()->GetFloatV(nDrawColor));

		VectorCopy(pEntity->m_vOrigin, vMins);
		VectorCopy(vMins, vMaxs);
		VectorSubtract(vMins, vSize, vMins);
		VectorAdd(vMaxs, vSize, vMaxs);

		if(nDrawFlags&RF_NOENTITYFILL)
		{
			m_pGLFT->glBegin(GL_LINE_LOOP);
			{
				m_pGLFT->glVertex2i((int)vMins[VI(VEC_X, pView)], (int)vMins[VI(VEC_Y, pView)]);
				m_pGLFT->glVertex2i((int)vMins[VI(VEC_X, pView)], (int)vMaxs[VI(VEC_Y, pView)]);
				m_pGLFT->glVertex2i((int)vMaxs[VI(VEC_X, pView)], (int)vMaxs[VI(VEC_Y, pView)]);
				m_pGLFT->glVertex2i((int)vMaxs[VI(VEC_X, pView)], (int)vMins[VI(VEC_Y, pView)]);
			}
			m_pGLFT->glEnd();
		}
		else
		{
			m_pGLFT->glBegin(GL_QUADS);
			{
				m_pGLFT->glVertex2i((int)vMins[VI(VEC_X, pView)], (int)vMins[VI(VEC_Y, pView)]);
				m_pGLFT->glVertex2i((int)vMins[VI(VEC_X, pView)], (int)vMaxs[VI(VEC_Y, pView)]);
				m_pGLFT->glVertex2i((int)vMaxs[VI(VEC_X, pView)], (int)vMaxs[VI(VEC_Y, pView)]);
				m_pGLFT->glVertex2i((int)vMaxs[VI(VEC_X, pView)], (int)vMins[VI(VEC_Y, pView)]);
			}
			m_pGLFT->glEnd();
		}
	
		if(bLog)
		{
			nX1 = (int)MP_TO_WX(vMins, pView);
			nX2 = (int)MP_TO_WX(vMaxs, pView);
			nY1 = (int)MP_TO_WY(vMins, pView);
			nY2 = (int)MP_TO_WY(vMaxs, pView);

			if(nDrawFlags&RF_NOENTITYFILL)
			{
				// 1
				pLine = m_pWR->GetWindowLine();
				pLine->m_fX1 = (float)nX1;
				pLine->m_fX2 = (float)nX2;
				pLine->m_fY1 = (float)nY1;
				pLine->m_fY2 = (float)nY1;
				pLine->m_nFlags = 0;
				pLine->m_bBuild = true;
				pLine->m_pObject = pEntity;
				// 2
				pLine = m_pWR->GetWindowLine();
				pLine->m_fX1 = (float)nX2;
				pLine->m_fX2 = (float)nX2;
				pLine->m_fY1 = (float)nY1;
				pLine->m_fY2 = (float)nY2;
				pLine->m_nFlags = 0;
				pLine->m_bBuild = true;
				pLine->m_pObject = pEntity;
				// 3
				pLine = m_pWR->GetWindowLine();
				pLine->m_fX1 = (float)nX2;
				pLine->m_fX2 = (float)nX1;
				pLine->m_fY1 = (float)nY2;
				pLine->m_fY2 = (float)nY2;
				pLine->m_nFlags = 0;
				pLine->m_bBuild = true;
				pLine->m_pObject = pEntity;
				// 4
				pLine = m_pWR->GetWindowLine();
				pLine->m_fX1 = (float)nX1;
				pLine->m_fX2 = (float)nX1;
				pLine->m_fY1 = (float)nY2;
				pLine->m_fY2 = (float)nY1;
				pLine->m_nFlags = 0;
				pLine->m_bBuild = true;
				pLine->m_pObject = pEntity;
			}
			else
			{
				pLine = m_pWR->GetWindowLine();
				pLine->m_fX1 = (float)nX1;
				pLine->m_fX2 = (float)nX2;
				pLine->m_fY1 = (float)nY1;
				pLine->m_fY2 = (float)nY2;
				pLine->m_bBuild = true;
				pLine->m_pObject = pEntity;
				pLine->m_nFlags = GWLF_RANGE;
			}	
		}
	}

	// Draw the name.
	if(nDrawFlags&RF_SELECTED)
	{
		if((nDrawFlags&RF_NOENTNAMES)==0)
		{
			CString sString;
			ENTITYDEFINITION* pEntDef = pEntity->GetEntityDefinition();
			if(pEntDef != NULL)
			{
				if(GetRegBool(B_SMARTNAME))
					sString = pEntDef->sSmartName;
				else
					sString = pEntDef->sName;


				if(!bLog && !pEntity->IsOwner())
				{
					nX1 = (int)MP_TO_WX(vMins, pView);
					nX2 = (int)MP_TO_WX(vMaxs, pView);
					nY1 = (int)MP_TO_WY(vMins, pView);
					nY2 = (int)MP_TO_WY(vMaxs, pView);
				}
				else if(pEntity->IsOwner())
				{
					nX1 = (int)MP_TO_WX(pEntity->m_vMins, pView);
					nX2 = (int)MP_TO_WX(pEntity->m_vMaxs, pView);
					nY1 = (int)MP_TO_WY(pEntity->m_vOrigin, pView) + 6;
					nY2 = nY1;
				}

				int nSize = sString.GetLength() * CHAR_WIDTH;
				int nX = ((nX1 + nX2) >> 1) - (nSize >> 1);
				int nY;
				if(VO(VT(pView), VEC_Y) > 0.0F)
					nY = nY1 - 15;
				else
					nY = nY2 - 15;
				
				m_pGLFT->glPushMatrix();
				m_pGLFT->glLoadIdentity();
				m_pGLFT->gluOrtho2D(0, pView->m_rcClientRect.right, pView->m_rcClientRect.bottom, 0);
				m_pGLFT->glListBase(ENTITYFONT_LIST_BASE);
				m_pGLFT->glColor3f(1.0F, 1.0F, 1.0F);
				m_pWR->DrawString(nX, nY+10, sString);
				m_pGLFT->glPopMatrix();
			}
		}
	}
}
