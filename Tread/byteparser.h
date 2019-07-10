// BYTEPARSER.H
// Copyright © 2000 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#ifndef BYTEPARSER_H
#define BYTEPARSER_h

#define MAKECOPY	0x00000001
#define BP_BEGIN	0x00000000
#define BP_CUR		0x00000001
#define BP_END		0x00000002

class CByteParser
{
protected:

	int m_nOffset, m_nLength;
	unsigned char* m_pData;
	bool m_bDelete;
	bool m_bEOF;

public:

	CByteParser();
	CByteParser(unsigned char* pData, int nLength, int nFlags=0);
	~CByteParser();

	void Seek(int nOffset, int nFlags);
	void Skip(int nNum) { Seek(nNum, BP_CUR); }
	void AttachData(unsigned char* pData, int nLength, int nFlags=0);
	void DetachData();
	bool GetEOF();
	void ClearEOF();

	void SetAutoDelete(bool bDelete=true) { m_bDelete = bDelete; }

	int GetInt();
	float GetFloat();
	int GetByte();
	int GetShort();
	void GetData(unsigned char* pBuff, int nLength);
};

#endif