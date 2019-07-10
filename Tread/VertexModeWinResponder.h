// VERTEXMODEWINRESPONDER.H
// Copyright © 1999 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#ifndef VERTEXMODEWINRESPONDER_H
#define VERTEXMODEWINRESPONDER_H

#include "EventManager.h"

class CVertexModeWinResponder : public CBaseWindowEventResponder
{
	int m_nState;
	int m_fX, m_fY;
	bool m_bMoved;
	bool m_bCanSwitch;
	bool m_bSnapped;
	vec3_t m_vMoveTotal;

public:

	CVertexModeWinResponder();

	virtual void OnMouseMove(CBaseWindowEventResponder* pParent, CWindowEventManager* pEventManager, CMapView* pView, CTread3DDoc* pDocument, int nMouseX, int nMouseY, int nButtons);
	virtual void OnMouseDown(CBaseWindowEventResponder* pParent, CWindowEventManager* pEventManager, CMapView* pView, CTread3DDoc* pDocument, int nMouseX, int nMouseY, int nButtons);
	virtual void OnMouseUp(CBaseWindowEventResponder* pParent, CWindowEventManager* pEventManager, CMapView* pView, CTread3DDoc* pDocument, int nMouseX, int nMouseY, int nButtons);
	virtual void OnKeyDown(CBaseWindowEventResponder* pParent, CWindowEventManager* pEventManager, CMapView* pView, CTread3DDoc* pDocument, int nKeyCode);
};

#endif
