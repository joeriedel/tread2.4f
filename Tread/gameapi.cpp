// GAMEAPI.CPP
// Copyright © 1999 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#include "stdafx.h"
#include "Entity.h"
#include "TexAPI.h"
#include "GameAPI.h"
#include "TreadSys.h"
#include "Tread3D.h"
#include "FUtils.h"
#include "Models.h"
#include "TreadMapAPI.h"
#include "MapBuild.h"
#include "PluginAPI.h"
#include "MapObject.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>
#include "szstring.h"
#ifdef _WIN32
#include <malloc.h>
#endif
#ifdef macintosh
#include "mac_registry.h"
#endif

CGameAPI gGameAPI;
CGameAPI* GetGameAPI() { return &gGameAPI; }

//////////////////////////////////////////////////////////////////////////
// Game API																//
//////////////////////////////////////////////////////////////////////////
// Parses a GDF Script.
bool CGameAPI::ParseGDFScript(CString sFilename)
{
	CGameDefinition* pGameDef = new CGameDefinition();

	Debug("CGameAPI::ParseGDFScript(): '" + sFilename + "'\n");

	CCSP(sFilename);
	if(!pGameDef->ParseGDFScript(sFilename))
	{
		delete pGameDef;
		return false;
	}

	// Add to list.
	m_GameDefinitionList.AddItem(pGameDef);

	return true;
}

CGameAPI::CGameAPI()
{
	m_pCurGameDef = NULL;
}

CGameAPI::~CGameAPI() 
{
}

// Purges the current game and all its resources.
bool CGameAPI::PurgeAndReset()
{
	if(m_pCurGameDef == NULL)
		return true;

	Console("sysFree: Purging '" + m_pCurGameDef->GetGameName() + "'\n");
	Debug("sysFree: Purging '" + m_pCurGameDef->GetGameName() + "'\n");
	m_pCurGameDef->FreeGameResources();
	GetTexAPI()->FreeAllLoadedTextures();
	GetTexAPI()->CloseAllPaks();
	m_pCurGameDef = NULL;

	return true;
}
// Set's the current game.
bool CGameAPI::SetCurrentGame(CString sName)
{
	// If we're just resetting, don't do anything.
	if(sName == "")
		return true;
	
	// Don't do anything if the games are the same.
	if(m_pCurGameDef == GetGameDefinition(sName))
		return true;

	// If we already have a game, close down the resources.
	if(m_pCurGameDef != NULL)
		PurgeAndReset();
	
	// Find the new game.
	m_pCurGameDef = GetGameDefinition(sName);
	if(m_pCurGameDef == NULL)
	{
		CString sMsg;
		sMsg.Format("CGameAPI::SetCurrentGame(): unable to find definition for '" + sName + "'");
		Error(sMsg);
		return false;
	}

	// Load the game resources.
	Console("sysLoad: '" + m_pCurGameDef->GetGameName() + "'\n");
	// Load the game resources.
	if(!m_pCurGameDef->LoadGameResources())
		return false;

	GetPluginAPI()->PostEvent(PEF_GAMECHANGE, 0, NULL);

	return true;
}

// Initializes the GameAPI
// Loads all GDF scripts.
bool CGameAPI::Initialize()
{
	CString sGDFPath;
	CString sSearchPath;
	CString sFilename;
	
	// Get the gdf path.
	sGDFPath = GetRegString(S_TREAD_PATH) + "\\scripts";
	sSearchPath = sGDFPath + "\\*.gdf2";
	
	Debug("CGameAPI::Initialize(): GDF2: '" + sGDFPath + "'\n");
	Debug("CGameAPI::Initialize(): Search: '" + sSearchPath + "'\n");

	PrintSplashLine("CGameAPI::Initialize(): GDF2: '" + sGDFPath);
	PrintSplashLine("CGameAPI::Initialize(): Search: '" + sSearchPath);
	PrintSplashLine("Parsing scripts...");

	CFileSearch FileSearch;

	if(!FileSearch.BeginFileSearch(sGDFPath, "*.gdf2"))
	{
		Error("CGameAPI::Initialize(): Unable to parse /scripts path");
		return false;
	}

	for(;;)
	{
		if(FileSearch.GetNextFile(sFilename))	// Skip directories.
			continue;
		if(sFilename == "")
			break;
		
		PrintSplashLine(sFilename);

		if(!ParseGDFScript(sGDFPath + "\\" + sFilename))
			Error("CGameAPI::Initialize(): Unable to parse script: " + sFilename);
	}

	FileSearch.EndFileSearch();

	if(m_GameDefinitionList.GetCount() == 0)
	{
		Error("CGameAPI::Initialize(): No scripts loaded.");
		return false;
	}

	return true;
}

// Saves all registry settings.
bool CGameAPI::SaveRegSettings()
{
	CGameDefinition* pGameDef;

	m_GameDefinitionList.First();
	for(pGameDef = m_GameDefinitionList.GetCurItem(); pGameDef; pGameDef = m_GameDefinitionList.GetNextItem())
		pGameDef->SaveRegSettings();
	return true;
}

//////////////////////////////////////////////////////////////////////////
// Game Definition.														//
//////////////////////////////////////////////////////////////////////////
bool CGameDefinition::IsCapEnabled(CString sName)
{
	GAMECAP* pCap;

	for(pCap = m_CapsList.ResetPos(); pCap != NULL; pCap = m_CapsList.GetNextItem())
	{
		if(pCap->sName == sName)
			return pCap->bEnabled;
	}

	return false;
}

GAMECAP* CGameDefinition::ParseDefCap()
{
	CString sT1, sT2;

	if(!VerifyGetToken(":"))
	{
		ScriptError(NULL, "CGameDefinition::ParseDefCap(): Missing ':'");
		return NULL;
	}

	if(!Parse2Pair(sT1, sT2))
	{
		ScriptError(NULL, "CGameDefinition::ParseDefModule(): Missing tokens");
		return NULL;
	}

	sT1.MakeLower(); sT2.MakeLower();

	GAMECAP* pCap = new GAMECAP();
	pCap->sName = sT1;
	pCap->bEnabled = (sT2 == "true");
	return pCap;
}

bool CGameDefinition::LoadModels()
{
	CString sPath = GetRegString(S_TREAD_PATH) + "\\models\\" + m_sName;
	CString sSearch = sPath + "\\*.tmd2";
	CString sFilename;
	CString sLoadFile;
	bool b;
	CFile hFile;
	MODEL* pModel;
	int nNum=0;

	CFileSearch FileSearch;

	Debug("mdlLoad: loading models from /models/" + m_sName + "\n");
	if(!FileSearch.BeginFileSearch(sPath, "*.tmd2"))
	{
		Debug("...error parsing models from: " + sPath + "\n");
		return true;
	}

	for(;;)
	{
		if(FileSearch.GetNextFile(sFilename))
			continue;
		if(sFilename == "")
			break;

		sLoadFile = sPath + "\\" + sFilename;
		pModel = new MODEL();
		if(!hFile.Open(sLoadFile, CFile::typeBinary | CFile::modeRead))
		{
			Debug("error opening: " + sLoadFile + "\n");
			continue;
		}

		b = GetTreadMapAPI()->ReadMap(hFile, &pModel->m_Header, NULL, pModel->m_pObjectList, NULL);
		hFile.Close();
		if(!b)
		{
			Debug("error reading: " + sLoadFile + "\n");
			continue;
		}

		if(pModel->m_Header.m_sName == "")
			pModel->m_Header.m_sName = "Undefined Name";
		if(pModel->m_Header.m_sCategory == "")
			pModel->m_Header.m_sCategory = "Undefined Category";

		m_pModelList->AddItem(pModel);
		Debug("...: " + pModel->m_Header.m_sCategory + ": " + pModel->m_Header.m_sName + "\n");
		nNum++;

		// Cube?
		if(pModel->m_Header.m_sCategory == "Primitives")
			if(pModel->m_Header.m_sName == "Cube")
				m_pCurModel = pModel;
	}

	FileSearch.EndFileSearch();
	Debug("mdlLoad: loaded %i model(s)\n", nNum);

	// Do we add any models by default?
	if(IsCapEnabled("q3_quad_patch"))
	{
		pModel = new MODEL();

		pModel->m_Header.m_sName = "Simple Patch";
		pModel->m_Header.m_sCategory = "Curves";
		pModel->m_Header.m_nFlags = T2XHF_RTOBJECT|T2XHF_NOAUTOSIZE;
		pModel->m_Header.m_nClass = OBJECT_CLASS_PATCHMESH;

		m_pModelList->AddItem(pModel);
	}

	// No cube, just default to first model loaded.
	if(m_pCurModel == NULL)
		m_pCurModel = m_pModelList->GetItem(0);

	return true;
}

