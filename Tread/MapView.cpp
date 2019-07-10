// MapView.cpp : implementation file
//

#include "stdafx.h"
#include "Tread3D.h"
#include "MapView.h"
#include "treadsys.h"
#include "RenderAPI.h"
#include "Camera.h"
#include "Tread3DDoc.h"
#include "ChildFrm.h"
#include "EventManager.h"
#include "MathLib.h"

#include "ClipToolModeGenerator.h"

#include "CameraModeGenerator.h"
#include "VertexModeGenerator.h"
#include "PointerModeGenerator.h"
#include "BrushModeWinResponder.h"
#include "PointerModeWinResponder.h"
#include "EntityModeWinResponder.h"
#include "ZoomModeWinResponder.h"
#include "3DWinResponder.h"
#include "VertexModeWinResponder.h"
#include "ClipToolModeWinResponder.h"
#include "CameraModeWinResponder.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define RADIUS 100.0
//#define PI 3.141592654

/////////////////////////////////////////////////////////////////////////////
// CMapView

IMPLEMENT_DYNCREATE(CMapView, CView)

CMapView::CMapView()
{
	m_nRange = 16384;
	m_nSmallStep = 24;
	m_nLargeStep = 112;
	m_nView = 0;
	m_vSize[0]		= m_vSize[1]		= 0;
	m_vWinCenter[0] = m_vWinCenter[1]	= 0;
	m_vMapCenter[0] = m_vMapCenter[1]	= 0;
	m_vMapMins[0] = m_vMapMaxs[0]		= 0;
	m_vMapMins[1] = m_vMapMaxs[1]		= 0;
	SetScale(0.50F);
	m_pWR = NULL;
	m_pCurCam = NULL;
	m_hCursor = NULL;
	m_bRedraw = true;
	m_nGlobalRenderFlags = 0;
	m_nClassDrawFilter = 0;
	m_bKey = false;
	m_bFocus = false;
	m_bBlock = false;
}

CMapView::~CMapView()
{
	if(m_pWR)
		delete m_pWR;
	m_pWR = NULL;
}

BEGIN_MESSAGE_MAP(CMapView, CView)
	//{{AFX_MSG_MAP(CMapView)
	ON_WM_SETFOCUS()
	ON_WM_RBUTTONDOWN()
	ON_WM_LBUTTONDOWN()
	ON_WM_SIZE()
	ON_WM_VSCROLL()
	ON_WM_HSCROLL()
	ON_WM_SETCURSOR()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONUP()
	ON_WM_TIMER()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMapView drawing

void CMapView::OnDraw(CDC* pDC)
{
	if(!m_bRedraw)
		return;

	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
	GetRenderAPI()->RenderMap(this, (CTread3DDoc*)pDoc, pDC, m_pWR, RF_NONE, m_nView);
}

/////////////////////////////////////////////////////////////////////////////
// CMapView diagnostics

#ifdef _DEBUG
void CMapView::AssertValid() const
{
	CView::AssertValid();
}

void CMapView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMapView message handlers

void CMapView::OnSetFocus(CWnd* pOldWnd) 
{
	CView::OnSetFocus(pOldWnd);
	GetDocument()->SetCurView(this);
	m_bFocus = true;
	m_pParentFrame->SetCurrentViewHandle(m_nHandle);
	RedrawWindow();
}

void CMapView::SetScale(float fScale)
{
	m_fScale = fScale;
	m_fInvScale = 1.0F / fScale;
	CalcMapBounds();
}

void CMapView::SetCursorToCenter()
{
	// Lock to position.
	POINT p;
	p.x = this->m_vWinCenter[0];
	p.y = this->m_vWinCenter[1];
	this->ClientToScreen(&p);
	SetCursorPos(p.x, p.y);
}

void CMapView::OnInitialUpdate() 
{
	CView::OnInitialUpdate();

	SetScrollRange(SB_VERT, -m_nRange, m_nRange, true);
	SetScrollRange(SB_HORZ, -m_nRange, m_nRange, true);

	SetScrollPos(SB_VERT, 0, true);
	SetScrollPos(SB_HORZ, 0, true);

	EnableScrollBarCtrl(SB_VERT, true);
	EnableScrollBarCtrl(SB_HORZ, true);
}

