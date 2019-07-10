// HLWALREADER.CPP
// Copyright © 2000 Joe Riedel, Nick Randal.
// Author: Nick Randal.
#include "stdafx.h"
#include "HLWalReader.h"
#include "TexAPI.h"
#include "futils.h"

CHLWalReader::CHLWalReader() : CTexReader()
{}

CHLWalReader::~CHLWalReader()
{}

// Reads info from a Q1. Wal File.
bool CHLWalReader::LoadTextureInfo(CTexture* pTex)
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
bool CHLWalReader::LoadTexture(CTexture* pTex)
{
	CFile* hFile;
	int nOffset;

	nOffset = CTexReader::GetFileOffset(pTex);

	// Have the pak seek our file position.
	if(!CTexReader::SeekToFile(pTex))
		return false;
	
	hFile = (CFile*)CTexReader::GetFileObject(pTex);
	if(hFile == NULL)
		return false;

	// Skip forward 16+8 (Skip Width/Height).
	hFile->Seek(24, CFile::current);

	int nMipOffsets[4];
	if(!hFile->Read(nMipOffsets, 4*4))
	{
		CTexReader::ReleaseFileObject(hFile);
		return false;
	}

	LOAD_INTEL_INTA(nMipOffsets[0]);
	LOAD_INTEL_INTA(nMipOffsets[1]);
	LOAD_INTEL_INTA(nMipOffsets[2]);
	LOAD_INTEL_INTA(nMipOffsets[3]);
	
	// Seek to the offset.
	hFile->Seek(nOffset + nMipOffsets[0], CFile::begin);

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

	int nSeekpos;

	nSeekpos = nOffset + nMipOffsets[3] + (pTex->m_nSize >> 6) + 2;
	hFile->Seek(nSeekpos, CFile::begin);
	
	unsigned char* pPalette = new unsigned char[768];
	if(pPalette == NULL)
	{
		CTexReader::ReleaseFileObject(hFile);
		return false;
	}

	pTex->m_pPalette = new CTexPalette(pPalette);
	if(pTex->m_pPalette == NULL)
	{
		CTexReader::ReleaseFileObject(hFile);
		return false;
	}

	hFile->Read(pPalette, 768);

	CTexReader::ReleaseFileObject(hFile);

	return pTex->To24BitColor();
}