// PAKLOADER.CPP
// Copyright © 1999 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#include "stdafx.h"
#include <memory.h>
#include "PakLoader.h"
#include "GameAPI.h"
#include "Q1PakFile.h"
#include "Q2PakFile.h"
#include "HLPakFile.h"
#include "Q2TexDir.h"
#include "pk3file.h"
#include "PluginAPI.h"

/*
12/15/99 - Joe Riedel
	- Added directory loading
*/

// Constructor.
CPakLoader::CPakLoader()
{ memset(m_pPakFiles, NULL, sizeof(m_pPakFiles)); }

// Finds a free slot.
int CPakLoader::FindAvailablePak()
{
	int i;

	for(i = 0; i < MAX_OPEN_PAKS; i++)
	{
		if(m_pPakFiles[i] == NULL)
			return i;
	}

	return -1;
}

// Destructor.
CPakLoader::~CPakLoader()
{
	CloseAllPaks();
}

CPakFile* CPakLoader::GetDirLoader(const char* szFilename)
{
	CString sType = GetGameAPI()->GetCurrentGame()->GetTextureTypeString();

	Debug("... querying plugin subsystem\n");

	// See if a plugin will load it.
	CPakFile* pPak = GetPluginAPI()->OpenTextureDirectory(sType);
	if(pPak != NULL)
	{
		Debug("... found plugin reader\n");
		return pPak;
	}

	Debug("... searching built-in readers\n");

	if(sType == "@Q2WAL" || sType == "@TARGA")
	{
		Debug("... Q2DIR reader selected\n");
		return new CQ2TexDir();
	}

	Debug("... no reader found!\n");

	return NULL;
}

// Opens and assigns a pak file.
CPakFile* CPakLoader::OpenAndAssignPak(const char* szFilename, bool bDir)
{
	int nPakIndex;
	CPakFile* pPak;

	nPakIndex = FindAvailablePak();
	if(nPakIndex == -1)
		return NULL;

	// Is it a dir?
	if(bDir)
	{
		pPak = GetDirLoader(szFilename);
		if(pPak == NULL)
			return NULL;
	}
	else
	{
		pPak = GetPakLoader(szFilename);
		if(pPak == NULL)
			return NULL;
	}

	// Load space.
	m_pPakFiles[nPakIndex] = pPak;

	return pPak;
}

// Get's the pak loader.
CPakFile* CPakLoader::GetPakLoader(const char* szFilename)
{
	Debug("... querying plugin subsystem\n");

	// See if a plugin will load it.
	CPakFile* pPak = GetPluginAPI()->OpenTexturePackage(szFilename);
	if(pPak != NULL)
	{
		Debug("... found plugin reader\n");
		return pPak;
	}

	Debug("... searching built-in readers\n");

	if(CQ1PakFile::ClaimFile(szFilename))
	{
		Debug("... QPAK reader selected\n");
		return new CQ1PakFile();
	}

	if(CQ2PakFile::ClaimFile(szFilename))
	{
		Debug("... Q2PAK reader selected\n");
		return new CQ2PakFile();
	}
	

	if(CHLPakFile::ClaimFile(szFilename))
	{
		Debug("... HLPAK reader selected\n");
		return new CHLPakFile();
	}

	if(CPk3File::ClaimFile(szFilename))
	{
		Debug("... Pk3 reader selected\n");
		return new CPk3File();
	}

	Debug("... no reader found!\n");

	return NULL;
}

// Get's a pakfile index.
int CPakLoader::GetPakIndex(CPakFile* pPakFile)
{
	int i;

	if(pPakFile == NULL)
		return -1;

	for(i = 0; i < MAX_OPEN_PAKS; i++)
	{
		if(m_pPakFiles[i] == pPakFile)
			return i;
	}

	return -1;
}

CPakFile* CPakLoader::GetPakFromIndex(int nPakIndex)
{
	if(nPakIndex < 0 || nPakIndex > MAX_OPEN_PAKS)
		return NULL;

	return m_pPakFiles[nPakIndex];
}


// Opens a pak file.
CPakFile* CPakLoader::OpenPak(const char* szFilename, bool bDir)
{
	CPakFile* pPak;

	pPak = OpenAndAssignPak(szFilename, bDir);
	if(pPak == NULL)
		return NULL;

	if(!pPak->OpenAndInitialize(szFilename))
	{
		ClosePak(pPak);
		return NULL;
	}

	return pPak;
}

// Closes a pak file.
bool CPakLoader::ClosePak(CPakFile* pPakFile)
{
	int i;

	i = GetPakIndex(pPakFile);
	if(i == -1)
		return false;

	// For some reason we weren't calling this.
	pPakFile->Close();

	delete pPakFile;
	m_pPakFiles[i] = NULL;
	return true;
}

// Closes all open pak files.
bool CPakLoader::CloseAllPaks()
{
	int i;

	for(i = 0; i < MAX_OPEN_PAKS; i++)
	{
		if(m_pPakFiles[i] == NULL)
			continue;
		if(!ClosePak(m_pPakFiles[i]))
			return false;
	}

	return true;
}