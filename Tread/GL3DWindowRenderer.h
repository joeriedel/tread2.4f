// GL3DWINDOWRENDERER.H
// Copyright © 1999 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#ifndef GL3DWINDOWRENDERER_H
#define GL3DWINDOWRENDERER_H

#include "RenderAPI.h"
#include "GLRenderPackage.h"
#include "Texture.h"

// $sb "gl3dwindowrenderer.h block1"

class CGL3DBrushRenderer;
class CGL3DBezierPatchRenderer;
class CGL3DEntityRenderer;

#define PICK_BUFF_SIZE	512
#define GL3D_BASE_ALPHA	0.50F

class _T_EXPORT CGL3DWindowRenderer : public CBaseWindowRenderer
{
protected:

	int m_nDownloadCount;
	
	bool m_bBias;
	bool m_bBilinear;
	int m_nBiasDrawFlags;
	int m_nTextureDrawFlags;
	int m_nNumHits;
	int m_nMouseX, m_nMouseY;
	unsigned int m_nCurName;
	unsigned int m_nPickBuff[PICK_BUFF_SIZE];
	CTexture* m_pCurTex;
	bool GeneratePixelFormat(CDC* pDC);
	bool SetPixelFormat(CDC* pDC);
	
#ifdef _WIN32
	HGLRC m_hglrc;
	// Generates the pixel format.
	int m_nPixelFormat;
#endif

	WOGLFUNCTIONTABLE* m_pGLFT;

	// Rendering objects.
	CGL3DBrushRenderer* m_pBR;
	CGL3DEntityRenderer* m_pER;

	
	void MakePowerOf2(CTexture* pTex);

public:

	CGL3DWindowRenderer(WOGLFUNCTIONTABLE* pGLFT);
	virtual ~CGL3DWindowRenderer();

	// Initializes the rendering interface.
	virtual bool Initialize(CMapView* pView, CDC* pDC, int nType);
	virtual bool Shutdown(CMapView* pView, CDC* pDC);
	virtual int GetRendererUID() { return GL3DWINDOWRENDERER_UID; }

	virtual bool LockDevice(CMapView* pView, CTread3DDoc* pDocument, CDC* pDC, int nDrawFlags);
	virtual bool UnlockDevice(CMapView* pView, CTread3DDoc* pDocument, CDC* pDC, int nDrawFlags);
	
	virtual void OnSize(CMapView* pView, CDC* pDC);
	virtual void ClearWindow(CMapView* pView, CTread3DDoc* pDocument, CDC* pDC, int nClearColor, int nFlags=0);
	virtual void DrawBBox(CMapView* pView, CTread3DDoc* pDocument, CDC* pDC, vec3_t vMins, vec3_t vMaxs, int nDrawColor, int nFlags=0);
	virtual void DrawBBoxSize(CMapView* pView, CTread3DDoc* pDocument, CDC* pDC, vec3_t vMins, vec3_t vMaxs, vec3_t vCenter, int nDrawColor){}
	virtual void DrawI_TARGET(CMapView* pView, CTread3DDoc* pDocument, CDC* pDC, CBaseObject* pObject, int nDrawFlags, int nType);

	virtual void DrawGrid(CMapView* pView, CTread3DDoc* pDocument, CDC* pDC, int nFlags=0) {}
	virtual void DrawObject(CMapView* pView, CTread3DDoc* pDocument, CDC* pDC, CBaseObject* pObject, int nObjectClass, int nObjectInterfaces, int nDrawFlags, int nFlags=0);
	virtual CBaseObject* GetHitObject(CMapView* pView, CTread3DDoc* pDocument, int nClassFlags, int nMouseX, int nMouseY);
	virtual void Flush(CMapView* pView, CTread3DDoc* pDocument, CDC* pDC);
	virtual void EnableTransparency(bool bTransparent=true);

	void SetBias(bool bBias, int nDrawFlags=0);
	bool GetBias() { return m_bBias; }
	void BindTexture(CTexture* pTexture, int nDrawFlags, bool bClamp=false);
	void DetachTexture(CTexture* pTexture);

	friend class CGLRenderPackage;
};

// $eb

#endif