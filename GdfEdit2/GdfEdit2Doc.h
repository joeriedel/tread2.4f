// GdfEdit2Doc.h : interface of the CGdfEdit2Doc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_GDFEDIT2DOC_H__E6785D4C_7D06_11D3_B673_004005A2D20C__INCLUDED_)
#define AFX_GDFEDIT2DOC_H__E6785D4C_7D06_11D3_B673_004005A2D20C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CGdfEdit2Doc : public CRichEditDoc
{
private:
	bool m_bNewDoc;

protected: // create from serialization only
	CGdfEdit2Doc();
	DECLARE_DYNCREATE(CGdfEdit2Doc)

// Attributes
public:
	bool IsNewDoc();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGdfEdit2Doc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL
	virtual CRichEditCntrItem* CreateClientItem(REOBJECT* preo) const;

// Implementation
public:
	virtual ~CGdfEdit2Doc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CGdfEdit2Doc)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GDFEDIT2DOC_H__E6785D4C_7D06_11D3_B673_004005A2D20C__INCLUDED_)
