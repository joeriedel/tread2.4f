// Q2WALREADER.H
// Copyright © 1999 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#ifndef Q2WALREADER_H
#define Q2WALREADER_H

// Loads. Quake2(tm) .wal files. Quake2(tm) is © by id Software®, All Rights Reserved.

// Reads a Q2 .wal file.
#include "TexReader.h"

#define	Q2_SURF_TRANS33	0x10
#define	Q2_SURF_TRANS66	0x20

class CQ2WalReader : public CTexReader
{
public:

	CQ2WalReader();
	~CQ2WalReader();

	int TranslateSurfaceToRenderFlags(int nFlags);

	// Reads info from a Q2. Wal File.
	bool LoadTextureInfo(CTexture* pTex);

	// Reads a Q2. Wal File.
	bool LoadTexture(CTexture* pTex);
};

#endif