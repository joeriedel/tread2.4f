// VERTEXMODERESPONDER.H
// Copyright © 1999 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#ifndef VERTEXMODERESPONDER_H
#define VERTEXMODERESPONDER_H

#include "EventManager.h"

class CVertexModeResponder : public CBaseModeChangeResponder
{
public :

	CVertexModeResponder();

	virtual int OnAction(int nAction, void* pParam, CModeChangeEventManager* pEventManager, CMode* pModeManager, CTread3DDoc* pDocument);
	virtual void OnModeChange(CModeChangeEventManager* pEventManager, CMode* pModeManager, CTread3DDoc* pDocument, int nOldMode, int nOldSubmode, int nNewMode, int nNewSubmode);
};

#endif