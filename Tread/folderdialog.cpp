// FolderDialog.cpp : implementation file
//

#include "stdafx.h"
#include "compinfo.h"
#include "FolderDialog.h"
#include <dos.h>
#include <direct.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CCompInfoApp theApp;
/////////////////////////////////////////////////////////////////////////////
// CFolderDialog dialog


CFolderDialog::CFolderDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CFolderDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFolderDialog)
	m_sPath = _T("");
	m_sDir = _T("");
	m_sFile = _T("");
	m_sSubFile = _T("");
	m_sTotal = _T("");
	m_sPercent = _T("");
	m_sSize = _T("");
	//}}AFX_DATA_INIT
}


void CFolderDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFolderDialog)
	DDX_Control(pDX, IDC_SPIN_SIZE, m_spSize);
	DDX_Control(pDX, IDC_PROGRESS_USED, m_prgUsed);
	DDX_Control(pDX, IDC_TREE_FILEVIEW, m_ctTree);
	DDX_Text(pDX, IDC_STATIC_PATH, m_sPath);
	DDX_Text(pDX, IDC_STATIC_DIR, m_sDir);
	DDX_Text(pDX, IDC_STATIC_FILE, m_sFile);
	DDX_Text(pDX, IDC_STATIC_SUBFILES, m_sSubFile);
	DDX_Text(pDX, IDC_STATIC_TOTALFILES, m_sTotal);
	DDX_Text(pDX, IDC_STATIC_USED, m_sPercent);
	DDX_Text(pDX, IDC_STATIC_SIZE, m_sSize);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFolderDialog, CDialog)
	//{{AFX_MSG_MAP(CFolderDialog)
	ON_WM_CTLCOLOR()
	ON_WM_DESTROY()
	ON_WM_SETCURSOR()
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_FILEVIEW, OnSelchangedTreeFileview)
	ON_WM_HSCROLL()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFolderDialog message handlers

HBRUSH CFolderDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	switch(nCtlColor)
	{
	case CTLCOLOR_STATIC:
		pDC->SetTextColor(theApp.clText[0]);
		break;
	case CTLCOLOR_LISTBOX:
		pDC->SetTextColor(theApp.clText[1]);
		break;
	case CTLCOLOR_EDIT:
		pDC->SetTextColor(theApp.clText[1]);
		break;
	}
	// TODO: Return a different brush if the default is not desired
	return hbr;
}

void CFolderDialog::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
}

BOOL CFolderDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	BeginWaitCursor();
	SetCapture();

	m_spSize.SetRange(0, 3);
	m_spSize.SetPos(2);

	m_prgUsed.SetRange(0, 100);
	m_imgDrives.Create (IDB_BITMAP_DRIVES, 16, 1, RGB (255, 0, 255));
    m_ctTree.SetImageList (&m_imgDrives, TVSIL_NORMAL);
	m_ctTree.InitTree();

	ReleaseCapture();
	EndWaitCursor();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CFolderDialog::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	// TODO: Add your message handler code here and/or call default
	HCURSOR cursor;
	cursor = theApp.LoadCursor(IDC_CURSOR_ARROW);
	if(cursor == NULL)
		return CDialog::OnSetCursor(pWnd, nHitTest, message);

	SetCursor(cursor);
	
	return TRUE;
}

