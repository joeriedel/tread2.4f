// LEAKTRACEPLUGIN.CPP
// Copyright © 2000 Joe Riedel, and Nick Randal.
// Author: Joe Riedel.
// Example plugin.
#include "stdafx.h"
#include "../TreadAPI.h"
#include "LeakTracePlugin.h"

// If you haven't looked at LeakTracePlugin.h, do so before browsing this file.

// All plugin .dll's must export a function called "T3D_CreatePluginInstance".
// If you don't Tread won't load your plugin. This should create a new instance
// of your plugin class. Don't return a static variable, as Tread will attempt
// to delete the object when it shuts down. Deleting a static variable will cause
// some strange behaviour. Use the T3DPLUGINAPI macro as shown here to ensure that
// the correct calling convention is used. It will also automatically export the 
// routine using an extern "C" block to remove name mangling. This is important,
// because Tread will look up by the non-mangled name.
T3DPLUGINAPI CTreadPlugin* T3D_CreatePluginInstance()
{ 
	return new CLeakTrace();
}

// Our constructor/destructor.
// We create our gdi drawing pen.
#define GDI_LEAK_COLOR RGB(230, 230, 0)
CLeakTrace::CLeakTrace()
{
	m_pLeakPen = new CPen(PS_SOLID, 1, GDI_LEAK_COLOR);
}

// Here we delete our gdi pen.
CLeakTrace::~CLeakTrace()
{
	delete m_pLeakPen;
}

// The API version let's Tread verify that it can correctly export
// the HookSys function table. It's a type of version checking.
int CLeakTrace::GetAPIVersion()
{
	return TP_API_VERSION;
}

// This will load a leak file up.
CLeakTraceObject* CLeakTrace::LoadLeakFile(CString sFilename)
{
	CTokenizer Tokenizer;

	// Here we use the CTokenizer class provided courtesy of us.
	if(!Tokenizer.InitParsing(sFilename))
		return NULL;

	// The GameAPI sub-system main job is to break down the gdf scripts into
	// program usable set of structures. GetMapFileTypeString() will return the
	// string declared in the 'def_gamemaptype' token in the script file. For a more
	// thorough description, look at TreadAPI.h and find then CGameAPI class
	// definition.

	// What game are we loading?
	CString sType = GetGameAPI()->GetCurrentGame()->GetMapFileTypeString();

	if(sType == "QUAKE2_MAP")
		return LoadQ2Leak(Tokenizer);

	return NULL;
}

// The Quake2 Leak Point File simply contains 3 pair coordinates.
CLeakTraceObject* CLeakTrace::LoadQ2Leak(CTokenizer& Tokenizer)
{

	int nNumPts;
	vec3_t vPts[MAX_LEAK_PTS];
	CString sToken;

	nNumPts = 0;
	for(;;)
	{
		if(!Tokenizer.GetToken(sToken))
			break;

		if(nNumPts == MAX_LEAK_PTS)
		{
			Warning("MAX_LEAK_PTS");
			break;
		}

		vPts[nNumPts][0] = atof(sToken);
		Tokenizer.GetToken(sToken);
		vPts[nNumPts][1] = atof(sToken);
		Tokenizer.GetToken(sToken);
		vPts[nNumPts][2] = atof(sToken);

		nNumPts++;
	}

	// Copy them.
	CLeakTraceObject* pLT = new CLeakTraceObject();
	pLT->m_vPts = new vec3_t[nNumPts];

	memcpy(pLT->m_vPts, vPts, nNumPts*sizeof(vec3_t));
	pLT->m_nNumPts = nNumPts;

	return pLT;
}

// This is called when the plugin is loaded. It's called once at the beginning of the plugins life to
// allow it to do any necessary initialization.
// If something goes wrong and you want Tread to abort the loading of the plugin
// then return TPF_ERROR. Otherwise, return TPF_OK;
int CLeakTrace::InitializePlugin()
{
	// During this stage of initialization we bind a functionality options via BindFunction()
	// that tells Tread that we can draw objects. This function is used to
	// bind all kinds of functionality options. If a function is not bound
	// Tread will never invoke the routines on the plugin that corresponds
	// to that function. Be sure that you don't bind an event or function
	// and then return TPF_ERROR as you will cause Tread to crash somewhere
	// down the line. After you bind the event/function you MUST return TPF_OK, unless
	// you unbind all your event/function(s).
	
	// Here we bind the POSTDRAWOBJECT function. For a more thorough
	// description look at the DrawObject() routine.
	GetPluginAPI()->BindFunction(this, PFF_POSTDRAWOBJECT);
	GetPluginAPI()->BindPostDrawClasses(this, EXT_OBJECT_CLASS_LEAKTRACE, PDT_2D|PDT_3D);
	// Here we bind the mapchange event. For a more thorough description
	// of the event, look at OnEvent().
	GetPluginAPI()->BindEvent(this, PEF_MAPCHANGE);

	// Add a menu item. This adds a menu item under the plugins list.
	m_hMenuItem = GetPluginAPI()->AddMenuItem("LeakTrace\nLoad Leak File...", this);

	return TPF_OK;
}

