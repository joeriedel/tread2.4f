// BindDialog.cpp : implementation file
//

#include "stdafx.h"
#include "tread3d.h"
#include "BindDialog.h"
#include "TreadSys.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBindDialog dialog


CBindDialog::CBindDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CBindDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBindDialog)
	m_sOutput = _T("");
	//}}AFX_DATA_INIT
	m_pTable = NULL;
}


void CBindDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBindDialog)
	DDX_Control(pDX, IDC_BUTTON_UNBINDALL, m_btUnbindAll);
	DDX_Control(pDX, IDC_BUTTON_CLEARKEY, m_ctClear);
	DDX_Control(pDX, IDC_BUTTON_SETKEY, m_ctSet);
	DDX_Control(pDX, IDC_LIST_KEYS, m_ctList);
	DDX_Control(pDX, IDC_HOTKEY_ENTER, m_ctHotkey);
	DDX_Text(pDX, IDC_STATIC_OUTPUT, m_sOutput);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBindDialog, CDialog)
	//{{AFX_MSG_MAP(CBindDialog)
	ON_NOTIFY(NM_CLICK, IDC_LIST_KEYS, OnClickListKeys)
	ON_BN_CLICKED(IDC_BUTTON_SETKEY, OnButtonSetkey)
	ON_BN_CLICKED(IDC_BUTTON_CLEARKEY, OnButtonClearkey)
	ON_BN_CLICKED(IDC_BUTTON_UNBINDALL, OnButtonUnbindall)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBindDialog message handlers

BOOL CBindDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	int n = 0;

	CRect rcSize;
	m_ctList.GetClientRect(&rcSize);
	
	m_ctList.InsertColumn(0, "Key", LVCFMT_LEFT, 80);
	m_ctList.InsertColumn(1, "Function", LVCFMT_LEFT, rcSize.right - 80);

	m_pTable = new ACCEL[30];
	if(m_pTable == NULL)
	{
		Error("Memory Allocation Error: Bind Dialog");
		return false;
	}
	
	///////////
	//IMPORTANT: make sure the array size is changed when new items are added
	///////////
	InsertItem(IDT_MODE_POINTER,			"Pointer Mode",				"",	n++);
	InsertItem(IDT_MODE_CAMERA,				"Camera Mode",				"",	n++);
	InsertItem(IDT_MODE_ZOOM,				"Zoom Mode",				"",	n++);
	InsertItem(IDT_MODE_PAINT,				"Paint Mode",				"", n++);
	InsertItem(IDT_MODE_ENTITY,				"Entity Mode",				"",	n++);
	InsertItem(IDT_MODE_BRUSH,				"Brush Mode",				"",	n++);
	InsertItem(IDT_MODE_FACE,				"Face Mode",				"",	n++);
	InsertItem(IDT_MODE_VERTEX,				"Vertex Mode",				"",	n++);
	InsertItem(IDT_MODE_CLIP,				"Clip Mode",				"",	n++);
	InsertItem(IDT_MODE_TEXLOCK,			"Texture Lock",				"",	n++);
	InsertItem(IDT_RUNMAP,					"Build / Run Map",			"",	n++);

	InsertItem(IDM_VIEW_EXPANDCURRENT,		"Expand / Collapse View",	"", n++);
	InsertItem(IDT_VIEW_TOPVIEW,			"Switch - Top View",		"",	n++);
	InsertItem(IDT_VIEW_SIDEVIEW,			"Switch - Side View",		"",	n++);
	InsertItem(IDT_VIEW_FRONTVIEW,			"Switch - Front View",		"",	n++);
	InsertItem(IDM_VIEW_CENTER,				"Center Views On Item",		"", n++);
	InsertItem(ID_GRID_SNAP,				"On/Off - Snap To Grid",	"", n++);
	InsertItem(ID_GRID_SMALLER,				"Grid Size - Smaller",		"", n++);
	InsertItem(ID_GRID_LARGER,				"Grid Size - Larger",		"", n++);
	
	InsertItem(IDT_OBJECT_GROUP,			"Group Objects",			"",	n++);
	InsertItem(IDT_OBJECT_UNGROUP,			"Ungroup Objects",			"",	n++);
	InsertItem(IDT_OBJECT_CARVE,			"Carve Brush(es)",			"",	n++);
	InsertItem(IDT_OBJECT_HOLLOW,			"Hollow Brush(es)",			"",	n++);
	InsertItem(IDT_OBJECT_FLIP_VERTICAL,	"Flip Vertical",			"",	n++);
	InsertItem(IDT_OBJECT_FLIP_HORIZONTAL,	"Flip Horizontal",			"",	n++);
	InsertItem(IDT_OBJECT_ROTATE_90_CW,		"Rotate 90 Clock-Wise",		"",	n++);
	InsertItem(IDT_OBJECT_ROTATE_90_CCW,	"Rotate 90 Counter Clock-Wise",	"",	n++);
	InsertItem(IDT_OBJECT_ROTATE_180_CW,	"Rotate 180",				"",	n++);
	InsertItem(IDT_OBJECT_LINK_ENTITY,		"Link Brush(es) to Entity",	"",	n++);
	InsertItem(IDT_OBJECT_UNLINK_ENTITY,	"Unlink Brush(es)",			"",	n++);
	InsertItem(IDT_OBJECT_PROPERTIES,		"Entity Properties",		"",	n++);

	///////////
	//IMPORTANT: make sure the array size is changed when new items are added
	///////////
	
	m_nCount = n;

	if(m_pOldTable != NULL)
	{
		delete [] m_pOldTable;
		m_pOldTable = NULL;
		m_nOldCount = 0;
	}

	m_ctList.SetTextColor(RGB(20, 20, 200));

	m_ctHotkey.EnableWindow(false);
	m_ctSet.EnableWindow(false);
	m_ctClear.EnableWindow(false);
	m_ctHotkey.SetRules(HKCOMB_CA
						| HKCOMB_C
						| HKCOMB_A 
						| HKCOMB_S 
						| HKCOMB_SA 
						| HKCOMB_SC 
						| HKCOMB_SCA, 0);

	m_sOutput = "Select an action to bind a key to...";
	UpdateData(false);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CBindDialog::InsertItem(UINT nID, CString sText, CString sDefault, int nItem)
{
	CString sKey;
	int nKey;
	int nPlace;

	nKey = GetOldKeys(nID, sKey);
	if(nKey == 0)
		sKey = sDefault;
	
	nPlace = m_ctList.InsertItem(nItem, sKey, -1);
	if(nPlace == -1)
	{
		Warning("Initializing key binding failed!\n");
		return;
	}

	m_ctList.SetItemText(nPlace, 1, sText);

	m_pTable[nItem].cmd = nID;
	m_pTable[nItem].fVirt = 0;
	m_pTable[nItem].key = nKey;
}

