// GLWINDOWRENDERER.CPP
// Copyright © 1999 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#include "stdafx.h"
#include "GLRenderPackage.h"
#include "GLWindowRenderer.h"
#include "TreadSys.h"
#include "ColorTable.h"
#include "GLBrushRenderer.h"
#include "GLCameraRenderer.h"
#include "MapView.h"
#include "HotSpotManager.h"
#include "GDIWindowRenderer.h"
#include "GLEntityRenderer.h"
#include "GLClipToolRenderer.h"
#include "GLAnchorRenderer.h"
#include "GLPatchMeshRenderer.h"
#include "TargetAPI.h"
#include "PluginAPI.h"

#define INIT_LINE_SIZE	1024
#define VIEWFONT_LIST_BASE	1000
#define BBOXFONT_LIST_BASE	2000

#ifdef _WIN32
static PIXELFORMATDESCRIPTOR gGlPFD = 
{
	sizeof( PIXELFORMATDESCRIPTOR ),
	1,
	PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
	PFD_TYPE_RGBA,
	16,
	0, 0, 0, 0, 0, 0,
	0,
	0,
	0,
	0, 0, 0, 0,
	16,                // 16-bit depth buffer
	0,                  // no stencil buffer
	0,                  // no aux buffers
	0,			/* main layer */
	0,	
	0, 0, 0
};
#endif

// Get's the line that was hit.
CBaseObject* CGLWindowRenderer::GetHitObject(CMapView* pView, CTread3DDoc* pDocument, int nClassFlags, int nMouseX, int nMouseY)
{
	int i;
	GDIWINDOWLINE* pLine;
	
	for(i = m_nNextLine-1; i >= 0; i--)
	{
		pLine = &m_pLines[i];

		// ???: This needs to be fixed.
		if(pLine->m_pObject == NULL)
			continue;

		// Skip if not correct type.
		if((pLine->m_pObject->GetObjectClass()&nClassFlags)==0)
			continue;
		pLine->BuildLine();
		if(pLine->Hit((float)nMouseX, (float)nMouseY))
			return pLine->m_pObject;
	}

	return NULL;
}

GDIWINDOWLINE* CGLWindowRenderer::GetWindowLine()
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
	return &m_pLines[m_nNextLine-1];
}

bool CGLWindowRenderer::ClipLine(CMapView* pView, vec_t* vPta, vec_t* vPtb)
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

void CGLWindowRenderer::DrawI_TARGET(CMapView* pView, CTread3DDoc* pDocument, CDC* pDC, CBaseObject* pObject, int nDrawFlags, int nType)
{
	CBaseTargetObject* pT = (CBaseTargetObject*)pObject->GetInterface(I_TARGET);
	CBaseTransformableObject* pSrcTR = (CBaseTransformableObject*)pObject->GetInterface(I_TRANSFORMABLE);
	CString sName = pT->GetTargetName();
	if(sName == "")
		return;
	TTARGET* pTSet = pDocument->GetTargetAPI()->FindTargetSet(sName);
	if(pTSet == NULL)
		return;

	m_pGLFT->glColor3fv(GetColorTable()->GetFloatV(CL_LINK));
	m_pGLFT->glBegin(GL_LINES);

	TREF* pRef;
	pTSet->TRefList.First();
	for(pRef = pTSet->TRefList.GetCurItem(); pRef != NULL; pRef = pTSet->TRefList.GetNextItem())
	{
		CBaseTransformableObject* pDstTR = (CBaseTransformableObject*)pRef->pObject->GetInterface(I_TRANSFORMABLE);

		// Draw the line.
		m_pGLFT->glVertex2i(pDstTR->m_vOrigin[VI(VEC_X, pView)], pDstTR->m_vOrigin[VI(VEC_Y, pView)]);
		m_pGLFT->glVertex2i(pSrcTR->m_vOrigin[VI(VEC_X, pView)], pSrcTR->m_vOrigin[VI(VEC_Y, pView)]);
	}

	m_pGLFT->glEnd();
	m_pGLFT->glColor3f(1.0F, 1.0F, 1.0F);
}

