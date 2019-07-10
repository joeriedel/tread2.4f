// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "Tread3D.h"

#include "MainFrm.h"
#include "Splash.h"

#include "AnchorView.h"
#include "VisgroupView.h"
#include "ObjectView.h"
#include "TextureView.h"
#include "TreadSys.h"
#include "BindDialog.h"
#include "AccelFile.h"
#include "ColorTable.h"
#include "ClrDialog.h"
#include "ViewControl.h"
#include "DialogControl.h"
#include "EntityDialog.h"
#include "MapObject.h"
#include "Mode.h"
#include "ChildFrm.h"
#include "GameSetupDialog.h"
#include "GameAPI.h"
#include "BuildDialog.h"
#include "HollowDialog.h"
#include "VisgroupDialog.h"
#include "SetupDialog.h"
#include "ScriptDialog.h"
#include "QuickHelpDialog.h"
#include "ColorCycleController.h"
#include "PluginAPI.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CCJMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CCJMDIFrameWnd)
	ON_WM_INITMENU()
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_UPDATE_COMMAND_UI(ID_VIEW_WORKSPACE, OnUpdateViewWorkspace)
	ON_COMMAND(ID_VIEW_WORKSPACE, OnViewWorkspace)
	ON_COMMAND(IDT_MODE_POINTER, OnModePointer)
	ON_UPDATE_COMMAND_UI(IDT_MODE_POINTER, OnUpdateModePointer)
	ON_COMMAND(IDT_MODE_CAMERA, OnModeCamera)
	ON_UPDATE_COMMAND_UI(IDT_MODE_CAMERA, OnUpdateModeCamera)
	ON_COMMAND(IDT_MODE_ZOOM, OnModeZoom)
	ON_UPDATE_COMMAND_UI(IDT_MODE_ZOOM, OnUpdateModeZoom)
	ON_COMMAND(IDT_MODE_PAINT, OnModePaint)
	ON_UPDATE_COMMAND_UI(IDT_MODE_PAINT, OnUpdateModePaint)
	ON_COMMAND(IDT_MODE_BRUSH, OnModeBrush)
	ON_UPDATE_COMMAND_UI(IDT_MODE_BRUSH, OnUpdateModeBrush)
	ON_COMMAND(IDT_MODE_FACE, OnModeFace)
	ON_UPDATE_COMMAND_UI(IDT_MODE_FACE, OnUpdateModeFace)
	ON_COMMAND(IDT_MODE_VERTEX, OnModeVertex)
	ON_UPDATE_COMMAND_UI(IDT_MODE_VERTEX, OnUpdateModeVertex)
	ON_COMMAND(IDT_MODE_CLIP, OnModeClip)
	ON_UPDATE_COMMAND_UI(IDT_MODE_CLIP, OnUpdateModeClip)
	ON_COMMAND(IDT_MODE_TEXLOCK, OnModeTexlock)
	ON_UPDATE_COMMAND_UI(IDT_MODE_TEXLOCK, OnUpdateModeTexlock)
	ON_COMMAND(IDT_RUNMAP, OnRunmap)
	ON_UPDATE_COMMAND_UI(IDT_RUNMAP, OnUpdateRunmap)
	ON_COMMAND(IDT_MODE_ENTITY, OnModeEntity)
	ON_UPDATE_COMMAND_UI(IDT_MODE_ENTITY, OnUpdateModeEntity)
	ON_COMMAND(ID_OPTIONS_KEYBINDING, OnOptionsKeybinding)
	ON_COMMAND(ID_OPTIONS_DEBUGWINDOW, OnOptionsDebugwindow)
	ON_COMMAND(ID_OPTIONS_COLOROPTIONS, OnOptionsColoroptions)
	ON_COMMAND(IDT_OBJECT_LINK_ENTITY, OnObjectLinkEntity)
	ON_COMMAND(IDT_OBJECT_PROPERTIES, OnObjectProperties)
	ON_CBN_SELCHANGE(IDT_GRID_COMBO, OnSelchangeGridCombo)
	ON_CBN_SELCHANGE(IDT_ZOOM_COMBO, OnSelchangeZoomCombo)
	ON_UPDATE_COMMAND_UI(IDT_GRID_COMBO, OnUpdateGridCombo)
	ON_UPDATE_COMMAND_UI(IDT_ZOOM_COMBO, OnUpdateZoomCombo)
	ON_COMMAND(ID_OPTIONS_GAMESETUP, OnOptionsGamesetup)
	ON_COMMAND(IDT_OBJECT_CARVE, OnObjectCarve)
	ON_UPDATE_COMMAND_UI(IDT_OBJECT_CARVE, OnUpdateObjectCarve)
	ON_COMMAND(IDT_OBJECT_FLIP_HORIZONTAL, OnObjectFlipHorizontal)
	ON_UPDATE_COMMAND_UI(IDT_OBJECT_FLIP_HORIZONTAL, OnUpdateObjectFlipHorizontal)
	ON_COMMAND(IDT_OBJECT_FLIP_VERTICAL, OnObjectFlipVertical)
	ON_UPDATE_COMMAND_UI(IDT_OBJECT_FLIP_VERTICAL, OnUpdateObjectFlipVertical)
	ON_COMMAND(IDT_OBJECT_GROUP, OnObjectGroup)
	ON_UPDATE_COMMAND_UI(IDT_OBJECT_GROUP, OnUpdateObjectGroup)
	ON_COMMAND(IDT_OBJECT_HOLLOW, OnObjectHollow)
	ON_UPDATE_COMMAND_UI(IDT_OBJECT_HOLLOW, OnUpdateObjectHollow)
	ON_UPDATE_COMMAND_UI(IDT_OBJECT_LINK_ENTITY, OnUpdateObjectLinkEntity)
	ON_UPDATE_COMMAND_UI(IDT_OBJECT_PROPERTIES, OnUpdateObjectProperties)
	ON_COMMAND(IDT_OBJECT_ROTATE_180_CW, OnObjectRotate180Cw)
	ON_UPDATE_COMMAND_UI(IDT_OBJECT_ROTATE_180_CW, OnUpdateObjectRotate180Cw)
	ON_COMMAND(IDT_OBJECT_ROTATE_90_CCW, OnObjectRotate90Ccw)
	ON_UPDATE_COMMAND_UI(IDT_OBJECT_ROTATE_90_CCW, OnUpdateObjectRotate90Ccw)
	ON_COMMAND(IDT_OBJECT_ROTATE_90_CW, OnObjectRotate90Cw)
	ON_UPDATE_COMMAND_UI(IDT_OBJECT_ROTATE_90_CW, OnUpdateObjectRotate90Cw)
	ON_COMMAND(IDT_OBJECT_UNGROUP, OnObjectUngroup)
	ON_UPDATE_COMMAND_UI(IDT_OBJECT_UNGROUP, OnUpdateObjectUngroup)
	ON_COMMAND(IDT_OBJECT_UNLINK_ENTITY, OnObjectUnlinkEntity)
	ON_UPDATE_COMMAND_UI(IDT_OBJECT_UNLINK_ENTITY, OnUpdateObjectUnlinkEntity)
	ON_COMMAND(IDT_VIEW_FRONTVIEW, OnViewFrontview)
	ON_UPDATE_COMMAND_UI(IDT_VIEW_FRONTVIEW, OnUpdateViewFrontview)
	ON_COMMAND(IDT_VIEW_SIDEVIEW, OnViewSideview)
	ON_UPDATE_COMMAND_UI(IDT_VIEW_SIDEVIEW, OnUpdateViewSideview)
	ON_COMMAND(IDT_VIEW_TOPVIEW, OnViewTopview)
	ON_UPDATE_COMMAND_UI(IDT_VIEW_TOPVIEW, OnUpdateViewTopview)
	ON_COMMAND(IDM_VIEW_GDI, OnViewGdi)
	ON_UPDATE_COMMAND_UI(IDM_VIEW_GDI, OnUpdateViewGdi)
	ON_COMMAND(IDM_VIEW_OPENGL, OnViewOpengl)
	ON_UPDATE_COMMAND_UI(IDM_VIEW_OPENGL, OnUpdateViewOpengl)
	ON_COMMAND(IDM_3D_LINE, On3dLine)
	ON_COMMAND(IDM_3D_SOLID, On3dSolid)
	ON_COMMAND(IDM_3D_TEXTURED, On3dTextured)
	ON_UPDATE_COMMAND_UI(IDM_3D_LINE, OnUpdate3dLine)
	ON_UPDATE_COMMAND_UI(IDM_3D_SOLID, OnUpdate3dSolid)
	ON_UPDATE_COMMAND_UI(IDM_3D_TEXTURED, OnUpdate3dTextured)
	ON_COMMAND(IDM_MAP_NOENTITYFILL, OnMapNoentityfill)
	ON_COMMAND(IDM_MAP_NOSELBOXSIZE, OnMapNoselboxsize)
	ON_COMMAND(IDM_3D_REALTIME, On3dRealtime)
	ON_UPDATE_COMMAND_UI(IDM_MAP_NOENTITYFILL, OnUpdateMapNoentityfill)
	ON_UPDATE_COMMAND_UI(IDM_MAP_NOSELBOXSIZE, OnUpdateMapNoselboxsize)
	ON_UPDATE_COMMAND_UI(IDM_3D_REALTIME, OnUpdate3dRealtime)
	ON_COMMAND(IDM_3D_NOSELBOXOUTLINE, On3dNoselboxoutline)
	ON_COMMAND(IDM_MAP_NOENTNAMES, OnMapNoentnames)
	ON_COMMAND(IDM_3D_NOBILINEAR, On3dNobilinear)
	ON_UPDATE_COMMAND_UI(IDM_3D_NOBILINEAR, OnUpdate3dNobilinear)
	ON_UPDATE_COMMAND_UI(IDM_3D_NOSELBOXOUTLINE, OnUpdate3dNoselboxoutline)
	ON_UPDATE_COMMAND_UI(IDM_MAP_NOENTNAMES, OnUpdateMapNoentnames)
	ON_COMMAND(IDM_3D_NODRAW, On3dNodraw)
	ON_UPDATE_COMMAND_UI(IDM_3D_NODRAW, OnUpdate3dNodraw)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_COMMAND(ID_EDIT_DELETE, OnEditDelete)
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	ON_COMMAND(ID_EDIT_UNDO, OnEditUndo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, OnUpdateEditCopy)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CUT, OnUpdateEditCut)
	ON_UPDATE_COMMAND_UI(ID_EDIT_DELETE, OnUpdateEditDelete)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, OnUpdateEditPaste)
	ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO, OnUpdateEditUndo)
	ON_COMMAND(ID_ADDVISGROUP, OnAddvisgroup)
	ON_UPDATE_COMMAND_UI(ID_ADDVISGROUP, OnUpdateAddvisgroup)
	ON_COMMAND(IDM_TREAD_SETUP, OnTreadSetup)
	ON_COMMAND(ID_VIEW_WORKSPACE2, OnViewWorkspace2)
	ON_UPDATE_COMMAND_UI(ID_VIEW_WORKSPACE2, OnUpdateViewWorkspace2)
	ON_COMMAND(IDR_TOOLBAR_MODES, OnToolbarModes)
	ON_COMMAND(IDR_TOOLBAR_OBJECTS, OnToolbarObjects)
	ON_COMMAND(IDR_TOOLBAR_VIEW, OnToolbarView)
	ON_UPDATE_COMMAND_UI(IDR_TOOLBAR_MODES, OnUpdateToolbarModes)
	ON_UPDATE_COMMAND_UI(IDR_TOOLBAR_OBJECTS, OnUpdateToolbarObjects)
	ON_UPDATE_COMMAND_UI(IDR_TOOLBAR_VIEW, OnUpdateToolbarView)
	ON_COMMAND(IDM_VIEW_OPENGLSOFT, OnViewOpenglsoft)
	ON_UPDATE_COMMAND_UI(IDM_VIEW_OPENGLSOFT, OnUpdateViewOpenglsoft)
	ON_COMMAND(IDM_ENTITY_PATH, OnEntityPath)
	ON_COMMAND(IDM_ENTITY_PATHLOOP, OnEntityPathloop)
	ON_COMMAND(IDM_ENTITY_TARGETPATH, OnEntityTargetpath)
	ON_UPDATE_COMMAND_UI(IDM_ENTITY_PATH, OnUpdateEntityPath)
	ON_UPDATE_COMMAND_UI(IDM_ENTITY_PATHLOOP, OnUpdateEntityPathloop)
	ON_UPDATE_COMMAND_UI(IDM_ENTITY_TARGETPATH, OnUpdateEntityTargetpath)
	ON_COMMAND(IDM_VIEW_CENTER, OnViewCenter)
	ON_UPDATE_COMMAND_UI(IDM_VIEW_CENTER, OnUpdateViewCenter)
	ON_COMMAND(IDM_CLEAR_MEMORY, OnClearMemory)
	ON_UPDATE_COMMAND_UI(IDM_CLEAR_MEMORY, OnUpdateClearMemory)
	ON_UPDATE_COMMAND_UI(IDM_MAP_PROPERTIES, OnUpdateMapProperties)
	ON_COMMAND(IDM_WORLDSPAWN, OnWorldspawn)
	ON_UPDATE_COMMAND_UI(IDM_WORLDSPAWN, OnUpdateWorldspawn)
	ON_UPDATE_COMMAND_UI(ID_BUILD_MODEL, OnUpdateBuildModel)
	ON_COMMAND(ID_SCRIPT_INFORMATION, OnScriptInformation)
	ON_COMMAND(ID_QUICK_HELP, OnQuickHelp)
	ON_COMMAND(ID_CLIP_ACTION, OnClipAction)
	ON_COMMAND(ID_3DVIEW_TRANSPARENTWATER, On3dviewTransparentwater)
	ON_UPDATE_COMMAND_UI(ID_3DVIEW_TRANSPARENTWATER, OnUpdate3dviewTransparentwater)
	ON_COMMAND(ID_EDIT_REDO, OnEditRedo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_REDO, OnUpdateEditRedo)
	ON_COMMAND(ID_VIEW_SMARTNAMES, OnViewSmartnames)
	ON_UPDATE_COMMAND_UI(ID_VIEW_SMARTNAMES, OnUpdateViewSmartnames)
	ON_COMMAND(IDM_ENTITY_DELETEPATH, OnEntityDeletepath)
	ON_UPDATE_COMMAND_UI(IDM_ENTITY_DELETEPATH, OnUpdateEntityDeletepath)
	ON_COMMAND(ID_FILE_IMPORT, OnFileImport)
	ON_COMMAND(ID_FILE_EXPORT, OnFileExport)
	ON_UPDATE_COMMAND_UI(ID_FILE_EXPORT, OnUpdateFileExport)
	ON_COMMAND(IDM_3D_DRAWENTITIES, On3dDrawentities)
	ON_UPDATE_COMMAND_UI(IDM_3D_DRAWENTITIES, OnUpdate3dDrawentities)
	ON_COMMAND(IDM_3D_SHADED, On3dShaded)
	ON_UPDATE_COMMAND_UI(IDM_3D_SHADED, OnUpdate3dShaded)
	ON_COMMAND(ID_DROP_ANCHOR, OnDropAnchor)
	ON_COMMAND(ID_GRID_SNAP, OnGridSnap)
	ON_UPDATE_COMMAND_UI(ID_GRID_SNAP, OnUpdateGridSnap)
	ON_UPDATE_COMMAND_UI(ID_PLUGIN_MENU, OnUpdatePluginMenu)
	ON_COMMAND(ID_VIEW_BRUSH_HANDLES, OnViewBrushHandles)
	ON_UPDATE_COMMAND_UI(ID_VIEW_BRUSH_HANDLES, OnUpdateViewBrushHandles)
	ON_COMMAND(ID_VIEW_ANCHORS, OnViewAnchors)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ANCHORS, OnUpdateViewAnchors)
	ON_COMMAND(IDM_MAP_DRAWPATHS, OnMapDrawpaths)
	ON_UPDATE_COMMAND_UI(IDM_MAP_DRAWPATHS, OnUpdateMapDrawpaths)
	ON_COMMAND(ID_VIEW_UPDATEALLVIEWS, OnViewUpdateallviews)
	ON_UPDATE_COMMAND_UI(ID_VIEW_UPDATEALLVIEWS, OnUpdateViewUpdateallviews)
	ON_COMMAND(IDR_TOOLBAR_ALIGN, OnToolbarAlign)
	ON_UPDATE_COMMAND_UI(IDR_TOOLBAR_ALIGN, OnUpdateToolbarAlign)
	ON_COMMAND(ID_GRID_LARGER, OnGridLarger)
	ON_UPDATE_COMMAND_UI(ID_GRID_LARGER, OnUpdateGridLarger)
	ON_UPDATE_COMMAND_UI(ID_GRID_SMALLER, OnUpdateGridSmaller)
	ON_COMMAND(ID_GRID_SMALLER, OnGridSmaller)
	ON_COMMAND(ID_ALIGN_EDGE_BOTTOM, OnAlignEdgeBottom)
	ON_UPDATE_COMMAND_UI(ID_ALIGN_EDGE_BOTTOM, OnUpdateAlignEdgeBottom)
	ON_COMMAND(ID_ALIGN_EDGE_LEFT, OnAlignEdgeLeft)
	ON_UPDATE_COMMAND_UI(ID_ALIGN_EDGE_LEFT, OnUpdateAlignEdgeLeft)
	ON_UPDATE_COMMAND_UI(ID_ALIGN_EDGE_RIGHT, OnUpdateAlignEdgeRight)
	ON_COMMAND(ID_ALIGN_EDGE_RIGHT, OnAlignEdgeRight)
	ON_COMMAND(ID_ALIGN_EDGE_TOP, OnAlignEdgeTop)
	ON_UPDATE_COMMAND_UI(ID_ALIGN_EDGE_TOP, OnUpdateAlignEdgeTop)
	ON_COMMAND(ID_ALIGN_SIZE_BOTH, OnAlignSizeBoth)
	ON_UPDATE_COMMAND_UI(ID_ALIGN_SIZE_BOTH, OnUpdateAlignSizeBoth)
	ON_COMMAND(ID_ALIGN_SIZE_HORIZONTAL, OnAlignSizeHorizontal)
	ON_UPDATE_COMMAND_UI(ID_ALIGN_SIZE_HORIZONTAL, OnUpdateAlignSizeHorizontal)
	ON_COMMAND(ID_ALIGN_SIZE_VERTICAL, OnAlignSizeVertical)
	ON_UPDATE_COMMAND_UI(ID_ALIGN_SIZE_VERTICAL, OnUpdateAlignSizeVertical)
	ON_COMMAND(ID_ALIGN_SPACE_HORIZONTAL, OnAlignSpaceHorizontal)
	ON_UPDATE_COMMAND_UI(ID_ALIGN_SPACE_HORIZONTAL, OnUpdateAlignSpaceHorizontal)
	ON_COMMAND(ID_ALIGN_SPACE_VERTICAL, OnAlignSpaceVertical)
	ON_UPDATE_COMMAND_UI(ID_ALIGN_SPACE_VERTICAL, OnUpdateAlignSpaceVertical)
	ON_WM_TIMER()
	ON_COMMAND(ID_VIEW_ANIMATED_SELECTION, OnViewAnimatedSelection)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ANIMATED_SELECTION, OnUpdateViewAnimatedSelection)
	//}}AFX_MSG_MAP
	// Global help commands
	ON_COMMAND(ID_HELP_FINDER, CCJMDIFrameWnd::OnHelpFinder)
	ON_COMMAND(ID_HELP, CMDIFrameWnd::OnHelp)
	ON_COMMAND(ID_CONTEXT_HELP, CCJMDIFrameWnd::OnContextHelp)
	ON_COMMAND(ID_DEFAULT_HELP, CCJMDIFrameWnd::OnHelpFinder)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	IDC_PROGRESS_STATUS,
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_NUM,
};

