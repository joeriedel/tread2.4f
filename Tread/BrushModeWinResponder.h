// BRUSHMODEWINRESPONDER.H
// Copyright © 1999 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#ifndef BRUSHMODEWINRESPONDER_H
#define BRUSHMODEWINRESPONDER_H

#include "EventManager.h"

class CBrushModeWinResponder : public CBaseWindowEventResponder
{
	bool m_bSizing;
	bool m_bIgnore;

public:

	CBrushModeWinResponder();

	virtual void OnMouseMove(CBaseWindowEventResponder* pParent, CWindowEventManager* pEventManager, CMapView* pView, CTread3DDoc* pDocument, int nMouseX, int nMouseY, int nButtons);
	virtual void OnMouseDown(CBaseWindowEventResponder* pParent, CWindowEventManager* pEventManager, CMapView* pView, CTread3DDoc* pDocument, int nMouseX, int nMouseY, int nButtons);
	virtual void OnMouseUp(CBaseWindowEventResponder* pParent, CWindowEventManager* pEventManager, CMapView* pView, CTread3DDoc* pDocument, int nMouseX, int nMouseY, int nButtons);
};

#endif
