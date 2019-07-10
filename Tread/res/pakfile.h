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

// Name holder.
class CTexture;

#define PAK_CLASS_QUAKE3		0x00000001
#define PAK_CLASS_QUAKE3DIR		0x00000002
#define PAK_CLASS_QUAKE2		0x00000003
#define PAK_CLASS_QUAKE2DIR		0x00000004
#define PAK_CLASS_QUAKE			0x00000005
#define PAK_CLASS_QUAKEDIR		0x00000006
#define PAK_CLASS_HALFLIFE		0x00000007
#define PAK_CLASS_HALFLIFEDIR	0x00000008
#define PAK_CLASS_VFS			0x00000009

//////////////////////////////////////////////////////////////////
// Description: Base Class For Pak File loading/reading.		//
//////////////////////////////////////////////////////////////////
class CPakFile
{

public:

	// Constructor.
	CPakFile() {}
	// Destructor.
	virtual ~CPakFile() {}

	virtual int GetPakClass() = 0;

	// Get the filename.
	virtual const char* GetFilename() = 0;

	// Derived class types should return TRUE on the file
	// type codes.
	/*static*/ virtual bool ClaimFile(const char* szFilename) { return false; }

	// Attaches a file to this object & initializes the reader.
	virtual bool OpenAndInitialize(const char* szFileName) = 0;

	// Get's the list of texture's in the pak.
	virtual CTexture* GetTextureList(int& nNumTextures) const  = 0;

	// Seeks to the file's position.
	virtual bool SeekToFile(CTexture* pTex) = 0;

	// Get's a files offset.
	virtual int GetFileOffset(CTexture* pTex) { return 0; }

	// Get's the file object.
	virtual void* GetFileObject(CTexture* pTex) = 0;
	virtual void ReleaseFileObject(void* pFile) {}

	// Gets a packages palette.
	virtual CTexPalette* GetPalette() { return NULL; }

	// Closes the pak file.
	virtual bool Close() = 0;

	virtual bool BuildPak() { return true; }

	// Free's the list of textures.
	// NOTE: Do NOT override.
	void FreeTextureList(CTexture** pTexList) const
	{
		if(pTexList != NULL)
			delete[] pTexList;
	}
};

#endif