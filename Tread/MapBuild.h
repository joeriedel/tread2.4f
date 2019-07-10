// MAPBUILD.H
// Copyright © 1999 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#ifndef MAPBUILD_H
#define MAPBUILD_H

#include "LinkedList.h"
#include "GameAPI.h"

struct MAPBUILDBLOCK
{
	DEFBUILD* pDefBuild;
	RUNBLOCK* pRunBlock;
};

UINT MapBuildThread(LPVOID pVoid);

#endif