// QSHADERS.H
// Copyright © 2000 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#ifndef QSHADERS_H
#define QSHADERS_H

#include "q3texture.h"
#include "tokenizer.h"

// Shader parsing stuff.
typedef struct _QSHADER
{
	CString sName;
	CString sTexture;
	CQ3Texture* pTexture;
	bool bIgnored;
} QSHADER;

class CQShaders
{
public:

	static bool SkipShader(CTokenizer& Tokenizer, int nBraceCount);
	static int CountShadersInScript(const char* szShaderScript, int nLen);
	static int ParseShaderScript(const char* szShaderScript, int nLen, QSHADER* pList);

};

#endif