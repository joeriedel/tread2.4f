// SHAPEPLUGIN.H
// Copyright © 2000 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#ifndef SHAPEPLUGIN_H
#define SHAPEPLUGIN_H
/*
//////////////////////////////////////////////////////////////////////
// FORWARD															//
//																	//
// I just keep adding features :) This plugin allows the user to	//
// fabricate shapes from dialogs. It's pretty cool. There is no		//
// real trick to this. We just throw in some menu's and pop-up		//
// dialogs.															//
//																	//
// One thing I found was that the VC generated stdafx.h file does	//
// not include afxcview.h which has definitions for some stuff in	//
// the TreadAPI.h file. When creating plugins that include MFC		//
// stuff you need to replace the stdafx files with the ones			//
// we've provided, or include afxcview.h in the stdafx.h file.		//
//////////////////////////////////////////////////////////////////////
*/
#include "../TreadAPI.h"
#include "ArchDialog.h"
#include "ConeDialog.h"
#include "CylinderDialog.h"

class CShapesPlugin : public CTreadPlugin
{
private:

	CArchDialog m_ArchDialog;
	CConeDialog m_ConeDialog;
	CCylinderDialog m_CylinderDialog;

	TPHMENUITEM m_hCone, m_hCylinder, m_hArch;

	void MakeArch();
	void MakeCone();
	void MakeCylinder();

public:

	CShapesPlugin();
	virtual ~CShapesPlugin();

	virtual int GetAPIVersion();
	virtual int InitializePlugin();
	virtual int ShutdownPlugin();
	virtual CString GetPluginName();

	virtual int InvokePlugin(CTread3DDoc* pDocument, TPHMENUITEM nMenuID);
};

#endif