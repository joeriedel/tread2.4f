#include "token.h"
//#include "TreeBar.h"
class CTreeBar;
class CConsoleView;

class CParseGdf
{
public:
	CParseGdf();
	~CParseGdf();
	bool ParseGdf(CString &sInput);
	void AttachTreeBar(CTreeBar* pTreeBar);
	void AttachConsole(CConsoleView* pConsole);
	int GetLine();
	void SetMessageLevel(bool bWarn, bool bErr);
	unsigned int GetCount();
	int GetTimeElapsed();
	int GetUniqueCount();
	bool IsEndOfScript();
	
private:
	bool ParseNames();	//not used

	//global parsing
	bool ParseGlobal();
	bool ParseDefBuild();
	bool ParseModule();
	bool ParseSpawnWait();
	bool ParseParams();
	bool ParseExport();

	//entity parsing
	bool ParseEntity();
	bool ParseAttribute();
	bool ParseKey();
	bool ParseFlags();
	
	//messages
	void Info(CString sMessage);
	void Warning(CString sMessage);
	void Error(CString sMessage);
	void Syntax(CString sUnknown, CString sExpected);
	void SetSectionString(CString sSection) { m_sSection = sSection; }
			
private:
	CToken parse;
	CTreeBar* m_pTreeBar;
	CConsoleView* m_pConsole;
	bool bError;
	bool bWarning;
	CTime m_Time1;
	CTime m_Time2;
	CTimeSpan m_Elapsed;
	int m_nSeconds;
	CString m_sSection;
};