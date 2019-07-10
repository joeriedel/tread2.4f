#include "stdafx.h"
#include "parse.h"
#include "ConsoleView.h"
#include "Treebar.h"


// Parses the GDF directory.
// Parses a game definition file.

CParseGdf::CParseGdf()
{
	bError = true;
	bWarning = true;
	m_pTreeBar = NULL;
	m_nSeconds = 0;
}

CParseGdf::~CParseGdf()
{
}

bool CParseGdf::ParseGdf(CString &sInput)
{
	bool bSkip = false;
	bool bError = true;
	m_nSeconds = 0;
	m_Time1 = CTime::GetCurrentTime();
	parse.Init(sInput);

	m_pConsole->ResetConsole();
	Info("Verifying the script ... checking for errors ...");

	for(;;)
	{
		if(!parse.GetToken())	//get the next token 
			break;
		if(IsEndOfScript())
			break;
		
		if(parse.GetString() == "$DefGlobal")
		{
			bSkip = false;
			parse.UngetToken();
			if(!ParseGlobal())
			{
				bSkip = true;
				bError = false;
			}
		}
	 	else if(parse.GetString() == "$DefEnt")
		{
			bSkip = false;
			parse.UngetToken();
			if(!ParseEntity())
			{
				bSkip = true;
				bError = false;
			}
		}
		else
		{
			if(bSkip)
				continue;
			
			Error("Unrecognized identifier (1): \"" + parse.GetString() + "\"");
			bSkip = true;
			bError = false;
		}
	}

	m_Time2 = CTime::GetCurrentTime();

	m_Elapsed = m_Time2 - m_Time1;
	m_nSeconds = m_Elapsed.GetSeconds();

	m_pConsole->PrintResults();

	return bError;
}

