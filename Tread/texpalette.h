// TEXPALETTE.H
// Copyright © 1999 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#ifndef TEXPALETTE_H
#define TEXPALETTE_H

#include "TExport.h"

// $sb "texpalette.h block1"
class _T_EXPORT CTexPalette
{
private:

	unsigned char* m_pPalData;

public:

	CTexPalette(unsigned char* pPalData)
	{
		m_pPalData = pPalData;
	}

	~CTexPalette()
	{
		if(m_pPalData)
			delete[] m_pPalData;
		m_pPalData = NULL;
	}

	unsigned char* GetPalData() { return m_pPalData; }
};
// $eb
#endif

