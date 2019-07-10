// Q2PAKFILE.H
// Copyright © 1999 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#ifndef Q2PAKFILE_H
#define Q2PAKFILE_H

#include "PakFile.h"
#include "Texture.h"
#include "Q2WalReader.h"
#include "SofReader.h"
#include "TexReader.h"

// Reads the Quake2(tm) pak file format. Quake2(tm) is © by id Software®, All Rights Reserved.

#define Q2PAKID 1262698832

#define WAL 1
#define TGA 2
#define M32	3
#define DAK 4

// Q2 Size == 64
// DK Size == 72
struct Q2FILEENTRY
{
	char	szName[56];	//name of the file
	int		nPos;		//offset of the file
	int		nLength;	//lenght of the file
	int		nCompSize, nCompressed;
	int		nHandle;	//unique index
};

class CQ2PakFile : public CPakFile
{
protected:
	// File.
	CFile m_File;
	CString m_sFilename;

	// Size/Dir.
	int m_nNumFiles;
	int m_nType;
	Q2FILEENTRY* m_pDir;

	int m_nNumTexs;
	CTexture* m_pTexs;
	CTexture** m_pList;

	// Wal Reader.
	CTexReader* m_pReader;
	//CQ2WalReader m_Q2WalReader;

public:
	CQ2PakFile();
	~CQ2PakFile();

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

	// Get's the file object.
	virtual void* GetFileObject(CTexture* pTex, int nContext=0);

	// Gets a packages palette.
	virtual CTexPalette* GetPalette() const { return NULL; }

	// Closes the pak file.
	virtual bool Close();
};

#endif