static CString pGridStrings[] = 
{
	"2",
	"4",
	"8",
	"16",
	"32",
	"64",
	"128",
	"512",
	"1024",
};

static int pGridValues[] = 
{
	2,
	4,
	8,
	16,
	32,
	64,
	128,
	512,
	1024
};

#define GRID_COMBO_NUM	9

static CString pZoomStrings[] = 
{
	"10%",
	"20%",
	"30%",
	"40%",
	"50%",
	"60%",
	"70%",
	"80%",
	"90%",
	"100%",
	"150%",
	"200%",
	"300%",
	"400%",
	"500%",
};

static float pZoomValues[] = 
{
	0.10F,
	0.20F,
	0.30F,
	0.40F,
	0.50F,
	0.60F,
	0.70F,
	0.80F,
	0.90F,
	1.0F,
	1.50F,
	2.0F,
	3.0F,
	4.0F,
	5.0F,
};

#define ZOOM_COMBO_NUM	15

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	m_pAccelTable = NULL;
	m_nAccelCount = 0;

	m_bMapOpen = false;
	m_bBuildMap = false;
	m_bEntitySelect = false;
	m_bBrushSelect = false;
	m_bAccel = true;
	m_bUpdatePointer = false;
	m_bUpdateClip = false;
	m_bModifier = false;
	m_bFaceDialog = false;
}

