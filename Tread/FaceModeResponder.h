// FACEMODERESPONDER.H
// Copyright © 1999 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#ifndef FACEMODERESPONDER_H
#define FACEMODERESPONDER_H

#include "EventManager.h"

class CObjectGroup;

class CFaceModeResponder : public CBaseModeChangeResponder
{
public:

	CFaceModeResponder();

	virtual int OnAction(int nAction, void* pParam, CModeChangeEventManager* pEventManager, CMode* pModeManager, CTread3DDoc* pDocument);
	virtual void OnModeChange(CModeChangeEventManager* pEventManager, CMode* pModeManager, CTread3DDoc* pDocument, int nOldMode, int nOldSubmode, int nNewMode, int nNewSubmode);
	virtual void AddGroup(CObjectGroup* pGroup, CTread3DDoc* pDocument);
};

#endif