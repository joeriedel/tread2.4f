// ZOOMMODEWINRESPONDER.H
// Copyright © 1999 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#ifndef ZOOMMODEWINRESPONDER_H
#define ZOOMMODEWINRESPONDER_H

#include "EventManager.h"

class CZoomModeWinResponder : public CBaseWindowEventResponder
{

public:

	CZoomModeWinResponder();
	virtual void OnMouseDown(CBaseWindowEventResponder* pParent, CWindowEventManager* pEventManager, CMapView* pView, CTread3DDoc* pDocument, int nMouseX, int nMouseY, int nButtons);
};

#endif