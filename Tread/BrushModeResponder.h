// BRUSHMODERESPONDER.H
// Copyright © 1999 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#ifndef BRUSHMODERESPONDER_H
#define BRUSHMODERESPONDER_H

#include "EventManager.h"

class CBrushModeResponder : public CBaseModeChangeResponder
{
public :

	CBrushModeResponder();

	virtual void OnModeChange(CModeChangeEventManager* pEventManager, CMode* pModeManager, CTread3DDoc* pDocument, int nOldMode, int nOldSubmode, int nNewMode, int nNewSubmode);
};

#endif