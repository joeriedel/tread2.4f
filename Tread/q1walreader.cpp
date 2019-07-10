// Q1WALREADER.CPP
// Copyright © 2000 Joe Riedel, Nick Randal.
// Author: Nick Randal.
#include "stdafx.h"
#include "TexAPI.h"
#include "Q1WalReader.h"
#include "futils.h"

CQ1WalReader::CQ1WalReader() : CTexReader()
{}

CQ1WalReader::~CQ1WalReader()
{}

// Reads info from a Q1. Wal File.
bool CQ1WalReader::LoadTextureInfo(CTexture* pTex)
{
	CFile* hFile;
	
	// Have the pak seek our file position.
	if(!CTexReader::SeekToFile(pTex))
		return false;
	
	hFile = (CFile*)CTexReader::GetFileObject(pTex);
	if(hFile == NULL)
		return false;

	// Skip forward 32.
	hFile->Seek(16, CFile::current);

	// Read.
	if(!ReadInt(*hFile, pTex->m_nWidth))
	{
		CTexReader::ReleaseFileObject(hFile);
		return false;
	}

	if(!ReadInt(*hFile, pTex->m_nHeight))
	{
		CTexReader::ReleaseFileObject(hFile);
		return false;
	}
	
	pTex->m_nSize = pTex->m_nWidth * pTex->m_nHeight * 3;

	CTexReader::ReleaseFileObject(hFile);
	return true;
}

// Reads a Q2. Wal File.
bool CQ1WalReader::LoadTexture(CTexture* pTex)
{
	CFile* hFile;

	// Have the pak seek our file position.
	if(!CTexReader::SeekToFile(pTex))
		return false;
	
	hFile = (CFile*)CTexReader::GetFileObject(pTex);
	if(hFile == NULL)
		return false;

	// Skip forward 16+8 (Skip Width/Height).
	hFile->Seek(24, CFile::current);

	int nMipOffsets[4];
	if(!hFile->Read(nMipOffsets, 16))
	{
		CTexReader::ReleaseFileObject(hFile);
		return false;
	}

	LOAD_INTEL_INTA(nMipOffsets[0]);
	LOAD_INTEL_INTA(nMipOffsets[1]);
	LOAD_INTEL_INTA(nMipOffsets[2]);
	LOAD_INTEL_INTA(nMipOffsets[3]);
	
	if(!CTexReader::SeekToFile(pTex))
	{
		CTexReader::ReleaseFileObject(hFile);
		return false;
	}

	// Seek to the offset.
	hFile->Seek(nMipOffsets[0], CFile::current);

	// Allocate texture storage.
	pTex->FreeData();
	pTex->m_nBits = 8;
	pTex->m_nSize = pTex->m_nWidth * pTex->m_nHeight;
	pTex->m_pData = new unsigned char[pTex->m_nSize];
	if(pTex->m_pData == NULL)
	{
		CTexReader::ReleaseFileObject(hFile);
		return false;
	}

	// Load.
	if(!hFile->Read(pTex->m_pData, pTex->m_nSize))
	{
		CTexReader::ReleaseFileObject(hFile);
		return false;
	}

	CTexReader::ReleaseFileObject(hFile);

	return pTex->To24BitColor();
}