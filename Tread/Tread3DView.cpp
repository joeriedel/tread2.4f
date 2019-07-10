// Tread3DView.cpp : implementation of the CTread3DView class
//

#include "stdafx.h"
#include "Tread3D.h"

#include "Tread3DDoc.h"
#include "Tread3DView.h"
#include "TreadSys.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTread3DView

IMPLEMENT_DYNCREATE(CTread3DView, CScrollView)

BEGIN_MESSAGE_MAP(CTread3DView, CScrollView)
	//{{AFX_MSG_MAP(CTread3DView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTread3DView construction/destruction

CTread3DView::CTread3DView()
{
	// TODO: add construction code here
	Debug("Construct TreadView\n");
}

CTread3DView::~CTread3DView()
{
}

BOOL CTread3DView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	
	return CScrollView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CTread3DView drawing

void CTread3DView::OnDraw(CDC* pDC)
{
	CTread3DDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
	//Debug("TreadView : Draw\n");
}

void CTread3DView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: calculate the total size of this view
	sizeTotal.cx = sizeTotal.cy = 400;
	SetScrollSizes(MM_TEXT, sizeTotal);
	//Debug("TreadView Resize\n");
}

/////////////////////////////////////////////////////////////////////////////
// CTread3DView diagnostics

#ifdef _DEBUG
void CTread3DView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CTread3DView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CTread3DDoc* CTread3DView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTread3DDoc)));
	return (CTread3DDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTread3DView message handlers