// This is called when Tread shutsdown a plugin and is about to
// delete it. Do any cleanup here. This will only be called when
// Tread is exiting after all maps have been closed.
int CLeakTrace::ShutdownPlugin()
{
	return TPF_OK;
}

// This returns the name of the plugin. Tread uses it for invokation procedures and display.
CString CLeakTrace::GetPluginName()
{
	return "LeakTrace";
}

// This routine is called when the plugin is selected from
// the plugins menu. It allows the plugin to do whatever it wants :)
int CLeakTrace::InvokePlugin(CTread3DDoc* pDocument, TPHMENUITEM nMenuID)
{
	//
	// NOTE: The document passed is the current map.
	//
	// For our leaktrace we want to do one of two things:
	//
	// If the current map already has a leak trace object
	// then we should delete it and move on.
	//
	// If the current map does not have a leak trace object
	// then we need to query the user for a leak trace file
	// and load the leak trace object.
	//
	// In both cases we should repaint the views.
	//
	// First, to check to see if we have a leak trace object
	// we can simply use the G_FindObject routine to search the UI list
	// for the object.
	//
	// The G_ routines are general purpose routines that aid in common
	// operations that need to be done on objects, or lists. To see a list
	// of the routines, look in TreadAPI.h.
	//
	CBaseObject* pObject = G_FindObject(pDocument->GetUIObjectList(), EXT_OBJECT_CLASS_LEAKTRACE);
	
	// If G_FindObject found something, then we want to delete the object.
	if(pObject != NULL)
	{
		// We know that this is the leak trace object. Tell Tread to release its bindings on the
		// object, making it safe for us to delete.
		pDocument->RemoveObjectFromMap(pObject);

		// Delete it.
		delete pObject;

		GetPluginAPI()->ChangeMenuItem(m_hMenuItem, "LeakTrace\nLoad Leak File...", this);
		GetPluginAPI()->UpdatePluginMenu();
	}
	else
	{
		// G_FindObject did not find anything. So query the user for a file to open.
		CFileDialog FileDialog(true, "pts");

		FileDialog.m_ofn.lpstrFilter = "Leak Files (*.lin)\0*.lin\0\0";
		FileDialog.m_ofn.lpstrTitle  = "Open Leak File:";
		FileDialog.m_ofn.Flags |= OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;
		if(FileDialog.DoModal() == IDCANCEL)
			return TPF_OK;

		// Load the file.
		CLeakTraceObject* pLT = LoadLeakFile(FileDialog.GetPathName());
		if(pLT == NULL)
			return TPF_OK;

		// Add the object to the map.
		pDocument->AddObjectToMap(pLT);

		GetPluginAPI()->ChangeMenuItem(m_hMenuItem, "LeakTrace\nClose Leak File", this);
		GetPluginAPI()->UpdatePluginMenu();
	}

	// Redraw the windows.
	pDocument->UpdateWindows();
	
	return TPF_OK;
}

