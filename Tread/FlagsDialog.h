#if !defined(AFX_FLAGSDIALOG_H__216872E2_8E1F_11D3_B08D_00A0CC582962__INCLUDED_)
#define AFX_FLAGSDIALOG_H__216872E2_8E1F_11D3_B08D_00A0CC582962__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FlagsDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFlagsDialog dialog
#include "GameAPI.h"

class CFlagsDialog : public CDialog
{
private:
	int m_nCount;
	CImageList m_imgList;
	CLinkedList<ENTITYFLAGOPTIONDEFINITION>* m_pFlagList;
	void AddFlag(CString sName, int nValue);

// Construction
public:
	CFlagsDialog(CWnd* pParent = NULL);   // standard constructor
	
	void SetFlagsList(CLinkedList<ENTITYFLAGOPTIONDEFINITION>* pFlagList) { m_pFlagList = pFlagList; }
	void SetFlags(int nFlags);
	void SetFlags(CString sFlags);
	CString GetFlags();

// Dialog Data
	//{{AFX_DATA(CFlagsDialog)
	enum { IDD = IDD_ENTITY_FLAGS };
	CListCtrl	m_lcFlags;
	int		m_nFlags;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFlagsDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFlagsDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonDone();
	afx_msg void OnClickListFlags(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FLAGSDIALOG_H__216872E2_8E1F_11D3_B08D_00A0CC582962__INCLUDED_)
