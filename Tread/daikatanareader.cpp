// DaikatanaREADER.CPP
// Copyright © 2000 Joe Riedel, Nick Randal.
// Author: Nick Randal.
#include "stdafx.h"
#include "RenderAPI.h"
#include "DaikatanaReader.h"

CDaikatanaReader::CDaikatanaReader() : CTexReader()
{}

CDaikatanaReader::~CDaikatanaReader()
{}

int CDaikatanaReader::TranslateSurfaceToRenderFlags(int nFlags)
{
	if(nFlags&(Q2_SURF_TRANS33|Q2_SURF_TRANS66))
		return RF_TRANSPARENT;
	return 0;
}

// Reads info from a Q2. Wal File.
bool CDaikatanaReader::LoadTextureInfo(CTexture* pTex)
{
	CFile* hFile;
	
	// Have the pak seek our file position.
	if(!CTexReader::SeekToFile(pTex))
		return false;
	
	hFile = (CFile*)CTexReader::GetFileObject(pTex);
	if(hFile == NULL)
		return false;

	// Skip forward 33.
	hFile->Seek(33, CFile::current);

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

	// Skip 68.
	hFile->Seek(68, CFile::current);

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

	// Skip 768.
	hFile->Seek(768, CFile::current);

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
bool CDaikatanaReader::LoadTexture(CTexture* pTex)
{
	CFile* hFile;

	// Have the pak seek our file position.
	if(!CTexReader::SeekToFile(pTex))
		return false;
	
	hFile = (CFile*)CTexReader::GetFileObject(pTex);
	if(hFile == NULL)
		return false;

	// Skip forward 33+8 (Skip Width/Height).
	hFile->Seek(41, CFile::current);

	int nMipOffsets[9];
	if(!hFile->Read(nMipOffsets, 36))
	{
		CTexReader::ReleaseFileObject(hFile);
		return false;
	}

	hFile->Seek(40, CFile::current);

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