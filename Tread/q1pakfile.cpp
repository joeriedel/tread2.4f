// Q2PAKFILE.CPP
// Copyright © 1999 Joe Riedel, Nick Randal.
// Author: Nick Randal.
#include "stdafx.h"
#include "szstring.h"
#include "Texture.h"
#include "Q1PakFile.h"
#include "futils.h"

// Get's the file object.
void* CQ1PakFile::GetFileObject(CTexture* pTex, int nContext)
{ return &m_hFile; }

// Seeks to the file's position.
bool CQ1PakFile::SeekToFile(CTexture* pTex)
{
	// Ensure a valid indice.
	if(pTex->m_nHandle < 0 || pTex->m_nHandle >= m_nNumFiles)
		return false;

	// Seek to position.
	m_hFile.Seek(m_pDir[pTex->m_nHandle].nPos, CFile::begin);
	return true;
}

int CQ1PakFile::GetFileOffset(CTexture* pTex)
{
	if(pTex->m_nHandle < 0 || pTex->m_nHandle >= m_nNumFiles)
		return -1;

	return m_pDir[pTex->m_nHandle].nPos;
}

// Get's the list of texture's in the pak.
CTexture** CQ1PakFile::GetTextureList(int& nNumTextures) const
{ nNumTextures = m_nNumTexs; return m_pList; }

bool CQ1PakFile::Close()
{
	m_hFile.Close();
	m_nNumFiles = 0;
	if(m_pDir != NULL)
		delete[] m_pDir;
	m_pDir = NULL;
	if(m_pTexs != NULL)
		delete[] m_pTexs;
	m_pTexs = NULL;
	if(m_pList != NULL)
		delete[] m_pList;
	m_pList = NULL;
	return true;
}

bool CQ1PakFile::ClaimFile(const char* szFilename)
{
	CFile hFile;

	// Open the file.
	if(!hFile.Open(szFilename, CFile::modeRead | CFile::typeBinary | CFile::shareDenyNone))
		return false;

	int nFileType;
	ReadInt(hFile, nFileType);
	hFile.Close();
	return nFileType == Q1PAKID;
}

// Attaches a file to this object & initializes the reader.
bool CQ1PakFile::OpenAndInitialize(const char* szFileName)
{
	int nDirOffset = 0, nDirCount = 0;

	m_sFilename = szFileName;

	// Open the file.
	if(!m_hFile.Open(szFileName, CFile::modeRead | CFile::modeNoTruncate | CFile::shareDenyNone))
		return false;

	int nId;
	if(!ReadInt(m_hFile, nId))
		return false;

	if(nId != Q1PAKID)
		return false;

	// Read # of files and dir location.
	if(!ReadInt(m_hFile, nDirCount))
		return false;

	if(!ReadInt(m_hFile, nDirOffset))
		return false;

	m_hFile.Seek(nDirOffset, CFile::begin);

	m_nNumFiles = nDirCount;
	// Allocate directory size.
	m_pDir = new Q1FILEENTRY[nDirCount];
	if(m_pDir == NULL)
		return false;

	// Read it.
	int i;
	m_nNumTexs = 0;
	for(i = 0; i < m_nNumFiles; i++)
	{
		m_pDir[i].nType = 0;

		ReadInt(m_hFile, m_pDir[i].nPos);		//File position
		ReadInt(m_hFile, m_pDir[i].nLength);	//File length
		m_hFile.Seek(4, CFile::current);		//skip duplicate value
		m_hFile.Read(&m_pDir[i].nType, 1);	//File type
		m_hFile.Seek(3, CFile::current);
		m_hFile.Read(&m_pDir[i].szName, 16);	//File name

		// Make the name lower case.
		StrLower(m_pDir[i].szName);

		m_pDir[i].nHandle = -1;

		if(m_pDir[i].nType == Q1FILEID)
		{
			// Filter animations.
			// Does it have a slash? If it does, the real name is just after.
			if(m_pDir[i].szName[0] == '+' && m_pDir[i].szName[1] != '0')
				continue;
			
			m_pDir[i].nHandle = i;
			m_nNumTexs++;
		}
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

		m_pList[m_nNumTexs] = &m_pTexs[m_nNumTexs];
		m_pTexs[m_nNumTexs].m_nHandle = m_pDir[i].nHandle;
		m_pTexs[m_nNumTexs].m_pReader = &m_WalReader;

		// Remove the prefixed textures/ and the postfixed .wal ext.
		/*
		int nLen;
		

		strcpy(szName, &m_pDir[i].szName[9]);
		nLen = strlen(szName);
		szName[nLen-4] = NULL;

		m_pTexs[m_nNumTexs].m_sName = szName;
		*/

		char szName[16];

		strcpy(szName, m_pDir[i].szName);
		m_pTexs[m_nNumTexs].m_sName = szName;
		m_pTexs[m_nNumTexs].m_sRealName = szName;

		m_nNumTexs++;
	}

	return true;
}