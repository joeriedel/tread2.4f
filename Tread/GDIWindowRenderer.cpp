// GDIWINDOWRENDERER.CPP
// Copyright © 1999 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#include "stdafx.h"
#include "RenderAPI.h"
#include "MapView.h"
#include "ColorTable.h"
#include "MapView.h"
#include "HotSpotManager.h"
#include "PluginAPI.h"
#include "GDIWindowRenderer.h"

#ifdef _WIN32
#include "Dib24.h"
#include "GDIBrushRenderer.h"
#include "GDIEntityRenderer.h"
#include "GDICameraRenderer.h"
#include "GDIClipToolRenderer.h"
#include "GDIAnchorRenderer.h"
#endif

#define INIT_LINE_SIZE	1024

GDIWINDOWLINE::GDIWINDOWLINE()
{
	m_fB = 0.0F;
	m_fMaxX = 0.0F;
	m_fMaxY = 0.0F;
	m_fMinX = 0.0F;
	m_fMinY = 0.0F;
	m_fSlope = 0.0F;
	m_fX1 = 0.0F;
	m_fX2 = 0.0F;
	m_fY1 = 0.0F;
	m_fY2 = 0.0F;
	m_nFlags = 0;
	m_pObject = NULL;
	m_bBuild = true;
}

#define LINE_SLOPE_PREC	4.0
void GDIWINDOWLINE::BuildLine()
{
	if(m_bBuild==false)
		return;

	float fTemp;
	
	// Get max's and swap if needed.
	m_fMinX = m_fX1; m_fMaxX = m_fX2;
	m_fMinY = m_fY1; m_fMaxY = m_fY2;

	if(m_fMaxX < m_fMinX)
	{
		fTemp = m_fMinX;
		m_fMinX = m_fMaxX;
		m_fMaxX = fTemp;
	}
	if(m_fMaxY < m_fMinY)
	{
		fTemp = m_fMinY;
		m_fMinY = m_fMaxY;
		m_fMaxY = fTemp;
	}
	
	if(m_nFlags&GWLF_RANGE)
	{
		m_bBuild=false;
		return;
	}

	float fNum = m_fY2 - m_fY1;
	float fDen = m_fX2 - m_fX1;

	if(fabs(fNum) < LINE_SLOPE_PREC)
	{
		m_nFlags = GWLF_HORZ;
		return;
	}
	if(fabs(fDen) < LINE_SLOPE_PREC)
	{
		m_nFlags = GWLF_VERT;
		return;
	}

	m_nFlags = 0;
	m_fSlope = fNum / fDen;
	m_fB = m_fY1 - (m_fSlope * m_fX1);
	m_bBuild = false;
}

#define SELECT_PREC	4.0F
bool GDIWINDOWLINE::Hit(float fX, float fY)
{
	float fDiff;
	switch(m_nFlags)
	{
	case GWLF_HORZ:
		if(fX < m_fMinX || fX > m_fMaxX)
			return false;
		fDiff = fY - m_fY1;
		break;
	case GWLF_VERT:
		if(fY < m_fMinY || fY > m_fMaxY)
			return false;
		fDiff = fX - m_fX1;
		break;
	case GWLF_RANGE:
		if(fX < m_fMinX || fX > m_fMaxX)
			return false;
		if(fY < m_fMinY || fY > m_fMaxY)
			return false;

		return true;
		break;
	default:
		if(fX < m_fMinX || fX > m_fMaxX)
			return false;
		if(fY < m_fMinY || fY > m_fMaxY)
			return false;
		fDiff = fY - (fX * m_fSlope + m_fB);
		break;
	}

	return fabs(fDiff) <= SELECT_PREC;
}

#ifdef _WIN32
GDIWINDOWLINE* CGDIWindowRenderer::GetWindowLine()
{
	int nNewSize;

	// Need to resize?
	if(m_nNextLine == m_nNumLines)
	{
		// Double.
		nNewSize = m_nNumLines << 1;
		GDIWINDOWLINE* pNewLines = new GDIWINDOWLINE[nNewSize];
		memcpy(pNewLines, m_pLines, m_nNumLines);
		delete[] m_pLines;
		m_pLines = pNewLines;
		m_nNumLines = nNewSize;
	}

	m_nNextLine++;
	m_pLines[m_nNextLine-1].m_nFlags = 0;
	return &m_pLines[m_nNextLine-1];
}

