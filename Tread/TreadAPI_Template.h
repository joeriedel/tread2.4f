//////////////////////////////////////////////////////////////////////
// TREADAPI.H														//
// Copyright © 2000 Joe Riedel, Nick Randal. All Rights Reserved.	//
// Author: Joe Riedel.												//
//////////////////////////////////////////////////////////////////////
// This code is supplied WITHOUT WARRANTY OF ANY KIND. So if		//
// something goes wrong, it isn't our fault.						//
//////////////////////////////////////////////////////////////////////
//
// This file contains all the Tread definitions you will need to create
// a plugin. Just about all the guts of Tread are exposed here, at least,
// the definitions are.
//
// FORWARD:
//
// It is my hope that the community will be able to take this and make
// some cool stuff. I expect to see some stuff from David Hyde! One cool
// idea of something is a plugin that actually draws the entities
// .md2 model in the 3D view and wireframe, like Qoole does for Quake.
// Hopefully we'll see GenSurf built in sometime too. These aren't 
// demands, they're just ideas. I've tried to make the plugin system
// as versatile and unlimited as possible, but with certain intentional
// hinderences. Like you can't bypass our license verification with
// a plugin. Here's what has been exposed:
//
// 1) Most of the MFC interface classes have been exposed so you
// can make calls to them when necessary.
//
// 2) The majority of the API's (GameAPI, TexAPI, VisgroupAPI,
// TargetAPI, UndoAPI) are all callable. This is where you will
// draw the majority of your functionality, at least when getting
// info from Tread.
//
// 3) The rendering system is fully expandable by plugins. You can
// override the drawing of buit-in objects. You can create new
// objects and draw them in existing renderers. Or you can create
// entirely new renderers.
//
// 4) A plethora of objects used internally by Tread. The majority
// of Tread works on virtual base classes, hence its expandability.
// Most of the system was designed to be a black-box. For the most
// part you should be able to derrive off the base classes to add
// your own stuff.
//
// 5) The map file system is fully expandable. You can create your
// own objects and save them with the .map. Tread will, however, become
// dependant on the plugin to load the object as well. You can make your
// own map reader/writer's to.
//
// SUPPORT:
//
// Officially we do not support ANYTHING created by the user community.
// If you make it you must make it explicitly clear that we are not the
// ones to come crawling to if it doesn't work. 
//
// We also do not provide "official" support of people developing plugins
// for Tread. That aside, you can always ask us questions, so feel free
// to email us about it. I'd love to work with people answering questions,
// etc, etc.
//
// SOURCE-CODE:
//
// You are not in anyway required to release source code to your plugins.
// You retain full rights to code you write. You have no right to the code
// we released for this API. We may request from time to time to release
// your plugin with selling versions of Tread. You are not entitled to any
// monetary compensation if we do. You, however, retain the right to
// decline the request to release it with Tread.
//
// If you do not plan to sell your plugin then releasing the source code 
// to it will greatly benefit the community. It would be my hope that 
// people start a site that contains Tread plugin source and current plugins 
// and what they do.
//
// CONTACT:
//
// jmriedel@planetquake.com -- That's me: Joe Riedel.
// nrandal@planetquake.com -- That's Nick Randal.
// sarnold@planetquake.com -- That Sean Arnold, our webmaster.
//
// DOCUMENTATION:
//
// Unfortunately the only documentation I plan to provide is what you see
// in this documented source code. Anyone who figures stuff out and would
// like to start documenting things is alright with me, as long as you
// make it clear that it's not official. You can always email Nick or I
// regarding clarification on things. I may release some example plugins.
//
// Look through this header for brief comments on how things works.
//
// EXPLANATION OF SUPPLIED FILES.
//
// The TreadAPI header file defines all the exported Tread definitions
// needed by the plugin. This is the only header you need to include
// to use the definitions. The TreadAPI.cpp file defines some extern
// storage. Sneak a peak if you want.
//
// HookSys.cpp exports a function that Tread hooks and uses to setup
// some global function calls and parameters, such as console routines
// and others. You need to simply include HookSys.h and HookSys.cpp in
// your application.
//
// Mathlib.cpp contains various defintions and function for manipulating
// vectors and quaternions. These are provided so you don't have to
// write your own, unless of course you want to.
//
// LinkedList.h is a template linked list that I think is one of the
// best things we ever did this time around. Alot of stability problems
// in the old versions had to do with directly manipulating list pointers.
// This template class allows you to make a linked list class that
// contains any type of object with prev,next pointers. There is a 
// special base class that you should derrive off of called CLLObject.
// There are exceptions to this, however, in which case you should just
// make prev,next pointers.
//
// wogl.h contains stripped down OGL definitions that Tread uses,
// incase you want to extend the OGL render package.
//
// Tokenizer.cpp is a white space file parsing class. It's very usefull
// and is used throughout Tread. I've decided to supply it to you as
// full source so modifications to it would be allowed. However, be
// carefull that if you change it you make a new class because there
// may be cases where Tread passes a CTokenizer object to you.
//
// SzString.cpp is a small null terminated string library. It works
// on CStrings as well thanx to the LPCTSTR operator.
//
// FUtils.cpp is a file utility library. It provides routines to read
// and write strings etc. It also gives you a class called CFileSearch
// wich is most usefull and will do recursive file searching up directory
// chains. The full source for that is provided as well.
//
#ifndef TREADSYS_H
#define TREADSYS_H