CMainFrame::~CMainFrame()
{
	TreadShutdown();
	UnregisterDialog(DLG_DEBUG);
	UnregisterDialog(DLG_PREVIEW);
	UnregisterDialog(DLG_SURFACE);
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	InitializeViewList();

	CConsoleDialog* pDebug;
	CPreviewDialog* pPreview;
	CSurfaceDialog* pSurface;
	CQuickHelpDialog* pQuickHelp;

	//show the splash screen
	if(GetRegBool(B_NOSPLASH) != true)
		CSplashWnd::ShowSplashScreen(this);

	if (CCJMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	try
	{
		pDebug = new CConsoleDialog;
		if(pDebug == NULL) throw "Console Dialog";
		pDebug->Create(IDD_DIALOG_CONSOLE, this);
		if(GetRegBool(B_DEBUG))
			pDebug->ShowWindow(SW_SHOW);

		//initialize tread
		PrintSplashLine("-------------------------------");
		PrintSplashLine("Tread3D 2.0 Startup...");
		PrintSplashLine("-------------------------------");
		PrintSplashLine("Exe: " + CString(__TIME__) + " " + CString(__DATE__));
		Debug("-------------------------------\n");
		Debug("Tread3D 2.0 Startup...\n");
		Debug("-------------------------------\n");
		Debug("Exe: %s, %s\n", __TIME__, __DATE__);

		PrintSplashLine("Init: Frame Class & Support Dialogs");
		Debug("Init: Frame Class & Support Dialogs\n");

		pSurface = new CSurfaceDialog;
		if(pSurface == NULL) throw "Surface Dialog";
		pSurface->Create(IDD_DIALOG_SURFACE, this);

		pQuickHelp = new CQuickHelpDialog;
		if(pQuickHelp == NULL) throw "Quick Help Dialog";
			pQuickHelp->Create(IDD_DIALOG_QUICKHELP, this);
		
	}
	catch(char* pException)
	{
		CString sBuffer;
		sBuffer.Format("Memory allocation failed initializing frame in: %s", pException);
		Error(sBuffer);
		return -1;
	}
	
	PrintSplashLine("Init: Toolbars");
	Debug("Init: Toolbars\n");

	PrintSplashLine("Init: Docking");
	Debug("Init: Docking\n");
	if (!m_wndModeBar.Create(this, WS_CHILD | WS_VISIBLE | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC, IDR_TOOLBAR_MODES) ||
		!m_wndModeBar.LoadToolBar(IDR_TOOLBAR_MODES))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndObjectBar.Create(this, WS_CHILD | WS_VISIBLE | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC, IDR_TOOLBAR_OBJECTS) ||
		!m_wndObjectBar.LoadToolBar(IDR_TOOLBAR_OBJECTS))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndViewBar.Create(this, WS_CHILD | WS_VISIBLE | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC, IDR_TOOLBAR_VIEW) ||
		!m_wndViewBar.LoadToolBar(IDR_TOOLBAR_VIEW))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndAlignBar.Create(this, WS_CHILD | WS_VISIBLE | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC, IDR_TOOLBAR_ALIGN) ||
		!m_wndAlignBar.LoadToolBar(IDR_TOOLBAR_ALIGN))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
	
	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}
	
	PrintSplashLine("Init: Frame Controls");
	Debug("Init: Frame Controls\n");
	m_pZoomBox = (CComboBox*)m_wndViewBar.InsertControl(RUNTIME_CLASS(CComboBox), "", CRect(-75,-200,0,0),
		IDT_ZOOM_COMBO, WS_CHILD | CBS_DROPDOWNLIST | CBS_AUTOHSCROLL | WS_VSCROLL | CBS_HASSTRINGS);
	m_pGridBox = (CComboBox*)m_wndViewBar.InsertControl(RUNTIME_CLASS(CComboBox), "", CRect(-75,-200,0,0),
		IDT_GRID_COMBO, WS_CHILD | CBS_DROPDOWNLIST | CBS_AUTOHSCROLL | WS_VSCROLL | CBS_HASSTRINGS);

	m_wndStatusBar.SetPaneInfo(0, IDC_PROGRESS_STATUS, SBPS_NOBORDERS, 210);
	m_wndStatusBar.SetPaneInfo(1, ID_SEPARATOR, SBPS_NOBORDERS, 300);

	m_pProgress = (CProgressCtrl*)m_wndStatusBar.InsertControl(RUNTIME_CLASS(CProgressCtrl), "", CRect(-200,-16,0,0),
		IDC_PROGRESS_STATUS, PBS_SMOOTH);

	m_pProgress->SetRange(0, 100);
	m_pProgress->SetStep(1);
	m_pProgress->SetPos(0);

	// Add text.
	int i;
	for(i = 0; i < GRID_COMBO_NUM; i++)
		m_pGridBox->AddString(pGridStrings[i]);
	for(i = 0; i < ZOOM_COMBO_NUM; i++)
		m_pZoomBox->AddString(pZoomStrings[i]);

	// Select first one.
	m_pGridBox->SetCurSel(5);
	m_pZoomBox->SetCurSel(4);
	
	m_nPointerImage = m_wndModeBar.GetToolBarCtrl().AddBitmap(3, IDB_BITMAP_POINTER);
	if(m_nPointerImage == -1)
		Fatal("OnCreate: Could not load image resource!");

	m_nClipImage = m_wndModeBar.GetToolBarCtrl().AddBitmap(2, IDB_BITMAP_CLIP);
	if(m_nClipImage == -1)
		Fatal("OnCreate: Could not load image resource!");

	ShowControlBar(&m_wndViewBar, true, false);

	m_wndViewBar.SetWindowText("View Options");
	m_wndViewBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndObjectBar.SetWindowText("Object Tools");
	m_wndObjectBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndModeBar.SetWindowText("Modes");
	m_wndModeBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndAlignBar.SetWindowText("Alignment");
	m_wndAlignBar.EnableDocking(CBRS_ALIGN_ANY);

	EnableDocking(CBRS_ALIGN_ANY);
	
	DockControlBar(&m_wndViewBar, AFX_IDW_DOCKBAR_TOP);
	DockControlBarLeftOf(&m_wndObjectBar, &m_wndViewBar);
	DockControlBarLeftOf(&m_wndAlignBar, &m_wndObjectBar);
	DockControlBar(&m_wndModeBar, AFX_IDW_DOCKBAR_LEFT);
		
	InitializeDockingWindows();

	m_wndWorkspace.SetActiveView(0);
	m_wndTexture.SetActiveView(0);

	//load the acclerator table
	CAccelFile hFile;
	CString sPath;

	PrintSplashLine("Verify: User License Pak");
	Debug("Verify: User License Pak\n");
	sPath = GetRegString(S_TREAD_PATH);
	sPath += "\\data1.tread";

	if(hFile.Open(sPath, READ))
	{
		m_pAccelTable = hFile.ReadTable(m_nAccelCount);
		hFile.Close();
	}

	PrintSplashLine("Init: Sys_Startup");
	Debug("Init: Sys_Startup\n");
	TreadInit();

	pPreview = new CPreviewDialog;
	pPreview->Create(IDD_DIALOG_PREVIEW, this);

	PrintSplashLine("Init: Frame Control States");
	Debug("Init: Frame Control States\n");
	LoadBarState(_T("Docking\\Tread3D"));

	PrintSplashLine("Init: User Colors");
	Debug("Init: User Colors\n");
	GetColorTable()->Initialize();
	GetColorCycleController()->Initialize();

	UpdateTreadMenu();

	GetMapManager()->SetProgressPtr(m_pProgress);
	GetMapManager()->RegisterMainFrame(this);
	
	// This will only start if the registry says we can.
	//GetColorCycleController()->StartAnimating();

	PrintSplashLine("Init: Complete");
	PrintSplashLine("------------------");
	PrintSplashLine("Author: Joe Riedel, Nick Randal");
	PrintSplashLine("Very special thanks to Sean Arnold and Wayne Smith");
	PrintSplashLine("");
	PrintSplashLine("Visit our website at: http://www.planetquake.com/tread");
	PrintSplashLine("------------------");
	PrintSplashLine("Copyright © 2000 Joe Riedel and Nick Randal, All Rights Reserved.");
	PrintSplashLine("------------------");
	PrintSplashLine("This program is protected by federal and international copyright law.");
	PrintSplashLine("Distribution or replication of Tread program files without");
	PrintSplashLine("express written permission from Joe Riedel and Nicholas Randal");
	PrintSplashLine("is forbidden. This version of Tread may *not* be used for");
	PrintSplashLine("commercial purposes of any kind.");
	PrintSplashLine("Violators will be prosecuted to the full extent of the law.");
	PrintSplashLine("------------------");

	Debug("Init: Complete\n");
	Debug("------------------\n");
	Debug("Author: Joe Riedel, Nick Randal\n");
	Debug("Very special thanks to Sean Arnold and Wayne Smith\n");
	Debug("\n");
	Debug("Visit our website at: http://www.planetquake.com/tread\n");
	Debug("------------------\n");
	Debug("Copyright © 2000 Joe Riedel and Nick Randal, All Rights Reserved.\n");
	Debug("------------------\n");
	Debug("This program is protected by federal and international copyright law.\n");
	Debug("Distribution or replication of Tread program files without\n");
	Debug("express written permission from Joe Riedel and Nicholas Randal\n");
	Debug("is forbidden. This version of Tread may *not* be used for commercial purposes of any kind.\n");
	Debug("Violators will be prosecuted to the full extent of the law.\n");
	Debug("------------------\n");

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CCJMDIFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CCJMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CCJMDIFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers


void CMainFrame::OnInitMenu(CMenu* pMenu)
{
   CCJMDIFrameWnd::OnInitMenu(pMenu);
}

void CMainFrame::DockControlBarLeftOf(CToolBar* Bar,CToolBar* LeftOf)
{
	CRect rect;
	DWORD dw;
	UINT n;

	// get MFC to adjust the dimensions of all docked ToolBars
	// so that GetWindowRect will be accurate
	RecalcLayout();
	LeftOf->GetWindowRect(&rect);
	rect.OffsetRect(1,0);
	dw=LeftOf->GetBarStyle();
	n = 0;
	n = (dw&CBRS_ALIGN_TOP) ? AFX_IDW_DOCKBAR_TOP : n;
	n = (dw&CBRS_ALIGN_BOTTOM && n==0) ? AFX_IDW_DOCKBAR_BOTTOM : n;
	n = (dw&CBRS_ALIGN_LEFT && n==0) ? AFX_IDW_DOCKBAR_LEFT : n;
	n = (dw&CBRS_ALIGN_RIGHT && n==0) ? AFX_IDW_DOCKBAR_RIGHT : n;

	// When we take the default parameters on rect, DockControlBar will dock
	// each Toolbar on a seperate line.  By calculating a rectangle, we in effect
	// are simulating a Toolbar being dragged to that location and docked.
	DockControlBar(Bar,n,&rect);
}

bool CMainFrame::InitializeDockingWindows()
{
	SetInitialSize(200,200,180,180);

	if (!m_wndWorkspace.Create(this, ID_VIEW_WORKSPACE,
		_T("Workspace"), CSize(200,300), CBRS_RIGHT))
	{
		TRACE0("Failed to create dialog bar m_wndWorkspace\n");
		return false;		// fail to create
	}
	// Define the image list to use with the tab control
	m_TabImages.Create (IDB_IL_TAB, 18, 1, RGB(255,0,255));
	m_wndWorkspace.SetTabImageList(&m_TabImages);
	
	// Add the views to the tab control.
	m_wndWorkspace.AddView(_T(""), RUNTIME_CLASS(CVisgroupView));	//visgroup
	m_wndWorkspace.AddView(_T(""), RUNTIME_CLASS(CAnchorView));		//anchor
	m_wndWorkspace.AddView(_T(""), RUNTIME_CLASS(CObjectView));		//object
	m_wndWorkspace.SetViewToolTip(0, "Visgroups");
	m_wndWorkspace.SetViewToolTip(1, "Anchors");
	m_wndWorkspace.SetViewToolTip(2, "Objects");
		
	if (!m_wndTexture.Create(this, ID_VIEW_WORKSPACE2,
		_T("Textures"), CSize(200,200), CBRS_RIGHT))
	{
		TRACE0("Failed to create dialog bar m_wndTexture\n");
		return false;		// fail to create
	}
	m_TabImages2.Create (IDB_IL_TAB2, 18, 1, RGB(255,0,255));
	m_wndTexture.SetTabImageList(&m_TabImages2);

	m_wndTexture.ShowFrameControls(false, false);
	m_wndTexture.AddView(_T(""), RUNTIME_CLASS(CTextureView));	//visgroup
	m_wndTexture.SetViewToolTip(0, "Textures");

	m_wndWorkspace.EnableDockingOnSizeBar(CBRS_ALIGN_ANY);
	m_wndTexture.EnableDockingOnSizeBar(CBRS_ALIGN_ANY);

	EnableDockingSizeBar(CBRS_ALIGN_ANY);

	DockSizeBar(&m_wndWorkspace);
	DockSizeBar(&m_wndTexture);
	
	m_wndTexture.SetActiveView(0);
	m_wndWorkspace.SetActiveView(0);

	return true;
}

void CMainFrame::OnClose() 
{
	if(m_pAccelTable != NULL)
		delete [] m_pAccelTable;

	SaveBarState(_T("Docking\\Tread3D"));
	CCJMDIFrameWnd::OnClose();
}

void CMainFrame::OnViewWorkspace() 
{
	OnBarCheck(ID_VIEW_WORKSPACE);
}

void CMainFrame::OnViewWorkspace2() 
{
	OnBarCheck(ID_VIEW_WORKSPACE2);
}

void CMainFrame::OnUpdateViewWorkspace(CCmdUI* pCmdUI) 
{
	pCmdUI->m_nID = ID_VIEW_WORKSPACE;
	OnUpdateControlBarMenu(pCmdUI);
}

void CMainFrame::OnUpdateViewWorkspace2(CCmdUI* pCmdUI) 
{
	pCmdUI->m_nID = ID_VIEW_WORKSPACE2;
	OnUpdateControlBarMenu(pCmdUI);
}

void CMainFrame::OnModePointer() 
{
	if(!m_bMapOpen)
		return;

	BeginWaitCursor();
	SetCapture();

	if(GetModeManager()->IsMode(MODE_POINTER))
	{
		int nSubMode = GetModeManager()->GetSubmode(POINTER_MASK_SUBMODE);

		switch(nSubMode)
		{
		case POINTER_BRUSH:
			nSubMode = POINTER_ENTITY;
			GetMapManager()->GetDocPtr()->RemoveClassSelectionFilters(OBJECT_CLASS_ALL);
			GetMapManager()->GetDocPtr()->AddClassSelectionFilters(OBJECT_CLASS_BRUSH);
			break;
		case POINTER_ENTITY:
			nSubMode = POINTER_ALL;
			GetMapManager()->GetDocPtr()->RemoveClassSelectionFilters(OBJECT_CLASS_ALL);
			break;
		case POINTER_ALL:
			nSubMode = POINTER_BRUSH;
			GetMapManager()->GetDocPtr()->RemoveClassSelectionFilters(OBJECT_CLASS_ALL);
			GetMapManager()->GetDocPtr()->AddClassSelectionFilters(OBJECT_CLASS_ENTITY);
		//default:
		//	nSubMode = POINTER_ALL;
		//	GetMapManager()->GetDocPtr()->RemoveClassSelectionFilters(OBJECT_CLASS_ALL);
		}

		m_bUpdatePointer = true;
		GetModeManager()->SetSubmode(nSubMode, POINTER_MASK_SUBMODE);
	}
	
	GetModeManager()->SetMode(MODE_POINTER);

	ReleaseCapture();
	EndWaitCursor();
}

