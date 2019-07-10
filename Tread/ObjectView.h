#if !defined(AFX_OBJECTVIEW_H__29192761_173B_11D3_9084_004005A2D20C__INCLUDED_)
#define AFX_OBJECTVIEW_H__29192761_173B_11D3_9084_004005A2D20C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ObjectView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CObjectView view

#include <afxcview.h>
#include "TExport.h"

#define LIST_NONE	0
#define LIST_ENTITY 1
#define LIST_BRUSH	2

class _T_EXPORT CTreeView;
// $sb "objectview.h block1"
class _T_EXPORT CObjectView : public CTreeView
{
protected:
	CObjectView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CObjectView)

// Attributes
public:
	void InsertGroup(CString sGroup);
	HTREEITEM InsertItem(CString sItem);
	HTREEITEM InsertItem(CString sGroup, CString sItem);

	void LoadEntityList();
	void LoadBrushList();
	bool EntityListLoaded();
	bool BrushListLoaded();

	HTREEITEM FindCategory(CString &sItem);
	HTREEITEM FindItem(CString &sItem, HTREEITEM hParent);
	HTREEITEM FindItem(CString &sItem);

	bool GetSelection(CString &sItem, CString &sGroup);

	void ResetContent();

private:
	CImageList imgList;
	CTreeCtrl* m_TreeCtrl;

	HTREEITEM m_hGroup;
	HTREEITEM m_hSelection;

	int m_nMode;
	bool m_bLoading;
	//bool m_bBrushLoaded;
	//bool m_bEntityLoaded;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CObjectView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CObjectView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CObjectView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRclick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemexpanding(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
// $eb
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OBJECTVIEW_H__29192761_173B_11D3_9084_004005A2D20C__INCLUDED_)
