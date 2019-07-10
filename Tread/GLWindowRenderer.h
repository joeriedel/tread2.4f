// GLWINDOWRENDERER.H
// Copyright © 1999 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#ifndef GLWINDOWRENDERER_H
#define GLWINDOWRENDERER_H

#include "RenderAPI.h"
#include "GLRenderPackage.h"
#include "Texture.h"
#include "GDIWindowRenderer.h"

// $sb "glwindowrenderer.h block1"

class CGLFaceRenderer;
class CGLBrushRenderer;
class CGLCameraRenderer;
class CGLEntityRenderer;
class CGLClipToolRenderer;
class CGLAnchorRenderer;
class CGLPatchMeshRenderer;

class _T_EXPORT CGLWindowRenderer : public CBaseWindowRenderer
{
private:

#ifdef _WIN32
	int m_nPixelFormat;
	HGLRC m_hglrc;
	// Generates the pixel format.
	
#endif

	bool GeneratePixelFormat(CDC* pDC);
	bool SetPixelFormat(CDC* pDC);
	WOGLFUNCTIONTABLE* m_pGLFT;

	CGLBrushRenderer* m_pBR;
	CGLCameraRenderer* m_pCR;
	CGLEntityRenderer* m_pER;
	CGLClipToolRenderer* m_pCTR;
	CGLAnchorRenderer* m_pAR;
	CGLPatchMeshRenderer* m_pMR;

	// We elicit some GDI routines to keep track of drawing. We do this
	// because GL's hit detection does not allow us to give the user some leeway.
	// The GDI renderer can tell if a user clicked "close" to a line. The GL pick
	// only tells if it was clicked on that exact pixel.
	int m_nNumLines;
	int m_nNextLine;
	GDIWINDOWLINE* m_pLines;

public:

	CGLWindowRenderer(WOGLFUNCTIONTABLE* pGLFT);
	virtual ~CGLWindowRenderer();

	// Initializes the rendering interface.
	virtual bool Initialize(CMapView* pView, CDC* pDC, int nType);
	virtual bool Shutdown(CMapView* pView, CDC* pDC);
	virtual int GetRendererUID() { return GLWINDOWRENDERER_UID; }

	void DrawString(int nX, int nY, CString sString);

	virtual bool LockDevice(CMapView* pView, CTread3DDoc* pDocument, CDC* pDC, int nDrawFlags);
	virtual bool UnlockDevice(CMapView* pView, CTread3DDoc* pDocument, CDC* pDC, int nDrawFlags);
	
	virtual void OnSize(CMapView* pView, CDC* pDC);
	virtual void ClearWindow(CMapView* pView, CTread3DDoc* pDocument, CDC* pDC, int nClearColor, int nFlags=0);
	virtual void DrawBBox(CMapView* pView, CTread3DDoc* pDocument, CDC* pDC, vec3_t vMins, vec3_t vMaxs, int nDrawColor, int nFlags=0);
	virtual void DrawBBoxSize(CMapView* pView, CTread3DDoc* pDocument, CDC* pDC, vec3_t vMins, vec3_t vMaxs, vec3_t vCenter, int nDrawColor);
	virtual void DrawI_TARGET(CMapView* pView, CTread3DDoc* pDocument, CDC* pDC, CBaseObject* pObject, int nDrawFlags, int nType);
	virtual void DrawLittleX(CMapView* pView, CTread3DDoc* pDocument, CDC* pDC, vec2_t vMapPos, int nColor);

	bool ClipLine(CMapView* pView, vec_t* vPta, vec_t* vPtb);

	virtual void DrawHotSpot(CMapView* pView, CTread3DDoc* pDocument, CDC* pDC, CHotSpot* pSpot, int nDrawFlags, int nFlags);
	virtual void DrawGrid(CMapView* pView, CTread3DDoc* pDocument, CDC* pDC, int nFlags=0);
	virtual void DrawObject(CMapView* pView, CTread3DDoc* pDocument, CDC* pDC, CBaseObject* pObject, int nObjectClass, int nObjectInterfaces, int nDrawFlags, int nFlags=0);
	virtual CBaseObject* GetHitObject(CMapView* pView, CTread3DDoc* pDocument, int nClassFlags, int nMouseX, int nMouseY);
	virtual void Flush(CMapView* pView, CTread3DDoc* pDocument, CDC* pDC);
	GDIWINDOWLINE* GetWindowLine();

	friend class CGLRenderPackage;
};
// $eb
#endif