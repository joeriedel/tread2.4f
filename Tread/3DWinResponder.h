// 3DWINRESPONDER.H
// Copyright © 1999 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#ifndef _3DWINRESPONDER_H
#define _3DWINRESPONDER_H

#include "EventManager.h"

class C3DWinResponder : public CBaseWindowEventResponder
{
private:

	int m_nX, m_nY;
	int m_nState;

public:

	C3DWinResponder();

	virtual void OnMouseDown(CBaseWindowEventResponder* pParent, CWindowEventManager* pEventManager, CMapView* pView, CTread3DDoc* pDocument, int nMouseX, int nMouseY, int nButtons);
	virtual void OnMouseMove(CBaseWindowEventResponder* pParent, CWindowEventManager* pEventManager, CMapView* pView, CTread3DDoc* pDocument, int nMouseX, int nMouseY, int nButtons);
	virtual void OnMouseUp(CBaseWindowEventResponder* pParent, CWindowEventManager* pEventManager, CMapView* pView, CTread3DDoc* pDocument, int nMouseX, int nMouseY, int nButtons);

	//virtual void OnKeyDown(CBaseWindowEventResponder* pParent, CWindowEventManager* pEventManager, CMapView* pView, CTread3DDoc* pDocument, int nKeyCode);
};

#endif