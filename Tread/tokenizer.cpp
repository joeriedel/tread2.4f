// TOKENIZER.CPP
// Copyright © 1999 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#include "stdafx.h"
#include "Tokenizer.h"

CTokenizer::CTokenizer()
{
	m_bUnget = false;
	m_pBuffer = NULL;
	m_pOffset = NULL;
	m_sToken  = "";
}

CTokenizer::~CTokenizer()
{
	FreeScript();
}

void CTokenizer::FreeScript()
{
	if(m_pBuffer != NULL)
		delete[] m_pBuffer;
	m_pBuffer = NULL;
}

void CTokenizer::Skip(int nNum)
{
	int i;
	CString s;

	for(i = 0; i < nNum; i++)
		GetToken(s);
}

bool CTokenizer::InitParsing(const char* szScript, int nNumBytes)
{
	FreeScript();

	if(nNumBytes == WHOLE_FILE)
		nNumBytes = strlen(szScript);

	m_pBuffer = new char[nNumBytes+1];
	if(m_pBuffer == 0)
		return false;

	memcpy(m_pBuffer, szScript, nNumBytes);
	m_nNumBytes = nNumBytes;
	m_nLine = 0;
	m_pOffset = m_pBuffer;
	m_pBuffer[nNumBytes] = NULL;
	return true;
}

// Initializes the parsing.
bool CTokenizer::InitParsing(CFile& File, int nNumBytes)
{
	FreeScript();

	// Get the length of the file.
	if(nNumBytes == WHOLE_FILE)
		nNumBytes = File.GetLength();

	DWORD nLength = (DWORD)nNumBytes;
	
	m_pBuffer = new char[nLength+1];
	if(m_pBuffer == NULL)
		return false;

	// Read the entire file.
	if(!File.Read(m_pBuffer, nLength))
		return false;

	// Terminate it.
	m_pBuffer[nLength] = NULL;
	m_pOffset = m_pBuffer;
	m_nLine = 0;
	m_nNumBytes = nNumBytes;

	return true;
}

// Initializes the parsing.
bool CTokenizer::InitParsing(const char* szFilename)
{
	CFile File;

	FreeScript();

	if(!File.Open(szFilename, CFile::modeRead | CFile::shareDenyWrite | CFile::typeBinary))
		return false;

	// Get the length of the file.
	DWORD nLength = File.GetLength();
	
	m_pBuffer = new char[nLength+1];
	if(m_pBuffer == NULL)
		return false;

	// Read the entire file.
	if(!File.Read(m_pBuffer, nLength))
		return false;

	// Terminate it.
	m_pBuffer[nLength] = NULL;
	m_pOffset = m_pBuffer;
	m_nNumBytes = nLength;

	File.Close();
	m_nLine = 0;
	return true;
}

// Get's the next token out of the input stream.
bool CTokenizer::GetToken(CString& sToken)
{
	if(m_bUnget)
	{
		m_bUnget = false;
		sToken = m_sToken;
		return true;
	}

	sToken = "";
	m_sToken = "";

	if(m_pOffset == NULL || *m_pOffset == '\0')
		return false;

CTokenizer_GetToken_Start:

	// Skip to the next.
	if(*m_pOffset <= 32 && *m_pOffset != '\0')
	{
		while(*m_pOffset <= 32 && *m_pOffset != '\0')
		{
			if(*m_pOffset == '\n')
				m_nLine++;

			m_pOffset++;
		}
	}

	if(*m_pOffset == '\0')
		return false;

	// Skip any comments.
	if(*m_pOffset == '/')
	{
		switch(m_pOffset[1])
		{

		case '/':

			while(*m_pOffset != '\n' && *m_pOffset != '\0')
				m_pOffset++;

			if(*m_pOffset == '\0')	// Bad
				return false;
			else
				m_pOffset++;

			m_nLine++;
			goto CTokenizer_GetToken_Start;

			break;

		case '*':

			// Skip till the next */.
			while(*m_pOffset != '\0' && (*m_pOffset != '*' || m_pOffset[1] != '/'))
			{
				if(*m_pOffset == '\n')
					m_nLine++;

				m_pOffset++;
			}

			if(*m_pOffset == '\0')
				return false;
			else
				m_pOffset+=2;
			
			goto CTokenizer_GetToken_Start;

			break;
		}
	}
	else if(*m_pOffset == '"')	// Quoted string?
	{
		// Read till closing ".
		m_pOffset++;
		while(*m_pOffset != '\0' && *m_pOffset != '"')
		{
			// Allow \" for a quote character in a string and a \\ to be a single \*
			if(*m_pOffset == '\\')
			{
				if(m_pOffset[1] == '"' || m_pOffset[1] == '\\')
					m_pOffset++;
			}

			if(*m_pOffset == '\n')
				m_nLine++;
			else
				m_sToken += *m_pOffset;
			m_pOffset++;
		}

		if(*m_pOffset != '\0')
			m_pOffset++;
		
		sToken = m_sToken;
		return true;
	}

	// Load the next white space delim token.
	while(*m_pOffset > 32 && *m_pOffset != '\0')
	{
		m_sToken += *m_pOffset;
		m_pOffset++;
	}

	// Copy the token.
	sToken = m_sToken;

	return true;
}