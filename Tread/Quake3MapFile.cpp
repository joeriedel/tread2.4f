// QUAKE3MAPFILE.CPP
// Copyright © 2000 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#include "stdafx.h"
#include "TreadMapAPI.h"
#include "QuakeMapFile.h"
#include "Quake3MapFile.h"
#include "szstring.h"
#include "TreadSys.h"
#include "Tread3DDoc.h"
#include "GameAPI.h"
#include <fstream.h>

bool CQuake3MapFile::WriteWorldspawn(CEntity* pEntity, CTread3DDoc* pDocument, CLinkedList<CBaseObject>* pObjectList, CLinkedList<CBaseObject>* pSelectedObjectList, fstream& file)
{	
	file << "{\n";
	file << "\"classname\" \"worldspawn\"\n";

	// Write keys;
	ENTITYKEY* pKey;
	CLinkedList<ENTITYKEY>* pKeyList = pEntity->GetKeyList();
	pKeyList->First();
	for(pKey = pKeyList->GetCurItem(); pKey != NULL; pKey = pKeyList->GetNextItem())
	{
		if(pKey->sName == "$SURFACE_77X" || pKey->sName == "$CONTENTS_77X")
			continue;
		if(pKey->sValue == "0" || pKey->sValue == "")
			continue;
		file << "\"" << pKey->sName << "\"" << " \"" << pKey->sValue << "\"\n";
	}

	// Normal.
	if(!WriteObjectList(pObjectList, OBJECT_CLASS_BRUSH|OBJECT_CLASS_GROUP, pDocument, file))
		return false;
	if(!WriteObjectList(pSelectedObjectList, OBJECT_CLASS_BRUSH|OBJECT_CLASS_GROUP, pDocument, file))
		return false;

	file << "}\n";

	return true;
}