void CMainFrame::OnUpdateModePointer(CCmdUI* pCmdUI) 
{
	if(m_bMapOpen)
	{
		if(m_bUpdateMenu)
		{
			UpdateTreadMenu();
			m_bUpdateMenu = false;
		}

		if(m_bUpdatePointer)
		{
			CToolBarCtrl &modeBar = m_wndModeBar.GetToolBarCtrl();
			TBBUTTON button;
			int nMode = GetModeManager()->GetSubmodeFor(MODE_POINTER, POINTER_MASK_SUBMODE);

			modeBar.GetButton(0, &button);

			switch(nMode)
			{
			case POINTER_BRUSH:
				button.iBitmap = m_nPointerImage + 1;
				break;
			case POINTER_ENTITY:
				button.iBitmap = m_nPointerImage + 2;
				break;
			case POINTER_ALL:
				button.iBitmap = m_nPointerImage;
			}

			modeBar.DeleteButton(0);
			modeBar.InsertButton(0, &button);
			m_bUpdatePointer = false;
		}

		pCmdUI->SetCheck(GetModeManager()->IsMode(MODE_POINTER));
		EnableAllViews(true);
		pCmdUI->Enable(true);
	}
	else
	{
		EnableAllViews(false);
		pCmdUI->SetCheck(false);
		pCmdUI->Enable(false);
	}
}

void CMainFrame::OnModeCamera() 
{
	if(!m_bMapOpen)
		return;

	BeginWaitCursor();
	SetCapture();

	GetModeManager()->Set(MODE_CAMERA, 0);
	
	ReleaseCapture();
	EndWaitCursor();
}

void CMainFrame::OnUpdateModeCamera(CCmdUI* pCmdUI) 
{
	if(m_bMapOpen)
	{
		pCmdUI->SetCheck(GetModeManager()->IsMode(MODE_CAMERA));
		pCmdUI->Enable(true);
	}
	else
	{
		pCmdUI->SetCheck(false);
		pCmdUI->Enable(false);
	}
}

void CMainFrame::OnModeZoom() 
{
	if(!m_bMapOpen)
		return;

	BeginWaitCursor();
	SetCapture();

	GetModeManager()->Set(MODE_ZOOM, 0);

	ReleaseCapture();
	EndWaitCursor();
}

void CMainFrame::OnUpdateModeZoom(CCmdUI* pCmdUI) 
{
	if(m_bMapOpen)
	{
		pCmdUI->SetCheck(GetModeManager()->IsMode(MODE_ZOOM));
		pCmdUI->Enable(true);
	}
	else
	{
		pCmdUI->SetCheck(false);
		pCmdUI->Enable(false);
	}
}

void CMainFrame::OnModePaint() 
{
	if(!m_bMapOpen)
		return;

	BeginWaitCursor();
	SetCapture();

	//GetModeManager()->Set(MODE_PAINT, 0);
	GetMapManager()->GetDocPtr()->InvokeAction(EF_ACTION_PAINT);

	//paint the brush and flip
	//GetModeManager()->FlipLast();

	ReleaseCapture();
	EndWaitCursor();
}

void CMainFrame::OnUpdateModePaint(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_bMapOpen);
}

void CMainFrame::OnModeBrush() 
{
	if(!m_bMapOpen)
		return;

	BeginWaitCursor();
	SetCapture();

	GetModeManager()->Set(MODE_BRUSH, 0);
	m_wndWorkspace.SetActiveView(2);

	ReleaseCapture();
	EndWaitCursor();
}

void CMainFrame::OnUpdateModeBrush(CCmdUI* pCmdUI) 
{
	if(m_bMapOpen)
	{
		pCmdUI->SetCheck(GetModeManager()->IsMode(MODE_BRUSH));
		pCmdUI->Enable(true);
	}
	else
	{
		pCmdUI->SetCheck(false);
		pCmdUI->Enable(false);
	}	
}

void CMainFrame::OnModeFace() 
{
	if(!m_bMapOpen)
		return;

	BeginWaitCursor();
	SetCapture();

	ShowDialog(DLG_SURFACE, true);
	GetSurfaceDialog()->TextureLockUpdate();
	GetModeManager()->Set(MODE_FACE, 0);
	m_bFaceDialog = true;

	ReleaseCapture();
	EndWaitCursor();
}

void CMainFrame::OnUpdateModeFace(CCmdUI* pCmdUI) 
{
	if(m_bMapOpen)
	{
		bool bMode = GetModeManager()->IsMode(MODE_FACE);
		pCmdUI->SetCheck(bMode);
		pCmdUI->Enable(true);

		if(m_bFaceDialog && !bMode)
			ShowDialog(DLG_SURFACE, false);
	}
	else
	{
		if(m_bFaceDialog)
			ShowDialog(DLG_SURFACE, false);
		pCmdUI->SetCheck(false);
		pCmdUI->Enable(false);
	}
}

void CMainFrame::OnModeVertex() 
{
	if(!m_bMapOpen)
		return;

	BeginWaitCursor();
	SetCapture();

	GetModeManager()->Set(MODE_VERTEX, 0);
	
	ReleaseCapture();
	EndWaitCursor();
}

void CMainFrame::OnModeClip() 
{
	BeginWaitCursor();
	SetCapture();

	//identify if we are already in the mode
	if(GetModeManager()->IsMode(MODE_CLIP))
	{
		int nSubMode = GetModeManager()->GetSubmode(CLIP_MASK_SUBMODE);

		switch(nSubMode)
		{
		case CLIP_CLIP:
			nSubMode = CLIP_SPLIT;
			break;
		case CLIP_SPLIT:
			nSubMode = CLIP_CLIP;
			break;
		}

		m_bUpdateClip = true;
		GetModeManager()->SetSubmode(nSubMode, CLIP_MASK_SUBMODE);
	}
	
	GetModeManager()->SetMode(MODE_CLIP);

	ReleaseCapture();
	EndWaitCursor();
}

void CMainFrame::OnModeEntity() 
{
	if(!m_bMapOpen)
		return;

	BeginWaitCursor();
	SetCapture();

	GetModeManager()->Set(MODE_ENTITY, 0);
	m_wndWorkspace.SetActiveView(2);

	ReleaseCapture();
	EndWaitCursor();
}

void CMainFrame::OnUpdateModeVertex(CCmdUI* pCmdUI) 
{
	if(m_bMapOpen)
	{
		pCmdUI->SetCheck(GetModeManager()->IsMode(MODE_VERTEX));
		pCmdUI->Enable(true);
	}
	else
	{
		pCmdUI->SetCheck(false);
		pCmdUI->Enable(false);
	}
}

void CMainFrame::OnUpdateModeClip(CCmdUI* pCmdUI) 
{
	if(m_bMapOpen)
	{
		if(m_bUpdateClip)
		{
			CToolBarCtrl &modeBar = m_wndModeBar.GetToolBarCtrl();
			TBBUTTON button;
			int nMode = GetModeManager()->GetSubmodeFor(MODE_CLIP, CLIP_MASK_SUBMODE);

			modeBar.GetButton(11, &button);

			switch(nMode)
			{
			case CLIP_CLIP:
				button.iBitmap = m_nClipImage;
				break;
			case CLIP_SPLIT:
				button.iBitmap = m_nClipImage + 1;
				break;
			}

			modeBar.DeleteButton(11);
			modeBar.InsertButton(11, &button);
			m_bUpdateClip = false;
		}

		pCmdUI->SetCheck(GetModeManager()->IsMode(MODE_CLIP));
		pCmdUI->Enable(true);
	}
	else
	{
		pCmdUI->SetCheck(false);
		pCmdUI->Enable(false);
	}
}

void CMainFrame::OnModeTexlock() 
{
	if(!m_bMapOpen)
		return;

	BeginWaitCursor();
	SetCapture();

	GetModeManager()->ToggleFlag(FLAG_TEXLOCK);
	GetSurfaceDialog()->TextureLockUpdate();

	ReleaseCapture();
	EndWaitCursor();
}

void CMainFrame::OnUpdateModeTexlock(CCmdUI* pCmdUI) 
{
	if(m_bMapOpen)
	{
		pCmdUI->SetCheck(GetModeManager()->IsFlag(FLAG_TEXLOCK));
		pCmdUI->Enable(true);
	}
	else
	{
		pCmdUI->SetCheck(false);
		pCmdUI->Enable(false);
	}
}

void CMainFrame::OnRunmap() 
{
	if(!m_bMapOpen)
		return;

	CTread3DDoc* pDoc = GetMapManager()->GetDocPtr();

	if(pDoc->IsModified())
	{
		if(MessageBox("The map must be saved before it can be built.\nSave it now?",
			"Save Map", MB_YESNOCANCEL | MB_ICONQUESTION) == IDYES)
		{
			CString sPath;
			CString sFile = pDoc->GetPathName();
			if(sFile == "")
				sFile = "unknown.trd2";

			CFileDialog fileDlg(false, "trd2", sFile,
				OFN_NONETWORKBUTTON | OFN_OVERWRITEPROMPT,
				"Tread3D 2.0 Files (*.trd2)|*.trd2", this);
			if(fileDlg.DoModal() != IDOK)
				return;

			sPath = fileDlg.GetPathName();
			pDoc->OnSaveDocument(sPath);
			pDoc->SetTitle(fileDlg.GetFileTitle());
		}
		else
			return;
	}

	CBuildDialog buildDlg;
	if(buildDlg.DoModal() == IDCANCEL)
		return;

	m_bBuildMap = true;
	GetModeManager()->SetFlag(FLAG_BUILDING, true);

	// Run.
	pDoc->RunMap();
	GetModeManager()->SetFlag(FLAG_TEXLOCK, false);
	m_bBuildMap = false;
}

void CMainFrame::OnUpdateRunmap(CCmdUI* pCmdUI) 
{
		pCmdUI->Enable(m_bMapOpen & !m_bBuildMap);
}


void CMainFrame::OnUpdateModeEntity(CCmdUI* pCmdUI) 
{
	if(m_bMapOpen)
	{
		pCmdUI->SetCheck(GetModeManager()->IsMode(MODE_ENTITY));
		pCmdUI->Enable(true);
	}
	else
	{
		pCmdUI->SetCheck(false);
		pCmdUI->Enable(false);
	}
}

void CMainFrame::OnOptionsKeybinding() 
{
	CBindDialog dlgBind;
	CString sPath;
	CAccelFile hFile;

	dlgBind.SetTableHandle(m_pAccelTable, m_nAccelCount);
	if(dlgBind.DoModal() == IDCANCEL)
		return;

	m_pAccelTable = dlgBind.GetTableRef();
	m_nAccelCount = dlgBind.GetTableCount();

	sPath = GetRegString(S_TREAD_PATH);
	sPath += "\\data1.tread";
	if(hFile.Open(sPath, WRITE))
	{
		hFile.WriteTable(m_pAccelTable, m_nAccelCount);
		hFile.Close();
	}

	UpdateTreadMenu();
}

void CMainFrame::OnOptionsDebugwindow() 
{
	CConsoleDialog* pTemp = (CConsoleDialog*)GetDialog(DLG_DEBUG);

	if(pTemp != NULL)
		pTemp->ShowWindow(SW_SHOW);
	else
		Warning("Debug window not available.\n");
}

BOOL CMainFrame::PreTranslateMessage(MSG* pMsg) 
{
	//if the message belongs to a dialog then ignore the message
	if(!m_bAccel && IsDialogMessage(pMsg))
	{
		//Dialog Message
		//Debug("Message Pump: Dialog Message Ignored\n");
		return true;
	}

	if(pMsg->message >= WM_KEYFIRST && pMsg->message <= WM_KEYLAST && pMsg->message == WM_KEYDOWN)
	{
		//if a shift key or ctrl key then mark the modifier to true and return
		if(pMsg->wParam == 16 || pMsg->wParam == 17)
		{
			m_bModifier = true;
			return true;
		}
		else if(m_bModifier == true)	//if a modifier key is being held then let system handle it
			return CCJMDIFrameWnd::PreTranslateMessage(pMsg);

		//if we have gotten this far then it is just a simple key press
		//loop through the list and see if there is a match
		for(int n = 0; n < m_nAccelCount; n++)
		{
			//if a match is found then send a message with the command
			if(pMsg->wParam == m_pAccelTable[n].key)
			{
				SendMessage(WM_COMMAND, m_pAccelTable[n].cmd, 0);
				return true;
			}
		}
	}
	else if(pMsg->message >= WM_KEYFIRST && pMsg->message <= WM_KEYLAST && pMsg->message == WM_KEYUP)
	{
		//if the shift or ctrl keys have been released then change the modifier to false
		if(pMsg->wParam == 16 || pMsg->wParam == 17)
			m_bModifier = false;
	}

	return CCJMDIFrameWnd::PreTranslateMessage(pMsg);
}