bool CGDIWindowRenderer::LockDevice(CMapView* pView, CTread3DDoc* pDocument, CDC* pDC, int nDrawFlags)
{
	// Reset the drawn lines.
	if((nDrawFlags&RF_FAST)==0)
		m_nNextLine = 0;
	return true;
}

#define CHAR_WIDTH	7
#define CHAR_HEIGHT 10
void CGDIWindowRenderer::DrawBBoxSize(CMapView* pView, CTread3DDoc* pDocument, CDC* pDC, vec3_t vSelMins, vec3_t vSelMaxs, vec3_t vCenter, int nDrawColor)
{
	CFont* pOldFont = m_pBitmapDC->SelectObject(&BBoxSizeFont);
	m_pBitmapDC->SetBkMode(TRANSPARENT);
	m_pBitmapDC->SetTextColor(GetColorTable()->GetColor(CL_BBOX_SIZE));
	char szBuff[256];

	// These aren't guarenteed to be ordered correctly.
	vec3_t vMins, vMaxs, vSize;
	VectorCopy(vSelMins, vMins);
	VectorCopy(vSelMaxs, vMaxs);
	// ... So order them.
	OrderRange(vMins, vMaxs);
	VectorSubtract(vMaxs, vMins, vSize);

	// X's
	if(vSize[VI(VEC_X, pView)] > 0.0F)
	{
		CString sWidth = itoa((int)vSize[VI(VEC_X, pView)], szBuff, 10);
		int nSize = sWidth.GetLength() * CHAR_WIDTH;
		int nY1 = MY_TO_WY(vMins[VI(VEC_Y, pView)], pView) + (VO(VT(pView), VEC_Y) > 0 ? -18 : 8);
		int nX1 = MX_TO_WX(vCenter[VI(VEC_X, pView)], pView) - (nSize>>1);
		m_pBitmapDC->TextOut(nX1, nY1, sWidth);

		nY1 = MY_TO_WY(vMaxs[VI(VEC_Y, pView)], pView) + (VO(VT(pView), VEC_Y) > 0 ? 8 : -18);
		m_pBitmapDC->TextOut(nX1, nY1, sWidth);
	}

	// Y's
	if(vSize[VI(VEC_Y, pView)] > 0.0F)
	{
		CString sHeight = itoa((int)vSize[VI(VEC_Y, pView)], szBuff, 10);
		int nSize = sHeight.GetLength() * CHAR_WIDTH;
		int nY1 = MY_TO_WY(vCenter[VI(VEC_Y, pView)], pView) - (CHAR_HEIGHT/2);
		int nX1 = MX_TO_WX(vMins[VI(VEC_X, pView)], pView);
		int nX2 = MX_TO_WX(vMaxs[VI(VEC_X, pView)], pView);
		if(VO(VT(pView), VEC_X) > 0)
		{
			m_pBitmapDC->TextOut(nX1-nSize-5, nY1, sHeight);
			m_pBitmapDC->TextOut(nX2+5, nY1, sHeight);
		}
		else
		{
			m_pBitmapDC->TextOut(nX1+5, nY1, sHeight);
			m_pBitmapDC->TextOut(nX2-nSize-5, nY1, sHeight);
		}
	}

	m_pBitmapDC->SelectObject(pOldFont);
}

