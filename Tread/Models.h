// MODELS.H
// Copyright © 1999 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#ifndef MODELS_H
#define MODELS_H

#include "MathLib.h"
#include "MapObject.h"
#include "TreadMapAPI.h"

// $sb "models.h block1"
// Models are trd2 files.
struct MODEL
{
	MODEL* prev, *next;
	TREAD2XHEADERINFO m_Header;
	CLinkedList<CBaseObject>* m_pObjectList;
	vec3_t vMins, vMaxs, vOrigin;

	MODEL();
	~MODEL();
};
// $eb
#endif