// Generates the pixel format.
bool CGLWindowRenderer::GeneratePixelFormat(CDC* pDC)
{
#ifdef _WIN32
	// Select the correct pixel format.
	m_nPixelFormat = ChoosePixelFormat(pDC->m_hDC, &gGlPFD);

	return m_nPixelFormat != 0;
#else
	return true;
#endif
}

// Set's the GL pixel format.
bool CGLWindowRenderer::SetPixelFormat(CDC* pDC)
{
#ifdef _WIN32
	if(!::SetPixelFormat(pDC->m_hDC, m_nPixelFormat, &gGlPFD))
		return(false);
#endif
	return true;
}

void CGLWindowRenderer::DrawBBox(CMapView* pView, CTread3DDoc* pDocument, CDC* pDC, vec3_t vMins, vec3_t vMaxs, int nDrawColor, int nFlags)
{
	int nX[2], nY[2];

	m_pGLFT->glColor3fv(GetColorTable()->GetFloatV(nDrawColor));

	nX[0] = (int)vMins[VI(VEC_X, pView)];
	nX[1] = (int)vMaxs[VI(VEC_X, pView)];
	nY[0] = (int)vMins[VI(VEC_Y, pView)];
	nY[1] = (int)vMaxs[VI(VEC_Y, pView)];

	m_pGLFT->glBegin(GL_LINES);
	m_pGLFT->glVertex2i(nX[0], nY[0]);
	m_pGLFT->glVertex2i(nX[1], nY[0]);
	m_pGLFT->glVertex2i(nX[1], nY[0]);
	m_pGLFT->glVertex2i(nX[1], nY[1]);
	m_pGLFT->glVertex2i(nX[1], nY[1]);
	m_pGLFT->glVertex2i(nX[0], nY[1]);
	m_pGLFT->glVertex2i(nX[0], nY[1]);
	m_pGLFT->glVertex2i(nX[0], nY[0]);
	m_pGLFT->glEnd();
}

#define CHAR_WIDTH	7
#define CHAR_HEIGHT 12
void CGLWindowRenderer::DrawBBoxSize(CMapView* pView, CTread3DDoc* pDocument, CDC* pDC, vec3_t vSelMins, vec3_t vSelMaxs, vec3_t vCenter, int nDrawColor)
{
	char szBuff[256];

	// These aren't guarenteed to be ordered correctly.
	vec3_t vMins, vMaxs, vSize;
	VectorCopy(vSelMins, vMins);
	VectorCopy(vSelMaxs, vMaxs);
	// ... So order them.
	OrderRange(vMins, vMaxs);
	VectorSubtract(vMaxs, vMins, vSize);

	// Save old projection matrix.
	m_pGLFT->glPushMatrix();
	m_pGLFT->glLoadIdentity();
	m_pGLFT->gluOrtho2D(0, pView->m_rcClientRect.right, pView->m_rcClientRect.bottom, 0);
	m_pGLFT->glListBase(BBOXFONT_LIST_BASE);
	m_pGLFT->glColor3fv(GetColorTable()->GetFloatV(CL_BBOX_SIZE));

	// X's
	if(vSize[VI(VEC_X, pView)] > 0.0F)
	{
		CString sWidth = itoa((int)vSize[VI(VEC_X, pView)], szBuff, 10);
		int nSize = sWidth.GetLength() * CHAR_WIDTH;
		int nY1 = MY_TO_WY(vMins[VI(VEC_Y, pView)], pView) - VO(VT(pView), VEC_Y) * 4;
		int nX1 = MX_TO_WX(vCenter[VI(VEC_X, pView)], pView) - (nSize>>1);
		DrawString(nX1, nY1+10, sWidth);

		nY1 = MY_TO_WY(vMaxs[VI(VEC_Y, pView)], pView) + VO(VT(pView), VEC_Y) * 16;
		DrawString(nX1, nY1+10, sWidth);
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
			DrawString(nX1-nSize-5, nY1+10, sHeight);
			DrawString(nX2+5, nY1+10, sHeight);
		}
		else
		{
			DrawString(nX1+5, nY1+10, sHeight);
			DrawString(nX2-nSize-5, nY1+10, sHeight);
		}
	}

	// Restore render matrix.
	m_pGLFT->glPopMatrix();

}