void CMainFrame::OnOptionsColoroptions() 
{
	CClrDialog dlgTemp;
	dlgTemp.DoModal();

	GetColorTable()->SaveTable(GetRegString(S_TREAD_PATH) + "\\data2.tread");
	if(GetMapManager()->GetMapCount() > 0)
		GetMapManager()->GetCurrentMap()->GetDocument()->UpdateWindows();
}

void CMainFrame::OnObjectLinkEntity() 
{
	CEntity* pEntity = GetGameAPI()->GetCurrentGame()->CreateEntityFromDefinition(GetGameAPI()->GetCurrentGame()->GetDefSolidString());
	if(pEntity == NULL)
	{
		Error("Unable to create '" + GetGameAPI()->GetCurrentGame()->GetDefSolidString() + "' entity.");
		return;
	}

	CEntityDialog dlgTemp;
	dlgTemp.SetEntity(pEntity);
	delete pEntity;
	pEntity = NULL;

	//open the dialog to get user selection
	if(dlgTemp.DoModal() == IDCANCEL)
		return;

	pEntity = dlgTemp.GetEntity();
	if(pEntity == NULL)
		return;

	GetMapManager()->GetDocPtr()->InvokeAction(EF_ACTION_LINK, pEntity);

	GetMapManager()->GetDocPtr()->UpdateWindows();
}

void CMainFrame::OnObjectProperties() 
{
	CLinkedList<CBaseObject>* pObjectList;
	CBaseObject* pObject;
	CEntity* pEntity;
	CEntity* pNewEnt;
	CEntityDialog dlgTemp;

	//get the selected object list
	pObjectList = GetMapManager()->GetDocPtr()->GetSelectedObjectList();
	if(pObjectList->IsEmpty())
		return;

	//find the first entity
	pObjectList->First();
	for(pObject = pObjectList->GetCurItem(); pObject != NULL; pObject = pObjectList->GetNextItem())
	{
		if(pObject->GetObjectClass() == OBJECT_CLASS_ENTITY)
		{
			pEntity = (CEntity*)pObject;
			break;
		}
		else if(pObject->GetObjectClass() == OBJECT_CLASS_LENTITY)
		{
			pEntity = (CEntity*)pObject;
			break;
		}
	}

	//set the entity and show the dialog
	dlgTemp.SetEntity(pEntity);
	if(dlgTemp.DoModal() == IDCANCEL)
		return;

	//get the entity that the user selected
	pEntity = dlgTemp.GetEntity();
	if(pEntity == NULL)
	{
		Error("Entity Dialog returned NULL\n");
		return;
	}
	
	pObjectList->First();
	for(pObject = pObjectList->GetCurItem(); pObject != NULL; pObject = pObjectList->GetNextItem())
	{
		if(pObject->GetObjectClass() != OBJECT_CLASS_ENTITY &&
			pObject->GetObjectClass() != OBJECT_CLASS_LENTITY)
			continue;

		pNewEnt = (CEntity*)pObject;
		pNewEnt->MorphEntity(pEntity);
	}

	delete pEntity;

	//fix all entities that have been selected
	//according to wether it is OWNER or NOT

	GetMapManager()->GetDocPtr()->UpdateWindows();
}

void CMainFrame::OnSelchangeGridCombo()
{
	int n = m_pGridBox->GetCurSel();
	if(n == -1)
		return;
	GetMapManager()->GetCurrentMap()->SetViewsGrid(pGridValues[n]);
}

void CMainFrame::OnSelchangeZoomCombo()
{
	int n = m_pZoomBox->GetCurSel();
	if(n == -1)
		return;
	GetMapManager()->GetCurrentMap()->SetViewsZoom(pZoomValues[n]);
}

void CMainFrame::SetComboGrid(int nGrid)
{
	CString sBuffer;
	int n;
	
	sBuffer.Format("%u", nGrid);
	n = m_pGridBox->FindString(-1, sBuffer);
	m_pGridBox->SetCurSel(n);
}

void CMainFrame::SetComboZoom(float fZoom)
{
	CString sBuffer;
	int n = (fZoom * 100);
	
	sBuffer.Format("%u%%", n);
	n = m_pZoomBox->FindString(-1, sBuffer);
	m_pZoomBox->SetCurSel(n);
}

void CMainFrame::OnUpdateGridCombo(CCmdUI* pCmdUI) 
{
	if(m_bMapOpen)
		pCmdUI->Enable(true);
	else
		pCmdUI->Enable(false);
}

void CMainFrame::OnUpdateZoomCombo(CCmdUI* pCmdUI) 
{
	if(m_bMapOpen)
		pCmdUI->Enable(true);
	else
		pCmdUI->Enable(false);
}

void CMainFrame::OnOptionsGamesetup() 
{
	CGameSetupDialog dlgTemp;

	BeginWaitCursor();
	SetCapture();

	dlgTemp.DoModal();

	ReleaseCapture();
	EndWaitCursor();
}

void CMainFrame::OnObjectCarve() 
{
	GetMapManager()->GetDocPtr()->InvokeAction(EF_ACTION_CARVE);
}

void CMainFrame::OnUpdateObjectCarve(CCmdUI* pCmdUI) 
{
	if(m_bMapOpen)
	{
		pCmdUI->Enable(GetMapManager()->GetDocPtr()->IsInterfaceSelected(I_CARVE));
	}
	else
		pCmdUI->Enable(false);
}

void CMainFrame::OnObjectFlipHorizontal() 
{
	GetMapManager()->GetDocPtr()->InvokeAction(EF_ACTION_FLIPHORZ);
}

void CMainFrame::OnObjectFlipVertical() 
{
	GetMapManager()->GetDocPtr()->InvokeAction(EF_ACTION_FLIPVERT);
}

void CMainFrame::OnUpdateObjectFlipHorizontal(CCmdUI* pCmdUI) 
{
	if(m_bMapOpen)
		pCmdUI->Enable(GetMapManager()->GetDocPtr()->IsInterfaceSelected(I_TRANSFORMABLE));
	else
		pCmdUI->Enable(false);
}

void CMainFrame::OnUpdateObjectFlipVertical(CCmdUI* pCmdUI) 
{
	if(m_bMapOpen)
		pCmdUI->Enable(GetMapManager()->GetDocPtr()->IsInterfaceSelected(I_TRANSFORMABLE));
	else
		pCmdUI->Enable(false);
}

void CMainFrame::OnObjectGroup() 
{
	GetMapManager()->GetDocPtr()->InvokeAction(EF_ACTION_GROUP);
}

void CMainFrame::OnUpdateObjectGroup(CCmdUI* pCmdUI) 
{
	if(m_bMapOpen)
	{
		pCmdUI->Enable(GetMapManager()->GetDocPtr()->GetSelectedInterfaceCount(I_GROUPABLE) > 1);
	}
	else
		pCmdUI->Enable(false);
}

void CMainFrame::OnObjectHollow() 
{
	CHollowDialog dlgTemp;
	dlgTemp.SetUnit(GetMapManager()->GetDocPtr()->GetHollowDepth());
	if(dlgTemp.DoModal() == IDCANCEL)
		return;
	
	float fSize = dlgTemp.GetUnit();

	GetMapManager()->GetDocPtr()->SetHollowDepth(fSize);
	GetMapManager()->GetDocPtr()->InvokeAction(EF_ACTION_HOLLOW, &fSize);
}

void CMainFrame::OnUpdateObjectHollow(CCmdUI* pCmdUI) 
{
	if(m_bMapOpen)
		pCmdUI->Enable(GetMapManager()->GetDocPtr()->IsInterfaceSelected(I_HOLLOW));
	else
		pCmdUI->Enable(false);
}

void CMainFrame::OnUpdateObjectLinkEntity(CCmdUI* pCmdUI) 
{
	if(m_bMapOpen)
	{
		if(GetMapManager()->GetDocPtr()->GetSelectedBrushCount() > 0)
			pCmdUI->Enable(true);
		else
			pCmdUI->Enable(false);
	}
	else
		pCmdUI->Enable(false);
}

void CMainFrame::OnUpdateObjectProperties(CCmdUI* pCmdUI) 
{
	if(m_bMapOpen)
	{
		if(GetMapManager()->GetDocPtr()->GetSelectedEntityCount() > 0)
			pCmdUI->Enable(true);
		else
			pCmdUI->Enable(false);
	}
	else
		pCmdUI->Enable(false);
}

void CMainFrame::OnObjectRotate180Cw() 
{
	UINT nView = GetMapManager()->GetCurrentMap()->GetCurrentViewType();
	GetMapManager()->GetDocPtr()->InvokeAction(EF_ACTION_ROT180, &nView);
}

void CMainFrame::OnUpdateObjectRotate180Cw(CCmdUI* pCmdUI) 
{
	if(m_bMapOpen)
	{
		if(GetMapManager()->GetDocPtr()->GetSelectedInterfaceCount(I_TRANSFORMABLE) > 0)
			pCmdUI->Enable(true);
		else
			pCmdUI->Enable(false);
	}
	else
		pCmdUI->Enable(false);
}

void CMainFrame::OnObjectRotate90Ccw() 
{
	UINT nView = GetMapManager()->GetCurrentMap()->GetCurrentViewType();
	GetMapManager()->GetDocPtr()->InvokeAction(EF_ACTION_ROT90CCW, &nView);
}

void CMainFrame::OnUpdateObjectRotate90Ccw(CCmdUI* pCmdUI) 
{
	if(m_bMapOpen)
	{
		if(GetMapManager()->GetDocPtr()->GetSelectedInterfaceCount(I_TRANSFORMABLE) > 0)
			pCmdUI->Enable(true);
		else
			pCmdUI->Enable(false);
	}
	else
		pCmdUI->Enable(false);
}

void CMainFrame::OnObjectRotate90Cw() 
{
	UINT nView = GetMapManager()->GetCurrentMap()->GetCurrentViewType();
	GetMapManager()->GetDocPtr()->InvokeAction(EF_ACTION_ROT90CW, &nView);
}

void CMainFrame::OnUpdateObjectRotate90Cw(CCmdUI* pCmdUI) 
{
	if(m_bMapOpen)
	{
		if(GetMapManager()->GetDocPtr()->GetSelectedInterfaceCount(I_TRANSFORMABLE) > 0)
			pCmdUI->Enable(true);
		else
			pCmdUI->Enable(false);
	}
	else
		pCmdUI->Enable(false);
}

void CMainFrame::OnObjectUngroup() 
{
	GetMapManager()->GetDocPtr()->InvokeAction(EF_ACTION_UNGROUP);
}

void CMainFrame::OnUpdateObjectUngroup(CCmdUI* pCmdUI) 
{
	if(m_bMapOpen)
	{
		pCmdUI->Enable(GetMapManager()->GetDocPtr()->GetSelectedInterfaceCount(I_GROUP) > 0);
	}
	else
		pCmdUI->Enable(false);
}

void CMainFrame::OnObjectUnlinkEntity() 
{
	CLinkedList<CBaseObject>* pObjectList;
	CBaseObject* pObject;
	CEntity* pEntity;
	
	//get the selected object list
	pObjectList = GetMapManager()->GetDocPtr()->GetSelectedObjectList();
	if(pObjectList->IsEmpty())
		return;

	pObjectList->First();
	for(pObject = pObjectList->GetCurItem(); pObject != NULL; pObject = pObjectList->GetNextItem())
	{
		if(pObject->GetObjectClass() == OBJECT_CLASS_LENTITY)
		{
			pEntity = (CEntity*)pObject;

			if(pEntity->GetEntityDefinition()->bOwner == true)
			{
				GetMapManager()->GetDocPtr()->InvokeAction(EF_ACTION_UNLINK);
			}
		}
	}
}

void CMainFrame::OnUpdateObjectUnlinkEntity(CCmdUI* pCmdUI) 
{
	if(m_bMapOpen)
	{
		if(GetMapManager()->GetDocPtr()->GetSelectedObjectCount(OBJECT_CLASS_LENTITY) > 0)
			pCmdUI->Enable(true);
		else
			pCmdUI->Enable(false);
	}
	else
		pCmdUI->Enable(false);
}

void CMainFrame::OnViewFrontview() 
{
	CMapFrame* pFrame = GetMapManager()->GetCurrentMap();
	if(pFrame->GetCurrentViewType() == VIEW_3D)
		return;
	pFrame->GetCurrentView()->SetView(VIEW_FRONT);
}

