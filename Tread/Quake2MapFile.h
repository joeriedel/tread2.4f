// QUAKE2MAPFILE.H
// Copyright © 1999 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#ifndef QUAKE2MAPFILE_H
#define QUAKE2MAPFILE_H

#include "TreadMapAPI.h"
#include "QuakeMapFile.h"
#include "MapObject.h"
#include "Entity.h"
#include "Brush.h"
#include "Face.h"
#include <fstream.h>

class CQuake2MapFile : public CQuakeMapFile
{
public:
	
	virtual bool WriteFace(CFace* pFace, CTread3DDoc* pDocument, fstream& file);
	virtual int ReadPlane(CQBrushPlane* pPlane, CTread3DDoc* pDocument, CTokenizer& Tokenizer);

};

#endif