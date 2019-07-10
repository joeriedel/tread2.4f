#if !defined(AFX_MAPVIEW_H__417ADD64_15A3_11D3_9084_004005A2D20C__INCLUDED_)
#define AFX_MAPVIEW_H__417ADD64_15A3_11D3_9084_004005A2D20C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MapView.h : header file
//
#include "mathlib.h"
#include "RenderAPI.h"
#include "Camera.h"
#include "EventManager.h"
#include "HotSpotManager.h"
#include "TExport.h"

/////////////////////////////////////////////////////////////////////////////
// CMapView view
class _T_EXPORT CView;
class _T_EXPORT CMapFrame;
// $sb "mapview.h block1"
class _T_EXPORT CMapView : public CView
{
protected:
	CMapView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CMapView)

// Attributes
public:
	void UnbindView();
	void SetView(UINT nView);
	UINT GetViewType();
	void SetParent(CMapFrame* pFrame, int nHandle);
	void KillFocus();

private:
	bool m_bKey;
	bool m_bBlock;
	
	CBaseWindowRenderer* m_pWR;

	bool m_bRedraw;
	int m_nGlobalRenderFlags;
	int m_nClassDrawFilter;
	int m_nRange;
	int m_nSmallStep;
	int m_nLargeStep;
	
	CCamera* m_pCurCam; // Used for 3D views.

	CWindowEventManager m_EventManager;
	CHotSpotManager m_HotSpotManager;

	CCamera* CreateDefaultCamera();
	CCamera* FindCameraForView();
	void SetupViewResponders();
	void AdjustCamera();
	
// Operations
public:
	CTread3DDoc* GetDocument() { return (CTread3DDoc*)m_pDocument; }
	
	int m_nView;
	bool m_bFocus;
	vec2_t m_vSize;
	vec2_t m_vWinCenter;
	vec2_t m_vMapCenter;
	vec2_t m_vMapMins, m_vMapMaxs;
	vec_t m_fScale, m_fInvScale;
	RECT m_rcClientRect;
	CString m_sView;
	HCURSOR m_hCursor;

	CMapFrame* m_pParentFrame;
	int m_nHandle;
	
	// Calculates map extents.
	void CalcMapBounds();
	void CalcWindowSizes();

	float GetScale() { return m_fScale; }
	float GetInvScale() { return m_fInvScale; }
	void SetScale(float fScale);

	void SetCursorToCenter();
	void SetMapXY(float fX, float fY);
	void SetupDefaultCamera();
	void AddDrawFilters(int nFlags);
	void RemoveDrawFilters(int nFlags);
	void AddGlobalRenderFlags(int nFlags);
	void RemoveGlobalRenderFlags(int nFlags);
	int GetGlobalRenderFlags() { return m_nGlobalRenderFlags; }
	int GetDrawFilters() { return m_nClassDrawFilter; }

	void SetCamera(CCamera* pCam);
	CCamera* GetCamera() { return m_pCurCam; }

	void RedrawWindow();
	void RedrawWindowSelections();
	CBaseWindowRenderer* GetWindowRenderer() { return m_pWR; }

	CWindowEventManager* GetWindowEventManager() { return &m_EventManager; }
	CHotSpotManager* GetHotSpotManager() { return &m_HotSpotManager; }
	
	void EnableRedraw(bool bRedraw=true) { m_bRedraw = bRedraw; }

	friend class CRenderAPI;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMapView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CMapView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CMapView)
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnVScroll(UINT nSBCode, int nPos, CScrollBar* pScrollBar);
	afx_msg void OnHScroll(UINT nSBCode, int nPos, CScrollBar* pScrollBar);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

// $eb

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAPVIEW_H__417ADD64_15A3_11D3_9084_004005A2D20C__INCLUDED_)
