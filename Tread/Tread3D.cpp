// Tread3D.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "Tread3D.h"

#include "MainFrm.h"
#include "ChildFrm.h"
#include "Tread3DDoc.h"
#include "Tread3DView.h"
#include "Splash.h"
#include "TreadSys.h"
#include "cpu_lib.h"
#include "StartupDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#pragma warning( disable : 4800 )

#include <cj60lib.h>
#include "EvaluateDialog.h"
#include "RegisterDialog.h"
#include "LicenseDialog.h"
/////////////////////////////////////////////////////////////////////////////
// CTread3DApp

BEGIN_MESSAGE_MAP(CTread3DApp, CWinApp)
	ON_COMMAND(CG_IDS_TIPOFTHEDAY, ShowTipOfTheDay)
	//{{AFX_MSG_MAP(CTread3DApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTread3DApp construction

CTread3DApp::CTread3DApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CTread3DApp object

CTread3DApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CTread3DApp initialization

BOOL CTread3DApp::InitInstance()
{
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);
	CSplashWnd::EnableSplashScreen(cmdInfo.m_bShowSplash);

	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	// Change the registry key under which our settings are stored.
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization.
	SetRegistryKey(_T("Prospect Vector"));
	LoadStdProfileSettings();  // Load standard INI file options (including MRU)
	LoadSettings();

	//check for a key file and verify it.
	//if(!VerifyKeyFile())
	{
		//init the socket interface
		//AfxSocketInit(&lpwsaData);

		//CEvaluateDialog dlgTemp;
		//if(dlgTemp.DoModal() != IDOK)
		//	return false;
	}

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CMultiDocTemplate(
		IDR_TREAD3TYPE,
		RUNTIME_CLASS(CTread3DDoc),
		RUNTIME_CLASS(CMapFrame), // custom MDI child frame
		RUNTIME_CLASS(CTread3DView));
	AddDocTemplate(pDocTemplate);

	// create main MDI Frame window
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame->LoadFrame(IDR_MAINFRAME))
		return FALSE;
	m_pMainWnd = pMainFrame;

	// Enable drag/drop open
	m_pMainWnd->DragAcceptFiles();

	// Enable DDE Execute open
	EnableShellOpen();
	RegisterShellFileTypes(TRUE);

	// Parse command line for standard shell commands, DDE, file open
	//CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);
	cmdInfo.m_nShellCommand = CCommandLineInfo::FileNothing;

	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The main window has been initialized, so show and update it.
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();

	/*if(GetRegBool(B_FIRST_RUN))
	{
		CStartupDialog dlg;
		dlg.DoModal();
	}*/

	// CG: This line inserted by 'Tip of the Day' component.
	ShowTipAtStartup();

	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	CHyperLink	m_stEmail4;
	CButton	m_btClose;
	CHyperLink	m_stEmail3;
	CHyperLink	m_stEmail1;
	CHyperLink	m_stEmail2;
	CHyperLink	m_stWebpage;
	CString	m_sClock;
	CString	m_sCpu;
	CString	m_sFeatures;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonClose();
	afx_msg void OnButtonRegister();
	afx_msg void OnButtonLicense();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	m_sClock = _T("");
	m_sCpu = _T("");
	m_sFeatures = _T("");
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	DDX_Control(pDX, IDC_STATIC_EMAIL4, m_stEmail4);
	DDX_Control(pDX, IDC_BUTTON_CLOSE, m_btClose);
	DDX_Control(pDX, IDC_STATIC_EMAIL3, m_stEmail3);
	DDX_Control(pDX, IDC_STATIC_EMAIL1, m_stEmail1);
	DDX_Control(pDX, IDC_STATIC_EMAIL2, m_stEmail2);
	DDX_Control(pDX, IDC_STATIC_WEBPAGE, m_stWebpage);
	DDX_Text(pDX, IDC_STATIC_CLOCK, m_sClock);
	DDX_Text(pDX, IDC_STATIC_CPU, m_sCpu);
	DDX_Text(pDX, IDC_STATIC_REVISION, m_sFeatures);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, OnButtonClose)
	ON_BN_CLICKED(IDC_BUTTON_REGISTER, OnButtonRegister)
	ON_BN_CLICKED(IDC_BUTTON_LICENSE, OnButtonLicense)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CTread3DApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CTread3DApp message handlers


BOOL CTread3DApp::PreTranslateMessage(MSG* pMsg)
{
	/*if (CSplashWnd::PreTranslateAppMessage(pMsg))
		return TRUE;*/

	// Kill it?
	/*if(pMsg->message == WM_CHAR)
	{
		Console("Keychar");
		if(LOWORD(pMsg->lParam) > 1)
		{
			Console("Ignored");
			return TRUE;	// ignore;
		}
	}*/
	// We filter multiple keydown messages cause it brings
	// mfc's event system to a crawl, which hurts us for updating.
	if(pMsg->message == WM_KEYDOWN)
	{
		//Console("Keychar: %i", pMsg->lParam&0x40000000);
		if((pMsg->lParam&0x40000000) != 0)
		{
			//Console("Ignored");
			return TRUE;	// ignore;
		}
	}

	return CWinApp::PreTranslateMessage(pMsg);
}

