// LIGHTPREVIEW.H
// Copyright © 2000 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#ifndef LIGHTPREVIEW_H
#define LIGHTPREVIEW_H

/*
//////////////////////////////////////////////////////////////////////
// FORWARD:															//
//																	//
// This plugin generates the light preview that you can use to see	//
// sorta what your lighting will look like in your level. I think	//
// that QER has a light preview mode, as well as Quark. I thought	//
// it would be cool to add Tread to that list. It's a rare feature	//
// and I think it was worth the effort. I decided to make it a		//
// plugin for two reasons: 1) The other plugin examples are ok, but	//
// they still don't demonstrate the real complexity and ability of	//
// a plugin. 2) Lightmapping is kinda intersting and it's amazing	//
// how a simply algorithm can generate some nice looking results.	//
//																	//
// The whole basis by which this sorta thing can work from a plugin	//
// is because of the PFF_OBJECTCREATOR function. Anytime Tread		//
// wants to make a new object it calls a factory function. This		//
// hooks into the plugin system allowing it to manufacture new		//
// objects.															//
//																	//
// Once the light plugin has bound the object creator function, it	//
// then provides routines to draw this new face. Actually, since	//
// brushes contain faces, it really provides routines to draw the	//
// brush, which simply loops and draws the faces. Note that the		//
// only drawing replacment is in the 3D view.						//
//																	//
// I will try to be thorough in my explanations of the lightmap		//
// code, but it's not really easy to articulate some of it without	//
// some paper and a pencil. It also isn't the sole focus of this	//
// example.															//
//																	//
// The brush and face renderer will show you some neat things.		//
// Besides just blending lightmaps, it shows how all the drawing	//
// and selection states result in visual feedback, and how things	//
// work inside. The code for the face renderer is basically copied	//
// straight out of the Tread source code. It's sorta scary and big.	//
//																	//
// This will probably be my last plugin example, so I hope they		//
// have been of assistance to those learning how to use the API.	//
// Please feel free to write me with questions at:					//
// jmriedel@planetquake.com.
//////////////////////////////////////////////////////////////////////
*/

#include "../TreadAPI.h"
#include "GL3DBrushRenderer.h"

struct POINTLIGHT
{
	POINTLIGHT* prev, *next;

	vec3_t vOrigin;
	vec3_t vColor;
	vec_t vBrightness;
};

// Defines a plugin object.
class CLightPreview : public CTreadPlugin
{
private:

	//
	// I use sorta a new paradigm as opposed to menus in LeakTrace. Tread does return handles
	// and we use those to determine which menu was selected.
	//
	TPHMENUITEM m_hRebuild;
	TPHMENUITEM m_hShowHide;
	TPHMENUITEM m_hShadows;
	bool m_bShowLight;
	bool m_bShadows;

	CGL3DBrushRenderer m_BR;

public:

	CLightPreview();
	virtual ~CLightPreview();

	virtual int GetAPIVersion();
	virtual int InitializePlugin();
	virtual int ShutdownPlugin();
	virtual CString GetPluginName();

	virtual int InvokePlugin(CTread3DDoc* pDocument, TPHMENUITEM nMenuID);
	virtual void OnEvent(int nEventFlags, int nFlags, int nMode, int nSubmode, CTread3DDoc* pDocument, void* pParam=NULL, void* pParam2=NULL, void* pParam3=NULL, void* pParam4=NULL);
	virtual CBaseObject* CreateBaseObject(int nObjectClass);
	
	// Build light.
	void BuildLight(CLinkedList<CBaseObject>* pList);
	void BuildLightList(CLinkedList<CBaseObject>* pList, CLinkedList<POINTLIGHT>* pListLight);

	void ClearBrushLightList(CLinkedList<CBaseObject>* pList);
	void AddLightToBrushList(CLinkedList<CBaseObject>* pList, POINTLIGHT* pLight, CLinkedList<CBaseObject>* pWorldList);

	void ClearBrushLight(CQBrush* pBrush);
	void AddLightToBrush(CQBrush* pBrush, POINTLIGHT* pLight, CLinkedList<CBaseObject>* pList);

	void BuildBrushListLightmaps(CLinkedList<CBaseObject>* pList);
	void BuildBrushLightmaps(CQBrush* pBrush);

	// Draws an object.
	virtual bool DrawObject(CMapView* pView, CTread3DDoc* pDocument, CBaseWindowRenderer* pWindowRenderer, CDC* pDC, void* pContext, CBaseObject* pObject, int nObjectClass, int nObjectInterfaces, int nDrawFlags, int nFlags, int nPAKUID, int nStage, int nType);
};

#endif