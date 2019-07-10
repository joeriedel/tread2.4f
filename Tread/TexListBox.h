#if !defined(AFX_TEXLISTBOX_H__A3893AC1_AA58_11D3_BD51_00A0CC582962__INCLUDED_)
#define AFX_TEXLISTBOX_H__A3893AC1_AA58_11D3_BD51_00A0CC582962__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TexListBox.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTexListBox window
#include "linkedlist.h"
#include "TExport.h"

class CTexture;
struct TEXMRUREF;

class _T_EXPORT CListBox;
// $sb "texlistbox.h block1"
class _T_EXPORT CTexListBox : public CListBox
{
	struct BITMAPDIB24
	{
		BITMAPINFOHEADER bmiHeader;
		RGBQUAD bmiColors;
	};
private:
	CFont ftText;
	BITMAPDIB24 bih;
	int	m_nMaxWidth, m_nMaxHeight;
	CTexture* m_pTexture;
	CTexture** m_pTexList;
	CLinkedList<TEXMRUREF>* m_pTexMru;
	int m_nCount;
	CRect m_rcClient;

	// Construction
public:
	CTexListBox();

// Attributes
public:
	void Initialize();
	void SetMaxItemSize(int width, int height);

	bool SetTexturePointer(CTexture* pTexture);
	bool SetTexturePointer(CTexture** pTexList, int nCount);
	bool SetTexturePointer(CLinkedList<TEXMRUREF>* pTexMru);

	CTexture* GetTexturePointer();

	void Update();
	void Update(CString sFilter);

	void ResetList();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTexListBox)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	virtual int CompareItem(LPCOMPAREITEMSTRUCT lpCompareItemStruct);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTexListBox();

	// Generated message map functions
protected:
	//{{AFX_MSG(CTexListBox)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};
// $eb
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TEXLISTBOX_H__A3893AC1_AA58_11D3_BD51_00A0CC582962__INCLUDED_)
