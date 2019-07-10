// GDIENTITYRENDERER.CPP
// Copyright © 1999 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#include "stdafx.h"
#include "MathLib.h"
#include "RenderAPI.h"
#include "ColorTable.h"
#include "Entity.h"
#include "GameAPI.h"
#include "GDIWindowRenderer.h"
#include "GDIEntityRenderer.h"
#include "MapView.h"
#include "TreadSys.h"
#include "Tread3D.h"

CGDIEntityRenderer::CGDIEntityRenderer(CGDIWindowRenderer* pWR)
{
	m_pWR = pWR;
	EntityFont.CreateFont(12, 0, 0, 0, 200, false, false, false, 
		ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, 
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, "MS Sans Serif");
}

#define CHAR_WIDTH	5
void CGDIEntityRenderer::RenderEntity(CMapView* pView, CTread3DDoc* pDocument, CDC* pDC, CEntity* pEntity, int nDrawFlags, int nFlags)
{
	int nDrawColor;
	vec3_t vMins, vMaxs;
	bool bLog = (nDrawFlags&RF_FAST)==0;
	GDIWINDOWLINE* pLine;
	int nX1, nY1, nX2, nY2;
	vec3_t vSize;
	CPen* pOldPen;

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

		VectorCopy(pEntity->m_vOrigin, vMins);
		VectorCopy(vMins, vMaxs);
		VectorSubtract(vMins, vSize, vMins);
		VectorAdd(vMaxs, vSize, vMaxs);
		
		nX1 = (int)MP_TO_WX(vMins, pView);
		nX2 = (int)MP_TO_WX(vMaxs, pView);
		nY1 = (int)MP_TO_WY(vMins, pView);
		nY2 = (int)MP_TO_WY(vMaxs, pView);

		if(nDrawFlags&RF_NOENTITYFILL)
		{
			pOldPen = pDC->SelectObject(GetColorTable()->GetPen(nDrawColor));

			// Draw the 4 sides.
			pDC->MoveTo(nX1, nY1);
			pDC->LineTo(nX2, nY1);
			pDC->LineTo(nX2, nY2);
			pDC->LineTo(nX1, nY2);
			pDC->LineTo(nX1, nY1);

			pDC->SelectObject(pOldPen);
		}
		else
			pDC->FillSolidRect((nX1 < nX2) ? nX1 : nX2, (nY1 < nY2) ? nY1 : nY2, abs(nX2-nX1), abs(nY2-nY1), GetColorTable()->GetColor(nDrawColor));

		if(bLog)
		{
			if(nDrawFlags&RF_NOENTITYFILL)
			{
				// 1
				pLine = m_pWR->GetWindowLine();
				pLine->m_fX1 = (float)nX1;
				pLine->m_fX2 = (float)nX2;
				pLine->m_fY1 = (float)nY1;
				pLine->m_fY2 = (float)nY1;
				pLine->m_bBuild = true;
				pLine->m_pObject = pEntity;
				// 2
				pLine = m_pWR->GetWindowLine();
				pLine->m_fX1 = (float)nX2;
				pLine->m_fX2 = (float)nX2;
				pLine->m_fY1 = (float)nY1;
				pLine->m_fY2 = (float)nY2;
				pLine->m_bBuild = true;
				pLine->m_pObject = pEntity;
				// 3
				pLine = m_pWR->GetWindowLine();
				pLine->m_fX1 = (float)nX2;
				pLine->m_fX2 = (float)nX1;
				pLine->m_fY1 = (float)nY2;
				pLine->m_fY2 = (float)nY2;
				pLine->m_bBuild = true;
				pLine->m_pObject = pEntity;
				// 4
				pLine = m_pWR->GetWindowLine();
				pLine->m_fX1 = (float)nX1;
				pLine->m_fX2 = (float)nX1;
				pLine->m_fY1 = (float)nY2;
				pLine->m_fY2 = (float)nY1;
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
	else
	{
		nX1 = (int)MP_TO_WX(pEntity->m_vMins, pView);
		nX2 = (int)MP_TO_WX(pEntity->m_vMaxs, pView);
		nY1 = (int)MP_TO_WY(pEntity->m_vOrigin, pView) + 8; // Counter act the -15 given later
		nY2 = nY1;
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

				int nSize = sString.GetLength() * CHAR_WIDTH;
				int nX = ((nX1 + nX2) >> 1) - (nSize >> 1);
				int nY;

				if(VO(VT(pView), VEC_Y) > 0.0F)
					nY = nY1 - 15;			
				else
					nY = nY2 - 15;

				CFont* pOldFont = pDC->SelectObject(&EntityFont);
				pDC->SetBkMode(TRANSPARENT);
				pDC->SetTextColor(RGB(255, 255, 255));
				pDC->TextOut(nX, nY, sString);
				pDC->SelectObject(pOldFont);
			}
		}
	}
}