void CGDIWindowRenderer::DrawI_TARGET(CMapView* pView, CTread3DDoc* pDocument, CDC* pDC, CBaseObject* pObject, int nDrawFlags, int nType)
{
	CBaseTargetObject* pT = (CBaseTargetObject*)pObject->GetInterface(I_TARGET);
	CBaseTransformableObject* pSrcTR = (CBaseTransformableObject*)pObject->GetInterface(I_TRANSFORMABLE);
	CString sName = pT->GetTargetName();
	if(sName == "")
		return;
	TTARGET* pTSet = pDocument->GetTargetAPI()->FindTargetSet(sName);
	if(pTSet == NULL)
		return;

	CPen* pOldPen = m_pBitmapDC->SelectObject(GetColorTable()->GetPen(CL_LINK));

	TREF* pRef;
	pTSet->TRefList.First();
	for(pRef = pTSet->TRefList.GetCurItem(); pRef != NULL; pRef = pTSet->TRefList.GetNextItem())
	{
		CBaseTransformableObject* pDstTR = (CBaseTransformableObject*)pRef->pObject->GetInterface(I_TRANSFORMABLE);

		// Draw the line.
		m_pBitmapDC->MoveTo(MX_TO_WX(pDstTR->m_vOrigin[VI(VEC_X, pView)], pView), MY_TO_WY(pDstTR->m_vOrigin[VI(VEC_Y, pView)], pView));
		m_pBitmapDC->LineTo(MX_TO_WX(pSrcTR->m_vOrigin[VI(VEC_X, pView)], pView), MY_TO_WY(pSrcTR->m_vOrigin[VI(VEC_Y, pView)], pView));
	}

	m_pBitmapDC->SelectObject(pOldPen);
}

// Get's the line that was hit.
CBaseObject* CGDIWindowRenderer::GetHitObject(CMapView* pView, CTread3DDoc* pDocument, int nClassFlags, int nMouseX, int nMouseY)
{
	int i;
	GDIWINDOWLINE* pLine;
	
	for(i = m_nNextLine-1; i >= 0; i--)
	{
		pLine = &m_pLines[i];
		// Skip if not correct type.

		// ???: This needs to be fixed.
		if(pLine->m_pObject == NULL)
			continue;	

		if((pLine->m_pObject->GetObjectClass()&nClassFlags)==0)
			continue;
		pLine->BuildLine();
		if(pLine->Hit((float)nMouseX, (float)nMouseY))
			return pLine->m_pObject;
	}

	return NULL;
}

#define LITTLEX_SIZE	6
void CGDIWindowRenderer::DrawLittleX(CMapView* pView, CTread3DDoc* pDocument, CDC* pDC, vec2_t vMapPos, int nColor)
{
	CPen* pOldPen = m_pBitmapDC->SelectObject(GetColorTable()->GetPen(nColor));

	int nWX, nWY;

	nWX = MX_TO_WX(vMapPos[0], pView);
	nWY = MY_TO_WY(vMapPos[1], pView);

	m_pBitmapDC->MoveTo(nWX-LITTLEX_SIZE, nWY-LITTLEX_SIZE);
	m_pBitmapDC->LineTo(nWX+LITTLEX_SIZE, nWY+LITTLEX_SIZE);
	m_pBitmapDC->MoveTo(nWX+LITTLEX_SIZE, nWY-LITTLEX_SIZE);
	m_pBitmapDC->LineTo(nWX-LITTLEX_SIZE, nWY+LITTLEX_SIZE);

	m_pBitmapDC->SelectObject(pOldPen);
}

