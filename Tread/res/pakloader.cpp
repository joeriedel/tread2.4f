// PAKLOADER.CPP
// Copyright © 1999 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#include "stdafx.h"
#include <memory.h>
#include "PakLoader.h"
#include "GameAPI.h"
#include "Q1PakFile.h"
#include "Q2PakFile.h"
#include "Q3PakFile.h"
#include "HLPakFile.h"
#include "Q3TexDir.h"
#include "vfspak.h"

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
	if(GetGameAPI()->GetCurrentGame()->GetTextureTypeString() == "@Q3PAK")
		return new CQ3TexDir();

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
	if(CVFSPakFile::ClaimFile(szFilename))
		return new CVFSPakFile();

	if(CQ1PakFile::ClaimFile(szFilename))
		return new CQ1PakFile();

	if(CQ2PakFile::ClaimFile(szFilename))
		return new CQ2PakFile();
	
	if(CQ3PakFile::ClaimFile(szFilename))
		return new CQ3PakFile();

	if(CHLPakFile::ClaimFile(szFilename))
		return new CHLPakFile();

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