/*
Parses an entity.
Syntax :
$DefEnt [ NAME : SMART-NAME ] : [ BASE_CLASS ] : [ GAME1 : GAME2 : etc ]
$DefEnt [ DERIVED_CLASS : NULL : @VIRTUAL ] : [ BASE_CLASS ] : [ GAME1 : GAME2 : etc ]
$DefEnt [ BASE_CLASS : NULL : @VIRTUAL ] : [] : [ GAME1 : GAME2 : etc ]
*/
bool CParseGdf::ParseEntity(void)
{
	SetSectionString("$DefEntity");

	bool bVirtual;
	CString sName, sSmart, sCategory, sBase;

	parse.GetToken();	//eat $DefEnt
	
	if(!parse.GetToken())	//[
		return false;
	if(parse.GetString() != "[")
	{
		Syntax(parse.GetString(), "[");
		return false;
	}
	
	//KEY NAME--------------------
	parse.GetToken();	//string
	if(parse.IsQuoted())
	{
		if(parse.GetString() == "")
		{
			Error("Quotes with missing 'KEY-NAME', this field is required");
			//return false;
		}
	}
	else
	{
		Error("'KEY-NAME' requires \"\"");
		return false;
	}
	sName = parse.GetString();

	parse.GetToken();	//:
	if(parse.GetString() == "]")
	{
		bVirtual = true;
		sCategory = sName;
		goto BASE_CLASS;
	}
	else if(parse.GetString() != ":")
	{
		Syntax(parse.GetString(), ":");
		//Error("Missing separator ':' #2");
		return false;
	}

	//SMART NAME--------------------
	parse.GetToken();	//string
	if(parse.IsQuoted())
	{
		if(parse.GetString() == "")
		{
			Warning("Quotes with missing 'SMART-NAME', if this is a virtual entity use @VIRTUAL");
			return false;
		}
		sSmart = parse.GetString();
		bVirtual = false;
	}
	else
	{
		if(parse.GetString() != "@VIRTUAL")
		{
			Error("'SMART-NAME' requires \"\"");
			return false;
		}
		else	//token was NULL
			bVirtual = true;
	}

	parse.GetToken();	//:
	if(parse.GetString() == "]")
	{
		if(bVirtual)
			sCategory = sName;
		goto BASE_CLASS;
	}
	else if(parse.GetString() != ":")
	{
		Syntax(parse.GetString(), ":");
		//Error("Missing separator ':' #2");
		return false;
	}
	
	//CATEGORY NAME--------------------
	parse.GetToken();	//string
	if(parse.IsQuoted())
	{
		if(parse.GetString() == "")
		{
			Warning("Quotes with missing 'CATEGORY-NAME', use @NULL");
		}
		sCategory = parse.GetString();
	}
	else
	{
		if(parse.GetString() != "@NULL")
		{
			Error("'CATEGORY-NAME' requires \"\"");
			return false;
		}
	}
	
	parse.GetToken();
	if(parse.GetString() != "]")
	{
		Syntax(parse.GetString(), "]");
		return false;
	}

BASE_CLASS:

	parse.GetToken(); // [ or []
	if(parse.GetString() == "[") //this is a base class
	{
		parse.GetToken();	//string
		if(parse.IsQuoted())
		{
			if(parse.GetString() == "")
			{
				Warning("Quotes with missing 'BASE-CLASS', use [] instead of [ \"\" ]");
			}
			else
				sBase = parse.GetString();
		}
		else
		{
			Error("'BASE-CLASS' requires \"\"");
			return false;
		}

		parse.GetToken();	//]
		if(parse.GetString() != "]")
		{
			Syntax(parse.GetString(), "]");
			return false;
		}
	}
	else if(parse.GetString() == "[]")
	{
		//bDerived = true;
	}
	else
	{
		Syntax(parse.GetString(), "[");
		return false;
	}
	
	if(m_pTreeBar != NULL)
		m_pTreeBar->AddEntity(sName, sSmart, sBase, "", parse.GetLine());
	
	parse.GetToken();
	if(parse.GetString() == "{")
	{
		while(true)
		{
			parse.GetToken();	//}
			if(parse.GetString() == "}")
				break;

			if(IsEndOfScript())
				break;

			if(parse.GetString() == "def_key")
			{
				parse.UngetToken();
				if(!ParseKey())
					return false;
			}
			else if(parse.GetString() == "def_att")
			{
				parse.UngetToken();
				if(!ParseAttribute())
					return false;
			}
			else if((parse.GetString() == "$DefFlags") || (parse.GetString() == "$DefOptions"))
			{
				parse.UngetToken();
				if(!ParseFlags())
					return false;
			}
			else
			{
				Error("Unrecognized identifier (2): \"" + parse.GetString() + "\"");
				return false;
			}
		}
	}
	else if(parse.GetString() != "{}")	// No immediate close?
	{

		Syntax(parse.GetString(), "{");
		return false;
	}

	return true;
}


/*
Parses a def_att.
Syntax: def_att : NAME : VALUE.
*/
bool CParseGdf::ParseAttribute()
{
	SetSectionString("def_att");

	parse.GetToken();
	/*if(parse.GetString() != "def_att")
	{
		Error("Missing 'def_att'");
		return false;
	}*/

	parse.GetToken();	//:
	if(parse.GetString() != ":")
	{
		Error("Missing ':'");
		return false;
	}
	
	parse.GetToken();	//string type
	if(parse.GetString() == "owner")
	{
		parse.GetToken();
		if(parse.GetString() != ":")
		{
			Error("Missing ':'");
			return false;
		}

		parse.GetToken();	//value
		if(parse.GetString() == "true");
		else if(parse.GetString() == "false");
		else if(parse.GetString() == "@FORCE-DERIVATION");
		else if(parse.GetString() == "@FORCE");
		else
		{
			Error("\"" + parse.GetString() + "\" - Unrecognized boolean value");
			return false;
		}
	}
	else if(parse.GetString() == "size")
	{
		parse.GetToken();	//:
		if(parse.GetString() != ":")
		{
			Error("Missing ':'");
			return false;
		}

		parse.GetToken();	//value
		if(parse.GetString() == "")
		{
			Error("Missing initializer");
			return false;
		}
	}
	else
	{
		Error(parse.GetString() + " - Unsupported attribute");
		return false;
	}

	return true;
}