void CTread3DApp::ShowTipAtStartup(void)
{
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);
	if (cmdInfo.m_bShowSplash)
	{
		CTipDlg dlg;
		if (dlg.m_bStartup)
			dlg.DoModal();
	}

}

void CTread3DApp::ShowTipOfTheDay(void)
{
	CTipDlg dlg;
	dlg.DoModal();
}

void CTread3DApp::LoadSettings()
{
	//NOTE: Read only keys are not to be added to SaveSettings. if they are then the key will
	//be written out when tread closes. These keys are semi secret and not intended for our
	//users to see. There intent is for debug purposes.

	//load the strings
	m_pString[S_TREAD_PATH] = GetProfileString("Settings", "Path", "");
	m_pString[S_USER_NAME] = GetProfileString("Settings", "User", "UN-LICENSED COPY");
	m_pString[S_3D_PAK] = GetProfileString("Settings", "3D_pak", "dflt_wogl");
	m_pString[S_2D_PAK] = GetProfileString("Settings", "2D_pak", "wingdi");
	
	//load the ints
	m_pInt[I_BACKPLANE] = GetProfileInt("Settings", "BackPlane", 1000);
	m_pInt[I_GAMMA] = GetProfileInt("Settings", "Gamma", 100);
	m_pInt[I_MAXMAP] = GetProfileInt("Settings", "MaxMap", 6);
	m_pInt[I_CURVESUBDEPTH] = GetProfileInt("Settings", "CurveSubDepth", 3);
	m_pInt[I_TEX_MEMORY] = GetProfileInt("Settings", "TextureMemory", 8388608);

	//load the bools
	m_pBool[B_FIRST_RUN] = GetProfileInt("Settings", "FirstRun", true);
	m_pBool[B_VISGROUP_APPLY] = GetProfileInt("Settings", "VisgroupApply", true);
	m_pBool[B_DEBUG] = GetProfileInt("Settings", "Debug", false);
	m_pBool[B_SMARTNAME] = GetProfileInt("Settings", "SmartName", false);
	m_pBool[B_SELECTIONMASK] = GetProfileInt("Settings", "SelectionMask", true);
	m_pBool[B_FACEAUTOAPPLY] = GetProfileInt("Settings", "FaceAutoApply", true);
	m_pBool[B_NOSPLASH] = GetProfileInt("Settings", "NoSplashScreen", false);	//READ ONLY
	m_pBool[B_LOAD_PLUGINS] = GetProfileInt("Settings", "LoadPlugins", true);	//READ ONLY
	m_pBool[B_REGISTER] = GetProfileInt("Settings", "Registered", false);		//READ ONLY
	m_pBool[B_IE4] = GetProfileInt("Settings", "IsIE4", false);					//READ ONLY
	m_pBool[B_ANIMATECOLORS] = GetProfileInt("Settings", "AnimateColors", true);
}

void CTread3DApp::SaveSettings()
{
	//save the strings
	WriteProfileString("Settings", "Path", m_pString[S_TREAD_PATH]);
	WriteProfileString("Settings", "User", m_pString[S_USER_NAME]);
	WriteProfileString("Settings", "3D_pak", m_pString[S_3D_PAK]);
	WriteProfileString("Settings", "2D_pak", m_pString[S_2D_PAK]);

	//save the ints
	WriteProfileInt("Settings", "BackPlane", m_pInt[I_BACKPLANE]);
	WriteProfileInt("Settings", "Gamma", m_pInt[I_GAMMA]);
	WriteProfileInt("Settings", "MaxMap", m_pInt[I_MAXMAP]);
	WriteProfileInt("Settings", "CurveSubDepth", m_pInt[I_CURVESUBDEPTH]);
	WriteProfileInt("Settings", "TextureMemory", m_pInt[I_TEX_MEMORY]);

	//save the bools
	WriteProfileInt("Settings", "FirstRun", false/*m_pBool[B_FIRST_RUN]*/);
	WriteProfileInt("Settings", "VisgroupApply", m_pBool[B_VISGROUP_APPLY]);
	WriteProfileInt("Settings", "Debug", m_pBool[B_DEBUG]);
	WriteProfileInt("Settings", "SmartName", m_pBool[B_SMARTNAME]);
	WriteProfileInt("Settings", "SelectionMask", m_pBool[B_SELECTIONMASK]);
	WriteProfileInt("Settings", "FaceAutoApply", m_pBool[B_FACEAUTOAPPLY]);
	WriteProfileInt("Settings", "AnimateColors", m_pBool[B_ANIMATECOLORS]);

	if(m_pBool[B_REGISTER])
		WriteProfileInt("Settings", "Registered", m_pBool[B_FACEAUTOAPPLY]);
}

