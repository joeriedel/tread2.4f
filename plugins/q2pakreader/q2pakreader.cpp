// Q2PAKREADER.CPP
// Copyright © 2000 Joe Riedel, Nick Randal.
// Author: Joe Riedel.

/*
//////////////////////////////////////////////////////////////////////
// Treads texture API is a dynamic system that allows texture/pak	//
// readers to be plugged into it thefore enabling it to read new	//
// texture formts.													//
//																	//
// The API deals with base level C++ classes upon which all			//
// functionality is extended. Classes are defined for texture		//
// package readers (i.e. a class that can read a file with a		//
// collection of texture in it), and texture readers (i.e. a class	//
// that knows how to read a specific texture format such as targa,	//
// or jpeg). The goal of all of these classes is to finally produce	//
// an array of CTexture objects that Tread can use.					//
//																	//
// The CTexture object stores information about a texture, and its	//
// raw image data for use in drawing. CTexture objects define		//
// routines to Load and Free themselves as well. When Tread wants	//
// to display the texture or get information about it, it will		//
// request that the texture load itself to first ensure that the	//
// data is valid.													//
//																	//
// Several things must take place for a texture to load itself.		//
// The texture may have come out of a .pak file, or it may be a		//
// lone file on disk somewhere in a directory. When a texture		//
// goes to load itself it asks it's format reader to read the data	//
// off the disk and into the texture object. Every texture has		//
// a format reader attached to it. One format reader may be bound	//
// by more than one texture. Here's what it looks like:				//
//																	//
//   -------            ---------            --------------			//
//   |Tread|-->(Load)-->|Texture|-->(Load)-->|FormatReader|			//
//   -------            ---------            --------------			//
//																	//
// Every format reader gets created from a pak reader object.		//
// As the pak reader parses out texture names, it attaches the		//
// correct reader to each texture. This implies that a pak reader	//
// must be able to determine the format of the texture in the case	//
// that a texture may be of more than a single format (i.e. a pak	//
// that holds .jpg and .tga files). Each format reader is attached	//
// to a pak reader.													//
//																	//
// When the load request arrives at the format reader the reader	//
// first requests its pak reader to position the file pointer at	//
// the start of the texture being loaded. The format reader	then	//
// requests the file pointer from the pak reader, and proceeds to	//
// load the data. Once the data has been loaded the format reader	//
// returns control of the file pointer to the pak reader. Here's	//
// what it looks like:												//
//																	//
//     --------------   			              -----------		//
//  1. |FormatReader|-->(position file pointer)-->|PakReader|		//
//     --------------                             -----------		//
//																	//
//     --------------                             -----------		//
//  2. |FormatReader|<--(requests file pointer)<--|PakReader|		//
//     --------------                             -----------		//
//																	//
//  3. Read texture data.											//
//																	//
//     --------------                           -----------			//
//  4. |FormatReader|-->(return file pointer)-->|PakReader|			//
//     --------------                           -----------			//
//																	//
// When this has been completed the texture has been loaded and is	//
// ready to be displayed.											//
//																	//
// PAKLOADING:														//
//																	//
// Tread never deals with a single texture. Instead it deals with	//
// collections of them. These containers are refered to as Paks.	//
// This is as precise as it gets with Tread: a pak is just a		//
// collection of textures. Therefore Tread does not distinguish		//
// between a Quake3 .pk3 file and a texture directory full of .jpg	//
// files. When they get to Tread they are both Pak objects with the	//
// same functionality. This allows directories of texture to be		//
// loaded seamlessly.												//
//																	//
// A pak object is responsible for opening the requested file		//
// (or directory) and making a list of textures. The pak object		//
// does not need to read any texture data and shouldn't unless		//
// it's necessary. The pak object is then responsible to attach		//
// the correct format readers to the textures before returning.		//
//																	//
// Once Tread has initialized all the pak objects it calls			//
// BuildPak() on each one. During this stage of initialization		//
// each pak can look at the contents of others and add/remove		//
// textures. This is usefull, for example, in Q3 shader parsing		//
// where the contents of other paks needs to be known.				//
//																	//
// The program flow through the TexAPI may not be very clear after	//
// this explanation. Code is worth a thousand words. So I leave you	//
// with this example. I will try to be as descriptive as possible.	//
//////////////////////////////////////////////////////////////////////
*/

#include "stdafx.h"
#include "../TreadAPI.h"
#include "Q2PakReader.h"
#include "Q2PakFile.h"
#include "Q2TexDir.h"

T3DPLUGINAPI CTreadPlugin* T3D_CreatePluginInstance()
{ return new CQ2PakReader(); }

CQ2PakReader::CQ2PakReader()
{}

CQ2PakReader::~CQ2PakReader()
{}

//
// GetAPIVersion(): called by Tread to see what version of the API
// the plugin has been compiled with. Should always return: TP_API_VERSION.
//
int CQ2PakReader::GetAPIVersion()
{ return TP_API_VERSION; }

//
// Called to do one-time initialization when the plugin is started.
//
int CQ2PakReader::InitializePlugin()
{
	// We use BindFunction() to bind some functionality of this plugin.
	// This plugin is capable of reader q2 wal files from a .pak file or
	// from a straight directory. Tell Tread to link us into this part of the system.
	GetPluginAPI()->BindFunction(this, PFF_TPAKREADER|PFF_TDIRREADER);

	// NOTE: We don't make any menu's. This plugin is a function only plugin and does
	// not interface to the user.

	return TPF_OK;
}

//
// Called to shutdown the plugin and perform any cleanup necessary before
// the system exits.
//
int CQ2PakReader::ShutdownPlugin()
{ return TPF_OK; }

//
// Called to get the name of the plugin. Used internally for invocation. This
// should be unique.
//
CString CQ2PakReader::GetPluginName()
{ return "Q2PakReader"; }

//
// Called when the user selects the plugin from a menu item. pDocument
// is the current map. For more info on the CTread3DDoc class, look at the
// declaration of the class.
//
int CQ2PakReader::InvokePlugin(CTread3DDoc* pDocument, TPHMENUITEM nMenuID)
{ return TPF_OK; }

//
// If the plugin bound PFF_TDIRREADER, then this routine is called to open
// a texture directory. More information on the Texture system is available
// at the CTexAPI declaration.
//
CPakFile* CQ2PakReader::OpenTextureDirectory(CString sFilename)
{
	// When we get called here we look in the script to tell us the format
	// of texture directories in this game.
	CString sType = GetGameAPI()->GetCurrentGame()->GetTextureTypeString();

	// We only handle @Q2WAL or @TARGA type (i.e. we can load .wal or .tga files.
	if(sType == "@Q2WAL" || sType == "@TARGA")
		return new CQ2TexDir();

	// We could not load this.
	return NULL;
}

//
// Similiar to OpenTextureDirectory(). The plugin must have bound PFF_TPAKREADER.
// Called to open and parse the texture directory.
//
CPakFile* CQ2PakReader::OpenTexturePackage(CString sFilename)
{
	// This is a request to open a texture package or libary.
	// We simply defer the question of whether or not we can load
	// this file to the Q2PakFile class.
	if(CQ2PakFile::ClaimFile(sFilename))
		return new CQ2PakFile();

	// We were unable to load this pak.
	return NULL;
}