#define HOTSPOT_SIZE	4
void CGDIWindowRenderer::DrawHotSpot(CMapView* pView, CTread3DDoc* pDocument, CDC* pDC, CHotSpot* pSpot, int nDrawFlags, int nFlags)
{
	int nColor;
	// Compute, for updated (scrolling etc).
	pSpot->ComputeWindowXY(pView);

	if(pSpot->m_nDrawFlags&HSDF_NODRAW)
		return;

	CPen* pOldPen;
	CBrush* pOldBrush;
	
	if(pSpot->m_nDrawFlags&HSDF_LINE)
	{
		if(pSpot->m_pLineSpot != NULL)
		{
			pOldPen = m_pBitmapDC->SelectObject(GetColorTable()->GetPen(CL_HOTSPOT_LINE));
			m_pBitmapDC->MoveTo(pSpot->m_nWX, pSpot->m_nWY);
			pSpot->m_pLineSpot->ComputeWindowXY(pView);
			m_pBitmapDC->LineTo(pSpot->m_pLineSpot->m_nWX, pSpot->m_pLineSpot->m_nWY);
			m_pBitmapDC->SelectObject(pOldPen);
		}
	}


	if(nDrawFlags&RF_SELECTED)
	{
		nColor = CL_HOTSPOT_SEL;
		pOldPen = m_pBitmapDC->SelectObject(GetColorTable()->GetPen(CL_HOTSPOT_SEL));
		pOldBrush = m_pBitmapDC->SelectObject(GetColorTable()->GetBrush(CL_HOTSPOT_SEL));
	}
	else if(pSpot->m_nDrawFlags&HSDF_HD_COLOR)
	{
		nColor = CL_BRUSH_HANDLE;
		pOldPen = m_pBitmapDC->SelectObject(GetColorTable()->GetPen(CL_BRUSH_HANDLE));
		pOldBrush = m_pBitmapDC->SelectObject(GetColorTable()->GetBrush(CL_BRUSH_HANDLE));
	}
	else
	{
		nColor = CL_HOTSPOT;
		pOldPen = m_pBitmapDC->SelectObject(GetColorTable()->GetPen(CL_HOTSPOT));
		pOldBrush = m_pBitmapDC->SelectObject(GetColorTable()->GetBrush(CL_HOTSPOT));
	}

	if(pSpot->m_nDrawFlags&HSDF_CROSS)
	{
		m_pBitmapDC->MoveTo(pSpot->m_nWX-HOTSPOT_SIZE, pSpot->m_nWY-HOTSPOT_SIZE);
		m_pBitmapDC->LineTo(pSpot->m_nWX+HOTSPOT_SIZE, pSpot->m_nWY+HOTSPOT_SIZE);
		m_pBitmapDC->MoveTo(pSpot->m_nWX+HOTSPOT_SIZE, pSpot->m_nWY-HOTSPOT_SIZE);
		m_pBitmapDC->LineTo(pSpot->m_nWX-HOTSPOT_SIZE, pSpot->m_nWY+HOTSPOT_SIZE);
	}
	else if(pSpot->m_nDrawFlags&HSDF_SQUARE)
		m_pBitmapDC->FillSolidRect(pSpot->m_nWX-(HOTSPOT_SIZE>>1), pSpot->m_nWY-(HOTSPOT_SIZE>>1), HOTSPOT_SIZE, HOTSPOT_SIZE, GetColorTable()->GetColor(nColor));
	else
		m_pBitmapDC->Ellipse(pSpot->m_nWX-(HOTSPOT_SIZE>>1), pSpot->m_nWY-(HOTSPOT_SIZE>>1), pSpot->m_nWX+(HOTSPOT_SIZE>>1), pSpot->m_nWY+(HOTSPOT_SIZE>>1));

	m_pBitmapDC->SelectObject(pOldPen);
	m_pBitmapDC->SelectObject(pOldBrush);
}

