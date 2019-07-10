// CntrItem.cpp : implementation of the CGdfEdit2CntrItem class
//

#include "stdafx.h"
#include "GdfEdit2.h"

#include "GdfEdit2Doc.h"
#include "GdfEdit2View.h"
#include "CntrItem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGdfEdit2CntrItem implementation

IMPLEMENT_SERIAL(CGdfEdit2CntrItem, CRichEditCntrItem, 0)

CGdfEdit2CntrItem::CGdfEdit2CntrItem(REOBJECT* preo, CGdfEdit2Doc* pContainer)
	: CRichEditCntrItem(preo, pContainer)
{
}

CGdfEdit2CntrItem::~CGdfEdit2CntrItem()
{
}

/////////////////////////////////////////////////////////////////////////////
// CGdfEdit2CntrItem diagnostics

#ifdef _DEBUG
void CGdfEdit2CntrItem::AssertValid() const
{
	CRichEditCntrItem::AssertValid();
}

void CGdfEdit2CntrItem::Dump(CDumpContext& dc) const
{
	CRichEditCntrItem::Dump(dc);
}
#endif

/////////////////////////////////////////////////////////////////////////////
