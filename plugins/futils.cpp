// FUTILS.CPP
// Copyright © 1999 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#include "stdafx.h"
#include "futils.h"
#include "mathlib.h"

#ifdef _WIN32
#include <io.h>
#elif macintosh
#include <Files.h>
#include <TextUtils.h>
#include <Resources.h>
#include "szstring.h"
#endif

CFileSearch::CFileSearch()
{
	m_pSubSearch = 0;
	
#ifdef _WIN32
	m_nHandle = -1;
#endif

}

CFileSearch::~CFileSearch()
{
	EndFileSearch();
}

bool CFileSearch::BeginFileSearch(CString sDir, CString sFileExt, bool bRecursive)
{
	m_sDir = sDir;
	m_sFileExt = sFileExt;
	m_bRecursive = bRecursive;
	m_pSubSearch = NULL;
	
#ifdef _WIN32
	m_bFirstFind = true;
	m_nHandle = _findfirst(sDir + "\\" + sFileExt, &m_fileinfo);
	if(m_nHandle == -1)
		return false;
#elif macintosh
	char szFilename[1024];
	
	//strcpy(szFilename, "big daddy:projects:tread3d:source:output:");
	//strcat(szFilename, sDir);
	strcpy(szFilename, sDir);
	ConvertUnixPath(szFilename);
	c2pstr(szFilename);
	
	memset(&pbRec, 0, sizeof(pbRec));
	pbRec.dirInfo.ioNamePtr = (unsigned char*)szFilename;
	
	m_nIndex = 1;
	
	// Get the dir info.
	OSErr err = PBGetCatInfoSync(&pbRec);
	if(err != noErr)
		return false;
	if(!(pbRec.dirInfo.ioFlAttrib&kioFlAttribDirMask))
		return false;
		
	m_nDrDirID = pbRec.dirInfo.ioDrDirID;
	m_nVRefNum  = pbRec.dirInfo.ioVRefNum;
	
#endif
	return true;
}

void CFileSearch::EndFileSearch()
{
	KillRecursiveSearch();
#ifdef _WIN32
	if(m_nHandle > -1)
		_findclose(m_nHandle);
#endif
}

bool CFileSearch::SetupRecursiveSearch()
{
	m_pSubSearch = new CFileSearch();

#ifdef _WIN32
	if(m_pSubSearch->BeginFileSearch(m_sDir + "\\" + m_fileinfo.name, m_sFileExt, true))
		return true;
#elif macintosh
	if(m_pSubSearch->BeginFileSearch(m_sDir + ":" + m_sCurFileName, m_sFileExt, true))
		return true;
#endif

	KillRecursiveSearch();
	return false;
}

bool CFileSearch::GetNextRecursiveFile(CString& sFilename)
{
	m_pSubSearch->GetNextFile(sFilename);
	if(sFilename == "")
		return false;

#ifdef _WIN32
	sFilename = CString(m_fileinfo.name) + "\\" + sFilename;
#elif macintosh
	sFilename = m_sCurFileName + ":" + sFilename;
#endif
	return false;
}

bool CFileSearch::KillRecursiveSearch()
{
	if(m_pSubSearch != NULL)
		delete m_pSubSearch;
	m_pSubSearch = NULL;
	return true;
}

#if macintosh
bool CFileSearch::MatchesFileExt(const char* szFile, const CString& sExt)
{
	CString sFile = szFile;
	CString sExt2;
	
	sExt2 = sExt;
	if(sExt2[0] == '*')
		sExt2.Delete(0, 1);
	
	return CompareExt(szFile, sExt2);
}
#endif

bool CFileSearch::GetNextFile(CString& sFilename)
{
	// Doing a recursive search?
	if(m_pSubSearch)
	{
		GetNextRecursiveFile(sFilename);
		if(sFilename == "")
		{
			KillRecursiveSearch();
			return GetNextFile(sFilename);
		}

		return false;
	}
	
#ifdef _WIN32
	if(m_bFirstFind)
	{
		m_bFirstFind = false;
		sFilename = m_fileinfo.name;
		if(sFilename == ".." || sFilename == ".")
			return GetNextFile(sFilename);
		
		if(m_bRecursive && (m_fileinfo.attrib&_A_SUBDIR))
		{
			if(SetupRecursiveSearch())
			{
				// Get next file.
				GetNextRecursiveFile(sFilename);
				if(sFilename == "")
				{
					KillRecursiveSearch();
					return GetNextFile(sFilename);
				}

				// Return not a directory.
				return false;
			}
			else
				return GetNextFile(sFilename);
		}

		return (m_fileinfo.attrib & _A_SUBDIR) != 0 ? true : false;
	}

	if(_findnext(m_nHandle, &m_fileinfo) == -1)
	{
		sFilename = "";
		return false;
	}

	sFilename = m_fileinfo.name;
	if(sFilename == ".." || sFilename == ".")
		return GetNextFile(sFilename);

	if(m_bRecursive && (m_fileinfo.attrib&_A_SUBDIR))
	{
		if(SetupRecursiveSearch())
		{
			// Get next file.
			GetNextRecursiveFile(sFilename);
			if(sFilename == "")
			{
				KillRecursiveSearch();
				return GetNextFile(sFilename);
			}

			// Return not a directory.
			return false;
		}
		else
			return GetNextFile(sFilename);
	}

	return (m_fileinfo.attrib & _A_SUBDIR) != 0 ? true : false;
#else
	
	char szFilename[1024];
	
	for(;;)
	{
		memset(&pbRec, 0, sizeof(pbRec));
		pbRec.dirInfo.ioNamePtr = (unsigned char*)szFilename;
		pbRec.dirInfo.ioVRefNum = m_nVRefNum;
		pbRec.dirInfo.ioDrDirID = m_nDrDirID;
		pbRec.dirInfo.ioFDirIndex = m_nIndex++;
		
		if(PBGetCatInfoSync(&pbRec) != noErr)
		{
			sFilename = "";
			return false;
		}
		
		p2cstr((unsigned char*)szFilename);
		m_sCurFileName = szFilename;
		if(m_bRecursive && (pbRec.dirInfo.ioFlAttrib&kioFlAttribDirMask))
		{
			if(SetupRecursiveSearch())
			{
				GetNextRecursiveFile(sFilename);
				if(sFilename == "")
				{
					KillRecursiveSearch();
					return GetNextFile(sFilename);
				}
				
				return false;
			}
			else
				return GetNextFile(sFilename);
		}
		
		if(MatchesFileExt(szFilename, m_sFileExt))
			break;
	}
	
	sFilename = szFilename;
	
	return false;
	
#endif
}

