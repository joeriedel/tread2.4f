// SurfaceDialog.cpp : implementation file
//

#include "stdafx.h"
#include "tread3d.h"
//#include "SurfaceDialog.h"
#include "DialogControl.h"
#include <math.h>
#include "GameApi.h"
#include "ViewControl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSurfaceDialog dialog


CSurfaceDialog::CSurfaceDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CSurfaceDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSurfaceDialog)
	m_nRotate = 0;
	m_nScaleX = 0;
	m_nShiftX = 0;
	m_nScaleY = 0;
	m_nShiftY = 0;
	m_bMask = FALSE;
	m_sTextureName = _T("");
	m_nValue = 0;
	m_bAutoApply = true;
	//}}AFX_DATA_INIT

	m_nCBSelect = 0;
	m_nStep = 1;
	m_nContents = 0;
	m_nSurface = 0;
	m_bFace = false;
}


void CSurfaceDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSurfaceDialog)
	DDX_Control(pDX, IDC_EDIT_VALUE, m_edValue);
	DDX_Control(pDX, IDC_CHECK_AUTOAPPLY, m_btAuto);
	DDX_Control(pDX, IDC_CHECK_MASK, m_btMask);
	DDX_Control(pDX, IDC_BUTTON_RESET, m_btReset);
	DDX_Control(pDX, IDC_BUTTON_GRABTEXTURE, m_btGrab);
	DDX_Control(pDX, IDC_ICON_TEXLOCK, m_icTexLock);
	DDX_Control(pDX, IDC_STATIC_TEXLOCK, m_stTexLock);
	DDX_Control(pDX, IDC_BUTTON_APPLY, m_btApply);
	DDX_Control(pDX, IDC_LIST_PREVIEW, m_lbPreview);
	DDX_Control(pDX, IDC_LIST_PROPERRTIES, m_lcProperties);
	DDX_Text(pDX, IDC_EDIT_ROTATION, m_nRotate);
	DDV_MinMaxInt(pDX, m_nRotate, 0, 359);
	DDX_Text(pDX, IDC_EDIT_XSCALE, m_nScaleX);
	DDV_MinMaxInt(pDX, m_nScaleX, -1000, 1000);
	DDX_Text(pDX, IDC_EDIT_XSHIFT, m_nShiftX);
	DDV_MinMaxInt(pDX, m_nShiftX, -16384, 16384);
	DDX_Text(pDX, IDC_EDIT_YSCALE, m_nScaleY);
	DDV_MinMaxInt(pDX, m_nScaleY, -1000, 1000);
	DDX_Text(pDX, IDC_EDIT_YSHIFT, m_nShiftY);
	DDV_MinMaxInt(pDX, m_nShiftY, -16384, 16384);
	DDX_Check(pDX, IDC_CHECK_MASK, m_bMask);
	DDX_Text(pDX, ID_STATIC_NAME, m_sTextureName);
	DDX_Text(pDX, IDC_EDIT_VALUE, m_nValue);
	DDX_Check(pDX, IDC_CHECK_AUTOAPPLY, m_bAutoApply);
	//}}AFX_DATA_MAP

	DDX_Control(pDX, IDC_EDIT_ROTATION, m_edControl[0]);
	DDX_Control(pDX, IDC_EDIT_XSHIFT, m_edControl[1]);
	DDX_Control(pDX, IDC_EDIT_YSHIFT, m_edControl[2]);
	DDX_Control(pDX, IDC_EDIT_XSCALE, m_edControl[3]);
	DDX_Control(pDX, IDC_EDIT_YSCALE, m_edControl[4]);

	DDX_Control(pDX, IDC_SPIN_ADJUST1, m_spAdjust[0]);
	DDX_Control(pDX, IDC_SPIN_ADJUST2, m_spAdjust[1]);
	DDX_Control(pDX, IDC_SPIN_ADJUST3, m_spAdjust[2]);
	DDX_Control(pDX, IDC_SPIN_ADJUST4, m_spAdjust[3]);
	DDX_Control(pDX, IDC_SPIN_ADJUST5, m_spAdjust[4]);
}

