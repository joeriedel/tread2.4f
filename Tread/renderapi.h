// RENDERAPI.H
// Copyright © 1999 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#ifndef RENDERAPI_H
#define RENDERAPI_H

#include "MathLib.h"
#include "MapObject.h"
#include "LinkedList.h"
#include "Tread3DDoc.h"
#include "TExport.h"

// Defines the view orientation array.
_T_EXPORT extern float gViewOr[9][2];
_T_EXPORT extern int   gViewIn[5][3];
//extern int	 gUnknown[5];
_T_EXPORT extern int	 g2DIndex[5][3];

#define VMI(ViewType, Axis)	(gViewIn[ViewType][Axis])	// Maps Vector Index.
#define VO(ViewType, Axis)	(gViewOr[ViewType][Axis])	// Get's Vector Orientation.
#define VI(Axis, v)			(VMI(VT(v), Axis))
#define T2I(ViewType, Axis)	(g2DIndex[ViewType][Axis])
#define V2I(Axis, v)		(T2I(v->m_nView, Axis))
#define TU(ViewType)		(gViewIn[ViewType][VEC_Z])
#define VU(v)				(TU(v->m_nView))
#define VMO(Axis, v)		(VO(v->m_nView, Axis))

// $sb "renderapi.h block1"

#define VEC_X				0
#define VEC_Y				1
#define VEC_Z				2

// Macros to index/convert coordinates.
#define VT(v) (v->m_nView)				// View Type.
#define VS(v) (v->m_fScale)				// View Scale.
#define VIS(v) (v->m_fInvScale)			// View Inverse Scale.
#define VWC_X(v) (v->m_vWinCenter[0])	// View WinCenterX.
#define VWC_Y(v) (v->m_vWinCenter[1])	// View WinCenterY.
#define VMC_X(v) (v->m_vMapCenter[0])	// View MapCenterX.
#define VMC_Y(v) (v->m_vMapCenter[1])	// View MapCenterY.
#define VMMIN_X(v) (v->m_vMapMins[0])
#define VMMAX_X(v) (v->m_vMapMaxs[0])
#define VMMIN_Y(v) (v->m_vMapMins[1])
#define VMMAX_Y(v) (v->m_vMapMaxs[1])

#define WX_TO_MX(x, v) (float)((int)(((float)x - VWC_X(v)) * VO(VT(v), VEC_X) * VIS(v) + VMC_X(v)))
#define WY_TO_MY(y, v) (float)((int)(((float)y - VWC_Y(v)) * VO(VT(v), VEC_Y) * VIS(v) + VMC_Y(v)))
#define MX_TO_WX(x, v) (float)((int)(((float)x - VMC_X(v)) * VO(VT(v), VEC_X) * VS(v) + VWC_X(v)))
#define MY_TO_WY(y, v) (float)((int)(((float)y - VMC_Y(v)) * VO(VT(v), VEC_Y) * VS(v) + VWC_Y(v)))
#define WP_TO_MX(p, v) (float)((int)(WX_TO_MX(p[VMI(VT(v), VEC_X)], v)))
#define WP_TO_MY(p, v) (float)((int)(WY_TO_MY(p[VMI(VT(v), VEC_Y)], v)))
#define MP_TO_WX(p, v) (float)((int)(MX_TO_WX(p[VMI(VT(v), VEC_X)], v)))
#define MP_TO_WY(p, v) (float)((int)(MY_TO_WY(p[VMI(VT(v), VEC_Y)], v)))