//////////////////////////////////////////////////////////////////////
// This header defines some global flags and system function calls	//
//////////////////////////////////////////////////////////////////////

#include "LinkedList.h"
#include "HookSys.h"
#include "MathLib.h"
#include "Tokenizer.h"
#include "wogl.h"

#define _T_EXPORT
#define T3DPLUGINAPI	extern "C" __declspec (dllexport)
#define T3DAFXSETUP()	AFX_MANAGE_STATE(AfxGetStaticModuleState())

class CMode;
class CMapFrame;
class CMapView;
class CConsoleView;
class CTread3DDoc;
class CMainFrame;
class CBaseObject;
class CRenderAPI;
class CTexture;
class CMode;
class CEntity;
class CVisgroupAPI;
class CVisgroup;
class CUndoAPI;
class CTargetAPI;
class CWindowEventManager;
class CBrowserDialog;
class CTexPalette;
class CPakFile;
class CBaseRenderPackage;
class CBaseTreadMapFile;
class CBaseWindowRenderer;

struct RUNSTATE;
struct TREAD2XHEADERINFO;

// $sb "$renderapi.h block2"
// $eb

//////////////////////////////////////////////////////////////////////
// PLUGINAPI.H														//
//////////////////////////////////////////////////////////////////////
// $sb "pluginapi.h block1"

// Plugin stuff will be inserted here.

// $eb
extern CTreadPluginAPI* (*GetPluginAPI)();

//
// You must export a function of this type, that creates your plugin object:
//
// CTreadPlugin* T3D_CreatePluginInstance();
//

//////////////////////////////////////////////////////////////////////
// TREADSYS.H														//
//////////////////////////////////////////////////////////////////////

/*
Nick : 9/8/99
Added CMode handling.
*/
/*
Joe : 10/10/99
Move flags to hex.
Modes are now bit flags so event responders can bind to 
multiple events.
*/


#ifdef _WIN32
#pragma warning(disable : 4244)     // truncate from double to float
#pragma warning(disable : 4800)		// forcing int to bool.
#endif

// $sb "treadsys.h block1"
// $eb

// Console routines.
extern void (*Debug)(CString sMessage, ...);
extern void (*Warning) (CString sMessage, bool bPopup = false);
extern void (*Error)(CString sMessage, bool bConsole = true);
extern void (*Fatal)(CString sMessage, bool bConsole = false);

// Registry calls.
extern CString (*GetRegString) (UINT nCode);
extern int (*GetRegInt) (UINT nCode);
extern bool (*GetRegBool) (UINT nCode);

extern void (*SetRegString) (UINT nCode, CString sData);
extern void (*SetRegInt) (UINT nCode, int nData);
extern void (*SetRegBool) (UINT nCode, bool bData);

// Tracks a popup menu.
extern void (*TreadPopupMenu) (UINT nMenuId, int nXPos, int nYPos, CWnd* pWnd, bool bTranslate=true);

// Get's the current clipboard.
extern CLinkedList<CBaseObject>* (*GetGlobalClipboard) ();

// $sb "treadsys.h block2"
// $eb

// Set's the mouse cursor.
extern void (*TreadSetCursor) (CMapView* pView, int nCursor);

// Console routines.
extern void (*Console)(CString sMessage, ...);
extern void (*SetConsole)(CString sFont, int nSize, COLORREF nColor);
extern void (*ResetConsole)();
extern void (*ClearConsole)();

// Get's the global mode manager.
extern CMode* (*GetModeManager) ();

// Status bar progess control.
extern void (*PrgSetPos)(int nPos);
extern void (*PrgSetRange)(long nMin, long nMax);
extern void (*PrgStepIt)();
extern void (*PrgSetStep)(int nStep);