BEGIN_MESSAGE_MAP(CSurfaceDialog, CDialog)
	//{{AFX_MSG_MAP(CSurfaceDialog)
	ON_WM_CREATE()
	ON_WM_CTLCOLOR()
	ON_WM_SETCURSOR()
	ON_BN_CLICKED(IDC_BUTTON_GRABTEXTURE, OnButtonGrabtexture)
	ON_BN_CLICKED(IDC_BUTTON_RESET, OnButtonReset)
	ON_WM_VSCROLL()
	ON_BN_CLICKED(IDC_BUTTON_APPLY, OnButtonApply)
	ON_BN_CLICKED(IDC_CHECK_AUTOAPPLY, OnCheckAutoapply)
	ON_BN_CLICKED(IDC_CHECK_MASK, OnCheckMask)
	ON_NOTIFY(NM_CLICK, IDC_LIST_PROPERRTIES, OnClickListProperrties)
	ON_WM_SHOWWINDOW()
	ON_EN_CHANGE(IDC_EDIT_VALUE, OnChangeEditValue)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSurfaceDialog message handlers

int CSurfaceDialog::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	RegisterDialog(this, DLG_SURFACE);
	
	return 0;
}

HBRUSH CSurfaceDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	
	// TODO: Return a different brush if the default is not desired
	return hbr;
}

BOOL CSurfaceDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_spAdjust[0].SetRange(0, 359);
	m_spAdjust[1].SetRange(-16384, 16384);
	m_spAdjust[2].SetRange(-16384, 16384);
	m_spAdjust[3].SetRange(-1000, 1000);
	m_spAdjust[4].SetRange(-1000, 1000);

	if(!m_imgList.Create(IDB_BITMAP_CHECK2, 16, 0, RGB(255,0,255)))
	{
		Error("Surface Dialog: Image list not created.\n");
		return false;
	}
	m_lcProperties.SetImageList(&m_imgList, LVSIL_STATE);
	m_lcProperties.InsertColumn(0, "", LVCFMT_LEFT, 200);

	m_lbPreview.SetMaxItemSize(32, 32);
	m_lbPreview.Initialize();

	m_bAutoApply = GetRegBool(B_FACEAUTOAPPLY);

	if(m_bAutoApply)
		m_btApply.EnableWindow(false);
	else
		m_btApply.EnableWindow(true);

	return true;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CSurfaceDialog::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	// TODO: Add your message handler code here and/or call default
	
	return CDialog::OnSetCursor(pWnd, nHitTest, message);
}

void CSurfaceDialog::OnButtonGrabtexture() 
{
	// TODO: Add your control notification handler code here
	CTexture* pTexture = m_lbPreview.GetTexturePointer();
	if(pTexture != NULL)
	{
		GetTexAPI()->SetCurTexture(pTexture);
		CTextureView* pView = (CTextureView*)GetView(TEXTURE_VIEW);
		pView->UpdateSelection();
	}
}

void CSurfaceDialog::OnButtonReset() 
{
	// TODO: Add your control notification handler code here
	m_nShiftX = 0;	
	m_nShiftY = 0;
	m_nScaleX = 100;
	m_nScaleY = 100;
	m_nRotate = 0;
	m_nValue = 0;

	UpdateData(false);

	if(m_bAutoApply)
		SaveFaceValues();
}

void CSurfaceDialog::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	if(nSBCode == SB_ENDSCROLL)
	{
		if(m_bAutoApply)
			SaveFaceValues();
	}

	//UpdateData(true);
	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CSurfaceDialog::OnButtonApply() 
{
	// TODO: Add your control notification handler code here
	SaveFaceValues();
}

void CSurfaceDialog::OnCheckAutoapply() 
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
	if(m_bAutoApply)
		m_btApply.EnableWindow(false);
	else
		m_btApply.EnableWindow(true);
}

void CSurfaceDialog::OnCheckMask() 
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
}

