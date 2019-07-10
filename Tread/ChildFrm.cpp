// ChildFrm.cpp : implementation of the CMapFrame class
//

#include "stdafx.h"
#include "Tread3D.h"
#include "ChildFrm.h"
#include "TreadSys.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMapFrame

IMPLEMENT_DYNCREATE(CMapFrame, CMDIChildWnd)

BEGIN_MESSAGE_MAP(CMapFrame, CMDIChildWnd)
	//{{AFX_MSG_MAP(CMapFrame)
	ON_WM_SIZE()
	ON_WM_SHOWWINDOW()
	ON_WM_SETFOCUS()
	ON_WM_CLOSE()
	ON_COMMAND(IDM_VIEW_EXPANDCURRENT, OnViewExpandcurrent)
	ON_UPDATE_COMMAND_UI(IDM_VIEW_EXPANDCURRENT, OnUpdateViewExpandcurrent)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMapFrame construction/destruction

CMapFrame::CMapFrame()
{
	// TODO: add member initialization code here
//	m_bInit = false;
	m_nCurrentView = 0;
	m_nHeight = m_nWidth = 0;
	m_nViewMode = VIEW_UNINITIALIZED;
}

CMapFrame::~CMapFrame()
{
	GetMapManager()->UnregisterMap(m_nHandle);
}

CTread3DDoc* CMapFrame::GetDocument()
{ return (CTread3DDoc*)m_pMapPane[0]->GetDocument(); }

BOOL CMapFrame::OnCreateClient( LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	 int nWidth, nHeight;

	lpcs->style &= WS_MAXIMIZE;
	nWidth = lpcs->cx / 2;
	nHeight = lpcs->cy / 2;

	if(!m_mainSplitter.CreateStatic(this, 3, 1))
	{
		Error("Unable to create split windows");
		return false;    // failed to create
	}
	else
	{
		if (!m_subSplitter1.CreateStatic(&m_mainSplitter, 1, 2, WS_CHILD | WS_VISIBLE,
										m_mainSplitter.IdFromRowCol(0, 0)))
		{
			Error("Unable to create split windows");
			return false;    // failed to create
		}
		else
		{
			if(!m_subSplitter1.CreateView(0, 0, RUNTIME_CLASS(CMapView), CSize(nWidth, nHeight), pContext)
			|| !m_subSplitter1.CreateView(0, 1, RUNTIME_CLASS(CMapView), CSize(nWidth, nHeight), pContext))
			{
				return false;
			}
		}

		if (!m_subSplitter2.CreateStatic(&m_mainSplitter, 1, 2, WS_CHILD | WS_VISIBLE,
										m_mainSplitter.IdFromRowCol(1, 0)))
		{
			Error("Unable to create split windows");
			return false;    // failed to create
		}
		else
		{
			if(!m_subSplitter2.CreateView(0, 0, RUNTIME_CLASS(CMapView), CSize(nWidth, nHeight), pContext)
			|| !m_subSplitter2.CreateView(0, 1, RUNTIME_CLASS(CMapView), CSize(nWidth, nHeight), pContext))
			{
				Error("Unable to create split windows");
				return false;
			}
		}

		if(!m_mainSplitter.CreateView(2, 0, RUNTIME_CLASS(CConsoleView), CSize(nWidth * 2, 64), pContext))
		{
			Error("Unable to create split windows");
				return false;
		}
	}

	//load the values of the dcs and the rects
	
	m_pMapPane[0] = (CMapView*)m_subSplitter1.GetPane(0, 0);
	if(m_pMapPane[0] == NULL)
	{
		Error("Map could not be created!");
		return false;
	}
	m_pMapPane[0]->SetView(VIEW_3D);
	m_pMapPane[0]->SetParent(this, 0);

	m_pMapPane[1] = (CMapView*)m_subSplitter1.GetPane(0, 1);
	if(m_pMapPane[1] == NULL)
	{
		Error("Map could not be created!");
		return false;
	}
	m_pMapPane[1]->SetView(VIEW_TOP);
	m_pMapPane[1]->SetParent(this, 1);

	m_pMapPane[2] = (CMapView*)m_subSplitter2.GetPane(0, 0);
	if(m_pMapPane[2] == NULL)
	{
		Error("Map could not be created!");
		return false;
	}
	m_pMapPane[2]->SetView(VIEW_SIDE);
	m_pMapPane[2]->SetParent(this, 2);

	m_pMapPane[3] = (CMapView*)m_subSplitter2.GetPane(0, 1);
	if(m_pMapPane[3] == NULL)
	{
		Error("Map could not be created!");
		return false;
	}
	m_pMapPane[3]->SetView(VIEW_FRONT);
	m_pMapPane[3]->SetParent(this, 3);

	m_pConsole = (CConsoleView*)m_mainSplitter.GetPane(2, 0);
	if(m_pConsole == NULL)
	{
		Error("Map could not be created!");
		return false;
	}

	//register the Map Frame and save the handle
	m_nHandle = GetMapManager()->RegisterMap(this);
	m_nTimerId = GetMapManager()->GetMapCount();

	SetTimer(m_nTimerId, 600000, NULL);

	if(m_nHandle == -1)
		return false;

	//m_bInit = true;
	m_nViewMode = VIEW_INITIALIZED;
	return true;
}

