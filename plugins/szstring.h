// SZSTRING.H
// Copyright © 1999 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#ifndef SZSTRING_H
#define SZSTRING_H

void StrLower(char* szString);
void StrUpper(char* szString);
bool CompareExt(const char* szString, const char* szExt);
// Include the '.', ex SetFileExtension(src, ".EXT");
CString SetFileExtension(const char* szSrc, const char* szExt);

#endif