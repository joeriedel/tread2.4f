// RegisterDialog.cpp : implementation file
//

#include "stdafx.h"
#include "tread3d.h"
#include "RegisterDialog.h"
#include "TreadSys.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRegisterDialog dialog


CRegisterDialog::CRegisterDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CRegisterDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRegisterDialog)
	m_sKey = _T("");
	m_sName = _T("");
	m_sEmail = _T("");
	//}}AFX_DATA_INIT

	m_pClient = NULL;
	m_bRegister = false;
	//m_sAddress = "24.5.139.131";
	m_sAddress = "Titan";
	m_nPort = 17834;
}


void CRegisterDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRegisterDialog)
	DDX_Control(pDX, IDC_EDIT_KEY, m_edKey);
	DDX_Control(pDX, IDC_LIST_MESSAGE, m_lbMessage);
	DDX_Control(pDX, IDC_PROGRESS_CONNECT, m_pgConnect);
	DDX_Control(pDX, IDC_BUTTON_DONE, m_btDone);
	DDX_Control(pDX, IDC_BUTTON_CONNECT, m_btConnect);
	DDX_Text(pDX, IDC_EDIT_KEY, m_sKey);
	DDX_Text(pDX, IDC_EDIT_NAME, m_sName);
	DDX_Text(pDX, IDC_EDIT_EMAIL, m_sEmail);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRegisterDialog, CDialog)
	//{{AFX_MSG_MAP(CRegisterDialog)
	ON_EN_CHANGE(IDC_EDIT_KEY, OnChangeEditKey)
	ON_BN_CLICKED(IDC_BUTTON_CONNECT, OnButtonConnect)
	ON_BN_CLICKED(IDC_BUTTON_DONE, OnButtonDone)
	ON_WM_DESTROY()
	ON_EN_CHANGE(IDC_EDIT_NAME, OnChangeEditName)
	ON_EN_CHANGE(IDC_EDIT_EMAIL, OnChangeEditEmail)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRegisterDialog message handlers

BOOL CRegisterDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_btConnect.EnableWindow(false);
	m_btDone.SetWindowText("&Cancel");
	m_pgConnect.SetRange(0, 8);
	m_pgConnect.SetStep(1);
	m_edKey.SetFilter(FILTER_CHAR | FILTER_PUNCTUATION);
	m_sKey = "Key";
	UpdateData(false);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CRegisterDialog::OnChangeEditKey() 
{
	/*
	UpdateData(true);

	if( m_sName.IsEmpty() || m_sKey.IsEmpty() || m_sEmail.IsEmpty() )
		m_btConnect.EnableWindow(false);
	else
		m_btConnect.EnableWindow(true);
	*/
}

void CRegisterDialog::OnChangeEditName() 
{
	UpdateData(true);

	if( m_sName.IsEmpty() || m_sKey.IsEmpty() || m_sEmail.IsEmpty() )
		m_btConnect.EnableWindow(false);
	else
		m_btConnect.EnableWindow(true);
}

void CRegisterDialog::OnChangeEditEmail() 
{
	UpdateData(true);

	if(m_sName.IsEmpty() || m_sKey.IsEmpty() || m_sEmail.IsEmpty())
		m_btConnect.EnableWindow(false);
	else
		m_btConnect.EnableWindow(true);
}

void CRegisterDialog::OnButtonConnect() 
{
	CMsg msg;

	UpdateData(false);

	if( m_sName.IsEmpty() || m_sKey.IsEmpty() || m_sEmail.IsEmpty() )
	{
		Error("Name and license key cannot be blank.");
		return;
	}

	/*
	if(m_sKey.Find('-') == -1)
	{
		Error("License key requires the hyphen ( - ).");
		return;
	}
	*/

	BeginWaitCursor();
	SetCapture();

	m_btConnect.EnableWindow(false);

	if(m_pClient != NULL)
	{
		delete m_pClient;
		m_pClient = NULL;
	}

	m_pClient = new CClientSocket(this);
	if(m_pClient == NULL)
		return;

	m_lbMessage.ResetContent();
	m_pgConnect.SetPos(0);

	CString sBuffer;

	if(m_pClient->Create())
	{
		m_pgConnect.StepIt();
		m_lbMessage.AddString("This may take a few minutes on some machines.");
		m_lbMessage.AddString("Creating Client Connection");
	}
	else
	{
		m_lbMessage.AddString("Failed to create client!");
		m_btConnect.EnableWindow(true);
		delete m_pClient;
		m_pClient = NULL;
		ReleaseCapture();
		EndWaitCursor();
		return;
	}

	if(m_pClient->Connect(m_sAddress, m_nPort))
	{
		m_pgConnect.StepIt();
		m_lbMessage.AddString("Server responded >> Connection granted.");
		m_pClient->Init();
		
		m_pgConnect.StepIt();
		msg.m_sText = "101";
		m_pClient->SendMsg(&msg);

		m_pgConnect.StepIt();
		msg.m_sText = m_sEmail;
		m_pClient->SendMsg(&msg);

		m_pgConnect.StepIt();
		msg.m_sText = m_sName;
		m_pClient->SendMsg(&msg);

		m_pgConnect.StepIt();
		msg.m_sText = m_sKey;
		m_pClient->SendMsg(&msg);
	}
	else
	{
		m_lbMessage.ResetContent();
		m_lbMessage.AddString("No response from server, Try again later.");
		m_btConnect.EnableWindow(true);
		delete m_pClient;
		m_pClient = NULL;
		ReleaseCapture();
		EndWaitCursor();
		return;
	}

	ReleaseCapture();
	EndWaitCursor();
}

