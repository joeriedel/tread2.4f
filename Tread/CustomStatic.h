#if !defined(AFX_CUSTOMSTATIC_H__B1F83B81_7C6A_11D4_81AC_00A0CC582962__INCLUDED_)
#define AFX_CUSTOMSTATIC_H__B1F83B81_7C6A_11D4_81AC_00A0CC582962__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CustomStatic.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCustomStatic window

class CCustomStatic : public CStatic
{
// Construction
public:
	CCustomStatic();

private:
	CString m_sText;

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCustomStatic)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCustomStatic();

	// Generated message map functions
protected:
	//{{AFX_MSG(CCustomStatic)
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CUSTOMSTATIC_H__B1F83B81_7C6A_11D4_81AC_00A0CC582962__INCLUDED_)
