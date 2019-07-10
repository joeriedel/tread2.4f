// EVENTMANAGER.H
// Copyright © 1999 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#ifndef EVENTMANAGER_H
#define EVENTMANAGER_H

#include "LinkedList.h"
#include "TExport.h"

// $sb "eventmanager.h block1"
/**********************************************************************************
** Treads Event System has been heavily redone since version 1.x. The new system **
** operates on a per object basis. There are two distinctions, and two sets of   **
** events that are trapped and handled. The first set are mode change events.    **
** Mode change events must be handled to alter various viewing flags, and build  **
** hotspots based on the current mode. For example, when switching from pointer  **
** mode, to vertex mode, hotspots should be generated for the vertices of the    **
** selected objects, not the global selection area. This would be done by        **
** attaching a mode change responder to the vertex mode, and then doing the      **
** appropriate procedures there.                                                 **
**                                                                               **
** The other set of tracked events are window events. Things such as the mouse   **
** moving in a view, or clicking. The window event manager ties in with the      **
** hotspot event manager. Hotspots are setup via the mode event responders. The  **
** window event manager passes events to even responders based on the current    **
** mode. These events can be usurped by the hotspot manager, if the event is     **
** occuring over a hotspot.                                                      **
**                                                                               **
**********************************************************************************/

class CWindowEventManager;
class CMapView;
class CMapFrame;
class CTread3DDoc;

#define ALL_MODES	-1

#ifdef _WIN32
#define EF_LBUTTON		MK_LBUTTON
#define EF_RBUTTON		MK_RBUTTON
#define EF_SHIFT		MK_SHIFT
#define EF_CONTROL		MK_CONTROL
#define EF_ALT			MK_ALT
#else
#define EF_LBUTTON		0x00000001
#define EF_RBUTTON		0x00000002
#define EF_SHIFT		0x00000004
#define EF_CONTROL		0x00000008
#define EF_ALT			0x00000010
#endif

// Define events that can happen via menus (i.e. Carving)
#define EF_ACTION_CARVE			0x00000001
#define EF_ACTION_HOLLOW		0x00000002
#define EF_ACTION_GROUP			0x00000003
#define EF_ACTION_UNGROUP		0x00000004
#define EF_ACTION_FLIPVERT		0x00000005
#define EF_ACTION_FLIPHORZ		0x00000006
#define EF_ACTION_CLIP			0x00000007
#define EF_ACTION_DELETE		0x00000008
#define EF_ACTION_MAKETARGETS	0x00000009
#define EF_ACTION_MAKEPATHS		0x0000000A
#define EF_ACTION_MAKEPATHSL	0x0000000B
#define EF_ACTION_LINK			0x0000000C
#define EF_ACTION_UNLINK		0x0000000D
#define EF_ACTION_UNDO			0x0000000E
#define EF_ACTION_CUT			0x0000000F
#define EF_ACTION_COPY			0x00000010
#define EF_ACTION_PASTE			0x00000011
#define EF_ACTION_CENTER		0x00000012
#define EF_ACTION_ROT90CW		0x00000013
#define EF_ACTION_ROT90CCW		0x00000014
#define EF_ACTION_ROT180		0x00000015
#define EF_ACTION_REDO			0x00000016	// Hehe, last minute idea.
#define EF_ACTION_UNDOSTRING	0x00000017  // Get Undo String.
#define EF_ACTION_REDOSTRING	0x00000018	// Get Redo String.
#define EF_ACTION_PAINT			0x00000019
#define EF_ACTION_DELETEPATHS	0x0000001A
#define EF_ACTION_DROPANCHOR	0x0000001B
#define EF_ACTION_ALIGNLEFT		0x0000001C
#define EF_ACTION_ALIGNRIGHT	0x0000001D
#define EF_ACTION_ALIGNTOP		0x0000001E
#define EF_ACTION_ALIGNBOTTOM	0x0000001F
#define EF_ACTION_ALIGNHORZCEN	0x00000020
#define EF_ACTION_ALIGNVERTCEN	0x00000021
#define EF_ACTION_SPACEHORZ		0x00000022
#define EF_ACTION_SPACEVERT		0x00000023
#define EF_ACTION_CLOSEHORZ		0x00000024
#define EF_ACTION_CLOSEVERT		0x00000025
#define EF_ACTION_SIZEHORZ		0x00000026
#define EF_ACTION_SIZEVERT		0x00000027
#define EF_ACTION_SIZEBOTH		0x00000028

