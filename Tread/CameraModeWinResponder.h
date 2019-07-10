// CAMERAMODEWINRESPONDER.H
// Copyright © 1999 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#ifndef CAMERAMODEWINRESPONDER_H
#define CAMERAMODEWINRESPONDER_H

#include "EventManager.h"

class CCameraModeWinResponder : public CBaseWindowEventResponder
{

public:

	CCameraModeWinResponder();
	virtual void OnMouseDown(CBaseWindowEventResponder* pParent, CWindowEventManager* pEventManager, CMapView* pView, CTread3DDoc* pDocument, int nMouseX, int nMouseY, int nButtons);
};

#endif