void CRegisterDialog::OnButtonDone() 
{
	if(m_bRegister)
		OnOK();
	else
		OnCancel();
}

void CRegisterDialog::OnDestroy() 
{
	CDialog::OnDestroy();
	
	if(m_pClient != NULL)
		delete m_pClient;
}

void CRegisterDialog::ProcessPendingRead()
{
	CMsg* pMsg;
	do
	{
		pMsg = ReadMsg(m_pClient);
	}
	while(!m_pClient->GetArchive(AR_INPUT)->IsBufferEmpty());

	if(pMsg != NULL)
	{
		int nCode = atoi(pMsg->m_sText);

		if(nCode == -1)
			m_lbMessage.AddString("Server responded >> Access Denied");
		else if(nCode == -2)
		{
			m_lbMessage.AddString("Server responded >> Connection Error");
			m_btConnect.EnableWindow(true);
		}
		else if(nCode == -3)
			m_lbMessage.AddString("Server responded >> Maximum Registration Reached");
		else if(nCode == -4)
		{
			m_lbMessage.AddString("Server responded >> Invalid License Key");
			m_btConnect.EnableWindow(true);
		}
		else if(nCode == -5)
		{
			m_lbMessage.AddString("Server responded >> Invalid version of Tread3D.");
			m_lbMessage.AddString("    Go to http://www.planetquake.com/tread");
		}
		else if(nCode >= 1)	//the login was successful
		{
			m_lbMessage.AddString("Server responded >> Login Succussful");
			m_lbMessage.AddString("Thank you for choosing Tread3D-2");
			m_bRegister = true;
			m_btDone.SetWindowText("&Thank You!");

			GetDlgItem(IDC_EDIT_NAME)->EnableWindow(false);
			GetDlgItem(IDC_EDIT_KEY)->EnableWindow(false);
			GetDlgItem(IDC_EDIT_EMAIL)->EnableWindow(false);

			SetRegString(S_USER_NAME, m_sName);
			SetRegBool(B_REGISTER, true);
			//CreateKeyFile(m_sKey);
			SaveTreadSettings();
		}

		m_pgConnect.StepIt();
		pMsg->m_bClose = true;
		m_pClient->SendMsg(pMsg);

		m_pgConnect.StepIt();
		m_pClient->Close();
		m_lbMessage.AddString("Closing connection to server.");

		delete m_pClient;
		m_pClient = NULL;
	}
}

CMsg* CRegisterDialog::ReadMsg(CClientSocket* pSocket)
{
	static CMsg msg;

	TRY
	{
		pSocket->ReceiveMsg(&msg);
	}
	CATCH(CFileException, e)
	{
		m_lbMessage.AddString("Client Read Error");

		msg.m_bClose = TRUE;
		pSocket->Abort();
	}
	END_CATCH

	return &msg;
}

void CRegisterDialog::SendMsg(CClientSocket* pSocket, CMsg* pMsg)
{
	TRY
	{
		pSocket->SendMsg(pMsg);
	}
	CATCH(CFileException, e)
	{
		pSocket->Abort();
		m_lbMessage.AddString("Client Send Error.");		
	}
	END_CATCH
}

void CRegisterDialog::CreateKeyFile(CString sKey)
{
	if(sKey.IsEmpty())
	{
		Error("Create Key : NULL string!");
		return;
	}

	CFile hFile;
	CString sPath;
	sPath.Format("%s\\data3.tread", GetRegString(S_TREAD_PATH));
	if(!hFile.Open(sPath, CFile::modeWrite | CFile::modeCreate | CFile::shareExclusive))
	{
		Error("File opening failed!");
		return;
	}

	int nLength = sKey.GetLength();

	hFile.Write(&nLength, 4);
	hFile.Write(sKey, nLength);

	hFile.Close();
}