//
// A plugin can bind events it wants to recieve using the BindEvent() function from
// the pluginAPI. The only available events as of now are map change events, 
// the selection change event, and the game change event. This will definently grow
// as time goes on.
//
// PEF_MAPCHANGE:
//
// This is invoked whenever the current map is changed as follows:
// 1. A different map gets the focus
// 2. All maps are closed and no map has the focus. In this case pDocument will be NULL.
//
// PEF_SELCHANGE:
//
// This is invoked whenever the selection changes in PointerMode, VertexMode, ClipToolMode, or FaceMode.
// In the future it may be invoked from other function modes.
//
// PEF_GAMECHANGE:
//
// The current game has been changed. You can make a call to the GameAPI to find out the game
// type.
//
void CLeakTrace::OnEvent(int nEventFlags, int nFlags, int nMode, int nSubmode, CTread3DDoc* pDocument, void* pParam, void* pParam2, void* pParam3, void* pParam4)
{
	// If the map changed, and we've got a valid document (i.e. all maps aren't closed).
	if(nEventFlags&PEF_MAPCHANGE && pDocument != NULL)
	{
		// If we have a LeakTrace obj then we want the menu item to say: "Close Leak File"
		// otherwise is should say "Load Leak File..."
		bool bHasLeakTrace = G_FindObject(pDocument->GetUIObjectList(), EXT_OBJECT_CLASS_LEAKTRACE) != NULL;
		if(bHasLeakTrace)
			GetPluginAPI()->ChangeMenuItem(m_hMenuItem, "LeakTrace\nClose Leak File", this);
		else
			GetPluginAPI()->ChangeMenuItem(m_hMenuItem, "LeakTrace\nLoad Leak File...", this);

		// Tell the system to rebuild the menu.
		// During InitializePlugin() you should NEVER call this. Tread will automatically
		// build the plugin menu when it's done with initialization. This should only be
		// called in mid-mapping stuff if you change the menu.
		GetPluginAPI()->UpdatePluginMenu();
	}
}

//
// Draws an object of the requested class.
//
// This allows plugins to extend Treads drawing facilities to draw new objects, or even
// objects that Tread provides.
//
// In order for this routine to be invoked you must call BindFunction() and register
// one of the drawing types: PFF_PREDRAWOBJECT/PFF_POSTDRAWOBJECT.
//
// PFF_PREDRAWOBJECT:
//
// Rendering packages will defer all drawing of all objects to the plugin system, allowing them
// to override drawing of all objects. If the object is not drawn at this stage, then the render package
// will see if it recognizes the object, and draw it. This should almost never be done as it forces
// every object to be negotiated with the pluginAPI for drawing and this can slow things down.
//
// PFF_POSTDRAWOBJECT:
//
// If the render package does not recognize the class type, it will then defer drawing to the
// pluginAPI. This is the preferred method if you are drawing a new object. If you want to override
// an existing one, you must use PFF_PREDRAWOBJECT.
//
bool CLeakTrace::DrawObject(CMapView* pView, CTread3DDoc* pDocument, CBaseWindowRenderer* pWindowRenderer, CDC* pDC, void* pContext, CBaseObject* pObject, int nObjectClass, int nObjectInterfaces, int nDrawFlags, int nFlags, int nPAKUID, int nStage, int nType)
{
	// We only draw LEAKTRACE.
	// Tread should only give us the EXT_OBJECT_CLASS_LEAKTRACE object.
	// You must register the types you want to draw with BindPreDrawObject() or BindPostDrawObject()
	// call.
	if(nObjectClass != EXT_OBJECT_CLASS_LEAKTRACE)
		return false;

	// 
	// Here is a brief description of the parameters. This will not be discussed in passing order,
	// but rather in the easiest order, since some params depend on others.
	//
	// The nStage parameter will be: PCF_PRE or PCF_POST, depending on what stage of drawing the plugin is
	// being invoked in. See the above discussion of PFF_PREDRAWOBJECT and PFF_POSTDRAWOBJECT.
	//
	// Since this plugin only bound PFF_POSTDRAWOBJECT, the nStage parameter will always be PCF_POST. That's
	// why we don't check it here. The nStage param will only become important if a plugin binds both
	// drawing stages.
	//
	// The nPakUID number is the unique identifier of package that is currently performing the rendering.
	// Tread currently provides three internal renderers: GLWINDOWRENDERER_UID, GL3DWINDOWRENDERER_UID
	// and GDIWINDOWRENDERER_UID. You are also free to write your own renderers.
	// 
	// This allows the plugin to decide how to draw the object, and how to interpret the pContext parameter.
	//
	// When the UID is GLWINDOWRENDERER or GL3DWINDOWRENDERER the pContext parameter is a pointer to the currently loaded
	// WOGLFUNCTIONTABLE (i.e. treads internal GL function table). Calls must be made from this to issue GL commands. Tread
	// holds an internal GL table because it may currently be using different GL implementations for each view. Since the GL
	// routines are dynamically loaded each GL instance is contained in this table. Don't link to opengl32.lib and issue
	// drawing commands: you'll mess something up.
	//
	// If it is GDIWINDOWRENDERER then the pContext member is a pointer to a CDC object that should be used to draw
	// with.
	//
	// pDC: is the dc issued by the paint command for the window. it's typically not used for drawing. The WinGDI renderer
	// keeps a seperate DC that draws to an offscreen bitmap for flicker free drawing.
	//
	// pObject: is the pointer to the object to be drawn.
	//
	// nObjectClass: is the class id of the object to be drawn. This is so that GetObjectClass() will not have to be called
	// multiple times. As a rule of thumb: nObjectClass == pObject->GetObjectClass().
	//
	// nObjectInterfaces: is the current I_ interfaces the object has declared. This may or may not effect drawing. Tread
	// automatically handles the I_TARGET interface drawing method (treads target drawing). This will only be important
	// if you create a new interface for objects and want to automate the drawing of that interface. We don't use it here.
	// As always if you are creating new interfaces, please respect the I_RESERVED bit marker.
	// 
	// nDrawFlags: The drawing attributes the object is supposed to be drawn with. For example, if the RF_SELECTED bit
	// is set in the nDrawFlags then the object should be drawn selected (i.e. a selected brush is red). nDrawFlags also
	// contain the m_nObjectRenderFlags of the object, so there is no need to check those seperately.
	//
	// nStyle: In the initialization we bound PDT_2D and PDT_3D. These are drawing styles, and it allows Tread to accelerate
	// the rendering by not querying plugins that have not bound that particular type of drawing. We have bound both, and so
	// we do not check here.

	// Switch based on the render package.
	switch(nPAKUID)
	{
	case GLWINDOWRENDERER_UID:
		DrawLeakTrace_WOGL(pView, pDocument, pDC, (WOGLFUNCTIONTABLE*)pContext, (CLeakTraceObject*)pObject);
		return true;
	case GL3DWINDOWRENDERER_UID:
		DrawLeakTrace_WOGL3D(pView, pDocument, pDC, (WOGLFUNCTIONTABLE*)pContext, (CLeakTraceObject*)pObject);
		return true;
	case GDIWINDOWRENDERER_UID:
		DrawLeakTrace_WGDI(pView, pDocument, pDC, (CDC*)pContext, (CLeakTraceObject*)pObject);
		return true;
	}
	
	// Didn't draw it.
	return false;
}

