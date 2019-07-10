// Q2PAKFILE.CPP
// Copyright © 1999 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#include "stdafx.h"
#include "szstring.h"
#include "Texture.h"
#include "Q2PakFile.h"
#include "Q2WalReader.h"
#include "TargaReader.h"
#include "dakrle.h"
#include "dakwalreader.h"
#include "GameApi.h"
#include "futils.h"

CQ2PakFile::CQ2PakFile()
{
	m_nNumFiles = 0;
	m_pDir = NULL;
	m_pTexs = NULL;
	m_pReader = NULL;
	m_pList = 0;
}

CQ2PakFile::~CQ2PakFile()
{
	if(m_pReader != NULL)
	{
		delete m_pReader;
		m_pReader = NULL;
	}
}

// Get's the file object.
void* CQ2PakFile::GetFileObject(CTexture* pTex, int nContext)
{ 
	if(m_nType == DAK)
	{
		int nSize;
		unsigned char* pRaw, *pData;

		// In dak we must decompress the chunk for the reader.
		Q2FILEENTRY* pEntry = &m_pDir[pTex->m_nHandle];
		if(pEntry->nCompressed>0)
			nSize = pEntry->nCompSize;
		else
			nSize = pEntry->nLength;

		pRaw = new unsigned char[nSize];
		if(pRaw == NULL)
			return NULL;

		// Load.
		if(m_File.Read(pRaw, nSize) != (unsigned int)nSize)
		{
			delete[] pRaw;
			return NULL;
		}

		// Compressed?
		if(pEntry->nCompressed>0)
		{
			pData = DecompressDAKRLEBlock(pRaw, pEntry->nCompSize, pEntry->nLength);
			delete[] pRaw;
		}
		else
			pData = pRaw;

		return pData;
	}

	return &m_File;
}

bool CQ2PakFile::ClaimFile(const char* szFilename)
{
	CFile hFile;

	// Open the file.
	if(!hFile.Open(szFilename, CFile::modeRead | CFile::typeBinary | CFile::shareDenyNone))
		return false;

	int nFileType;
	ReadInt(hFile, nFileType);
	hFile.Close();
	return nFileType == Q2PAKID;
}

// Seeks to the file's position.
bool CQ2PakFile::SeekToFile(CTexture* pTex)
{
	// Ensure a valid indice.
	if(pTex->m_nHandle < 0 || pTex->m_nHandle >= m_nNumFiles)
		return false;

	// Seek to position.
	m_File.Seek(m_pDir[pTex->m_nHandle].nPos, CFile::begin);
	return true;
}

// Get's the list of texture's in the pak.
CTexture** CQ2PakFile::GetTextureList(int& nNumTextures) const
{ nNumTextures = m_nNumTexs; return m_pList; }

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
	if(m_pList != 0)
		delete[] m_pList;
	m_pList = NULL;
	return true;
}

// Attaches a file to this object & initializes the reader.
bool CQ2PakFile::OpenAndInitialize(const char* szFileName)
{
	int nDirOffset, nDirLength;

	m_sFilename= szFileName;
	CGameDefinition* pGame = GetGameAPI()->GetCurrentGame();
	if(pGame->GetTextureTypeString() == "@TARGA")
	{
		m_pReader = new CTargaReader();
		m_nType = TGA;
	}
	else if(pGame->GetTextureTypeString() == "@M32")
	{
		m_pReader = new CSofReader();
		m_nType = M32;
	}
	else if(pGame->GetTextureTypeString() == "@DAIWAL")
	{
		m_pReader = new CDakWalReader();
		m_nType = DAK;
	}
	else
	{
		m_pReader = new CQ2WalReader();
		m_nType = WAL;
	}

	m_pReader->SetPakFile(this);
	// Open the file.
	if(!m_File.Open(szFileName, CFile::modeRead | CFile::typeBinary | CFile::shareDenyNone))
		return false;

	int nId;
	if(!ReadInt(m_File, nId))
		return false;

	if(nId != Q2PAKID)
		return false;

	// Read # of files and dir location.
	if(!ReadInt(m_File, nDirOffset))
		return false;
	if(!ReadInt(m_File, nDirLength))
		return false;

	m_File.Seek(nDirOffset, CFile::begin);
	if(m_nType == DAK)
		m_nNumFiles = nDirLength / 72;
	else
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
	else if(m_nType == M32)
		strcpy(sExt, "m32\0");
	else
		strcpy(sExt, "wal\0");

	m_nNumTexs = 0;
	for(i = 0; i < m_nNumFiles; i++)
	{
		// Read the dir entry.
		if(m_nType == DAK)
		{
			if(!m_File.Read(&m_pDir[i], 72))
				return false;
		}
		else
		{
			if(!m_File.Read(&m_pDir[i], 64))
				return false;
		}
		
		LOAD_INTEL_INTA(m_pDir[i].nCompressed);
		LOAD_INTEL_INTA(m_pDir[i].nCompSize);
		LOAD_INTEL_INTA(m_pDir[i].nLength);
		LOAD_INTEL_INTA(m_pDir[i].nPos);
		
		// Make the name lower case.
		StrLower(m_pDir[i].szName);
		m_pDir[i].nHandle = -1;

		// Is this a Q2 .wal file?
		if(CompareExt(m_pDir[i].szName, sExt) 
			&& (m_pDir[i].szName[0] == 't')
			&& (m_pDir[i].szName[1] == 'e')
			&& (m_pDir[i].szName[2] == 'x'))
		{
			// Filter animations.
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

	// Allocate textures.
	m_pTexs = new CTexture[m_nNumTexs];
	m_pList = new CTexture*[m_nNumTexs];
	if(m_pTexs == NULL)
		return false;

	m_nNumTexs = 0;
	
	// Setup each texture, and parses info.
	for(i = 0; i < m_nNumFiles; i++)
	{
		if(m_pDir[i].nHandle == -1)
			continue;

		int nLen;
		char szName[64];

		m_pList[m_nNumTexs] = &m_pTexs[m_nNumTexs];
		m_pTexs[m_nNumTexs].m_nHandle = m_pDir[i].nHandle;
		m_pTexs[m_nNumTexs].m_pReader = m_pReader;

		strcpy(szName, &m_pDir[i].szName[9]);
		nLen = strlen(szName);
		szName[nLen-4] = NULL;

		m_pTexs[m_nNumTexs].m_sName = szName;
		m_pTexs[m_nNumTexs].m_sRealName = szName;

		m_nNumTexs++;
	}

	return true;
}