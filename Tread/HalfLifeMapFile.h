// HALFLIFEMAPFILE.H
// Copyright © 1999 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#ifndef HALFLIFEMAPFILE_H
#define HALFLIFEMAPFILE_H

#include "TreadMapAPI.h"
#include "QuakeMapFile.h"
#include "MapObject.h"
#include <fstream.h>

class CHalfLifeMapFile : public CQuakeMapFile
{
public:

	virtual bool WriteWorldspawn(CEntity* pEntity, CTread3DDoc* pDocument, CLinkedList<CBaseObject>* pObjectList, CLinkedList<CBaseObject>* pSelectedObjectList, fstream& file);
	virtual bool WriteFace(CFace* pFace, CTread3DDoc* pDocument, fstream& file);
	virtual int ReadEntity(CEntity** ppEntity, CTread3DDoc* pDocument, CTokenizer& Tokenizer);

};

#endif