void CMainFrame::OnViewTopview() 
{
	CMapFrame* pFrame = GetMapManager()->GetCurrentMap();
	if(pFrame->GetCurrentViewType() == VIEW_3D)
		return;
	pFrame->GetCurrentView()->SetView(VIEW_TOP);
}

void CMainFrame::OnViewSideview() 
{
	CMapFrame* pFrame = GetMapManager()->GetCurrentMap();
	if(pFrame->GetCurrentViewType() == VIEW_3D)
		return;
	pFrame->GetCurrentView()->SetView(VIEW_SIDE);
}

void CMainFrame::OnUpdateViewFrontview(CCmdUI* pCmdUI) 
{
	if(m_bMapOpen)
	{
		pCmdUI->Enable(true);
		if(GetMapManager()->GetCurrentMap()->GetCurrentViewType() == VIEW_FRONT)
			pCmdUI->SetRadio(true);
		else
			pCmdUI->SetRadio(false);
	}
	else
		pCmdUI->Enable(false);
}

void CMainFrame::OnUpdateViewSideview(CCmdUI* pCmdUI) 
{
	if(m_bMapOpen)
	{
		pCmdUI->Enable(true);
		if(GetMapManager()->GetCurrentMap()->GetCurrentViewType() == VIEW_SIDE)
			pCmdUI->SetRadio(true);
		else
			pCmdUI->SetRadio(false);
	}
	else
		pCmdUI->Enable(false);
}

void CMainFrame::OnUpdateViewTopview(CCmdUI* pCmdUI) 
{
	if(m_bMapOpen)
	{
		pCmdUI->Enable(true);
		if(GetMapManager()->GetCurrentMap()->GetCurrentViewType() == VIEW_TOP)
			pCmdUI->SetRadio(true);
		else
			pCmdUI->SetRadio(false);
	}
	else
		pCmdUI->Enable(false);
}

void CMainFrame::OnViewGdi() 
{
	BeginWaitCursor();
	SetCapture();

	GetMapManager()->GetDocPtr()->UnbindViews(FLAT_VIEWS);
	if(GetRenderAPI()->Load2DRenderPackage("wingdi") == false)
		return;
	GetMapManager()->GetDocPtr()->RebindViews(FLAT_VIEWS);
	GetMapManager()->GetDocPtr()->UpdateWindows();
	SetConsole("", 200, RGB(0,50,150));
	Console("Switched Rendering Package to: ");
	SetConsole("", -1, RGB(0,150,50));
	Console("Windows GDI\n");
	ResetConsole();
	SetRegString(S_2D_PAK, "wingdi");

	ReleaseCapture();
	EndWaitCursor();
}

void CMainFrame::OnViewOpenglsoft() 
{
	BeginWaitCursor();
	SetCapture();

	GetMapManager()->GetDocPtr()->UnbindViews(FLAT_VIEWS);
	if(GetRenderAPI()->Load2DRenderPackage("sft_wogl") == false)
		return;
	GetMapManager()->GetDocPtr()->RebindViews(FLAT_VIEWS);
	GetMapManager()->GetDocPtr()->UpdateWindows();
	SetConsole("", 200, RGB(0,50,150));
	Console("Switched Rendering Package to: ");
	SetConsole("", -1, RGB(0,150,50));
	Console("OpenGL - Software\n");
	ResetConsole();
	SetRegString(S_2D_PAK, "sft_wogl");

	ReleaseCapture();
	EndWaitCursor();
}

void CMainFrame::OnViewOpengl() 
{
	BeginWaitCursor();
	SetCapture();

	GetMapManager()->GetDocPtr()->UnbindViews(FLAT_VIEWS);
	if(GetRenderAPI()->Load2DRenderPackage("hdw_wogl") == false)
		return;
	GetMapManager()->GetDocPtr()->RebindViews(FLAT_VIEWS);
	GetMapManager()->GetDocPtr()->UpdateWindows();
	SetConsole("", 200, RGB(0,50,150));
	Console("Switched Rendering Package to: ");
	SetConsole("", -1, RGB(0,150,50));
	Console("OpenGL - Hardware\n");
	ResetConsole();
	SetRegString(S_2D_PAK, "hdw_wogl");

	ReleaseCapture();
	EndWaitCursor();
}

void CMainFrame::OnUpdateViewGdi(CCmdUI* pCmdUI) 
{
	if(GetRegString(S_2D_PAK) == "wingdi")
		pCmdUI->SetRadio(true);
	else
		pCmdUI->SetRadio(false);
}

void CMainFrame::OnUpdateViewOpenglsoft(CCmdUI* pCmdUI) 
{
	if(GetRegString(S_2D_PAK) == "sft_wogl")
		pCmdUI->SetRadio(true);
	else
		pCmdUI->SetRadio(false);
}

void CMainFrame::OnUpdateViewOpengl(CCmdUI* pCmdUI) 
{
	if(GetRegString(S_2D_PAK) == "hdw_wogl")
		pCmdUI->SetRadio(true);
	else
		pCmdUI->SetRadio(false);
}

void CMainFrame::On3dLine() 
{
	GetMapManager()->GetDocPtr()->RemoveGlobalRenderFlags(RF_SOLID | RF_TEXTURED);
	GetMapManager()->GetDocPtr()->AddGlobalRenderFlags(RF_LINE);
	GetMapManager()->GetDocPtr()->UpdateWindows();
}

void CMainFrame::On3dSolid() 
{
	GetMapManager()->GetDocPtr()->RemoveGlobalRenderFlags(RF_LINE | RF_TEXTURED);
	GetMapManager()->GetDocPtr()->AddGlobalRenderFlags(RF_SOLID);
	GetMapManager()->GetDocPtr()->UpdateWindows();
}

void CMainFrame::On3dTextured() 
{
	GetMapManager()->GetDocPtr()->RemoveGlobalRenderFlags(RF_SOLID | RF_LINE);
	GetMapManager()->GetDocPtr()->AddGlobalRenderFlags(RF_TEXTURED);
	GetMapManager()->GetDocPtr()->UpdateWindows();
}

void CMainFrame::On3dShaded() 
{
	if(GetMapManager()->GetDocPtr()->GetGlobalRenderFlags() & RF_SHADED)
		GetMapManager()->GetDocPtr()->RemoveGlobalRenderFlags(RF_SHADED);
	else
		GetMapManager()->GetDocPtr()->AddGlobalRenderFlags(RF_SHADED);
	GetMapManager()->GetDocPtr()->UpdateWindows();
}

void CMainFrame::OnMapNoentityfill() 
{
	GetMapManager()->GetDocPtr()->ToggleRenderFlag(RF_NOENTITYFILL);
	GetMapManager()->GetDocPtr()->UpdateWindows();
}

void CMainFrame::OnMapNoselboxsize() 
{
	GetMapManager()->GetDocPtr()->ToggleRenderFlag(RF_NOSELBOXSIZE);
	GetMapManager()->GetDocPtr()->UpdateWindows();
}

void CMainFrame::On3dRealtime() 
{
	GetMapManager()->GetDocPtr()->ToggleRenderFlag(RF_REALTIME3D);
	GetMapManager()->GetDocPtr()->UpdateWindows();
}

void CMainFrame::On3dNoselboxoutline() 
{
	GetMapManager()->GetDocPtr()->ToggleRenderFlag(RF_NO3DSELOUTLINE);
	GetMapManager()->GetDocPtr()->UpdateWindows();
}

void CMainFrame::OnMapNoentnames() 
{
	GetMapManager()->GetDocPtr()->ToggleRenderFlag(RF_NOENTNAMES);
	GetMapManager()->GetDocPtr()->UpdateWindows();
}

void CMainFrame::On3dNobilinear() 
{
	GetMapManager()->GetDocPtr()->ToggleRenderFlag(RF_NOBILINEARFILTER);
	GetMapManager()->GetDocPtr()->RebindViews(VIEW_3D);
	GetMapManager()->GetDocPtr()->UpdateWindows();
}

void CMainFrame::On3dNodraw() 
{
	GetMapManager()->GetDocPtr()->ToggleRenderFlag(RF_NO3DVIEW);
	GetMapManager()->GetDocPtr()->UpdateWindows();
}

void CMainFrame::OnUpdate3dNobilinear(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(!(GetMapManager()->GetDocPtr()->GetGlobalRenderFlags() & RF_NOBILINEARFILTER));
}

void CMainFrame::OnUpdate3dNoselboxoutline(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(!(GetMapManager()->GetDocPtr()->GetGlobalRenderFlags() & RF_NO3DSELOUTLINE));
}

void CMainFrame::OnUpdateMapNoentnames(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(!(GetMapManager()->GetDocPtr()->GetGlobalRenderFlags() & RF_NOENTNAMES));
}

void CMainFrame::OnUpdateMapNoentityfill(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(!(GetMapManager()->GetDocPtr()->GetGlobalRenderFlags() & RF_NOENTITYFILL));
}

void CMainFrame::OnUpdateMapNoselboxsize(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(!(GetMapManager()->GetDocPtr()->GetGlobalRenderFlags() & RF_NOSELBOXSIZE));
}

void CMainFrame::OnUpdate3dRealtime(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(GetMapManager()->GetDocPtr()->GetGlobalRenderFlags() & RF_REALTIME3D);
}

void CMainFrame::OnUpdate3dLine(CCmdUI* pCmdUI) 
{
	pCmdUI->SetRadio(GetMapManager()->GetDocPtr()->GetGlobalRenderFlags() & RF_LINE);
}

void CMainFrame::OnUpdate3dSolid(CCmdUI* pCmdUI) 
{
	pCmdUI->SetRadio(GetMapManager()->GetDocPtr()->GetGlobalRenderFlags() & RF_SOLID);
}

void CMainFrame::OnUpdate3dTextured(CCmdUI* pCmdUI) 
{
	pCmdUI->SetRadio(GetMapManager()->GetDocPtr()->GetGlobalRenderFlags() & RF_TEXTURED);
}

void CMainFrame::OnUpdate3dShaded(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(GetMapManager()->GetDocPtr()->GetGlobalRenderFlags() & RF_SHADED);
}

void CMainFrame::OnUpdate3dNodraw(CCmdUI* pCmdUI) 
{
	if(m_bMapOpen)
	{
		pCmdUI->Enable(true);
		pCmdUI->SetCheck(!(GetMapManager()->GetDocPtr()->GetGlobalRenderFlags() & RF_NO3DVIEW));
	}
	else
	{
		pCmdUI->Enable(false);
		pCmdUI->SetCheck(false);
	}
}

void CMainFrame::OnEditCopy() 
{
	GetMapManager()->GetDocPtr()->InvokeAction(EF_ACTION_COPY);
}

void CMainFrame::OnEditCut() 
{
	GetMapManager()->GetDocPtr()->InvokeAction(EF_ACTION_CUT);
}

void CMainFrame::OnEditDelete() 
{
	GetMapManager()->GetDocPtr()->InvokeAction(EF_ACTION_DELETE);
}

void CMainFrame::OnEditPaste() 
{
	GetMapManager()->GetDocPtr()->InvokeAction(EF_ACTION_PASTE);
}

void CMainFrame::OnEditUndo() 
{
	GetMapManager()->GetDocPtr()->InvokeAction(EF_ACTION_UNDO);
}

void CMainFrame::OnEditRedo() 
{
	GetMapManager()->GetDocPtr()->InvokeAction(EF_ACTION_REDO);
}

void CMainFrame::OnUpdateEditUndo(CCmdUI* pCmdUI) 
{
	if(m_bMapOpen)
	{
		CUndoAPI* pUndo = GetMapManager()->GetDocPtr()->GetUndoAPI();
		if(pUndo->CanUndo())
		{
			pCmdUI->Enable(true);
			pCmdUI->SetText(pUndo->GetUndoString() + "\tCtrl+Z");
		}
		else
		{
			pCmdUI->Enable(false);
			pCmdUI->SetText("Undo\tCtrl+Z");
		}
	}
	else
	{
		pCmdUI->Enable(false);
	}
}

void CMainFrame::OnUpdateEditRedo(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if(m_bMapOpen)
	{
		CUndoAPI* pUndo = GetMapManager()->GetDocPtr()->GetUndoAPI();
		if(pUndo->CanRedo())
		{
			pCmdUI->Enable(true);
			pCmdUI->SetText(pUndo->GetRedoString() + "\tCtrl+R");
		}
		else
		{
			pCmdUI->Enable(false);
			pCmdUI->SetText("Redo\tCtrl+R");
		}
	}
	else
	{
		pCmdUI->Enable(false);
	}
}

