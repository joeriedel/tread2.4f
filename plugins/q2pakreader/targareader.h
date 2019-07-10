// Q2WALREADER.H
// Copyright © 1999 Joe Riedel, Nick Randal.
// Author: Nick Randal.
#ifndef TARGAREADER_H
#define TARGAREADER_H

// Loads. Quake2(tm) .wal files. Quake2(tm) is © by id Software®, All Rights Reserved.

// Reads a Q2 .wal file.
#include "../TreadAPI.h"

class CTargaReader : public CTexReader
{
public:

	CTargaReader();
	~CTargaReader();

	// Reads info from a Q2. Wal File.
	bool LoadTextureInfo(CTexture* pTex);

	// Reads a Q2. Wal File.
	bool LoadTexture(CTexture* pTex);

};

#endif