void CMapView::SetupViewResponders()
{
	m_EventManager.ClearResponderList();
	m_HotSpotManager.ClearGeneratorList();

	if(m_nView != VIEW_3D)
	{
		// Setup hotspot generators.
		m_HotSpotManager.AddModeGenerator(new CCameraModeGenerator());
		m_HotSpotManager.AddModeGenerator(new CVertexModeGenerator());
		m_HotSpotManager.AddModeGenerator(new CPointerModeGenerator());
		m_HotSpotManager.AddModeGenerator(new CClipToolModeGenerator());

		m_EventManager.AddEventResponder(new CBrushModeWinResponder());
		m_EventManager.AddEventResponder(new CPointerModeWinResponder());
		m_EventManager.AddEventResponder(new CEntityModeWinResponder());
		m_EventManager.AddEventResponder(new CZoomModeWinResponder());
		m_EventManager.AddEventResponder(new CVertexModeWinResponder());
		m_EventManager.AddEventResponder(new CClipToolModeWinResponder());
		m_EventManager.AddEventResponder(new CCameraModeWinResponder());
	}
	else
	{
		m_EventManager.AddEventResponder(new C3DWinResponder());
	}
}

void CMapView::AddDrawFilters(int nFlags)
{
	m_nClassDrawFilter |= nFlags;
}

void CMapView::RemoveDrawFilters(int nFlags)
{
	m_nClassDrawFilter &= ~nFlags;
}

void CMapView::AddGlobalRenderFlags(int nFlags)
{
	m_nGlobalRenderFlags |= nFlags;
}

void CMapView::RemoveGlobalRenderFlags(int nFlags)
{
	m_nGlobalRenderFlags &= ~nFlags;
}

BOOL CMapView::PreCreateWindow(CREATESTRUCT& cs) 
{
	cs.style |= WS_VSCROLL | WS_HSCROLL;
	LPCTSTR cname = AfxRegisterWndClass(CS_BYTEALIGNWINDOW | CS_BYTEALIGNCLIENT | CS_OWNDC | CS_VREDRAW | CS_HREDRAW, LoadCursor(NULL, IDC_ARROW), NULL, LoadIcon(NULL, IDI_APPLICATION));
	cs.lpszClass = cname;

	return CView::PreCreateWindow(cs);
}

void CMapView::UnbindView()
{
	if(m_pWR != NULL)
	{
		CDC* pDC = GetDC();
		m_pWR->Shutdown(this, pDC);
		ReleaseDC(pDC);
		delete m_pWR;
	}
	m_pWR = NULL;
}

void CMapView::SetCamera(CCamera* pCam)
{
	if(m_pCurCam == pCam)
		return;

	if(m_pCurCam)
		m_pCurCam->m_nObjectRenderFlags &= ~RF_SELECTED;
	
	pCam->m_nObjectRenderFlags = RF_SELECTED;
	m_pCurCam = pCam;
}