//
// The CMapManager class maintains the current list of maps. It's main advantage
// is to be able to get the documents and mode manager of that class by calling:
//
// GetModeManager()->GetDocPtr() which retrieves the current document.
//
// It's heavily used on Nicks side to maintain the interface.
//
// $sb "treadsys.h mapmanager block1"
// $eb
// $sb "treadsys.h mapmanager block2"
// $eb

extern CMapManager* (*GetMapManager) ();

//////////////////////////////////////////////////////////////////////
// VIEWCONTROL.H													//
//////////////////////////////////////////////////////////////////////
// $sb "viewcontrol.h block1"
// $eb

extern CView* (*GetView) (UINT nCode);
extern void (*EnableView) (UINT nCode, bool bEnable = true);
extern void (*EnableAllViews) (bool bEnable = true);

//////////////////////////////////////////////////////////////////////
// MODE.H															//
//////////////////////////////////////////////////////////////////////
// Defined mode controller.
// $sb "mode.h block1"
// $eb

//////////////////////////////////////////////////////////////////////
// TEXCOMBOBOX.H													//
//////////////////////////////////////////////////////////////////////
// $sb "texcombobox.h block1"
// $eb

//////////////////////////////////////////////////////////////////////
// TEXLISTBOX.H														//
//////////////////////////////////////////////////////////////////////
// $sb "texlistbox.h block1"
// $eb

//////////////////////////////////////////////////////////////////////
// VISGROUPVIEW.H													//
//////////////////////////////////////////////////////////////////////
// $sb "visgroupview.h block1"
//$eb
//////////////////////////////////////////////////////////////////////
// TEXTUREVIEW.H													//
//////////////////////////////////////////////////////////////////////
#define IDD_TEXTURE                     160
// $sb "textureview.h block1"
// $eb
//////////////////////////////////////////////////////////////////////
// OBJECTVIEW.H														//
//////////////////////////////////////////////////////////////////////
// $sb "objectview.h block1"
// $eb

//////////////////////////////////////////////////////////////////////
// TARGETAPI.H														//
//////////////////////////////////////////////////////////////////////
//
// This is Treads built in Targetting system (i.e. the system that draws
// lines between objects to represent targets). It works simply by
// associating an object with it's name. To setup this association you
// call: SetTargetBinding("TargetName", pObject);. You can associate
// more than one object with the same name.
//
// Tread will automatically draw the targets of any object that returns
// the I_TARGETABLE interface. This means that it must derrive from
// CBaseTargetObject. Tread looks up the name that the object targets
// and asks the TargetAPI to return a list of objects that have bound
// to that name via SetTargetBinding(). This returns a TTARGET struct
// that contains a list of TREF's. Each TREF references an object.
//
// To unbind an object from it's name call:
// SetTargetBinding("", pObject);.
//
// $sb "targetapi.h block1"
// $eb

//////////////////////////////////////////////////////////////////////
// UNDOAPI.H														//
//////////////////////////////////////////////////////////////////////
//
// Treads UndoAPI is the class that maintains a list of operations
// done on geometry. This list can then be used to "undo" that set
// of operations.
//
// Unfortunately this API is not currently expandable as far as the types
// of operations it will keep track of. This may change in the future.
//
// The UAF_ flags define the operations that Tread will undo/redo. The
// system works simply by storing the operation done, and the list of
// operands effected by the operation. This is stored via the UNDOITEM
// structure.
//
// If you have a list of items that are affected by an action, there
// is a convenient function provided that will produce an UNDOITEM struct
// for you: CUndoAPI::CreateUndoItem().
//
// It would be too exhaustive for me to attempt to explain this whole
// system without source, so I'll probably have to give some sample
// code at some point that uses this.
//

// $sb "undoapi.h block1"
// $eb

//////////////////////////////////////////////////////////////////////
// MAPOBJECT.H														//
//////////////////////////////////////////////////////////////////////
//
// This defines the base classes at which the heart of Tread really lies.
// It works with these base classes, allowing all sorts of crazy derrivations
// to take place without any internal rewritting.
//
// $sb "mapobject.h block1"
// $eb

extern CBaseObject* (*NewBaseObject) (int nClassType);

//////////////////////////////////////////////////////////////////////
// VISGROUP.H														//
//////////////////////////////////////////////////////////////////////
// Defines visgroup api system.										

// $sb "visgroup.h block1"
// $eb

extern CVisgroupAPI* (*GetVisgroupAPI) ();

//////////////////////////////////////////////////////////////////////
// TREADMAPAPI.H													//
//////////////////////////////////////////////////////////////////////
// Treads map system.
// $sb "treadmapapi.h block1"
// $eb
extern CTreadMapAPI* (*GetTreadMapAPI) ();