// Parses a $DefGlobal.
// Syntax: 
// $DefGlobal [ GAME ]
// {
//		... stuff.
// }
bool CParseGdf::ParseGlobal(void)
{
	SetSectionString("$DefGlobal");

	CString sBuffer;

	parse.GetToken();	//eat $DefGlobal
	
	parse.GetToken();
	if(parse.GetString() != "[")
	{
		Syntax(parse.GetString(), "[");
		return false;
	}

	parse.GetToken(); //Game Name
	sBuffer = parse.GetString();
	if(parse.IsQuoted())
	{
		if(sBuffer == "")
		{
			Error("No game name defined.");
			return false;
		}

		m_pTreeBar->AddGame(sBuffer, parse.GetLine());
	}
	else
	{
		Error("Game name must have \"\"");
		return false;
	}
			
	parse.GetToken();
	if(parse.GetString() != "]")
	{
		Syntax(parse.GetString(), "]");
		return false;
	}

	parse.GetToken();
	if(parse.GetString() != "{")
	{
		Syntax(parse.GetString(), "{");
		return false;
	}

	for(;;)
	{
		parse.GetToken();
		sBuffer = parse.GetString();

		if(sBuffer == "}")
			break;

		if(sBuffer == "def_author") {}
		else if(sBuffer == "def_version") {}
		else if(sBuffer == "def_solid") {}
		else if(sBuffer == "def_point") {}
		else if(sBuffer == "def_palette") {}
		else if(sBuffer == "def_pakdir") {}
		else if(sBuffer == "def_namekey") {}
		else if(sBuffer == "def_targetkey") {}
		else if(sBuffer == "def_gamemaptype") {}
		else if(sBuffer == "def_worldspawn") {}
		else if(sBuffer == "def_textype") {}
		else if(sBuffer == "def_key")
		{
			parse.UngetToken();
			if(!ParseKey())
				return false;
			continue;
		}
		else if((sBuffer == "$DefFlags") || (sBuffer == "$DefOptions"))
		{
			parse.UngetToken();
			if(!ParseFlags())
				return false;
			continue;
		}
		else if(sBuffer == "$DefBuild")
		{
			parse.UngetToken();
			if(!ParseDefBuild())
				return false;
			continue;
		}
		else
		{
			Error("Unrecognized identifier (DefGlobal): \"" + parse.GetString() + "\"");
			return false;
		}

		//parse the remaining string definition
		parse.GetToken();	//:
		if(parse.GetString() != ":")
		{
			Syntax(parse.GetString(), ":");
			return false;
		}

		parse.GetToken();	//string
		if(parse.IsQuoted())
		{
			if(parse.GetString() == "")
			{
				Warning("No string defined for : " + sBuffer);
				//return false;
			}
		}
	}

	return true;
}

bool CParseGdf::ParseDefBuild()
{
	SetSectionString("$DefBuild");

	parse.GetToken();
	
	parse.GetToken();
	if(parse.GetString() != "{")
	{
		Syntax(parse.GetString(), "{");
		return false;
	}

	while(true)
	{
		parse.GetToken();
		if(parse.GetString() == "}")
			break;

		if(parse.GetString() == "$DefModule")
		{
			parse.UngetToken();
			if(!ParseModule())
				return false;
		}
		else
		{
			Error("Unrecognized identifier (DefBuild): \"" + parse.GetString() + "\"");
			return false;
		}

	}
	return true;
}

