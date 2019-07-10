// GAMEAPI.H
// Copyright © 1999 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#ifndef GAMEAPI_H
#define GAMEAPI_H

////////////////////////////////////////////////////////////////////////////
// Defines the entity scripting classes and definitions structures.
////////////////////////////////////////////////////////////////////////////
#ifdef _WIN32
#include "resource.h"
#endif
#include "MathLib.h"
#include "LinkedList.h"
#include "TexAPI.h"
#include "Tokenizer.h"
#include "Entity.h"
#include "Tread3DDoc.h"
#include "Models.h"
#include "TExport.h"

// $sb "gameapi.h block1"

// Define possible key types.
#define EKD_TYPE_STRING			1
#define EKD_TYPE_INTEGER		2
#define EKD_TYPE_ANGLE			3
#define EKD_TYPE_Q2COLOR		4
#define EKD_TYPE_HLCOLOR		5
#define EKD_TYPE_FLAGS			6
#define EKD_TYPE_VECTOR			7

// NOTE: Because of the way the l-list and structures are set up, the auto-deconstructors
// automatically free all the memory.

// Defines an entity flag.
struct _T_EXPORT ENTITYFLAGOPTIONDEFINITION;
struct _T_EXPORT ENTITYFLAGOPTIONDEFINITION
{
	ENTITYFLAGOPTIONDEFINITION* prev, *next;

	ENTITYFLAGOPTIONDEFINITION()
	{}

	ENTITYFLAGOPTIONDEFINITION(const ENTITYFLAGOPTIONDEFINITION& FlagToCopy)
	{
		sString = FlagToCopy.sString;
		nValue = FlagToCopy.nValue;
		prev = next = NULL;
	}

	CString sString;
	int nValue;
	bool sUser;
	bool bType;
};

#define EFD_TYPE_FLAGS		true
#define EFD_TYPE_OPTIONS	false

struct _T_EXPORT ENTITYFLAGDEFINITION;
struct _T_EXPORT ENTITYFLAGDEFINITION
{
	ENTITYFLAGDEFINITION* prev, *next;

	ENTITYFLAGDEFINITION()
	{
		pFlagList = new CLinkedList<ENTITYFLAGOPTIONDEFINITION>();
		prev = next = NULL;
	}

	ENTITYFLAGDEFINITION(const ENTITYFLAGDEFINITION& FlagToCopy)
	{
		nValue = FlagToCopy.nValue;
		bType = FlagToCopy.bType;
		if(FlagToCopy.pFlagList)
			pFlagList = new CLinkedList<ENTITYFLAGOPTIONDEFINITION>(*FlagToCopy.pFlagList);
		else
			pFlagList = new CLinkedList<ENTITYFLAGOPTIONDEFINITION>();
		prev = next = NULL;
	}

	~ENTITYFLAGDEFINITION()
	{
		if(pFlagList)
			delete pFlagList;
		pFlagList = NULL;
	}

	int nValue;	// Default value of flag.
	bool bType;	// TRUE == Flags, FALSE == Options.
	CLinkedList<ENTITYFLAGOPTIONDEFINITION>* pFlagList;
};

struct _T_EXPORT ENTITYKEYDEFINITION;
struct _T_EXPORT ENTITYKEYDEFINITION
{
	ENTITYKEYDEFINITION* prev, *next;
	
	ENTITYKEYDEFINITION()
	{
		pFlagDef=NULL;
		prev = next = NULL;
		nType;
		sName = sSmartName = sValue = "";
	}

	ENTITYKEYDEFINITION(const ENTITYKEYDEFINITION& KeyDef)
	{
		if(KeyDef.pFlagDef != NULL)
			pFlagDef = new ENTITYFLAGDEFINITION(*KeyDef.pFlagDef);
		else
			pFlagDef = NULL;
		nType = KeyDef.nType;
		sName = KeyDef.sName;
		sSmartName = KeyDef.sSmartName;
		sValue = KeyDef.sValue;
		prev = next = NULL;
	}

	~ENTITYKEYDEFINITION()
	{
		if(pFlagDef)
			delete pFlagDef;
		pFlagDef = NULL;
	}

	ENTITYFLAGDEFINITION* pFlagDef;	// Flag definition. NULL == none.
	int nType;
	CString sName, sSmartName, sValue;
};

// Entity definition.
struct _T_EXPORT ENTITYDEFINITION;
struct _T_EXPORT EDBASECLASSLIST;
struct _T_EXPORT EDBASECLASSLIST
{
	EDBASECLASSLIST* prev, *next;
	ENTITYDEFINITION* pEntDef;
};

struct _T_EXPORT ENTITYDEFINITION
{
	ENTITYDEFINITION* prev, *next;
	
