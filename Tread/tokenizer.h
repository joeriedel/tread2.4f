// TOKENIZER.H
// Copyright © 1999 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#ifndef TOKENIZER_H
#define TOKENIZER_H

#define WHOLE_FILE		-1

// Tokenizes input from a file.
class CTokenizer
{
private:

	bool m_bUnget;
	char* m_pBuffer;
	char* m_pOffset;
	CString m_sToken;
	int m_nLine;
	int m_nNumBytes;

public:

	CTokenizer();
	virtual ~CTokenizer();

	bool InitParsing(const char* szFilename);
	bool InitParsing(CFile& hFile, int nNumBytes);
	bool InitParsing(const char* szScript, int nNumBytes);
	void FreeScript();
	void Skip(int nNum=1);	// Skips n tokens.
	bool GetToken(CString& sToken);
	int GetNumBytes() { return m_nNumBytes; }
	int GetByteOffset() { return m_pOffset-m_pBuffer; }
	int  GetLine() { return m_nLine+1; }
	void UngetToken() {m_bUnget = true; }

};

#endif