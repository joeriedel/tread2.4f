// HLPAKFILE.H
// Copyright © 1999 Joe Riedel, Nick Randal.
// Author: Nick Randal.
#ifndef HLPAKFILE_H
#define HLPAKFILE_H

#include "PakFile.h"
#include "Texture.h"
#include "HLWalReader.h"

// Reads the Quake3(tm) pak file format. Quake3(tm) is © by id Software®, All Rights Reserved.

#define HLPAKID 860111191
#define HLFILEID 67

struct HLFILEENTRY
{
	
	int		nPos;		//offset of the file
	int		nLength;	//lenght of the file
	int		nType;
	char	szName[16];
	int		nHandle;	//unique index
};

class CHLPakFile : public CPakFile
{
protected:

	// File.
	CFile m_hFile;
	CString m_sFilename;

	// Size/Dir.
	int m_nNumFiles;
	HLFILEENTRY* m_pDir;

	int m_nNumTexs;
	CTexture* m_pTexs;
	CTexture** m_pList;

	// Wal Reader.
	CHLWalReader m_WalReader;

public:

	CHLPakFile() : CPakFile()
	{
		m_nNumFiles = 0;
		m_pDir = NULL;
		m_pTexs = NULL;
		m_pList = NULL;
		m_WalReader.SetPakFile(this);
	}

	~CHLPakFile()
	{
	}

	// Q2 Pak Id: 1262698832
	static bool ClaimFile(const char* szFilename);

	// Get the filename.
	virtual const char* GetFilename() { return m_sFilename; }

	// Attaches a file to this object & initializes the reader.
	bool OpenAndInitialize(const char* szFileName);

	// Get's the list of texture's in the pak.
	CTexture** GetTextureList(int& nNumTextures) const;

	// Seeks to the file's position.
	bool SeekToFile(CTexture* pTex);

	//Get the files position
	int GetFileOffset(CTexture* pTex);
	
	// Get's the file object.
	void* GetFileObject(CTexture* pTex, int nContext=0);

	// Gets a packages palette.
	CTexPalette* GetPalette() const { return NULL; }

	// Closes the pak file.
	bool Close();
};

#endif