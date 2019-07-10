// Q2TEXDIR.CPP
// Copyright © 2000 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#include "stdafx.h"
#include "../TreadAPI.h"
#include "../szString.h"
#include "Q2TexDir.h"
#include "TargaReader.h"
#include "Q2WalReader.h"

//
// Every texture that is found be the GenTexDirectory passes through
// here. In this case we do some extension mangling: change all '\' to '/'
// and we remove the extension. Tread will display the m_sName field of the
// CTexture object in the texture browser dialog. The m_sRealName field
// is what "should" be written to the file. However, this is really up to 
// the implementation of the file writer, as you will see in the QuakeMap 
// plugin example.
//
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

//
// GenTexDirectory calls this routine to see if we want
// to keep this file or not. In Quake2 we skip animated
// textures.
//
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

// Called to get the search filters. NOTE: *.* must be used
// for recursive searches.
bool CQ2TexDir::GetFileFilters(int& nNumFilters, CString* pFilters)
{
	nNumFilters = 1;
	pFilters[0] = "*.*";

	if(GetGameAPI()->GetCurrentGame()->GetTextureTypeString() == "@TARGA")
		m_sType = ".tga";
	else
		m_sType = ".wal";

	//
	// These lines are critical: we attach the format readers
	// to this pak object. This allows them to call back into us
	// to get file pointers and seek to the correct offsets.
	//
	m_TargaReader.SetPakFile(this);
	m_Q2WalReader.SetPakFile(this);

	return true;
}