#define LITTLEX_SIZE	6
void CGLWindowRenderer::DrawLittleX(CMapView* pView, CTread3DDoc* pDocument, CDC* pDC, vec2_t vMapPos, int nColor)
{
	vec3_t vP[4];

	m_pGLFT->glColor3fv(GetColorTable()->GetFloatV(nColor));

	vP[0][0] = vP[3][0] = WX_TO_MX(MX_TO_WX(vMapPos[0], pView) - (LITTLEX_SIZE), pView);
	vP[1][0] = vP[2][0] = WX_TO_MX(MX_TO_WX(vMapPos[0], pView) + (LITTLEX_SIZE), pView);
	vP[0][1] = vP[1][1] = WY_TO_MY(MY_TO_WY(vMapPos[1], pView) - (LITTLEX_SIZE), pView);
	vP[2][1] = vP[3][1] = WY_TO_MY(MY_TO_WY(vMapPos[1], pView) + (LITTLEX_SIZE), pView);

	vP[0][2] = vP[1][2] = vP[2][2] = vP[3][2] = 0.0f;

	m_pGLFT->glBegin(GL_LINES);
	m_pGLFT->glVertex3fv(vP[0]);
	m_pGLFT->glVertex3fv(vP[2]);
	m_pGLFT->glVertex3fv(vP[1]);
	m_pGLFT->glVertex3fv(vP[3]);
	m_pGLFT->glEnd();
}

