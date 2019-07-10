#include "stdafx.h"
#include "treadsys.h"
#include "Tread3d.h"

#ifdef _WIN32
	#include "ConsoleDialog.h"
	#include "Splash.h"
	#include "DialogControl.h"
	#include "ViewControl.h"	
#endif

#ifdef macintosh
#include <stdio.h>
#include <stdarg.h>
#endif

#include "ChildFrm.h"
#include "MainFrm.h"
#include "GameAPI.h"
#include "RenderAPI.h"
#include "Tread3DDoc.h"
#include "MapObject.h"
#include "PluginAPI.h"
#include "ColorTable.h"
#include "ColorCycleController.h"

#ifdef macintosh
#include "mac_registry.h"
#endif

#ifdef _WIN32
extern CTread3DApp theApp;
#endif

static CMapManager* pMapManager;
static CLinkedList<CBaseObject> m_Clipboard;

#ifdef _WIN32
static LPCTSTR gCursors[TC_NUMCURSORS] = {
				IDC_ARROW, IDC_IBEAM, IDC_WAIT, IDC_CROSS, 
				IDC_UPARROW, IDC_SIZEALL, IDC_SIZENWSE, 
				IDC_SIZENESW, IDC_SIZEWE, IDC_SIZENS, MAKEINTRESOURCE(IDC_ZOOM)};
#endif

CLinkedList<CBaseObject>* GetGlobalClipboard()
{
	return &m_Clipboard;
}


void TreadPopupMenu(unsigned int nMenuId, int nXPos, int nYPos, CWnd* pWnd, bool bTranslate)
{
#ifdef _WIN32
	CMenu Menu;
	if(!Menu.LoadMenu(nMenuId))
		return;
	CMenu* pPopup = Menu.GetSubMenu(0);
	
	POINT pt = {nXPos, nYPos};
	if(pWnd!=NULL && bTranslate)
		pWnd->ClientToScreen(&pt);

	pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON, pt.x, pt.y, AfxGetMainWnd());
#endif
}

void PrintSplashLine(CString sInput)
{
#ifdef _WIN32
	CSplashWnd::StaticPrintLine(sInput);
#endif
}

void TreadSetCursor(CMapView* pView, int nCursor)
{
#ifdef _WIN32
	if(nCursor == TC_DEFAULT)
	{
		pView->m_hCursor = NULL;
		SetCursor(LoadCursor(NULL, IDC_ARROW));
		return;
	}
	else if(nCursor <= TC_IDC_SIZENS && nCursor > 0)
		pView->m_hCursor = LoadCursor(NULL, gCursors[nCursor]);
	else
		pView->m_hCursor = LoadCursor((HINSTANCE)GetModuleHandle(NULL), gCursors[nCursor]);
#endif
}

void TreadInit()
{
#if macintosh
	GetTreadMacRegistry()->InitRegistry();
#endif
	PrintSplashLine("Init: MapManager...");
	Debug("Init: MapManager...\n");
	pMapManager = new CMapManager;
	if(pMapManager == NULL)
	{
		Fatal("Map manager could not be started. Closing...!");
		return;
	}

	PrintSplashLine("Init: GameAPI");
	Debug("Init: GameAPI\n");
	GetGameAPI()->Initialize();

	PrintSplashLine("Init: RenderAPI");
	Debug("Init: RenderAPI\n");
	GetRenderAPI()->Initialize();

#ifndef DEBUG
	PrintSplashLine("Init: Plugins");
	Debug("Init: Plugins\n");
	GetPluginAPI()->Initialize();
#endif

}

void TreadShutdown()
{
	GetColorTable()->Destroy();

#ifdef _WIN32
	CTread3DApp* pApp = (CTread3DApp*)AfxGetApp();
	if(pApp != NULL)
		pApp->SaveSettings();
#endif

	GetPluginAPI()->Shutdown();
	GetGameAPI()->SaveRegSettings();
	GetGameAPI()->PurgeAndReset();
	GetRenderAPI()->Shutdown();

	delete pMapManager;
}

CMapManager* GetMapManager()
{
	return pMapManager;
}