void CMainFrame::OnUpdateEditCopy(CCmdUI* pCmdUI) 
{
	if(m_bMapOpen)
	{
		pCmdUI->Enable(GetMapManager()->GetDocPtr()->GetSelectedObjectCount() > 0);
	}
	else
	{
		pCmdUI->Enable(false);
	}
}

void CMainFrame::OnUpdateEditCut(CCmdUI* pCmdUI) 
{
	if(m_bMapOpen)
	{
		pCmdUI->Enable(GetMapManager()->GetDocPtr()->GetSelectedObjectCount() > 0);
	}
	else
	{
		pCmdUI->Enable(false);
	}	
}

void CMainFrame::OnUpdateEditDelete(CCmdUI* pCmdUI) 
{
	if(m_bMapOpen)
	{
		pCmdUI->Enable(GetMapManager()->GetDocPtr()->GetSelectedObjectCount() > 0 || GetModeManager()->IsMode(MODE_CAMERA));
	}
	else
	{
		pCmdUI->Enable(false);
	}
}

void CMainFrame::OnUpdateEditPaste(CCmdUI* pCmdUI) 
{
	if(m_bMapOpen)
	{
		pCmdUI->Enable(true);
	}
	else
	{
		pCmdUI->Enable(false);
	}
}

void CMainFrame::OnAddvisgroup() 
{
	CVisgroupDialog dlgTemp;
	dlgTemp.DoModal();
}

void CMainFrame::OnUpdateAddvisgroup(CCmdUI* pCmdUI) 
{
	if(m_bMapOpen)
	{
		CTread3DDoc* pDoc = GetMapManager()->GetDocPtr();
		if((pDoc->GetSelectedBrushCount() > 0) || (pDoc->GetSelectedEntityCount() > 0))
			pCmdUI->Enable(true);
		else
			pCmdUI->Enable(false);
	}
	else
		pCmdUI->Enable(false);
}

void CMainFrame::OnTreadSetup() 
{
	// TODO: Add your command handler code here
	CSetupDialog dlgTemp;
	dlgTemp.DoModal();

	GetTexAPI()->FreeAllLoadedTextures();
	GetView(TEXTURE_VIEW)->RedrawWindow();
	
}

void CMainFrame::OnToolbarModes() 
{
	CWnd* pWnd = (CWnd*)&m_wndModeBar;
	if(pWnd->GetStyle() & WS_VISIBLE)
		ShowControlBar(&m_wndModeBar, false, false);
	else
		ShowControlBar(&m_wndModeBar, true, false);
}

void CMainFrame::OnToolbarObjects() 
{
	CWnd* pWnd = (CWnd*)&m_wndObjectBar;
	if(pWnd->GetStyle() & WS_VISIBLE)
		ShowControlBar(&m_wndObjectBar, false, false);
	else
		ShowControlBar(&m_wndObjectBar, true, false);
}

void CMainFrame::OnToolbarView() 
{
	CWnd* pWnd = (CWnd*)&m_wndViewBar;
	if(pWnd->GetStyle() & WS_VISIBLE)
		ShowControlBar(&m_wndViewBar, false, false);
	else
		ShowControlBar(&m_wndViewBar, true, false);
}


void CMainFrame::OnToolbarAlign() 
{
	CWnd* pWnd = (CWnd*)&m_wndAlignBar;
	if(pWnd->GetStyle() & WS_VISIBLE)
		ShowControlBar(&m_wndAlignBar, false, false);
	else
		ShowControlBar(&m_wndAlignBar, true, false);}

void CMainFrame::OnUpdateToolbarAlign(CCmdUI* pCmdUI) 
{
	CWnd* pWnd = (CWnd*)&m_wndAlignBar;
	pCmdUI->SetCheck(pWnd->GetStyle() & WS_VISIBLE);
}

void CMainFrame::OnUpdateToolbarModes(CCmdUI* pCmdUI) 
{
	CWnd* pWnd = (CWnd*)&m_wndModeBar;
	pCmdUI->SetCheck(pWnd->GetStyle() & WS_VISIBLE);
}

void CMainFrame::OnUpdateToolbarObjects(CCmdUI* pCmdUI) 
{
	CWnd* pWnd = (CWnd*)&m_wndObjectBar;
	pCmdUI->SetCheck(pWnd->GetStyle() & WS_VISIBLE);
}

void CMainFrame::OnUpdateToolbarView(CCmdUI* pCmdUI) 
{
	CWnd* pWnd = (CWnd*)&m_wndViewBar;
	pCmdUI->SetCheck(pWnd->GetStyle() & WS_VISIBLE);
}

void CMainFrame::OnEntityPath() 
{
	GetMapManager()->GetDocPtr()->InvokeAction(EF_ACTION_MAKEPATHS);
}

void CMainFrame::OnEntityPathloop() 
{
	GetMapManager()->GetDocPtr()->InvokeAction(EF_ACTION_MAKEPATHSL);
}

void CMainFrame::OnEntityTargetpath() 
{
	GetMapManager()->GetDocPtr()->InvokeAction(EF_ACTION_MAKETARGETS);
}

void CMainFrame::OnEntityDeletepath() 
{
	GetMapManager()->GetDocPtr()->InvokeAction(EF_ACTION_DELETEPATHS);
}

void CMainFrame::OnUpdateEntityPath(CCmdUI* pCmdUI) 
{
	if(m_bMapOpen)
	{
		if(GetMapManager()->GetDocPtr()->GetSelectedEntityCount() > 1)
			pCmdUI->Enable(true);
		else
			pCmdUI->Enable(false);
	}
	else
		pCmdUI->Enable(false);
}

void CMainFrame::OnUpdateEntityPathloop(CCmdUI* pCmdUI) 
{
	if(m_bMapOpen)
	{
		if(GetMapManager()->GetDocPtr()->GetSelectedEntityCount() > 1)
			pCmdUI->Enable(true);
		else
			pCmdUI->Enable(false);
	}
	else
		pCmdUI->Enable(false);
}

void CMainFrame::OnUpdateEntityTargetpath(CCmdUI* pCmdUI) 
{
	if(m_bMapOpen)
	{
		if(GetMapManager()->GetDocPtr()->GetSelectedEntityCount() > 1)
			pCmdUI->Enable(true);
		else
			pCmdUI->Enable(false);
	}
	else
		pCmdUI->Enable(false);
}

void CMainFrame::OnUpdateEntityDeletepath(CCmdUI* pCmdUI) 
{
	if(m_bMapOpen)
	{
		if(GetMapManager()->GetDocPtr()->GetSelectedEntityCount() > 1)
			pCmdUI->Enable(true);
		else
			pCmdUI->Enable(false);
	}
	else
		pCmdUI->Enable(false);
}

void CMainFrame::OnViewCenter() 
{
	GetMapManager()->GetDocPtr()->InvokeAction(EF_ACTION_CENTER);
}

void CMainFrame::OnUpdateViewCenter(CCmdUI* pCmdUI) 
{
	if(m_bMapOpen)
	{
		if((GetMapManager()->GetDocPtr()->GetSelectedObjectCount() > 0) || (GetModeManager()->GetMode() == MODE_CAMERA))
			pCmdUI->Enable(true);
		else
			pCmdUI->Enable(false);
	}
	else
		pCmdUI->Enable(false);
}

void CMainFrame::OnClearMemory() 
{
	GetTexAPI()->FreeAllLoadedTextures();
}

void CMainFrame::OnUpdateClearMemory(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_bMapOpen);
}

void CMainFrame::OnWorldspawn() 
{
	CEntityDialog dlgTemp;

	CEntity* pEntity = GetMapManager()->GetDocPtr()->GetWorldspawn();
	dlgTemp.SetEntity(pEntity, true);
	
	if(dlgTemp.DoModal() == IDOK)
	{
		pEntity = dlgTemp.GetEntity();
		GetMapManager()->GetDocPtr()->SetWorldspawn(pEntity);
	}
}

void CMainFrame::OnUpdateMapProperties(CCmdUI* pCmdUI) 
{
}

void CMainFrame::OnUpdateBuildModel(CCmdUI* pCmdUI) 
{
	if(m_bMapOpen)
		pCmdUI->Enable(GetMapManager()->GetDocPtr()->GetSelectedObjectCount() > 0);
	else
		pCmdUI->Enable(false);
}

void CMainFrame::OnUpdateWorldspawn(CCmdUI* pCmdUI) 
{
}

void CMainFrame::OnScriptInformation() 
{
	CScriptDialog dlgTemp;
	dlgTemp.DoModal();
}

void CMainFrame::OnQuickHelp() 
{
	ShowDialog(DLG_QUICKHELP, true);
}

void CMainFrame::OnClipAction() 
{
	GetMapManager()->GetDocPtr()->InvokeAction(EF_ACTION_CLIP);
}

void CMainFrame::On3dviewTransparentwater() 
{
	GetMapManager()->GetDocPtr()->ToggleRenderFlag(RF_NOTRANSPARENCY);
	GetMapManager()->GetDocPtr()->UpdateWindows();
}

void CMainFrame::OnUpdate3dviewTransparentwater(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(!(GetMapManager()->GetDocPtr()->GetGlobalRenderFlags() & RF_NOTRANSPARENCY));
}

void CMainFrame::OnViewSmartnames() 
{
	SetRegBool(B_SMARTNAME, !GetRegBool(B_SMARTNAME));
	SaveTreadSettings();
	GetMapManager()->GetCurrentMap()->GetDocument()->UpdateWindows();
	CObjectView* pView = (CObjectView*)GetView(OBJECT_VIEW);

	if(pView->EntityListLoaded())
	{
		pView->ResetContent();
		pView->LoadEntityList();
	}
}

void CMainFrame::OnUpdateViewSmartnames(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(GetRegBool(B_SMARTNAME));
}

void CMainFrame::OnFileImport() 
{
	CString sPath;
	CFileDialog dlgTemp(true, "map", NULL,
			OFN_NONETWORKBUTTON, "Map Files (*.map)|*.map|All Files (*.*)|*.*||", this);
	if(dlgTemp.DoModal() != IDOK)
		return;

	sPath = dlgTemp.GetPathName();

	CTread3DApp* pApp = (CTread3DApp*)AfxGetApp();

	POSITION curTemplatePos = pApp->GetFirstDocTemplatePosition();
	CString sTemp;

	while(curTemplatePos != NULL)
	{
		CDocTemplate* curTemplate = pApp->GetNextDocTemplate(curTemplatePos);

		curTemplate->GetDocString(sTemp, CDocTemplate::docName);
		if(sTemp == "Map")
		{
			curTemplate->OpenDocumentFile(sPath);
			return;
		}
	}

	Error("Could not open map file. Reason: Document could not be created.");
}

void CMainFrame::OnFileExport() 
{
	CString sPath;
	CString sDefaultName = "";
	sDefaultName = GetMapManager()->GetDocPtr()->GetTitle();
	int nIndex = sDefaultName.Find('.');
	if(nIndex != -1)
		sDefaultName = sDefaultName.Left(nIndex);

	CFileDialog dlgTemp(false, "map", sDefaultName,
			OFN_NONETWORKBUTTON, "Map Files (*.map)|*.map|All Files (*.*)|*.*||", this);
	if(dlgTemp.DoModal() != IDOK)
		return;

	sPath = dlgTemp.GetPathName();

	GetMapManager()->GetDocPtr()->ExportMap(sPath);
}

void CMainFrame::OnUpdateFileExport(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_bMapOpen);
}

void CMainFrame::On3dDrawentities() 
{
	CMapView* pView = GetMapManager()->GetDocPtr()->GetViewByType(VIEW_3D);
	if(pView->GetDrawFilters() & OBJECT_CLASS_ENTITY)
		pView->RemoveDrawFilters(OBJECT_CLASS_ENTITY);
	else
		pView->AddDrawFilters(OBJECT_CLASS_ENTITY);

	GetMapManager()->GetDocPtr()->UpdateWindows(VIEW_3D);
}

void CMainFrame::OnUpdate3dDrawentities(CCmdUI* pCmdUI) 
{
	CMapView* pView = GetMapManager()->GetDocPtr()->GetViewByType(VIEW_3D);
	pCmdUI->SetCheck(!(pView->GetDrawFilters() & OBJECT_CLASS_ENTITY));
}

void CMainFrame::OnUpdatePluginMenu(CCmdUI* pCmdUI) 
{
	SetCapture();
	BeginWaitCursor();

	Debug("Building plugin menu.\n");
	BuildPluginMenu();

	GetPluginAPI()->UpdateCommandUI(ID_PLUGIN_MIN, pCmdUI);

	EndWaitCursor();
	ReleaseCapture();
}