//////////////////////////////////////////////////////////////////////
// EVENTMANAGER.H													//
//////////////////////////////////////////////////////////////////////
// $sb "eventmanager.h block1"
// $eb

//////////////////////////////////////////////////////////////////////
// DEFAULTACTIONRESPONDER.H											//
// $sb "defaultactionresponder.h block1"								//
// $eb																//
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// HOTSPOTMANAGER.H													//
//////////////////////////////////////////////////////////////////////
// Hotspot system.
// $sb "hotspotmanager.h block1"
// $eb

//////////////////////////////////////////////////////////////////////
// MODELS.H															//
//////////////////////////////////////////////////////////////////////
// $sb "models.h block1"
// $eb

//////////////////////////////////////////////////////////////////////
// Tread Texturing System.											//
//////////////////////////////////////////////////////////////////////

// Defines a texture.
class CTexReader;
class CTexPalette;

// $sb "texture.h block1"
// $eb

// $sb "texpalette.h block1"
// $eb

// $sb "pakfile.h block1"
// $eb

// $sb "texreader.h block1"
// $eb

// $sb "texdir.h block1"
// $eb

// $sb "pakloader.h block1"
// $eb

// $sb "texapi.h block1"
// $eb

// Get's the texAPI.
extern CTexAPI* (*GetTexAPI)();

//////////////////////////////////////////////////////////////////////
// WINDING.H														//
//////////////////////////////////////////////////////////////////////
// $sb "winding.h block1"
// $eb

//////////////////////////////////////////////////////////////////////
// FACE.H															//
//////////////////////////////////////////////////////////////////////
// $sb "face.h block1"
// $eb

//////////////////////////////////////////////////////////////////////
// BRUSH.H															//
//////////////////////////////////////////////////////////////////////
// $sb "brush.h block1"
// $eb

//////////////////////////////////////////////////////////////////////
// ENTITY.H															//
//////////////////////////////////////////////////////////////////////
// $sb "entity.h block1"
// $eb

//////////////////////////////////////////////////////////////////////
// CAMERA.H															//
//////////////////////////////////////////////////////////////////////

// $sb "camera.h block1"
// $eb

//////////////////////////////////////////////////////////////////////
// CCLIPTOOL.H														//
//////////////////////////////////////////////////////////////////////
// $sb "cliptool.h block1"
// $eb

//////////////////////////////////////////////////////////////////////
// GAMEAPI.H														//
//////////////////////////////////////////////////////////////////////
// The Game Definition System.										//
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
// Defines the entity scripting classes and definitions structures. //
//////////////////////////////////////////////////////////////////////
// $sb "gameapi.h block1"
// $eb

extern CGameAPI* (*GetGameAPI) ();

//////////////////////////////////////////////////////////////////////
// General purpose routines.										//
//////////////////////////////////////////////////////////////////////

// This is the reference stuff. NOTE: we need to check this
// after the plugin header is built.
/*
// $sb "geometry.h block1"
// $eb
*/

#define GF_NONE			0x00000000
#define GF_ORIGIN		0x00000001	// Build the origin.
#define GF_RANGE		0x00000002	// Build the range.

struct GDATA
{
	int m_nFlags;
	vec3_t m_vMins, m_vMaxs, m_vOrigin;
	CLinkedList<CBaseObject>* m_pList;
};

//extern int gnRecurseCount;

