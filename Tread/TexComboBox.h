#if !defined(AFX_TEXCOMBOBOX_H__9E0CB0C1_AB0E_11D3_BD51_00A0CC582962__INCLUDED_)
#define AFX_TEXCOMBOBOX_H__9E0CB0C1_AB0E_11D3_BD51_00A0CC582962__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TexComboBox.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTexComboBox window
#include "LinkedList.h"
#include "TExport.h"
// $sb "texcombobox.h block1"
struct TEXMRUREF;
class CTexture;

class _T_EXPORT CComboBox;
class _T_EXPORT CTexComboBox : public CComboBox
{
	struct BITMAPDIB24
	{
		BITMAPINFOHEADER bmiHeader;
		RGBQUAD bmiColors;
	};

private:
	CFont m_ftText;
	BYTE* m_bits;
	HBITMAP m_bmhandle;
	BITMAPDIB24 m_bih;
	CTexture* m_pTexture;
	CTexture** m_pTexList;
	CLinkedList<TEXMRUREF>* m_pTexMru;
	int m_nCount;
	CRect m_rcClient;

// Construction
public:
	CTexComboBox();

// Attributes
public:

// Operations
public:
	void Initialize();

	bool SetTexturePointer(CTexture* pTexture);
	bool SetTexturePointer(CTexture** pTexList, int nCount);
	bool SetTexturePointer(CLinkedList<TEXMRUREF>* pTexMru);

	void Update();
	
	void ResetList();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTexComboBox)
	public:
	virtual int CompareItem(LPCOMPAREITEMSTRUCT lpCompareItemStruct);
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTexComboBox();

	// Generated message map functions
protected:
	//{{AFX_MSG(CTexComboBox)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};
// $eb
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TEXCOMBOBOX_H__9E0CB0C1_AB0E_11D3_BD51_00A0CC582962__INCLUDED_)