BOOL CMapFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIChildWnd::PreCreateWindow(cs) )
		return false;

	return true;
}



/////////////////////////////////////////////////////////////////////////////
// CMapFrame diagnostics

#ifdef _DEBUG
void CMapFrame::AssertValid() const
{
	CMDIChildWnd::AssertValid();
}

void CMapFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMapFrame message handlers

CMapView* CMapFrame::GetMapPane(UINT nPane)
{
	switch(nPane)
	{
	case 0:
	case 1:
	case 2:
	case 3:
		return m_pMapPane[nPane];
	}

	return NULL;
}

CConsoleView* CMapFrame::GetConsolePane()
{
	//return (CConsoleView*)m_mainSplitter.GetPane(2, 0);
	return m_pConsole;
}

void CMapFrame::OnSize(UINT nType, int cx, int cy) 
{
	CMDIChildWnd::OnSize(nType, cx, cy);
	
	// We get this message twice before OnCreateClient() is called, so don't
	// call any size routines or else we'll crash.
	m_nHeight = cy;
	m_nWidth = cx;

	if(m_nViewMode == VIEW_INITIALIZED)
	{
		int nHeight, nConsole = 100;
		if(m_nHeight > 200)
			nHeight = (m_nHeight - 100)/ 2;
		else
		{
			nHeight = m_nHeight / 2;
			nConsole = 0;
		}
		int nWidth = m_nWidth / 2;
		m_mainSplitter.SetRowInfo(0, nHeight, 10);
		m_mainSplitter.SetRowInfo(1, nHeight, 10);
		m_mainSplitter.SetRowInfo(2, 100, 10);

		m_subSplitter1.SetColumnInfo(0, nWidth, 10);
		m_subSplitter1.SetColumnInfo(1, nWidth, 10);
		m_subSplitter2.SetColumnInfo(0, nWidth, 10);
		m_subSplitter2.SetColumnInfo(1, nWidth, 10);
		m_mainSplitter.SetColumnInfo(0, nWidth*2, 10);
		
		m_mainSplitter.RecalcLayout();
	}
}

void CMapFrame::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CMDIChildWnd::OnShowWindow(bShow, nStatus);
	
	if(bShow)
		ShowWindow(SW_MAXIMIZE);
}

void CMapFrame::OnSetFocus(CWnd* pOldWnd) 
{
	CMDIChildWnd::OnSetFocus(pOldWnd);
	GetMapManager()->SetCurrentMap(m_nHandle);
}

void CMapFrame::OnClose() 
{
	CMDIChildWnd::OnClose();
}

CDC* CMapFrame::GetPaneDC(UINT nPane)
{
	switch(nPane)
	{
	case 0:
	case 1:
	case 2:
	case 3:
		return m_pMapPane[nPane]->GetDC();
	}
	
	return NULL;
}

CRect CMapFrame::GetPaneRect(UINT nPane)
{
	CRect rcClient;

	switch(nPane)
	{
	case 0:
	case 1:
	case 2:
	case 3:
		m_pMapPane[nPane]->GetClientRect(&rcClient);
		break;
	}
	
	return rcClient;
}

void CMapFrame::SetViewsZoom(float fZoom)
{
	GetDocument()->SetScale(fZoom);
	GetDocument()->UpdateWindows(FLAT_VIEWS);
}

void CMapFrame::SetViewsGrid(int fGrid)
{
	GetDocument()->SetGridSize(fGrid);
	GetDocument()->UpdateWindows(FLAT_VIEWS);
}

void CMapFrame::SetCurrentViewHandle(int nView)
{
	switch(nView)
	{
	case 0:
		m_nCurrentView = 0;
		break;
	case 1:
		m_pMapPane[2]->KillFocus();
		m_pMapPane[3]->KillFocus();
		m_nCurrentView = 1;
		break;
	case 2:
		m_pMapPane[1]->KillFocus();
		m_pMapPane[3]->KillFocus();
		m_nCurrentView = 2;
		break;
	case 3:
		m_pMapPane[1]->KillFocus();
		m_pMapPane[2]->KillFocus();
		m_nCurrentView = 3;
		break;
	}
}

int CMapFrame::GetCurrentViewHandle()
{
	return m_nCurrentView;
}

CMapView* CMapFrame::GetCurrentView()
{
	return m_pMapPane[m_nCurrentView];
}

