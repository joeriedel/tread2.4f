// GENTEXDIRFP.H
// Copyright © 2000 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#ifndef GENTEXDIRFP_H
#define GENTEXDIRFP_H

// This version uses FILE*.

#include "TexDir.h"

#define MAX_GENDIR_FILTERS	2

class CGenTexDirectoryFp : public CTexDirectory
{
	int m_nNumTextures;
	CTexture* m_pTextures;

public:

	CGenTexDirectoryFp() {}
	virtual ~CGenTexDirectoryFp();

	// Return true to skip.
	virtual bool SkipFile(CString sFilename, CString* pFilter) { return false; }
	virtual void ProcessTexture(CTexture* pTexture, CString* pFilter) = 0;
	virtual bool GetFileFilters(int& nNumFilters, CString* pFilters) = 0;

	// Attaches a file to this object & initializes the reader.
	virtual bool OpenAndInitialize(const char* szFileName);

	// Get's the list of texture's in the pak.
	virtual CTexture* GetTextureList(int& nNumTextures) const;

	// Get's the file object.
	virtual void* GetFileObject(CTexture* pTex, int nContext=0);
	virtual void ReleaseFileObject(void* pFile, int nContext=0);

	// Closes the pak file.
	virtual bool Close() { return true; }
	virtual int GetPakClass() = 0;
};

#endif