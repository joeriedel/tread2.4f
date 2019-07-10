// DEFAULTACTIONRESPONDER.H
// Copyright © 2000 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#ifndef DEFAULTACTIONRESPONDER_H
#define DEFAULTACTIONRESPONDER_H

#include "EventManager.h"
#include "MapObject.h"
#include "TExport.h"

// $sb "defaultactionresponder.h block1"

class _T_EXPORT CDefaultActionResponder
{
private:

	static float GetObjectSpace(CBaseTransformableObject* pTA, CBaseTransformableObject* pTB, int nAxis);
	static void  SpaceObjects(CBaseTransformableObject* pTA, CBaseTransformableObject* pTB, int nAxis, float fSpace);

public:

	static int OnAction(int nAction, void* pParam, CModeChangeEventManager* pEventManager, CMode* pModeManager, CTread3DDoc* pDocument);
};

// $eb

#endif