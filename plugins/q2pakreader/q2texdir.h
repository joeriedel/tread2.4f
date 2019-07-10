// Q2TEXDIR.H
// Copyright © 2000 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#ifndef Q2TEXDIR_H
#define Q2TEXDIR_H

//
// A texture directory is no different than a texture package. The readers, also, are not inherently different.
// The directory loaders derrive from CTexDirectory, which itself derrives from CPakFile. The CTexDirectory class
// simply removes some of the useless CPakFile virtual routines, like SeekToFile() and GetFileOffset().
//
// In this example we derrive from CGenTexDirectory. This class automates the recursive searching of directories
// and provides some event ovveridable for filtering textures.
// 

#include "../TreadAPI.h"
#include "GenTexDir.h"
#include "TargaReader.h"
#include "Q2WalReader.h"

class CQ2TexDir : public CGenTexDirectory
{
protected:

	CString m_sType;
	CTargaReader m_TargaReader;
	CQ2WalReader m_Q2WalReader;

public:

	virtual bool SkipFile(CString sFilename, CString* pFilter);
	virtual bool GetFileFilters(int& nNumFilters, CString* pFilters);
	virtual void ProcessTexture(CTexture* pTexture, CString* pFilter);

	virtual int GetPakClass() { return PAK_CLASS_QUAKE2DIR; }
};

#endif