bool CParseGdf::ParseModule()
{
	SetSectionString("$DefModule");

	parse.GetToken();	//eat $DefModule
	
	parse.GetToken();
	if(parse.GetString() != "[")
	{
		Syntax(parse.GetString(), "[");
		return false;
	}

	parse.GetToken();
	
	parse.GetToken();
	if(parse.GetString() != ":")
	{
		Syntax(parse.GetString(), ":");
		return false;
	}

	parse.GetToken();

	parse.GetToken();
	if(parse.GetString() != ":")
	{
		Syntax(parse.GetString(), ":");
		return false;
	}

	parse.GetToken();
			
	parse.GetToken();
	if(parse.GetString() != "]")
	{
		Syntax(parse.GetString(), "]");
		return false;
	}

	parse.GetToken();
	if(parse.GetString() != "{")
	{
		Syntax(parse.GetString(), "{");
		return false;
	}

	while(true)
	{
		parse.GetToken();
		if(parse.GetString() == "}")
			break;

		if((parse.GetString() == "$SpawnWait") || (parse.GetString() == "$SpawnNoWait"))
		{
			parse.UngetToken();
			if(!ParseSpawnWait())
				return false;
		}
		else if(parse.GetString() == "$DefParams")
		{
			parse.UngetToken();
			if(!ParseParams())
				return false;
		}
		else if(parse.GetString() == "$ExportMap")
		{
			parse.UngetToken();
			if(!ParseExport())
				return false;
		}
		else if(parse.GetString() == "def_path")
		{
			parse.GetToken();
			if(parse.GetString() != ":")
			{
				Syntax(parse.GetString(), ":");
				return false;
			}

			parse.GetToken();
		}
		else if(parse.GetString() == "def_type")
		{
			parse.GetToken();
			if(parse.GetString() != ":")
			{
				Syntax(parse.GetString(), ":");
				return false;
			}

			parse.GetToken();
		}
		else if(parse.GetString() == "def_pathtype")
		{
			parse.GetToken();
			if(parse.GetString() != ":")
			{
				Syntax(parse.GetString(), ":");
				return false;
			}

			parse.GetToken();
		}
		else
		{
			Error("Unrecognized identifier (DefModule): \"" + parse.GetString() + "\"");
			return false;
		}
	}

	return true;
}

bool CParseGdf::ParseExport()
{
	SetSectionString("$DefExport");

	parse.GetToken();	//eat $ExportMap
	
	parse.GetToken();
	if(parse.GetString() != ":")
	{
		Syntax(parse.GetString(), ":");
		return false;
	}

	parse.GetToken();
	/*if(!parse.IsQuoted())
	{
		Error("\"" + parse.GetString() + "\" - Should be in quotes.");
		return false;
	}*/

	parse.GetToken();
	if(parse.GetString() != ":")
	{
		Syntax(parse.GetString(), ":");
		return false;
	}

	parse.GetToken();
	if(!parse.IsQuoted())
	{
		Error("\"" + parse.GetString() + "\" - Should be in quotes.");
		return false;
	}

	return true;
}

bool CParseGdf::ParseSpawnWait()
{
	SetSectionString("$SpawnWait");

	parse.GetToken();
	/*if(parse.GetString() != "$SpawnWait")
	{
		Error("Missing '$SpawnWait'");
		return false;
	}*/

	parse.GetToken();
	if(parse.GetString() != ":")
	{
		Syntax(parse.GetString(), ":");
		return false;
	}

	parse.GetToken();	//identifier

	parse.GetToken();
	if(parse.GetString() != ":")
	{
		Syntax(parse.GetString(), ":");
		return false;
	}

	parse.GetToken();	//identifier

	parse.GetToken();
	if(parse.GetString() != ":")
	{
		Syntax(parse.GetString(), ":");
		return false;
	}

	parse.GetToken();	//identifier

	parse.GetToken();
	if(parse.GetString() != ":")
	{
		Syntax(parse.GetString(), ":");
		return false;
	}

	parse.GetToken();	//identifier

	return true;
}

