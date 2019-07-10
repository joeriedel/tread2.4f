// PK3FILE.H
// Copyright © 2000 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#ifndef PK3FILE_H
#define PK3FILE_H

#include "q3pakbase.h"
#include "pakfile.h"
#include "unzip.h"	// Modified unzip lib from Q3Radiant, special thx to Robert Duffy.
#include "qshaders.h"
#include "targastream.h"
#include "jpegreader.h"

typedef struct
{
	CString sName;
	int nSize;
	unz_s zInfo;
	unzFile zFile;

} UNZENTRY;

#define PK3_IGNORE		0x00000000
#define PK3_TEXTURE		0x00000001
#define PK3_SHADER		0x00000002

class CPk3File : public CPakFile, public CQ3PakBase
{
protected:

	unzFile m_unzFile;
	CString m_sFilename;
	int m_nNumFiles;
	UNZENTRY* m_pUnzFiles;

	int m_nNumTextures;
	CQ3Texture* m_pTextures;
	CTexture** m_pTexList;

	int m_nAllTextures;
	CQ3Texture* m_pAllTextures;

	int m_nNumShaders;
	QSHADER* m_pShaders;

	CTargaStreamReader m_TargaReader;
	CJPEGReader m_JPGReader;

	virtual int ClassifyFile(CString sFilename);
	//virtual bool BuildTextureList(void);
	virtual bool ParseImage(CString sFilename, UNZENTRY* pEntry, int nHandle, CQ3Texture* pTexture);
	virtual int ParseShader(CString sFilename, UNZENTRY* pEntry, int nHandle, QSHADER* pList);
	virtual int CountShader(CString sFilename, UNZENTRY* pEnty);
	virtual void FormatName(CString sName, CString& sFormatted);
	bool LoadFile(UNZENTRY* pEntry, unsigned char** pBuffer,  int nLength);
	virtual CQ3Texture* FindTexture(CString sFilename);
	virtual CQ3Texture* FindTextureInAllPaks(CString sFilename);

public:

	CPk3File();
	virtual ~CPk3File();

	// Get the filename.
	virtual const char* GetFilename() { return m_sFilename; }

	// Derived class types should return TRUE on the file
	// type codes.
	static bool ClaimFile(const char* szFilename);

	// Attaches a file to this object & initializes the reader.
	virtual bool OpenAndInitialize(const char* szFileName);

	// Get's the list of texture's in the pak.
	virtual CTexture** GetTextureList(int& nNumTextures) const;

	// Get's all the textures.
	virtual CQ3Texture* GetAllTextures(int& nNumTextures) const;

	// Seeks to the file's position.
	virtual bool SeekToFile(CTexture* pTex) { return true; }

	// Get's a files offset.
	virtual int GetFileOffset(CTexture* pTex) { return 0; }

	// Get's the file object.
	virtual void* GetFileObject(CTexture* pTex, int nContext=0);
	virtual void ReleaseFileObject(void* pFile, int nContext=0);

	// Gets a packages palette.
	virtual CTexPalette* GetPalette() { return NULL; }

	virtual bool BuildPak();
	virtual bool PakBuildsComplete();

	// Closes the pak file.
	virtual bool Close();
};

#endif