// Drawing flags.
#define RF_NONE					0x00000000
#define RF_FAST					0x00000001	// Draw fast.
#define RF_SELECTED				0x00000002	// Draw selected.
#define RF_OWNED				0x00000004	// Draw owned.
#define RF_NODRAW				0x00000008	// Don't draw the class.
#define RF_NODRAW_INTERFACES	0x00000010	// Don't draw the objects polymorphic interfaces.
#define RF_TEXTURED				0x00000020	// Texture mode.
#define RF_SOLID				0x00000040	// Solid texturing.
#define RF_LINE					0x00000080	// Line mode.
#define RF_PICKMODE				0x00000100	// Picking.
#define RF_NOHOTSPOTS			0x00000200	// Don't draw hotspots.
#define RF_REALTIME3D			0x00000400	// Don't update 3D view in realtime.
#define RF_NOSELBOX				0x00000800	// Don't draw the selection box.
#define RF_BBOXHATCHED			0x00001000	// Draw hatched bbox.
#define RF_SELBOXONTOP			0x00002000	// Force the selbox on top.
#define RF_NOSELBOXSIZE			0x00004000	// Don't show the selbox size.
#define RF_FORCESELBOX			0x00008000	// Force the selbox to be drawn
#define RF_NOENTNAMES			0x00010000	// Don't draw entity names.
#define RF_NOENTITYFILL			0x00020000	// Don't fill in entities.
#define RF_NOBILINEARFILTER		0x00040000	// Turn off bilinear filtering.
#define RF_NO3DSELBOX			0x00080000	// Turn off the 3d selbox
#define RF_NO3DSELOUTLINE		0x00100000	// Turn off 3d selection outline.
#define RF_DRAWLITTLEX			0x00200000	// Draw's a cute little X on the first selected item :)
#define RF_NO3DVIEW				0x00400000	// Disable the 3D view.
#define RF_NODRAWSEL			0x00800000	// Don't draw things selected.
#define RF_NOTRANSPARENCY		0x01000000	// Don't draw transparent object.
#define RF_NOFOG				0x02000000	// No fog
#define RF_TRANSPARENT			0x04000000	// It's transparent.
#define RF_FOG					0x08000000	// It's foggy.
#define RF_SHADED				0x10000000	// Draw angle shaded.

#define RF_ALL					0xFFFFFFFF

#define RT_TRANSPARENT			0x00000001

#define GLWINDOWRENDERER_UID		0x00000001
#define GL3DWINDOWRENDERER_UID		0x00000002
#define GDIWINDOWRENDERER_UID		0x00000004

class CHotSpot;
class CMapView;
class CBaseRenderPackage;

class _T_EXPORT CBaseWindowRenderer
{
protected:

	vec3_t m_vCamPos, m_vCamRot, m_vCamScale;

public:

	CBaseWindowRenderer() 
	{
		vec3_t v = {1.0F, 1.0F, 1.0F};
		VectorCopy(v, m_vCamScale);
	}
	virtual ~CBaseWindowRenderer() {}

	// Initializes the rendering interface.
	virtual bool Initialize(CMapView* pView, CDC* pDC, int nType) = 0;
	virtual bool Shutdown(CMapView* pView, CDC* pDC) = 0;
	virtual int GetRendererUID() = 0;

	void SetCameraPos(vec3_t vCamPos);
	void SetCameraRot(vec3_t vCamRot);
	void SetCameraScale(vec3_t vCamScale);

	virtual bool LockDevice(CMapView* pView, CTread3DDoc* pDocument, CDC* pDC, int nDrawFlags) { return true; }
	virtual bool UnlockDevice(CMapView* pView, CTread3DDoc* pDocument, CDC* pDC, int nDrawFlags) { return true; }
	virtual void OnSize(CMapView* pView, CDC* pDC)=0;
	virtual void ClearWindow(CMapView* pView, CTread3DDoc* pDocument, CDC* pDC, int nClearColor, int nFlags=0)=0;
	virtual void DrawBBox(CMapView* pView, CTread3DDoc* pDocument, CDC* pDC, vec3_t vMins, vec3_t vMaxs, int nDrawColor, int nFlags=0)=0;
	virtual void DrawBBoxSize(CMapView* pView, CTread3DDoc* pDocument, CDC* pDC, vec3_t vMins, vec3_t vMaxs, vec3_t vCenter, int nDrawColor)=0;
	virtual void DrawGrid(CMapView* pView, CTread3DDoc* pDocument, CDC* pDC, int nFlags=0) = 0;
	virtual void DrawObject(CMapView* pView, CTread3DDoc* pDocument, CDC* pDC, CBaseObject* pObject, int nObjectClass, int nObjectInterfaces, int nDrawFlags, int nFlags=0) = 0;
	virtual void DrawHotSpot(CMapView* pView, CTread3DDoc* pDocument, CDC* pDC, CHotSpot* pSpot, int nDrawFlags, int nFlags) {}
	virtual void DrawLittleX(CMapView* pView, CTread3DDoc* pDocument, CDC* pDC, vec2_t vMapPos, int nColor) {}
	virtual void EnableTransparency(bool bTransparent=true) {}

