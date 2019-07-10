// TEXPORT.H
// Copyright © 2000 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#ifndef TEXPORT_H
#define TEXPORT_H

#ifdef _WIN32

#pragma warning(disable : 4251)

#define _T_EXPORT		_declspec (dllexport)
#define _T_IMPORT		_declspec (dllimport)

#else

#define _T_EXPORT
#define _T_IMPORT

#endif

#endif