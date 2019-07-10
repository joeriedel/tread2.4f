// GDIWINDOWRENDERER.H
// Copyright © 1999 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#ifndef GDIWINDOWRENDERER_H
#define GDIWINDOWRENDERER_H

#include "RenderAPI.h"

// $sb "gdiwindowrenderer.h block1"

#define GWLF_NONE	0x00000000
#define GWLF_VERT	0x00000001
#define GWLF_HORZ	0x00000002
#define GWLF_RANGE	0x00000004

class CBaseObject;
struct GDIWINDOWLINE
{
	float m_fMinX, m_fMaxX;
	float m_fMinY, m_fMaxY;
	float m_fX1, m_fX2;
	float m_fY1, m_fY2;
	float m_fB, m_fSlope;
	int m_nFlags;
	bool m_bBuild;
	CBaseObject* m_pObject;

	GDIWINDOWLINE();
	void BuildLine();
	bool Hit(float fX, float fY);
};

#ifdef _WIN32
class CGDIRenderPackage;
class CGDIBrushRenderer;
class CGDIEntityRenderer;
class CGDICameraRenderer;
class CGDIClipToolRenderer;
class CGDIAnchorRenderer;

class _T_EXPORT CGDIWindowRenderer : public CBaseWindowRenderer
{
protected:

	CDC* m_pBitmapDC;
	HBITMAP m_hBitmap, m_hOldBitmap;
	CGDIBrushRenderer* m_pBR;
	CGDIEntityRenderer* m_pER;
	CGDICameraRenderer* m_pCR;
	CGDIClipToolRenderer* m_pCTR;
	CGDIAnchorRenderer* m_pAR;
	CFont BBoxSizeFont;
	CFont ViewTypeFont;

	int m_nNumLines;
	int m_nNextLine;
	GDIWINDOWLINE* m_pLines;

	bool CreateDib24(CMapView* pView, CDC* pDC);
	bool DeleteDib24();

public:

	CGDIWindowRenderer();
	virtual ~CGDIWindowRenderer();

	// Initializes the rendering interface.
	virtual bool Initialize(CMapView* pView, CDC* pDC, int nType);
	virtual bool Shutdown(CMapView* pView, CDC* pDC);
	virtual int GetRendererUID() { return GDIWINDOWRENDERER_UID; }

	virtual bool LockDevice(CMapView* pView, CTread3DDoc* pDocument, CDC* pDC, int nDrawFlags);
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

	friend class CGDIRenderPackage;
};

#endif
// $eb

#endif