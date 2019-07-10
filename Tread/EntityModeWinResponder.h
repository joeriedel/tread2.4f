// ENTITYMODEWINRESPONDER.H
// Copyright © 1999 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#ifndef ENTITYMODEWINRESPONDER_H
#define ENTITYMODEWINRESPONDER_H

#include "EventManager.h"

class CEntityModeWinResponder : public CBaseWindowEventResponder
{
	int m_nState;
	int m_fX, m_fY;

public:

	CEntityModeWinResponder();

	virtual void OnMouseDown(CBaseWindowEventResponder* pParent, CWindowEventManager* pEventManager, CMapView* pView, CTread3DDoc* pDocument, int nMouseX, int nMouseY, int nButtons);
};

#endif