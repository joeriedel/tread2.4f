// stdafx.h
//

#ifndef STDAFX_H
#define STDAFX_H

// Define basic types.
typedef int CWnd;
typedef unsigned int COLORREF;
typedef int HCURSOR;
typedef const char* LPCTSTR;
typedef char* LPSTR;
typedef unsigned int UINT;
typedef int INT;
typedef UINT DWORD;
typedef bool BOOL;
typedef void* LPVOID;

#define RGB(r, g, b) (r+(g<<8)+(b<<16))
#define GetRValue(c) (c&0xFF)
#define GetGValue(c) ((c&0xFF00)>>8)
#define GetBValue(c) ((c&0xFF0000)>>16)

// We include our standard string class.
#include "mac_cstring.h"
// Define the standard window CRect.
#include "mac_rect.h"
// Define our mac device context.
#include "mac_dc.h"
// Define or mac CFile object.
#include "mac_cfile.h"
// Define the mmystem.h call timeGetTime().
#include "mac_time.h"
#include "mac_syscalls.h"

#endif
