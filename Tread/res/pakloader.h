// PAKLOADER.H
// Copyright © 1999 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#ifndef PAKLOADER_H
#define PAKLOADER_H

#include "PakFile.h"
#include "TexDir.h"

/*
12/15/99 - Joe Riedel
	- Added directory loading
*/

// Loads a pak file.
#define MAX_OPEN_PAKS	64

class CPakLoader
{
private:

	CPakFile* m_pPakFiles[MAX_OPEN_PAKS];
	CPakFile* OpenAndAssignPak(const char* szFilename, bool bDir = false);
	int FindAvailablePak();
	CPakFile* GetPakLoader(const char* szFilename);
	CPakFile* GetDirLoader(const char* szFilename);

public:

	CPakLoader();
	virtual ~CPakLoader();

	// Get's a pakfile index.
	int GetPakIndex(CPakFile* pPak);
	CPakFile* GetPakFromIndex(int nPakIndex);

	// Opens a pak file.
	CPakFile* OpenPak(const char* szFilename, bool bDir = false);
	bool ClosePak(CPakFile* pPakFile);
	bool CloseAllPaks();
};

#endif