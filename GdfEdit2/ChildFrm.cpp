// ChildFrm.cpp : implementation of the CChildFrame class
//

#include "stdafx.h"
#include "GdfEdit2.h"

#include "ChildFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChildFrame

IMPLEMENT_DYNCREATE(CChildFrame, CMDIChildWnd)

BEGIN_MESSAGE_MAP(CChildFrame, CMDIChildWnd)
	//{{AFX_MSG_MAP(CChildFrame)
	ON_WM_SIZE()
	ON_WM_SETFOCUS()
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChildFrame construction/destruction

CChildFrame::CChildFrame()
{
	m_bInit = false;
	m_pTreeBar = NULL;
}

CChildFrame::~CChildFrame()
{
	if(m_pTreeBar != NULL)
	{
		//
		delete m_pTreeBar;
		m_pTreeBar = NULL;
	}
}

BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIChildWnd::PreCreateWindow(cs) )
		return FALSE;

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CChildFrame diagnostics

#ifdef _DEBUG
void CChildFrame::AssertValid() const
{
	CMDIChildWnd::AssertValid();
}

void CChildFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CChildFrame message handlers

BOOL CChildFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{
	int nWidth, nHeight;

	lpcs->style |= WS_MAXIMIZE;
	nWidth = lpcs->cx;
	nHeight = lpcs->cy / 4;

	if(!m_mainSplitter.CreateStatic(this, 2, 1))
	{
		//TreadError(ERR_MSG, "Unable to create split windows");
		return false;    // failed to create
	}
	
	if(!m_mainSplitter.CreateView(0, 0, RUNTIME_CLASS(CGdfEdit2View), CSize(nWidth, nHeight * 3), pContext)
	|| !m_mainSplitter.CreateView(1, 0, RUNTIME_CLASS(CConsoleView), CSize(nWidth, nHeight), pContext))
	{
		return false;
	}

	//store pointers to the views
	m_pTextView = (CGdfEdit2View*)m_mainSplitter.GetPane(0, 0);
	m_pTextView->SetOwnerFrame(this);
	m_pConsole = (CConsoleView*)m_mainSplitter.GetPane(1, 0);
	m_pConsole->SetOwnerFrame(this);

	//create and store a pointer to the tree bar
	m_pTreeBar = new CTreeBar;
	if(m_pTreeBar == NULL)
		return false;
	m_pTreeBar->Create(this, IDD_DIALOGBAR_TREE, CBRS_ALIGN_LEFT | CBRS_GRIPPER, IDD_DIALOGBAR_TREE);
	m_pTreeBar->Initialize();
	m_pTreeBar->SetWindowText("Class / Entity View");
	m_pTreeBar->SetOwnerFrame(this);
	
	m_bInit = true;
	return true;
	//return CMDIChildWnd::OnCreateClient(lpcs, pContext);
}

void CChildFrame::OnSize(UINT nType, int cx, int cy) 
{
	CMDIChildWnd::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	if(m_bInit)
	{
		int nHeight = cy / 5;
		int nWidth = cx;
		m_mainSplitter.SetRowInfo(0, nHeight*4, 10);
		m_mainSplitter.SetRowInfo(1, nHeight, 10);
		m_mainSplitter.RecalcLayout();
		m_bInit = false;
	}
}

void CChildFrame::OnSetFocus(CWnd* pOldWnd) 
{
	CMDIChildWnd::OnSetFocus(pOldWnd);
	
	// TODO: Add your message handler code here
	
}

CGdfEdit2View* CChildFrame::GetTextView()
{
	return m_pTextView;
}

CConsoleView* CChildFrame::GetConsole()
{
	return m_pConsole;
}

CTreeBar* CChildFrame::GetTreeBar()
{
	return m_pTreeBar;
}

void CChildFrame::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	CMDIChildWnd::OnClose();
}