bool CParseGdf::ParseParams()
{
	SetSectionString("$DefParams");

	parse.GetToken();
	/*if(parse.GetString() != "$DefParams")
	{
		Error("Missing 'def_state'");
		return false;
	}
	*/

	parse.GetToken();
	if(parse.GetString() != "{")
	{
		Syntax(parse.GetString(), "{");
		return false;
	}

	while(true)
	{
		parse.GetToken();
		if(parse.GetString() == "}")
			break;

		if(parse.GetString() == "def_param")
		{
			parse.GetToken();
			if(parse.GetString() != ":")
			{
				Syntax(parse.GetString(), ": (1)");
				return false;
			}

			parse.GetToken();	//identifier

			parse.GetToken();
			if(parse.GetString() != ":")
			{
				Syntax(parse.GetString(), ": (2)");
				return false;
			}

			parse.GetToken();	//identifier

			parse.GetToken();
			if(parse.GetString() != ":")
			{
				Syntax(parse.GetString(), ": (3)");
				return false;
			}

			parse.GetToken();	//identifier

			parse.GetToken();
			if(parse.GetString() != ":")
			{
				Syntax(parse.GetString(), ": (4)");
				return false;
			}

			parse.GetToken();	//identifier
		}
		else
		{
			Error("Unrecognized identifier (DefParams): \"" + parse.GetString() + "\"");
			return false;
		}

	}

	return true;
}

// Parses a def_key.
bool CParseGdf::ParseKey()
{
	SetSectionString("def_key");

	// Syntax: def_key : TYPE : NAME : SMART-NAME : DEF-VAL
	parse.GetToken();
	/*if(parse.GetString() != "def_key")
	{
		Error("Missing 'def_key'");
		return false;
	}*/

	parse.GetToken();	// :
	if(parse.GetString() != ":")
	{
		Error("Missing ':' #1");
		return false;
	}

	parse.GetToken();	// TYPE
	if(parse.GetString() == "string");
	else if(parse.GetString() == "integer");
	else if(parse.GetString() == "color");
	else if(parse.GetString() == "colorb");
	else if(parse.GetString() == "vector");
	else
	{
		Error("Unrecognized key type.");
		return false;
	}

	parse.GetToken();	//:
	if(parse.GetString() != ":")
	{
		Error("Missing ':' #2");
		return false;
	}

	parse.GetToken();	//string
	if(parse.IsQuoted())
	{
		if(parse.GetString() == "")
		{
			Error("Missing 'KEY-NAME'");
			return false;
		}
	}
	
	parse.GetToken();	//:
	if(parse.GetString() != ":")
	{
		Error("Missing ':' #3");
		return false;
	}

	parse.GetToken();	//string
	if(parse.IsQuoted())
	{
		if(parse.GetString() == "")
		{
			Error("Missing 'SMART-NAME'");
			return false;
		}
	}

	parse.GetToken();	//:
	if(parse.GetString() != ":")
	{
		Error("Missing ':' #4");
		return false;
	}

	parse.GetToken();	//string
				
	return true;
}

