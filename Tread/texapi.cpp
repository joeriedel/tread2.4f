// TEXAPI.CPP
// Copyright © 1999 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#include "stdafx.h"
#include <string.h>
#include "TexApi.h"
#include "Texture.h"
#include "TreadSys.h"
#include "LinkedList.h"
#include "PakLoader.h"
#include "Tread3D.h"

/*
12/15/99 - Joe Riedel
	- Added directory loading
*/

CTexAPI	m_TexAPI;

// Gets the tex api object.
CTexAPI* GetTexAPI() { return &m_TexAPI; }

CString CTexAPI::GetCurTextureName()
{
	if(GetCurTexture() != NULL)
		return GetCurTexture()->m_sRealName;

	return "";
}

// Called when a texture requests to load itself.
void CTexAPI::PreTextureLoad(CTexture* pTexture)
{
	pTexture->LoadInfo();

	// See if this puts us over the top.
	if(pTexture->m_nSize+m_nUsedMem > (GetRegInt(I_TEX_MEMORY)*1024*1024))
	{
		FreeAllLoadedTextures();
		m_nUsedMem = 0;
	}
}

// Called after a texture is loaded.
void CTexAPI::PostTextureLoad(CTexture* pTexture)
{
	m_nUsedMem += pTexture->m_nSize;
	m_LoadedTextureList.AddItem(pTexture);
}

// Free's all loaded textures.
void CTexAPI::FreeAllLoadedTextures()
{
	int nCount=0;
	CTexture* pTexture;

	m_LoadedTextureList.First();
	for(pTexture = m_LoadedTextureList.GetCurItem(); pTexture; pTexture = m_LoadedTextureList.GetNextItem())
	{
		nCount++;
		pTexture->FreeData();
	}
	m_LoadedTextureList.ReleaseList();

	Debug("texAPI: purged %d textures.\n", nCount);
}

// Open's the pak.
int CTexAPI::OpenPak(const char* szFilename, bool bDir)
{
	CPakFile* pPak;

	pPak = m_PakLoader.OpenPak(szFilename, bDir);
	if(!pPak)
		return BAD_PAK_HANDLE;

	// Get the handle.
	int nPakHandle = m_PakLoader.GetPakIndex(pPak);
	if(nPakHandle == -1)
		return BAD_PAK_HANDLE;

	// Add this pak to the valid list.
	AddPakToList(nPakHandle);

	return nPakHandle;
}

// Get's the current texture.
CTexture* CTexAPI::GetCurTexture()
{
	if(m_pCurTex != NULL)
		return m_pCurTex;

	if(m_ppTexList != NULL)
	{
		SetCurTexture(m_ppTexList[0]);
		return m_pCurTex;
	}

	return NULL;
}

// Set's the current texture.
void CTexAPI::SetCurTexture(CTexture* pTexture)
{
	m_pCurTex = pTexture;
}

// Closes a pak file.
bool CTexAPI::ClosePak(int nPakHandle)
{
	CPakFile* pPak = m_PakLoader.GetPakFromIndex(nPakHandle);
	if(pPak == NULL)
		return false;

	bool b = m_PakLoader.ClosePak(pPak);
	RemovePakToList(nPakHandle);

	return b;
}

// Closes all pak files.
bool CTexAPI::CloseAllPaks()
{
	int i;

	bool b = m_PakLoader.CloseAllPaks();
	for(i = 0; i < MAX_OPEN_PAKS; i++)
		RemovePakToList(i);

	FreePakTexList();
	if(m_ppTexList != NULL)
		delete[] m_ppTexList;
	m_ppTexList = NULL;
	m_nNumTexs = 0;
	m_pCurTex = NULL;

	return b;
}

void CTexAPI::AssignID(CTexture* pTexture)
{
	pTexture->m_nUniqueID = m_nNextID++;
}

// Called after the game has loaded.
bool CTexAPI::BuildOpenPaks()
{
	int i, k;
	CPakFile* pPak;

	Debug("texAPI: building open paks\n");

	for(i = 0; i < MAX_OPEN_PAKS; i++)
	{
		pPak = m_PakLoader.GetPakFromIndex(i);
		if(pPak == NULL)
			continue;
		if(!pPak->BuildPak())
			return false;
	}

	// Load the list o' textures.
	int nNumTexs;
	CTexture** pTexs;

	for(i = 0; i < MAX_OPEN_PAKS; i++)
	{
		pPak = m_PakLoader.GetPakFromIndex(i);
		if(pPak == NULL)
			continue;

		pTexs = pPak->GetTextureList(nNumTexs);
		Debug("... %i texture(s) in %s\n", nNumTexs, pPak->GetFilename());

		// For use by rendering we assign each texture a unique id.
		// For GL this is used for bindings. It may be used for other
		// things later.
		for(k = 0; k < nNumTexs; k++)
			AssignID(pTexs[k]);

		if(pTexs != NULL)
		{
			// Add to total list.
			int nOldNum = m_nNumTexs;
			m_nNumTexs += nNumTexs;
			CTexture** ppOldList;

			ppOldList = m_ppTexList;
			m_ppTexList = new CTexture* [m_nNumTexs];

			// Copy old list.
			memcpy(m_ppTexList, ppOldList, nOldNum * sizeof(CTexture*));
			if(ppOldList != NULL)
				delete ppOldList;

			int i;
			
			// Copy.
			for(i = 0; i < nNumTexs; i++)
				m_ppTexList[i+nOldNum] = pTexs[i];
		}
	}

	// Tell 'em where done.
	for(i = 0; i < MAX_OPEN_PAKS; i++)
	{
		pPak = m_PakLoader.GetPakFromIndex(i);
		if(pPak == NULL)
			continue;
		if(!pPak->PakBuildsComplete())
			return false;
	}

	Debug("... %d total textures parsed\n", m_nNumTexs);

	return true;
}


