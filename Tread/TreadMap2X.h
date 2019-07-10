// TREADMAP2X.H
// Copyright © 1999 Joe Riedel, Nick Randal.
// Author: Joe Riedel
#ifndef TREADMAP2X_H
#define TREADMAP2X_H

#include "TreadMapAPI.h"

#define _2X_HEADER_TAG			"tread2x"
#define _2X_HEADER_MAGIC		0xFA067802
#define _2X_HEADER_VERSION_1	0x00000001
#define _2X_HEADER_VERSION_2	0x00000002
#define _2X_HEADER_VERSION_3	0x00000003
#define _2X_HEADER_VERSION_4	0x00000004
#define _2X_HEADER_VERSION_5	0x00000005
#define _2X_HEADER_VERSION_6	0x00000006
#define _2X_HEADER_VERSION_7	0x00000007
#define _2X_HEADER_VERSION_8	0x00000008
#define _2X_HEADER_VERSION_9	0x00000009
#define _2X_HEADER_VERSION_10	0x0000000A
#define _2X_HEADER_VERSION		0x0000000B

class CTreadMap2X : public CBaseTreadMapFile
{

public:

	bool WriteHeader(CFile& hFile, TREAD2XHEADERINFO* pHeader, CTread3DDoc* pDocument);
	bool ReadHeader(CFile& hFile, TREAD2XHEADERINFO* pHeader, CTread3DDoc* pDocument);
	bool WriteMap(CFile& hFile, TREAD2XHEADERINFO* pHeader, CTread3DDoc* pDocument, CLinkedList<CBaseObject>* pObjectList, CLinkedList<CBaseObject>* pSelectedObjectList, CLinkedList<CBaseObject>* pUIObjectList);
	bool ReadMap(CFile& hFile, TREAD2XHEADERINFO* pHeader, CTread3DDoc* pDocument, CLinkedList<CBaseObject>* pObjectList, CLinkedList<CBaseObject>* pUIObjectList);

};

#endif