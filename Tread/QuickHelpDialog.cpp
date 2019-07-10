// QuickHelpDialog.cpp : implementation file
//

#include "stdafx.h"
#include "tread3d.h"
//#include "QuickHelpDialog.h"
#include "DialogControl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CQuickHelpDialog dialog


CQuickHelpDialog::CQuickHelpDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CQuickHelpDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CQuickHelpDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_sFile = "index";
	m_bInit = false;
}

void CQuickHelpDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CQuickHelpDialog)
	DDX_Control(pDX, IDC_EXPLORER, m_HtmlViewer);
	DDX_Control(pDX, IDC_BUTTON_BACK, m_btBack);
	DDX_Control(pDX, IDC_BUTTON_FORWARD, m_btForward);
	DDX_Control(pDX, IDC_BUTTON_HOME, m_btHome);
	DDX_Control(pDX, IDC_BUTTON_CLOSE, m_btClose);
	DDX_Control(pDX, IDC_BUTTON_SITE, m_btSite);
	DDX_Text(pDX, IDC_STATIC_STATUS, m_sStatus);
	DDX_Control(pDX, IDC_PROGRESS_DOWNLOAD, m_pgDownload);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CQuickHelpDialog, CDialog)
	//{{AFX_MSG_MAP(CQuickHelpDialog)
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_BUTTON_BACK, OnButtonBack)
	ON_BN_CLICKED(IDC_BUTTON_FORWARD, OnButtonForward)
	ON_BN_CLICKED(IDC_BUTTON_HOME, OnButtonHome)
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, OnButtonClose)
	ON_BN_CLICKED(IDC_BUTTON_SITE, OnButtonSite)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CQuickHelpDialog message handlers

BEGIN_EVENTSINK_MAP(CQuickHelpDialog, CDialog)
    //{{AFX_EVENTSINK_MAP(CQuickHelpDialog)
	ON_EVENT(CQuickHelpDialog, IDC_EXPLORER, 106 /* DownloadBegin */, OnDownloadBeginExplorer, VTS_NONE)
	ON_EVENT(CQuickHelpDialog, IDC_EXPLORER, 104 /* DownloadComplete */, OnDownloadCompleteExplorer, VTS_NONE)
	ON_EVENT(CQuickHelpDialog, IDC_EXPLORER, 108 /* ProgressChange */, OnProgressChangeExplorer, VTS_I4 VTS_I4)
	ON_EVENT(CQuickHelpDialog, IDC_EXPLORER, 250 /* BeforeNavigate2 */, OnBeforeNavigate2Explorer1, VTS_DISPATCH VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PBOOL)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CQuickHelpDialog::OnDownloadBeginExplorer() 
{
	m_sStatus = "Beginning download...";
	UpdateData(false);
}

void CQuickHelpDialog::OnDownloadCompleteExplorer() 
{
	m_sStatus = "Download complete.";
	UpdateData(false);
}

void CQuickHelpDialog::OnProgressChangeExplorer(long Progress, long ProgressMax)
{
	if((Progress == -1) || (ProgressMax == 0))
		m_pgDownload.SetPos(0);
	else if(Progress == 0)
		m_pgDownload.SetRange(0, ProgressMax);
	else
		m_pgDownload.SetPos(Progress);
}

void CQuickHelpDialog::OnBeforeNavigate2Explorer1(LPDISPATCH pDisp, VARIANT FAR* URL, VARIANT FAR* Flags, VARIANT FAR* TargetFrameName, VARIANT FAR* PostData, VARIANT FAR* Headers, BOOL FAR* Cancel) 
{
}

BOOL CQuickHelpDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CWinApp* pApp;
	HICON hIcon;

	pApp = AfxGetApp();

	hIcon = pApp->LoadIcon(IDI_ICON_BACK);
	m_btBack.SetIcon(hIcon);
	hIcon = pApp->LoadIcon(IDI_ICON_FORWARD);
	m_btForward.SetIcon(hIcon);
	hIcon = pApp->LoadIcon(IDI_ICON_HOME);
	m_btHome.SetIcon(hIcon);
	hIcon = pApp->LoadIcon(IDI_ICON_CLOSE);
	m_btClose.SetIcon(hIcon);
	hIcon = pApp->LoadIcon(IDI_ICON_GLOBE);
	m_btSite.SetIcon(hIcon);

	CRect rcClient1;

	//get dialog client rect
	GetClientRect(&m_rcClient);
	m_rcSize = m_rcClient;
	CalcWindowRect(m_rcSize);
	ClientToScreen(&m_rcClient);

	//put the html viewer in a position that is offset by the buttons
	m_btBack.GetClientRect(&rcClient1);

	m_ptAnchor.y = rcClient1.bottom + 10;;
	m_ptAnchor.x = 2;

	m_ptMin.y = rcClient1.bottom;
	m_ptMin.x = rcClient1.right;

	//set properties of the html control
	m_HtmlViewer.SetOffline(true);
	m_HtmlViewer.SetSilent(true);
	m_HtmlViewer.SetToolBar(1);

	m_bInit = true;
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

int CQuickHelpDialog::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	RegisterDialog(this, DLG_QUICKHELP);

	return 0;
}

void CQuickHelpDialog::OnSize(UINT nType, int cx, int cy) 
{
	if(m_bInit)
	{
		if((cx >= m_ptMin.x) && (cy >= m_ptMin.y))
		{
			CDialog::OnSize(nType, cx, cy);
			m_HtmlViewer.MoveWindow(m_ptAnchor.x, m_ptAnchor.y, cx - m_ptAnchor.x, cy - m_ptAnchor.y);
			
			GetClientRect(&m_rcClient);
			m_rcSize = m_rcClient;
			CalcWindowRect(m_rcSize);
			ClientToScreen(&m_rcClient);
		}
	}
}

void CQuickHelpDialog::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);

	if(bShow)
	{
		CString sPath;
		sPath.Format("%s/Help/%s.html", GetRegString(S_TREAD_PATH), m_sFile);
		m_HtmlViewer.Navigate(sPath, NULL, NULL, NULL, NULL);
	}
}

void CQuickHelpDialog::SetHtmlFile(CString sFile)
{
	m_sFile = sFile;
}

void CQuickHelpDialog::OnButtonBack()
{
	m_HtmlViewer.GoBack();
}

void CQuickHelpDialog::OnButtonForward()
{
	m_HtmlViewer.GoForward();
}

void CQuickHelpDialog::OnButtonHome()
{
	CString sPath;
	sPath.Format("%s/Help/%s.html", GetRegString(S_TREAD_PATH), m_sFile);
	m_HtmlViewer.Navigate(sPath, NULL, NULL, NULL, NULL);
}

void CQuickHelpDialog::OnButtonClose()
{
	OnOK();
}

void CQuickHelpDialog::OnButtonSite()
{
	m_HtmlViewer.Navigate("www.planetquake.com/tread", NULL, NULL, NULL, NULL);
}