// Get's a pak's filename.
const char* CTexAPI::GetPakFilename(int nPakHandle)
{
	CPakFile* pPak = m_PakLoader.GetPakFromIndex(nPakHandle);
	if(pPak == NULL)
		return NULL;

	return pPak->GetFilename();
}

// Set's the filter for the pak.
void CTexAPI::SetNameFilter(const char* szFilter)
{
	strcpy(m_szFilter, szFilter);
}

// Add/Remove paks to list.
bool CTexAPI::AddPakToList(int nPakHandle)
{
	if(nPakHandle < 0 || nPakHandle > MAX_OPEN_PAKS)
		return false;

	m_bPaksToList[nPakHandle] = true;
	return true;
}

bool CTexAPI::RemovePakToList(int nPakHandle)
{
	if(nPakHandle < 0 || nPakHandle > MAX_OPEN_PAKS)
		return false;

	m_bPaksToList[nPakHandle] = false;
	return true;
}

// Free's the pak tex list.
void CTexAPI::FreePakTexList()
{
	if(m_ppFilterTexList != NULL)
		delete m_ppFilterTexList;
	m_ppFilterTexList = NULL;
	m_nFilterNumTexs = 0;
}

// Get's a texture based on the name.
CTexture* CTexAPI::GetTextureFromName(CString sName)
{
	int i;
	int nNumTexs;
	CTexture** pTexs;

	pTexs = GetFullPakTexList(nNumTexs);
	sName.MakeLower();
	for(i = 0; i < nNumTexs; i++)
		if(pTexs[i]->m_sRealName == sName)
			return pTexs[i];


	return NULL;
}

// Get's the filtered list.
CTexture** CTexAPI::GetFilterTexList(int& nNumTextures)
{
	nNumTextures = m_nFilterNumTexs;
	return m_ppFilterTexList;
}

// Gets the unfiltered list.
CTexture** CTexAPI::GetFullPakTexList(int& nNumTextures)
{
	nNumTextures = m_nNumTexs;
	return m_ppTexList;
}

// Build's the tex list.
bool CTexAPI::BuildFilterTexList()
{
	int i, k;
	int nNumTexs;
	int nTotalNumTexs;
	CPakFile* pPak;
	CTexture** pTexList;

	FreePakTexList();

	nTotalNumTexs = 0;

	// Count the total # of texs.
	for(i = 0; i < MAX_OPEN_PAKS; i++)
	{
		// Skip this pak?
		if(!m_bPaksToList[i])
			continue;

		pPak = m_PakLoader.GetPakFromIndex(i);
		if(pPak == NULL)
			continue;

		pTexList = pPak->GetTextureList(nNumTexs);
		if(!pTexList)
			continue;

		// Short-curcuit if no search string.
		if(m_szFilter[0] == NULL)
		{
			nTotalNumTexs += nNumTexs;
			continue;
		}

		for(k = 0; k < nNumTexs; k++)
		{
			if(pTexList[k]->m_sName.Find(m_szFilter) != -1)
				nTotalNumTexs++;
		}
	}

	// Allocate.
	m_ppFilterTexList = new CTexture* [nTotalNumTexs];

	nTotalNumTexs = 0;
	// Now the we have the full count, add each one.
	for(i = 0; i < MAX_OPEN_PAKS; i++)
	{
		// Skip this pak?
		if(!m_bPaksToList[i])
			continue;

		pPak = m_PakLoader.GetPakFromIndex(i);
		if(pPak == NULL)
			continue;

		pTexList = pPak->GetTextureList(nNumTexs);
		if(!pTexList)
			continue;

		for(k = 0; k < nNumTexs; k++)
		{
			if(m_szFilter[0] == NULL || pTexList[k]->m_sName.Find(m_szFilter) != -1)
				m_ppFilterTexList[nTotalNumTexs++] = pTexList[k];
		}
	}

	m_nFilterNumTexs = nTotalNumTexs;

	return true;
}