// QUAKE3MAPFILE.H
// Copyright © 2000 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#ifndef QUAKE3MAPFILE_H
#define QUAKE3MAPFILE_H

#include "TreadMapAPI.h"
#include "QuakeMapFile.h"
#include "MapObject.h"
#include <fstream.h>

class CQuake3MapFile : public CQuakeMapFile
{
public:

	virtual bool WriteWorldspawn(CEntity* pEntity, CTread3DDoc* pDocument, CLinkedList<CBaseObject>* pObjectList, CLinkedList<CBaseObject>* pSelectedObjectList, fstream& file);

};

#endif