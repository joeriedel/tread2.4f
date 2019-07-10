// Q2WALREADER.H
// Copyright © 1999 Joe Riedel, Nick Randal.
// Author: Nick Randal.
#ifndef Q1WALREADER_H
#define Q1WALREADER_H

// Loads. Quake2(tm) .wal files. Quake2(tm) is © by id Software®, All Rights Reserved.

// Reads a Q2 .wal file.
#include "TexReader.h"

class CQ1WalReader : public CTexReader
{
public:

	CQ1WalReader();
	~CQ1WalReader();

	// Reads info from a Q1. Wal File.
	bool LoadTextureInfo(CTexture* pTex);

	// Reads a Q2. Wal File.
	bool LoadTexture(CTexture* pTex);

};

#endif