bool CGDIWindowRenderer::ClipLine(CMapView* pView, vec_t* vPta, vec_t* vPtb)
{
	bool bXMin[2]={false,false}, bXMax[2]={false,false};
	bool bYMin[2]={false,false}, bYMax[2]={false,false};

	//return false;

	// Pta
	if(VO(VT(pView), VEC_X) > 0.0F)
	{
		if(vPta[VI(VEC_X, pView)] > VMMAX_X(pView))
			bXMax[0] = true;
		if(vPta[VI(VEC_X, pView)] < VMMIN_X(pView))
			bXMin[0] = true;
	}
	else
	{
		if(vPta[VI(VEC_X, pView)] < VMMAX_X(pView))
			bXMax[0] = true;
		if(vPta[VI(VEC_X, pView)] > VMMIN_X(pView))
			bXMin[0] = true;
	}

	if(VO(VT(pView), VEC_Y) > 0.0F)
	{
		if(vPta[VI(VEC_Y, pView)] > VMMAX_Y(pView))
			bYMax[1] = true;
		if(vPta[VI(VEC_Y, pView)] < VMMIN_Y(pView))
			bYMin[1] = true;
	}
	else
	{
		if(vPta[VI(VEC_Y, pView)] < VMMAX_Y(pView))
			bYMax[1] = true;
		if(vPta[VI(VEC_Y, pView)] > VMMIN_Y(pView))
			bYMin[1] = true;
	}

	// Ptb
	if(VO(VT(pView), VEC_X) > 0.0F)
	{
		if(vPtb[VI(VEC_X, pView)] > VMMAX_X(pView))
			bXMax[1] = true;
		if(vPtb[VI(VEC_X, pView)] < VMMIN_X(pView))
			bXMin[1] = true;
	}
	else
	{
		if(vPtb[VI(VEC_X, pView)] < VMMAX_X(pView))
			bXMax[1] = true;
		if(vPtb[VI(VEC_X, pView)] > VMMIN_X(pView))
			bXMin[1] = true;
	}

	if(VO(VT(pView), VEC_Y) > 0.0F)
	{
		if(vPtb[VI(VEC_Y, pView)] > VMMAX_Y(pView))
			bYMax[1] = true;
		if(vPtb[VI(VEC_Y, pView)] < VMMIN_Y(pView))
			bYMin[1] = true;
	}
	else
	{
		if(vPtb[VI(VEC_Y, pView)] < VMMAX_Y(pView))
			bYMax[1] = true;
		if(vPtb[VI(VEC_Y, pView)] > VMMIN_Y(pView))
			bYMin[1] = true;
	}

	return (bXMin[0] && bXMin[1]) ||
		(bXMax[0] && bXMax[1]) ||
		(bYMin[0] && bYMin[1]) ||
		(bYMax[0] && bYMax[1]);
}

void CGDIWindowRenderer::OnSize(CMapView* pView, CDC* pDC)
{
	DeleteDib24();
	CreateDib24(pView, pDC);
}

void CGDIWindowRenderer::Flush(CMapView* pView, CTread3DDoc* pDocument, CDC* pDC)
{
	CFont* pOldFont = m_pBitmapDC->SelectObject(&ViewTypeFont);
	m_pBitmapDC->SetBkMode(TRANSPARENT);
	if(pView->m_bFocus)
		m_pBitmapDC->SetTextColor(GetColorTable()->GetColor(CL_VIEW_SELECTED));
	else
		m_pBitmapDC->SetTextColor(GetColorTable()->GetColor(CL_VIEW_NORMAL));
	m_pBitmapDC->TextOut(5, 5, pView->m_sView);
	m_pBitmapDC->SelectObject(pOldFont);

	BlitDib24(pDC, m_pBitmapDC, pView->m_rcClientRect);	
}

bool CGDIWindowRenderer::CreateDib24(CMapView* pView, CDC* pDC)
{
	m_pBitmapDC = new CDC();
	m_pBitmapDC->CreateCompatibleDC(pDC);
	m_hBitmap = ::CreateDib24(m_pBitmapDC->m_hDC, pView->m_rcClientRect);
	if(m_hBitmap == NULL)
	{
		delete m_pBitmapDC;
		m_pBitmapDC = NULL;
		return false;
	}

	m_hOldBitmap = (HBITMAP)m_pBitmapDC->SelectObject(m_hBitmap);
	return true;
}

bool CGDIWindowRenderer::DeleteDib24()
{
	if(m_pBitmapDC == NULL)
		return true;

	m_pBitmapDC->SelectObject(m_hOldBitmap);
	::DeleteDib24(m_hBitmap);
	m_hBitmap = NULL;

	delete m_pBitmapDC;
	m_pBitmapDC = NULL;
	return true;
}

bool CGDIWindowRenderer::Initialize(CMapView* pView, CDC* pDC, int nType)
{
	Debug("gdiwindowrenderer init\n");
	CreateDib24(pView, pDC);
	Debug("done\n");
	return true;
}

bool CGDIWindowRenderer::Shutdown(CMapView* pView, CDC* pDC)
{
	Debug("gdiwindowrenderer shutdown\n");
	Debug("done\n");
	return true;
}