CMapManager::CMapManager()
{
	m_nMax = GetRegInt(I_MAXMAP);
	m_nCurrent = -1;
	m_pMode = NULL;
	m_pDoc = NULL;
	m_pWndTable = new CMapFrame*[m_nMax];
	if(m_pWndTable == NULL)
		Fatal("Memory allocation error: Map Manager");

	for(int n = 0; n < m_nMax; n++)
		m_pWndTable[n] = NULL;
		
#ifdef _WIN32
	m_pProgress = NULL;

	for(n = 0; n < 4; n++)
		m_rcView[n] = 0;
#endif
}

CMapManager::CMapManager(int nMax)
{
	m_nMax = nMax;
	m_nCurrent = -1;
	
	m_pMode = NULL;
	m_pDoc = NULL;
	
	m_pWndTable = new CMapFrame*[m_nMax];
	if(m_pWndTable == NULL)
		Fatal("Memory allocation error: Map Manager");

	for(int n = 0; n < m_nMax; n++)
		m_pWndTable[n] = NULL;
		
#ifdef _WIN32
	m_pProgress = NULL;
	
	for(n = 0; n < 4; n++)
	{
		m_rcView[n] = 0;
	}
#endif
}

CMapManager::~CMapManager()
{
	delete [] m_pWndTable;
}

void CMapManager::CycleColorIfSelected()
{
	GetColorCycleController()->StartAnimating(ObjectsSelected());
}

bool CMapManager::ObjectsSelected()
{
	int i;
	bool b=true;

	for(i = 0; i < m_nMax; i++)
	{
		if(m_pWndTable[i] != NULL)
			b = (m_pWndTable[i]->GetDocument()->GetSelectedObjectList()->IsEmpty()==false) && b;

		if(!b)
			break;
	}		

	return b;
}

int CMapManager::RegisterMap(const CMapFrame* pWnd)
{
	if(pWnd == NULL)
		return -1;

	//look for an open index until the maximum is reached
	for(int n = 0; n < m_nMax; n++)
	{
		if(m_pWndTable[n] == NULL)
		{
			m_pWndTable[n] = (CMapFrame*)pWnd;
			SetCurrentMap(n);
			return n;
		}
	}

	return -1;
}

void CMapManager::UnregisterMap(int nHandle)
{
	if(m_pWndTable[nHandle] == m_pWndTable[m_nCurrent])
	{
		for(int n = 0; n < 4; n++)
		{
			m_rcView[n] = 0;
		}
		m_pMode = NULL;
		
		m_nCurrent = -1;
	}
	
	m_pWndTable[nHandle] = NULL;
	
	if(GetMapCount() == 0)
	{
		m_pDoc = NULL;
		UpdateInterface();
		GetPluginAPI()->PostEvent(PEF_MAPCHANGE, 0, NULL);
	}
}

CMapFrame* CMapManager::GetCurrentMap()
{
	return m_pWndTable[m_nCurrent];
}

bool CMapManager::SetCurrentMap(int nHandle)
{
	if(m_pWndTable[nHandle] == NULL)
		return false;

	//check if the map is already current
	if(m_nCurrent == nHandle)
		return true;

	m_nCurrent = nHandle;

	for(int n = 0; n < 4; n++)
		m_rcView[n] = m_pWndTable[nHandle]->GetPaneRect(n);

	m_pWndTable[nHandle]->BeginWaitCursor();
	m_pWndTable[nHandle]->SetCapture();

	m_pDoc = m_pWndTable[nHandle]->GetDocument();
	m_pMode = m_pDoc->GetModePtr();
	m_pMode->RefreshMode();
	
	ReleaseCapture();
	m_pWndTable[nHandle]->EndWaitCursor();
	m_pMainFrame->m_bMapOpen = true;

	UpdateInterface();

	// Tell the plugin system that the map changed.
	GetPluginAPI()->PostEvent(PEF_MAPCHANGE, 0, m_pDoc);

	return true;
}

CMapView* CMapManager::GetMapPane(unsigned int nPane)
{
	if(m_nCurrent == -1)
		return NULL;

	if(m_pWndTable[m_nCurrent])
		return NULL;

	return m_pWndTable[m_nCurrent]->GetMapPane(nPane);
}