class _T_EXPORT CBaseWindowEventResponder : public CLLObject
{
public:

	CBaseWindowEventResponder() {}
	virtual ~CBaseWindowEventResponder() {}

	int m_nMode;
	int m_nSubmode;

	virtual void OnMouseMove(CBaseWindowEventResponder* pParent, CWindowEventManager* pEventManager, CMapView* pView, CTread3DDoc* pDocument, int nMouseX, int nMouseY, int nButtons) {}
	virtual void OnMouseDown(CBaseWindowEventResponder* pParent, CWindowEventManager* pEventManager, CMapView* pView, CTread3DDoc* pDocument, int nMouseX, int nMouseY, int nButtons) {}
	virtual void OnMouseUp(CBaseWindowEventResponder* pParent, CWindowEventManager* pEventManager, CMapView* pView, CTread3DDoc* pDocument, int nMouseX, int nMouseY, int nButtons) {}
	virtual void OnKeyDown(CBaseWindowEventResponder* pParent, CWindowEventManager* pEventManager, CMapView* pView, CTread3DDoc* pDocument, int nKeyCode) {}
};

// NOTE: The event manager will delete all the trapper objects
// that get added. There is no need to delete them yourself.
class _T_EXPORT CWindowEventManager
{
private:

	bool m_bAutoScroll;
	bool m_bTimerActive;
	int m_nMouseX, m_nMouseY, m_nButtons;
	CBaseWindowEventResponder* m_pAutoScrollResponder;
	CBaseWindowEventResponder* m_pGlobalResponder;
	CLinkedList<CBaseWindowEventResponder>* m_pResponderList;

public:

	CWindowEventManager();
	~CWindowEventManager();


	void EnableAutoScroll(CBaseWindowEventResponder* pResp, bool bAuto=true);

	void ClearResponderList() { m_pResponderList->DestroyList(); }
	void SetGlobalResponder(CBaseWindowEventResponder* pResponder);
	void AddEventResponder(CBaseWindowEventResponder* pResponder);

	void OnTimer(CMapView* pView, CTread3DDoc* pDocument, int nCode);
	void OnMouseMove(CMapView* pView, CTread3DDoc* pDocument, int nMouseX, int nMouseY, int nButtons);
	void OnMouseDown(CMapView* pView, CTread3DDoc* pDocument, int nMouseX, int nMouseY, int nButtons);
	void OnMouseUp(CMapView* pView, CTread3DDoc* pDocument, int nMouseX, int nMouseY, int nButtons);
	void OnKeyDown(CMapView* pView, CTread3DDoc* pDocument, int nKeyCode);
};

// Action responder flags.
#define ARF_DEFAULT			0x00000000
#define ARF_HANDLED			0x00000001
#define ARF_IGNORE			0x00000002
#define ARF_NOTHANDLED		0x00000003

// Responds to a specific mode change.
class CMode;
class CModeChangeEventManager;
class _T_EXPORT CBaseModeChangeResponder : public CLLObject
{
public:

	CBaseModeChangeResponder() {}
	virtual ~CBaseModeChangeResponder() {}

	int m_nMode;
	int m_nSubmode;

	virtual int OnAction(int nAction, void* pParam, CModeChangeEventManager* pEventManager, CMode* pModeManager, CTread3DDoc* pDocument) { return ARF_DEFAULT; }
	virtual void OnModeChange(CModeChangeEventManager* pEventManager, CMode* pModeManager, CTread3DDoc* pDocument, int nOldMode, int nOldSubmode, int nNewMode, int nNewSubmode) {}
};

// Responds to mode changes.
// All responders will be deleted with the manager.
class _T_EXPORT CModeChangeEventManager
{
private:
	CLinkedList<CBaseModeChangeResponder>* m_pResponderList;

public:

	CModeChangeEventManager();
	~CModeChangeEventManager();
 
	void InvokeAction(int nAction, void* pParam, CTread3DDoc* pDocument);
	void AddEventResponder(CBaseModeChangeResponder* pReponder);
	void OnModeChange(CMode* pModeManager, CTread3DDoc* pDocument, int nOldMode, int nOldSubmode, int nNewMode, int nNewSubmode);
};
// $eb
#endif