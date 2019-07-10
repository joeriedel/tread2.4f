// TREADSYS.H
// Copyright © 1999 Joe Riedel, Nick Randal.
// Author: Joe Riedel, Nick Randal.

/*
Nick : 9/8/99
Added CMode handling.
*/
/*
Joe : 10/10/99
Move flags to hex.
Modes are now bit flags so event responders can bind to 
multiple events.
*/
#ifndef TREADSYS_H
#define TREADSYS_H

#include "LinkedList.h"
#include "TExport.h"
//#include "mode.h"

class CMode;
class CMapFrame;
class CMapView;
class CConsoleView;

#ifdef _WIN32
#pragma warning(disable : 4244)     // truncate from double to float
//#pragma warning(disable : 4237)		// true, false extensions.
#pragma warning(disable : 4800)		// forcing int to bool.
//#pragma warning(disable : 4251)
#endif

// $sb "treadsys.h block1"


//#define DIALOGBAR_MAX 1	//max dialog bar entries

//view types
#define VIEW_TOP		0x00000001
#define VIEW_SIDE		0x00000002
#define VIEW_FRONT		0x00000004
#define VIEW_GL			0x00000008
#define VIEW_3D			0x00000008

#define FLAT_VIEWS			(VIEW_TOP|VIEW_SIDE|VIEW_FRONT)
#define ALL_VIEWS			(VIEW_3D|FLAT_VIEWS)

#define ALL_MODE_FLAGS	0xFFFFFFFF

//MODE FLAGS
#define DEFAULT_MODE	0x00000001
#define DEFAULT_SUBMODE 0x00000000
#define DEFAULT_FLAG	0x00000004

// These are bitflags so that event responders can
// mask and attach to multiple mode events.
#define MODE_POINTER	0x00000001
#define MODE_CAMERA		0x00000002
#define MODE_ZOOM		0x00000004
#define MODE_PAINT		0x00000008
#define MODE_ENTITY		0x00000010
#define MODE_BRUSH		0x00000020
#define MODE_FACE		0x00000040
#define MODE_VERTEX		0x00000080
#define MODE_CLIP		0x00000100

//SUBMODE FLAGS

#define SUB_NONE		0x00000000

//SUBMODE POINTER FLAGS
#define POINTER_MASK_SUBMODE	0x00000003	//MASK
#define POINTER_BRUSH	0x00000001			//INDEX VALUES
#define POINTER_ENTITY	0x00000002
#define POINTER_ALL		0x00000003
#define POINTER_ROTATE	0x00000004

//SUBMODE CLIP FLAGS
#define CLIP_MASK_SUBMODE		0x00000001	//MASK
#define CLIP_CLIP		0x00000000			//INDEX VALUES
#define CLIP_SPLIT		0x00000001


//FLAGS
#define FLAG_TEXLOCK		0x00000001
#define FLAG_BUILDING		0x00000002

// $eb

//#define FLAG_	0x00000004
//#define FLAG_		0x00000008
//#define FLAG_		0x00000010
//#define FLAG_	16
//#define FLAG_	32
//#define FLAG_	64
//#define FLAG_	128

void Debug(CString sMessage, ...);
void Warning(CString sMessage, bool bPopup = false);
void Error(CString sMessage, bool bConsole = true);
void Fatal(CString sMessage, bool bConsole = false);

CString GetRegString(unsigned int nCode);
int GetRegInt(unsigned int nCode);
bool GetRegBool(unsigned int nCode);

void SetRegString(unsigned int nCode, CString sData);
void SetRegInt(unsigned int nCode, int nData);
void SetRegBool(unsigned int nCode, bool bData);
void SaveRegistrySettings();

void SaveTreadSettings();

/*void SetSplashTextLine1(CString);
void SetSplashTextLine2(CString);
void SetSplashColorLine1(COLORREF);
void SetSplashColorLine2(COLORREF);*/
void PrintSplashLine(CString);

// Tracks a popup menu.
void TreadPopupMenu(unsigned int nMenuId, int nXPos, int nYPos, CWnd* pWnd, bool bTranslate=true);

class CBaseObject;
CLinkedList<CBaseObject>* GetGlobalClipboard();

// $sb "treadsys.h block2"

// Tread Cursors.
#define TC_DEFAULT			-1
#define TC_IDC_ARROW		0
#define TC_IDC_IBEAM		1
#define TC_IDC_WAIT			2
#define TC_IDC_CROSS		3
#define TC_IDC_UPARROW		4
#define TC_IDC_SIZEALL		5
#define TC_IDC_SIZENWSE		6
#define TC_IDC_SIZENESW		7
#define TC_IDC_SIZEWE		8
#define TC_IDC_SIZENS		9
#define TC_IDC_ZOOM			10
#define TC_NUMCURSORS		11

// $eb

void TreadSetCursor(CMapView* pView, int nCursor);

void TreadInit();
void TreadShutdown();

class CTread3DDoc;
class CMainFrame;

// $sb "treadsys.h mapmanager block1"
class _T_EXPORT CMapManager
{
public:
	CMapManager();
	CMapManager(int nMax);
	~CMapManager();

	int RegisterMap(const CMapFrame* pWnd);
	void UnregisterMap(int nHandle);
	CMapFrame* GetCurrentMap();
	bool SetCurrentMap(int nHandle);

#ifdef _WIN32
	void SetProgressPtr(CProgressCtrl* pProgress) { m_pProgress = pProgress; }
#endif

// $eb

	friend void Console(CString sMessage, ...);
	friend void SetConsole(CString sFont, int nSize, COLORREF nColor);
	friend void ConsoleMod(CString sMessage, int nModifier);
	friend void ResetConsole();
	friend void ClearConsole();

	friend void PrgSetPos(int nPos);
	friend void PrgSetRange(long nMin, long nMax);
	friend void PrgStepIt();
	friend void PrgSetStep(int nStep);

	friend CMode* GetModeManager();

// $sb "treadsys.h mapmanager block2"

	bool IsMap();
	int GetMapCount();

	CDC* GetPaneDC(unsigned int nPane);
	void ReleasePaneDC(unsigned int nPane, CDC* pDC);
	CRect GetPaneRect(unsigned int nPane);
	CMode* GetModePtr();
	CTread3DDoc* GetDocPtr();
	CMainFrame* GetMainFramePtr();

	void RedrawAllMaps(int nFlags=ALL_VIEWS, bool bFast=true);
	void RedrawAllMapsSelections(int nFlags=ALL_VIEWS, bool bFast=true);

	void AddGlobalRenderFlagsToAllMaps(int nFlags);
	void RemoveGlobalRenderFlagsFromAllMaps(int nFlags);

	void UpdateInterface(CTread3DDoc* pDocument = NULL);
	void RegisterMainFrame(CMainFrame* pMainFrame);
	
#ifdef _WIN32
	
	void EnableAccelerator(bool bEnable = true);
	CWinApp* GetMainApp() { return m_pWinApp; }
#endif

	bool ObjectsSelected();
	void CycleColorIfSelected();

private:
	CMapView* GetMapPane(unsigned int nPane);
	CTread3DDoc* m_pDoc;

#ifdef _WIN32
	CConsoleView* GetConsole();
	CProgressCtrl* m_pProgress;
#endif

	CMainFrame* m_pMainFrame;
	CMapFrame** m_pWndTable;

	
	int m_nMax;
	int m_nCurrent;
	CMode *m_pMode;

	CRect m_rcView[4];
	
#ifdef _WIN32
	CWinApp* m_pWinApp;
#endif
};
// $eb
CMapManager* GetMapManager();

#endif