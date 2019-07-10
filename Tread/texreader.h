// TEXREADER.H
// Copyright © 1999 Joe Riedel, Nick Randal.
// Author: Joe Riedel.

/*
10/2/99 - Nick Randal
	- Added a function to get the file offset which was needed for halflife texture loading.
	- Changed some functions to pure virtual instead of a default failure in which we just
	returned false.
*/

#ifndef TEXREADER_H
#define TEXREADER_H

#include "PakFile.h"
#include "TExport.h"

class CTexture;

// $sb "texreader.h block1"
class _T_EXPORT CTexReader
{
private:

	CPakFile* m_pPakFile;

public:

	CTexReader() { m_pPakFile = NULL; }
	virtual ~CTexReader() {}

	void SetPakFile(CPakFile* pPakFile) { m_pPakFile = pPakFile; }
	CPakFile* GetPakFile() { return m_pPakFile; }
	void* GetFileObject(CTexture* pTex, int nContext=0)
	{
		if(m_pPakFile == NULL)
			return NULL;

		return m_pPakFile->GetFileObject(pTex, nContext);
	}
	void ReleaseFileObject(void* pFile, int nContext=0)
	{
		if(m_pPakFile == NULL)
			return;

		m_pPakFile->ReleaseFileObject(pFile, nContext);
	}
	bool SeekToFile(CTexture* pTex)
	{
		if(m_pPakFile == NULL)
			return false;

		return m_pPakFile->SeekToFile(pTex);
	}
	int GetFileOffset(CTexture* pTex)
	{
		if(m_pPakFile == NULL)
			return 0;

		return m_pPakFile->GetFileOffset(pTex);
	}

	// Returns the RF_ flags based on surface flags.
	virtual int TranslateSurfaceToRenderFlags(int nFlags) { return 0; }

	// Loads the texture.
	// After the call pTex object should be a 24 bit color image.
	virtual bool LoadTexture(CTexture* pTex) = 0;

	// Get's the texture information.
	virtual bool LoadTextureInfo(CTexture* pTex) = 0;

	// Called to free a texture.
	virtual bool FreeTexture(CTexture* pTex);
	virtual bool FreeTextureData(CTexture* pTex);

};
// $eb
#endif
