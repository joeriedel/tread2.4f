// DAKWALREADER.CPP
// Copyright © 2000 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#include "stdafx.h"
#include "dakwalreader.h"
#include "byteparser.h"
#include "texture.h"

CDakWalReader::CDakWalReader()
{
}

CDakWalReader::~CDakWalReader()
{
}

// Reads info from a Q2. Wal File.
bool CDakWalReader::LoadTextureInfo(CTexture* pTex)
{
	CByteParser bp;
	unsigned char* data;

	if(!CTexReader::SeekToFile(pTex))
		return false;

	// The dak reader will decompress the rle info for this file.
	data = (unsigned char*)CTexReader::GetFileObject(pTex);
	if(!data)
		return false;
	
	bp.AttachData(data, -1);
	bp.Skip(32+4);
	pTex->m_nWidth = bp.GetInt();
	pTex->m_nHeight = bp.GetInt();
	
	pTex->m_nSize = pTex->m_nWidth*pTex->m_nHeight*3;
	pTex->m_nSurface = pTex->m_nContents = pTex->m_nValue = 0;
	pTex->m_nBits = 8;

	CTexReader::ReleaseFileObject(data);
	return true;
}

// Reads a Q2. Wal File.
bool CDakWalReader::LoadTexture(CTexture* pTex)
{
	CByteParser bp;
	unsigned char* data;

	if(!CTexReader::SeekToFile(pTex))
		return false;

	// The dak reader will decompress the rle info for this file.
	data = (unsigned char*)CTexReader::GetFileObject(pTex);
	if(!data)
		return false;
	
	bp.AttachData(data, -1);
	bp.Skip(32+4+4+4);
	int nMipOff = bp.GetInt();
	
	// Need to load the palette?
	if(pTex->m_pPalette == NULL)
	{
		unsigned char* palData = new unsigned char[768];
		if(!palData)
			return false;
		bp.Seek(120, BP_BEGIN);
		bp.GetData(palData, 768);
		pTex->m_pPalette = new CTexPalette(palData);
		if(!pTex->m_pPalette)
			return false;
	}

	// Goto the mipoff.
	bp.Seek(nMipOff, BP_BEGIN);
	pTex->m_nSize /= 3;
	pTex->m_pData = new unsigned char[pTex->m_nSize];
	if(!pTex->m_pData)
		return false;
	bp.GetData(pTex->m_pData, pTex->m_nSize);
	CTexReader::ReleaseFileObject(data);
	pTex->m_nBits = 8;

	// Convert
	return pTex->To24BitColor();
}