int CTread3DApp::VerifyKeyFile()
{
	CFile hFile;
	int nLength = 0;
	char pString[65];
	CString sPath;

	sPath.Format("%s\\data3.tread", GetRegString(S_TREAD_PATH));
	if(!hFile.Open(sPath, CFile::modeRead | CFile::modeNoTruncate | CFile::shareExclusive))
	{
		//Error("File opening failed!");
		return 0;
	}

	hFile.Read(&nLength, 4);
	hFile.Read(pString, nLength);
	pString[nLength] = '\0';

	CString sName = GetRegString(S_USER_NAME);
	hFile.Close();

	if(VerifyKey(sName, pString))
		return 1;
	else
		return 0;
}

bool CTread3DApp::VerifyKey(CString &sUser, CString sKey)
{
	unsigned int nKey1 = 0;
	unsigned int nKey2 = 0;

	CString sBuffer;

	unsigned int nTemp;
	unsigned int nSum = 0;
	unsigned int nLength;
	unsigned int nSplit;

	nLength = sUser.GetLength();

	nSplit = sKey.Find('-');

	sBuffer = sKey.Left(nSplit);
	nKey1 = atoi((LPCSTR)sBuffer);

	sBuffer = sKey.Right(sKey.GetLength() - 1 - nSplit);
	nKey2 = atoi((LPCSTR)sBuffer);

	_asm
	{
		mov eax, nKey1	;move the value in the variable to the EAX register
		bswap eax		;byte swap the value
		mov nKey1, eax	;move the value in EAX to the variable
	}

	_asm
	{
		mov eax, nKey2	;move the value in the variable to the EAX register
		bswap eax		;byte swap the value
		mov nKey2, eax	;move the value in EAX to the variable
	}

	//Verify all the parts of the first key
	nTemp = (nKey1 & 0xFFFF0000) >> 16;
	for(unsigned int n = 0; n < nLength; n++)
		nSum += sUser.GetAt(n) + 2 * n;
	if(nSum != nTemp)
		return false;
		
	nTemp = (nKey1 & 0x0000F000) >> 12;
	if((nTemp < 1)  && (nTemp > 12))
		return false;

	nTemp = (nKey1 & 0x00000FC0) >> 6;
	if((nTemp < 0)  && (nTemp > 59))
		return false;

	if((nKey1 & 0x0000003F) != nLength)
		return false;

	//verify all the parts of the second key
	nTemp = ((nKey2 & 0xFF000000) >> 24) / 2;
	if(nTemp != (unsigned int)sUser.GetAt(nLength - 1))
		return false;

	nTemp = ((nKey2 & 0x00FF0000) >> 16) / 2;
	if(nTemp != (unsigned int)sUser.GetAt(0))
		return false;

	nTemp = (nKey2 & 0x0000F800) >> 11;
	if((nTemp < 1)  && (nTemp > 31))
		return false;

	nTemp = (nKey2 & 0x000007C0) >> 6;
	if((nTemp < 0)  && (nTemp > 23))
		return false;

	nTemp = (nKey2 & 0x0000003F);
	if((nTemp < 0)  && (nTemp > 59))
		return false;
	
	return true;
}

HBRUSH CAboutDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	return hbr;
}

BOOL CAboutDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	HICON hIcon;

	hIcon = AfxGetApp()->LoadIcon(IDI_ICON_ABOUTCLOSE);
	m_btClose.SetIcon(hIcon);

	BeginWaitCursor();
	SetCapture();

	m_sClock.Format("%u MHz", GetCpuSpeed());
	CPU_INFO cpu;
	GetCpuInfo(cpu);
	m_sCpu = GetCpuString(cpu);
	
	if(GetCpuFeature(cpu, MMX))
		m_sFeatures = "MMX  ";

	if(GetCpuFeature(cpu, FPU))
		m_sFeatures += "FPU  ";
	
	UpdateData(false);

	ReleaseCapture();
	EndWaitCursor();
	
	return TRUE;	// return TRUE unless you set the focus to a control
					// EXCEPTION: OCX Property Pages should return FALSE
}

void CAboutDlg::OnButtonClose() 
{
	OnOK();
}

void CAboutDlg::OnButtonRegister() 
{
	CLicenseDialog dlgLicense;
	if(dlgLicense.DoModal() == IDCANCEL)
		return;

	CRegisterDialog dlgRegister;
	dlgRegister.DoModal();
}

void CAboutDlg::OnButtonLicense() 
{
	CLicenseDialog dlgLicense;
	dlgLicense.HideCancelButton(true);
	dlgLicense.DoModal();
}
