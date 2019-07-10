#if !defined(AFX_PREVIEWDIALOG_H__AD925001_847A_11D3_B673_004005A2D20C__INCLUDED_)
#define AFX_PREVIEWDIALOG_H__AD925001_847A_11D3_B673_004005A2D20C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PreviewDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPreviewDialog dialog

#define IDD_DIALOG_PREVIEW              177

#include "GLRenderPackage.h"

struct MODEL;
class CQBrush;

class CPreviewDialog : public CDialog
{
private:
	CPoint m_ptLast;
	CGLRenderPackage glPack;
	WOGLFUNCTIONTABLE* m_pGL;
	HGLRC m_glContext;
	CDC* m_pDC;

	GLint m_nWidth;
	GLint m_nHeight;
	GLdouble m_dAspect;
	GLdouble m_dTrans[4];
	vec3_t m_vTrans;

	MODEL* m_pModel;
	CString m_sCategory;
	CString m_sName;

	void Render();
		
	void DrawEntity(CEntity* pEntity);
	void DrawBrush(CQBrush* pBrush);
	void DrawGroup(CObjectGroup* pGroup);
	void DrawObjectList(CLinkedList<CBaseObject>* pList);

// Construction
public:
	CPreviewDialog(CWnd* pParent = NULL);   // standard constructor
	bool Initialize();
	void PrepareObject();

// Dialog Data
	//{{AFX_DATA(CPreviewDialog)
	enum { IDD = IDD_DIALOG_PREVIEW };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPreviewDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPreviewDialog)
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PREVIEWDIALOG_H__AD925001_847A_11D3_B673_004005A2D20C__INCLUDED_)
