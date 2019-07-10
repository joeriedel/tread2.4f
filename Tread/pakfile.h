// PAKFILE.H
// Copyright © 1999 Joe Riedel, Nick Randal.
// Author: Joe Riedel.

/*
10/2/99 - Nick Randal
	- Added a function to get the file offset which was needed for halflife texture loading.
	- Changed some functions to pure virtual instead of a default failure in which we just
	returned false.
*/

#ifndef PAKFILE_H
#define PAKFILE_H

#include "TexPalette.h"
#include "TExport.h"

// Name holder.
class CTexture;

// $sb "pakfile.h block1"
//////////////////////////////////////////////////////////////////
// Description: Base Class For Pak File loading/reading.		//
//////////////////////////////////////////////////////////////////
class _T_EXPORT CPakFile
{

public:

	// Constructor.
	CPakFile() {}
	// Destructor.
	virtual ~CPakFile() {}

	// Get the filename.
	virtual const char* GetFilename() = 0;

	// Derived class types should return TRUE on the file
	// type codes.
	/*static*/ virtual bool ClaimFile(const char* szFilename) { return false; }

	// Attaches a file to this object & initializes the reader.
	virtual bool OpenAndInitialize(const char* szFileName) = 0;

	// Get's the list of texture's in the pak.
	virtual CTexture** GetTextureList(int& nNumTextures) const  = 0;

	// Seeks to the file's position.
	virtual bool SeekToFile(CTexture* pTex) = 0;

	// Get's a files offset.
	virtual int GetFileOffset(CTexture* pTex) { return 0; }

	// Get's the file object.
	virtual void* GetFileObject(CTexture* pTex, int nContext=0) = 0;
	virtual void ReleaseFileObject(void* pFile, int nContext=0) {}

	// Gets a packages palette.
	virtual CTexPalette* GetPalette() { return NULL; }

	// Closes the pak file.
	virtual bool Close() = 0;

	virtual bool BuildPak() { return true; }
	virtual bool PakBuildsComplete() { return true; }

	// Free's the list of textures.
	// NOTE: Do NOT override.
	void FreeTextureList(CTexture** pTexList) const
	{
		if(pTexList != NULL)
			delete[] pTexList;
	}
};

// $eb

#endif