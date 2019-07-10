// GENTEXDIR.CPP
// Copyright © 2000 Joe Riedel, Nick Randal.
// Author: Joe Riedel.

// Generic texture directory loader.
#include "stdafx.h"
#include "TexAPI.h"
#include "Texture.h"
#include "GenTexDirFp.h"
#include "FUtils.h"
#include <stdio.h>

CGenTexDirectoryFp::~CGenTexDirectoryFp()
{
	if(m_nNumTextures > 0)
		delete [] m_pTextures;
	m_nNumTextures = 0;
}

// Attaches a file to this object & initializes the reader.
bool CGenTexDirectoryFp::OpenAndInitialize(const char* szFileName)
{
	int i, nNumFilters, nCount;
	CString sFilename;
	CFileSearch FileSearch;
	CString Filters[MAX_GENDIR_FILTERS];

	if(!GetFileFilters(nNumFilters, Filters))
		return false;

	m_nNumTextures=0;

	for(i = 0; i < nNumFilters; i++)
	{
		if(!FileSearch.BeginFileSearch(szFileName, Filters[i], true))
			continue;

		for(;;)
		{
			FileSearch.GetNextFile(sFilename);
			if(sFilename == "")
				break;
			sFilename.MakeLower();
			if(SkipFile(sFilename, &Filters[i]))
				continue;
			m_nNumTextures++;
		}
	}

	if(m_nNumTextures == 0)
		return false;

	nCount = 0;
	m_pTextures = new CTexture[m_nNumTextures];

	for(i = 0; i < nNumFilters; i++)
	{
		if(!FileSearch.BeginFileSearch(szFileName, Filters[i], true))
			continue;

		for(;;)
		{
			FileSearch.GetNextFile(sFilename);
			if(sFilename == "")
				break;
			sFilename.MakeLower();
			if(SkipFile(sFilename, &Filters[i]))
				continue;
			m_pTextures[nCount].m_sName = sFilename;
			m_pTextures[nCount].m_sRealName = sFilename;
			m_pTextures[nCount].m_sFilename = CString(szFileName) + "\\" + sFilename;
			ProcessTexture(&m_pTextures[nCount], &Filters[i]);
			nCount++;
		}
	}

	return true;
}

// Get's the list of texture's in the pak.
CTexture* CGenTexDirectoryFp::GetTextureList(int& nNumTextures) const
{
	nNumTextures = m_nNumTextures;
	return m_pTextures;
}

// Get's the file object.
void* CGenTexDirectoryFp::GetFileObject(CTexture* pTex, int nContext)
{
	// Open the texture and return a CFile.
	FILE* fp = fopen(pTex->m_sFilename, "rb");
	return fp;
}

void CGenTexDirectoryFp::ReleaseFileObject(void* pFile, int nContext)
{
	if(pFile != NULL)
		fclose((FILE*)pFile);
}
