// MAPBUILD.CPP
// Copyright © 1999 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#include "stdafx.h"
#include "GameAPI.h"
#include "MapBuild.h"
#include "TreadSys.h"
#include "Tread3D.h"
#include <process.h>

struct SPAWNINFO
{
	int* pnThreadCount;
	DEFMODULE* pModule;
	RUNPROGRAMCMD* pProg;
	RUNBLOCK* pRunBlock;
	CString sCmdLine;
};

int gNumProc=0;

UINT SpawnThread(LPVOID pVoid);
bool Spawn(RUNPROGRAMCMD* pProg, DEFMODULE* pModule, CString sCmdLine, RUNBLOCK* pRunBlock);
bool ExecuteModule(DEFMODULE* pModule, RUNBLOCK* pRunBlock, RUNSTATE* pRunState);
bool ResolveProgram(RUNPROGRAMCMD* pProg, RUNBLOCK* pRunBlock);

// Runs a build process.
bool RunConsoleProcess(const char* exe, const char* cmdline, const char* wkdir);
// Runs a non-console process.
bool RunProcess(const char* exe, const char* cmdline, const char* wkdir);

// Runs a non-console process.
bool RunProcess(const char* exe, const char* cmdline, const char* wkdir)
{
	BOOL bProc;
	DWORD status;
	PROCESS_INFORMATION pInfo;
	STARTUPINFO sInfo;

	memset(&sInfo, 0, sizeof(STARTUPINFO));
	sInfo.cb = sizeof(STARTUPINFO);

	bProc = CreateProcess(exe, (char*)cmdline, NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS, 
		NULL, wkdir, &sInfo, &pInfo);

	if(!bProc)
	{
		Warning("Unable to run process!\n");
		return(false);
	}

	for(;;)
	{
		GetExitCodeProcess(pInfo.hProcess, &status);
		if(status != STILL_ACTIVE) // Nope, die.
			break;

		Sleep(1000);
	}

	return(true);
}

