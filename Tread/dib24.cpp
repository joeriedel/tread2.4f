// DIB24.CPP - Dib24 library.
// Copyright © 1999 Joe Riedel, Nick Randal.
// Author: Joe Riedel.

#include "stdafx.h"
#include "dib24.h"

typedef struct
{
	BITMAPINFOHEADER bmiHeader;
	RGBQUAD bmiColors;
} BITMAPDIB24;

// Blits the bitmap onto the window.
void BlitDib24(CDC* pDC, CDC* bmDC, RECT size)
{
	// Blit it to the screen.
	pDC->BitBlt(0, 0, size.right, size.bottom, bmDC, 0, 0, SRCCOPY);
}

// Creates a 24 bit bitmaps of RECT size.
HBITMAP CreateDib24(HDC hdc, RECT size)
{
	BYTE* bits;
	HBITMAP bmhandle;
	BITMAPDIB24 BMInfo;
	DEVMODE devmode;

	// Get the current display mode.
	EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &devmode);

	memset(&BMInfo, 0, sizeof(BITMAPDIB24));
	BMInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	BMInfo.bmiHeader.biWidth = size.right;
	BMInfo.bmiHeader.biHeight = size.bottom;
	BMInfo.bmiHeader.biPlanes = 1;
	BMInfo.bmiHeader.biBitCount = (devmode.dmBitsPerPel > 8) ? (USHORT)devmode.dmBitsPerPel : (USHORT)16;
	BMInfo.bmiHeader.biCompression = BI_RGB;	// No compression.
	BMInfo.bmiHeader.biSizeImage = NULL;
	BMInfo.bmiHeader.biXPelsPerMeter = NULL;
	BMInfo.bmiHeader.biYPelsPerMeter = NULL;
	BMInfo.bmiHeader.biClrUsed = 0;//256;
	BMInfo.bmiHeader.biClrImportant = 0;//256;

	bmhandle = CreateDIBSection(hdc, (BITMAPINFO*)&BMInfo, DIB_RGB_COLORS, 
		(VOID**)&bits, NULL, NULL);

	return(bmhandle);
}

// Deletes the bitmap.
void DeleteDib24(HBITMAP bm)
{
	DeleteObject(bm);
}