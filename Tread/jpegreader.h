// JPEGREADER.H
// Copyright © 2000 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#ifndef JPEGREADER_H
#define JPEGREADER_H

#include "TexReader.h"

//
// Reads a targa out of a stream.
//
class CJPEGReader : public CTexReader
{
public:

	CJPEGReader();
	~CJPEGReader();

	// Reads info from a Q2. Wal File.
	bool LoadTextureInfo(CTexture* pTex);

	// Reads a Q2. Wal File.
	bool LoadTexture(CTexture* pTex);

};


#endif