CDC* CMapManager::GetPaneDC(unsigned int nPane)
{
	if(m_nCurrent == -1)
		return NULL;

	if(m_pWndTable[m_nCurrent] == NULL)
		return NULL;

	CMapView* pMap;
	pMap = m_pWndTable[m_nCurrent]->GetMapPane(nPane);

	if(pMap == NULL)
		return NULL;

	return pMap->GetDC();
}

void CMapManager::ReleasePaneDC(UINT nPane, CDC* pDC)
{
	if(m_nCurrent == -1)
		return;

	if(m_pWndTable[m_nCurrent] == NULL)
		return;

	CMapView* pMap;
	pMap = m_pWndTable[m_nCurrent]->GetMapPane(nPane);

	if(pMap == NULL)
		return;

	pMap->ReleaseDC(pDC);
}

CRect CMapManager::GetPaneRect(UINT nPane)
{
	if(m_nCurrent == -1)
		return NULL;

	if(m_pWndTable[m_nCurrent] == NULL)
		return NULL;

	return m_rcView[nPane];
}

#ifdef _WIN32
CConsoleView* CMapManager::GetConsole()
{
	if(m_pWndTable[m_nCurrent] == NULL || (m_nCurrent == -1))
		return NULL;

	return m_pWndTable[m_nCurrent]->GetConsolePane();
}
#endif

CMode* CMapManager::GetModePtr()
{
	return m_pMode;
}

bool CMapManager::IsMap()
{
	if(m_nCurrent == -1)
		return false;
	else
		return true;
}

int CMapManager::GetMapCount()
{
	int nCount = 0;

	for(int n = 0; n < m_nMax; n++)
	{
		if(m_pWndTable[n] != NULL)
			nCount++;
	}

	return nCount;
}

void CMapManager::RegisterMainFrame(CMainFrame* pMainFrame)
{
	m_pMainFrame = pMainFrame;
#ifdef _WIN32
	m_pWinApp = AfxGetApp();
#endif
}

void CMapManager::RedrawAllMaps(int nFlags, bool bFast)
{
	int i;
	bool bRF;

	for(i = 0; i < m_nMax; i++)
	{
		if(m_pWndTable[i] != NULL)
		{
			bRF = bFast && (m_pWndTable[i]->GetDocument()->GetGlobalRenderFlags()&RF_FAST)==RF_FAST;

			if(bRF)
				m_pWndTable[i]->GetDocument()->AddGlobalRenderFlags(RF_FAST);

			m_pWndTable[i]->GetDocument()->UpdateWindows(nFlags);

			if(bRF)
				m_pWndTable[i]->GetDocument()->RemoveGlobalRenderFlags(RF_FAST);
		}
	}
}

void CMapManager::RedrawAllMapsSelections(int nFlags, bool bFast)
{
	int i;
	bool bRF;

	for(i = 0; i < m_nMax; i++)
	{
		if(m_pWndTable[i] != NULL)
		{
			bRF = bFast && (m_pWndTable[i]->GetDocument()->GetGlobalRenderFlags()&RF_FAST)==RF_FAST;

			if(bRF)
				m_pWndTable[i]->GetDocument()->AddGlobalRenderFlags(RF_FAST);

			m_pWndTable[i]->GetDocument()->UpdateWindowsSelection(nFlags);

			if(bRF)
				m_pWndTable[i]->GetDocument()->RemoveGlobalRenderFlags(RF_FAST);
		}
	}
}

void CMapManager::AddGlobalRenderFlagsToAllMaps(int nFlags)
{
	int i;
	for(i = 0; i < m_nMax; i++)
	{
		if(m_pWndTable[i] != NULL)
			m_pWndTable[i]->GetDocument()->AddGlobalRenderFlags(nFlags);
	}
}

void CMapManager::RemoveGlobalRenderFlagsFromAllMaps(int nFlags)
{
	int i;
	for(i = 0; i < m_nMax; i++)
	{
		if(m_pWndTable[i] != NULL)
			m_pWndTable[i]->GetDocument()->RemoveGlobalRenderFlags(nFlags);
	}
}