// Runs a build process.
bool RunConsoleProcess(const char* exe, const char* cmdline, const char* wkdir)
{
	char data[257], filename[256];
	BOOL bProc, bOk;
	HANDLE hReadFile, hWriteFile;
	DWORD status;
	PROCESS_INFORMATION pInfo;
	STARTUPINFO sInfo;
	DWORD nBytesRead;
	SECURITY_ATTRIBUTES sa;

	sprintf(filename, "%s\\bp%i.txt", GetRegString(S_TREAD_PATH), gNumProc++);

	sa.nLength = sizeof(sa);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle = true;

	/*hWriteFile = CreateFile(filename, GENERIC_WRITE, FILE_SHARE_READ|FILE_SHARE_DELETE, &sa, 
		CREATE_ALWAYS, FILE_ATTRIBUTE_TEMPORARY|FILE_FLAG_DELETE_ON_CLOSE*, NULL);

	hReadFile = CreateFile(filename, GENERIC_READ, FILE_SHARE_WRITE|FILE_SHARE_DELETE, &sa, 
		OPEN_EXISTING, FILE_ATTRIBUTE_TEMPORARY|FILEFLAG_DELETE_ON_CLOSE, NULL);*/

	hWriteFile = CreateFile(filename, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, &sa, 
		CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL , NULL);

	hReadFile = CreateFile(filename, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, &sa, 
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if(hWriteFile == INVALID_HANDLE_VALUE || hReadFile == INVALID_HANDLE_VALUE)
	{
		CloseHandle(hWriteFile);
		CloseHandle(hReadFile);
		DeleteFile(filename);
		return(false);
	}
	
	memset(&sInfo, 0, sizeof(STARTUPINFO));
	sInfo.cb = sizeof(STARTUPINFO);
	sInfo.dwFlags = STARTF_USESTDHANDLES;
	sInfo.hStdError  = GetStdHandle(STD_ERROR_HANDLE);
	sInfo.hStdInput  = GetStdHandle(STD_INPUT_HANDLE);
	sInfo.hStdOutput = hWriteFile;

	bProc = CreateProcess(exe, (char*)cmdline, NULL, NULL, TRUE, DETACHED_PROCESS | NORMAL_PRIORITY_CLASS, 
		NULL, wkdir, &sInfo, &pInfo);

	if(!bProc)
	{
		Warning("Unable to run process!\n");
		CloseHandle(hWriteFile);
		CloseHandle(hReadFile);
		return(false);
	}

	for(;;)
	{
		// Read.
		memset(data, 0, 256);
		FlushFileBuffers(hWriteFile);
		bOk = ReadFile(hReadFile, data, 256, &nBytesRead, NULL);
		if(bOk && nBytesRead == 0)
		{
			// EOF, Process running?
			GetExitCodeProcess(pInfo.hProcess, &status);
			if(status != STILL_ACTIVE) // Nope, die.
				break;

			Sleep(200);
			continue;
		}
		
		// Print out what we got.
		data[nBytesRead] = '\0';
		Console(data);
	}

	CloseHandle(hWriteFile);
	CloseHandle(hReadFile);
	DeleteFile(filename);
	return(true);
}

// Spawns a process and prints output to the console.
bool Spawn(RUNPROGRAMCMD* pProg, DEFMODULE* pModule, CString sCmdLine, RUNBLOCK* pRunBlock)
{
	CString sPath;

	// Find the module to run.
	if(pProg->sModule == "@THIS")
		sPath = pModule->sPath;
	else
	{
		DEFMODULE* pMod = pRunBlock->pGameDef->GetDefModule(pProg->sModule);
		if(pMod == NULL)
		{
			Error("null module!\n");
			return false;
		}
		else if(pMod->nType == MODULE_TYPE_RUNNABLE)
		{
			RUNSTATE* pTempState = pRunBlock->pDocument->GetRunState(pProg->sModule);
			if(pTempState == NULL)
			{
				Error("module without state!\n");
				return false;
			}

			sPath = pTempState->sValue;
		}
		else
			sPath = pMod->sPath;
	}

	Console("spawn: "  + sPath + " " + sCmdLine + "\n");

	if(pProg->bConsole)
		//RunConsoleProcess("\"" + pModule->sPath + "\"", "\"" + pModule->sPath + "\" " + sCmdLine, pProg->sWorkDir);
		RunConsoleProcess(sPath, sPath + " " + sCmdLine, pProg->sWorkDir);
	else
		RunProcess(sPath, sPath + " " + sCmdLine, pProg->sWorkDir);

	return true;
}

// Spawns the spawn function.
UINT SpawnThread(LPVOID pVoid)
{
	SPAWNINFO* pInfo = (SPAWNINFO*)pVoid;
	*(pInfo->pnThreadCount)++;
	Spawn(pInfo->pProg, pInfo->pModule, pInfo->sCmdLine, pInfo->pRunBlock);
	*(pInfo->pnThreadCount)--;
	delete pInfo;

	return 0;
}

// Resolve's a program.
bool ResolveProgram(RUNPROGRAMCMD* pProg, RUNBLOCK* pRunBlock)
{
	RUNSTATE* pState;

	// If it's not runnable, then we must look it up globally.
	DEFMODULE* pMod = pRunBlock->pGameDef->GetDefModule(pProg->sWorkDirAlias);
	if(pMod != NULL)
	{
		if(pMod->nType == MODULE_TYPE_RUNNABLE)
		{
			pState = pRunBlock->pDocument->GetRunState(pProg->sWorkDirAlias);
			if(pState != NULL)
				pProg->sWorkDir = pState->sValue;
			else
			{
				Error("No state for runnable module\n");
				return false;
			}
		}
		else
			pProg->sWorkDir = pMod->sPath;
	}
	else
		pProg->sWorkDir = pProg->sWorkDirAlias;

	return true;
}

// Executes an eval state.
bool ExecuteModule(DEFMODULE* pModule, RUNBLOCK* pRunBlock, RUNSTATE* pRunState)
{
	CString sCmdLine="";
	DEFPARAM* pDefParam = pModule->pDefParam;
	DEFPARAMPARAM* pParam;
	bool bAbort=false;
	
	if(pDefParam != NULL)
	{

		pDefParam->pParamList->First();
		for(pParam = pDefParam->pParamList->GetCurItem(); pParam != NULL; pParam = pDefParam->pParamList->GetNextItem())
		{
			// An always eval?
			if(pParam->bAlways == true || (pParam->nValue&pRunState->nValue) == pParam->nValue)
			{
				if(pParam->sParam == SCRIPT_ABORT_MODULE)
				{
					bAbort=true;
					break;
				}
				
				// If it's not runnable, then we must look it up globally.
				DEFMODULE* pMod = pRunBlock->pGameDef->GetDefModule(pParam->sParam);
				if(pMod != NULL)
				{
					if(pMod->nType == MODULE_TYPE_RUNNABLE)
					{
						RUNSTATE* pTempState = pRunBlock->pDocument->GetRunState(pParam->sParam);
						if(pTempState != NULL)
							sCmdLine += pTempState->sValue;
						else
						{
							Error("No state for a runnable module!\n");
							bAbort=true;
							break;
						}
					}
					else
						sCmdLine += pMod->sPath;
				}		
				else if(pParam->sParam == SCRIPT_MAP_FILE)
					sCmdLine += pRunBlock->sTreadFile;
				else
					sCmdLine += pParam->sParam;
			}
		}

		if(bAbort)
		{
			Console("[ '" + pModule->sName + "' skipped due to user request ]\n");
			return true;
		}

		// Strip the cmdline of pre/post spaces.
		sCmdLine.TrimLeft(); sCmdLine.TrimRight();
		pRunState->sValue = sCmdLine;
	}
	
	// Run exports.
	EXPORTMAPCMD* pExport;

	pModule->pExportList->First();
	for(pExport = pModule->pExportList->GetCurItem(); pExport != NULL; pExport = pModule->pExportList->GetNextItem())
	{
		// Export the map.
		RUNSTATE* pTempState = pRunBlock->pDocument->GetRunState(pExport->sOutName);
		if(pTempState != NULL)
		{
			Console("[ " + pExport->sOutput + " ]\n");
			Console("exporting map to: " + pTempState->sValue + "\n");
			if(!pRunBlock->pDocument->ExportMap(pTempState->sValue))
			{
				Console("$ExportMap failed!\n");
				return false;
			}
		}
		else
		{
			Console("[ " + pExport->sOutput + " ]\n");
			Console("exporting map to: " + pExport->sOutName + "\n");
			if(!pRunBlock->pDocument->ExportMap(pExport->sOutName))
			{
				Console("$ExportMap failed!\n");
				return false;
			}
		}
	}

	int nThreadCount=0;
	RUNPROGRAMCMD* pProg;
	SPAWNINFO* pInfo;

	pModule->pProgramList->First();
	for(pProg = pModule->pProgramList->GetCurItem(); pProg != NULL; pProg = pModule->pProgramList->GetNextItem())
	{
		Console("[ " + pProg->sOutput + " ]\n");

		// Resolve the programs args.
		ResolveProgram(pProg, pRunBlock);

		if(pProg->bWait)
			Spawn(pProg, pModule, pRunState->sValue + " " + pRunState->sCmdLine, pRunBlock);
		else
		{

			// New spawn block.
			pInfo = new SPAWNINFO();
			pInfo->pProg = pProg;
			pInfo->pModule = pModule;
			pInfo->sCmdLine = pRunState->sValue + " " + pRunState->sCmdLine;
			pInfo->pRunBlock = pRunBlock;
			pInfo->pnThreadCount = &nThreadCount;
			
			AfxBeginThread(SpawnThread, pInfo);
		}
	}

	// Wait for all threads to exit.
	while(nThreadCount > 0)
	{ Sleep(1500); }

	return true;
}

UINT MapBuildThread(LPVOID pVoid)
{
	RUNSTATE* pState;
	DEFMODULE* pModule;
	MAPBUILDBLOCK* pBlock = (MAPBUILDBLOCK*)pVoid;

	pBlock->pDefBuild->pModuleList->First();
	for(pModule = pBlock->pDefBuild->pModuleList->GetCurItem(); pModule != NULL; pModule = pBlock->pDefBuild->pModuleList->GetNextItem())
	{
		if(pModule->nType == MODULE_TYPE_NOTRUNNABLE)
			continue;
		pState = pBlock->pRunBlock->pDocument->GetRunState(pModule->sName);
		if(pState == NULL)
		{
			Warning("Error: Missing runstate for '" + pModule->sName + "'\n");
			continue;
		}
		if(!ExecuteModule(pModule, pBlock->pRunBlock, pState))
		{
			Console("----------------------------------------\n");
			Console("Fatal error encountered.\n");
			break;
		}
		//Console("--" + pEval->m_sName + ": ended.");
		Console("----------------------------------------\n");

		// Restore location.
		pBlock->pDefBuild->pModuleList->SetPosition(pModule);
	}

	Console("Build Completed.\n");
	Console("----------------------------------------\n");

	// Kill blocks.
	delete pBlock->pRunBlock;
	delete pBlock;


	return 0;
}
