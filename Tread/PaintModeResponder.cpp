// PAINTMODERESPONDER.CPP
// Copyright © 1999 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#include "stdafx.h"
#include "PaintModeResponder.h"
#include "Tread3DDoc.h"
#include "TreadSys.h"
#include "TexAPI.h"

CPaintModeResponder::CPaintModeResponder()
{
	m_nMode = MODE_PAINT;
	m_nSubmode = ALL_MODES;
}

void CPaintModeResponder::OnModeChange(CModeChangeEventManager* pEventManager, CMode* pModeManager, CTread3DDoc* pDocument, int nOldMode, int nOldSubmode, int nNewMode, int nNewSubmode)
{
	if(nNewMode&MODE_PAINT)
	{
		pDocument->SetSelectedObjectsTexture(GetTexAPI()->GetCurTextureName());
		pDocument->UpdateWindows();
	}
}
