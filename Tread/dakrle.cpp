// DAKRLE.CPP
// Copyright © 2000 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#include "stdafx.h"
#include "dakrle.h"
#include "treadsys.h"

unsigned char* DecompressDAKRLEBlock(unsigned char* pSrc, int nLength, int nDstLength)
{
	int i;
	unsigned char* pDst;
	unsigned char code;
	int nDst, nSrc;

	pDst = new unsigned char[nDstLength];
	if(!pDst)
	{
		Warning("DecompressDAKRLEBlock: allocation for outbuf failed!\n");
		return NULL;
	}

	nDst = nSrc = 0;
	for(;;)
	{
		code = pSrc[nSrc++];
		if(code == 255)	// Done
			break;
		if(code < 64)
		{
			// uncompressed.
			for(i = -1; i < (int)code; i++)
			{
				if(nDst >= nDstLength)
				{
					Warning("DecompressDAKRLEBlock: decompress size exceeds dstlength!\n");
					delete[] pDst;
					return NULL;
				}
				pDst[nDst++] = pSrc[nSrc++];
			}
		}
		else
		if(code < 128)
		{
			// this is a bit spanky.
			for(i = 62; i < (int)code; i++)
			{
				if(nDst >= nDstLength)
				{
					Warning("DecompressDAKRLEBlock: decompress size exceeds dstlength!\n");
					delete[] pDst;
					return NULL;
				}
				pDst[nDst++] = 0;
			}
		}
		else
		if(code < 192)
		{
			for(i = 126; i < (int)code; i++)
			{
				if(nDst >= nDstLength)
				{
					Warning("DecompressDAKRLEBlock: decompress size exceeds dstlength!\n");
					delete[] pDst;
					return NULL;
				}
				pDst[nDst++] = pSrc[nSrc];
			}
			nSrc++;
		}
		else
		if(code < 254)
		{
			// umm... right.
			int offset;
			offset = (int)pSrc[nSrc++];
			for(i = 190; i < (int)code; i++)
			{
				if(nDst >= nDstLength)
				{
					Warning("DecompressDAKRLEBlock: decompress size exceeds dstlength!\n");
					delete[] pDst;
					return NULL;
				}
				pDst[nDst] = pDst[nDst-offset-2];
				nDst++;
			}
		}
	}

	return pDst;
}