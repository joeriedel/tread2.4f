// SOFREADER.H
// Copyright © 1999 Joe Riedel, Nick Randal.
// Author: Nick Randal.
#ifndef SOFREADER_H
#define SOFREADER_H

#include "TexReader.h"

class CSofReader : public CTexReader
{
public:

	CSofReader();
	~CSofReader();

	int TranslateSurfaceToRenderFlags(int nFlags);

	// Reads info from a Q2. Wal File.
	bool LoadTextureInfo(CTexture* pTex);

	// Reads a Q2. Wal File.
	bool LoadTexture(CTexture* pTex);
};

#endif