/*
Parses $DefFlags or $DefOptions.
	Syntax:
	
	$DefFlags [ KEY-NAME : SMART-NAME ] : DEF-VAL
	{
		def_flg : SMART-NAME : value : true/false.
	}
*/
bool CParseGdf::ParseFlags()
{
	SetSectionString("$DefFlags");

	int nFlags;

	parse.GetToken();
	/*if(parse.GetString() == "$DefFlags");
	else if(parse.GetString() == "$DefOptions");
	else
	{
		Error("Missing '$DefFlags' / '$DefOptions'");
		return false;
	}*/

	parse.GetToken();	//[
	if(parse.GetString() != "[")
	{
		Error("Missing '['");
		return false;
	}

	parse.GetToken();
	if(parse.GetString() == "")	// NAME.
	{
		Error("Missing 'KEY-NAME'");
		return false;
	}

	parse.GetToken();	//:
	if(parse.GetString() != ":")
	{
		Error("Missing ':'");
		return false;
	}

	parse.GetToken();
	if(parse.GetString() == "")	// SMART-NAME.
	{
		Error("Missing 'SMART-NAME'");
		return false;
	}

	parse.GetToken();	//]
	if(parse.GetString() != "]")
	{
		Error("Missing ']'");
		return false;
	}

	parse.GetToken();	//{
	if(parse.GetString() != "{")
	{
		Error("Missing '{'");
		return(NULL);
	}

	// Read each def_flg.
	nFlags = 0;
	for(;;)
	{
		parse.GetToken();	//}
		if(parse.GetString() == "}")
			break;
		
		if(parse.GetString() != "def_flg")
		{
			Error("Missing 'def_flg'");
			return(NULL);
		}

		parse.GetToken();	//:
		if(parse.GetString() != ":")
		{
			Error("Missing ':' #1");
			return false;
		}

		parse.GetToken();	//string
		if(parse.IsQuoted())
		{
			if(parse.GetString() == "")
			{
				Error("Missing 'FLAG-NAME'");
				return false;
			}
		}

		parse.GetToken();	//:
		if(parse.GetString() != ":")
		{
			Error(parse.GetString());
			Error("Missing ':' #2");
			return false;
		}

		parse.GetToken();	//string
		if(parse.IsQuoted())
		{
			if(parse.GetString() == "")
			{
				Error("Missing 'FLAG-VALUE'");
				return false;
			}
		}

		parse.GetToken();	//:
		if(parse.GetString() != ":")
		{
			Error("Missing ':' #3");
			return false;
		}

		parse.GetToken();	//true/false
		if(parse.GetString() == "true");
		else if(parse.GetString() == "false");
		else
		{
			Error("\"" + parse.GetString() + "\" - Unrecognized boolean operand");
			return false;
		}

		nFlags++;
	}

	if(nFlags == 0)
	{
		Error("No legal flags/options found");
		return false;
	}

	return true;
}

int CParseGdf::GetLine()
{
	return parse.GetLine();
}

void CParseGdf::SetMessageLevel(bool bWarn, bool bErr)
{
	bWarning = bWarn;
	bError = bErr;
}

unsigned int CParseGdf::GetCount()
{
	return parse.GetSymbolCount();
}

int CParseGdf::GetTimeElapsed()
{
	return m_nSeconds;
}

int CParseGdf::GetUniqueCount()
{
	return parse.GetUniqueCount();
}

void CParseGdf::AttachTreeBar(CTreeBar* pTreeBar)
{
	m_pTreeBar = pTreeBar;
}

void CParseGdf::AttachConsole(CConsoleView* pConsole)
{
	m_pConsole = pConsole;
}

bool CParseGdf::IsEndOfScript()
{
	if(parse.GetCurPos() > parse.GetLength())
		return true;
	else
		return false;
}

void CParseGdf::Info(CString sMessage)
{
	m_pConsole->AddMessage(sMessage, INFO_MSG, -1);
}

void CParseGdf::Warning(CString sMessage)
{
	if(!bWarning)
		return;

	CString sBuffer;
	sBuffer.Format("  (%d)  WARNING  >>  %s", GetLine(), sMessage);
	m_pConsole->AddMessage(sBuffer, WARNING_MSG, GetLine());
}

void CParseGdf::Error(CString sMessage)
{
	if(!bError)
		return;

	CString sBuffer;
	sBuffer.Format("  (%d)  ERROR  >>  %s", GetLine(), sMessage);
	m_pConsole->AddMessage(sBuffer, ERROR_MSG, GetLine());
}

void CParseGdf::Syntax(CString sUnknown, CString sExpected)
{
	if(!bError)
		return;

	CString sBuffer;
	sBuffer.Format("  (%d)  SYNTAX  >>  Expected ' %s ' :: Unknown syntax ' %s ' in %s", GetLine(), sExpected, sUnknown, m_sSection);
	m_pConsole->AddMessage(sBuffer, ERROR_MSG, GetLine());
}