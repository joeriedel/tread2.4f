// TEXREADER.CPP
// Copyright © 2000 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#include "stdafx.h"
#include "TexReader.h"
#include "Texture.h"

// Called to free a texture.
bool CTexReader::FreeTexture(CTexture* pTex)
{
	pTex->FreeData();
	if(pTex->m_pPalette)
	{
		delete pTex->m_pPalette;
		pTex->m_pPalette = NULL;
	}

	return true;
}

bool CTexReader::FreeTextureData(CTexture* pTex)
{
	if(pTex->m_pData != NULL)
		delete[] pTex->m_pData;
	pTex->m_pData = NULL;
	pTex->m_bApplyGamma = true;
	return true;
}