void CMapView::SetView(UINT nView)
{
	if(m_nView != (int)nView)
	{
		if(m_nView == VIEW_3D)
			m_nGlobalRenderFlags = RF_SELBOXONTOP;
		else
			m_nGlobalRenderFlags = 0;

		m_nView = nView;

		switch(m_nView)
		{
		case VIEW_SIDE:
			m_sView = "Side Y/Z";
			m_nRange = 16384;
			m_nSmallStep = 24;
			m_nLargeStep = 112;
			break;
		case VIEW_FRONT:
			m_sView = "Front X/Z";
			m_nRange = 16384;
			m_nSmallStep = 24;
			m_nLargeStep = 112;
			break;
		case VIEW_3D:
			m_sView = "3D X/Y/Z";
			m_nRange = 180;
			m_nSmallStep = 2;
			m_nLargeStep = 8;
			break;
		default:
			m_nView = VIEW_TOP;
			m_sView = "Top X/Y";
			m_nRange = 16384;
			m_nSmallStep = 24;
			m_nLargeStep = 112;
		}
	}

	// Load a renderer.
	CalcWindowSizes();
	CDC* pDC = GetDC();
	if(m_pWR != NULL)
	{
		m_pWR->Shutdown(this, pDC);
		delete m_pWR;
	}
	m_pWR = GetRenderAPI()->GetWindowRenderer(this, pDC, m_nView);
	ReleaseDC(pDC);

	// Find a camera to use.
	if(m_nView == VIEW_3D)
	{
		CCamera* pCam = FindCameraForView();
		/*if(pCam == NULL)
			pCam = CreateDefaultCamera();*/
		if(pCam != NULL)
			m_pCurCam = pCam;
		else m_pCurCam = NULL;
		//m_pCurCam->nObjectRenderFlags |= RF_SELECTED;
	}
	else
		m_pCurCam = NULL;

	SetScrollRange(SB_VERT, -m_nRange, m_nRange, true);
	SetScrollRange(SB_HORZ, -m_nRange, m_nRange, true);

	SetupViewResponders();

	CalcMapBounds();

	// Rebuild the hotspots.
	m_HotSpotManager.BuildHotSpots(GetDocument(), this);

	RedrawWindow();
}

// Create's a default camera.
CCamera* CMapView::CreateDefaultCamera()
{
	CCamera* pCam = new CCamera();

	pCam->SetView(this, GetDocument());

	// Default looking.
	pCam->GetOrigin()[0] = 0.0F;
	pCam->GetOrigin()[1] = -384.0F;
	pCam->GetOrigin()[2] = 0.0F;

	pCam->GetLook()[0] = 0.0F;
	pCam->GetLook()[1] = 0.0F;
	pCam->GetLook()[2] = 0.0F;

	// Add to the map.
	GetDocument()->AddObjectToMap(pCam);

	return pCam;
}

void CMapView::SetupDefaultCamera()
{
	CCamera* pCam = CreateDefaultCamera();
	SetCamera(pCam);
}

CCamera* CMapView::FindCameraForView()
{
	CBaseObject* pObj;
	CCamera* pCam;

	if(m_pCurCam != NULL)
		return m_pCurCam;

	CLinkedList<CBaseObject>* pList = ((CTread3DDoc*)GetDocument())->GetUIObjectList();
	
	pList->First();
	// Find the first one.
	for(pObj = pList->GetCurItem(); pObj != NULL; pObj = pList->GetNextItem())
	{
		if(pObj->GetObjectClass() != OBJECT_CLASS_CAMERA)
			continue;

		pCam = (CCamera*)pObj;
		// Owned by this view.
		if((pCam->m_nObjectRenderFlags&RF_SELECTED) == RF_SELECTED && pCam->GetView() == this)
			return pCam;
	}

	return NULL;
}


void CMapView::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);

	CalcWindowSizes();
	CalcMapBounds();

	if(m_pWR != NULL)
	{
		CDC* pDC = GetDC();
		m_pWR->OnSize(this, pDC);
		ReleaseDC(pDC);
	}
}

void CMapView::CalcWindowSizes()
{
	GetClientRect(&m_rcClientRect);
	m_vWinCenter[0] = m_rcClientRect.right/2;
	m_vWinCenter[1] = m_rcClientRect.bottom/2;
	m_vSize[0] = m_rcClientRect.right;
	m_vSize[1] = m_rcClientRect.bottom;
}

// Calculates map extents.
void CMapView::CalcMapBounds()
{
	m_vMapMins[0] = WX_TO_MX(0, this);
	m_vMapMaxs[0] = WX_TO_MX(m_vSize[0], this);
	m_vMapMins[1] = WY_TO_MY(0, this);
	m_vMapMaxs[1] = WY_TO_MY(m_vSize[1], this);
}