extern void (*G_Undo)(CLinkedList<UNDOREF>* pList, CTread3DDoc* pDocument, int nFlags);
extern void (*G_Redo)(CLinkedList<UNDOREF>* pList, CTread3DDoc* pDocument, int nFlags);
extern void (*G_PreDelete)(CLinkedList<CBaseObject>* pList, CTread3DDoc* pDocument);
extern CBaseObject* (*G_FindObject)(CLinkedList<CBaseObject>* pList, int nClassType);
extern void (*G_SplitObjectList)(CPlane* pPlane, CLinkedList<CBaseObject>* pSrcList, CLinkedList<CBaseObject>* pFrontList, CLinkedList<CBaseObject>* pDstList);
extern void (*G_InitFromSelection)(CLinkedList<CBaseObject>* pList, CTread3DDoc* pDocument, CMapView* pView, vec3_t vMins, vec3_t vMaxs, vec3_t vOrigin);
extern void (*G_Translate)(vec3_t vTrans, GDATA* pGD);
extern void (*G_Scale)(vec3_t vScale, GDATA* pGD);
extern void (*G_SetOrigin)(vec3_t vOrigin, GDATA* pGD);
extern void (*G_Reflect)(int nAxis, GDATA* pGD);
extern void (*G_Rotate)(vec3_t vRot, GDATA* pGD);
extern void (*G_Build)(GDATA* pGD);
extern float (*G_SnapFloat)(vec_t fPoint, int nGridSize);
extern float (*G_SnapFloatDelta)(vec_t fPoint, vec_t fLast, int nGridSize);
extern void (*G_SnapVector)(vec3_t vPoint, vec3_t vOut, int nGridSize, vec_t* pTrans=NULL);
extern void (*G_SnapVectorDelta)(vec3_t vPoint, vec3_t vLast, vec3_t vOut, int nGridSize, vec_t* pTrans=NULL);
extern void (*G_Snap)(GDATA* pGD, int nGridSize, vec_t* pTrans=NULL);
extern void (*G_SnapDelta)(GDATA* pGD, vec3_t vLast, int nGridSize, vec_t* pTrans=NULL);
extern void (*G_SetSize)(vec3_t vSize, GDATA* pGD);
extern void (*G_AddToMap)(CLinkedList<CBaseObject>* pList, CTread3DDoc* pDocument);
extern void (*G_AddToSelection)(CLinkedList<CBaseObject>* pList, CTread3DDoc* pDocument);
extern void (*G_CopyList)(CLinkedList<CBaseObject>* pSrc, CLinkedList<CBaseObject>* pDst);
extern void (*G_TextureList)(CLinkedList<CBaseObject>* pList, CString sTexture);
extern void (*G_TransferList)(CLinkedList<CBaseObject>* pSrc, CLinkedList<CBaseObject>* pDst);
extern int (*G_GetInterfaceCount)(CLinkedList<CBaseObject>* pList, int nInterface=I_ALL);
extern int (*G_GetClassCount)(CLinkedList<CBaseObject>* pList, int nClass=OBJECT_CLASS_ALL);
extern bool (*G_WriteObjectList)(int nVersion, CFile& hFile, CLinkedList<CBaseObject>* pList);
extern bool (*G_ReadObjectList)(int nNumObjects, int nVersion, CFile& hFile, CLinkedList<CBaseObject>* pList, CLinkedList<CBaseObject>* pUIList);
extern void (*G_InitAfterLoad)(CLinkedList<CBaseObject>* pList, CTread3DDoc* pDocument);

//////////////////////////////////////////////////////////////////////
// COLORTABLE.H														//
//////////////////////////////////////////////////////////////////////
// $sb "colortable.h block1"
// $eb

extern CColorTable* (*GetColorTable) ();

//////////////////////////////////////////////////////////////////////
// RENDERAPI.H														//
//////////////////////////////////////////////////////////////////////

// Defines the view orientation array.
extern GVIEWOR* gViewOr;
extern GVIEWIN* gViewIn;
extern G2DINDEX* g2DIndex;

#define VMI(ViewType, Axis)	((*gViewIn)[ViewType][Axis])	// Maps Vector Index.
#define VO(ViewType, Axis)	((*gViewOr)[ViewType][Axis])	// Get's Vector Orientation.
#define VI(Axis, v)			(VMI(VT(v), Axis))
#define T2I(ViewType, Axis)	((*g2DIndex)[ViewType][Axis])
#define V2I(Axis, v)		(T2I(v->m_nView, Axis))
#define TU(ViewType)		((*gViewIn)[ViewType][VEC_Z])
#define VU(v)				(TU(v->m_nView))
#define VMO(Axis, v)		(VO(v->m_nView, Axis))

// $sb "renderapi.h block1"
// $eb

// $sb "renderapi.h block3"
// $eb

// $sb "glrenderpackage.h block1"
// $eb

// $sb "gdiwindowrenderer.h block1"
// $eb

// $sb "gl3dwindowrenderer.h block1"
// $eb

// $sb "glwindowrenderer.h block1"
// $eb

extern CRenderAPI* (*GetRenderAPI) ();

//////////////////////////////////////////////////////////////////////
// MAPFRAME.H														//
//////////////////////////////////////////////////////////////////////
// $sb "childfrm.h block1"
// $eb

//////////////////////////////////////////////////////////////////////
// MAPVIEW.H														//
//////////////////////////////////////////////////////////////////////
// $sb "mapview.h block1"
// $eb

//////////////////////////////////////////////////////////////////////
// TREAD3DDOC.H														//
//////////////////////////////////////////////////////////////////////
// Defines Map Context, MFC Document used by Tread.
// $sb "tread3ddoc.h block1"
// $eb

#endif