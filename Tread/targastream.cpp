// TARGASTREAM.CPP
// Copyright © 2000 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#include "stdafx.h"
#include "targastream.h"
#include "byteparser.h"
#include "texture.h"
#include "treadsys.h"

CTargaStreamReader::CTargaStreamReader() : CTexReader()
{}

CTargaStreamReader::~CTargaStreamReader()
{}

// Reads info from a Q2. Wal File.
bool CTargaStreamReader::LoadTextureInfo(CTexture* pTex)
{
	unsigned char* pData;
	CByteParser bp;
	
	// Have the pak seek our file position.
	if(!CTexReader::SeekToFile(pTex))
		return false;
	
	pData = (unsigned char*)CTexReader::GetFileObject(pTex);
	if(pData == NULL)
		return false;

	bp.AttachData(pData, -1);

	bp.Seek(12, BP_CUR);
	pTex->m_nWidth = pTex->m_nHeight = 0;

	pTex->m_nWidth = bp.GetShort();
	pTex->m_nHeight = bp.GetShort();
	
	pTex->m_nSize = pTex->m_nWidth * pTex->m_nHeight * 3;

	CTexReader::ReleaseFileObject(pData);

	return true;
}

// Reads a Q2. Wal File.
bool CTargaStreamReader::LoadTexture(CTexture* pTex)
{
	unsigned char* pData;
	CByteParser bp;

	// Have the pak seek our file position.
	if(!CTexReader::SeekToFile(pTex))
		return false;
	
	pData = (unsigned char*)CTexReader::GetFileObject(pTex);
	if(pData == NULL)
		return false;

	bp.AttachData(pData, -1);

	unsigned char* pBuffer, *pBufPtr, *pPtr;
	int nLength = 0;
	int nType;
	unsigned char nFlags;
	int nOrig;
	int nBpp = 0;
	int nSize = 3;	//standard color depth
	int k, n;
	unsigned int nFC[3] = {0, 0, 0};

	nLength = bp.GetByte();
	bp.Seek(1, BP_CUR);
	nType = bp.GetByte();

	// Must be true color uncompressed.
	if(nType != 2)
	{
		pTex->m_pData = 0;
		return false;
	}

	bp.Seek(9, BP_CUR);
	pTex->m_nWidth = pTex->m_nHeight = 0;

	pTex->m_nWidth = bp.GetShort();
	pTex->m_nHeight = bp.GetShort();

	nBpp = bp.GetByte();
	if(nBpp == 32)
		nSize = 4;	//alternative color depth

	pTex->m_nBits = 24;

	nFlags = bp.GetByte();
	nOrig = (nFlags & 0x20) >> 5;

	bp.Seek(nLength, BP_CUR);

	// Read the raw uncompressed data.
	k = (pTex->m_nWidth * pTex->m_nHeight);
	nLength = pTex->m_nWidth * pTex->m_nHeight * nSize;
	pBuffer = new unsigned char[nLength];
	bp.GetData(pBuffer, nLength);
	
	if(nSize > 3)
	{
		nLength = pTex->m_nWidth * pTex->m_nHeight * 3;
		pTex->m_pData = new unsigned char[nLength];
		if(pTex->m_pData == NULL)
			return false;

		pPtr = pTex->m_pData;
		pBufPtr = pBuffer;

		for(n = 0; n < k; n++)
		{
			// Nick being anal.
			*(pPtr++) = *(pBufPtr++);
			*(pPtr++) = *(pBufPtr++);
			*(pPtr++) = *(pBufPtr++);

			// Buffer is always advanced 4.
			pBufPtr++;
		}
	}
	else
		pTex->m_pData = pBuffer;
	
	// Do we need to flip the image?
	if(nOrig == 0)	// yes.
	{
		k = pTex->m_nWidth * 3;

		// We need to allocate again if we didn't remove alpha channel.
		if(nSize == 3)
			pBuffer = new unsigned char[nLength];

		// Flip the image over the x axis.
		for(int row = 0; row < pTex->m_nHeight; row++)
		{
			int nFliprow = pTex->m_nHeight - row - 1;

			// Offsets;
			int component=0;
			int offset = row * k;
			int flipoffset = nFliprow * k;

			pBufPtr = &pBuffer[flipoffset];
			pPtr = &pTex->m_pData[offset];

			// Copy the scanline.
			for(n = 0, component=0; n < k; n++, component++)
			{
				// Add for flat color.
				nFC[component] += (unsigned int)*pPtr;
				if(component == 2)
					component = 0;
				
				*(pBufPtr++) = *(pPtr++);//pBuffer[flipoffset++] = pTex->m_pData[offset++];
			}
		}

		// Set up new data.
		delete [] pTex->m_pData;
		pTex->m_pData = pBuffer;
	}
	else if(nSize > 3)
		delete [] pBuffer;


	// We must add every pixel for flat color.
	pPtr = pTex->m_pData;
	for(k = 0; k < nLength; k+=(32*nSize))
	{
		nFC[0] += (unsigned int)*pPtr;
		nFC[1] += (unsigned int)*(pPtr+1);
		nFC[2] += (unsigned int)*(pPtr+2);
	}
	
	pTex->m_vColor[0] = (nFC[0] / (float)(nLength/32) / 255.0f);
	pTex->m_vColor[1] = (nFC[1] / (float)(nLength/32) / 255.0f);
	pTex->m_vColor[2] = (nFC[2] / (float)(nLength/32) / 255.0f);

	CTexReader::ReleaseFileObject(pData);

	return true;
}