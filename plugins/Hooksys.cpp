//////////////////////////////////////////////////////////////////////
// HOOKSYS.CPP														//
// Copyright © 2000 Joe Riedel, Nick Randal.						//
// Author: Joe Riedel.												//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TreadAPI.h"
#include "HookSys.h"

/*struct T3DEXPORTTABLE
{
	TF_DEBUG Debug;
	TF_WARNING Warning;
	TF_ERROR Error;
	TF_FATAL Fatal;
	TF_CONSOLE Console;
	TF_SETCONSOLE SetConsole;
	TF_RESETCONSOLE ResetConsole;
	TF_CLEARCONSOLE ClearConsole;

	TF_GETMAPMANAGER GetMapManager;
	TF_GETMODEMANAGER GetModeManager;

	TF_GETGLOBALCLIPBOARD GetGlobalClipboard;

	TF_GETVIEW GetView;
	TF_ENABLEVIEW EnableView;
	TF_ENABLEALLVIEWS EnableAllViews;

	TF_GETVISGROUPAPI GetVisgroupAPI;
	TF_GETTREADMAPAPI GetTreadMapAPI;
	TF_GETGAMEAPI GetGameAPI;
	TF_GETTEXAPI GetTexAPI;
	TF_GETCOLORTABLE GetColorTable;
	TF_GETPLUGINAPI GetPluginAPI;

	TF_NEWBASEOBJECT NewBaseObject;

	TF_PRGSETPOS PrgSetPos;
	TF_PRGSETRANGE PrgSetRange;
	TF_PRGSTEPIT PrgStepIt;
	TF_PRGSETSTEP PrgSetStep;

	TF_G_UNDO	G_Undo;
	TF_G_REDO	G_Redo;
	TF_G_PREDELETE	G_PreDelete;
	TF_G_FINDOBJECT	G_FindObject;
	TF_G_COPYOBJECT	G_CopyObject;
	TF_G_SPLITOBJECTLIST	G_SplitObjectList;
	TF_G_INITASMODEL G_InitAsModel;
	TF_G_TRANSLATE G_Translate;
	TF_G_SCALE G_Scale;
	TF_G_SETORIGIN G_SetOrigin;
	TF_G_REFLECT G_Reflect;
	TF_G_ROTATE G_Rotate;
	TF_G_BUILD G_Build;
	TF_G_SNAPFLOAT G_SnapFloat;
	TF_G_SNAPFLOATDELTA G_SnapFloatDelta;
	TF_G_SNAPVECTOR G_SnapVector;
	TF_G_SNAPVECTORDELTA G_SnapVectorDelta;
	TF_G_SNAP G_Snap;
	TF_G_SNAPDELTA G_SnapDelta;
	TF_G_SETSIZE G_SetSize;
	TF_G_ADDTOMAP G_AddToMap;
	TF_G_ADDTOSELECTION G_AddToSelection;
	TF_G_COPYLIST G_CopyList;
	TF_G_TEXTURELIST G_TextureList;
	TF_G_TRANSFERLIST G_TransferList;
	TF_G_GETINTERFACECOUNT G_GetInterfaceCount;
	TF_G_GETCLASSCOUNT G_GetClassCount;
	TF_G_WRITEOBJECTLIST G_WriteObjectList;
	TF_G_READOBJECTLIST G_ReadObjectList;
	TF_G_INITAFTERLOAD G_InitAfterLoad;

	GVIEWOR* gViewOr;
	GVIEWIN* gViewIn;
	G2DINDEX* g2DIndex;
};*/


T3DPLUGINAPI void T3D_RegisterFunctionTable(T3DEXPORTTABLE* pTable)
{
	//T3DAFXSETUP();

	Debug = pTable->Debug;
	Warning = pTable->Warning;
	Error = pTable->Error;
	Fatal = pTable->Fatal;
	Console = pTable->Console;
	ClearConsole = pTable->ClearConsole;
	ResetConsole = pTable->ResetConsole;
	SetConsole = pTable->SetConsole;
	GetGlobalClipboard = pTable->GetGlobalClipboard;
	GetMapManager = pTable->GetMapManager;
	GetModeManager = pTable->GetModeManager;
	GetView = pTable->GetView;
	EnableView = pTable->EnableView;
	EnableAllViews = pTable->EnableAllViews;
	GetVisgroupAPI = pTable->GetVisgroupAPI;
	GetTreadMapAPI = pTable->GetTreadMapAPI;
	GetGameAPI = pTable->GetGameAPI;
	GetTexAPI = pTable->GetTexAPI;
	GetColorTable = pTable->GetColorTable;
	GetPluginAPI = pTable->GetPluginAPI;
	NewBaseObject = pTable->NewBaseObject;
	PrgSetPos = pTable->PrgSetPos;
	PrgSetRange = pTable->PrgSetRange;
	PrgStepIt = pTable->PrgStepIt;
	PrgSetStep = pTable->PrgSetStep;
	G_Undo = pTable->G_Undo;
	G_Redo = pTable->G_Redo;
	G_PreDelete = pTable->G_PreDelete;
	G_FindObject = pTable->G_FindObject;
	G_SplitObjectList = pTable->G_SplitObjectList;
	//G_InitAsModel = pTable->G_InitAsModel;
	G_Translate = pTable->G_Translate;
	G_Scale = pTable->G_Scale;
	G_SetOrigin = pTable->G_SetOrigin;
	G_Reflect = pTable->G_Reflect;
	G_Rotate = pTable->G_Rotate;
	G_Build = pTable->G_Build;
	G_SnapFloat = pTable->G_SnapFloat;
	G_SnapFloatDelta = pTable->G_SnapFloatDelta;
	G_SnapVector = pTable->G_SnapVector;
	G_SnapVectorDelta = pTable->G_SnapVectorDelta;
	G_Snap = pTable->G_Snap;
	G_SnapDelta = pTable->G_SnapDelta;
	G_SetSize = pTable->G_SetSize;
	G_AddToMap = pTable->G_AddToMap;
	G_AddToSelection = pTable->G_AddToSelection;
	G_CopyList = pTable->G_CopyList;
	G_TextureList = pTable->G_TextureList;
	G_TransferList = pTable->G_TransferList;
	G_GetInterfaceCount = pTable->G_GetInterfaceCount;
	G_GetClassCount = pTable->G_GetClassCount;
	G_WriteObjectList = pTable->G_WriteObjectList;
	G_ReadObjectList = pTable->G_ReadObjectList;
	G_InitAfterLoad = pTable->G_InitAfterLoad;
	G_InitFromSelection = pTable->G_InitFromSelection;
	gViewOr = pTable->gViewOr;
	gViewIn = pTable->gViewIn;
	g2DIndex = pTable->g2DIndex;
}