#define HOTSPOT_SIZE	4
void CGLWindowRenderer::DrawHotSpot(CMapView* pView, CTread3DDoc* pDocument, CDC* pDC, CHotSpot* pSpot, int nDrawFlags, int nFlags)
{
	vec3_t vP[4];

	// Compute the window coords.
	pSpot->ComputeWindowXY(pView);
	if(pSpot->m_nDrawFlags&HSDF_NODRAW)
		return;

	if(pSpot->m_nDrawFlags&HSDF_LINE)
	{
		if(pSpot->m_pLineSpot != NULL)
		{
			m_pGLFT->glColor3fv(GetColorTable()->GetFloatV(CL_HOTSPOT_LINE));
			m_pGLFT->glBegin(GL_LINES);
			{
				m_pGLFT->glVertex2i(WX_TO_MX(pSpot->m_nWX, pView), WY_TO_MY(pSpot->m_nWY, pView));
				pSpot->m_pLineSpot->ComputeWindowXY(pView);
				m_pGLFT->glVertex2i(WX_TO_MX(pSpot->m_pLineSpot->m_nWX, pView), WY_TO_MY(pSpot->m_pLineSpot->m_nWY, pView));

			}
			m_pGLFT->glEnd();
		}
	}

	if(nDrawFlags&RF_SELECTED)
		m_pGLFT->glColor3fv(GetColorTable()->GetFloatV(CL_HOTSPOT_SEL));
	else if(pSpot->m_nDrawFlags&HSDF_HD_COLOR)
		m_pGLFT->glColor3fv(GetColorTable()->GetFloatV(CL_BRUSH_HANDLE));
	else
		m_pGLFT->glColor3fv(GetColorTable()->GetFloatV(CL_HOTSPOT));

	// GL only does square and cross.
	if(pSpot->m_nDrawFlags&HSDF_CROSS)
	{
		vP[0][0] = vP[3][0] = WX_TO_MX(MX_TO_WX(pSpot->m_fMX, pView) - (HOTSPOT_SIZE), pView);
		vP[1][0] = vP[2][0] = WX_TO_MX(MX_TO_WX(pSpot->m_fMX, pView) + (HOTSPOT_SIZE), pView);
		vP[0][1] = vP[1][1] = WY_TO_MY(MY_TO_WY(pSpot->m_fMY, pView) - (HOTSPOT_SIZE), pView);
		vP[2][1] = vP[3][1] = WY_TO_MY(MY_TO_WY(pSpot->m_fMY, pView) + (HOTSPOT_SIZE), pView);

		vP[0][2] = vP[1][2] = vP[2][2] = vP[3][2] = 0.0f;


		m_pGLFT->glBegin(GL_LINES);
		m_pGLFT->glVertex3fv(vP[0]);
		m_pGLFT->glVertex3fv(vP[2]);
		m_pGLFT->glVertex3fv(vP[1]);
		m_pGLFT->glVertex3fv(vP[3]);
		m_pGLFT->glEnd();
	}
	else
	{
		vP[0][0] = vP[3][0] = WX_TO_MX(MX_TO_WX(pSpot->m_fMX, pView) - (HOTSPOT_SIZE>>1), pView);
		vP[1][0] = vP[2][0] = WX_TO_MX(MX_TO_WX(pSpot->m_fMX, pView) + (HOTSPOT_SIZE>>1), pView);
		vP[0][1] = vP[1][1] = WY_TO_MY(MY_TO_WY(pSpot->m_fMY, pView) - (HOTSPOT_SIZE>>1), pView);
		vP[2][1] = vP[3][1] = WY_TO_MY(MY_TO_WY(pSpot->m_fMY, pView) + (HOTSPOT_SIZE>>1), pView);

		vP[0][2] = vP[1][2] = vP[2][2] = vP[3][2] = 0.0f;

		m_pGLFT->glBegin(GL_QUADS);
		m_pGLFT->glVertex3fv(vP[0]);
		m_pGLFT->glVertex3fv(vP[1]);
		m_pGLFT->glVertex3fv(vP[2]);
		m_pGLFT->glVertex3fv(vP[3]);
		m_pGLFT->glEnd();

	}
}

void CGLWindowRenderer::DrawObject(CMapView* pView, CTread3DDoc* pDocument, CDC* pDC, CBaseObject* pObject, int nObjectClass, int nObjectInterfaces, int nDrawFlags, int nFlags)
{
	// Allowing plugins to intercept drawing is done on a per renderer basis.
	// We allow it.
	if(GetPluginAPI()->m_n2DPreDrawFlags&nObjectClass)
	{
		if(GetPluginAPI()->PreDrawObject(pView, pDocument, this, pDC, m_pGLFT, pObject, nObjectClass, nObjectInterfaces, nDrawFlags, nFlags, PDT_2D, GLWINDOWRENDERER_UID))
			return;
	}

	switch(nObjectClass)
	{
	case OBJECT_CLASS_BRUSH:
		m_pBR->RenderBrush((CQBrush*)pObject, pView, nDrawFlags, nFlags);
		return;
	case OBJECT_CLASS_CAMERA:
		m_pCR->RenderCamera((CCamera*)pObject, pView, nDrawFlags, nFlags);
		return;
	case OBJECT_CLASS_ENTITY:
	case OBJECT_CLASS_LENTITY:
		m_pER->RenderEntity((CEntity*)pObject, pView, nDrawFlags, nFlags);
		return;
	case OBJECT_CLASS_CLIPTOOL:
		m_pCTR->RenderClipTool((CClipTool*)pObject, pDC, pView, pDocument, nDrawFlags, nFlags);
		return;
	case OBJECT_CLASS_ANCHOR:
		m_pAR->RenderAnchor((CAnchor*)pObject, pView, nDrawFlags, nFlags);
		return;
	case OBJECT_CLASS_PATCHMESH:
		m_pMR->RenderPatchMesh((CPatchMesh*)pObject, pView, nDrawFlags);
		return;
	}
	if(GetPluginAPI()->m_n2DPostDrawFlags&nObjectClass)
		GetPluginAPI()->PostDrawObject(pView, pDocument, this, pDC, m_pGLFT, pObject, nObjectClass, nObjectInterfaces, nDrawFlags, nFlags, PDT_2D, GLWINDOWRENDERER_UID);
}

