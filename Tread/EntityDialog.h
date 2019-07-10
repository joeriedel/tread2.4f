#if !defined(AFX_ENTITYDIALOG_H__94DC9841_7809_11D3_B673_004005A2D20C__INCLUDED_)
#define AFX_ENTITYDIALOG_H__94DC9841_7809_11D3_B673_004005A2D20C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EntityDialog.h : header file
//

#include "SelListCtrl.h"
/////////////////////////////////////////////////////////////////////////////
// CEntityDialog dialog
#include "Entity.h"

class CEntityDialog : public CDialog
{
private:
	bool m_bLink;
	bool m_bSmart;
	bool m_bWorldspawn;
	CImageList m_imgList;
	CImageList m_imgList2;
	CEntity* m_pEntity;

	CImageList imgList;
	HTREEITEM m_hGroup;
	HTREEITEM m_hSelection;

	HTREEITEM InsertItem(CString sGroup, CString sItem);
	HTREEITEM FindCategory(CString &sItem);
	void LoadProperties();
	void LoadEntityList();

// Construction
public:
	CEntityDialog(CWnd* pParent = NULL);   // standard constructor
	void SetEntity(CEntity* pEntity, bool bWorldspawn = false);
	CEntity* GetEntity();
	
// Dialog Data
	//{{AFX_DATA(CEntityDialog)
	enum { IDD = IDD_DIALOG_ENTITY };
	CTreeCtrl	m_tcEntity;
	CSelListCtrl	m_lcProps;
	CString	m_sEntity;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEntityDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CEntityDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnClickListProperties(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkListProperties(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangedTreeEntity(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRclickListProperties(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonAddkey();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ENTITYDIALOG_H__94DC9841_7809_11D3_B673_004005A2D20C__INCLUDED_)