	ENTITYDEFINITION()
	{
		prev = next = NULL;
		sName = sSmartName = sCategory = "";
		pKeyList = new CLinkedList<ENTITYKEYDEFINITION>();
		memset(nMiscFlags, -1, sizeof(nMiscFlags));
	}

	ENTITYDEFINITION(const ENTITYDEFINITION& EntityToCopy)
	{
		bVirtual = EntityToCopy.bVirtual;
		bOwner = EntityToCopy.bOwner;
		VectorCopy(EntityToCopy.vSize, vSize);
		sName = EntityToCopy.sName;
		sSmartName = EntityToCopy.sSmartName;
		sCategory = EntityToCopy.sCategory;
		pBaseClass = EntityToCopy.pBaseClass;
		if(EntityToCopy.pKeyList)
			pKeyList = new CLinkedList<ENTITYKEYDEFINITION>(*EntityToCopy.pKeyList);
		else
			pKeyList = new CLinkedList<ENTITYKEYDEFINITION>();
		prev = next = NULL;

		memcpy(nMiscFlags, EntityToCopy.nMiscFlags, sizeof(nMiscFlags));
	}

	~ENTITYDEFINITION()
	{
		if(pKeyList)
			delete pKeyList;
		pKeyList = NULL;
	}

	bool AddKeyDefinition(ENTITYKEYDEFINITION* pKeyDef);
	ENTITYKEYDEFINITION* FindKeyDefinition(CString sKeyName);

	ENTITYDEFINITION* pBaseClass;
	CLinkedList<ENTITYKEYDEFINITION>* pKeyList;
	ENTITYKEYDEFINITION* GetKeyDefinition(const char* szName);

	bool bVirtual;
	bool bOwner;
	
	vec3_t vSize;
	CString sName;
	CString sSmartName;
	CString sCategory;

	int nMiscFlags[4];
};

//////////////////////////////////////////////////////////////////////////
// Scripted Build Structs.												//
//////////////////////////////////////////////////////////////////////////
struct _T_EXPORT RUNPROGRAMCMD
{
	RUNPROGRAMCMD* prev, *next;

	bool bWait;
	bool bConsole;
	CString sModule;
	CString sOutput;
	CString sWorkDirAlias;
	CString sWorkDir;
};

struct _T_EXPORT EXPORTMAPCMD
{
	EXPORTMAPCMD* prev, *next;
	CString sOutName;
	CString sOutput;
};

struct _T_EXPORT DEFPARAMPARAM
{
	DEFPARAMPARAM* prev, *next;

	CString sTitle;
	CString sParam;
	int nValue;
	bool bDefault;
	bool bAlways;
};

struct _T_EXPORT DEFPARAM
{
	DEFPARAM* prev, *next;

	int nValue;
	CLinkedList<DEFPARAMPARAM>* pParamList;

	DEFPARAM()
	{
		pParamList = new CLinkedList<DEFPARAMPARAM>();
	}

	~DEFPARAM()
	{
		delete pParamList;
	}

};

#define MODULE_TYPE_NOTRUNNABLE	0x00000000
#define MODULE_TYPE_RUNNABLE	0x00000001

#define PATH_TYPE_NONE			0x00000000
#define PATH_TYPE_FILE			0x00000001
#define PATH_TYPE_DIR			0x00000002

struct _T_EXPORT DEFMODULE
{
	DEFMODULE* prev, *next;
	
	DEFMODULE()
	{
		pDefParam = NULL;
		pProgramList = new CLinkedList<RUNPROGRAMCMD>();
		pExportList = new CLinkedList<EXPORTMAPCMD>();
	}

	~DEFMODULE()
	{
		if(pDefParam != NULL)
			delete pDefParam;
		delete pProgramList;
		delete pExportList;
	}

	int nType;
	int nPathType;
	CString sName;
	CString sSetupTitle;
	CString sRunTitle;
	CString sPath;
	CString sCmdLine;
	
	DEFPARAM* pDefParam;
	CLinkedList<RUNPROGRAMCMD>* pProgramList;
	CLinkedList<EXPORTMAPCMD>* pExportList;
};

struct _T_EXPORT DEFBUILD
{
	DEFBUILD()
	{
		pModuleList = new CLinkedList<DEFMODULE>();
	}

	DEFBUILD(const DEFBUILD& Copy)
	{
		pModuleList = new CLinkedList<DEFMODULE>(*Copy.pModuleList);
	}

	~DEFBUILD()
	{
		if(pModuleList)
			delete pModuleList;
		pModuleList = NULL;

	}


	CLinkedList<DEFMODULE>* pModuleList;
};

struct _T_EXPORT RUNSTATE
{
	RUNSTATE* prev, *next;

