// ENTITYMODEGENERATOR.CPP
// Copyright © 1999 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#include "stdafx.h"
#include "MapObject.h"
#include "HotSpotManager.h"
#include "BrushModeGenerator.h"
#include "MapView.h"
#include "Tread3DDoc.h"
#include "TreadSys.h"
#include "Brush.h"

CBrushModeGenerator::CBrushModeGenerator()
{
	m_nMode = MODE_BRUSH;
	m_nSubmode = ALL_MODES;
}
