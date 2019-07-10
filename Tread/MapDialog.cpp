// MapDialog.cpp : implementation file
//

#include "stdafx.h"
#include "tread3d.h"
#include "MapDialog.h"
#include "TreadSys.h"
#include "MapObject.h"
#include "Mode.h"
#include "Tread3DDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMapDialog dialog


CMapDialog::CMapDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CMapDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMapDialog)
	m_sAuthor = _T("");
	m_sComments = _T("");
	m_sBrush = _T("");
	m_sPointEntity = _T("");
	m_sSolidEntity = _T("");
	//}}AFX_DATA_INIT
}


void CMapDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMapDialog)
	DDX_Text(pDX, IDC_EDIT_AUTHOR, m_sAuthor);
	DDX_Text(pDX, IDC_EDIT_COMMENTS, m_sComments);
	DDX_Text(pDX, IDC_STATIC_BRUSH, m_sBrush);
	DDX_Text(pDX, IDC_STATIC_POINTENTITY, m_sPointEntity);
	DDX_Text(pDX, IDC_STATIC_SOLIDENTITY, m_sSolidEntity);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMapDialog, CDialog)
	//{{AFX_MSG_MAP(CMapDialog)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, OnButtonSave)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMapDialog message handlers

BOOL CMapDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	int nBrush = 0;
	int nSolid = 0;
	int nPoint = 0;

	CLinkedList<CBaseObject>* pObjectList;
	CBaseObject* pObject;
	pObjectList = GetMapManager()->GetDocPtr()->GetObjectList();

	pObjectList->First();

	for(pObject = pObjectList->GetCurItem(); pObject != NULL; pObject = pObjectList->GetNextItem())
	{
		if(pObject->GetObjectClass() == OBJECT_CLASS_BRUSH)
		{
			nBrush++;
		}
		else if(pObject->GetObjectClass() == OBJECT_CLASS_ENTITY)
		{
			nPoint++;
		}
		else if(pObject->GetObjectClass() == OBJECT_CLASS_LENTITY)
		{
			nSolid++;
		}
	}

	m_sBrush.Format("%d", nBrush);
	m_sPointEntity.Format("%d", nPoint);
	m_sSolidEntity.Format("%d", nSolid);

	UpdateData(false);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CMapDialog::OnButtonSave() 
{
	OnOK();
}

CString CMapDialog::GetAuthor()
{
	return m_sAuthor;
}

CString CMapDialog::GetComment()
{
	return m_sComments;
}

void CMapDialog::SetStrings(CString sAuthor, CString sComments)
{
	m_sAuthor = sAuthor;
	m_sComments = sComments;
}