void CFolderDialog::OnSelchangedTreeFileview(NMHDR* pNMHDR, LRESULT* pResult) 
{
	HANDLE hFind;
	WIN32_FIND_DATA fd;
	int nFile = 0,
		nDir = 0,
		nTemp = 0,
		nExFile = 0,
		nRem;		
	__int64 nSize = 0;
	char sArray[16];
	CString sBuffer, sBuffer2;
	__int64 nSpace, nTotal;
	ULARGE_INTEGER x, y, z;
	
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	m_sPath = m_ctTree.GetSelectedPath(pNMHDR, pResult);
	if(m_sPath.IsEmpty())
		return;

	BeginWaitCursor();
	SetCapture();

	sBuffer = m_sPath;
	nTemp = sBuffer.GetLength() - 1;
	if(sBuffer.GetAt(nTemp) == '\\')
		sBuffer += "*.*";
	else
		sBuffer += "\\*.*";

	hFind = ::FindFirstFile((LPCSTR)sBuffer, &fd);

	if(hFind != INVALID_HANDLE_VALUE)
	{
		if(strcmp(fd.cFileName, "."))
		{
			if(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				nDir++;
				sBuffer2 = sBuffer;
				sBuffer2.Delete(sBuffer.GetLength() - 3, 3);
				sBuffer2 += fd.cFileName;
				sBuffer2 += "\\*.*";
				FollowSub(sBuffer2, nSize, nExFile, nDir, fd);
			}
			else
				nFile++;
		}
		while(::FindNextFile(hFind, &fd))
		{
			if(!strcmp(fd.cFileName, ".."))
				continue;
			
			//add the new files size
			if(fd.nFileSizeHigh != 0)
				nTemp = (fd.nFileSizeHigh * MAXDWORD) + fd.nFileSizeLow;
			else
				nTemp = fd.nFileSizeLow;

			//if((nTemp % 1024) != 0)
			//	nTemp = nTemp / 1024 + 1024

			nSize += nTemp;

			//Blank out all attributes accept directory
			//if it exists then handle the sub directory
			fd.dwFileAttributes &= FILE_ATTRIBUTE_DIRECTORY;
			if(fd.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)
			{
				nDir++;
				sBuffer2 = sBuffer;
				sBuffer2.Delete(sBuffer.GetLength() - 3, 3);
				sBuffer2 += fd.cFileName;
				sBuffer2 += "\\*.*";
				FollowSub(sBuffer2, nSize, nExFile, nDir, fd);
			}
			else
				nFile++;
		}
		::CloseHandle(hFind);
	}

	itoa(nFile, sArray, 10);
	m_sFile = sArray;

	itoa(nExFile, sArray, 10);
	m_sSubFile = sArray;

	itoa(nFile, sArray, 10);
	m_sFile = sArray;

	itoa(nFile + nExFile, sArray, 10);
	m_sTotal = sArray;
	
	itoa(nDir, sArray, 10);
	m_sDir = sArray;

	m_nSize = nSize;

	if(nSize == 0)
	{
		m_sSize = "Empty";
		m_sPercent = "";
		m_prgUsed.SetPos(0);
		UpdateData(false);
		ReleaseCapture();
		EndWaitCursor();
		return;
	}
	
	nTemp = (int)m_sPath.GetAt(0) - 64;

	if(GetDiskFreeSpaceEx((LPCSTR)m_sPath, &x, &y, &z))
	{
		nSpace = z.QuadPart;
		nTotal = y.QuadPart;
		
		if(!nTotal)
			return;
		nTemp = 100 * nSize / nTotal;
		if(nTemp == 0)
			m_sPercent = "<1%";
		else
		{
			itoa(nTemp, sArray, 10);
			m_sPercent = sArray;
			m_sPercent += "%";
		}
		m_prgUsed.SetPos(nTemp);
	}

	switch(m_spSize.GetPos() & 0xFFFF)
	{
	case 0:
		if(nSize == 0)
			m_sSize = " 0 Bytes";
		else
		{
			itoa(nSize, sArray, 10);
			m_sSize = sArray;
			m_sSize += " Byte(s)";
		}
		break;
	case 1:
		nRem = nSize % 1024;
		nSize /= 1024;
		if(nSize == 0)
			m_sSize = " <1 KB";
		else
		{
			
			itoa(nSize, sArray, 10);
			m_sSize = sArray;
			if(nRem >= 512)
				m_sSize += ".5";
			m_sSize += " KB";
		}
		break;
	case 2:
		nRem = nSize % 1048576;
		nSize /= 1048576;
		if(nSize == 0)
			m_sSize = " <1 MB";
		else
		{
			itoa(nSize, sArray, 10);
			m_sSize = sArray;
			if(nRem >= 524288)
				m_sSize += ".5";
			m_sSize += " MB";
		}
		break;
	case 3:
		nRem = nSize % 1073741824;
		nSize /= 1073741824;
		if(nSize == 0)
			m_sSize = " <1 GB";
		else
		{
			itoa(nSize, sArray, 10);
			m_sSize = sArray;
			if(nRem >= 536870912)
				m_sSize += ".5";
			m_sSize += " GB";
		}
		break;
	}

	UpdateData(false);
	*pResult = 0;

	ReleaseCapture();
	EndWaitCursor();
}

