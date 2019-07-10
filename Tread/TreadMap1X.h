// TREADMAP1X.H
// Copyright © 1999 Joe Riedel, Nick Randal.
// Author: Joe Riedel
#ifndef TREADMAP1X_H
#define TREADMAP1X_H

#include "TreadMapAPI.h"
#include "Entity.h"
#include "Face.h"

#define _1X_HEADER_TAG		"dilbert"
#define _1X_HEADER_MAGIC	1979
#define _1X_HEADER_VERSION	((1<<16)+1)

class CTreadMap1X : public CBaseTreadMapFile
{
	bool ReadVisgroups(CFile& hFile, CTread3DDoc* pDocument);
	bool ReadEntity(CFile& hFile, CEntity* pOwner, CTread3DDoc* pDocument);
	bool ReadEntities(CFile& hFile, CTread3DDoc* pDocument);
	bool ReadBrush(CFile& hFile, CEntity* pOwner, CTread3DDoc* pDocument);
	CFace* ReadFace(CFile& hFile, CTread3DDoc* pDocument);
	bool ReadCameras(CFile& hFile, CTread3DDoc* pDocument);

public:

	bool ReadHeader(CFile& hFile, TREAD2XHEADERINFO* pHeader, CTread3DDoc* pDocument);
	bool ReadMap(CFile& hFile, TREAD2XHEADERINFO* pHeader, CTread3DDoc* pDocument, CLinkedList<CBaseObject>* pObjectList, CLinkedList<CBaseObject>* pUIObjectList);

};

#endif