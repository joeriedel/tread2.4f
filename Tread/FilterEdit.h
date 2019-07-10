#if !defined(AFX_FILTEREDIT_H__5386B7C1_9BB5_11D3_BD51_00A0CC582962__INCLUDED_)
#define AFX_FILTEREDIT_H__5386B7C1_9BB5_11D3_BD51_00A0CC582962__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FilterEdit.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFilterEdit window
#define FILTER_CHAR			1
#define FILTER_NUMBER		2
#define FILTER_PUNCTUATION	4
#define FILTER_BACKSPACE	8
#define FILTER_DASH			16
#define FILTER_SPACE		32

class CFilterEdit : public CEdit
{
private:
	int m_nFilter;

// Construction
public:
	CFilterEdit();
	void SetFilter(int nFilter);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFilterEdit)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CFilterEdit();

	// Generated message map functions
protected:
	//{{AFX_MSG(CFilterEdit)
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FILTEREDIT_H__5386B7C1_9BB5_11D3_BD51_00A0CC582962__INCLUDED_)
