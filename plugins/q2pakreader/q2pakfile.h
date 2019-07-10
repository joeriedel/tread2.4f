// Q2PAKFILE.H
// Copyright © 1999 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#ifndef Q2PAKFILE_H
#define Q2PAKFILE_H

#include "../TreadAPI.h"
#include "Q2WalReader.h"

// Reads the Quake2(tm) pak file format. Quake2(tm) is © by id Software®, All Rights Reserved.
// The job of a PakFile object is to read a texture package and make available a list of Texture
// for Tread to use.

// Hehe, I'm documenting Nick's code so bare with me :)
// Actually most of this code is from 1.12, but we ported it over. What can I say? How
// do you improve the code that reads a Q2 pak file?

// The Id of the Quake2 pak file:
#define Q2PAKID 1262698832

#define WAL 1
#define TGA 2

// The Quake2 pak file contains a list of these:
struct Q2FILEENTRY
{
	char	szName[56];	//name of the file
	int		nPos;		//offset of the file
	int		nLength;	//lenght of the file
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

	// Wal Reader.
	CTexReader* m_pReader;
	//CQ2WalReader m_Q2WalReader;

public:
	CQ2PakFile();
	~CQ2PakFile();

	// Q2 Pak Id: 1262698832
	static bool ClaimFile(const char* szFilename);

	// The PakClass is the class id of the PakFile object. This
	// allows other packages to find out types during the BuildPak stage.
	virtual int GetPakClass() { return PAK_CLASS_QUAKE2; }

	// Get the filename.
	virtual const char* GetFilename() { return m_sFilename; }

	// Attaches a file to this object & initializes the reader.
	virtual bool OpenAndInitialize(const char* szFileName);

	// Get's the list of texture's in the pak.
	virtual CTexture* GetTextureList(int& nNumTextures) const;

	// The system requestes the PakFile object to seek to a particular
	// texture by calling this.
	virtual bool SeekToFile(CTexture* pTex);

	// Returns the file object of the specified texture.
	virtual void* GetFileObject(CTexture* pTex, int nContext=0);

	// If the pak has a texture, it should be returned through this.
	virtual CTexPalette* GetPalette() const { return NULL; }

	// Closes the pak file.
	virtual bool Close();
};

#endif