void CGLWindowRenderer::OnSize(CMapView* pView, CDC* pDC)
{
}

void CGLWindowRenderer::ClearWindow(CMapView* pView, CTread3DDoc* pDocument, CDC* pDC, int nClearColor, int nFlags)
{
	m_pGLFT->glClearColor(GetColorTable()->GetFloatV(CL_FLAT_BACKGROUND)[0], GetColorTable()->GetFloatV(CL_FLAT_BACKGROUND)[1], GetColorTable()->GetFloatV(CL_FLAT_BACKGROUND)[2], 0.5F);
	m_pGLFT->glClear(GL_COLOR_BUFFER_BIT);
}

void CGLWindowRenderer::DrawGrid(CMapView* pView, CTread3DDoc* pDocument, CDC* pDC, int nFlags)
{
	int nX, nY;
	int nXStart, nYStart;
	int nGridStepX, nGridStepY;
	int nGridSize = pDocument->GetGridSize();
	float* vNormal, *vOrigin;

	vNormal = GetColorTable()->GetFloatV(CL_GRID);
	vOrigin = GetColorTable()->GetFloatV(CL_GRID_ORIG);

	m_pGLFT->glColor3fv(vNormal);

	nXStart = (int)VMMIN_X(pView);
	if(nXStart < 0)
		nXStart += -(int)(((int)VMMIN_X(pView)) % nGridSize);
	else
		nXStart -= (int)(((int)VMMIN_X(pView)) % nGridSize);

	nYStart = (int)VMMIN_Y(pView);
	if(nYStart < 0)
		nYStart += -(int)(((int)VMMIN_Y(pView)) % nGridSize);
	else
		nYStart -= (int)(((int)VMMIN_Y(pView)) % nGridSize);
	
	m_pGLFT->glBegin(GL_LINES);
	{
		if(VO(VT(pView), VEC_X) > 0)
		{
			nGridStepX = (int)nGridSize;
			for(nX = nXStart; nX < pView->m_vMapMaxs[0]; nX += nGridStepX)
			{
				/*m_pBitmapDC->MoveTo((int)fWX, 0);
				m_pBitmapDC->LineTo((int)fWX, pView->m_rcClientRect.bottom);*/
				m_pGLFT->glVertex2i(nX, VMMIN_Y(pView));
				m_pGLFT->glVertex2i(nX, VMMAX_Y(pView));
			}
		}
		else
		{
			nGridStepX = (int)-nGridSize;
			for(nX = nXStart; nX > pView->m_vMapMaxs[0]; nX += nGridStepX)
			{
				/*m_pBitmapDC->MoveTo((int)fWX, 0);
				m_pBitmapDC->LineTo((int)fWX, pView->m_rcClientRect.bottom);*/
				m_pGLFT->glVertex2i(nX, VMMIN_Y(pView));
				m_pGLFT->glVertex2i(nX, VMMAX_Y(pView));

			}
		}

		if(VO(VT(pView), VEC_Y) > 0)
		{
			nGridStepY = (int)nGridSize;
			for(nY = nYStart; nY < pView->m_vMapMaxs[1]; nY += nGridStepY)
			{
				/*m_pBitmapDC->MoveTo(0, (int)fWY);
				m_pBitmapDC->LineTo(pView->m_rcClientRect.right, (int)fWY);*/
				m_pGLFT->glVertex2i(VMMIN_X(pView), nY);
				m_pGLFT->glVertex2i(VMMAX_X(pView), nY);
			}
		}
		else
		{
			nGridStepY = (int)-nGridSize;
			for(nY = nYStart; nY > pView->m_vMapMaxs[1]; nY += nGridStepY)
			{
				/*m_pBitmapDC->MoveTo(0, (int)fWY);
				m_pBitmapDC->LineTo(pView->m_rcClientRect.right, (int)fWY);*/
				m_pGLFT->glVertex2i(VMMIN_X(pView), nY);
				m_pGLFT->glVertex2i(VMMAX_X(pView), nY);
			}
		}
	}

	m_pGLFT->glColor3fv(vOrigin);
	m_pGLFT->glVertex2i(VMMIN_X(pView), 0);
	m_pGLFT->glVertex2i(VMMAX_X(pView), 0);
	m_pGLFT->glVertex2i(0, VMMIN_Y(pView));
	m_pGLFT->glVertex2i(0, VMMAX_Y(pView));

	m_pGLFT->glEnd();
}

