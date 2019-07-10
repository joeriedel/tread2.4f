#include "StringTable.h"

class CToken
{
public:
	CToken();
	CToken(CString &sInput);
	~CToken();
	int GetLine();
	void Init(CString &sInput);
	bool GetToken();
	void UngetToken();
	CString GetString();
	bool IsQuoted();
	bool IsNewline();
	unsigned int GetSymbolCount();
	int GetUniqueCount();
	int GetCurPos();
	int GetLength();
	
protected:
	CString sToken;
	CString sScript;
	int nLine;
	int nPos;
	int nLast;
	bool bUnget;
	int nLength;
	bool bQuote;
	bool bNewline;
	unsigned int uWord;
	CStringTable table;
};