	// Draws the I_TARGET interface.
	virtual void DrawI_TARGET(CMapView* pView, CTread3DDoc* pDocument, CDC* pDC, CBaseObject* pObject, int nDrawFlags, int nType)=0;

	virtual CBaseObject* GetHitObject(CMapView* pView, CTread3DDoc* pDocument, int nClassFlags, int nMouseX, int nMouseY) = 0;
	virtual void Flush(CMapView* pView, CTread3DDoc* pDocument, CDC* pDC)=0; // Flushes the renderer. When it returns all draw commands have been completed and displayed.

	friend class CBaseRenderPackage;
};

#define RF_3D_API	0x00000001
#define RF_2D_API	0x00000002

class _T_EXPORT CBaseRenderPackage
{
public:

	CBaseRenderPackage() {}
	virtual ~CBaseRenderPackage() {}

	static bool StaticClaimPackageName(CString sName) { return false; }

	virtual int GetRenderFlags() = 0;
	virtual bool LoadPackage(CString sName) = 0;
	virtual bool UnloadPackage() = 0;
	virtual CString GetPackageName() = 0;
	virtual CBaseWindowRenderer* GetWindowRenderer(CMapView* pView, CDC* pDC, int nType) = 0;
};
// $eb

// $sb "$renderapi.h block2"
#define MAX_TRANSPARENTS		256
#define MAX_RENDER_PACKAGES		256
// $eb

// $sb "renderapi.h block3"

class _T_EXPORT CRenderAPI
{
private:

	int m_nGlobalRenderFlags;
	int m_nClassDrawFilter;
	CBaseRenderPackage* m_p2DPackage, *m_p3DPackage;
	int m_nNumTransparents;
	int m_nNumSelectedTransparents;
	CBaseObject* m_pTransparents[MAX_TRANSPARENTS];
	CBaseObject* m_pSelectedTransparents[MAX_TRANSPARENTS];
	void ShutdownPackages(void);
	void Shutdown2DPackage(void);
	void Shutdown3DPackage(void);
	CBaseRenderPackage* CreateRenderPackage(CString sName);

	bool AddTransparent(CBaseObject* pObject);
	bool AddSelectedTransparent(CBaseObject* pObject);
	void ResetTransparents();
	void ResetSelectedTransparents();
	void DrawTransparents(CBaseObject**, int nNum, CMapView* pView, CTread3DDoc* pDocument, CDC* pDC, CBaseWindowRenderer* pWR, int nDrawFlags, int nType);

public:

	CRenderAPI()
	{
		m_nClassDrawFilter=0;
		m_p2DPackage=NULL;
		m_p3DPackage=NULL;
	}

	~CRenderAPI()
	{}

	bool Initialize();
	bool Shutdown();

	bool Load2DRenderPackage(CString nName);
	bool Load3DRenderPackage(CString nName);

	void Get2DRenderPackageList(CString sNames[MAX_RENDER_PACKAGES], CString sDescriptions[MAX_RENDER_PACKAGES], int& nNumPaks);
	void Get3DRenderPackageList(CString sNames[MAX_RENDER_PACKAGES], CString sDescriptions[MAX_RENDER_PACKAGES], int& nNumPaks);

	CString Get2DPackageName();
	CString Get3DPackageName();
	CBaseWindowRenderer* GetWindowRenderer(CMapView* pView, CDC* pDC, int nType);

	void AddDrawFilters(int nFlags);
	void RemoveDrawFilters(int nFlags);
	void AddGlobalRenderFlags(int nFlags);
	void RemoveGlobalRenderFlags(int nFlags);
	int GetGlobalRenderFlags() { return m_nGlobalRenderFlags; }
	int GetDrawFilters() { return m_nClassDrawFilter; }

	// Draws an object list.
	void DrawObjectList(CMapView* pView, CTread3DDoc* pDocument, CDC* pDC, CBaseWindowRenderer* pWR, CLinkedList<CBaseObject>* pObjectList, int nDrawFlags, int nType);

	// Renders a map.
	void RenderMap(CMapView* pView, CTread3DDoc* pDocument, CDC* pDC, CBaseWindowRenderer* pWR, int nRenderFlags, int nType);
	void RenderMapSelection(CMapView* pView, CTread3DDoc* pDocument, CDC* pDC, CBaseWindowRenderer* pWR, int nRenderFlags, int nType);

};

// $eb

CRenderAPI* GetRenderAPI();

#endif