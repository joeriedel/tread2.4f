// TEXTURE.CPP
// Copyright © 1999 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#include "stdafx.h"
#include "TexAPI.h"
#include "Texture.h"
#include "GameAPI.h"
#include "TreadSys.h"
#include "Tread3D.h"

// Constructor.
CTexture::CTexture()
{
	m_pPalette = NULL;
	m_pData    = NULL;
	m_pReader  = NULL;
	m_nExtraFlags = 0;
	m_nUniqueID = 0;
	m_bInfo = false;
	m_bApplyGamma = true;
}
	
// Destructor.
CTexture::~CTexture()
{
	Free();
}

// Loads only info.
bool CTexture::LoadInfo()
{
	if(m_bInfo)
		return true;

	if(m_pReader == NULL)
		return false;

	if(m_pReader->LoadTextureInfo(this))
	{
		m_bInfo = true;
		return true;
	}

	return false;
}

// Free's any memory.
bool CTexture::Free()
{
	if(m_pReader == NULL)
		return false;
	return m_pReader->FreeTexture(this);
}

// Free's the texture data.
bool CTexture::FreeData()
{
	if(m_pReader == NULL)
		return false;
	if(m_pData == NULL)
		return true;
	return m_pReader->FreeTextureData(this);
}

// Loads the texture.
bool CTexture::Load()
{
	if(m_pReader == NULL)
		return false;
	if(m_pData)
		return true;

	GetTexAPI()->PreTextureLoad(this);
	bool b = m_pReader->LoadTexture(this);
	if(!b)
		return false;
	GetTexAPI()->PostTextureLoad(this);

	m_nSclWidth = m_nWidth;
	m_nSclHeight = m_nHeight;

	ApplyGamma();

	return true;
}

#define BASE_GAMMA 100.0F
bool CTexture::ApplyGamma()
{
	if(m_bApplyGamma == false)
		return false;

	// Multiply and clamp.
	int i;
	float c;
	float fFactor = ((float)GetRegInt(I_GAMMA)) / BASE_GAMMA;
	unsigned char* pData = m_pData;
	for(i = 0; i < m_nSize; i += 3)
	{
		// Compute and store.
		c = (float)*pData;
		c *= fFactor;
		*pData = (c > 255.0F) ? 255 : (unsigned char)c;

		pData++;
		// Compute and store.
		c = (float)*pData;
		c *= fFactor;
		*pData = (c > 255.0F) ? 255 : (unsigned char)c;

		pData++;
		// Compute and store.
		c = (float)*pData;
		c *= fFactor;
		*pData = (c > 255.0F) ? 255 : (unsigned char)c;

		pData++;
	}

	m_bApplyGamma = false;
	return true;
}

// Converts an 8 bit texture to a 24bit color image.
bool CTexture::To24BitColor()
{
	CTexPalette* pPal;

	if(m_nBits == 24)
		return true;

	if(m_nBits == 32)	// Scale down to 24.
	{
		unsigned char* pData = new unsigned char[m_nWidth*m_nHeight*3];
		int i, k;

		for(i = k = 0; i < m_nSize; i+=4, k+=3)
		{
			pData[k] = m_pData[i];
			pData[k+1] = m_pData[i+1];
			pData[k+2] = m_pData[i+2];
		}

		delete[] m_pData;
		m_nSize = m_nWidth*m_nHeight*3;
		m_pData = pData;
		return true;
	}

	// Get the palette.
	pPal = m_pPalette;
	if(!pPal)
	{
		// Get the reader's pal.
		if(m_pReader && m_pReader->GetPakFile())
			pPal = m_pReader->GetPakFile()->GetPalette();
		
		if(pPal == NULL)
		{
			// Get the game palette.
			pPal = GetGameAPI()->GetCurrentGame()->GetPalette();
		}

		if(pPal == NULL)
			return false;
	}

	int i, nOffset, nNewSize, nColor;
	unsigned char* pPalData, *pPalPtr;
	unsigned int nR, nG, nB;

	nNewSize = m_nSize * 3;
	//  Allocate temp buffer.
	unsigned char* pNewData = new unsigned char[nNewSize];
	if(pNewData == NULL)
		return false;

	// Grab the palette color array.
	pPalData = pPal->GetPalData();

	nR = nG = nB = 0;

	// Convert.
	for(i = 0; i < m_nSize; i++)
	{
		nColor = m_pData[i];

		nColor = nColor+nColor+nColor;
		nOffset = i+i+i;
		
		pPalPtr = &pPalData[nColor];

		pNewData[nOffset+2] = *pPalPtr;
		pNewData[nOffset+1] = *(pPalPtr+1);
		pNewData[nOffset] = *(pPalPtr+2);

		// Add for flat color.
		nR += (unsigned int)*pPalPtr;
		nG += (unsigned int)*(pPalPtr+1);
		nB += (unsigned int)*(pPalPtr+2);
	}

	m_vColor[0] = (nR / (float)m_nSize / 255.0f);
	m_vColor[1] = (nG / (float)m_nSize / 255.0f);
	m_vColor[2] = (nB / (float)m_nSize / 255.0f);

	// Store.
	delete[] m_pData;
	m_pData = pNewData;
	m_nSize = nNewSize;
	m_nBits = 24;

	return true;
}