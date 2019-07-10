// GdfEdit2Doc.cpp : implementation of the CGdfEdit2Doc class
//

#include "stdafx.h"
#include "GdfEdit2.h"

#include "GdfEdit2Doc.h"
#include "CntrItem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGdfEdit2Doc

IMPLEMENT_DYNCREATE(CGdfEdit2Doc, CRichEditDoc)

BEGIN_MESSAGE_MAP(CGdfEdit2Doc, CRichEditDoc)
	//{{AFX_MSG_MAP(CGdfEdit2Doc)
	//}}AFX_MSG_MAP
	// Enable default OLE container implementation
	ON_UPDATE_COMMAND_UI(ID_OLE_EDIT_LINKS, CRichEditDoc::OnUpdateEditLinksMenu)
	ON_COMMAND(ID_OLE_EDIT_LINKS, CRichEditDoc::OnEditLinks)
	ON_UPDATE_COMMAND_UI_RANGE(ID_OLE_VERB_FIRST, ID_OLE_VERB_LAST, CRichEditDoc::OnUpdateObjectVerbMenu)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGdfEdit2Doc construction/destruction

CGdfEdit2Doc::CGdfEdit2Doc()
{
	m_bNewDoc = false;
}

CGdfEdit2Doc::~CGdfEdit2Doc()
{
}

BOOL CGdfEdit2Doc::OnNewDocument()
{
	if (!CRichEditDoc::OnNewDocument())
		return FALSE;

	m_bNewDoc = true;

	return TRUE;
}

CRichEditCntrItem* CGdfEdit2Doc::CreateClientItem(REOBJECT* preo) const
{
	// cast away constness of this
	return new CGdfEdit2CntrItem(preo, (CGdfEdit2Doc*) this);
}



/////////////////////////////////////////////////////////////////////////////
// CGdfEdit2Doc serialization

void CGdfEdit2Doc::Serialize(CArchive& ar)
{
	CGdfEdit2Doc::m_bRTF = false;

	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}

	// Calling the base class CRichEditDoc enables serialization
	//  of the container document's COleClientItem objects.
	// TODO: set CRichEditDoc::m_bRTF = FALSE if you are serializing as text
	CRichEditDoc::Serialize(ar);
}

/////////////////////////////////////////////////////////////////////////////
// CGdfEdit2Doc diagnostics

#ifdef _DEBUG
void CGdfEdit2Doc::AssertValid() const
{
	CRichEditDoc::AssertValid();
}

void CGdfEdit2Doc::Dump(CDumpContext& dc) const
{
	CRichEditDoc::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CGdfEdit2Doc commands

bool CGdfEdit2Doc::IsNewDoc()
{
	return m_bNewDoc;
}
