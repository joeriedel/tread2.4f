// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "GdfEdit2.h"

#include "MainFrm.h"
#include "Splash.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CCJMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CCJMDIFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.Create(this,
						WS_CHILD | WS_VISIBLE | CBRS_TOOLTIPS | CBRS_TOP | CBRS_FLYBY | CBRS_SIZE_DYNAMIC,
						IDR_MAINFRAME) || !m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndOptionsBar.Create(this,
						WS_CHILD | WS_VISIBLE | CBRS_TOOLTIPS | CBRS_TOP | CBRS_FLYBY | CBRS_SIZE_DYNAMIC,
						IDR_TOOLBAR_TOOLS) || !m_wndOptionsBar.LoadToolBar(IDR_TOOLBAR_TOOLS))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}
	
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

	m_wndOptionsBar.EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndOptionsBar);

	// install/load cool menus
	m_menuManager.Install(this);
	m_menuManager.LoadToolbar(IDR_MAINFRAME);
	m_menuManager.LoadToolbar(IDR_TOOLBAR_TOOLS);

	LoadBarState(_T("Docking\\GDFEdit2"));

	// CG: The following line was added by the Splash Screen component.
	CGdfEdit2App* pApp = (CGdfEdit2App*)AfxGetApp();
	pApp->AttachStatusBar(&m_wndStatusBar);

	m_wndStatusBar.SetPaneInfo(1, ID_SEPARATOR, SBPS_NORMAL, 64);
	m_wndStatusBar.SetPaneText(1, "");

	CSplashWnd::ShowSplashScreen(this);
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CCJMDIFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CCJMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CCJMDIFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers

void CMainFrame::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	SaveBarState(_T("Docking\\GDFEdit2"));

	CCJMDIFrameWnd::OnClose();
}
