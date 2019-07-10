// JPEGREADER.CPP
// Copyright © 2000 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#include "stdafx.h"
#include "jpegreader.h"
#include "texture.h"
#ifdef _WIN32
#include "./jpeg/jpeglib.h"
#else
#include "jpeglib.h"
#endif

CJPEGReader::CJPEGReader()
{
}

CJPEGReader::~CJPEGReader()
{
}

// Reads info from a Q2. Wal File.
bool CJPEGReader::LoadTextureInfo(CTexture* pTex)
{
	unsigned char* pData;

	if(!CTexReader::SeekToFile(pTex))
		return false;

	pData = (unsigned char*)CTexReader::GetFileObject(pTex);
	if(pData == NULL)
		return false;

	ReadJPGInfo(pData, &pTex->m_nWidth, &pTex->m_nHeight, &pTex->m_nBits);
	CTexReader::ReleaseFileObject(pData);

	if(pTex->m_nBits == 32)
		pTex->m_nSize = pTex->m_nWidth*pTex->m_nHeight*4;
	else
		pTex->m_nSize = pTex->m_nWidth*pTex->m_nHeight*3;

	return true;
}

// Reads a Q2. Wal File.
bool CJPEGReader::LoadTexture(CTexture* pTex)
{
	unsigned char* pData;

	if(!CTexReader::SeekToFile(pTex))
		return false;

	pData = (unsigned char*)CTexReader::GetFileObject(pTex);
	if(pData == NULL)
		return false;

	LoadJPGBuff(pData, &pTex->m_pData, &pTex->m_nWidth, &pTex->m_nHeight, &pTex->m_nBits);
	CTexReader::ReleaseFileObject(pData);

	if(pTex->m_nBits == 32)
		pTex->m_nSize = pTex->m_nWidth*pTex->m_nHeight*4;
	else
		pTex->m_nSize = pTex->m_nWidth*pTex->m_nHeight*3;

	// Drop alpha on 32 bit.
	return pTex->To24BitColor();
}