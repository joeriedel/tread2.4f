// GdfEdit2View.h : interface of the CGdfEdit2View class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_GDFEDIT2VIEW_H__E6785D4E_7D06_11D3_B673_004005A2D20C__INCLUDED_)
#define AFX_GDFEDIT2VIEW_H__E6785D4E_7D06_11D3_B673_004005A2D20C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxrich.h>
#include "GdfEdit2Doc.h"
#include "PopupDialog.h"

class CGdfEdit2CntrItem;
class CChildFrame;

class CGdfEdit2View : public CRichEditView
{
DECLARE_DYNCREATE(CGdfEdit2View)

protected: //create from serialization only
	CGdfEdit2View();
	CChildFrame* m_pOwnerFrame;
	CHARFORMAT2 m_chFormat;
	PARAFORMAT2 m_prFormat;
	CRichEditCtrl* m_RichEdit;
	CString m_sFont;
	int m_nSize;
	int m_nStart;
	int m_nEnd;
	int m_nTab;
	CPopupDialog* m_pSmartDlg;
	bool m_bSmart;
	
	//bool m_bValid;
	bool FindLine(int nLine, int &nPos, int nGap);
	int GetLine();
	void UpdateLastLine();
	void ColorCodeLine(long nPos);

	char m_pBuffer[200];
	int Find(char nChar, int nStart);

	void ColorCode(bool bSpecial = false);
	int FindCharReverse(char* pString, int nStart, char cFind);
	int FindCharForward(char* pString, int nStart, char cFind);
	bool FindMatch(char* pString);

// Attributes
public:
	CGdfEdit2Doc* GetDocument();
	void SetOwnerFrame(CChildFrame* pOwnerFrame);
	void InsertText( const CString& sInput);
	void InsertTextAndMove(const CString & sInput);
	void RemoveTextAndMove(int nCount);
	void RemoveText(int nCount);
	void AppendText(CString sInput, ...);
	void ScrollToLine(int nLine, bool bHighlite = false);

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGdfEdit2View)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CGdfEdit2View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CGdfEdit2View)
	afx_msg void OnDestroy();
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnInsertEntity();
	afx_msg void OnInsertClass();
	afx_msg void OnInsertGlobal();
	afx_msg void OnInsertAttribute();
	afx_msg void OnInsertKey();
	afx_msg void OnInsertFlag();
	afx_msg void OnInsertOption();
	afx_msg void OnInsertFlagkey();
	afx_msg void OnWizardEntity();
	afx_msg void OnWizardKey();
	afx_msg void OnVerifyScript();
	afx_msg void OnInsertClassDerived();
	afx_msg void OnFont();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in GdfEdit2View.cpp
inline CGdfEdit2Doc* CGdfEdit2View::GetDocument()
   { return (CGdfEdit2Doc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GDFEDIT2VIEW_H__E6785D4E_7D06_11D3_B673_004005A2D20C__INCLUDED_)