CGDIWindowRenderer::CGDIWindowRenderer()
{
	m_pBR = new CGDIBrushRenderer(this);
	m_pER = new CGDIEntityRenderer(this);
	m_pCR = new CGDICameraRenderer();
	m_pCTR = new CGDIClipToolRenderer();
	m_pAR = new CGDIAnchorRenderer();
	m_pLines = new GDIWINDOWLINE[INIT_LINE_SIZE];
	m_nNumLines = INIT_LINE_SIZE;
	m_nNextLine = 0;

	BBoxSizeFont.CreateFont(12, 0, 0, 0, 200, false, false, false, 
		ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, 
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "MS Sans Serif");
	
	ViewTypeFont.CreateFont(14, 0, 0, 0, 200, false, false, false, 
		ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, 
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "MS Sans Serif");

	m_pBitmapDC = NULL;
	m_hBitmap = NULL;
}

CGDIWindowRenderer::~CGDIWindowRenderer()
{
	DeleteDib24();

	if(m_pBR != NULL)
		delete m_pBR;
	if(m_pER != NULL)
		delete m_pER;
	if(m_pCR != NULL)
		delete m_pCR;
	if(m_pCTR != NULL)
		delete m_pCTR;
	if(m_pAR != NULL)
		delete m_pAR;

	delete[] m_pLines;

	m_pBR = NULL;
	m_pER = NULL;
	m_pCR = NULL;
}

// Draw's a bounding box.
void CGDIWindowRenderer::DrawBBox(CMapView* pView, CTread3DDoc* pDocument, CDC* pDC, vec3_t vMins, vec3_t vMaxs, int nDrawColor, int nFlags)
{
	int nX1, nY1, nX2, nY2;
	CPen* pOldPen;

	nX1 = (int)MP_TO_WX(vMins, pView);
	nX2 = (int)MP_TO_WX(vMaxs, pView);
	nY1 = (int)MP_TO_WY(vMins, pView);
	nY2 = (int)MP_TO_WY(vMaxs, pView);

	pOldPen = m_pBitmapDC->SelectObject(GetColorTable()->GetPen(nDrawColor));

	// Draw the 4 sides.
	m_pBitmapDC->MoveTo(nX1, nY1);
	m_pBitmapDC->LineTo(nX2, nY1);
	m_pBitmapDC->LineTo(nX2, nY2);
	m_pBitmapDC->LineTo(nX1, nY2);
	m_pBitmapDC->LineTo(nX1, nY1);

	m_pBitmapDC->SelectObject(pOldPen);
}

void CGDIWindowRenderer::DrawObject(CMapView* pView, CTread3DDoc* pDocument, CDC* pDC, CBaseObject* pObject, int nObjectClass, int nObjectInterfaces, int nDrawFlags, int nFlags)
{
	// Allowing plugins to intercept drawing is done on a per renderer basis.
	// We allow it.
	if(GetPluginAPI()->m_n2DPreDrawFlags&nObjectClass)
	{
		if(GetPluginAPI()->PreDrawObject(pView, pDocument, this, m_pBitmapDC, m_pBitmapDC, pObject, nObjectClass, nObjectInterfaces, nDrawFlags, nFlags, PDT_2D, GDIWINDOWRENDERER_UID))
			return;
	}

	// A brush?
	switch(nObjectClass)
	{
	case OBJECT_CLASS_BRUSH:
			m_pBR->RenderBrush(pView, m_pBitmapDC, (CQBrush*)pObject, nDrawFlags, nFlags);
			break;
	case OBJECT_CLASS_ENTITY:
	case OBJECT_CLASS_LENTITY:
			m_pER->RenderEntity(pView, pDocument, m_pBitmapDC, (CEntity*)pObject, nDrawFlags, nFlags);
			break;
	case OBJECT_CLASS_CAMERA:
			m_pCR->RenderCamera(pView, m_pBitmapDC, (CCamera*)pObject, nDrawFlags, nFlags);
			break;
	case OBJECT_CLASS_ANCHOR:
			m_pAR->RenderAnchor(pView, m_pBitmapDC, (CAnchor*)pObject, nDrawFlags, nFlags);
			break;
	case OBJECT_CLASS_CLIPTOOL:
			m_pCTR->RenderClipTool(pView, m_pBitmapDC, pDocument, (CClipTool*)pObject, nDrawFlags, nFlags);
			break;		
	}

	if(GetPluginAPI()->m_n2DPostDrawFlags&nObjectClass)
		GetPluginAPI()->PostDrawObject(pView, pDocument, this, m_pBitmapDC, m_pBitmapDC, pObject, nObjectClass, nObjectInterfaces, nDrawFlags, nFlags, PDT_2D, GDIWINDOWRENDERER_UID);
}

