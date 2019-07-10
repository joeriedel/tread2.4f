// TARGASTREAM.H
// Copyright © 2000 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#ifndef TARGASTREAM_H
#define TARGASTREAM_H

#include "TexReader.h"

//
// Reads a targa out of a stream.
//
class CTargaStreamReader : public CTexReader
{
public:

	CTargaStreamReader();
	~CTargaStreamReader();

	// Reads info from a Q2. Wal File.
	bool LoadTextureInfo(CTexture* pTex);

	// Reads a Q2. Wal File.
	bool LoadTexture(CTexture* pTex);

};


#endif