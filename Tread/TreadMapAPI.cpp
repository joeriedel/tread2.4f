// TREADMAPAPI.CPP
// Copyright © 1999 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#include "stdafx.h"
#include "TreadMapAPI.h"
#include "MapObject.h"
#include "TreadSys.h"
#include "TreadMap2X.h"
#include "Brush.h"
#include "Entity.h"
#include "Face.h"
#include "Winding.h"
#include "Camera.h"
#include "szstring.h"
#include "QuakeMapFile.h"
#include "Quake2MapFile.h"
#include "Quake3MapFile.h"
#include "HalfLifeMapFile.h"
#include "NighthawkMapFile.h"
#include "GameAPI.h"
#include "PluginAPI.h"

// Only the win32 version reads old tread 1.12 files.
#ifdef _WIN32
#include "TreadMap1X.h"
#include "GameDialog.h"
#include "ObjectView.h"
#include "TextureView.h"
#include "DialogControl.h"
#include "ViewControl.h"
#endif

CTreadMapAPI m_TreadMapAPI;
CTreadMapAPI* GetTreadMapAPI() { return &m_TreadMapAPI; }

CTreadMapAPI::CTreadMapAPI()
{
}

CTreadMapAPI::~CTreadMapAPI()
{
}

CBaseTreadMapFile* CTreadMapAPI::GetFileReader(CString sTag, int nMagic, CTread3DDoc* pDocument)
{
	if(sTag == _2X_HEADER_TAG && nMagic == _2X_HEADER_MAGIC)
	{
		Debug("... map is a 2x file\n");
		return new CTreadMap2X();
	}

#ifdef _WIN32
	if(sTag == _1X_HEADER_TAG && nMagic == _1X_HEADER_MAGIC)
	{
		Debug("... map is a 1x file\n");
		return new CTreadMap1X();
	}
#endif

	// We let them choose.
	if(GetMapManager()->GetMapCount() == 1)
	{
		Debug("... unkown game type, querying user...\n");

#ifdef _WIN32
		CGameDialog dlgTemp;

		if(dlgTemp.DoModal() != IDOK)
			return false;

		Debug("... setting game type\n");

		if(!GetGameAPI()->SetCurrentGame(dlgTemp.GetGameSelection()))
		{
			Error("SetGame(): failed!\n");
			return false;
		}

		Debug("... initing controls\n");

		CTextureView* pTextureView = (CTextureView*)GetView(TEXTURE_VIEW);
		pTextureView->LoadList();

		CObjectView* pObjectView = (CObjectView*)GetView(OBJECT_VIEW);
		pObjectView->LoadBrushList();

		CVisgroupView* pVisgroupView = (CVisgroupView*)GetView(VISGROUP_VIEW);
		pVisgroupView->LoadVisgroup();
#else
		return false;
#endif
	}
	else
	{
		Debug("... assuming concurent game types\n");
#ifdef _WIN32
		int nMsg = AfxMessageBox("Tread will assume that the format of file being imported is of the current game.", MB_OKCANCEL|MB_ICONEXCLAMATION);
		if(nMsg != IDOK)
			return NULL;
#endif
	}

	CString sType = GetGameAPI()->GetCurrentGame()->GetMapFileTypeString();

	Debug("... searching map subsystem for file reader\n");
	Debug("... checking: plugin subsystem\n");

	// Give the plugins a chance.
	CBaseTreadMapFile* pF = GetPluginAPI()->GetMapReader(sTag, nMagic, pDocument);
	if(pF != NULL)
	{
		Debug("... plugin subsystem reader found\n");
		return pF;
	}

	Debug("... checking built-in readers\n");

	// Select file type.
	if(sType == "QUAKE_MAP")
	{
		Debug("... quake reader selected\n");
		return new CQuakeMapFile();
	}
	else if(sType == "QUAKE2_MAP" || sType == "NIGHTHAWK_MAP")
	{
		Debug("... quake2 reader selected\n");
		return new CQuake2MapFile();
	}
	else if(sType == "QUAKE3_MAP")
	{
		Debug("... quake3 reader selected\n");
		return new CQuake3MapFile();
	}
	else if(sType == "HALFLIFE_MAP")
	{
		Debug("... halflife reader selected\n");
		return new CHalfLifeMapFile();
	}

	Debug("... abort: no reader was found\n");

	return NULL;
}

