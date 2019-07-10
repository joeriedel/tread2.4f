#if !defined(AFX_OPTIONSDIALOG_H__216872E3_8E1F_11D3_B08D_00A0CC582962__INCLUDED_)
#define AFX_OPTIONSDIALOG_H__216872E3_8E1F_11D3_B08D_00A0CC582962__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OptionsDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// COptionsDialog dialog

#include "GameAPI.h"

class COptionsDialog : public CDialog
{
private:
	int m_nCount;
	CImageList m_imgList;
	CLinkedList<ENTITYFLAGOPTIONDEFINITION>* m_pOptionList;
	int m_nOption;

// Construction
public:
	COptionsDialog(CWnd* pParent = NULL);   // standard constructor

	void SetOptionsList(CLinkedList<ENTITYFLAGOPTIONDEFINITION>* pOptionList) { m_pOptionList = pOptionList; }
	void SetOption(int nOption);
	void SetOption(CString sOption);
	CString GetOption();

// Dialog Data
	//{{AFX_DATA(COptionsDialog)
	enum { IDD = IDD_ENTITY_OPTIONS };
	CComboBox	m_cbOptions;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COptionsDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(COptionsDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonDone();
	afx_msg void OnSelchangeComboOptions();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPTIONSDIALOG_H__216872E3_8E1F_11D3_B08D_00A0CC582962__INCLUDED_)