UINT CMapFrame::GetCurrentViewType()
{
	return m_pMapPane[m_nCurrentView]->GetViewType();
}

void CMapFrame::ExpandCurrentView()
{
	switch(m_nCurrentView)
	{
	case 0:
		m_mainSplitter.SetRowInfo(0, m_nHeight, 10);
		m_mainSplitter.SetRowInfo(1, 0, 10);
		m_mainSplitter.SetRowInfo(2, 0, 10);

		m_subSplitter1.SetColumnInfo(0, m_nWidth, 10);
		m_subSplitter1.SetColumnInfo(1, 0, 10);
		m_subSplitter2.SetColumnInfo(0, 0, 10);
		m_subSplitter2.SetColumnInfo(1, 0, 10);
		m_mainSplitter.SetColumnInfo(0, m_nWidth, 10);
		break;
	case 1:
		m_mainSplitter.SetRowInfo(0, m_nHeight, 10);
		m_mainSplitter.SetRowInfo(1, 0, 10);
		m_mainSplitter.SetRowInfo(2, 0, 10);

		m_subSplitter1.SetColumnInfo(0, 0, 10);
		m_subSplitter1.SetColumnInfo(1, m_nWidth, 10);
		m_subSplitter2.SetColumnInfo(0, 0, 10);
		m_subSplitter2.SetColumnInfo(1, 0, 10);
		m_mainSplitter.SetColumnInfo(0, m_nWidth, 10);
		break;
	case 2:
		m_mainSplitter.SetRowInfo(0, 0, 10);
		m_mainSplitter.SetRowInfo(1, m_nHeight, 10);
		m_mainSplitter.SetRowInfo(2, 0, 10);

		m_subSplitter1.SetColumnInfo(0, 0, 10);
		m_subSplitter1.SetColumnInfo(1, 0, 10);
		m_subSplitter2.SetColumnInfo(0, m_nWidth, 10);
		m_subSplitter2.SetColumnInfo(1, 0, 10);
		m_mainSplitter.SetColumnInfo(0, m_nWidth, 10);
		break;
	case 3:
		m_mainSplitter.SetRowInfo(0, 0, 10);
		m_mainSplitter.SetRowInfo(1, m_nHeight, 10);
		m_mainSplitter.SetRowInfo(2, 0, 10);

		m_subSplitter1.SetColumnInfo(0, 0, 10);
		m_subSplitter1.SetColumnInfo(1, 0, 10);
		m_subSplitter2.SetColumnInfo(0, 0, 10);
		m_subSplitter2.SetColumnInfo(1, m_nWidth, 10);
		m_mainSplitter.SetColumnInfo(0, m_nWidth, 10);
		break;
	default:
		Error("Invalid View Type.");
		return;
	}

	m_mainSplitter.RecalcLayout();
}


void CMapFrame::OnViewExpandcurrent() 
{
	if((m_nViewMode == VIEW_NORMAL) || (m_nViewMode == VIEW_INITIALIZED))
	{
		ExpandCurrentView();
		m_nViewMode = VIEW_EXPANDED;
	}
	else
	{
		m_nViewMode = VIEW_NORMAL;

		int nHeight, nConsole = 100;
		if(m_nHeight > 200)
			nHeight = (m_nHeight - 100)/ 2;
		else
		{
			nHeight = m_nHeight / 2;
			nConsole = 0;
		}
		int nWidth = m_nWidth / 2;
		m_mainSplitter.SetRowInfo(0, nHeight, 10);
		m_mainSplitter.SetRowInfo(1, nHeight, 10);
		m_mainSplitter.SetRowInfo(2, 100, 10);

		m_subSplitter1.SetColumnInfo(0, nWidth, 10);
		m_subSplitter1.SetColumnInfo(1, nWidth, 10);
		m_subSplitter2.SetColumnInfo(0, nWidth, 10);
		m_subSplitter2.SetColumnInfo(1, nWidth, 10);
		m_mainSplitter.SetColumnInfo(0, nWidth*2, 10);
		
		m_mainSplitter.RecalcLayout();
	}
}

void CMapFrame::OnUpdateViewExpandcurrent(CCmdUI* pCmdUI) 
{	
	if((m_nViewMode == VIEW_NORMAL)|| (m_nViewMode == VIEW_INITIALIZED))
		pCmdUI->SetText("Expand Current View");
	else
		pCmdUI->SetText("Collapse Current View");
}

void CMapFrame::OnTimer(UINT nIDEvent) 
{
	SetConsole("", 220, RGB(200,50,50));
	ConsoleMod("REMINDER - Save your map.\n", CFM_BOLD);
	ResetConsole();
	
	CMDIChildWnd::OnTimer(nIDEvent);
}

BOOL CMapFrame::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	KillTimer(m_nTimerId);
	
	return CMDIChildWnd::DestroyWindow();
}