void CMapView::SetMapXY(float fX, float fY)
{
	m_vMapCenter[0] = fX;
	m_vMapCenter[1] = fY;

	CalcMapBounds();
	
	SetScrollPos(SB_HORZ, (int)fX, true);
	SetScrollPos(SB_VERT, (int)fY, true);
}

void CMapView::OnHScroll(UINT nSBCode, int nPos, CScrollBar* pScrollBar) 
{
	switch(nSBCode)
	{
	case SB_THUMBTRACK:
		m_vMapCenter[0] = VO(VT(this), VEC_X) * nPos;
		break;
	case SB_LINELEFT:
		m_vMapCenter[0] -= VO(VT(this), VEC_X) * m_fInvScale * m_nSmallStep;
		break;
	case SB_LINERIGHT:
		m_vMapCenter[0] += VO(VT(this), VEC_X) * m_fInvScale * m_nSmallStep;
		break;
	case SB_PAGELEFT:
		m_vMapCenter[0] -= VO(VT(this), VEC_X) * m_fInvScale * m_nLargeStep;
		break;
	case SB_PAGERIGHT:
		m_vMapCenter[0] += VO(VT(this), VEC_X) * m_fInvScale * m_nLargeStep;
		break;
	}

	if(m_vMapCenter[0] > m_nRange)
		m_vMapCenter[0] = m_nRange;
	if(m_vMapCenter[0] < -m_nRange)
		m_vMapCenter[0] = -m_nRange;
	
	if(m_nView == VIEW_3D)
	{
		if(m_pCurCam == NULL)
			return;

		SetScrollPos(SB_HORZ, (int)(m_vMapCenter[0]), true);
		m_pCurCam->GetLook()[0] = m_pCurCam->GetOrigin()[0] - (RADIUS * sin(m_vMapCenter[0] * PI / 180) + .5);
		m_pCurCam->GetLook()[1] = m_pCurCam->GetOrigin()[1] + (RADIUS * cos(m_vMapCenter[0] * PI / 180) + .5);
		GetDocument()->UpdateWindows(FLAT_VIEWS);
	}
	else
		SetScrollPos(SB_HORZ, (int)(VO(VT(this), VEC_X) * m_vMapCenter[0]), true);
	CalcMapBounds();
	RedrawWindow();
}

void CMapView::OnVScroll(UINT nSBCode, int nPos, CScrollBar* pScrollBar) 
{
	switch(nSBCode)
	{
	case SB_THUMBTRACK:
		m_vMapCenter[1] = VO(VT(this), VEC_Y) * nPos;
		break;
	case SB_LEFT:
		break;
	case BS_RIGHT:
		break;
	case SB_LINELEFT:
		m_vMapCenter[1] -= VO(VT(this), VEC_Y) * m_fInvScale * m_nSmallStep;
		break;
	case SB_LINERIGHT:
		m_vMapCenter[1] += VO(VT(this), VEC_Y) * m_fInvScale * m_nSmallStep;
		break;
	case SB_PAGELEFT:
		m_vMapCenter[1] -= VO(VT(this), VEC_Y) * m_fInvScale * m_nLargeStep;
		break;
	case SB_PAGERIGHT:
		m_vMapCenter[1] += VO(VT(this), VEC_Y) * m_fInvScale * m_nLargeStep;
		break;
	}

	if(m_vMapCenter[1] > m_nRange)
		m_vMapCenter[1] = m_nRange;
	if(m_vMapCenter[1] < -m_nRange)
		m_vMapCenter[1] = -m_nRange;
	
	if(m_nView == VIEW_3D)
	{
		if(m_pCurCam == NULL)
			return;

		SetScrollPos(SB_VERT, (int)(m_vMapCenter[1]), true);
		m_pCurCam->GetLook()[2] = m_pCurCam->GetOrigin()[2] - (RADIUS * sin(m_vMapCenter[1] * PI / 180) + .5);
		m_pCurCam->GetLook()[1] = m_pCurCam->GetOrigin()[1] + (RADIUS * cos(m_vMapCenter[1] * PI / 180) + .5);
		GetDocument()->UpdateWindows(FLAT_VIEWS);
	}
	else
		SetScrollPos(SB_VERT, (int)(VO(VT(this), VEC_Y) * m_vMapCenter[1]), true);
	CalcMapBounds();
	RedrawWindow();
}

