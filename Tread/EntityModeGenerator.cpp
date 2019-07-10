// ENTITYMODEGENERATOR.CPP
// Copyright © 1999 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#include "stdafx.h"
#include "MapObject.h"
#include "HotSpotManager.h"
#include "EntityModeGenerator.h"
#include "MapView.h"
#include "Tread3DDoc.h"
#include "TreadSys.h"
#include "Brush.h"

CEntityModeGenerator::CEntityModeGenerator()
{
	m_nMode = MODE_ENTITY;
	m_nSubmode = ALL_MODES;
}
