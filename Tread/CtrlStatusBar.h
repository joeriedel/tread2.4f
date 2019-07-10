#ifndef CTRLSTATUSBAR_H
#define CTRLSTATUSBAR_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "LinkedList.h"

class CSBCtrl : public CWnd
{
public:
	CSBCtrl* prev;
	CSBCtrl* next;
};

class CCtrlStatusBar : public CStatusBar
{
	DECLARE_DYNAMIC(CCtrlStatusBar)

protected:
	CLinkedList<CSBCtrl> m_ctrlList;

public:
	CCtrlStatusBar();

	CWnd* InsertControl(CRuntimeClass* pClass, CString strTitle, CRect& pRect, UINT nID, DWORD dwStyle );

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCtrlStatusBar)
	public:
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCtrlStatusBar();

// Generated message map functions
protected:
	//{{AFX_MSG(CCtrlStatusBar)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif