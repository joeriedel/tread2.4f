// TEXDIR.H
// Copyright © 1999 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#ifndef TEXDIR_H
#define TEXDIR_H

#include "pakfile.h"
#include "TExport.h"

// $sb "texdir.h block1"
// Parses a texture directory.
class _T_EXPORT CTexDirectory : public CPakFile
{
public:

	CTexDirectory() : CPakFile() {}
	virtual ~CTexDirectory() {}

	// Get the filename.
	const char* GetFilename() { return "Texture Directory"; }

	// Attaches a file to this object & initializes the reader.
	virtual bool OpenAndInitialize(const char* szFileName) = 0;

	// Get's the list of texture's in the pak.
	virtual CTexture** GetTextureList(int& nNumTextures) const = 0;

	// Get's the file object.
	virtual void* GetFileObject(CTexture* pTex, int nContext) = 0;
	virtual void ReleaseFileObject(void* pFile, int nContext) = 0;

	// Closes the pak file.
	virtual bool Close() = 0;

	virtual bool BuildPak() { return true; }
	
	// Seeks to the file's position.
	bool SeekToFile(CTexture* pTex) { return true; }

	// Get the file's position
	int GetFileOffset(CTexture* pTex) { return 0; }
	
	// Gets a packages palette.
	virtual CTexPalette* GetPalette() const { return NULL; }
};
// $eb
#endif