#define GL_LT_COLOR		230.0F/255.0F, 230.0F/255.0F, 0.0F

// Draws the flat GL leaktrace object.
void CLeakTrace::DrawLeakTrace_WOGL(CMapView* pView, CTread3DDoc* pDocument, CDC* pDC, WOGLFUNCTIONTABLE* pGLFT, CLeakTraceObject* pLT)
{
	if(pLT->m_nNumPts == 0)
		return;

	//
	// The purpose of this isn't to teach you openGL, so I won't get too into what exactly GL is doing here.
	// All that's important is you know that glVertex2i() takes two integer vertex points. The GLWINDOWRENDERER package
	// supplied by Tread sets up an orthographic viewing matrix that is over-looking the part of the map that the window
	// is scrolled too. All that is required by us is to output the untransformed raw map coordinates. It's actually
	// really easy. Things get more complex when drawing the WinGDI view as all view transforms must be done explicitly by
	// us.
	//
	// The one thing I want to discuss here is the use of the VI() macro, which stands simply for "View Index". The problem
	// being addressed here is that we have a 3d coordinate defined by X, Y, Z respectively. For each
	// view Top/Side/Front which index do we map to the X, Y of the window? In Quake, the top view X, Y is a straightforward
	// map to the Points X, Y or: Xv = Xp and Yv = Yp. The Side view maps: Xv = Yp and Yv = Zp. The Front view maps:
	// Xv = Xp and Yv = Zp. The other problem is, for example, on the side view, does the y axis, traveling in screen space,
	// map to positive or negative world space? In GL this is taken care of us by the orthographic view transform. That
	// problem must be addressed in the WinGDI version.
	//
	// The VI macro will take a coordinate type (X, Y, Z) and a CMapView object (this represents the view window) and
	// produce the correct index into a vec3_t (defined as a float[3]). 0 = X, 1 = Y, 2 = Z. This mapping is actually
	// stored in some external arrays given by Tread. The macros should be used ALL the time as a game may come out
	// and Tread may alter the mapping of these transforms for that game, making it inconsistent with your hard-coded
	// versions. The end result of that would be incorrect drawing on your part.
	//
	// The GL renderer already has bound our context and setup all the matrices. If you need to change a matrix, make
	// sure you restore it. On entrance to every drawing routine the GL_MODELVIEW is identity, and GL_PROJECTION is the
	// ortho projection. Rule of thumb: they should be restored to what they where when you were called.
	//
	int i;

	// Set the drawing color.
	pGLFT->glColor3f(GL_LT_COLOR);

	// Begin line-strip.
	pGLFT->glBegin(GL_LINE_STRIP);

	// Use the VI() macro to index the vertices.
	for(i = 0; i < pLT->m_nNumPts; i++)
		pGLFT->glVertex2i(pLT->m_vPts[i][VI(VEC_X, pView)], pLT->m_vPts[i][VI(VEC_Y, pView)]);
	pGLFT->glEnd();

	// Do not call glFinish(). It's not necessary. The GL renderer will do so when it's ready.
}