void CMapManager::UpdateInterface(CTread3DDoc* pDocument)
{
	if(pDocument == NULL)
		pDocument = m_pDoc;
	
	if(pDocument == NULL)
	{
		m_pMainFrame->m_bMapOpen = false;
		
#ifdef _WIN32
		m_pMainFrame->m_bUpdatePointer = false;
		m_pMainFrame->m_bUpdateClip = false;
		m_pMainFrame->SetComboGrid(-1);
		m_pMainFrame->SetComboZoom(-1);

		CTextureView* pTextureView = (CTextureView*)GetView(TEXTURE_VIEW);
		pTextureView->UnloadList();

		CObjectView* pObjectView = (CObjectView*)GetView(OBJECT_VIEW);
		pObjectView->ResetContent();

		CAnchorView* pAnchorView = (CAnchorView*)GetView(ANCHOR_VIEW);
		pAnchorView->ResetContent();

		CVisgroupView* pVisgroupView = (CVisgroupView*)GetView(VISGROUP_VIEW);
		pVisgroupView->ResetContent();

		ShowAllDialogs(false);
#endif

		return;
	}
	else
	{
		m_pMainFrame->m_bMapOpen = true;
		
#ifdef _WIN32
		m_pMainFrame->m_bUpdateMenu = true;
		m_pMainFrame->m_bUpdatePointer = true;
		m_pMainFrame->m_bUpdateClip = true;
		//m_pMainFrame->m_bBuildMap = false;
		m_pMainFrame->SetComboGrid(pDocument->GetGridSize());
		m_pMainFrame->SetComboZoom(m_pWndTable[m_nCurrent]->GetMapPane(1)->GetScale());

		ShowAllDialogs(false);

		CAnchorView* pAnchorView = (CAnchorView*)GetView(ANCHOR_VIEW);
		pAnchorView->LoadAnchorList();

		CVisgroupView* pVisgroupView = (CVisgroupView*)GetView(VISGROUP_VIEW);
		pVisgroupView->LoadVisgroup();
#endif

	}
}

CTread3DDoc* CMapManager::GetDocPtr()
{
	return m_pDoc;
}

CMainFrame* CMapManager::GetMainFramePtr()
{
	return m_pMainFrame;
}

#ifdef _WIN32
void CMapManager::EnableAccelerator(bool bEnable)
{
	m_pMainFrame->m_bAccel = bEnable;
}
#endif

//END OF CMapManager

//---------------------------------------------------
//System functions
//---------------------------------------------------

CMode* GetModeManager()
{
	return GetMapManager()->GetModePtr();
}

void Debug(CString sMessage, ...)
{
	va_list argptr;
	char	text[256];
	
	va_start(argptr, sMessage);
	vsprintf(text, sMessage, argptr);
	va_end(argptr);

#ifdef _WIN32
	CConsoleDialog* pDlg = GetConsoleDialog();
	if(pDlg == NULL || pDlg->m_hWnd == NULL)
		return;

	pDlg->AppendText(text);
#endif
}

void Console(CString sMessage, ...)
{
	va_list argptr;
	char	text[256];

	va_start(argptr, sMessage);
	vsprintf(text, sMessage/* + "\n"*/, argptr);
	va_end(argptr);
	
#ifdef _WIN32
	CConsoleView* pConsole = GetMapManager()->GetConsole();
	if(pConsole == NULL || pConsole->m_hWnd == NULL)
		return;
	pConsole->AppendText(text, 0);
#endif
}

void SetConsole(CString sFont, int nSize, COLORREF nColor)
{
#ifdef _WIN32
	CConsoleView* pConsole = GetMapManager()->GetConsole();
	if(pConsole == NULL || pConsole->m_hWnd == NULL)
		return;

	pConsole->SetFont(sFont, nSize, nColor);
#endif
}

void ResetConsole()
{
#ifdef _WIN32
	CConsoleView* pConsole = GetMapManager()->GetConsole();
	if(pConsole == NULL || pConsole->m_hWnd == NULL)
		return;

	pConsole->ResetFont();
#endif
}

void ClearConsole()
{
#ifdef _WIN32
	CConsoleView* pConsole = GetMapManager()->GetConsole();
	if(pConsole == NULL || pConsole->m_hWnd == NULL)
		return;

	pConsole->ResetContent();
#endif
}

