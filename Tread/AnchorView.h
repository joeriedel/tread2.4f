#if !defined(AFX_ANCHORVIEW_H__D8C80D45_168F_11D3_9084_004005A2D20C__INCLUDED_)
#define AFX_ANCHORVIEW_H__D8C80D45_168F_11D3_9084_004005A2D20C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AnchorView.h : header file
//

#include <cj60lib.h>
/////////////////////////////////////////////////////////////////////////////
// CAnchorView view

#include <afxcview.h>

class CAnchorView : public CCJListView
{
protected:
	CAnchorView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CAnchorView)
private:

// Attributes
public:
	int InsertItem(CString sName);
	int InsertItem(CString sName, bool bState);
	void Enable(bool bEnable = true);
	static int CALLBACK SortList(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
	void ResetContent();

	void LoadAnchorList();

private:
	CImageList m_imgList;
	CListCtrl* m_ListCtrl;
	int m_nItem;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAnchorView)
	public:
	virtual void OnInitialUpdate();
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CAnchorView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CAnchorView)
	afx_msg void OnBeginlabeledit(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEndlabeledit(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDblclk(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnAnchorRemove();
	afx_msg void OnRclick(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ANCHORVIEW_H__D8C80D45_168F_11D3_9084_004005A2D20C__INCLUDED_)
