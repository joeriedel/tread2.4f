// Tread3DView.h : interface of the CTread3DView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_TREAD3DVIEW_H__E08CCCEF_1023_11D3_9084_004005A2D20C__INCLUDED_)
#define AFX_TREAD3DVIEW_H__E08CCCEF_1023_11D3_9084_004005A2D20C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CTread3DView : public CScrollView
{
protected: // create from serialization only
	CTread3DView();
	DECLARE_DYNCREATE(CTread3DView)

// Attributes
public:
	CTread3DDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTread3DView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void OnInitialUpdate(); // called first time after construct
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTread3DView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CTread3DView)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in Tread3DView.cpp
inline CTread3DDoc* CTread3DView::GetDocument()
   { return (CTread3DDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TREAD3DVIEW_H__E08CCCEF_1023_11D3_9084_004005A2D20C__INCLUDED_)
