// LEAKTRACEPLUGIN.H
// Copyright © 2000 Joe Riedel, Nick Randal.
// Author: Joe Riedel.

/*
//////////////////////////////////////////////////////////////////////
// FORWARD:															//
//																	//
// This sample plugin displays the leak-trace view that Tread		//
// supplies	for loading the .lin file that QBSP3 (or derrivitives)	//
// create to aid in leak finding.									//
//																	//
// I intend to exhaustively explain (in code comments) exactly what //
// is going on, as the purpose of this is to help explain how to	//
// make a good plugin.												//
//																	//
// The scope of what a plugin can do in Tread is almost limitless.	//
// An ambitios group of people could beat Nick and I to the chase	//
// with Q3A support if they were so inclined. That's how flexible	//
// it is. I have tried to expose as much as I could of the internal	//
// system.															//
//																	//
// Unfortunately this plugin will only barely scratch the surface	//
// of what can be done. It does, however, illustrate how to make	//
// a plugin. It also shows how a plugin can create it's own object	//
// and draw that object in the views under the existing render		//
// packages that have been written.									//
//																	//
// I won't get into how to write your own rendering libraries but	//
// this is definently possible. Someone could write a DirectX		//
// renderAPI for Tread, or a software renderer, etc, etc.			//
//																	//
// At some later date I may release another tutorial sample that	//
// illustrates more advanced concepts like texture loading etc.		//
//																	//
// Anyone is welcome to create tutorial plugins :)					//
//																	//
//																	//
// WHAT A PLUGIN IS:												//
//																	//
// A plugin is simply a piece of code that Tread invokes that was	//
// written to extend functionality which was not originally			//
// included in the program. These plugins can be passed around		//
// and used by anyone who has a registered version of the editor.	//
//																	//
// SPECIFICALLY HOW TREAD USES THEM:								//
//																	//
// Tread searches it's plugin directory for dll's that contain		//
// plugins. Tread searches the dll's export table for 2 routines	//
// used to create the plugin object. Once Tread has verified that	//
// the dll contains a Tread plugin, it requests that the dll		//
// construct a plugin object and return it to the system. Tread		//
// will then invoke the plugin when requested.						//
//////////////////////////////////////////////////////////////////////
*/

#ifndef LEAKTRACEPLUGIN_H
#define LEAKTRACEPLUGIN_H

// Look in TreadAPI.h for short explanation of supplied files.
#include "../TreadAPI.h"

// Class ID.
// TreadAPI defines OBJECT_CLASS_RESERVED_LAST. It is simply,
// the last bit that Tread3D reserved for it's use with object class id's.
// Plugins are free to use any other upper bits.
// TreadAPI conveniently defines OBJECT_CLASS_USER1-OBJECT_CLASS_USER8
#define EXT_OBJECT_CLASS_LEAKTRACE OBJECT_CLASS_USER1

// This is defining the leak trace object that will actually
// be stored in the map and drawn.
#define MAX_LEAK_PTS	256
class CLeakTraceObject : public CBaseObject
{

	friend class CLeakTrace;

private:

	int m_nNumPts;
	vec3_t* m_vPts;

public:

	CLeakTraceObject()
	{
		m_vPts = 0;
		m_nNumPts = 0;
	}

	~CLeakTraceObject()
	{
		if(m_vPts != NULL)
			delete[] m_vPts;
	}

	// By returning I_UI Tread knows that this is a UI object
	// and it will be maintained in the proper lists. A UI is a thing
	// like a camera. It doesn't directly relate to any geometry in the map.
	// UI objects are also drawn on top of everything else.
	virtual int GetObjectInterfaces() { return I_UI; }
	virtual int GetObjectClass() { return EXT_OBJECT_CLASS_LEAKTRACE; }
};

// Here we are defining the plugin class that the DLL will create
// and return to Tread. For a discussion of the member routines,
// look in the LeakTracePlugin.cpp file.
class CLeakTrace : public CTreadPlugin
{
private:

	CPen* m_pLeakPen;
	TPHMENUITEM m_hMenuItem;

	CLeakTraceObject* LoadLeakFile(CString sFilename);
	CLeakTraceObject* LoadQ2Leak(CTokenizer& Tokenizer);

public:

	CLeakTrace();
	virtual ~CLeakTrace();

	virtual int GetAPIVersion();
	virtual int InitializePlugin();
	virtual int ShutdownPlugin();
	virtual CString GetPluginName();

	virtual int InvokePlugin(CTread3DDoc* pDocument, TPHMENUITEM nMenuID);
	virtual void OnEvent(int nEventFlags, int nFlags, int nMode, int nSubmode, CTread3DDoc* pDocument, void* pParam=NULL, void* pParam2=NULL, void* pParam3=NULL, void* pParam4=NULL);

	// Draws an object.
	virtual bool DrawObject(CMapView* pView, CTread3DDoc* pDocument, CBaseWindowRenderer* pWindowRenderer, CDC* pDC, void* pContext, CBaseObject* pObject, int nObjectClass, int nObjectInterfaces, int nDrawFlags, int nFlags, int nPAKUID, int nStage, int nType);

	// We draw in both GDI & GL.
	void DrawLeakTrace_WGDI(CMapView* pView, CTread3DDoc* pDocument, CDC* pDC, CDC* pDrawDC, CLeakTraceObject* pLT);
	void DrawLeakTrace_WOGL(CMapView* pView, CTread3DDoc* pDocument, CDC* pDC, WOGLFUNCTIONTABLE* pGLFT, CLeakTraceObject* pLT);
	void DrawLeakTrace_WOGL3D(CMapView* pView, CTread3DDoc* pDocument, CDC* pDC, WOGLFUNCTIONTABLE* pGLFT, CLeakTraceObject* pLT);
};

#endif