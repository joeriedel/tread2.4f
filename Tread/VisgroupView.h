#if !defined(AFX_VISGROUPVIEW_H__D8C80D44_168F_11D3_9084_004005A2D20C__INCLUDED_)
#define AFX_VISGROUPVIEW_H__D8C80D44_168F_11D3_9084_004005A2D20C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// VisgroupView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CVisgroupView view

#include <afxcview.h>
#include "TExport.h"

class _T_EXPORT CTreeView;
// $sb "visgroupview.h block1"
class _T_EXPORT CVisgroupView : public CTreeView
{
protected:
	CVisgroupView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CVisgroupView)

// Attributes
public:
	void LoadVisgroup();
	void UpdateVisgroup();
	void ResetContent();

	HTREEITEM InsertItem(CString sGroup, CString sItem, bool bVisible = true);
	HTREEITEM InsertItem(HTREEITEM hGroup, CString sItem, bool bVisible = true);

	HTREEITEM FindCategory(CString &sItem);
	HTREEITEM FindVisgroup(CString &sItem, HTREEITEM hParent);

	bool GetSelection(CString &sItem, CString &sGroup);

private:
	HTREEITEM HighlightDropTarget(CPoint point);
	void CopyChildren (HTREEITEM hDest, HTREEITEM hSrc);
    void CopyTree (HTREEITEM hDest, HTREEITEM hSrc);
    void MoveTree (HTREEITEM hDest, HTREEITEM hSrc);
    bool IsChildOf (HTREEITEM hItem1, HTREEITEM hItem2);
	bool IsItemExpanded(HTREEITEM hItem);

	int m_nDelayInterval;
	int m_nScrollMargin;
	int m_nScrollInterval;
	
	bool m_bDragging;
	UINT nTimer;

	CTreeCtrl* m_TreeCtrl;
	CImageList m_ImgList;
	CImageList* m_pImgDrag;
	
	HTREEITEM m_hSelect;
	HTREEITEM m_hDrag;
		
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVisgroupView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CVisgroupView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CVisgroupView)
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnVisgroupAddcat();
	afx_msg void OnBeginlabeledit(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEndlabeledit(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnVisgroupHideall();
	afx_msg void OnVisgroupShowall();
	afx_msg void OnItemexpanding(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBegindrag(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnVisgroupDelete();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
// $eb
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VISGROUPVIEW_H__D8C80D44_168F_11D3_9084_004005A2D20C__INCLUDED_)
