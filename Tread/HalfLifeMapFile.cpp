// HALFLIFEMAPFILE.CPP
// Copyright © 1999 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#include "stdafx.h"
#include "TreadMapAPI.h"
#include "QuakeMapFile.h"
#include "HalfLifeMapFile.h"
#include "szstring.h"
#include "Entity.h"
#include "TreadSys.h"
#include "Tread3DDoc.h"
#include "GameAPI.h"
#include <fstream.h>


// We override this to remove the "wad" key from the worldspawn.
int CHalfLifeMapFile::ReadEntity(CEntity** ppEntity, CTread3DDoc* pDocument, CTokenizer& Tokenizer)
{
	int r = CQuakeMapFile::ReadEntity(ppEntity, pDocument, Tokenizer);
	if(r != QRF_WORLDSPAWN)
		return r;

	// Remove the key.
	(*ppEntity)->DeleteKey("wad");
	return r;
}

// We override this to write the texture name in UPPERCASE
bool CHalfLifeMapFile::WriteFace(CFace* pFace, CTread3DDoc* pDocument, fstream& file)
{
	int i;
	// Just skip.
	if(pFace->GetNumPoints() < 3)
	{
		Warning("CQuakeMapFile::WriteFace(): < 3 pts.");
		return true;
	}

	// Write the 3 point plane def.
	vec3_t* vPoints = pFace->GetPoints();
	for(i = 0; i < 3; i++)
		file << "( " << (int)(floor(vPoints[i][0] - 0.5) + 1) << " " << (int)(floor(vPoints[i][1] - 0.5) + 1) << " " << (int)(floor(vPoints[i][2] - 0.5) + 1) << " ) ";

	CString sTexture = pFace->GetTextureName();
	sTexture.MakeUpper();
	file << sTexture;

	vec_t vShifts[2], vScales[2];
	pFace->GetTexScales(vScales);
	pFace->GetTexShifts(vShifts);

	file << " " << (int)vShifts[0] << " " << (int)vShifts[1] << " " << (int)pFace->GetRotation() << " " << vScales[0] << " " << vScales[1];
	file << "\n";

	m_nFaceCount++;
	return true;
}

bool CHalfLifeMapFile::WriteWorldspawn(CEntity* pEntity, CTread3DDoc* pDocument, CLinkedList<CBaseObject>* pObjectList, CLinkedList<CBaseObject>* pSelectedObjectList, fstream& file)
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

	// Write the .wad list.
	file << "\"wad\" \"";
	bool bSemi = false;

	GAMEPAKFILE* pPak;
	CLinkedList<GAMEPAKFILE>* pPakList = GetGameAPI()->GetCurrentGame()->GetPakFileList();
	pPakList->First();
	for(pPak = pPakList->GetCurItem(); pPak != NULL; pPak = pPakList->GetNextItem())
	{
		if(bSemi)
			file << ";";
		bSemi = true;
		file << pPak->sFilename;
	}
	file << "\"\n";

	// Normal.
	if(!WriteObjectList(pObjectList, OBJECT_CLASS_BRUSH|OBJECT_CLASS_GROUP, pDocument, file))
		return false;
	if(!WriteObjectList(pSelectedObjectList, OBJECT_CLASS_BRUSH|OBJECT_CLASS_GROUP, pDocument, file))
		return false;

	file << "}\n";

	return true;
}