bool ReadFloat(CFile& hFile, float& fFloat)
{
	float f;

	hFile.Read(&f, 4);
	fFloat = LOAD_INTEL_FLOAT(f);
	return true;
}

bool WriteFloat(CFile& hFile, float fFloat)
{
	LOAD_INTEL_FLOATA(fFloat);
	hFile.Write(&fFloat, 4);
	return true;
}

bool ReadBool(CFile& hFile, bool& bVal)
{
	char b = 0;
	hFile.Read(&b, 1);
	bVal = (b!=0)?true:false;
	return true;
}

bool WriteBool(CFile& hFile, bool bVal)
{
	char b = (bVal)?1:0;
	hFile.Write(&b, 1);
	return true;
}


bool ReadNString(CFile& hFile, CString& sString, int nLen)
{
	char* pChar = new char[nLen+1];
	if(pChar == NULL)
		return false;
	hFile.Read(pChar, nLen);
	pChar[nLen] = NULL;
	sString = pChar;
	delete[] pChar;
	return true;
}

bool WriteNString(CFile& hFile, const char* szString, int nLen)
{
	hFile.Write((void*)szString, nLen);
	return true;
}

bool ReadShort(CFile& hFile, int& nInt)
{
	short nNum=0;
	
	if(hFile.Read(&nNum, 2) != 2)
		return false;
		
	nInt = (int)LOAD_INTEL_SHORT(nNum);
	return true;
}

bool WriteShort(CFile& hFile, int nInt)
{
	short nNum;
	
	nNum = (short)nInt;
	LOAD_INTEL_SHORTA(nNum);
	hFile.Write(&nNum, 2);
	return true;
}

bool ReadInt(CFile& hFile, int& nInt)
{
	int nNum;

	hFile.Read(&nNum, 4);
	nInt = LOAD_INTEL_INT(nNum);
	return true;
}

bool WriteInt(CFile& hFile, int nInt)
{
	LOAD_INTEL_INTA(nInt);
	hFile.Write(&nInt, 4);
	return true;
}

bool WriteString(CFile& hFile, const char* szString)
{
	int nLen;
	if(szString != NULL)
		nLen = strlen(szString);
	else
		nLen = 0;
	WriteShort(hFile, nLen);
	if(nLen > 0)
		return WriteNString(hFile, szString, nLen);
	return true;
}

bool ReadLongString(CFile& hFile, CString& sString)
{
	int nLen=0;
	
	ReadInt(hFile, nLen);
	if(nLen > 0)
		return ReadNString(hFile, sString, nLen);
	sString = "";
	return true;
}

bool ReadString(CFile& hFile, CString& sString)
{
	int nLen=0;
	
	ReadShort(hFile, nLen);
	if(nLen > 0)
		return ReadNString(hFile, sString, nLen);
	sString = "";
	return true;
}

bool ReadVec2t(CFile& hFile, vec2_t vVec)
{
	return ReadFloat(hFile, vVec[0]) && ReadFloat(hFile, vVec[1]);
}

bool WriteVec2t(CFile& hFile, vec2_t vVec)
{
	return WriteFloat(hFile, vVec[0]) && WriteFloat(hFile, vVec[1]);
}

bool ReadVec3t(CFile& hFile, vec3_t vVec)
{
	return ReadFloat(hFile, vVec[0]) && ReadFloat(hFile, vVec[1]) && ReadFloat(hFile, vVec[2]);
}

bool WriteVec3t(CFile& hFile, vec3_t vVec)
{
	return WriteFloat(hFile, vVec[0]) && WriteFloat(hFile, vVec[1]) && WriteFloat(hFile, vVec[2]);
}

// Bloads a file into memory.
unsigned char* BloadFile(CString sFilename, int& nNumBytes)
{
	CFile hFile;

	nNumBytes = 0;

	if(!hFile.Open(sFilename, CFile::modeRead | CFile::shareDenyWrite | CFile::typeBinary))
		return NULL;

	nNumBytes = hFile.GetLength();
	if(nNumBytes == 0)
	{
		hFile.Close();
		return NULL;
	}

	// Allocate.
	unsigned char* pData = new unsigned char[nNumBytes];
	if(pData == NULL)
	{
		hFile.Close();
		nNumBytes = 0;
		return NULL;
	}

	// Load the file.
	if(hFile.Read(pData, nNumBytes) != (unsigned int)nNumBytes)
	{
		hFile.Close();
		nNumBytes = 0;
		delete[] pData;
		return NULL;
	}

	hFile.Close();

	return pData;
}

void ConvertUnixPath(char* szString)
{
	int i;
	int l = strlen(szString);
	
	for(i = 0; i < l; i++)
	{
		if(szString[i] == '\\' || szString[i] == '/')
			szString[i] = ':';
	}
}
