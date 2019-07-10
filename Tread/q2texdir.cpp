// Q2TEXDIR.CPP
// Copyright © 2000 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#include "stdafx.h"
#include "TexAPI.h"
#include "Texture.h"
#include "Q2TexDir.h"
#include "TargaReader.h"
#include "Q2WalReader.h"
#include "GameAPI.h"
#include "szString.h"

void CQ2TexDir::ProcessTexture(CTexture* pTexture, CString* pFilter)
{
	pTexture->m_sName.MakeLower();
	pTexture->m_sName = SetFileExtension(pTexture->m_sName, "");
	pTexture->m_sName.Replace('\\', '/');

	pTexture->m_sRealName.MakeLower();
	pTexture->m_sRealName = SetFileExtension(pTexture->m_sRealName, "");
	pTexture->m_sRealName.Replace('\\', '/');

	pTexture->m_sFilename.MakeLower();


	if(CompareExt(pTexture->m_sFilename, ".tga"))
		pTexture->m_pReader = &m_TargaReader;
	else
		pTexture->m_pReader = &m_Q2WalReader;
}

bool CQ2TexDir::SkipFile(CString sFilename, CString* pFilter)
{
	sFilename.MakeLower();
	if(!CompareExt(sFilename, m_sType))
		return true;

	// Filter animations.
	// Does it have a slash? If it does, the real name is just after.
	int nStart;
	int nLen = sFilename.GetLength();
	for(nStart = 0; nStart < nLen; nStart++)
	{
		if(sFilename[nStart] == '\\' || sFilename[nStart] == '/')
		{
			nStart++;
			break;
		}
	}

	if(nStart == nLen)	// No \.
		nStart = 0;

	if(sFilename[nStart] == '+' && sFilename[nStart+1] != '0')
		return true;

	return false;
}

bool CQ2TexDir::GetFileFilters(int& nNumFilters, CString* pFilters)
{
	nNumFilters = 1;
	pFilters[0] = "*.*";

	if(GetGameAPI()->GetCurrentGame()->GetTextureTypeString() == "@TARGA")
		m_sType = ".tga";
	else
		m_sType = ".wal";

	m_TargaReader.SetPakFile(this);
	m_Q2WalReader.SetPakFile(this);

	return true;
}