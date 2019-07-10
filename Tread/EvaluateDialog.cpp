// EvaluateDialog.cpp : implementation file
//

#include "stdafx.h"
#include "tread3d.h"
#include "EvaluateDialog.h"
#include "TreadSys.h"
#include "RegisterDialog.h"
#include "Registry.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEvaluateDialog dialog


CEvaluateDialog::CEvaluateDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CEvaluateDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEvaluateDialog)
	m_sDays = _T("");
	//}}AFX_DATA_INIT
}


void CEvaluateDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEvaluateDialog)
	DDX_Control(pDX, IDC_BUTTON_REGISTER, m_btRegister);
	DDX_Control(pDX, IDC_BUTTON_EVALUATE, m_btEvaluate);
	DDX_Control(pDX, IDC_STATIC_URL, m_stUrl);
	DDX_Text(pDX, IDC_EDIT_DAYS, m_sDays);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEvaluateDialog, CDialog)
	//{{AFX_MSG_MAP(CEvaluateDialog)
	ON_BN_CLICKED(IDC_BUTTON_EVALUATE, OnButtonEvaluate)
	ON_BN_CLICKED(IDC_BUTTON_REGISTER, OnButtonRegister)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEvaluateDialog message handlers

BOOL CEvaluateDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	bool bRet = Evaluate();

	if(!bRet)
		m_btEvaluate.EnableWindow(false);
	else
		m_btEvaluate.EnableWindow(true);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CEvaluateDialog::OnButtonEvaluate() 
{
	OnOK();
}

void CEvaluateDialog::OnButtonRegister() 
{
	CRegisterDialog dlgTemp;
	if(dlgTemp.DoModal() == IDOK)
		OnOK();
}

bool CEvaluateDialog::Evaluate()
{
	CFile hFile;
	bool bFile;
	bool bReg;
	bool bEval = true;
	int nKey;
	CTime oTime = CTime::GetCurrentTime();
	CTime* pastTime;
	CTimeSpan* timeSpan;
	CTimeSpan diff;
	char sPath[256];
	int nTemp;
	int nDays;
	CRegistry reg;
	int nCheck;
		
	GetSystemDirectory(sPath, 128);
	strcat(sPath, "\\prvea32.dll");
	//open the file
	bFile = hFile.Open(sPath, CFile::modeRead | CFile::modeNoTruncate, NULL);
	if(bFile)
	{
		//open the registry keys
		hFile.Read(&nCheck, sizeof(int));

		hFile.Close();
	}
	else
		bFile = false;

	bReg = reg.Open(HKEY_LOCAL_MACHINE, "Software\\prve");
	if(bReg)
	{
		bReg = reg.Read("Size", nKey);
		reg.Close();
	}
	else
		bReg = false;

	if(bReg && bFile)	//continued use
	{
		if(nKey == nCheck)
		{
			//get the number of days
			nDays = (nCheck & 0xF000) >> 12;

			timeSpan = new CTimeSpan(nDays, 0, 0, 0);
			pastTime = new CTime(nCheck & 0xFFF, (nCheck & 0xFF000000) >> 24, (nCheck & 0xFF0000) >> 16, 0, 0, 0);

			if(oTime < *pastTime)
			{
				nDays = 0;
				bEval = false;
				AfxMessageBox("We have noticed that you have set\n your clock back to violate the evaluation period.\nWe would like to punish you severely but we won't.\nPUT IT BACK!", MB_OK, 0);
				return bEval;
			}
			else
			{
				diff = oTime - *pastTime;
				nDays = nDays - diff.GetDays();

				if(nDays <= 0)
					bEval = false;
			}
			delete pastTime;
			delete timeSpan;
		}
		else
			bEval = false;
	}
	else if(!bReg && !bFile)	//first time use
	{
		nDays = 10;
	}
	else
	{
		bEval = false;
	}

	if(bEval)
	{
		nTemp = oTime.GetYear();
		nTemp |= nDays << 12;
		nTemp |= oTime.GetDay() << 16;
		nTemp |= oTime.GetMonth() << 24;

		bFile = hFile.Open(sPath, CFile::modeWrite | CFile::modeCreate | CFile::modeNoTruncate, NULL);
		if(bFile)
		{
			hFile.Write(&nTemp, sizeof(int));
			hFile.Close();
		}

		bReg = reg.CreateKey(HKEY_LOCAL_MACHINE, "Software\\prve");

		bReg = reg.Open(HKEY_LOCAL_MACHINE, "Software\\prve");
		if(bReg)
		{
			reg.Write("Size", nTemp);
			reg.Close();
		}
	}

	if(!bEval)
	{
		AfxMessageBox("Your evaluation period has expired. You must register Tread to continue using it.", MB_OK, 0);
		nDays = 0;
	}

	if(nDays <= 0)
		m_sDays = "Expired";
	else
		m_sDays.Format("%u", nDays);

	UpdateData(false);

	return bEval;
}