void CGDIWindowRenderer::ClearWindow(CMapView* pView, CTread3DDoc* pDocument, CDC* pDC, int nClearColor, int nFlags)
{
	m_pBitmapDC->FillSolidRect(&pView->m_rcClientRect, GetColorTable()->GetColor(nClearColor));
}

void CGDIWindowRenderer::DrawGrid(CMapView* pView, CTread3DDoc* pDocument, CDC* pDC, int nFlags)
{
	float fX, fY;
	float fXStart, fYStart;
	float fWX, fWY;
	float fGridStepX, fGridStepY;
	int nGridSize = pDocument->GetGridSize();
	CPen* pOldPen, *pOrigPen, *pGridPen;

	pOrigPen = GetColorTable()->GetPen(CL_GRID_ORIG);
	pGridPen = GetColorTable()->GetPen(CL_GRID);
	pOldPen = m_pBitmapDC->SelectObject(pGridPen);

	fXStart = VMMIN_X(pView);
	if(fXStart < 0)
		fXStart += -(float)(((int)VMMIN_X(pView)) % nGridSize);
	else
		fXStart -= (float)(((int)VMMIN_X(pView)) % nGridSize);

	fYStart = VMMIN_Y(pView);
	if(fYStart < 0)
		fYStart += -(float)(((int)VMMIN_Y(pView)) % nGridSize);
	else
		fYStart -= (float)(((int)VMMIN_Y(pView)) % nGridSize);

	if(VO(VT(pView), VEC_X) > 0)
	{
		fGridStepX = (float)nGridSize;
		for(fX = fXStart; fX < pView->m_vMapMaxs[0]; fX += fGridStepX)
		{
			fWX = MX_TO_WX(fX, pView);
			m_pBitmapDC->MoveTo((int)fWX, 0);
			m_pBitmapDC->LineTo((int)fWX, pView->m_rcClientRect.bottom);
		}
	}
	else
	{
		fGridStepX = (float)-nGridSize;
		for(fX = fXStart; fX > pView->m_vMapMaxs[0]; fX += fGridStepX)
		{
			fWX = MX_TO_WX(fX, pView);
			m_pBitmapDC->MoveTo((int)fWX, 0);
			m_pBitmapDC->LineTo((int)fWX, pView->m_rcClientRect.bottom);
		}
	}

	if(VO(VT(pView), VEC_Y) > 0)
	{
		fGridStepY = (float)nGridSize;
		for(fY = fYStart; fY < pView->m_vMapMaxs[1]; fY += fGridStepY)
		{
			fWY = MY_TO_WY(fY, pView);
			m_pBitmapDC->MoveTo(0, (int)fWY);
			m_pBitmapDC->LineTo(pView->m_rcClientRect.right, (int)fWY);
		}
	}
	else
	{
		fGridStepY = (float)-nGridSize;
		for(fY = fYStart; fY > pView->m_vMapMaxs[1]; fY += fGridStepY)
		{
			fWY = MY_TO_WY(fY, pView);
			m_pBitmapDC->MoveTo(0, (int)fWY);
			m_pBitmapDC->LineTo(pView->m_rcClientRect.right, (int)fWY);
		}
	}

	// Draw origin lines.
	fWX = MX_TO_WX(0, pView);
	fWY = MY_TO_WY(0, pView);

	m_pBitmapDC->SelectObject(pOrigPen);
	m_pBitmapDC->MoveTo((int)fWX, 0);
	m_pBitmapDC->LineTo((int)fWX, pView->m_rcClientRect.bottom);
	m_pBitmapDC->MoveTo(0, (int)fWY);
	m_pBitmapDC->LineTo(pView->m_rcClientRect.right, (int)fWY);

	m_pBitmapDC->SelectObject(pOldPen);
}
#endif
