#include "stdafx.h"
#include "token.h"

CToken::CToken()
{
	//bEof = false;
	bUnget = false;
	nLine = 1;
	nPos = 0;
	nLast = 0;
	uWord = 0;
}

CToken::CToken(CString &sInput)
{
	sScript = sInput;
	nLength = sScript.GetLength();
	bUnget = false;
	nLine = 1;
	nPos = 0;
	nLast = 0;
	uWord = 0;
}

CToken::~CToken()
{
}

int CToken::GetLine()
{
	return nLine;
}

void CToken::Init(CString &sInput)
{
	sScript = sInput;
	nLength = sScript.GetLength() - 1;
	nLine = 1;
	nPos = 0;
	bUnget = false;
}

bool CToken::GetToken()
{
	if(nPos > nLength)
		return false;
	
	char c;
		
	sToken.Empty();
	bQuote = false;
	bNewline = false;

gettoken_skipwhite:
	
	while((c = sScript.GetAt(nPos)) <= 32)
	{
		if(c == '\n')
		{
			nLine++;
			bNewline = true;
		}
		
		nPos++;

		if(nPos > nLength)
			return false;
	}

	nLast = nPos;
		
	// Skip comments.
	if( (sScript.GetAt(nPos) == '/') && (sScript.GetAt(nPos + 1) == '/') )
	{
		while(sScript.GetAt(nPos) != '\n')
		{
			if((nPos >= sScript.GetLength()) || (sScript.GetAt(nPos) == NULL))
				return false;

			nPos++;
			if(nPos > nLength)
				return false;
		}
		
		goto gettoken_skipwhite;
	}
	
	// Get "quoted" token.
	if(sScript.GetAt(nPos) == '"')
	{
		bQuote = true;
		nPos++;
		// Copy it.
		while(sScript.GetAt(nPos) != '"')
		{
			if(sScript.GetAt(nPos) == NULL)
				return false;

			sToken += sScript.GetAt(nPos);
			nPos++;
			if(nPos > nLength)
				return true;
		}

		nPos++;
	}
	else
	{
		// Copy until next space.
		while(sScript.GetAt(nPos) > 32)
		{
			sToken += sScript.GetAt(nPos);
			nPos++;
			if(nPos > nLength)
				return true;
		}
	}

	table.AddItem(sToken);

	uWord++;

	if(!bQuote)
		if(sToken.IsEmpty())
			return false;

	return true;
}

void CToken::UngetToken()
{
	bUnget = true;
	nPos = nLast;
	if(uWord > 0)
		uWord--;
}

CString CToken::GetString()
{
	return sToken;
}

bool CToken::IsQuoted()
{
	return bQuote;
}

bool CToken::IsNewline()
{
	return bNewline;
}

unsigned int CToken::GetSymbolCount()
{
	return uWord;
}

int CToken::GetUniqueCount()
{
	return table.GetCount();
}

int CToken::GetCurPos()
{
	return nPos;
}

int CToken::GetLength()
{
	return nLength;
}