void CSurfaceDialog::OnClickListProperrties(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	NMLISTVIEW* pNMItem = (NMLISTVIEW*)pNMHDR;

	//get the hit test information to find out where the user clicked
	LVHITTESTINFO hitTest;
	int nItem;
	hitTest.pt = pNMItem->ptAction;
	m_lcProperties.SubItemHitTest(&hitTest);
	nItem = hitTest.iItem;

	//if the item is invalid then return
	if(nItem == -1)
		return;
	
	ENTITYFLAGOPTIONDEFINITION* pDef = (ENTITYFLAGOPTIONDEFINITION*)m_lcProperties.GetItemData(nItem);
	if(pDef->bType)
	{
		if((pDef->nValue & m_nContents) >= pDef->nValue)
		{
			m_lcProperties.SetCheck(nItem, false);
			m_nContents &= ~pDef->nValue;
		}
		else
		{
			m_lcProperties.SetCheck(nItem, true);
			m_nContents |= pDef->nValue;
		}		
	}
	else
	{
		if((pDef->nValue & m_nSurface) >= pDef->nValue)
		{
			m_lcProperties.SetCheck(nItem, false);
			m_nSurface &= ~pDef->nValue;
		}
		else
		{
			m_lcProperties.SetCheck(nItem, true);
			m_nSurface |= pDef->nValue;
		}
	}

	if(m_bFace)
	{
		GetMapManager()->GetDocPtr()->SetSurfaceFlags(m_nSurface);
		GetMapManager()->GetDocPtr()->SetContentsFlags(m_nContents);
	}

	*pResult = 0;
}

void CSurfaceDialog::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	if(bShow)
	{
		m_bFace = false;
		EnableControls(m_bFace);

		if(m_bAutoApply)
			m_btApply.EnableWindow(false);
		else
			m_btApply.EnableWindow(true);

		TextureLockUpdate();

		try
		{
			m_lcProperties.DeleteAllItems();
			ENTITYDEFINITION* pDefinition = GetGameAPI()->GetCurrentGame()->GetWorldspawnDefinition();
			if(pDefinition == NULL)
				throw "Worldspawn entity is NULL.";

			ENTITYKEYDEFINITION* pKeyContent = pDefinition->FindKeyDefinition("$CONTENTS_77X");
			ENTITYKEYDEFINITION* pKeySurface = pDefinition->FindKeyDefinition("$SURFACE_77X");

			CLinkedList<ENTITYFLAGOPTIONDEFINITION>* pFlagList;
			ENTITYFLAGOPTIONDEFINITION* pDef;
			int nItem = 0;

			if(pKeyContent != NULL)
			{
				pFlagList = pKeyContent->pFlagDef->pFlagList;
				pFlagList->First();
				for(pDef = pFlagList->GetCurItem(); pDef != NULL; pDef = pFlagList->GetNextItem())
				{
					nItem = m_lcProperties.InsertItem(nItem, pDef->sString, 0);
					m_lcProperties.SetItemData(nItem, (ULONG)pDef);
					pDef->bType = true;
					m_lcProperties.SetCheck(nItem, false);
					nItem++;
				}
			}

			if(pKeySurface != NULL)
			{
				pFlagList = pKeySurface->pFlagDef->pFlagList;
				pFlagList->First();
				for(pDef = pFlagList->GetCurItem(); pDef != NULL; pDef = pFlagList->GetNextItem())
				{
					nItem = m_lcProperties.InsertItem(nItem, pDef->sString, 0);
					m_lcProperties.SetItemData(nItem, (ULONG)pDef);
					pDef->bType = false;
					m_lcProperties.SetCheck(nItem, false);
					nItem++;
				}
			}
		}
		catch(char* pException)
		{
			Error(pException);
		}
	}
}

void CSurfaceDialog::TextureLockUpdate()
{
	int n = 0;

	if(GetModeManager()->GetFlags() & FLAG_TEXLOCK)
	{
		m_bTexLock = true;
		m_btApply.EnableWindow(false);
		m_icTexLock.ShowWindow(true);
		m_stTexLock.ShowWindow(true);

		for(; n < 5; n++)
		{
			m_spAdjust[n].EnableWindow(false);
			m_edControl[n].EnableWindow(false);
		}
	}
	else
	{
		m_bTexLock = false;
		m_icTexLock.ShowWindow(false);
		m_stTexLock.ShowWindow(false);

		if(m_bFace)
		{
			m_btApply.EnableWindow(true && !m_bAutoApply);
			for(; n < 5; n++)
			{
				m_spAdjust[n].EnableWindow(true);
				m_edControl[n].EnableWindow(true);
			}
		}
	}
}

