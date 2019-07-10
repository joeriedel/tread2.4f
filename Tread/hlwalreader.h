// HLWALREADER.H
// Copyright © 1999 Joe Riedel, Nick Randal.
// Author: Nick Randal.
#ifndef HLWALREADER_H
#define HLWALREADER_H

// Loads HalfLife .wal files © Valve Software LLC.

#include "TexReader.h"

class CHLWalReader : public CTexReader
{
public:

	CHLWalReader();
	~CHLWalReader();

	// Reads info from a Q1. Wal File.
	bool LoadTextureInfo(CTexture* pTex);
	// Reads a Q2. Wal File.
	bool LoadTexture(CTexture* pTex);
};

#endif