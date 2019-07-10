// POINTERMODEWINRESPONDER.H
// Copyright © 1999 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#ifndef POINTERMODEWINRESPONDER_H
#define POINTERMODEWINRESPONDER_H

#include "EventManager.h"

class CPointerModeWinResponder : public CBaseWindowEventResponder
{
	int m_nState;
	int m_fX, m_fY;
	int m_nX, m_nY;
	bool m_bMoved;
	bool m_bCanSwitch;
	bool m_bSnapped;
	vec3_t vMoveTotal;

public:

	CPointerModeWinResponder();

	virtual void OnMouseMove(CBaseWindowEventResponder* pParent, CWindowEventManager* pEventManager, CMapView* pView, CTread3DDoc* pDocument, int nMouseX, int nMouseY, int nButtons);
	virtual void OnMouseDown(CBaseWindowEventResponder* pParent, CWindowEventManager* pEventManager, CMapView* pView, CTread3DDoc* pDocument, int nMouseX, int nMouseY, int nButtons);
	virtual void OnMouseUp(CBaseWindowEventResponder* pParent, CWindowEventManager* pEventManager, CMapView* pView, CTread3DDoc* pDocument, int nMouseX, int nMouseY, int nButtons);
	virtual void OnKeyDown(CBaseWindowEventResponder* pParent, CWindowEventManager* pEventManager, CMapView* pView, CTread3DDoc* pDocument, int nKeyCode);

};

#endif