// Initializes the rendering window.
bool CGLWindowRenderer::Initialize(CMapView* pView, CDC* pDC, int nType)
{
	Debug("glwindowrenderer init\n");
	Debug("... making pixel format\n");

	if(!GeneratePixelFormat(pDC))
		return false;

	Debug("... setting pixel format\n");

	// Set the pixel format.
	SetPixelFormat(pDC);

#ifdef _WIN32
	Debug("... making context\n");

	// Create the rendering context.
	m_hglrc = m_pGLFT->e_wglCreateContext(pDC->m_hDC);
	if(m_hglrc == NULL)
		return false;

	Debug("... binding context\n");

	m_pGLFT->e_wglMakeCurrent(pDC->m_hDC, m_hglrc);

#endif

	Debug("... seting gl defaults\n");

	// Set Initial states.
	m_pGLFT->glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	m_pGLFT->glClearColor(0.0F, 0.0F, 0.0F, 0.0F);
	m_pGLFT->glShadeModel(GL_FLAT);

	m_pGLFT->glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
   	
#ifdef _WIN32

	// Lock and attach fonts.
	CFont BBoxSizeFont;
	CFont ViewTypeFont;

	BBoxSizeFont.CreateFont(12, 0, 0, 0, 200, false, false, false, 
		ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, 
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, "MS Sans Serif");
	
	ViewTypeFont.CreateFont(14, 0, 0, 0, 200, false, false, false, 
		ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, 
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, "MS Sans Serif");

	// Build first one.
	CFont* pOldFont = pDC->SelectObject(&BBoxSizeFont);
	m_pGLFT->e_wglUseFontBitmapsA(pDC->m_hDC, 0, 255, BBOXFONT_LIST_BASE);
	pDC->SelectObject(&ViewTypeFont);
	m_pGLFT->e_wglUseFontBitmapsA(pDC->m_hDC, 0, 255, VIEWFONT_LIST_BASE);
	pDC->SelectObject(pOldFont);

#endif

	// Initialize the entity renderer.
	m_pER->Initialize(pView, pDC);

	Debug("... vendor: %s\n", m_pGLFT->glGetString(GL_VENDOR));
	Debug("... renderer: %s\n", m_pGLFT->glGetString(GL_RENDERER));
	Debug("... version: %s\n", m_pGLFT->glGetString(GL_VERSION));
	Debug("done\n");

#ifdef _WIN32
	m_pGLFT->e_wglMakeCurrent(NULL, NULL);
#endif

	return true;
}

bool CGLWindowRenderer::Shutdown(CMapView* pView, CDC* pDC)
{
	return true;
}

