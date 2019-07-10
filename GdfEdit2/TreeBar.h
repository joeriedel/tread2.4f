#if !defined(AFX_TREEBAR_H__E6785D5B_7D06_11D3_B673_004005A2D20C__INCLUDED_)
#define AFX_TREEBAR_H__E6785D5B_7D06_11D3_B673_004005A2D20C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TreeBar.h : header file
//

struct entry_t;

struct entry_t
{
	CString sKey;
	CString sName;
	CString sBase;
	CString sGame;
	int nLine;
	entry_t* pSub;
	entry_t* pNext;
};
/////////////////////////////////////////////////////////////////////////////
// CTreeBar dialog
#define IDD_DIALOGBAR_TREE              141

class CChildFrame;

class CTreeBar : public CDialogBar
{
private:
	void DestroyNode(entry_t* pNode);
	void TraceNode(entry_t* pNode, HTREEITEM &hItem);
	void MatchString(entry_t* pNode, entry_t* &pFound, CString &sFind);

	CTreeCtrl* m_ctTree;
	CChildFrame* m_pOwnerFrame;
	CImageList m_imgList;
	entry_t Head;
	bool m_bWarning;
	bool m_bError;
	unsigned int m_nClasses;
	unsigned int m_nEntities;
	bool GetCurItem(entry_t* &pEntry);

// Construction
public:
	CTreeBar(CWnd* pParent = NULL);   // standard constructor
	virtual bool Initialize();
	void SetOwnerFrame(CChildFrame* pOwnerFrame);
	void ResetContent();

	void SetMessageLevel(bool bWarn, bool bErr);
	void Warning(CString sMessage, int nLine);
	void Error(CString sMessage, int nLine);

	void FillTreeCtrl();
	void DestroyTree();
	bool AddEntity(CString sName, CString sSmart, CString sBase, CString sGame, int nLine);
	bool AddGame(CString sGame, int nLine);
	unsigned int GetEntities();
	unsigned int GetClasses();

	CString GetGameName();

// Dialog Data
	//{{AFX_DATA(CTreeBar)
	enum { IDD = IDD_DIALOGBAR_TREE };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTreeBar)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTreeBar)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnDestroy();
	afx_msg void OnRclickTreeScript(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TREEBAR_H__E6785D5B_7D06_11D3_B673_004005A2D20C__INCLUDED_)
