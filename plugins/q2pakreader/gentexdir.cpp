// GENTEXDIR.CPP
// Copyright © 2000 Joe Riedel, Nick Randal.
// Author: Joe Riedel.

// Generic texture directory loader.
#include "stdafx.h"
#include "../TreadAPI.h"
#include "../FUtils.h"
#include "GenTexDir.h"

//
// The job of this class is to search (recursivley) the requested directory
// using file filters obtained from derrived classes. It then passes each
// file found to the derrived class to ask if it should store this file in its
// list.
//

CGenTexDirectory::~CGenTexDirectory()
{
	if(m_nNumTextures > 0)
		delete [] m_pTextures;
	m_nNumTextures = 0;
}

// Attaches a file to this object & initializes the reader.
bool CGenTexDirectory::OpenAndInitialize(const char* szFileName)
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
CTexture* CGenTexDirectory::GetTextureList(int& nNumTextures) const
{
	nNumTextures = m_nNumTextures;
	return m_pTextures;
}

// Get's the file object.
void* CGenTexDirectory::GetFileObject(CTexture* pTex, int nContext)
{
	// Open the texture and return a CFile.
	CFile* pFile = new CFile();
	if(!pFile->Open(pTex->m_sFilename, CFile::modeRead|CFile::typeBinary))
	{
		delete pFile;
		return NULL;
	}

	return pFile;
}

void CGenTexDirectory::ReleaseFileObject(void* pFile, int nContext)
{
	CFile* pCFile = (CFile*)pFile;
	pCFile->Close();
	delete pCFile;
}
