#if !defined(AFX_TEXTUREVIEW_H__BBDFEA04_71A6_11D3_B673_004005A2D20C__INCLUDED_)
#define AFX_TEXTUREVIEW_H__BBDFEA04_71A6_11D3_B673_004005A2D20C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TextureView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTextureView form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

//#define IDD_TEXTURE		160

#include "BrowserDialog.h"
#include "TexComboBox.h"
#include "TexListBox.h"
#include "TExport.h"

class _T_EXPORT CFormView;
// $sb "textureview.h block1"
class _T_EXPORT CTextureView : public CFormView
{
private:
	CRect m_rcBrowse;
	CString m_sFilter;
	int m_nPos;
	//CTexture
	
protected:
	CTextureView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CTextureView)

// Form Data
public:
	//{{AFX_DATA(CTextureView)
	enum { IDD = IDD_TEXTURE };
	CTexComboBox	m_cbMru;
	CTexListBox	m_lbPreview;
	CButton	m_btBrowse;
	CTexComboBox	m_cbTexture;
	CString	m_sName;
	//}}AFX_DATA

// Attributes
public:

// Operations
public:
	void LoadList();
	void UnloadList();
	void LoadMRU(CLinkedList<TEXMRUREF>* pList);
	void UpdateSelection();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTextureView)
	public:
	virtual void OnInitialUpdate();
	virtual DROPEFFECT OnDragOver(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CTextureView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CTextureView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnButtonBrowse();
	afx_msg void OnSelchangeComboTexture();
	afx_msg void OnSelchangeComboMru();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
// $eb
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TEXTUREVIEW_H__BBDFEA04_71A6_11D3_B673_004005A2D20C__INCLUDED_)
