// DIB24.H - Library for using 24 bit DIBS for quick drawing.
// Copyright © 1999 Joe Riedel, Nick Randal
// Author: Joe Riedel.

#ifndef DIB24_H
#define DIB24_H

HBITMAP CreateDib24(HDC hdc, RECT size);
void DeleteDib24(HBITMAP);
void BlitDib24(CDC* pDC, CDC* bmDC, RECT size);

#endif