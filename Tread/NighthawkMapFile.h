// NIGHTHAWKMAPFILE.H
// Copyright © 1999 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#ifndef NIGHTHAWKMAPFILE_H
#define NIGHTHAWKMAPFILE_H

#include "TreadMapAPI.h"
#include "QuakeMapFile.h"
#include "MapObject.h"
#include "Entity.h"
#include "Brush.h"
#include "Face.h"
#include <fstream.h>

class CNighthawkMapFile : public CQuakeMapFile
{
public:
	
	virtual bool WriteBrush(CQBrush* pBrush, CTread3DDoc* pDocument, fstream& file);
	virtual bool WriteFace(CFace* pFace, CTread3DDoc* pDocument, fstream& file);
	virtual int ReadPlane(CQBrushPlane* pPlane, CTread3DDoc* pDocument, CTokenizer& Tokenizer);

};

#endif