// Q3PAKBASE.H
// Copyright © 2000 Joe Riedel.
// Author: Joe Riedel.
#ifndef Q3BASE_H
#define Q3BASE_H

#include "q3texture.h"

class CQ3PakBase
{
public:

	virtual CQ3Texture* FindTexture(CString sFilename)=0;
};

#endif