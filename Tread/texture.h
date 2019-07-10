// TEXTURE.H
// Copyright © 1999 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#ifndef TEXTURE_H
#define TEXTURE_H

#include "TexPalette.h"
#include "TexReader.h"
#include "mathlib.h"
#include "TExport.h"

// $sb "texture.h block1"
// Defines a texture.
class _T_EXPORT CTexture
{

public:

	// Linked list links.
	CTexture* prev, *next;

	// Constructor.
	CTexture();
	
	// Destructor.
	virtual ~CTexture();

	CString m_sName;						// Texture Display Name.
	CString m_sRealName;					// The real texture name.
	CString m_sFilename;					// The filename.
	int m_nUserFlags;						// Pak defined flags.
	int m_nRenderFlags;						// Flags that get rendered onto the object.
	int m_nExtraFlags;						// More flags.
	int m_nUniqueID;						// Unique Identifier.
	int m_nBits;							// 8, 24, 32 == Alpha bit color.
	int m_nHandle;							// Handle (for Reader).
	int m_nWidth, m_nHeight;				// Width/Height.
	int m_nSclWidth, m_nSclHeight;			// Scaled height (for non-pwr of 2).
	int m_nContents, m_nSurface, m_nValue;	// Q2 Surface/Contents.
	int m_nSize;							// Size of data block.
	unsigned char* m_pData;					// Image data.
	vec3_t m_vColor;						// For line/solid mode.
	bool m_bInfo;							// true if info is loaded.
	bool m_bApplyGamma;						// true if we need to apply gamma.
	CTexReader* m_pReader;					// The Reader.
	CTexPalette* m_pPalette;				// Palette to display with.

	virtual bool Load();

	// Loads only info.
	virtual bool LoadInfo();

	// Free's any memory.
	virtual bool Free();

	// Free's the texture data.
	virtual bool FreeData();

	// Converts a texture to 24bit color.
	virtual bool To24BitColor();
	virtual bool ApplyGamma();

};
// $eb

#endif