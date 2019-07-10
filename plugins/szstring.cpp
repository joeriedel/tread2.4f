// SZSTRING.CPP
// Copyright © 1999 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#include "stdafx.h"
#include "szstring.h"
#include <string.h>
#include <ctype.h>

// Make the string lower case.
void StrLower(char* szString)
{
	int i;

	for(i = 0; szString[i] != NULL; i++)
		szString[i] = tolower(szString[i]);
}

// Make the string upper case.
void StrUpper(char* szString)
{
	int i;

	for(i = 0; szString[i] != NULL; i++)
		szString[i] = toupper(szString[i]);
}

// Returns true if the extensions match.
bool CompareExt(const char* szString, const char* szExt)
{
	int nStrLen, nExtLen;

	nStrLen = strlen(szString);
	nExtLen = strlen(szExt);

	if(nStrLen < nExtLen)
		return false;

	return !strncmp(&szString[nStrLen-nExtLen], szExt, nExtLen);
}

// Changes/Sets file extension.
CString SetFileExtension(const char* src, const char* ext)
{
	int len = 0;
	int extlen = 0;
	bool hasext = false;
	char temp[1024];
	char dst[1024];

	// Make sure the string has an extension.
	while(src[len] != NULL)
	{
		if(src[len] == '.')
			hasext = true;
		
		if(hasext)
			extlen++;

		len++;
	}

	// Subtract string.
	memset(temp, 0, 1024);
	
	if(hasext == true)
		strncpy(temp, src, strlen(src) - extlen);
	else
		strcpy(temp, src);

	if(ext) { strcat(temp, ext); }

	strcpy(dst, temp);
	return dst;
}