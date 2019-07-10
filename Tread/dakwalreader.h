// DAKWALREADER.H
// Copyright © 2000 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#ifndef DAKWALREADER_H
#define DAKWALREADER_H

// Loads. Daikatana(tm) .wal files. Daikatana(tm) is © by Ion Storm®, All Rights Reserved.

#include "TexReader.h"

class CDakWalReader : public CTexReader
{
public:

	CDakWalReader();
	~CDakWalReader();

	// Reads info from a Q2. Wal File.
	bool LoadTextureInfo(CTexture* pTex);

	// Reads a Q2. Wal File.
	bool LoadTexture(CTexture* pTex);
};

#endif
