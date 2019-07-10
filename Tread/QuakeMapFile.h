// QUAKEMAPFILE.H
// Copyright © 1999 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#ifndef QUAKEMAPFILE_H
#define QUAKEMAPFILE_H

#include "TreadMapAPI.h"
#include "MapObject.h"
#include "Entity.h"
#include "Brush.h"
#include "Face.h"
#include "Tokenizer.h"
#include <fstream.h>

class CQuakeMapFile : public CBaseTreadMapFile
{
protected:

	int m_nEntityCount;
	int m_nBrushCount;
	int m_nFaceCount;

	virtual bool FindDuplicatePlane(CLinkedList<CQBrushPlane>* pPlaneList, CQBrushPlane* pPlane);

public:

	virtual bool WriteMap(CFile& hFile, TREAD2XHEADERINFO* pHeader, CTread3DDoc* pDocument, CLinkedList<CBaseObject>* pObjectList, CLinkedList<CBaseObject>* pSelectedObjectList, CLinkedList<CBaseObject>* pUIObjectList);
	virtual bool ReadMap(CFile& hFile, TREAD2XHEADERINFO* pHeader, CTread3DDoc* pDocument, CLinkedList<CBaseObject>* pObjectList, CLinkedList<CBaseObject>* pUIObjectList);

#define QRF_FATAL			-1
#define QRF_ERROR			0
#define QRF_OK				1
#define QRF_WORLDSPAWN		2
#define QRF_GROUP			3

	virtual int ReadEntity(CEntity** ppEntity, CTread3DDoc* pDocument, CTokenizer& Tokenizer);
	virtual int ReadBrush(CQBrush** ppBrush, CTread3DDoc* pDocument, CTokenizer& Tokenizer);
	virtual int ReadPlane(CQBrushPlane* pPlane, CTread3DDoc* pDocument, CTokenizer& Tokenizer);

	virtual bool WriteObject(CBaseObject* pObject, int nClass, CTread3DDoc* pDocument, fstream& file);
	virtual bool WriteObjectList(CLinkedList<CBaseObject>* pList, int nClass, CTread3DDoc* pDocument, fstream& file);
	virtual bool WriteGroup(CObjectGroup* pGroup, int nClass, CTread3DDoc* pDocument, fstream& file);
	virtual bool WriteWorldspawn(CEntity* pEntity, CTread3DDoc* pDocument, CLinkedList<CBaseObject>* pObjectList, CLinkedList<CBaseObject>* pSelectedObjectList, fstream& file);
	virtual bool WriteFace(CFace* pFace, CTread3DDoc* pDocument, fstream& file);
	virtual bool WriteBrush(CQBrush* pBrush, CTread3DDoc* pDocument, fstream& file);
	virtual bool WriteEntity(CEntity* pEntity, CTread3DDoc* pDocument, fstream& file);

};

#endif