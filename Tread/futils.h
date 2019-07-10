// FUTILS.H -- File utils.
// Copyrigth © 1999 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#ifndef FUTILS_H
#define FUTILS_H

#include "mathlib.h"

#ifdef _WIN32
#include <io.h>
#elif macintosh
#include <Files.h>
#include <TextUtils.h>
#include <Resources.h>
#endif

unsigned char* BloadFile(CString sFilename, int& nNumBytes);

class CFileSearch
{
private:

	CFileSearch* m_pSubSearch;
	bool SetupRecursiveSearch();
	bool GetNextRecursiveFile(CString& sFilename);
	bool KillRecursiveSearch();
	CString m_sDir, m_sFileExt;
	bool m_bRecursive;
	
#ifdef _WIN32
	bool m_bFirstFind;
	struct _finddata_t m_fileinfo;
	int	m_nHandle;
#elif macintosh
	int m_nIndex;
	short m_nVRefNum;
	long m_nDrDirID; 
	CInfoPBRec pbRec;
	CString m_sCurFileName;
	
	bool MatchesFileExt(const char* szFile, const CString& sExt);
	
#endif

public:

	CFileSearch();
	virtual ~CFileSearch();
	
	bool BeginFileSearch(CString sDir, CString sFileExt, bool bRecursive=false);
	void EndFileSearch();
	bool GetNextFile(CString& sFilename);

};

// Endian flip routines.
#ifdef macintosh
#define LOAD_INTEL_SHORT(a)			(((a&0xFF)<<8)+((a&0xFF00)>>8))
#define LOAD_INTEL_INT(a)			(((a&0xFF)<<24)+((a&0xFF00)<<8)+((a&0xFF0000)>>8)+((a&0xFF000000)>>24))
#define LOAD_INTEL_SHORTA(a) 		(a = LOAD_INTEL_SHORT(a))
#define LOAD_INTEL_INTA(a)			(a = LOAD_INTEL_INT(a))
#define LOAD_INTEL_FLOATA(a)		(a = LOAD_INTEL_FLOAT(a))
#define LOAD_INTEL_VEC2T(a)			LOAD_INTEL_FLOATP(a[0]); LOAD_INTEL_FLOATP(a[1])
#define LOAD_INTEL_VEC3T(a)			LOAD_INTEL_VEC2T(a); LOAD_INTEL_FLOATP(a[2])	
inline float LOAD_INTEL_FLOAT( const float &f )
{
	union {
		unsigned u;
		float f;
	} temp;
	temp.u = __lwbrx(const_cast<float *>(&f),0);
	return temp.f;
}

#define CSZP(a) ConvertUnixPath(a)
#define CCSP(a) a.ConvertUnixPath()

#else
#define LOAD_INTEL_SHORT(a)			(a)		
#define LOAD_INTEL_INT(a)			(a)
#define LOAD_INTEL_FLOAT(a)			(a)
#define LOAD_INTEL_SHORTA(a)
#define LOAD_INTEL_INTA(a)
#define LOAD_INTEL_FLOATA(a)
#define LOAD_INTEL_VEC2T(a)
#define LOAD_INTEL_VEC3T(a)		
#define CSZP(a)
#define CCSP(a)
#endif

void ConvertUnixPath(char* szString);
bool ReadNString(CFile& hFile, CString& sString, int nLen);
bool WriteNString(CFile& hFile, const char* szString, int nLen);
bool ReadString(CFile& hFile, CString& sString);
bool WriteString(CFile& hFile, const char* szString);
bool ReadLongString(CFile& hFile, CString& sString);
bool ReadShort(CFile& hFile, int& nInt);
bool WriteShort(CFile& hFile, int nInt);
bool ReadInt(CFile& hFile, int& nInt);
bool WriteInt(CFile& hFile, int nInt);
bool ReadBool(CFile& hFile, bool& bVal);
bool WriteBool(CFile& hFile, bool bVal);
bool ReadFloat(CFile& hFile, float& fFloat);
bool WriteFloat(CFile& hFile, float fFloat);
bool ReadVec2t(CFile& hFile, vec2_t vVec);
bool WriteVec2t(CFile& hFile, vec2_t vVec);
bool ReadVec3t(CFile& hFile, vec3_t vVec);
bool WriteVec3t(CFile& hFile, vec3_t vVec);

#endif