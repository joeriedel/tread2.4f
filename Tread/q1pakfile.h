// Q2PAKFILE.H
// Copyright © 1999 Joe Riedel, Nick Randal.
// Author: Nick Randal.

/*
10/2/99 - Nick Randal
	Fixed the offset of the textures because of bad file position.
*/

#ifndef Q1PAKFILE_H
#define Q1PAKFILE_H

#include "PakFile.h"
#include "Texture.h"
#include "Q1WalReader.h"

// Reads the Quake3(tm) pak file format. Quake3(tm) is © by id Software®, All Rights Reserved.

#define Q1PAKID 843333975
#define Q1FILEID 68

struct Q1FILEENTRY
{
	
	int		nPos;		//offset of the file
	int		nLength;	//lenght of the file
	int		nType;
	char	szName[16];
	int		nHandle;	//unique index
};

class CQ1PakFile : public CPakFile
{
protected:

	// File.
	CFile m_hFile;
	CString m_sFilename;

	// Size/Dir.
	int m_nNumFiles;
	Q1FILEENTRY* m_pDir;

	int m_nNumTexs;
	CTexture* m_pTexs;
	CTexture** m_pList;

	// Wal Reader.
	CQ1WalReader m_WalReader;

public:

	CQ1PakFile() : CPakFile()
	{
		m_nNumFiles = 0;
		m_pDir = NULL;
		m_pTexs = NULL;
		m_pList = NULL;
		m_WalReader.SetPakFile(this);
	}

	virtual ~CQ1PakFile()
	{
	}

	// Q2 Pak Id: 1262698832
	static bool ClaimFile(const char* szFilename);
	
	// Get the filename.
	virtual const char* GetFilename() { return m_sFilename; }

	// Attaches a file to this object & initializes the reader.
	virtual bool OpenAndInitialize(const char* szFileName);

	// Get's the list of texture's in the pak.
	virtual CTexture** GetTextureList(int& nNumTextures) const;

	// Seeks to the file's position.
	virtual bool SeekToFile(CTexture* pTex);

	//Get the files position
	int GetFileOffset(CTexture* pTex);
	
	// Get's the file object.
	virtual void* GetFileObject(CTexture* pTex, int nContext=0);

	// Gets a packages palette.
	virtual CTexPalette* GetPalette() const { return NULL; }

	// Closes the pak file.
	virtual bool Close();
};

#endif