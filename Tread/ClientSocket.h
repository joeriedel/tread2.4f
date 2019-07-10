#if !defined(AFX_CLIENTSOCKET_H__E5779886_885E_11D3_B673_00A0CC582962__INCLUDED_)
#define AFX_CLIENTSOCKET_H__E5779886_885E_11D3_B673_00A0CC582962__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ClientSocket.h : header file
//



/////////////////////////////////////////////////////////////////////////////
// CClientSocket command target

#include <afxsock.h>
#include "msg.h"

#define AR_INPUT 1
#define AR_OUTPUT 2

class CRegisterDialog;
class CClientSocket : public CSocket
{
private:
	CSocketFile* m_pFile;
	CArchive* m_pArchiveIn;
	CArchive* m_pArchiveOut;
	
	BOOL IsAborted() { return m_pArchiveOut == NULL; }
	CRegisterDialog* m_pClientWnd;

// Attributes
public:

// Operations
public:
	CClientSocket(CRegisterDialog* pClientWnd);
	virtual ~CClientSocket();

	void Init();
	void Abort();
	void SendMsg(CMsg* pMsg);
	void ReceiveMsg(CMsg* pMsg);
	CArchive* GetArchive(UINT nCode);

// Overrides
public:
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CClientSocket)
	public:
	virtual void OnReceive(int nErrorCode);
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CClientSocket)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

// Implementation
protected:
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CLIENTSOCKET_H__E5779886_885E_11D3_B673_00A0CC582962__INCLUDED_)