void CBindDialog::OnClickListKeys(NMHDR* pNMHDR, LRESULT* pResult) 
{
	m_nPos = m_ctList.GetSelection();
	if(m_nPos < 0)
	{
		m_ctSet.EnableWindow(false);
		m_ctHotkey.EnableWindow(false);
		m_ctClear.EnableWindow(false);
		return;
	}

	m_ctList.EditLabel(m_nPos);
	m_ctHotkey.EnableWindow(true);
	m_ctSet.EnableWindow(true);
	m_ctClear.EnableWindow(true);

	if(m_pTable != NULL)
		m_ctHotkey.SetHotKey(m_pTable[m_nPos].key, 0);

	m_ctHotkey.SetFocus();

	m_sOutput = "Press a key on the keyboard and click Bind (or press Enter)...";
	UpdateData(false);

	*pResult = 0;
}

void CBindDialog::OnButtonSetkey() 
{
	if(m_pTable == NULL)
		return;

	unsigned short nKey;
	unsigned short nMod;
	CString sBuffer;

	m_ctHotkey.GetHotKey(nKey, nMod);

	//check if the key is already bound
	for(int n = 0; n < m_nCount; n++)
	{
		if(nKey == m_pTable[n].key)
		{
			sBuffer.Format("Key already bound to: %s", m_ctList.GetItemText(n, 1));
			Error(sBuffer);
			m_ctHotkey.SetFocus();
			return;
		}
	}

	nKey &= 0xFF;
	if(VtoS(nKey, sBuffer))
	{
		m_pTable[m_nPos].fVirt = 0;		//FVIRTKEY;
		m_pTable[m_nPos].key = nKey; 	//KEY

		m_ctList.SetItemText(m_nPos, 0, sBuffer);

		m_ctHotkey.EnableWindow(false);
		m_ctSet.EnableWindow(false);
		m_ctClear.EnableWindow(false);
	}
	else
	{
		m_ctHotkey.SetHotKey(0, 0);
		m_ctHotkey.SetFocus();
	}

	m_sOutput = "Select an action to bind a key to...";
	UpdateData(false);
}

