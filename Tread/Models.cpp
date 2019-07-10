// MODELS.CPP
// Copyright © 1999 Joe Riedel, Nick Randal.
// Author: Joe Riedel
#include "stdafx.h"
#include "Models.h"
#include "MapObject.h"

MODEL::MODEL()
{
	m_pObjectList = new CLinkedList<CBaseObject>();
}

MODEL::~MODEL()
{
	if(m_pObjectList != NULL)
		delete m_pObjectList;
}