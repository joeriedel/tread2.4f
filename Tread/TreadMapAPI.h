// TREADMAPAPI.H
// Copyright © 1999 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#ifndef TREADMAPAPI_H
#define TREADMAPAPI_H

#include "LinkedList.h"
#include "MapObject.h"
#include "TExport.h"

// $sb "treadmapapi.h block1"
#define T2XHF_NONE			0x00000000
#define T2XHF_BRUSH			0x00000001
#define T2XHF_MAPFILE		0x00000002
#define T2XHF_MODEL			0x00000004
#define T2XHF_TRD2FILE		0x00000008
#define T2XHF_IGNORE		0x00000010
#define T2XHF_NOAUTOSIZE	0x00000020
#define T2XHF_RTOBJECT		0x00000040

struct RUNSTATE;

struct _T_EXPORT TREAD2XHEADERINFO
{
	int m_nVersion;
	int m_nFlags;
	int m_nValue;
	int m_nClass;	// Only for T2XHF_OBJX

	int m_nRenderFlags;
	int m_nGridSize;
	int m_nCurveSubDepth;
	bool m_bGridSnap;
	bool m_bBrushHandles;
	bool m_bDrawTargets;
	bool m_bUpdateAllViews;
	int m_nViewTypes[4];
	float m_fScale[4];
	float m_fX[4];
	float m_fY[4];
	float m_fHollowDepth;

	CString m_sGame;
	CString m_sAuthor;
	CString m_sComments;
	CString m_sCategory;
	CString m_sName;

	int m_nNumStates;
	RUNSTATE* m_pRunStates;

	TREAD2XHEADERINFO()
	{
		m_nVersion = 0;
		m_nFlags = T2XHF_NONE;
		m_nClass = -1;
		m_sGame="";
		m_sAuthor="";
		m_sComments="";
		m_sCategory=""; // For models.
		m_sName="";		// For models.
	}
};

class CTread3DDoc;
class _T_EXPORT CBaseTreadMapFile
{
public:

	CBaseTreadMapFile() {}
	virtual ~CBaseTreadMapFile() {}

	virtual bool WriteHeader(CFile& hFile, TREAD2XHEADERINFO* pHeader, CTread3DDoc* pDocument) { return true; }
	virtual bool ReadHeader(CFile& hFile, TREAD2XHEADERINFO* pHeader, CTread3DDoc* pDocument) { return true; }
	virtual bool WriteMap(CFile& hFile, TREAD2XHEADERINFO* pHeader, CTread3DDoc* pDocument, CLinkedList<CBaseObject>* pObjectList, CLinkedList<CBaseObject>* pSelectedObjectList, CLinkedList<CBaseObject>* pUIObjectList) { return true; }
	virtual bool ReadMap(CFile& hFile, TREAD2XHEADERINFO* pHeader, CTread3DDoc* pDocument, CLinkedList<CBaseObject>* pObjectList, CLinkedList<CBaseObject>* pUIObjectList) { return true; }

};


class _T_EXPORT CTreadMapAPI
{
	
private:

	CBaseTreadMapFile* GetFileReader(CString sTag, int nMagic, CTread3DDoc* pDocument);
	CBaseTreadMapFile* GetFileWriter(TREAD2XHEADERINFO* pHeader, CTread3DDoc* pDocument);

public:
	
	CTreadMapAPI();
	~CTreadMapAPI();

	bool WriteMap(CFile& hFile, TREAD2XHEADERINFO* pHeader, CTread3DDoc* pDocument, CLinkedList<CBaseObject>* pObjectList, CLinkedList<CBaseObject>* pSelectedObjectList, CLinkedList<CBaseObject>* pUIObjectList);
	bool ReadMap(CFile& hFile, TREAD2XHEADERINFO* pHeader, CTread3DDoc* pDocument, CLinkedList<CBaseObject>* pObjectList, CLinkedList<CBaseObject>* pUIObjectList);

};
// $eb

CTreadMapAPI* GetTreadMapAPI();

#endif