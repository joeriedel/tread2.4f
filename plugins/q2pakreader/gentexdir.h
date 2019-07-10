// GENTEXDIR.H
// Copyright © 2000 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#ifndef GENTEXDIR_H
#define GENTEXDIR_H

//
// This general purpose directory loading class is here to help automate the process of loading a texture directory.
// The process looks like this:
//
// For every texture:
//	-- can we read it?
//  -- YES: keep it.
//  -- NO:  discard it.
// End for every texture.
//
// This class allows inheritants of the class to specify search filters and return pass or fail on a per file basis.
//

#include "../TreadAPI.h"

#define MAX_GENDIR_FILTERS	2

class CGenTexDirectory : public CTexDirectory
{
	int m_nNumTextures;
	CTexture* m_pTextures;

public:

	CGenTexDirectory() {}
	virtual ~CGenTexDirectory();

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