bool CGameDefinition::SaveModel(MODEL* pModel)
{
	CString sFilePath;
	CString sFilename;
	CFile hFile;

	sFilePath = GetRegString(S_TREAD_PATH) + "\\models\\" + m_sName;

	// Make sure this dir exists.
	if(GetFileAttributes(sFilePath) == 0xFFFFFFFF)
	{
		// Create it.
		if(!CreateDirectory(sFilePath, NULL))
		{
			Error("unable to create dir: " + sFilePath);
			return false;
		}
	}

	if(pModel->m_Header.m_nFlags & T2XHF_BRUSH)
		sFilename = pModel->m_Header.m_sCategory + "$" + pModel->m_Header.m_sName + "$brush";
	else
		sFilename = m_sName + "$" + pModel->m_Header.m_sCategory + "$" + pModel->m_Header.m_sName + "$model";

	sFilename += ".tmd2";

	sFilePath += "\\" + sFilename;
	
	if(!hFile.Open(sFilePath, CFile::typeBinary | CFile::modeWrite | CFile::modeCreate))
	{
		Error("Unable to open: " + sFilePath);
		return false;
	}

	if(!GetTreadMapAPI()->WriteMap(hFile, &pModel->m_Header, NULL, pModel->m_pObjectList, NULL, NULL))
	{
		hFile.Close();
		Error("Unable to write: " + sFilePath);
		DeleteFile(sFilePath);
		return false;
	}

	hFile.Close();

	Debug("wrote model: /models/" + m_sName + "/" + sFilename + "\n");
	Console("wrote model: /models/" + m_sName + "/" + sFilename + "\n");

	return true;
}

// Finds the game definition with the spec name.
CGameDefinition* CGameAPI::GetGameDefinition(CString sName)
{
	CGameDefinition* pGameDef;

	m_GameDefinitionList.First();
	for(pGameDef = m_GameDefinitionList.GetCurItem(); pGameDef; pGameDef = m_GameDefinitionList.GetNextItem())
	{
		if(pGameDef->GetGameName() == sName)
			return pGameDef;
	}

	return NULL;
}

CEntity* CGameDefinition::CreateEntityFromDefinition(CString sClassname)
{
	ENTITYDEFINITION* pEntDef;

	pEntDef = FindEntityDefinition(sClassname);
	if(pEntDef == NULL)
	{
		Debug("CGameDefinition::CreateEntityFromDefinition(): undefined class '" + sClassname + "'\n");
		return NULL;
	}

	CEntity* pEnt = (CEntity*)NewBaseObject(OBJECT_CLASS_ENTITY);
	pEnt->InitFromDefinition(pEntDef);
	return pEnt;
}

// Loads all the game resources.
bool CGameDefinition::LoadGameResources()
{
	unsigned char* pPalLump;

	Debug("sysLoad: " + m_sName + "...\n");

	// Try to load the palette.
	if(m_sPalFile != "")
	{
		int nNumBytes;

		Debug("palette: /palettes/" + m_sPalFile + "\n");

		// Get Tread's path.
		CString sPath = GetRegString(S_TREAD_PATH);
	
		// Read the palette.
		pPalLump = BloadFile(sPath + "\\palettes\\" + m_sPalFile, nNumBytes);
		if(pPalLump == NULL)
		{
			Error("CGameDefinition::LoadGameResources(): Unable to load game palette");
			return false;
		}

		// How big is it?
		if(nNumBytes != 768)	// Assuming Quake/Quake2 style palette lump.
			Warning("CGameDefinition::LoadGameResources(): Palette file is not a 768 byte lump file. Paletted texture colors may be incorrect.\n");

		m_pPalette = new CTexPalette(pPalLump);
	}
	else
		m_pPalette = NULL;

	// Load all the pak files.
	int nCount=0;
	GAMEPAKFILE* pPak;
	m_PakFileList.First();
	for(pPak = m_PakFileList.GetCurItem(); pPak; pPak = m_PakFileList.GetNextItem())
	{
		Debug("* " + pPak->sTitle + "\n");
		pPak->nHandle = GetTexAPI()->OpenPak(pPak->sFilename, pPak->bDir);
		if(pPak->nHandle == BAD_PAK_HANDLE)
		{
			CString sMsg;
			sMsg.Format("pakError: Unable to load pak '" + pPak->sFilename + "'\n");
			Warning(sMsg);
		}
		else
			nCount++;
	}

	GetTexAPI()->BuildOpenPaks();

	Debug("pakLoad: loaded %i pak(s)\n", nCount);
	
	LoadModels();

	// Set the default solid/point entities.
	m_pCurSolidEntity = FindEntityDefinition(m_sDefSolid);
	m_pCurPointEntity = FindEntityDefinition(m_sDefPoint);

	return true;
}

// Free's games resources.
// NOTE: It is the GameAPI's job to free textures from the API.
bool CGameDefinition::FreeGameResources()
{
	// Palette?
	if(m_pPalette != NULL)
		delete m_pPalette;

	m_pPalette = NULL;

	m_pModelList->DestroyList();

	return true;
}

// Loads Reg Settings.
bool CGameDefinition::LoadRegSettings()
{
	int i, nNumPaks;
	GAMEPAKFILE* pPak;
	CString sPakString, sRegDir;
	
#if _WIN32
	CWinApp* pApp = AfxGetApp();
	if(pApp == NULL)
		return false;
#else
	CMacRegistry* pApp = GetTreadMacRegistry();
#endif

	sRegDir = "settings\\games\\" + m_sName;

	// We load the pak list here.
	nNumPaks = pApp->GetProfileInt("settings\\games\\" + m_sName, "NumPaks", 0);
	//if(nNumPaks == 0)
	//	return true;

	for(i = 0; i < nNumPaks; i++)
	{
		pPak = new GAMEPAKFILE();
		sPakString.Format(sRegDir + "\\pak%i", i);
		
		pPak->sTitle = pApp->GetProfileString(sPakString, "title", "");
		pPak->sFilename = pApp->GetProfileString(sPakString, "filepath", "");
		pPak->bDir = (bool)pApp->GetProfileInt(sPakString, "isdir", 0);
		pPak->nHandle = BAD_PAK_HANDLE;

		m_PakFileList.AddItem(pPak);
	}

	// Read the progs.
	DEFMODULE* pModule;
	m_DefBuild.pModuleList->First();
	for(pModule = m_DefBuild.pModuleList->GetCurItem(); pModule != NULL; pModule = m_DefBuild.pModuleList->GetNextItem())
	{
		if(pModule->nPathType == PATH_TYPE_NONE)
			continue;
		pModule->sPath = pApp->GetProfileString(sRegDir+"\\build\\"+pModule->sName, "path", pModule->sPath);
	}
	return true;
}

