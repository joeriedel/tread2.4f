// ZOOMMODERESPONDER.H
// Copyright © 1999 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#ifndef ZOOMMODERESPONDER_H
#define ZOOOMMODERESPONDER_H

#include "EventManager.h"

class CZoomModeResponder : public CBaseModeChangeResponder
{
public :

	CZoomModeResponder();

	virtual void OnModeChange(CModeChangeEventManager* pEventManager, CMode* pModeManager, CTread3DDoc* pDocument, int nOldMode, int nOldSubmode, int nNewMode, int nNewSubmode);
};

#endif