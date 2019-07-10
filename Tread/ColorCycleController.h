// COLORCYCLE.H
// Copyright © 2000 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#ifndef COLORCYCLE_H
#define COLORCYCLE_H

#include "TExport.h"
#include "ColorTable.h"
#include "LinkedList.h"

// Cycles a particular color in the color table.
// These cycles are global, so for things that don't cycle, you must save/restore real
// state during drawing.

// $sb "colorcyclecontroller.h block1"

#define CCC_TIMER_ID		2343

struct _T_EXPORT CCCITEM
{
	CCCITEM* prev, *next;

	int nID;
	int nAnimated;
	int nOriginal;
	int nInterval;
	int nTime;
	bool bUp;
};

class _T_EXPORT CColorCycleController
{
private:

	bool m_bAnimating;
	CLinkedList<CCCITEM> m_CycleList;
	CCCITEM Colors[CL_TABLE_LENGTH];

public:

	void Initialize();

	void ResetColor(int nID);
	void ResetAllColors();

	void ResetAnimated(int nID);

	void StartTimer();
	void KillTimer();

	void StartAnimating(bool bStart=true);	
	void SetColorInterval(int nID, int nInterval);

	void Cycle();
};

// $eb

CColorCycleController* GetColorCycleController();

#endif