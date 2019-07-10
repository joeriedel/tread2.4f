// ENTITYMODERESPONDER.H
// Copyright © 1999 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#ifndef ENTITYMODERESPONDER_H
#define ENTITYMODERESPONDER_H

#include "EventManager.h"

class CEntityModeResponder : public CBaseModeChangeResponder
{
public :

	CEntityModeResponder();

	virtual void OnModeChange(CModeChangeEventManager* pEventManager, CMode* pModeManager, CTread3DDoc* pDocument, int nOldMode, int nOldSubmode, int nNewMode, int nNewSubmode);
};

#endif