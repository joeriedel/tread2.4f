// SetupDialog.cpp : implementation file
//

#include "stdafx.h"
#include "tread3d.h"
#include "Tread3DDoc.h"
#include "SetupDialog.h"
#include "TreadSys.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define MUL_GAMMA 25
#define MUL_BACKPLANE 100
/////////////////////////////////////////////////////////////////////////////
// CSetupDialog dialog

CSetupDialog::CSetupDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CSetupDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSetupDialog)
	m_sBackClip = _T("");
	m_sGamma = _T("");
	m_sCurve = _T("");
	m_nSaveOption = -1;
	//}}AFX_DATA_INIT
}

void CSetupDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetupDialog)
	DDX_Control(pDX, IDC_SLIDER_CURVE, m_slCurve);
	DDX_Control(pDX, IDC_COMBO_TEXMEMORY, m_cbTexMemory);
	DDX_Control(pDX, IDC_SLIDER_GAMMA, m_slGamma);
	DDX_Control(pDX, IDC_SLIDER_BACKPLANE, m_slBackplane);
	DDX_Control(pDX, IDC_COMBO_NUMMAP, m_cbMaxmap);
	DDX_Text(pDX, IDC_STATIC_CLIPPING, m_sBackClip);
	DDX_Text(pDX, IDC_STATIC_GAMMA, m_sGamma);
	DDX_Text(pDX, IDC_STATIC_CURVE, m_sCurve);
	DDX_Radio(pDX, IDC_RADIO_SAVE, m_nSaveOption);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSetupDialog, CDialog)
	//{{AFX_MSG_MAP(CSetupDialog)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_BUTTON_SAVE, OnButtonSave)
	ON_BN_CLICKED(IDC_BUTTON_CANCEL, OnButtonCancel)
	ON_BN_CLICKED(IDC_RADIO_SAVE, OnRadioSave)
	ON_BN_CLICKED(IDC_RADIO_SAVE_OFF, OnRadioSaveOff)
	ON_BN_CLICKED(IDC_RADIO_SAVE_REMINDER, OnRadioSaveReminder)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetupDialog message handlers

BOOL CSetupDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	int n =0;
	int nGamma;
	int nBackPlane;
	int nSelect;
	int nSub;
	CString sTemp;

	//get registry information
	nBackPlane = GetRegInt(I_BACKPLANE);
	nGamma = GetRegInt(I_GAMMA);
	nSub = GetRegInt(I_CURVESUBDEPTH);
	
	//init backplane slider
	m_slBackplane.SetRange(0, 5000 / MUL_BACKPLANE, true);
	m_slBackplane.SetLineSize(5);
	m_slBackplane.SetPos(nBackPlane / MUL_BACKPLANE);
	m_sBackClip.Format("%d", nBackPlane);

	//init gamma slider
	m_slGamma.SetRange(0, 500 / MUL_GAMMA, true);
	m_slGamma.SetLineSize(5);
	m_slGamma.SetPos(nGamma / MUL_GAMMA);
	m_sGamma.Format("%d", nGamma);

	//init gamma slider
	m_slCurve.SetRange(1, 6, true);
	m_slCurve.SetPos(nSub);
	m_sCurve.Format("%d", nSub);

	//init max map combo control
	sTemp.Format("%d", GetRegInt(I_MAXMAP));
	nSelect = m_cbMaxmap.FindStringExact(-1, sTemp);
	if(nSelect != -1)
		m_cbMaxmap.SetCurSel(nSelect);
	else
	{
		nSelect = m_cbMaxmap.AddString(sTemp);
		m_cbMaxmap.SetCurSel(nSelect);
	}

	sTemp.Format("%d", GetRegInt(I_TEX_MEMORY));
	nSelect = m_cbTexMemory.FindStringExact(-1, sTemp);
	if(nSelect != -1)
		m_cbTexMemory.SetCurSel(nSelect);
	else
	{
		nSelect = m_cbMaxmap.AddString(sTemp);
		m_cbTexMemory.SetCurSel(nSelect);
	}

	UpdateData(false);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSetupDialog::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	m_sBackClip.Format("%d", m_slBackplane.GetPos() * MUL_BACKPLANE);
	m_sGamma.Format("%d", m_slGamma.GetPos() * MUL_GAMMA);
	m_sCurve.Format("%d", m_slCurve.GetPos());

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
	UpdateData(false);
}

void CSetupDialog::OnButtonSave() 
{
	UpdateData(true);

	SetRegInt(I_GAMMA, m_slGamma.GetPos() * MUL_GAMMA);
	SetRegInt(I_BACKPLANE, m_slBackplane.GetPos() * MUL_BACKPLANE);
	SetRegInt(I_CURVESUBDEPTH, m_slCurve.GetPos());

	int nSelect = m_cbMaxmap.GetCurSel();
	if(nSelect != -1)
	{
		CString sBuffer;
		m_cbMaxmap.GetLBText(nSelect, sBuffer);
		int nValue = atoi(sBuffer);
		SetRegInt(I_MAXMAP, nValue);
	}

	nSelect = m_cbTexMemory.GetCurSel();
	if(nSelect != -1)
	{
		CString sBuffer;
		m_cbTexMemory.GetLBText(nSelect, sBuffer);
		int nValue = atoi(sBuffer);
		SetRegInt(I_TEX_MEMORY, nValue);
	}

	//save the settings and close the dialog with the OK message
	SaveTreadSettings();
	if(GetMapManager()->IsMap() == true)
	{
		GetTexAPI()->FreeAllLoadedTextures();
		GetMapManager()->GetDocPtr()->RebindViews(VIEW_3D);
	}

	OnOK();
}

void CSetupDialog::OnButtonCancel() 
{
	OnCancel();
}

void CSetupDialog::OnRadioSave()
{
	UpdateData(false);	
}

void CSetupDialog::OnRadioSaveOff()
{
	UpdateData(false);
}

void CSetupDialog::OnRadioSaveReminder()
{
	UpdateData(false);
}
