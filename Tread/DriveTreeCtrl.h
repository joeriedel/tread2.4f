#if !defined(AFX_DRIVETREECTRL_H__56F34715_AACC_11D2_9B53_004005A2D20C__INCLUDED_)
#define AFX_DRIVETREECTRL_H__56F34715_AACC_11D2_9B53_004005A2D20C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DriveTreeCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDriveTreeCtrl window

class CDriveTreeCtrl : public CTreeCtrl
{
private:
	int m_dwMediaID[26];
	bool bFirst;
	COLORREF clText[3];
public:
	// Construction
	CDriveTreeCtrl();
	UINT InitTree ();
	CString GetSelectedPath(NMHDR* pNMHDR, LRESULT* pResult);
	
private:
	BOOL ExpandPath(LPCTSTR pszPath, BOOL bSelectItem);
	void RefreshDrive(UINT nDrive);
	CString GetPathFromItem (HTREEITEM hItem);
	BOOL AddDriveNode(CString& strDrive);
	UINT AddDirectoryNodes(HTREEITEM hItem, CString& strPathName);
	void SetButtonState(HTREEITEM hItem, CString& strPathName);
	void UpdateButtonState(HTREEITEM hItem, CString& strPathName);
	BOOL HasSubdirectory(CString& strPathName);
	BOOL IsDriveNode(HTREEITEM hItem);
	void AddDummyNode(HTREEITEM hItem);
	HTREEITEM FindItem(HTREEITEM hItem, CString& strTarget);
	UINT DeleteChildren(HTREEITEM hItem);
	HTREEITEM GetDriveNode(HTREEITEM hItem);
	DWORD GetSerialNumber(CString& strDrive);
	BOOL IsMediaValid(CString& strPathName);
	BOOL IsPathValid(CString& strPathName);
	void RefreshDirectory(HTREEITEM hItem);
	
// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDriveTreeCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CDriveTreeCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CDriveTreeCtrl)
	afx_msg void OnItemexpanding(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DRIVETREECTRL_H__56F34715_AACC_11D2_9B53_004005A2D20C__INCLUDED_)
