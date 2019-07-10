// BYTEPARSER.CPP
// Copyright © 2000 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#include "stdafx.h"
#include "byteparser.h"
#include "futils.h"

CByteParser::CByteParser()
: m_pData(0),
m_nOffset(0),
m_nLength(0),
m_bDelete(false)
{
}

CByteParser::CByteParser(unsigned char* pData, int nLength, int nFlags)
{
	AttachData(pData, nLength, nFlags);
}

CByteParser::~CByteParser()
{
	DetachData();
}

void CByteParser::GetData(unsigned char* pBuff, int nLength)
{
	if(m_nLength > -1 && nLength+m_nOffset >= m_nLength)
	{
		m_bEOF = true;
		return;
	}

	memcpy(pBuff, &m_pData[m_nOffset], nLength);
	m_nOffset += nLength;
}

void CByteParser::Seek(int nOffset, int nFlags)
{
	if(nFlags == BP_BEGIN)
		m_nOffset = nOffset;
	else if(nFlags == BP_CUR)
		m_nOffset += nOffset;
	else if(nFlags == BP_END)
		m_nOffset = m_nLength + nOffset;

	if(m_nLength > -1 && m_nOffset >= m_nLength)
		m_bEOF = true;
}

bool CByteParser::GetEOF()
{
	return m_bEOF;
}

void CByteParser::ClearEOF()
{
	m_bEOF = false;
}

int CByteParser::GetByte()
{
	if(m_nLength > -1 && m_nOffset >= m_nLength)
	{
		m_bEOF=true;
		return 0;
	}

	return (int)(m_pData[m_nOffset++]&0xFF);	
}

int CByteParser::GetShort()
{
	int n;

	n = GetByte();
	n += (GetByte()<<8);
	
	return (int)LOAD_INTEL_SHORT(n);
}

int CByteParser::GetInt()
{
	int n;

	n = GetByte();
	n += (GetByte()<<8);
	n += (GetByte()<<16);
	n += (GetByte()<<24);
	return LOAD_INTEL_INT(n);
}

float CByteParser::GetFloat()
{
	return (float)GetInt();
}

void CByteParser::DetachData()
{
	if(!m_bDelete)
		return;
	if(m_pData == NULL)
		return;
	delete[] m_pData;
	m_pData = NULL;
	m_nLength = 0;
	m_nOffset = 0;
}

void CByteParser::AttachData(unsigned char* pData, int nLength, int nFlags)
{
	m_bDelete = false;
	if(nFlags==MAKECOPY)
	{
		m_pData = new unsigned char[nLength];
		memcpy(m_pData, pData, nLength);
		m_bDelete = true;
	}
	else
		m_pData = pData;

	m_nOffset = 0;
	m_nLength = nLength;
	ClearEOF();
}