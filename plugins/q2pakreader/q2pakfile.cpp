// Q2PAKFILE.CPP
// Copyright © 1999 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#include "stdafx.h"
#include "../TreadAPI.h"
#include "../szstring.h"
#include "Q2PakFile.h"
#include "Q2WalReader.h"
#include "TargaReader.h"

CQ2PakFile::CQ2PakFile()
{
	m_nNumFiles = 0;
	m_pDir = NULL;
	m_pTexs = NULL;
	m_pReader = NULL;
}

CQ2PakFile::~CQ2PakFile()
{
	Close();
	if(m_pReader != NULL)
	{
		delete m_pReader;
		m_pReader = NULL;
	}
}

//
// FormatReaders call this to obtain the file object of their pakfile.
// This enables them to read data :)
//
void* CQ2PakFile::GetFileObject(CTexture* pTex, int nContext)
{ return &m_File; }

//
// This function is not required by any means, but we typically add
// a static function called ClaimFile. It allows us to easily go through
// lists of PakFile objects and see if they can read the file.
//
bool CQ2PakFile::ClaimFile(const char* szFilename)
{
	CFile hFile;

	// Open the file.
	if(!hFile.Open(szFilename, CFile::modeRead | CFile::typeBinary | CFile::shareDenyNone))
		return false;

	int nFileType;
	hFile.Read(&nFileType, 4);
	hFile.Close();
	return nFileType == Q2PAKID;
}

//
// A FormatReader will typically request that the PakFile
// object locate the file pointer at the begining of the texture
// being read. This function should set the file pointer to the
// starting location of the requested file.
//
bool CQ2PakFile::SeekToFile(CTexture* pTex)
{
	// Ensure a valid indice.
	if(pTex->m_nHandle < 0 || pTex->m_nHandle >= m_nNumFiles)
		return false;

	// Seek to position.
	m_File.Seek(m_pDir[pTex->m_nHandle].nPos, CFile::begin);
	return true;
}

//
// Tread calls this to obtain a list of textures parsed by the PakFile
// object. You are still responsible to delete this list when you
// are deleted.
//
CTexture* CQ2PakFile::GetTextureList(int& nNumTextures) const
{ nNumTextures = m_nNumTexs; return m_pTexs; }

//
// Called by Tread to close down the PakFile.
//
bool CQ2PakFile::Close()
{
	m_File.Close();
	m_nNumFiles = 0;
	if(m_pDir != NULL)
		delete[] m_pDir;
	m_pDir = NULL;
	if(m_pTexs != NULL)
		delete[] m_pTexs;
	m_pTexs = NULL;
	return true;
}

//
// Called to open the requested file. The actually verification of
// format is done on the plugins calls: OpenTexturePackage() and OpenTextureDirectory().
// Once you return the object from those rouines Tread will proceed to call
// this routine to open the package and parse out the textures.
//
bool CQ2PakFile::OpenAndInitialize(const char* szFileName)
{
	int nDirOffset, nDirLength;

	m_sFilename = szFileName;

	// First we need to determine the format of the textures in this
	// .pak file. This implementation only supports one type per package.
	CGameDefinition* pGame = GetGameAPI()->GetCurrentGame();

	if(pGame->GetTextureTypeString() == "@TARGA")
	{
		m_pReader = new CTargaReader();
		m_nType = TGA;
	}
	else
	{
		m_pReader = new CQ2WalReader();
		m_nType = WAL;
	}

	// Attach ourselves to the reader so it can perform its
	// file requests.
	m_pReader->SetPakFile(this);

	// Open the file.
	if(!m_File.Open(szFileName, CFile::modeRead | CFile::typeBinary | CFile::shareDenyNone))
		return false;

	// Double check file type.
	int nId;
	if(!m_File.Read(&nId, 4))
		return false;

	if(nId != Q2PAKID)
		return false;

	// Read # of files and dir location.
	if(!m_File.Read(&nDirOffset, 4))
		return false;
	if(!m_File.Read(&nDirLength, 4))
		return false;

	m_File.Seek(nDirOffset, CFile::begin);
	m_nNumFiles = nDirLength / 64;			//calculate the number of entries

	// Allocate directory size.
	m_pDir = new Q2FILEENTRY[m_nNumFiles];
	if(m_pDir == NULL)
		return false;

	// Read it.
	int i;
	char sExt[4];

	if(m_nType == TGA)
		strcpy(sExt, "tga\0");
	else
		strcpy(sExt, "wal\0");

	m_nNumTexs = 0;
	for(i = 0; i < m_nNumFiles; i++)
	{
		// Read the dir entry.
		if(!m_File.Read(&m_pDir[i], 64))
			return false;
		
		// Make the name lower case.
		StrLower(m_pDir[i].szName);
		m_pDir[i].nHandle = -1;

		// Is this a Q2 .wal file?
		if(CompareExt(m_pDir[i].szName, sExt) && (m_pDir[i].szName[0] == 't'))
		{
			// Filter animated texture names. Texture animations take the form:
			// +0texname +1texname +2texname. We filter everything after +0.

			// Does it have a slash? If it does, the real name is just after.
			int nStart;
			int nLen = (int)strlen(m_pDir[i].szName);
			for(nStart = 9; nStart < nLen; nStart++)
			{
				if(m_pDir[i].szName[nStart] == '\\' || m_pDir[i].szName[nStart] == '/')
				{
					nStart++;
					break;
				}
			}

			if(nStart == nLen)	// No \.
				nStart = 9;

			if(m_pDir[i].szName[nStart] == '+' && m_pDir[i].szName[nStart+1] != '0')
				continue;

			m_pDir[i].nHandle = i;
			m_nNumTexs++;
		}
	}
	
	if(m_nNumTexs == 0)
	{
		m_pTexs = NULL;
		return true;
	}

	// Allocate textures that we return to Tread.
	m_pTexs = new CTexture[m_nNumTexs];
	if(m_pTexs == NULL)
		return false;

	m_nNumTexs = 0;
	
	// Setup each texture, and parses info.
	for(i = 0; i < m_nNumFiles; i++)
	{
		// Filtered?
		if(m_pDir[i].nHandle == -1)
			continue;

		int nLen;
		char szName[64];

		m_pTexs[m_nNumTexs].m_nHandle = m_pDir[i].nHandle;
		m_pTexs[m_nNumTexs].m_pReader = m_pReader;

		// Kill extension.
		strcpy(szName, &m_pDir[i].szName[9]);
		nLen = strlen(szName);
		szName[nLen-4] = NULL;

		m_pTexs[m_nNumTexs].m_sName = szName;
		m_pTexs[m_nNumTexs].m_sRealName = szName;

		m_nNumTexs++;
	}

	return true;
}