void ConsoleMod(CString sMessage, int nModifier)
{
	va_list argptr;
	char	text[256];
	
	va_start(argptr, sMessage);
	vsprintf(text, sMessage, argptr);
	va_end(argptr);

#ifdef _WIN32	
	CConsoleView* pConsole = GetMapManager()->GetConsole();
	if(pConsole == NULL || pConsole->m_hWnd == NULL)
		return;

	pConsole->AppendText(text, nModifier);
#endif
}

void Warning(CString sMessage, bool bPopup)
{
	//print the console message here
#ifdef _WIN32
	if(bPopup)
		MessageBox(NULL, sMessage, "Warning", MB_OK | MB_ICONINFORMATION);
#endif

	SetConsole("", 0, RGB(255, 0, 0));
	Console("Warning: " + sMessage);
	ResetConsole();
	//popup the message box
}

void Error(CString sMessage, bool bConsole)
{
	//print the console message here
	if(bConsole)
	{
		SetConsole("", 0, RGB(200, 0, 0));
		Console("Error:" + sMessage + "\n");
		ResetConsole();
	}

#ifdef _WIN32
	//popup the message box
	MessageBox(NULL, sMessage, "Error", MB_OK | MB_ICONINFORMATION);
#endif
}

void Fatal(CString sMessage, bool bConsole)
{
	//print the console message here
	if(bConsole)
	{
		SetConsole("", 0, RGB(200, 0, 0));
		Console(sMessage);
		ResetConsole();
	}

#ifdef _WIN32
	//popup the message box
	CWinApp* pApp = AfxGetApp();
	pApp->SaveAllModified();
	pApp->CloseAllDocuments(true);

	AfxAbort();
#endif
}

void PrgSetPos(int nPos)
{
#ifdef _WIN32
	GetMapManager()->m_pProgress->SetPos(nPos);
#endif
}

void PrgSetRange(long nMin, long nMax)
{
#ifdef _WIN32
	GetMapManager()->m_pProgress->SetRange32(nMin, nMax);
#endif
}

void PrgStepIt()
{
#ifdef _WIN32
	GetMapManager()->m_pProgress->StepIt();
#endif
}

void PrgSetStep(int nStep)
{
#ifdef _WIN32
	GetMapManager()->m_pProgress->SetStep(nStep);
#endif
}

CString GetRegString(UINT nCode)
{
	if(nCode >= STRING_MAX)
		return "";

#ifdef _WIN32
	return theApp.m_pString[nCode];
#else
	return GetTreadMacRegistry()->m_pString[nCode];
#endif
}

int GetRegInt(UINT nCode)
{
	if(nCode >= MAX_INT)
		return -1;

#ifdef _WIN32
	return theApp.m_pInt[nCode];
#else
	return GetTreadMacRegistry()->m_pInt[nCode];
#endif
}

bool GetRegBool(UINT nCode)
{
	if(nCode >= BOOL_MAX)
		return (bool)-1;

#ifdef _WIN32
	return theApp.m_pBool[nCode];
#else
	return GetTreadMacRegistry()->m_pBool[nCode];
#endif
}

void SetRegString(UINT nCode, CString sData)
{
	if(nCode >= STRING_MAX)
		return;

#ifdef _WIN32
	theApp.m_pString[nCode] = sData;
#else
	GetTreadMacRegistry()->m_pString[nCode] = sData;
#endif
}

void SetRegInt(UINT nCode, int nData)
{
	if(nCode >= MAX_INT)
		return;

#ifdef _WIN32
	theApp.m_pInt[nCode] = nData;
#else
	GetTreadMacRegistry()->m_pInt[nCode] = nData;
#endif
}

void SetRegBool(UINT nCode, bool bData)
{
	if(nCode >= BOOL_MAX)
		return;

#ifdef _WIN32
	theApp.m_pBool[nCode] = bData;
#else
	GetTreadMacRegistry()->m_pBool[nCode] = bData;
#endif
}

void SaveTreadSettings()
{
#ifdef _WIN32
	theApp.SaveSettings();
#else
	GetTreadMacRegistry()->FlushRegistry();
#endif
}