	CString sName;
	CString sValue;
	CString sCmdLine;
	int nValue;
};

class CTread3DDoc;
class CGameDefinition;

struct _T_EXPORT RUNBLOCK
{
	RUNBLOCK()
	{}

	~RUNBLOCK()
	{}

	CString sTreadFile;
	CGameDefinition* pGameDef;
	CTread3DDoc* pDocument;
};

// Predefined SCRIPT constants.
#define SCRIPT_ALWAYS		"@ALWAYS"
#define SCRIPT_ABORT_MODULE	"@ABORT"
#define SCRIPT_MAP_FILE		"@FILENAME"

//////////////////////////////////////////////////////////////////////////
// GameDefinition														//
//////////////////////////////////////////////////////////////////////////
struct	_T_EXPORT GAMEPAKFILE
{
	GAMEPAKFILE* prev, *next;

	int nHandle;
	CString sTitle;
	CString sFilename;
	bool bDir;
};

// Game API class.
class _T_EXPORT CGameAPI; // Place holder.

struct GAMECAP
{
	GAMECAP* prev, *next;

	CString sName;
	bool bEnabled;
};

// Game Definition.
class _T_EXPORT CGameDefinition : public CLLObject
{
private:

	CString m_sName;			// Game name.
	CString	m_sPalFile;			// Palette file.
	CString m_sClassNameKey;	// Name of the classname key.
	CString m_sNameKey;
	CString m_sTargetKey;
	CString m_sDefPoint;
	CString m_sDefSolid;
	CString m_sWorldspawn;
	CString m_sAuthor;
	CString m_sVersion;
	CString m_sScriptFile;
	CString m_sMapFileType;
	CString m_sTextureType;
	CString m_sGameExe;
	CString m_sGameDir;
	CString m_sGameMapDir;
	CString m_sTexDirModel;

	CLinkedList<GAMECAP> m_CapsList;
	CLinkedList<ENTITYDEFINITION> m_EntDefList;
	CLinkedList<GAMEPAKFILE> m_PakFileList;
	CLinkedList<MODEL>* m_pModelList;
	MODEL* m_pCurModel;
	ENTITYDEFINITION* m_pCurSolidEntity;
	ENTITYDEFINITION* m_pCurPointEntity;

	CTokenizer m_Tokenizer;
	DEFBUILD m_DefBuild;
	ENTITYDEFINITION m_GlobalEntDef;

#ifdef _WIN32
	CWinThread* m_pCurThread;
#endif

	// The game's palette.
	CTexPalette* m_pPalette;

	// Displays a scripting error.
	void ScriptError(ENTITYDEFINITION* pEntityDef, const char* szMessage, ...);

	// Parses $DefBuildOptions
	bool ParseDefBuild();
	DEFMODULE* ParseDefModule();
	DEFPARAM* ParseDefParams();
	
	// Parses a global defintion.
	bool ParseGlobalDefinition();
	// Parses an entity.
	ENTITYDEFINITION* ParseEntityDefinition();
	// Parses an attribute.
	bool ParseDefAttribute(ENTITYDEFINITION* pEntityDef);
	// Assigns a key type. -1 for error.
	int AssignKeyType(CString sType);
	// Verify key value based on type.
	bool VerifyKeyValue(CString sValue, int nType);
	// Verifies and gets a token.
	bool VerifyGetToken(const char* szValue);

	// Actual parsing routines.
	GAMECAP* ParseDefCap();
	ENTITYKEYDEFINITION* ParseDefKey(ENTITYDEFINITION* pEntityDef);
	ENTITYKEYDEFINITION* ParseDefOptions(CString sType, ENTITYDEFINITION* pEntityDef);
	
	// Parses ': "string"'
	bool Parse1Pair(CString& sString);
	// Parses ': "string1" : "string2"'
	bool Parse2Pair(CString& sString1, CString& sString2);
	// Parses ': "string1" : "string2" : "string3"'
	bool Parse3Pair(CString& sString1, CString& sString2, CString& sString3);
	// Parses ': "string1" : "string2" : "string3" : "string4"'
	bool Parse4Pair(CString& sString1, CString& sString2, CString& sString3, CString& sString4);

	// Allocates an entity definition.
	ENTITYDEFINITION* AllocateEntityDefinition();
	// Allocates an entity key definition.
	ENTITYKEYDEFINITION* AllocateEntityKeyDefinition();
	// Allocates a flag definition.
	ENTITYFLAGDEFINITION* AllocateEntityFlagDefinition();
	// Allocates an option definition.
	ENTITYFLAGOPTIONDEFINITION* AllocateEntityOptionDefinition();

	// Loads/Saves the games reg settings.
	bool LoadRegSettings();
	bool SaveRegSettings();

