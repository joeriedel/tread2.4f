// DAKRLE.H
// Copyright © 2000 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#ifndef DAKRLE_H
#define DAKRLE_H

// Implements Daikatana's very strange form of RLE.
unsigned char* DecompressDAKRLEBlock(unsigned char* pData, int nLength, int nOutLength);

#endif