void CGLWindowRenderer::DrawString(int nX, int nY, CString sString)
{
	m_pGLFT->glRasterPos2i(nX, nY);
	m_pGLFT->glCallLists(sString.GetLength(), GL_UNSIGNED_BYTE, sString);
}

bool CGLWindowRenderer::LockDevice(CMapView* pView, CTread3DDoc* pDocument, CDC* pDC, int nDrawFlags)
{
	if((nDrawFlags&RF_FAST)==0)
		m_nNextLine = 0;

#ifdef _WIN32
	if(m_hglrc == NULL)
		return false;

	if(!m_pGLFT->e_wglMakeCurrent(pDC->m_hDC, m_hglrc))
		return false;
#endif

	// Pick mode?
	if(nDrawFlags&RF_PICKMODE)
	{
	}
	else
	{
		m_pGLFT->glColor3f(1.0F, 1.0F, 1.0F);
		m_pGLFT->glRenderMode(GL_RENDER);
	}

	// Use an orthographic view projection.
	m_pGLFT->glMatrixMode(GL_PROJECTION);
	m_pGLFT->glLoadIdentity();
	m_pGLFT->glViewport(0, 0, pView->m_rcClientRect.right, pView->m_rcClientRect.bottom);
	m_pGLFT->gluOrtho2D(pView->m_vMapMins[0], pView->m_vMapMaxs[0], pView->m_vMapMaxs[1], pView->m_vMapMins[1]);

	return true;
}

bool CGLWindowRenderer::UnlockDevice(CMapView* pView, CTread3DDoc* pDocument, CDC* pDC, int nDrawFlags)
{
#ifdef _WIN32
	return m_pGLFT->e_wglMakeCurrent(NULL, NULL);
#else
	return true;
#endif
}

void CGLWindowRenderer::Flush(CMapView* pView, CTread3DDoc* pDocument, CDC* pDC)
{
	m_pGLFT->glLoadIdentity();
	m_pGLFT->gluOrtho2D(0, pView->m_rcClientRect.right, pView->m_rcClientRect.bottom, 0);
	m_pGLFT->glListBase(VIEWFONT_LIST_BASE);
	if(pView->m_bFocus)
		m_pGLFT->glColor3fv(GetColorTable()->GetFloatV(CL_VIEW_SELECTED));
	else
		m_pGLFT->glColor3fv(GetColorTable()->GetFloatV(CL_VIEW_NORMAL));
	DrawString(5, 13, pView->m_sView);
	m_pGLFT->glFlush();
	
#ifdef _WIN32
	SwapBuffers(pDC->m_hDC);
#endif
}

CGLWindowRenderer::CGLWindowRenderer(WOGLFUNCTIONTABLE* pGLFT)
{
	// Get the GL function table.
	m_pGLFT = pGLFT;
#ifdef _WIN32
	m_hglrc = NULL;
#endif

	m_pBR = new CGLBrushRenderer(this, pGLFT);
	m_pCR = new CGLCameraRenderer(this, pGLFT);
	m_pER = new CGLEntityRenderer(this, pGLFT);
	m_pCTR = new CGLClipToolRenderer(this, pGLFT);
	m_pAR = new CGLAnchorRenderer(this, pGLFT);
	m_pMR = new CGLPatchMeshRenderer(pGLFT);
	m_pLines = new GDIWINDOWLINE[INIT_LINE_SIZE];
	m_nNumLines = INIT_LINE_SIZE;
	m_nNextLine = 0;
}

CGLWindowRenderer::~CGLWindowRenderer()
{
#ifdef _WIN32
	m_pGLFT->e_wglMakeCurrent(NULL, NULL);
	if(m_hglrc != NULL)
		m_pGLFT->e_wglDeleteContext(m_hglrc);
	m_hglrc = NULL;
#endif

	delete[] m_pLines;

	delete m_pBR;
	delete m_pCR;
	delete m_pCTR;
	delete m_pER;
	delete m_pAR;
	delete m_pMR;
}