	// Loads models.
	bool LoadModels();

	// Loads all resources for the game.
	bool LoadGameResources();
	bool FreeGameResources();

	// Resolves aliases.
	//bool ResolveBuildAliases(RUNBLOCK* pRunBlock);
	
public:

	CGameDefinition() 
	{ 
		m_pCurModel = NULL;
		m_pPalette = NULL; 

#ifdef _WIN32
		m_pCurThread = NULL;
#endif

		m_pModelList = new CLinkedList<MODEL>();
	}
	~CGameDefinition()
	{
		if(m_pPalette != NULL)
			delete m_pPalette;
		delete m_pModelList;
		m_pPalette = NULL;
	}

	CTexPalette* GetPalette() { return m_pPalette; }

	CString GetGameName() { return m_sName; }
	CString GetGamePaletteName() { return m_sPalFile; }
	CString GetAuthorString() { return m_sAuthor; }
	CString GetVersionString() { return m_sVersion; }
	CString GetWorldspawnName() { return m_sWorldspawn; }
	CString GetClassNameString() { return m_sClassNameKey; }
	CString GetNameKeyString() { return m_sNameKey; }
	CString GetTargetKeyString() { return m_sTargetKey; }
	CString GetDefPointString() { return m_sDefPoint; }
	CString GetDefSolidString() { return m_sDefSolid; }
	CString GetMapFileTypeString() { return m_sMapFileType; }
	CString GetTextureTypeString() { return m_sTextureType; }
	CString GetGameExe() { return m_sGameExe; }
	void SetGameExe(CString sGameExe) { m_sGameExe = sGameExe; }
	CString GetGameDir() { return m_sGameDir; }
	void SetGameDir(CString sGameDir) { m_sGameDir = sGameDir; }
	CString GetGameMapDir() { return m_sGameMapDir; }
	void SetGameMapDir(CString sGameMapDir) { m_sGameMapDir = sGameMapDir; }

	CLinkedList<GAMEPAKFILE>* GetPakFileList() { return &m_PakFileList; }
	CLinkedList<ENTITYDEFINITION>* GetEntityDefinitionList() { return &m_EntDefList; }
	CLinkedList<MODEL>* GetModelList() { return m_pModelList; }
	CLinkedList<GAMECAP>* GetGameCapsList() { return &m_CapsList; }

	bool IsCapEnabled(CString sName);

	MODEL* GetCurrentModel() { return m_pCurModel; }
	void SetCurrentModel(MODEL* pModel) { m_pCurModel = pModel; }

	ENTITYDEFINITION* GetWorldspawnDefinition() { return &m_GlobalEntDef; }
	ENTITYDEFINITION* GetCurrentSolidEntity() { return m_pCurSolidEntity; }
	void SetCurrentSolidEntity(ENTITYDEFINITION* pEntDef) { m_pCurSolidEntity = pEntDef; }
	ENTITYDEFINITION* GetCurrentPointEntity() { return m_pCurPointEntity; }
	void SetCurrentPointEntity(ENTITYDEFINITION* pEntDef) { m_pCurPointEntity = pEntDef; }

	bool ParseGDFScript(CString sFilename);

	ENTITYDEFINITION* FindEntityDefinition(CString sClassname);
	CEntity* CreateEntityFromDefinition(CString sClassname);

	// Get's the build section. Used for programmable map compiling features.
	DEFBUILD* GetDefBuild() { return &m_DefBuild; }
	DEFMODULE* GetDefModule(CString sName);

	// Compiles/Run's a map.
	bool RunMap(RUNBLOCK* pRunBlock);

	CLinkedList<GAMEPAKFILE>* GetGamePakList() { return &m_PakFileList; }

	// Friends.
	friend class CGameAPI;

	bool SaveModel(MODEL* pModel);
};	

// The GameAPI controller.
class CBrowserDialog;
class _T_EXPORT CGameAPI
{
private:

	CGameDefinition* m_pCurGameDef;
	CLinkedList<CGameDefinition> m_GameDefinitionList;

	bool ParseGDFScript(CString sFilename);

public:

	CGameAPI();
	~CGameAPI();

	// Returns the list of gdefs.
	CLinkedList<CGameDefinition>* GetGameDefinitionList() { return &m_GameDefinitionList; }
	
	// Purges the current game and data.
	bool PurgeAndReset();

	bool SetCurrentGame(CString sName);
	CGameDefinition* GetCurrentGame() { return m_pCurGameDef; }
	// Returns the game definition with the specified name.
	CGameDefinition* GetGameDefinition(CString sName);

	// Initializes the game api.
	bool Initialize();
	bool SaveRegSettings();

};

// $eb

CGameAPI* GetGameAPI();


#endif