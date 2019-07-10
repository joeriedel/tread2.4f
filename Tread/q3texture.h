// Q3TEXTURE.H
// Copyright © 2000 Joe Riedel.
// Author: Joe Riedel.
#ifndef Q3TEXTURE_H
#define Q3TEXTURE_H

#include "texture.h"


class CQ3Texture : public CTexture
{
public:

	struct _QSHADER* m_pShader;
	bool m_bLinkedToShader;

	CQ3Texture() : CTexture()
	{
	}

	~CQ3Texture()
	{
	}

};

#endif