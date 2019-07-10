// COLORCYLCE.CPP
// Copyright © 2000 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#include "stdafx.h"
#include "ColorCycleController.h"
#include "Tread3D.h"
#include "TreadSys.h"
#include "Mainfrm.h"

#define CCC_TIMER_INTERVAL	50

CColorCycleController m_CCC;
CColorCycleController* GetColorCycleController() { return &m_CCC; }

void CColorCycleController::Initialize()
{
	// Copy all the color table colors.
	int i;
	for(i = 0; i < CL_TABLE_LENGTH; i++)
	{
		Colors[i].nID = i;
		Colors[i].nOriginal = GetColorTable()->GetColor(i);
		Colors[i].nAnimated = Colors[i].nOriginal;
		Colors[i].nInterval = 0;
		Colors[i].nTime = 0;
		Colors[i].bUp = true;
	}

	m_bAnimating = false;
	m_CycleList.SetAutoDelete(false);

	// Setup default animated colors:
	SetColorInterval(CL_LINE_SEL, 10);
}

void CColorCycleController::ResetColor(int nID)
{
	GetColorTable()->SetColor(nID, Colors[nID].nOriginal);
}

void CColorCycleController::ResetAnimated(int nID)
{
	GetColorTable()->SetColor(nID, Colors[nID].nAnimated);
}

void CColorCycleController::ResetAllColors()
{
	int i;
	for(i = 0; i < CL_TABLE_LENGTH; i++)
		ResetColor(i);
}

void CColorCycleController::KillTimer()
{
	GetMapManager()->GetMainFramePtr()->KillTimer(CCC_TIMER_ID);
}

void CColorCycleController::StartTimer()
{
	GetMapManager()->GetMainFramePtr()->SetTimer(CCC_TIMER_ID, CCC_TIMER_INTERVAL, NULL);
}

void CColorCycleController::StartAnimating(bool bStart)
{
	if(GetRegBool(B_ANIMATECOLORS) == false)
		return;

	if(bStart && !m_bAnimating)
	{
		m_bAnimating = true;
		StartTimer();
		//Console("Started animator\n");
	}
	else if(!bStart && m_bAnimating)
	{
		m_bAnimating = false;
		KillTimer();
		//Console("Stopped animator\n");
		ResetAllColors();
		GetMapManager()->RedrawAllMapsSelections(FLAT_VIEWS, true);
	}
}

void CColorCycleController::SetColorInterval(int nID, int nInterval)
{
	Colors[nID].nInterval = nInterval;
	Colors[nID].nTime = 0;
	Colors[nID].nAnimated = Colors[nID].nOriginal;

	if(nInterval == 0)
		m_CycleList.RemoveItem(&Colors[nID]);
	else if(m_CycleList.InList(&Colors[nID]) == false)
		m_CycleList.AddItem(&Colors[nID]);
}

void CColorCycleController::Cycle()
{
	CCCITEM* pColor;
	int nR, nG, nB;

	for(pColor = m_CycleList.ResetPos(); pColor != NULL; pColor = m_CycleList.GetNextItem())
	{
		if(pColor->bUp)
		{
			if(pColor->nTime == pColor->nInterval)
			{
				pColor->bUp = false;
				pColor->nTime--;
			}
			else
				pColor->nTime++;
		}
		else
		{
			if(pColor->nTime == 0)
			{
				pColor->bUp = true;
				pColor->nTime++;
			}
			else
				pColor->nTime--;
		}

		// Compute color fraction.
		nR = ((float)pColor->nTime/(float)pColor->nInterval*(255.0f-GetRValue(pColor->nOriginal)))+GetRValue(pColor->nOriginal);
		nG = ((float)pColor->nTime/(float)pColor->nInterval*(255.0f-GetGValue(pColor->nOriginal)))+GetGValue(pColor->nOriginal);
		nB = ((float)pColor->nTime/(float)pColor->nInterval*(255.0f-GetBValue(pColor->nOriginal)))+GetBValue(pColor->nOriginal);
		pColor->nAnimated = RGB(nR, nG, nB);
		GetColorTable()->SetColor(pColor->nID, pColor->nAnimated);
	}

	GetMapManager()->RedrawAllMapsSelections(FLAT_VIEWS, true);
}