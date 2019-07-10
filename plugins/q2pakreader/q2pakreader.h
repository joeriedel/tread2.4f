// Q2PAKREADER.H
// Copyright © 2000 Joe Riedel, Nick Randal.
// Author: Joe Riedel.

/*
//////////////////////////////////////////////////////////////////////
// FORWARD:															//
//																	//
// This sample plugin takes over the Quake2(tm) .wal file loading.	//
//																	//
// This will cover the TexAPI subsystem of Tread and how to use it	//
// in a plugin. This will not go over the basics of how a plugin	//
// should be made. For imformation regarding that please consult	//
// the "LeakTrace" plugin example.									//
//																	//
//////////////////////////////////////////////////////////////////////
*/
#ifndef Q2PAKREADER_H
#define Q2PAKREADER_H

#include "../TreadAPI.h"

class CQ2PakReader : public CTreadPlugin
{
public:

	CQ2PakReader();
	virtual ~CQ2PakReader();

	//
	// GetAPIVersion(): called by Tread to see what version of the API
	// the plugin has been compiled with. Should always return: TP_API_VERSION.
	//
	virtual int GetAPIVersion();

	//
	// Called to do one-time initialization when the plugin is started.
	//
	virtual int InitializePlugin();

	//
	// Called to shutdown the plugin and perform any cleanup necessary before
	// the system exits.
	//
	virtual int ShutdownPlugin();

	//
	// Called to get the name of the plugin. Used internally for invocation. This
	// should be unique.
	//
	virtual CString GetPluginName();

	//
	// Called when the user selects the plugin from a menu item. pDocument
	// is the current map. For more info on the CTread3DDoc class, look at the
	// declaration of the class.
	//
	virtual int InvokePlugin(CTread3DDoc* pDocument, TPHMENUITEM nMenuID);

	//
	// If the plugin bound PFF_TDIRREADER, then this routine is called to open
	// a texture directory. More information on the Texture system is available
	// at the CTexAPI declaration.
	//
	virtual CPakFile* OpenTextureDirectory(CString sFilename);

	//
	// Similiar to OpenTextureDirectory(). The plugin must have bound PFF_TPAKREADER.
	// Called to open and parse the texture directory.
	//
	virtual CPakFile* OpenTexturePackage(CString sFilename);

};

#endif