void CSurfaceDialog::SaveFaceValues()
{
	if(!m_bFace)
		return;

	float fTemp;

	UpdateData(true);
	
	vec_t vShift[2];
	vec_t vScale[2];
	vec_t vRotate;

	vShift[0] = m_nShiftX;
	vShift[1] = m_nShiftY;

	fTemp = m_nScaleX;
	vScale[0] = fTemp / 100.0;
	fTemp = m_nScaleY;
	vScale[1] = fTemp / 100.0;

	/*CString sBuffer;
	sBuffer.Format("%f %f", vScale[0], vScale[1]);
	Warning(sBuffer);*/

	vRotate = m_nRotate;

	GetMapManager()->GetDocPtr()->SetTexShifts(vShift);
	GetMapManager()->GetDocPtr()->SetTexScales(vScale);
	GetMapManager()->GetDocPtr()->SetTexRotate(vRotate);

	//save rotation here
	GetMapManager()->GetDocPtr()->SetValueFlags(m_nValue);
	GetMapManager()->GetDocPtr()->SetSurfaceFlags(m_nSurface);
	GetMapManager()->GetDocPtr()->SetContentsFlags(m_nContents);

	GetMapManager()->GetDocPtr()->UpdateWindows();
}

void CSurfaceDialog::LoadFaceValues(bool bFace)
{
	m_bFace = bFace;

	EnableControls(m_bFace);
	if(!m_bFace)
		return;

	vec_t vShift[2];
	vec_t vScale[2];
	vec_t vRotate;

	vRotate = GetMapManager()->GetDocPtr()->GetTexRotate();
	GetMapManager()->GetDocPtr()->GetTexShifts(vShift);
	GetMapManager()->GetDocPtr()->GetTexScales(vScale);

	m_nShiftX = vShift[0];
	m_nShiftY = vShift[1];

	m_nScaleX = vScale[0] * 100;
	m_nScaleY = vScale[1] * 100;

	m_nRotate = vRotate;

	m_nValue = GetMapManager()->GetDocPtr()->GetValueFlags();
	m_nSurface = GetMapManager()->GetDocPtr()->GetSurfaceFlags();
	m_nContents = GetMapManager()->GetDocPtr()->GetContentsFlags();
	m_sTextureName = GetMapManager()->GetDocPtr()->GetTextureName();

	m_lbPreview.SetTexturePointer(GetMapManager()->GetDocPtr()->GetTexturePtr());

	for(int n = 0; n < m_lcProperties.GetItemCount(); n++)
	{
		ENTITYFLAGOPTIONDEFINITION* pDef = (ENTITYFLAGOPTIONDEFINITION*)m_lcProperties.GetItemData(n);
	
		if(pDef->bType == true)
		{
			if((pDef->nValue & m_nContents) >= pDef->nValue)
				m_lcProperties.SetCheck(n, true);
			else
				m_lcProperties.SetCheck(n, false);
		}
		else
		{
			if((pDef->nValue & m_nSurface) >= pDef->nValue)
				m_lcProperties.SetCheck(n, true);
			else
				m_lcProperties.SetCheck(n, false);
		}
	}

	UpdateData(false);
}

void CSurfaceDialog::OnChangeEditValue() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	UpdateData(true);

	if(m_bFace)
		GetMapManager()->GetDocPtr()->SetValueFlags(m_nValue);
}

void CSurfaceDialog::EnableControls(bool bEnable)
{
	m_lcProperties.EnableWindow(bEnable);
	m_btGrab.EnableWindow(bEnable);
	m_btReset.EnableWindow(bEnable);
	m_btMask.EnableWindow(bEnable);
	m_btAuto.EnableWindow(bEnable);
	m_edValue.EnableWindow(bEnable);
	m_btApply.EnableWindow(bEnable && !m_bAutoApply);

	for(int n = 0; n < 5; n++)
	{
		m_spAdjust[n].EnableWindow(bEnable && !m_bTexLock);
		m_edControl[n].EnableWindow(bEnable && !m_bTexLock);
	}
}