// Saves the Reg settings.
bool CGameDefinition::SaveRegSettings()
{
	int i, nNumPaks;
	GAMEPAKFILE* pPak;
	CString sPakString, sRegDir;
	
#ifdef _WIN32
	CWinApp* pApp = AfxGetApp();
#else
	CMacRegistry* pApp = GetTreadMacRegistry();
#endif

	if(pApp == NULL)
		return false;

	sRegDir = "settings\\games\\" + m_sName;

	// Write the # of paks.
	nNumPaks = m_PakFileList.GetCount();
	pApp->WriteProfileInt(sRegDir, "NumPaks", nNumPaks);
	//if(nNumPaks == 0)
	//	return true;

	// Store each.
	m_PakFileList.First();
	for(i=0, pPak = m_PakFileList.GetCurItem(); pPak; pPak = m_PakFileList.GetNextItem(), i++)
	{
		sPakString.Format(sRegDir + "\\pak%i", i);
		
		pApp->WriteProfileString(sPakString, "title", pPak->sTitle);
		pApp->WriteProfileString(sPakString, "filepath", pPak->sFilename);
		pApp->WriteProfileInt(sPakString, "isdir", (int)pPak->bDir);
	}

	// Write the progs.
	// Read the progs.
	DEFMODULE* pModule;
	m_DefBuild.pModuleList->First();
	for(pModule = m_DefBuild.pModuleList->GetCurItem(); pModule != NULL; pModule = m_DefBuild.pModuleList->GetNextItem())
	{
		if(pModule->nPathType == PATH_TYPE_NONE)
			continue;
		pApp->WriteProfileString(sRegDir+"\\build\\"+pModule->sName, "path", pModule->sPath);
	}
	
#ifdef macintosh
	pApp->FlushRegistry();
#endif

	return true;
}

//////////////////////////////////////////////////////////////////
// Game Definition File Scripting Section						//
//////////////////////////////////////////////////////////////////
bool CGameDefinition::ParseGDFScript(CString sFilename)
{
	CString sString;

	m_sScriptFile = sFilename;

	if(!m_Tokenizer.InitParsing(sFilename))
	{
		ScriptError(NULL, "CGameDefinition::ParseGDFScript(): Unable to init tokenizer on file '%s'", (LPCTSTR)sFilename);
		return false;
	}

	for(;;)
	{
		if(!m_Tokenizer.GetToken(sString))
			break;

		if(sString == "$DefGlobal")
		{
			if(!ParseGlobalDefinition())
				return false;
		}
		else
		if(sString == "$DefEnt")
		{
			ENTITYDEFINITION* pEntDef = ParseEntityDefinition();
			if(pEntDef == NULL)
				return false;

			m_EntDefList.AddItem(pEntDef);
		}
		else
		if(sString == "$DefNames")
		{
			// Parse until ']'
			for(;;)
			{
				if(!m_Tokenizer.GetToken(sString))
				{
					ScriptError(NULL, "CGameDefinition::ParseGDFScript(): bad $DefNames");
					return false;
				}

				if(sString == "]")
					break;
			}

			Debug("Script Deprication Warning:\nThe '$DefNames' specifier is no longer used. Please upgrade the script.\n");
		}
		else
		{
			ScriptError(NULL, "CGameDefintion::ParseGDFScript(): Unrecognized token '%s'", (LPCTSTR)sString);
			return false;
		}
	}

	m_Tokenizer.FreeScript();
	Debug("-- gdf: parsed %i entity(s)\n", m_EntDefList.GetCount());
	return LoadRegSettings();
}

