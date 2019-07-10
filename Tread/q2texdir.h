// Q2TEXDIR.H
// Copyright © 2000 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#ifndef Q2TEXDIR_H
#define Q2TEXDIR_H

#include "GenTexDir.h"
#include "TargaReader.h"
#include "Q2WalReader.h"
#include "TexDir.h"

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

};

#endif