void CMainFrame::BuildPluginMenu(bool bDelete)
{
	CMenu* pMainMenu = NULL;
	CMenu* pPluginMenu = NULL;
	CPopupMenu* pParentMenu = NULL;
	CString sTemp;
	CString sBuffer;
	CString sHandle;
	int nCount;
	bool bFirst = true;
	int nCommand = ID_PLUGIN_MIN;

	//attempt to get the current menu
	pMainMenu = GetMenu();
	if(pMainMenu == NULL)
	{
		Debug("Could not access the map menu.\n");
		return;
	}

	//get the plugin menu by its submenu position
	pPluginMenu = pMainMenu->GetSubMenu(7);
	if(pPluginMenu == NULL)
	{
		Debug("Could not access the plugin menu.\n");
		return;
	}

	//delete the items in the menu
	while(pPluginMenu->DeleteMenu(0, MF_BYPOSITION) != 0);

	if(bDelete)
		m_MenuList.DestroyList();

	//Get the plugin menu item list
	CLinkedList<TPMENUITEM>* pList = GetPluginAPI()->GetPluginMenuList();
	TPMENUITEM* pMenuItem;

	//go to the first position
	pList->First();
	for(pMenuItem = pList->GetCurItem(); pMenuItem != NULL; pMenuItem = pList->GetNextItem())
	{
		bFirst = true;
		sTemp = pMenuItem->sTitle;
		
		nCount = sTemp.Find('\n');
		if(nCount == -1)
		{
			pParentMenu = (CPopupMenu*)pPluginMenu;
		}
		else
		{
			while(nCount != -1)
			{
				//get the first word up to the carriage return
				sBuffer = sTemp.Left(nCount);
				//delete the word from the string
				sTemp.Delete(0, nCount + 1);
				//build a string handle to identify this popup menu
				sHandle = pMenuItem->sTitle.Left(pMenuItem->sTitle.GetLength() - sTemp.GetLength() - 1);
				
				//check if there is already an existing menu
				CPopupMenu* pPopup = GetPopupMenu(sHandle);
	
				//if not then create a new one
				if(pPopup == NULL)
				{
					pPopup = new CPopupMenu();
					pPopup->CreatePopupMenu();
					pPopup->m_sHandle = sHandle;
					m_MenuList.AddItem(pPopup);

					if(bFirst)
						pPluginMenu->AppendMenu(MF_POPUP, (UINT)pPopup->GetSafeHmenu(), sBuffer);
					else
						pParentMenu->AppendMenu(MF_POPUP, (UINT)pPopup->GetSafeHmenu(), sBuffer);
				}
				
				bFirst = false;

				pParentMenu = pPopup;
				nCount = sTemp.Find('\n');
			}
		}

		//add the new menu item
		pParentMenu->AppendMenu(MF_STRING, nCommand, sTemp);
		pMenuItem->nID = nCommand;		//store the command id used for this item
		nCommand++;		//increment to the next available command id
		if(nCommand > ID_PLUGIN_MAX)
		{
			Error("Cannot add menu item for plugin (" + pMenuItem->sPluginName + ").\n Out of resources.");
			return;
		}
	}
}

CPopupMenu* CMainFrame::GetPopupMenu(CString sName)
{
	m_MenuList.First();
	CPopupMenu* pItem;

	for(pItem = m_MenuList.GetCurItem(); pItem != NULL; pItem = m_MenuList.GetNextItem() )
	{
		if(sName == pItem->m_sHandle)
			return pItem;
	}

	return NULL;
}

bool CMainFrame::UpdateTreadMenu()
{
	if(!m_bMapOpen)
		return false;

	CMenu orgMenu;
	orgMenu.LoadMenu(IDR_TREAD3TYPE);
	CMenu* mainMenu;
	CString sTemp;
	int nLast = 0;

	mainMenu = GetMenu();
	if(mainMenu == NULL)
		return false;

	for(int n = 0; n < m_nAccelCount; n++)
	{
		nLast = orgMenu.GetMenuString(m_pAccelTable[n].cmd, sTemp, MF_BYCOMMAND);
		if(nLast <= 0)
			continue;

		sTemp += '\t' + VtoS(m_pAccelTable[n].key);
		nLast = mainMenu->ModifyMenu(m_pAccelTable[n].cmd, MF_STRING | MF_BYCOMMAND, m_pAccelTable[n].cmd, sTemp);
		if(!nLast)
			Error("Could not load menu comman.\n");
	}

	return true;
}

CString CMainFrame::VtoS(int nKey)
{
	CString sTemp;

	if((nKey >= 65) && (nKey <= 90))
	{
		sTemp += (char)nKey;
		return sTemp;
	}
	else if((nKey >= 48) && (nKey <= 57))
	{
		sTemp += (char)nKey;
		return sTemp;
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
		break;
	}

	 return sTemp;
}

void CMainFrame::OnDropAnchor() 
{
	GetMapManager()->GetDocPtr()->InvokeAction(EF_ACTION_DROPANCHOR);
}

void CMainFrame::OnGridSnap() 
{
	CTread3DDoc* pDoc = GetMapManager()->GetDocPtr();
	pDoc->SetGridSnap(!pDoc->GetGridSnap());
}

void CMainFrame::OnUpdateGridSnap(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(GetMapManager()->GetDocPtr()->GetGridSnap());
}

void CMainFrame::OnViewBrushHandles() 
{
	CTread3DDoc* pDoc = GetMapManager()->GetDocPtr();

	pDoc->SetBrushHandles(!pDoc->GetBrushHandles());
	pDoc->BuildHotSpots();
	pDoc->UpdateWindows(FLAT_VIEWS);
}

void CMainFrame::OnUpdateViewBrushHandles(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(GetMapManager()->GetDocPtr()->GetBrushHandles());	
}

void CMainFrame::OnViewAnchors() 
{
	CTread3DDoc* pDoc = GetMapManager()->GetDocPtr();

	if(pDoc->GetDrawFilters() & OBJECT_CLASS_ANCHOR)
		pDoc->RemoveDrawFilters(OBJECT_CLASS_ANCHOR);
	else
		pDoc->AddDrawFilters(OBJECT_CLASS_ANCHOR);

	pDoc->UpdateWindows(FLAT_VIEWS);	
}

void CMainFrame::OnUpdateViewAnchors(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(!(GetMapManager()->GetDocPtr()->GetDrawFilters() & OBJECT_CLASS_ANCHOR));	
}

void CMainFrame::OnMapDrawpaths() 
{
	CTread3DDoc* pDoc = GetMapManager()->GetDocPtr();
	pDoc->SetDrawTargets(!pDoc->GetDrawTargets());
	pDoc->UpdateWindows();
}

void CMainFrame::OnUpdateMapDrawpaths(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(GetMapManager()->GetDocPtr()->GetDrawTargets());
}

void CMainFrame::OnViewUpdateallviews() 
{
	GetMapManager()->GetDocPtr()->SetUpdateAllViews(!GetMapManager()->GetDocPtr()->GetUpdateAllViews());
}

void CMainFrame::OnUpdateViewUpdateallviews(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable();
	pCmdUI->SetCheck(GetMapManager()->GetDocPtr()->GetUpdateAllViews());
}

void CMainFrame::OnGridSmaller() 
{
	int n = m_pGridBox->GetCurSel();
	if(n == -1)
		return;
	n--;
	m_pGridBox->SetCurSel(n);
	GetMapManager()->GetCurrentMap()->SetViewsGrid(pGridValues[n]);
}

void CMainFrame::OnGridLarger() 
{
	int n = m_pGridBox->GetCurSel();
	if(n == -1)
		return;
	n++;
	m_pGridBox->SetCurSel(n);
	GetMapManager()->GetCurrentMap()->SetViewsGrid(pGridValues[n]);
}

void CMainFrame::OnUpdateGridLarger(CCmdUI* pCmdUI) 
{
	if(m_pGridBox->GetCurSel() < (GRID_COMBO_NUM - 1))
		pCmdUI->Enable(true);
	else
		pCmdUI->Enable(false);
	
}

void CMainFrame::OnUpdateGridSmaller(CCmdUI* pCmdUI) 
{
	if(m_pGridBox->GetCurSel() > 0)
		pCmdUI->Enable(true);
	else
		pCmdUI->Enable(false);
}

void CMainFrame::OnAlignEdgeBottom() 
{
	GetMapManager()->GetDocPtr()->InvokeAction(EF_ACTION_ALIGNBOTTOM);
}

void CMainFrame::OnAlignEdgeLeft() 
{
	GetMapManager()->GetDocPtr()->InvokeAction(EF_ACTION_ALIGNLEFT);
}

void CMainFrame::OnAlignEdgeRight() 
{
	GetMapManager()->GetDocPtr()->InvokeAction(EF_ACTION_ALIGNRIGHT);
}

void CMainFrame::OnAlignEdgeTop() 
{
	GetMapManager()->GetDocPtr()->InvokeAction(EF_ACTION_ALIGNTOP);
}

void CMainFrame::OnAlignSizeBoth() 
{
	GetMapManager()->GetDocPtr()->InvokeAction(EF_ACTION_SIZEBOTH);
}

void CMainFrame::OnAlignSizeHorizontal() 
{
	GetMapManager()->GetDocPtr()->InvokeAction(EF_ACTION_SIZEHORZ);
}

void CMainFrame::OnAlignSizeVertical() 
{
	GetMapManager()->GetDocPtr()->InvokeAction(EF_ACTION_SIZEVERT);
}

void CMainFrame::OnAlignSpaceHorizontal() 
{
	GetMapManager()->GetDocPtr()->InvokeAction(EF_ACTION_SPACEHORZ);
}

void CMainFrame::OnUpdateAlignSpaceHorizontal(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_bMapOpen && GetMapManager()->GetDocPtr()->GetSelectedObjectList()->IsEmpty() == false);
}

void CMainFrame::OnAlignSpaceVertical() 
{
	GetMapManager()->GetDocPtr()->InvokeAction(EF_ACTION_SPACEVERT);
}

void CMainFrame::OnUpdateAlignSizeHorizontal(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_bMapOpen && GetMapManager()->GetDocPtr()->GetSelectedObjectList()->IsEmpty() == false);
}

void CMainFrame::OnUpdateAlignSizeBoth(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_bMapOpen && GetMapManager()->GetDocPtr()->GetSelectedObjectList()->IsEmpty() == false);
}

void CMainFrame::OnUpdateAlignSizeVertical(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_bMapOpen && GetMapManager()->GetDocPtr()->GetSelectedObjectList()->IsEmpty() == false);
}

void CMainFrame::OnUpdateAlignSpaceVertical(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_bMapOpen && GetMapManager()->GetDocPtr()->GetSelectedObjectList()->IsEmpty() == false);
}

void CMainFrame::OnUpdateAlignEdgeBottom(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_bMapOpen && GetMapManager()->GetDocPtr()->GetSelectedObjectList()->IsEmpty() == false);
}

void CMainFrame::OnUpdateAlignEdgeLeft(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_bMapOpen && GetMapManager()->GetDocPtr()->GetSelectedObjectList()->IsEmpty() == false);
}

void CMainFrame::OnUpdateAlignEdgeRight(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_bMapOpen && GetMapManager()->GetDocPtr()->GetSelectedObjectList()->IsEmpty() == false);
}

void CMainFrame::OnUpdateAlignEdgeTop(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_bMapOpen && GetMapManager()->GetDocPtr()->GetSelectedObjectList()->IsEmpty() == false);
}

void CMainFrame::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if(nIDEvent == CCC_TIMER_ID)
	{
		if(GetMapManager()->ObjectsSelected() == true)
			GetColorCycleController()->Cycle();
		else
			GetColorCycleController()->StartAnimating(false);
	}
}

void CMainFrame::OnViewAnimatedSelection() 
{
	if(GetRegBool(B_ANIMATECOLORS))
	{
		GetColorCycleController()->StartAnimating(false);
		GetMapManager()->GetDocPtr()->UpdateWindows(FLAT_VIEWS);
		SetRegBool(B_ANIMATECOLORS, false);
	}
	else
	{
		SetRegBool(B_ANIMATECOLORS, true);
		GetMapManager()->GetDocPtr()->CycleColorIfSelected();
	}
}

void CMainFrame::OnUpdateViewAnimatedSelection(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_bMapOpen);
	pCmdUI->SetCheck(GetRegBool(B_ANIMATECOLORS));
}
