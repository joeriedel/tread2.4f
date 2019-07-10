// CLIPTOOLMODEWINRESPONDER.H
// Copyright © 1999 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#ifndef CLIPTOOLMODEWINRESPONDER_H
#define CLIPTOOLMODEWINRESPONDER_H

#include "EventManager.h"

class CClipToolModeWinResponder : public CBaseWindowEventResponder
{
	int m_nState;
	int m_fX, m_fY;
	bool m_bMoved;
	bool m_bCanSwitch;

public:

	CClipToolModeWinResponder();

	virtual void OnMouseMove(CBaseWindowEventResponder* pParent, CWindowEventManager* pEventManager, CMapView* pView, CTread3DDoc* pDocument, int nMouseX, int nMouseY, int nButtons);
	virtual void OnMouseDown(CBaseWindowEventResponder* pParent, CWindowEventManager* pEventManager, CMapView* pView, CTread3DDoc* pDocument, int nMouseX, int nMouseY, int nButtons);
	virtual void OnMouseUp(CBaseWindowEventResponder* pParent, CWindowEventManager* pEventManager, CMapView* pView, CTread3DDoc* pDocument, int nMouseX, int nMouseY, int nButtons);
	virtual void OnKeyDown(CBaseWindowEventResponder* pParent, CWindowEventManager* pEventManager, CMapView* pView, CTread3DDoc* pDocument, int nKeyCode);
};

#endif