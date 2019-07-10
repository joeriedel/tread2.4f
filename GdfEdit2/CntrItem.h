// CntrItem.h : interface of the CGdfEdit2CntrItem class
//

#if !defined(AFX_CNTRITEM_H__E6785D51_7D06_11D3_B673_004005A2D20C__INCLUDED_)
#define AFX_CNTRITEM_H__E6785D51_7D06_11D3_B673_004005A2D20C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CGdfEdit2Doc;
class CGdfEdit2View;

class CGdfEdit2CntrItem : public CRichEditCntrItem
{
	DECLARE_SERIAL(CGdfEdit2CntrItem)

// Constructors
public:
	CGdfEdit2CntrItem(REOBJECT* preo = NULL, CGdfEdit2Doc* pContainer = NULL);

// Attributes
public:
	CGdfEdit2Doc* GetDocument()
		{ return (CGdfEdit2Doc*)CRichEditCntrItem::GetDocument(); }
	CGdfEdit2View* GetActiveView()
		{ return (CGdfEdit2View*)CRichEditCntrItem::GetActiveView(); }

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGdfEdit2CntrItem)
	public:
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:
	~CGdfEdit2CntrItem();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CNTRITEM_H__E6785D51_7D06_11D3_B673_004005A2D20C__INCLUDED_)