bool CBindDialog::VtoS(short nKey, CString &sTemp)
{
	if(m_pTable == NULL)
		return false;
	
	if(((nKey >= 65) && (nKey <= 90)) || ((nKey >= 48) && (nKey <= 57)))
	{
		sTemp += (char)nKey;
		return true;
	}
	
	switch(nKey)
	{
	case 113:
		sTemp += "F2";
		break;
	case 114:
		sTemp += "F3";
		break;
	case 115:
		sTemp += "F4";
		break;
	case 116:
		sTemp += "F5";
		break;
	case 117:
		sTemp += "F6";
		break;
	case 118:
		sTemp += "F7";
		break;
	case 119:
		sTemp += "F8";
		break;
	case 120:
		sTemp += "F9";
		break;
	case 121:
		sTemp += "F10";
		break;
	case 122:
		sTemp += "F11";
		break;
	case 123:
		sTemp += "F12";
		break;
	case 36:
		sTemp += "HOME";
		break;
	case 35:
		sTemp += "END";
		break;
	case 33:
		sTemp += "PGUP";
		break;
	case 34:
		sTemp += "PGDOWN";
		break;
	case 45:
		sTemp += "INSERT";
		break;
	case 37:
		sTemp += "LEFT";
		break;
	case 39:
		sTemp += "RIGHT";
		break;
	case 38:
		sTemp += "UP";
		break;
	case 40:
		sTemp += "DOWN";
		break;
	case 219:
	case 220:
	case 221:
	case 186:
	case 222:
	case 188:
	case 190:
	case 191:
		sTemp = (char)(nKey & ~0x80);
		//sTemp = (char)nKey;
		break;
	default:
		return false;
	}

	return true;
}

int CBindDialog::GetOldKeys(int nID, CString &sBuffer)
{
	if(m_pOldTable == NULL)
		return 0;

	int nKey = -1;

	for(int n = 0; n < m_nOldCount; n++)
	{
		if(nID == m_pOldTable[n].cmd)
		{
			nKey = m_pOldTable[n].key;
		}
	}

	if(nKey == -1)
		return 0;
	
	if((nKey >= 65) && (nKey <= 90))
	{
		sBuffer = (char)nKey;
		return nKey;
	}
	else if((nKey >= 48) && (nKey <= 57))
	{
		sBuffer = (char)nKey;
		return nKey;
	}
	
	switch(nKey)
	{
	case 113:
		sBuffer = "F2";
		break;
	case 114:
		sBuffer = "F3";
		break;
	case 115:
		sBuffer = "F4";
		break;
	case 116:
		sBuffer = "F5";
		break;
	case 117:
		sBuffer = "F6";
		break;
	case 118:
		sBuffer = "F7";
		break;
	case 119:
		sBuffer = "F8";
		break;
	case 120:
		sBuffer = "F9";
		break;
	case 121:
		sBuffer = "F10";
		break;
	case 122:
		sBuffer = "F11";
		break;
	case 123:
		sBuffer = "F12";
		break;
	case 36:
		sBuffer = "HOME";
		break;
	case 35:
		sBuffer = "END";
		break;
	case 33:
		sBuffer = "PGUP";
		break;
	case 34:
		sBuffer = "PGDOWN";
		break;
	case 45:
		sBuffer = "INSERT";
		break;
	case 37:
		sBuffer = "LEFT";
		break;
	case 39:
		sBuffer = "RIGHT";
		break;
	case 38:
		sBuffer = "UP";
		break;
	case 40:
		sBuffer = "DOWN";
		break;
	case 219:
	case 220:
	case 221:
	case 186:
	case 222:
	case 188:
	case 190:
	case 191:
		sBuffer = (char)(nKey & ~0x80);
		break;
	}

	 return nKey;
}

void CBindDialog::SetTableHandle(ACCEL* pHandle, int nCount)
{
	m_pOldTable = pHandle;
	m_nOldCount = nCount;
}

int CBindDialog::GetTableCount()
{
	return m_nCount;
}

ACCEL* CBindDialog::GetTableRef()
{
	return m_pTable;
}

void CBindDialog::OnButtonClearkey() 
{
	if(m_pTable == NULL)
		return;

	m_pTable[m_nPos].key = 0;
	m_ctList.SetItemText(m_nPos, 0, "");
	m_ctHotkey.EnableWindow(false);
	m_ctSet.EnableWindow(false);
	m_ctClear.EnableWindow(false);

	m_sOutput = "Select an action to bind a key to...";
	UpdateData(false);
}

void CBindDialog::OnButtonUnbindall() 
{
	if(MessageBox("Are you sure you want to unbind all keys?", "Unbind Confirmation", MB_YESNOCANCEL | MB_ICONQUESTION) != IDYES)
		return;

	for(int n = 0; n < m_nCount; n++)
	{
		m_pTable[n].key = 0;
		m_ctList.SetItemText(n, 0, "");
	}
	
	m_ctHotkey.EnableWindow(false);
	m_ctSet.EnableWindow(false);
	m_ctClear.EnableWindow(false);

	m_sOutput = "Select an action to bind a key to...";
	UpdateData(false);
}