void CFolderDialog::FollowSub(CString path, __int64 &size, int &file, int &dir, WIN32_FIND_DATA &fd)
{
	HANDLE hFind;
	CString sBuffer;
	hFind = ::FindFirstFile((LPCSTR)path, &fd);
	__int64 nTemp;

	if(hFind != INVALID_HANDLE_VALUE)
	{
		if(strcmp(fd.cFileName, "."))
		{
			if(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				dir++;
				sBuffer = path;
				sBuffer.Delete(sBuffer.GetLength() - 3, 3);
				sBuffer += fd.cFileName;
				sBuffer += "\\*.*";
				FollowSub(sBuffer, size, file, dir, fd);
			}
		}
		while(::FindNextFile(hFind, &fd))
		{
			if(!strcmp(fd.cFileName, ".."))
				continue;
			
			//add the new files size
			if(fd.nFileSizeHigh != 0)
				nTemp = (__int64)(fd.nFileSizeHigh * MAXDWORD) + fd.nFileSizeLow;
			else
				nTemp = fd.nFileSizeLow;

			//if((nTemp % 1024) != 0)
			//	nTemp = nTemp / 1024 + 1024

			size += nTemp;

			//Blank out all attributes accept directory
			//if it exists then handle the sub directory
			fd.dwFileAttributes &= FILE_ATTRIBUTE_DIRECTORY;
			if(fd.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)
			{
				dir++;
				sBuffer = path;
				sBuffer.Delete(sBuffer.GetLength() - 3, 3);
				sBuffer += fd.cFileName;
				sBuffer += "\\*.*";
				FollowSub(sBuffer, size, file, dir, fd);
			}
			else
				file++;
		}
		::CloseHandle(hFind);
	}
}

void CFolderDialog::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	int nSize = m_nSize;
	char sArray[16];
	int nRem;

	if(nSize == 0)
	{
		m_sSize = "Empty";
		return;
	}
	
	switch(m_spSize.GetPos() & 0xFFFF)
	{
	case 0:
		if(nSize == 0)
			m_sSize = " 0 Bytes";
		else
		{
			itoa(nSize, sArray, 10);
			m_sSize = sArray;
			m_sSize += " Byte(s)";
		}
		break;
	case 1:
		nRem = nSize % 1024;
		nSize /= 1024;
		if(nSize == 0)
			m_sSize = " <1 KB";
		else
		{
			
			itoa(nSize, sArray, 10);
			m_sSize = sArray;
			if(nRem >= 512)
				m_sSize += ".5";
			m_sSize += " KB";
		}
		break;
	case 2:
		nRem = nSize % 1048576;
		nSize /= 1048576;
		if(nSize == 0)
			m_sSize = " <1 MB";
		else
		{
			itoa(nSize, sArray, 10);
			m_sSize = sArray;
			if(nRem >= 524288)
				m_sSize += ".5";
			m_sSize += " MB";
		}
		break;
	case 3:
		nRem = nSize % 1073741824;
		nSize /= 1073741824;
		if(nSize == 0)
			m_sSize = " <1 GB";
		else
		{
			itoa(nSize, sArray, 10);
			m_sSize = sArray;
			if(nRem >= 536870912)
				m_sSize += ".5";
			m_sSize += " GB";
		}
		break;
	}

	UpdateData(false);
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}