CBaseTreadMapFile* CTreadMapAPI::GetFileWriter(TREAD2XHEADERINFO* pHeader, CTread3DDoc* pDocument)
{
	if(pHeader->m_nFlags&T2XHF_MAPFILE)
	{
		Debug("... map is of the 'map' subtype\n");
		Debug("... checking: plugin subsystem\n");

		// Give plugins a change.
		CBaseTreadMapFile* pF = GetPluginAPI()->GetMapWriter(pHeader, pDocument);
		if(pF != NULL)
		{
			Debug("... plugin subsytem writer found\n");
			return pF;
		}

		Debug("... checking built-in writers\n");
		
		//return NULL;
		CString sType = GetGameAPI()->GetCurrentGame()->GetMapFileTypeString();
		if(sType == "QUAKE_MAP")
		{
			Debug("... quake writer selected\n");
			return new CQuakeMapFile();
		}
		else 
		if(sType == "QUAKE2_MAP")
		{
			Debug("... quake2 writer selected\n");
			return new CQuake2MapFile();
		}
		else
		if(sType == "QUAKE3_MAP")
		{
			Debug("... quake3 writer selected\n");
			return new CQuake3MapFile();
		}
		else
		if(sType == "HALFLIFE_MAP")
		{
			Debug("... halflife writer selected\n");
			return new CHalfLifeMapFile();
		}
		else
		if(sType == "NIGHTHAWK_MAP")
		{
			Debug("... nighthawk writer selected\n");
			return new CNighthawkMapFile();
		}

		Debug("... abort: no writer found\n");
		return NULL;
	}
	
	Debug("... writing 2x file\n");
	return new CTreadMap2X();
}

bool CTreadMapAPI::WriteMap(CFile& hFile, TREAD2XHEADERINFO* pHeader, CTread3DDoc* pDocument, CLinkedList<CBaseObject>* pObjectList, CLinkedList<CBaseObject>* pSelectedObjectList, CLinkedList<CBaseObject>* pUIObjectList)
{

	Debug("mapAPI: writing %s\n", hFile.GetFilePath());
	Debug("mapAPI: running type identification\n");

	CBaseTreadMapFile* pWriter = GetFileWriter(pHeader, pDocument);

	if(pWriter != NULL)
		return pWriter->WriteMap(hFile, pHeader, pDocument, pObjectList, pSelectedObjectList, pUIObjectList);
	
	delete pWriter;

	return false;
}

bool CTreadMapAPI::ReadMap(CFile& hFile, TREAD2XHEADERINFO* pHeader, CTread3DDoc* pDocument, CLinkedList<CBaseObject>* pObjectList, CLinkedList<CBaseObject>* pUIObjectList)
{
	int nMagic;
	char szTag[8] = {NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL};

	Debug("mapAPI: reading %s\n", hFile.GetFilePath());
	Debug("mapAPI: running format identification\n");

	// A trd/trd2/tmd2 file?
	if(CompareExt(hFile.GetFilePath(), "trd2") || CompareExt(hFile.GetFilePath(), "tmd2") || CompareExt(hFile.GetFilePath(), "trd"))
	{
		hFile.Read(szTag, 7);
		hFile.Read(&nMagic, 4);
	}
	else
	{
		strcpy(szTag, "map");
		nMagic = 0;
	}

	CBaseTreadMapFile* pReader = GetFileReader(szTag, nMagic, pDocument);
	if(pReader == NULL)
		return false;

	bool s = pReader->ReadMap(hFile, pHeader, pDocument, pObjectList, pUIObjectList);
	delete pReader;

	return s;
}
