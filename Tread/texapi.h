// TEXAPI.H
// Copyright © 1999 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#ifndef TEXAPI_H
#define TEXAPI_H

#include <memory.h>
#include "PakFile.h"
#include "Texture.h"
#include "PakLoader.h"
#include "TexPalette.h"
#include "LinkedList.h"
#include "TExport.h"

// $sb "texapi.h block1"

/*
12/15/99 - Joe Riedel
	- Added directory loading
*/

// Provides interface to pak classes and routines.
#define BAD_PAK_HANDLE		-1

#define MAX_TEXTURE_MEM		((1024*1000)*96)	// 96 Megs.

class _T_EXPORT CTexAPI
{
private:

	char m_szFilter[256];
	int m_nNextID;
	int m_nNumTexs, m_nFilterNumTexs;
	CTexture* m_pCurTex;
	CTexture** m_ppTexList;
	CTexture** m_ppFilterTexList;
	CPakLoader m_PakLoader;
	bool m_bPaksToList[MAX_OPEN_PAKS];

	int m_nUsedMem;
	CLinkedList<CTexture> m_LoadedTextureList;

	void FreePakTexList();
	
public:

	CTexAPI()
	{ 
		m_pCurTex = NULL;
		m_nNextID = 1;
		m_nNumTexs = m_nFilterNumTexs = 0;
		m_ppTexList = m_ppFilterTexList = NULL; 
		m_szFilter[0] = NULL;
		memset(m_bPaksToList, 0, sizeof(m_bPaksToList));
		m_LoadedTextureList.SetAutoDelete(false);
	}

	virtual ~CTexAPI()
	{
		FreePakTexList();
		if(m_ppTexList != NULL)
			delete m_ppTexList;
		m_PakLoader.CloseAllPaks();
	}

	// Openstex pak.
	int OpenPak(const char* szFilename, bool bDir = false);
	bool BuildOpenPaks();
	bool ClosePak(int nPakHandle);
	bool CloseAllPaks(void);
	const char* GetPakFilename(int nPakHandle);

	void AssignID(CTexture* pTexture);

	// Get's a paks texture list.
	bool AddPakToList(int nPakHandle);
	bool RemovePakToList(int nPakHandle);
	void SetNameFilter(const char* szFilter);
	bool BuildFilterTexList();
	CTexture* GetTextureFromName(CString sName);
	CTexture** GetFilterTexList(int& nNumTextures);
	CTexture** GetFullPakTexList(int& nNumTextures);
	CPakLoader* GetPakLoader() { return &m_PakLoader; }

	// Set's/Get's current texture.
	void SetCurTexture(CTexture* pTexture);
	CTexture* GetCurTexture();
	CString GetCurTextureName();
	
	// Called after the game has loaded.
	void PostGameLoad();

	// Called when a texture requests to load itself.
	void PreTextureLoad(CTexture* pTexture);
	// Called when the texture sucessfully loaded.
	void PostTextureLoad(CTexture* pTexture);
	// Free's all loaded textures.
	void FreeAllLoadedTextures();

};

// $eb

// Get's the texAPI.
CTexAPI* GetTexAPI();

#endif