// Draw's the 3D GL view.
void CLeakTrace::DrawLeakTrace_WOGL3D(CMapView* pView, CTread3DDoc* pDocument, CDC* pDC, WOGLFUNCTIONTABLE* pGLFT, CLeakTraceObject* pLT)
{
	if(pLT->m_nNumPts == 0)
		return;

	//
	// This version of the renderer draws the line in the 3D view. This is more straighforward than 2D drawing. It does
	// not make any use of the tranform macros like the 2D version did.
	//
	// On entrance to this routine the GL context is bound and the GL_PROJECTION matrix contains the current
	// perspective camera transform. The GL_MODELVIEW matrix is identity. Please ensure that these are restored if
	// necessary when writting your own plugins.
	//
	// The states upon entrance: GL_TEXTURE2D and GL_DEPTHTEST are both enabled. If you must disable them, please
	// restore them upon exit of the drawing call.
	//

	int i;
	pGLFT->glDisable(GL_TEXTURE_2D);
	pGLFT->glColor3f(GL_LT_COLOR);
	pGLFT->glBegin(GL_LINE_STRIP);
	for(i = 0; i < pLT->m_nNumPts; i++)
		pGLFT->glVertex3fv(pLT->m_vPts[i]);
	pGLFT->glEnd();
	pGLFT->glEnable(GL_TEXTURE_2D);
}

// Draws the winGDI flat view.
void CLeakTrace::DrawLeakTrace_WGDI(CMapView* pView, CTread3DDoc* pDocument, CDC* pDC, CDC* pDrawDC, CLeakTraceObject* pLT)
{
	if(pLT->m_nNumPts == 0)
		return;

	int i;
	CPen* pOldPen = pDrawDC->SelectObject(m_pLeakPen);
	
	//
	// As mentioned in the WOGL routine, the winGDI drawing is the most complicated of the three, simply because it must
	// explicitly transform points based on the scroll offset and scaling factor of the window. Tread provides very
	// convenient macros to perform these transforms. All the math is directly inlined, so a function is not called per
	// macro. This helps retain some speed. If you wish to see the actual transform math then look in TreadAPI.h for 
	// the MX_TO_WX() or one of its sister macros.
	//
	// M?_TO_W? (where ? can be X or Y) takes a number (i.e. one component of a point) and transforms it via
	// the x or y offset and scaling factor.
	//
	// This is coupled with the VI() macro to extract the correct point.
	//
	// There is a macro, and come to think of it, I should have used it instead of MY_TO_WY. It's MP_TO_WX/MP_TO_WY and it
	// converts a point to the window coordinate. It's a collapsed version of what's done here. This is better
	// for explanation though. With that in mind I could have replaced:
	//
	// MoveTo(MX_TO_WX(pLT->m_vPts[0][VI(VEC_X, pView)], pView), MY_TO_WY(pLT->m_vPts[0][VI(VEC_Y, pView)], pView));
	//
	// with.
	//
	// MoveTo(MP_TO_WX(pLT->m_vPts[0], pView), MP_TO_WY(pLT->m_vPts[0], pView));
	//
	// It's a bit more collapsed, as you can see.
	//

	pDrawDC->MoveTo(MX_TO_WX(pLT->m_vPts[0][VI(VEC_X, pView)], pView), MY_TO_WY(pLT->m_vPts[0][VI(VEC_Y, pView)], pView));

	for(i = 1; i < pLT->m_nNumPts; i++)
		pDrawDC->LineTo(MX_TO_WX(pLT->m_vPts[i][VI(VEC_X, pView)], pView), MY_TO_WY(pLT->m_vPts[i][VI(VEC_Y, pView)], pView));

	pDrawDC->SelectObject(pOldPen);
}
