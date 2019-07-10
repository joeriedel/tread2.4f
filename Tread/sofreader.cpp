// SOFREADER.CPP
// Copyright © 2000 Joe Riedel, Nick Randal.
// Author: Nick Randal.
#include "stdafx.h"
#include "RenderAPI.h"
#include "SofReader.h"
#include "futils.h"

CSofReader::CSofReader() : CTexReader()
{}

CSofReader::~CSofReader()
{}

int CSofReader::TranslateSurfaceToRenderFlags(int nFlags)
{
	// Note we need to fill this in!.
	return 0;
}

// Reads info from a Q2. Wal File.
bool CSofReader::LoadTextureInfo(CTexture* pTex)
{
	CFile* hFile;
	
	// Have the pak seek our file position.
	if(!CTexReader::SeekToFile(pTex))
		return false;
	
	hFile = (CFile*)CTexReader::GetFileObject(pTex);
	if(hFile == NULL)
		return false;

	// Skip forward 516.
	hFile->Seek(516, CFile::current);

	// Read.
	if(!ReadInt(*hFile, pTex->m_nWidth))
	{
		CTexReader::ReleaseFileObject(hFile);
		return false;
	}

	// Skip forward 60.
	hFile->Seek(60, CFile::current);

	if(!ReadInt(*hFile, pTex->m_nHeight))
	{
		CTexReader::ReleaseFileObject(hFile);
		return false;
	}
	
	pTex->m_nSize = pTex->m_nWidth * pTex->m_nHeight * 4;

	// Skip 124.
	hFile->Seek(124, CFile::current);

	// Read : Contents : Surface : Value.
	if(!ReadInt(*hFile, pTex->m_nSurface))
	{
		CTexReader::ReleaseFileObject(hFile);
		return false;
	}

	if(!ReadInt(*hFile, pTex->m_nContents))
	{
		CTexReader::ReleaseFileObject(hFile);
		return false;
	}

	if(!ReadInt(*hFile, pTex->m_nValue))
	{
		CTexReader::ReleaseFileObject(hFile);
		return false;
	}

	pTex->m_nRenderFlags = TranslateSurfaceToRenderFlags(pTex->m_nSurface);

	CTexReader::ReleaseFileObject(hFile);
	return true;
}

// Reads a Q2. Wal File.
bool CSofReader::LoadTexture(CTexture* pTex)
{
	CFile* hFile;
	unsigned char* pBuffer, * pPtr;
	int nLength;
	int nDim;

	// Have the pak seek our file position.
	if(!CTexReader::SeekToFile(pTex))
		return false;
	
	hFile = (CFile*)CTexReader::GetFileObject(pTex);
	if(hFile == NULL)
		return false;

	// Skip forward 644.
	hFile->Seek(644, CFile::current);

	int nMipOffset;
	if(!ReadInt(*hFile, nMipOffset))
	{
		CTexReader::ReleaseFileObject(hFile);
		return false;
	}

	// Seek to the offset.
	hFile->Seek(nMipOffset-648, CFile::current);

	// Allocate texture storage.
	pTex->FreeData();
	pTex->m_nBits = 24;
	nDim = pTex->m_nWidth * pTex->m_nHeight;
	pTex->m_nSize = nDim * 3;
	
	nLength = nDim * 4;
	pBuffer = new unsigned char[nLength];
	if(pBuffer == NULL)
		return false;
	hFile->Read(pBuffer, nLength);
	
	pTex->m_pData = new unsigned char[pTex->m_nSize];
	if(pTex->m_pData == NULL)
		return false;

	pPtr = pTex->m_pData;

	//the color scheme is bgra
	for(int n = 0; n < nDim; n++)
	{
		*(pPtr++) = *(pBuffer+2);
		pBuffer++;
		*(pPtr++) = *(pBuffer++);
		*(pPtr++) = *(pBuffer-2);
		pBuffer++;

		// Buffer is always advanced 4.
		pBuffer++;
	}

	// Load.
	//if(!hFile->Read(pTex->m_pData, pTex->m_nSize))
	{
	//	CTexReader::ReleaseFileObject(hFile);
	//	return false;
	}

	CTexReader::ReleaseFileObject(hFile);

	return true;
}