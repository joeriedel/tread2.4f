// PAINTMODERESPONDER.H
// Copyright © 1999 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#ifndef PAINTMODERESPONDER_H
#define PAINTMODERESPONDER_H

#include "EventManager.h"

class CPaintModeResponder : public CBaseModeChangeResponder
{
public :

	CPaintModeResponder();

	virtual void OnModeChange(CModeChangeEventManager* pEventManager, CMode* pModeManager, CTread3DDoc* pDocument, int nOldMode, int nOldSubmode, int nNewMode, int nNewSubmode);
};

#endif