// Compiles/Run's a map.
bool CGameDefinition::RunMap(RUNBLOCK* pRunBlock)
{
	ClearConsole();
	Console("Running Map: Compiling GDF2 Script...\n");
	/*if(!ResolveBuildAliases(pRunBlock))
	{
		Error("Run script compilation error! Check script and console for errors.");
		return false;
	}*/
#ifdef _WIN32
	// Running already?
	if(m_pCurThread != NULL)
	{
		DWORD dwStatus;

		// Alive?
		GetExitCodeProcess(m_pCurThread->m_hThread, &dwStatus);
		if(dwStatus == STILL_ACTIVE)
		{
			Warning("build/aborting: build process already active.\n");
			return false;
		}

		// Delete the thread object.
		delete m_pCurThread;
		m_pCurThread = NULL;
	}
#endif

	Console("Running Code...\n");
	Console("----------------------------------------\n");
	MAPBUILDBLOCK* pMapBlock = new MAPBUILDBLOCK();
	pMapBlock->pDefBuild = &m_DefBuild;
	pMapBlock->pRunBlock = pRunBlock;
	
#ifdef _WIN32
	m_pCurThread = AfxBeginThread(MapBuildThread, pMapBlock, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
	m_pCurThread->m_bAutoDelete = false;
	m_pCurThread->ResumeThread();
#endif

	return true;
}

DEFMODULE* CGameDefinition::GetDefModule(CString sName)
{
	DEFMODULE* pModule;

	m_DefBuild.pModuleList->First();
	for(pModule = m_DefBuild.pModuleList->GetCurItem(); pModule != NULL; pModule = m_DefBuild.pModuleList->GetNextItem())
	{
		if(pModule->sName == sName)
			return pModule;
	}


	return NULL;
}

DEFMODULE* CGameDefinition::ParseDefModule()
{
	// [ MODULE_NAME ]
	CString sString;
	DEFMODULE* pModule;

	pModule = new DEFMODULE();
	if(!pModule)
	{
		ScriptError(NULL, "CGameDefinition::ParseDefModule(): Memory allocation failure");
		return NULL;
	}

	// [ STATE_OBJECT ]
	if(!VerifyGetToken("["))
	{
		ScriptError(NULL, "CGameDefinition::ParseDefModule(): Missing '['");
		return NULL;
	}

	if(!Parse3Pair(pModule->sName, pModule->sSetupTitle, pModule->sRunTitle))
	{
		ScriptError(NULL, "CGameDefinition::ParseDefModule(): Bad 3 pair statement.");
		return NULL;
	}


	if(!VerifyGetToken("]"))
	{
		ScriptError(NULL, "CGameDefinition::ParseDefModule(): Missing ']'");
		return NULL;
	}

	if(!VerifyGetToken("{"))
	{
		ScriptError(NULL, "CGameDefinition::ParseDefModule(): Missing '{'");
		return NULL;
	}

	pModule->nType = MODULE_TYPE_NOTRUNNABLE;
	pModule->nPathType = PATH_TYPE_NONE;
	pModule->pDefParam;

	for(;;)
	{
		if(!m_Tokenizer.GetToken(sString))
		{
			ScriptError(NULL, "CGameDefinition::ParseDefModule(): Unexpected EOF");
			delete pModule;
			return NULL;
		}

		if(sString == "}")
			break;
		
		if(sString == "$DefParams")
		{
			if(pModule->pDefParam != NULL)
			{
				ScriptError(NULL, "CGameDefinition::ParseDefModule(): multiple states.");
				delete pModule;
				return NULL;
			}

			DEFPARAM* pDef = ParseDefParams();
			if(pDef == NULL)
			{
				delete pModule;
				return NULL;
			}

			pModule->pDefParam = pDef;
		}
		else
		if(sString == "$SpawnWait" || sString == "$SpawnNoWait")
		{
			RUNPROGRAMCMD* pRunProgram = new RUNPROGRAMCMD();
			if(pRunProgram == NULL)
			{
				ScriptError(NULL, "CGameDefinition::ParseDefModule(): Memory allocation failure");
				delete pModule;
				return NULL;
			}

			CString sTemp;

			if(!VerifyGetToken(":") || !Parse4Pair(pRunProgram->sModule, pRunProgram->sWorkDirAlias, sTemp, pRunProgram->sOutput))
			{
				ScriptError(NULL, "CGameDefinition::ParseDefModule(): Bad 4 Pair statement");
				delete pModule;
				return NULL;
			}
			
			if(sString == "$SpawnWait")
				pRunProgram->bWait = true;
			else
				pRunProgram->bWait = false;

			if(sTemp == "@CONSOLE")
				pRunProgram->bConsole = true;
			else
				pRunProgram->bConsole = false;

			// Add it.
			pModule->pProgramList->AddItem(pRunProgram);
		}
		else
		if(sString == "$ExportMap")
		{
			CString sOutput;

			if(!VerifyGetToken(":") || !Parse2Pair(sString, sOutput))
			{
				ScriptError(NULL, "CGameDefinition::ParseDefParam(): missing ':' or EOF");
				delete pModule;
				return NULL;
			}

			EXPORTMAPCMD* pExportCmd = new EXPORTMAPCMD();
			pExportCmd->sOutName = sString;
			pExportCmd->sOutput = sOutput;
			pModule->pExportList->AddItem(pExportCmd);
		}
		else
		if(sString == "def_path")
		{
			if(!VerifyGetToken(":"))
			{
				ScriptError(NULL, "CGameDefinition::ParseDefModule(): missing ':'");
				delete pModule;
				return NULL;
			}

			if(!m_Tokenizer.GetToken(pModule->sPath))
			{
				ScriptError(NULL, "CGameDefinition::ParseDefModule(): Unexpected EOF.");
				delete pModule;
				return NULL;
			}
		}
		else
		if(sString == "def_type")
		{
			if(!VerifyGetToken(":"))
			{
				ScriptError(NULL, "CGameDefinition::ParseDefModule(): missing ':'");
				delete pModule;
				return NULL;
			}

			if(!m_Tokenizer.GetToken(sString))
			{
				ScriptError(NULL, "CGameDefinition::ParseDefModule(): Unexpected EOF.");
				delete pModule;
				return NULL;
			}

			if(sString == "@RUNNABLE")
				pModule->nType = MODULE_TYPE_RUNNABLE;
			else
				pModule->nType = MODULE_TYPE_NOTRUNNABLE;
		}
		else
		if(sString == "def_pathtype")
		{
			if(!VerifyGetToken(":"))
			{
				ScriptError(NULL, "CGameDefinition::ParseDefModule(): missing ':'");
				delete pModule;
				return NULL;
			}

			if(!m_Tokenizer.GetToken(sString))
			{
				ScriptError(NULL, "CGameDefinition::ParseDefModule(): Unexpected EOF.");
				delete pModule;
				return NULL;
			}

			if(sString == "@FILE")
				pModule->nPathType = PATH_TYPE_FILE;
			else if(sString == "@DIR")
				pModule->nPathType = PATH_TYPE_DIR;
			else
			{
				ScriptError(NULL, "CGameDefinition::ParseDefModule(): unrecognized type.");
				delete pModule;
				return NULL;
			}
		}
		else
		{
			ScriptError(NULL, "CGameDefinition::ParseDefModule(): unrecognized token " + sString);
			delete pModule;
			return NULL;		
		}
	}

	return pModule;
}

bool CGameDefinition::ParseDefBuild()
{
	CString sString;
	
	if(!VerifyGetToken("{"))
	{
		ScriptError(NULL, "CGameDefinition::ParseDefBuild(): Missing '{'");
		return false;
	}

	for(;;)
	{
		if(!m_Tokenizer.GetToken(sString))
		{
			ScriptError(NULL, "CGameDefinition::ParseDefBuild(): Unexpected EOF");
			return false;
		}
	
		if(sString == "}")
			break;

		// Hopefully it's a $DefModule, cause we don't do anything else.
		if(sString == "$DefModule")
		{
			DEFMODULE* pMod = ParseDefModule();
			if(pMod == NULL)
				return false;

			m_DefBuild.pModuleList->AddItem(pMod);
		}
		/*else if(sString == "def_mapext" || sString == "def_bspext")
		{
			CString sToken;

			if(!VerifyGetToken(":"))
			{
				ScriptError(NULL, "CGameDefinition::ParseDefBuild(): missing ':'");
				return false;
			}

			if(!m_Tokenizer.GetToken(sToken))
			{
				ScriptError(NULL, "CGameDefinition::ParseDefBuild(): Unexpected EOF");
				return false;
			}

			if(sString == "def_mapext")
				m_DefBuild.m_sMapExt = sToken;
			else
				m_DefBuild.m_sBspExt = sToken;
		}*/
		else
		{
			ScriptError(NULL, "CGameDefinition::ParseDefBuild(): Unrecognized token '%s'", (LPCTSTR)sString);
			return false;
		}
	}

	return true;
}

// Actual parsing routines.
DEFPARAM* CGameDefinition::ParseDefParams()
{
	CString sString;
	DEFPARAM* pDefParam;
	int nVal=0;

	pDefParam = new DEFPARAM();

	if(!VerifyGetToken("{"))
	{
		ScriptError(NULL, "CGameDefinition::ParseDefParam(): Missing '{'");
		return NULL;
	}

	for(;;)
	{
		if(!m_Tokenizer.GetToken(sString))
		{
			ScriptError(NULL, "CGameDefinition::ParseEvalState(): Unexpected EOF");
			delete pDefParam;
			return NULL;
		}

		if(sString == "}")
			break;

		// What is it?
		if(sString == "def_param")
		{
			CString sValue;
			DEFPARAMPARAM* pParam = new DEFPARAMPARAM();

			if(pParam == NULL)
			{
				ScriptError(NULL, "CGameDefinition::ParseDefParam(): Unexpected EOF");
				delete pDefParam;
				return NULL;
			}

			if(!VerifyGetToken(":") || !Parse4Pair(pParam->sTitle, pParam->sParam, sValue, sString))
			{
				ScriptError(NULL, "CGameDefinition::ParseDefParam(): Bad 4 Pair statement");
				delete pDefParam;
				return NULL;
			}

			pParam->bAlways = false;
			if(sValue != "@ALWAYS")
				pParam->nValue = atoi(sValue);
			else
				pParam->bAlways = true;

			pParam->bDefault = (sString == "true" ) ? true : false;
			if(!pParam->bAlways && pParam->bDefault)
				nVal |= pParam->nValue;

			pDefParam->pParamList->AddItem(pParam);
		}
		else
		{
			ScriptError(NULL, "CGameDefinition::ParseDefParam(): Unrecognized token '%s'", (LPCTSTR)sString);
			delete pDefParam;
			return NULL;
		}
	}

	pDefParam->nValue = nVal;
	return pDefParam;
}

// Parses a global defintion. Assumes that $DefGlobal has been parsed.
bool CGameDefinition::ParseGlobalDefinition()
{
	CString sString;

	m_sTextureType = "@Q2_WAL";

	// [ GAMENAME ]
	if(!VerifyGetToken("["))
	{
		ScriptError(NULL, "CGameDefinition::ParseGlobalDefinition(): Missing '['");
		return false;
	}

	// Store name.
	if(!m_Tokenizer.GetToken(m_sName))
	{
		ScriptError(NULL, "CGameDefinition::ParseGlobalDefinition(): Unexpected EOF");
		return false;
	}
	
	if(!VerifyGetToken("]"))
	{
		ScriptError(NULL, "CGameDefinition::ParseGlobalDefinition(): Missing ']'");
		return false;
	}

	if(!VerifyGetToken("{"))
	{
		ScriptError(NULL, "CGameDefinition::ParseGlobalDefinition(): Missing '{'");
		return false;
	}

	for(;;)
	{
		if(!m_Tokenizer.GetToken(sString))
		{
			ScriptError(NULL, "CGameDefintion::ParseGlobalDefinition(): Unexpected EOF");
			return false;
		}

		if(sString == "}")
			break;

		// What is it?
		if(sString == "def_solid")
		{
			if(!VerifyGetToken(":") || !m_Tokenizer.GetToken(m_sDefSolid))
			{
				ScriptError(NULL, "CGameDefinition::ParseGlobalDefinition(): bad def_solid");
				return false;
			}
		}
		else
		if(sString == "def_point")
		{
			if(!VerifyGetToken(":") || !m_Tokenizer.GetToken(m_sDefPoint))
			{
				ScriptError(NULL, "CGameDefinition::ParseGlobalDefinition(): bad def_solid");
				return false;
			}
		}
		else
		if(sString == "def_worldspawn")
		{
			if(!VerifyGetToken(":") || !m_Tokenizer.GetToken(m_sWorldspawn))
			{
				ScriptError(NULL, "CGameDefinition::ParseGlobalDefinition(): bad def_worldspawn");
				return false;
			}
		}
		else
		if(sString == "def_palette" || sString == "palette")
		{
			if(!VerifyGetToken(":") || !m_Tokenizer.GetToken(m_sPalFile))
			{
				ScriptError(NULL, "CGameDefinition::ParseGlobalDefinition(): bad def_solid");
				return false;
			}
			
			if(sString == "palette")
			{
				CString sWarnMessage;
				sWarnMessage.Format("Script Deprication Warning:\nThe 'palette' specifier is now 'def_palette'. Please upgrade the script.\n");
				Debug(sWarnMessage);
			}
		}
		else
		if(sString == "def_namekey" || sString == "def_etn")
		{
			if(!VerifyGetToken(":") || !m_Tokenizer.GetToken(m_sNameKey))
			{
				ScriptError(NULL, "CGameDefinition::ParseGlobalDefinition(): bad def_namekey");
				return false;
			}

			if(sString == "palette")
			{
				CString sWarnMessage;
				sWarnMessage.Format("Script Deprication Warning:\nThe 'palette' specifier is now 'def_palette'. Please upgrade the script.\nScript: %s\n", m_sScriptFile);
				Debug(sWarnMessage);
			}
		}
		else
		if(sString == "def_targetkey" || sString == "def_etk")
		{
			if(!VerifyGetToken(":") || !m_Tokenizer.GetToken(m_sTargetKey))
			{
				ScriptError(NULL, "CGameDefinition::ParseGlobalDefinition(): bad def_targetkey\n");
				return false;
			}

			if(sString == "def_etk")
			{
				CString sWarnMessage;
				sWarnMessage.Format("Script Deprication Warning:\nThe 'def_etk' specifier is now 'def_targetkey'. Please upgrade the script.\n");
				Debug(sWarnMessage);
			}
		}
		else
		if(sString == "def_pakdir")
		{
			if(!VerifyGetToken(":") || !m_Tokenizer.GetToken(sString))
			{
				ScriptError(NULL, "CGameDefinition::ParseGlobalDefinition(): bad def_pakdir");
				return false;
			}

			CString sWarnMessage;
			sWarnMessage.Format("Script Deprication Warning:\nThe 'def_pakdir' specifier is no longer used. Please upgrade the script.\n");
			Debug(sWarnMessage);
		}
		else
		if(sString == "def_key")
		{
			// Parse the key.
			ENTITYKEYDEFINITION* pKeyDef = ParseDefKey(NULL);
			if(pKeyDef == NULL)
				return false;
			m_GlobalEntDef.AddKeyDefinition(pKeyDef);
		}
		else
		if(sString == "def_author")
		{
			if(!VerifyGetToken(":") || !m_Tokenizer.GetToken(m_sAuthor))
			{
				ScriptError(NULL, "CGameDefinition::ParseGlobalDefinition(): bad def_author");
				return false;
			}
		}
		else
		if(sString == "def_version")
		{
			if(!VerifyGetToken(":") || !m_Tokenizer.GetToken(m_sVersion))
			{
				ScriptError(NULL, "CGameDefinition::ParseGlobalDefinition(): bad def_version");
				return false;
			}
		}
		else
		if(sString == "def_gamemaptype" || sString == "def_gft")
		{
			if(!VerifyGetToken(":") || !m_Tokenizer.GetToken(m_sMapFileType))
			{
				ScriptError(NULL, "CGameDefinition::ParseGlobalDefinition(): bad def_gamemaptype");
				return false;
			}

			if(sString == "def_gft")
			{
				CString sWarnMessage;
				sWarnMessage.Format("Script Deprication Warning:\nThe 'def_gft' specifier is now 'def_gamemaptype'. Please upgrade the script.\n");
				Debug(sWarnMessage);
			}
		}
		else
		if(sString == "$DefFlags" || sString == "$DefOptions")
		{
			ENTITYKEYDEFINITION* pKeyDef = ParseDefOptions(sString, NULL);
			if(pKeyDef == NULL)
				return false;
			m_GlobalEntDef.AddKeyDefinition(pKeyDef);
		}
		else
		if(sString == "def_key")
		{
			ENTITYKEYDEFINITION* pKeyDef = ParseDefKey(NULL);
			if(pKeyDef == NULL)
				return false;
			m_GlobalEntDef.AddKeyDefinition(pKeyDef);
		}
		else
		if(sString == "$DefBuild")
		{
			if(!ParseDefBuild())
				return false;
		}
		else
		if(sString == "def_textype")
		{
			if(!VerifyGetToken(":") || !m_Tokenizer.GetToken(m_sTextureType))
			{
				ScriptError(NULL, "CGameDefinition::ParseGlobalDefinition(): bad def_textype");
				return false;
			}
		}
		else
		if(sString == "def_cap")
		{
			GAMECAP* pCap = ParseDefCap();
			if(!pCap)
				return false;
			m_CapsList.AddItem(pCap);
		}
		else
		{
			ScriptError(NULL, "CGameDefinition::ParseGlobalDefinition(): unrecognized token '%s'. Script parsing not completed, this script will be unavailable.", (LPCTSTR)sString);
			return false;
		}
	}	
	
	if(m_sWorldspawn == "")
		m_sWorldspawn = "worldspawn";

	m_GlobalEntDef.sName = m_sWorldspawn;
	m_GlobalEntDef.pBaseClass = NULL;
	m_GlobalEntDef.sSmartName = m_sWorldspawn;

	return true;
}

// Adds a key def to an entity def.
bool ENTITYDEFINITION::AddKeyDefinition(ENTITYKEYDEFINITION* pKeyDef)
{
	ENTITYKEYDEFINITION* pCurKeyDef;

	// See if a key w/ this name exists.
	pCurKeyDef = FindKeyDefinition(pKeyDef->sName);
	if(pCurKeyDef)
	{
		// Remove it, and add this one.
		pKeyList->DeleteItem(pCurKeyDef);	// Delete it.
	}

	return pKeyList->AddItem(pKeyDef);
}
		
ENTITYKEYDEFINITION* ENTITYDEFINITION::FindKeyDefinition(CString sKeyName)
{
	ENTITYKEYDEFINITION* pKeyDef;

	pKeyList->SetPosition(LL_HEAD);
	for(pKeyDef = pKeyList->GetCurItem(); pKeyDef; pKeyDef = pKeyList->GetNextItem())
	{
		if(pKeyDef->sName == sKeyName)
			return pKeyDef;
	}

	return NULL;
}

// Finds an entity definition.
ENTITYDEFINITION* CGameDefinition::FindEntityDefinition(CString sClassName)
{
	CString sString;
	ENTITYDEFINITION* pEntDef;

	if(sClassName == GetWorldspawnName())
		return GetWorldspawnDefinition();

	m_EntDefList.SetPosition(LL_HEAD);
	for(pEntDef = m_EntDefList.GetCurItem(); pEntDef; pEntDef = m_EntDefList.GetNextItem())
	{
		sString = pEntDef->sName;
		if(sString == sClassName)
			return pEntDef;
	}

	return NULL;
}

// Parses an entity. Assumes that $DefEnt has been parsed.
ENTITYDEFINITION* CGameDefinition::ParseEntityDefinition()
{
	CString sTemp;
	CString sBaseClass;
	CString sName, sSmartName="@VIRTUAL", sCategory;
	bool bVirtual=false;
	ENTITYDEFINITION* pEntityDef;

	if(!VerifyGetToken("["))
	{
		ScriptError(NULL, "CGameDefinition::ParseEntityDefinition(): missing '['");
		return NULL;
	}

	// Read first pair.
	if(!Parse2Pair(sName, sSmartName))
	{
		ScriptError(NULL, "CGameDefinition::ParseEntityDefinition(): Invalid 2 pair. Check for a missing ':'");
		return NULL;
	}

	//Debug("dbg: $DefEnt: '" + sName + "'");

	// What's the next token?
	if(!m_Tokenizer.GetToken(sTemp))
	{
		ScriptError(NULL, "CGameDefinition::ParseEntityDefinition(): Unexpected EOF");
		return NULL;
	}

	// Close ] or a : @VIRTUAL. or a category name.
	if(sTemp != ":" && sTemp != "]")
	{
		ScriptError(NULL, "CGameDefinition::ParseEntityDefinition(): Invalid block");
		return NULL;
	}

	// In v1.x we only allowed for classname/smartname
	// We have added the ability to sort entity names via a category as well, so here's
	// the new syntax:
	//
	// $DefEnt [ CLASSNAME : (SMARTNAME|@VIRTUAL) : CATEGORY|@NULL]
	// Should be @VIRTUAL/CATEGORY
	if(sTemp == ":")
	{
		if(!m_Tokenizer.GetToken(sTemp))
		{
			ScriptError(NULL, "CGameDefinition::ParseEntityDefinition(): Unexpected EOF");
			return NULL;
		}
		if(sTemp != "@VIRTUAL")	// Must be a category
		{
			// Store the category.
			if(sTemp != "@NULL")
				sCategory = sTemp;
			else
				sCategory = "Global";

			if(!m_Tokenizer.GetToken(sTemp))
			{
				ScriptError(NULL, "CGameDefinition::ParseEntityDefinition(): Unexpected EOF");
				return NULL;
			}

			if(sTemp == ":")
			{
				if(!VerifyGetToken("@VIRTUAL"))
				{
					ScriptError(NULL, "CGameDefinition::ParseEntityDefinition(): Expecting '@VIRTUAL' class modifier");
					return NULL;
				}

				bVirtual = true;
				
				// Next should be a closing ].
				if(!m_Tokenizer.GetToken(sTemp))
				{
					ScriptError(NULL, "CGameDefinition::ParseEntityDefinition(): Unexpected EOF");
					return NULL;
				}
			}
			else
				bVirtual = false;

			// Must be closing bracket.
			if(sTemp != "]")
			{
				ScriptError(NULL, "CGameDefinition::ParseEntityDefinition(): Expecting ']'");
				return NULL;
			}
		}
		else
		{
			bVirtual = true;
				
			// Next should be a closing ].
			if(!VerifyGetToken("]"))
			{
				ScriptError(NULL, "CGameDefinition::ParseEntityDefinition(): Missing ']'");
				return NULL;
			}
		}
	}	
	else
		sCategory = "";

	if(sSmartName == "@VIRTUAL")
	{
		bVirtual = true;
		sSmartName = "";
	}

	// Now get the baseclass, if there is one.
	if(!VerifyGetToken(":"))
		m_Tokenizer.UngetToken();
	/*{
		ScriptError(NULL, "GameAPI::ParseEntityDefinition(): Expecting ':'");
		return NULL;
	}*/

	if(!m_Tokenizer.GetToken(sTemp))
	{
		ScriptError(NULL, "CGameDefinition::ParseEntityDefinition(): Unexpected EOF");
		return NULL;
	}

	// Just []?
	if(sTemp == "[]")
	{
		pEntityDef = AllocateEntityDefinition();
		if(pEntityDef == NULL)
			return NULL;
		pEntityDef->pBaseClass = NULL;
	}
	else if(sTemp == "[")
	{
		// Get the base class name.
		if(!m_Tokenizer.GetToken(sBaseClass))
		{
			ScriptError(NULL, "CGameDefinition::ParseEntityDefinition(): Unexpected EOF");
			return NULL;
		}

		if(!VerifyGetToken("]"))
		{
			ScriptError(NULL, "CGameDefinition::ParseEntityDefinition(): Missing ']'");
			return NULL;
		}

		ENTITYDEFINITION* pBaseClass = FindEntityDefinition(sBaseClass);
		if(pBaseClass == NULL)
		{
			ScriptError(NULL, "CGameDefinition::ParseEntityDefinition(): Undefined base class '%s'", (LPCTSTR)sBaseClass);
			return NULL;
		}

		// Make this version derrived from the base class.
		pEntityDef = new ENTITYDEFINITION(*pBaseClass);
		if(!pEntityDef)
			return NULL;
		pEntityDef->pBaseClass = pBaseClass;

		// Inherit the category. If it has no cat name, then use the classname.
		if(sCategory == "")
		{
			if(pEntityDef->sCategory.IsEmpty())
			{
				if(pEntityDef->bVirtual)
					sCategory = sName;
				else
					sCategory = pEntityDef->pBaseClass->sName;
			}
			else
				sCategory = pEntityDef->sCategory;
		}
	}
	else
	{
		// Unknown symbol.
		ScriptError(NULL, "CGameDefinition::ParseEntityDefinition(): Expecting [ 'BASECLASS' ]");
		return NULL;
	}

	// Fill in entity info.
	pEntityDef->bVirtual = bVirtual;
	pEntityDef->sName = sName;
	pEntityDef->sSmartName = sSmartName;
	pEntityDef->sCategory = sCategory;
	
	// v1.02 - v1.1x all had to specify a ': [ GAME ]' section. v2.0 doesn't require this
	// anymore. However, we must be able to ignore it.
	if(!m_Tokenizer.GetToken(sTemp))
	{
		ScriptError(pEntityDef, "CGameDefinition::ParseEntityDefinition(): Unexpected EOF");
		return NULL;
	}

	// : ?
	if(sTemp == ":")
	{
		// Parse the [ GAME ].
		if(!VerifyGetToken("["))
		{
			ScriptError(pEntityDef, "CGameDefinition::ParseEntityDefinition(): Expecting '[ GAME ]'");
			delete pEntityDef;
			return NULL;
		}

		m_Tokenizer.GetToken(sTemp);

		if(!VerifyGetToken("]"))
		{
			ScriptError(pEntityDef, "CGameDefinition::ParseEntityDefinition(): Expecting '[ GAME ]'");
			delete pEntityDef;
			return NULL;
		}

		// Verify an opening block.
		if(!m_Tokenizer.GetToken(sTemp))
		{
			ScriptError(pEntityDef, "CGameDefinition::ParseEntityDefinition(): Unexpected EOF");
			delete pEntityDef;
			return NULL;
		}

		CString sWarnMessage;
		sWarnMessage.Format("Script Deprication Warning: Entity: %s\nThe [ GAMENAME ] block is no longer necessary. Please upgrade the script.\n", sName);
		Debug(sWarnMessage);

		if(sTemp == "{}")
			return pEntityDef;
		else if(sTemp != "{")
		{
			ScriptError(pEntityDef, "CGameDefinition::ParseEntityDefinition(): Missing '{'");
			delete pEntityDef;
			return NULL;
		}

	}
	else if(sTemp == "{}") // Nothing.
	{
		return pEntityDef;
	}
	else if(sTemp != "{")
	{
		ScriptError(pEntityDef, "CGameDefinition::ParseEntityDefinition(): Expecting '{'");
		delete pEntityDef;
		return NULL;
	}

	// Read Keys/Attributes/Flags.
	for(;;)
	{
		if(!m_Tokenizer.GetToken(sTemp))
		{
			ScriptError(pEntityDef, "CGameDefinition::ParseEntityDefinition(): Unexpected EOF");
			delete pEntityDef;
			return NULL;
		}
		
		if(sTemp == "}")
			break;
		else if(sTemp == "def_att")
		{
			// Parse the attribute.
			if(!ParseDefAttribute(pEntityDef))
			{
				delete pEntityDef;
				return NULL;
			}
		}
		else if(sTemp == "def_key")
		{
			// Parse the key.
			ENTITYKEYDEFINITION* pKeyDef;

			pKeyDef = ParseDefKey(pEntityDef);
			if(pKeyDef == NULL)
			{
				delete pEntityDef;
				return NULL;
			}

			// Add this key.
			if(!pEntityDef->AddKeyDefinition(pKeyDef))
			{
				delete pEntityDef;
				return NULL;
			}
		}
		else if(sTemp == "$DefOptions" || sTemp == "$DefFlags")
		{
			ENTITYKEYDEFINITION* pKeyDef;
			
			pKeyDef = ParseDefOptions(sTemp, pEntityDef);
			if(pKeyDef == NULL)
			{
				delete pEntityDef;
				return NULL;
			}
			
			pKeyDef->nType = EKD_TYPE_FLAGS;

			// Add the key.
			if(!pEntityDef->AddKeyDefinition(pKeyDef))
			{
				delete pEntityDef;
				return NULL;
			}
		}
		else
		{
			ScriptError(pEntityDef, "CGameDefinition::ParseEntityDefinition(): Unrecognized token '%s'", (LPCTSTR)sTemp);
			delete pEntityDef;
			return NULL;
		}
	}

	// Parsing of this entity is finished.
	return pEntityDef;
}
			
// Parses an attribute. Assumes that def_att has already been parsed.
bool CGameDefinition::ParseDefAttribute(ENTITYDEFINITION* pEntityDef)
{
	CString sType, sValue;

	// Parse the 2 pair.
	if(!VerifyGetToken(":") || !Parse2Pair(sType, sValue))
	{
		ScriptError(pEntityDef, "CGameDefinition::ParseDefAttribute(): Invalid 2 pair statement. Check for a missing ':'");
		return false;
	}

	// Type of attribute?
	if(sType == "owner")
	{
		// Boolean type.
		if(sValue == "true")
			pEntityDef->bOwner = true;
		else if(sValue == "false")
			pEntityDef->bOwner = false;
		else
		{
			ScriptError(pEntityDef, "CGameDefinition::ParseDefAttribute(): Invalid boolean operand.");
			return false;
		}
	}
	else if(sType == "size")
	{
		// Parse the vector.
		sscanf(sValue, "%f %f %f", &pEntityDef->vSize[0], &pEntityDef->vSize[1], &pEntityDef->vSize[2]);

		// Divide by two for drawing.
		pEntityDef->vSize[0] /= 2.0F;
		pEntityDef->vSize[1] /= 2.0F;
		pEntityDef->vSize[2] /= 2.0F;
	}
	else
	{
		ScriptError(pEntityDef, "CGameDefinition::ParseDefAttribute(): Invalid attribute specifier %s", sType);
		return false;
	}

	return true;
}

// Parses a def_key. Assumes that def_key has already been parsed.
ENTITYKEYDEFINITION* CGameDefinition::ParseDefKey(ENTITYDEFINITION* pEntityDef)
{
	ENTITYKEYDEFINITION* pKeyDef;
	CString sType, sName, sSmartName, sValue;

	if(!VerifyGetToken(":") || !Parse4Pair(sType, sName, sSmartName, sValue))
	{
		ScriptError(pEntityDef, "CGameDefinition::ParseDefKey(): Invalid 4 pair statement. Check for a missing ':'");
		return false;
	}

	// Allocate.
	pKeyDef = AllocateEntityKeyDefinition();
	if(!pKeyDef)
		return NULL;

	// Assign fields.
	pKeyDef->nType = AssignKeyType(sType);
	if(pKeyDef->nType == -1)
	{
		ScriptError(pEntityDef, "CGameDefinition::ParseKeyDef(): Invalid key type %s.", (LPCTSTR)sType);
		delete pKeyDef;
		return NULL;
	}

	// Verify that this key value matches the type.
	if(!VerifyKeyValue(sValue, pKeyDef->nType))
	{
		ScriptError(pEntityDef, "CGameDefinition::ParseKeyDef(): String value type mismatch. sValue = %s, nType = %d", sType, pKeyDef->nType);
		delete pKeyDef;
		return NULL;
	}

	// Store value.
	pKeyDef->sName = sName;
	pKeyDef->sSmartName = sSmartName;
	pKeyDef->sValue = sValue;
	pKeyDef->pFlagDef = NULL;

	return pKeyDef;
}

// Parses a $DefOptions/$DefFlags block. Assumes the $Def part has been parsed.
ENTITYKEYDEFINITION* CGameDefinition::ParseDefOptions(CString sType, ENTITYDEFINITION* pEntityDef)
{
	bool bSet;
	CString sToken;
	CString sName, sSmartName;
	CString sString, sValue, sBool;
	ENTITYKEYDEFINITION* pKeyDef;
	ENTITYFLAGDEFINITION* pFlagDef;
	ENTITYFLAGOPTIONDEFINITION* pOptionDef;

	// Parse the name/value.
	if(!VerifyGetToken("["))
	{
		ScriptError(pEntityDef, "CGameDefinition::ParseDefOptions(): Missing '['");
		return NULL;
	}

	if(!Parse2Pair(sName, sSmartName))
	{
		ScriptError(pEntityDef, "CGameDefinition::ParseDefOptions(): Invalid 2 pair statement. Check for a missing ':'");
		return NULL;
	}

	if(!VerifyGetToken("]"))
	{
		ScriptError(pEntityDef, "CGameDefinition::ParseDefOptions(): Missing ']'");
		return NULL;
	}

	if(!VerifyGetToken("{"))
	{
		ScriptError(pEntityDef, "CGameDefinition::ParseDefOptions(): Missing '{'");
		return NULL;
	}

	pKeyDef = AllocateEntityKeyDefinition();
	if(!pKeyDef)
		return NULL;
	pFlagDef = AllocateEntityFlagDefinition();
	if(!pFlagDef)
	{
		delete pKeyDef;
		return NULL;
	}

	pKeyDef->sName = sName;
	pKeyDef->sSmartName = sSmartName;

	// Link defintions.
	pKeyDef->pFlagDef = pFlagDef;

	// Type?
	if(sType == "$DefFlags")
		pFlagDef->bType = TRUE;
	else
		pFlagDef->bType = FALSE;


	// Read all keys.
	pFlagDef->nValue = 0;
	for(;;)
	{
		if(!m_Tokenizer.GetToken(sToken))
		{
			ScriptError(pEntityDef, "CGameDefinition::ParseDefOptions(): Unexpected EOF");
			delete pKeyDef;
			return NULL;
		}

		if(sToken == "}")
			break;

		if(sToken != "def_flg")
		{
			ScriptError(pEntityDef, "CGameDefinition::ParseDefOptions(): Expecting 'def_flg', found '%s'", (LPCTSTR)sToken);
			delete pKeyDef;
			return NULL;
		}

		// Parse the 3 pair.
		if(!VerifyGetToken(":") || !Parse3Pair(sString, sValue, sBool))
		{
			ScriptError(pEntityDef, "CGameDefinition::ParseDefOptions(): Invalid 3 pair. Check for a missing ':'");
			delete pKeyDef;
			return NULL;
		}

		// Make sure we have a valid boolean value.
		if(sBool != "true" && sBool != "false")
		{
			ScriptError(pEntityDef, "CGameDefinition::ParseDefOptions(): Unrecognized boolean operand. Must be [true,false]");
			delete pKeyDef;
			return NULL;
		}

		// True if value is set.
		bSet = sBool == "true";

		// Allocate.
		pOptionDef = AllocateEntityOptionDefinition();
		if(!pOptionDef)
		{
			delete pKeyDef;
			return NULL;
		}

		// Verify this key value.
		if(!VerifyKeyValue(sValue, EKD_TYPE_INTEGER))
		{
			ScriptError(pEntityDef, "CGameDefinition::ParseDefOptions(): Invalid integer value '%s'", (LPCTSTR)sValue);
			delete pKeyDef;
			return NULL;
		}

		// Get the integer value.
		if(sValue != "@USER")
		{
			pOptionDef->sUser = false;
			pOptionDef->nValue = (int)atoi(sValue);
		}
		else // For program scripting
		{
			bSet = false;
			pOptionDef->sUser = true;
			pOptionDef->nValue = 0;
		}

		pOptionDef->sString = sString;
		
		// Or in the value?
		if(bSet)
		{
			if(pFlagDef->bType)	// $DefFlags?
				pFlagDef->nValue |= pOptionDef->nValue;
			else
				pFlagDef->nValue = pOptionDef->nValue;
		}

		// Add to set list.
		pFlagDef->pFlagList->AddItem(pOptionDef);
	}

	return pKeyDef;
}

// Displays a scripting error.
void CGameDefinition::ScriptError(ENTITYDEFINITION* pEntityDef, const char* szMessage, ...)
{
	va_list argptr;
	char	szText[1024];
	char	szText2[1024];
	
	va_start(argptr, szMessage);
	vsprintf(szText, szMessage, argptr);
	va_end(argptr);

	sprintf(szText2, "%s\n\nLine: %d", szText, m_Tokenizer.GetLine());
	if(pEntityDef != NULL)
	{
		sprintf(szText, "%s\nClass: %s", szText2, (LPCTSTR)pEntityDef->sName);
	}
	else
		sprintf(szText, "%s\nClass: None", szText2);

#ifdef _WIN32
	MessageBox(NULL, szText, "Script Error:", 0 /* MB_OK */ );
#endif
}

// Verifies and gets a token.
bool CGameDefinition::VerifyGetToken(const char* szValue)
{
	CString sToken;
	return (m_Tokenizer.GetToken(sToken) && sToken == szValue);
}


// Parses "string"
bool CGameDefinition::Parse1Pair(CString& sString)
{
	CString sTemp;

	if(!m_Tokenizer.GetToken(sTemp))
		return false;

	if(sTemp == "NULL" || sTemp == "@NULL")
	{
		if(sTemp == "NULL")
			Debug("Script Deprication Warning: NULL has been changed to @NULL, line: %i\n", m_Tokenizer.GetLine());
		sString = "";
	}
	else
		sString = sTemp;

	return true;
}

// Assigns a key type. -1 for error.
int CGameDefinition::AssignKeyType(CString sType)
{
	if(sType == "integer")
		return EKD_TYPE_INTEGER;
	if(sType == "string")
		return EKD_TYPE_STRING;
	if(sType == "color")
		return EKD_TYPE_Q2COLOR;
	if(sType == "colorb" || sType == "color_hl")
		return EKD_TYPE_HLCOLOR;
	if(sType == "vector")
		return EKD_TYPE_ANGLE;

	return -1;
}

// Verify key value based on type.
bool CGameDefinition::VerifyKeyValue(CString sValue, int nType)
{
	if(nType != EKD_TYPE_INTEGER)
		return true;

	// Ensure we have all digits.
	int i=0;
	const char* szValue = (LPCTSTR)sValue;
	while(szValue[i] != NULL)
	{
		if(!isdigit((int)szValue[i]) && szValue[i] != '-' && szValue[i] != '.')
			return false;
		i++;
	}

	return true;
}

// Parses '"string1" : "string2"'
bool CGameDefinition::Parse2Pair(CString& sString1, CString& sString2)
{
	return (Parse1Pair(sString1) && VerifyGetToken(":") && Parse1Pair(sString2));
}

// Parses '"string1" : "string2" : "string3"'
bool CGameDefinition::Parse3Pair(CString& sString1, CString& sString2, CString& sString3)
{
	return (Parse2Pair(sString1, sString2) && VerifyGetToken(":") && Parse1Pair(sString3));
}

// Parses '"string1" : "string2" : "string3" : "string4"'
bool CGameDefinition::Parse4Pair(CString& sString1, CString& sString2, CString& sString3, CString& sString4)
{
	return (Parse3Pair(sString1, sString2, sString3) && VerifyGetToken(":") && Parse1Pair(sString4));
}


// Allocates an entity definition.
ENTITYDEFINITION* CGameDefinition::AllocateEntityDefinition()
{
	ENTITYDEFINITION* pEntDef;

	pEntDef = new ENTITYDEFINITION;
	if(!pEntDef)
	{
		ScriptError(NULL, "CGameDefinition::AllocateEntityDefinition(): Unable to allocate memory for entity definition");
		return NULL;
	}
	memset(pEntDef, 0, sizeof(pEntDef));
	return pEntDef;
}

// Allocates an entity key definition.
ENTITYKEYDEFINITION* CGameDefinition::AllocateEntityKeyDefinition()
{
	ENTITYKEYDEFINITION* pKeyDef;

	pKeyDef = new ENTITYKEYDEFINITION;
	if(!pKeyDef)
	{
		ScriptError(NULL, "CGameDefinition::AllocateEntityKeyDefinition(): Unable to allocate memory for key definition");
		return NULL;
	}
	memset(pKeyDef, 0, sizeof(pKeyDef));
	return pKeyDef;
}

// Allocates a flag definition.
ENTITYFLAGDEFINITION* CGameDefinition::AllocateEntityFlagDefinition()
{
	ENTITYFLAGDEFINITION* pFlagDef;

	pFlagDef = new ENTITYFLAGDEFINITION();
	if(!pFlagDef)
	{
		ScriptError(NULL, "CGameDefinition::AllocateEntityFlagDefinition(): Unable to allocate memory for flag definition");
		return NULL;
	}
	memset(pFlagDef, 0, sizeof(pFlagDef));
	return pFlagDef;
}

// Allocates an option definition.
ENTITYFLAGOPTIONDEFINITION* CGameDefinition::AllocateEntityOptionDefinition()
{
	ENTITYFLAGOPTIONDEFINITION* pOptionDef;

	pOptionDef = new ENTITYFLAGOPTIONDEFINITION();
	if(!pOptionDef)
	{
		ScriptError(NULL, "CGameDefinition::AllocateEntityFlagDefinition(): Unable to allocate memory for flag definition");
		return NULL;
	}
	memset(pOptionDef, 0, sizeof(pOptionDef));
	return pOptionDef;
}