void CMapView::RedrawWindow()
{
	if(m_bRedraw)
		CView::RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
}

void CMapView::RedrawWindowSelections()
{
	if(m_bRedraw)
	{
		CDC* pDC = GetDC();
		GetRenderAPI()->RenderMapSelection(this, (CTread3DDoc*)GetDocument(), pDC, m_pWR, RF_NONE, m_nView);
		ReleaseDC(pDC);
	}
}

BOOL CMapView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	if(nHitTest == HTCLIENT && m_hCursor != NULL)
		SetCursor(m_hCursor);
	else
		SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));

	return TRUE;
}

void CMapView::OnRButtonDown(UINT nFlags, CPoint point) 
{
	m_EventManager.OnMouseDown(this, (CTread3DDoc*)GetDocument(), point.x, point.y, (int)nFlags|EF_RBUTTON);
}

void CMapView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	SetCapture();
	SetTimer(1000, 50, NULL);
	m_EventManager.OnMouseDown(this, (CTread3DDoc*)GetDocument(), point.x, point.y, (int)nFlags|EF_LBUTTON);
}

void CMapView::OnMouseMove(UINT nFlags, CPoint point) 
{
	if((nFlags & MK_LBUTTON) && (nFlags & (MK_SHIFT | MK_CONTROL)))
	{
		if(!m_bBlock)
		{
			m_bBlock = true;
			m_EventManager.OnMouseMove(this, (CTread3DDoc*)GetDocument(), point.x, point.y, (int)nFlags);
		}
	}
	else
		m_EventManager.OnMouseMove(this, (CTread3DDoc*)GetDocument(), point.x, point.y, (int)nFlags);
}

void CMapView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	m_EventManager.OnMouseUp(this, (CTread3DDoc*)GetDocument(), point.x, point.y, (int)nFlags|EF_LBUTTON);
	KillTimer(1000);
	ReleaseCapture();
}

void CMapView::OnRButtonUp(UINT nFlags, CPoint point) 
{
	m_EventManager.OnMouseUp(this, (CTread3DDoc*)GetDocument(), point.x, point.y, (int)nFlags|EF_RBUTTON);
}

void CMapView::OnTimer(UINT nIDEvent) 
{
	m_bBlock = false;
	m_EventManager.OnTimer(this, (CTread3DDoc*)GetDocument(), nIDEvent);
}

void CMapView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if(nRepCnt == 1)
	{
		if(!m_bKey)
		{
			m_EventManager.OnKeyDown(this, (CTread3DDoc*)GetDocument(), (int)nChar);
			m_bKey = true;
		}
	}
}

void CMapView::AdjustCamera()
{
	if(m_pCurCam == NULL)
		return;

	m_pCurCam->GetLook()[0] = m_pCurCam->GetOrigin()[0] - (RADIUS * sin(m_vMapCenter[0] * PI / 180) + .5);
	m_pCurCam->GetLook()[1] = m_pCurCam->GetOrigin()[1] + (RADIUS * cos(m_vMapCenter[0] * PI / 180) + .5);
	m_pCurCam->GetLook()[2] = m_pCurCam->GetOrigin()[2] - (RADIUS * sin(m_vMapCenter[1] * PI / 180) + .5);
	GetDocument()->UpdateWindows(FLAT_VIEWS);
}

void CMapView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	m_bKey = false;
	CView::OnKeyUp(nChar, nRepCnt, nFlags);
}

void CMapView::SetParent(CMapFrame* pFrame, int nHandle)
{
	m_nHandle = nHandle;
	m_pParentFrame = pFrame;
}

UINT CMapView::GetViewType()
{
	return m_nView;
}

void CMapView::KillFocus()
{
	m_bFocus = false;
	RedrawWindow();
}
