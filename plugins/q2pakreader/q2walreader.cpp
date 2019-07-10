// Q2WALREADER.CPP
// Copyright © 2000 Joe Riedel, Nick Randal.
// Author: Nick Randal.
#include "stdafx.h"
#include "../TreadAPI.h"
#include "Q2WalReader.h"

CQ2WalReader::CQ2WalReader() : CTexReader()
{}

CQ2WalReader::~CQ2WalReader()
{}

int CQ2WalReader::TranslateSurfaceToRenderFlags(int nFlags)
{
	if(nFlags&(Q2_SURF_TRANS33|Q2_SURF_TRANS66))
		return RF_TRANSPARENT;
	return 0;
}

// Reads info from a Q2. Wal File.
bool CQ2WalReader::LoadTextureInfo(CTexture* pTex)
{
	CFile* hFile;
	
	// Have the pak seek our file position.
	if(!CTexReader::SeekToFile(pTex))
		return false;
	
	hFile = (CFile*)CTexReader::GetFileObject(pTex);
	if(hFile == NULL)
		return false;

	// Skip forward 32.
	hFile->Seek(32, CFile::current);

	// Read.
	if(!hFile->Read(&pTex->m_nWidth, 4))
	{
		CTexReader::ReleaseFileObject(hFile);
		return false;
	}

	if(!hFile->Read(&pTex->m_nHeight, 4))
	{
		CTexReader::ReleaseFileObject(hFile);
		return false;
	}
	
	pTex->m_nSize = pTex->m_nWidth * pTex->m_nHeight * 3;

	// Skip 48.
	hFile->Seek(48, CFile::current);

	// Read : Contents : Surface : Value.
	if(!hFile->Read(&pTex->m_nSurface, 4))
	{
		CTexReader::ReleaseFileObject(hFile);
		return false;
	}

	if(!hFile->Read(&pTex->m_nContents, 4))
	{
		CTexReader::ReleaseFileObject(hFile);
		return false;
	}

	if(!hFile->Read(&pTex->m_nValue, 4))
	{
		CTexReader::ReleaseFileObject(hFile);
		return false;
	}

	pTex->m_nRenderFlags = TranslateSurfaceToRenderFlags(pTex->m_nSurface);

	CTexReader::ReleaseFileObject(hFile);
	return true;
}

// Reads a Q2. Wal File.
bool CQ2WalReader::LoadTexture(CTexture* pTex)
{
	CFile* hFile;

	// Have the pak seek our file position.
	if(!CTexReader::SeekToFile(pTex))
		return false;
	
	hFile = (CFile*)CTexReader::GetFileObject(pTex);
	if(hFile == NULL)
		return false;

	// Skip forward 32+8 (Skip Width/Height).
	hFile->Seek(40, CFile::current);

	int nMipOffsets[4];
	if(!hFile->Read(nMipOffsets, 16))
	{
		CTexReader::ReleaseFileObject(hFile);
		return false;
	}

	// Seek to the offset.
	hFile->Seek(nMipOffsets[0]-56, CFile::current);

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