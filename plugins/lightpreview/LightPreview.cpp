// LIGHTPREVIEW.CPP
// Copyright © 2000 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#include "stdafx.h"
#include "../TreadAPI.h"
#include "LightPreview.h"
#include "LitFace.h"

T3DPLUGINAPI CTreadPlugin* T3D_CreatePluginInstance()
{ 
	return new CLightPreview();
}

CLightPreview::CLightPreview()
{
}

CLightPreview::~CLightPreview()
{
}

int CLightPreview::GetAPIVersion()
{ return TP_API_VERSION; }

int CLightPreview::InitializePlugin()
{
	//
	// We derrive our lightface from the CFace structure,
	// and we need to replace all calls to create a face
	// object to make a lightface. We must bind the PFF_OBJECTCREATOR.
	// We are essentially replacing a class in Tread. There are places that
	// internally reference the OBJECT_CLASS_FACE type, so we must intercept
	// the creation and drawing of this type of object.
	//
	GetPluginAPI()->BindFunction(this, PFF_OBJECTCREATOR|PFF_PREDRAWOBJECT);

	//
	// Brush objects to not generate draw requests on faces, which means
	// the only place we can intercept face drawing is at the level of the object
	// containg the faces. Currently brushes are the only objects to contain faces
	// so we are only intercepting them at the moment.
	//
	// The other thing of interest is that we are only binding the 3D stage of drawing.
	// We will never be called to draw on any of the flat views.
	//
	GetPluginAPI()->BindPreDrawClasses(this, OBJECT_CLASS_BRUSH, PDT_3D);

	//
	// Add our menu items.
	//
	m_hShowHide = GetPluginAPI()->AddMenuItem("ZombieLight\nShow Light", this);
	m_hShadows  = GetPluginAPI()->AddMenuItem("ZombieLight\nShadows", this);
	m_hRebuild  = GetPluginAPI()->AddMenuItem("ZombieLight\nBuild Light Preview", this);

	m_bShadows = false;
	m_bShowLight = false;

	return TPF_OK;
}

int CLightPreview::ShutdownPlugin()
{
	return TPF_OK;
}

CString CLightPreview::GetPluginName()
{
	//
	// I sorta ripped this name off from a friend of mine, Aaron Sittig. He works at Napster (awesome company).
	// I interviewed there and was offered a position, but I declined it in leu of my game job. He made a hotline client
	// called zombie, so I decided to call this light plugin "ZombieLight". It's kinda a gay story, but bear with me,
	// I'm not wearing any pants right now.
	//
	return "ZombieLight";
}

int CLightPreview::InvokePlugin(CTread3DDoc* pDocument, TPHMENUITEM nMenuID)
{
	// NOTE: We can't call AfxGetApp() from plugins.
	SetCursor(LoadCursor(NULL, IDC_WAIT));

	if(nMenuID == m_hShadows)
	{
		m_bShadows = !m_bShadows;
		GetPluginAPI()->SetMenuItemState(m_hShadows, m_bShadows, true, this);
		GetPluginAPI()->UpdatePluginMenu();
		return TPF_OK;
	}

	// 
	// Build light preview.
	//
	if(nMenuID == m_hShowHide)
	{
		m_bShowLight = !m_bShowLight;
		pDocument->UpdateWindows(VIEW_3D);
	}
	if(nMenuID == m_hRebuild)
	{
		pDocument->ClearSelection();
		BuildLight(pDocument->GetObjectList());
		pDocument->UpdateWindows(VIEW_3D);
	}

	GetPluginAPI()->SetMenuItemState(m_hShowHide, m_bShowLight, true, this);
	GetPluginAPI()->UpdatePluginMenu();
		
	SetCursor(LoadCursor(NULL, IDC_ARROW));

	return TPF_OK;
}

void CLightPreview::OnEvent(int nEventFlags, int nFlags, int nMode, int nSubmode, CTread3DDoc* pDocument, void* pParam, void* pParam2, void* pParam3, void* pParam4)
{
}

//
// Creates a list of point lights. This can be modified to parse any entity as a light
//
void CLightPreview::BuildLightList(CLinkedList<CBaseObject>* pList, CLinkedList<POINTLIGHT>* pLightList)
{
	POINTLIGHT* pLight;
	CEntity* pEntity;
	CBaseObject* pObj;

	//
	// make a point light list.
	//

	pList->First();
	for(pObj = pList->GetCurItem(); pObj != NULL; pObj = pList->GetNextItem())
	{
		if(pObj->GetObjectClass() == OBJECT_CLASS_GROUP)
		{
			BuildLightList(((CObjectGroup*)pObj)->GetObjectList(), pLightList);
			continue;
		}

		if(pObj->GetObjectClass() != OBJECT_CLASS_ENTITY)
			continue;
		pEntity = (CEntity*)pObj;

		if(pEntity->IsVisible() == false)
			continue;

		// Is it a light?
		if(pEntity->GetClassName() == "light")
		{
			CString sColor;
			CString sBrightness;

			pLight = new POINTLIGHT();
			VectorCopy(pEntity->m_vOrigin, pLight->vOrigin);

			sColor = pEntity->GetKeyValue("_color");
			sBrightness = pEntity->GetKeyValue("light");

			// Half life color type.
			if(sBrightness == "")
			{
				CString sKey = pEntity->GetKeyValue("_light");
				if(sKey == "")
				{
					Warning("BuildLight: 'light' entity missing any type of recongnizable light information\n");
					delete pLight;
					continue;
				}

				sscanf(sKey, "%f %f %f %f", &pLight->vColor[0], &pLight->vColor[1], &pLight->vColor[2], &pLight->vBrightness);
				VectorScale(pLight->vColor, 1.0f/255.0f, pLight->vColor);

			}
			else
			{
				if(sColor == "")
				{
					Warning("BuildLight: 'light' entity missing _color or light key!\n");
					delete pLight;
					continue;
				}

				sscanf(sColor, "%f %f %f", &pLight->vColor[0], &pLight->vColor[1], &pLight->vColor[2]);
				sscanf(sBrightness, "%f", &pLight->vBrightness);
			}

			// Add.
			pLightList->AddItem(pLight);
		}
	}
}

// Build light.
void CLightPreview::BuildLight(CLinkedList<CBaseObject>* pList)
{
	POINTLIGHT* pLight;
	CLinkedList<POINTLIGHT> LightList;

	Debug("... building light preview\n");
	Console("... building light preview\n");

	BuildLightList(pList, &LightList);

	int nNumLights = LightList.GetCount();
	Debug("... found %d point light(s)\n", nNumLights);
	Console("... found %d point light(s)\n", nNumLights);

	if(nNumLights == 0)
	{
		Debug("... aborting, no light to build\n");
		Console("... aborting, no light to build\n");
		m_bShowLight = false;
		return;
	}
	
	if(m_bShadows)
	{
		Debug("... shadows are enabled\n");
		Console("... shadows are enabled\n");
	}
	else
	{
		Debug("... shadows are disabled\n");
		Console("... shadows are disabled\n");
	}

	Debug("... applying brush light\n");
	Console("... applying brush light\n");

	g_nNumLightmapsBuilt = 0;

	//
	// Takes the object list and clears out the lighting on every brush.
	//
	ClearBrushLightList(pList);
	
	// Add light.
	g_fSceneMax=0.0f;

	//
	// The progress control is that funny little blue bar in the left corner of the
	// editor status bar that you see moving when stuff loads. We use it here as a progress
	// indicator for the status of our lighting computations.
	//
	PrgSetRange(0, nNumLights);
	PrgSetPos(0);
	PrgSetStep(1);

	LightList.First();
	for(pLight = LightList.GetCurItem(); pLight != NULL; pLight = LightList.GetNextItem())
	{
		//
		// This takes the given light and computes the direct surface lighting on each
		// brush in the list. This routine also follows group chains.
		//
		AddLightToBrushList(pList, pLight, pList);
		PrgStepIt();
	}

	BuildBrushListLightmaps(pList);

	// Done.
	m_bShowLight = true;

	Debug("... built %d lightmap(s)\n", g_nNumLightmapsBuilt);
	Console("... built %d lightmap(s)\n", g_nNumLightmapsBuilt);

	Debug("done\n");
	Console("done\n");

	PrgSetPos(0);
}

void CLightPreview::BuildBrushListLightmaps(CLinkedList<CBaseObject>* pList)
{
	CQBrush* pBrush;
	CBaseObject* pObj;

	pList->First();
	for(pObj = pList->GetCurItem(); pObj != NULL; pObj = pList->GetNextItem())
	{
		if(pObj->GetObjectClass() == OBJECT_CLASS_GROUP || pObj->GetObjectClass() == OBJECT_CLASS_LENTITY)
		{
			if(((CObjectGroup*)pObj)->IsVisible() == false)
				continue;
			BuildBrushListLightmaps(((CObjectGroup*)pObj)->GetObjectList());
			continue;
		}
		
		if(pObj->GetObjectClass() != OBJECT_CLASS_BRUSH)
			continue;
		pBrush = (CQBrush*)pObj;
		if(pBrush->IsVisible() == false)
			continue;
		BuildBrushLightmaps(pBrush);
	}
}

void CLightPreview::BuildBrushLightmaps(CQBrush* pBrush)
{
	CFace* pFace;
	pBrush->m_pFaceList->First();
	for(pFace = pBrush->m_pFaceList->GetCurItem(); pFace != NULL; pFace = pBrush->m_pFaceList->GetNextItem())
		((CLitFace*)pFace)->BuildLightmaps();
}

void CLightPreview::ClearBrushLightList(CLinkedList<CBaseObject>* pList)
{
	CQBrush* pBrush;
	CBaseObject* pObj;

	pList->First();
	for(pObj = pList->GetCurItem(); pObj != NULL; pObj = pList->GetNextItem())
	{
		if(pObj->GetObjectClass() == OBJECT_CLASS_GROUP || pObj->GetObjectClass() == OBJECT_CLASS_LENTITY)
		{
			if(((CObjectGroup*)pObj)->IsVisible() == false)
				continue;
			ClearBrushLightList(((CObjectGroup*)pObj)->GetObjectList());
			continue;
		}

		if(pObj->GetObjectClass() != OBJECT_CLASS_BRUSH)
			continue;
		pBrush = (CQBrush*)pObj;
		if(pBrush->IsVisible() == false)
			continue;
		ClearBrushLight(pBrush);
	}
}

void CLightPreview::AddLightToBrushList(CLinkedList<CBaseObject>* pList, POINTLIGHT* pLight, CLinkedList<CBaseObject>* pWorldList)
{
	CQBrush* pBrush;
	CBaseObject* pObj;

	pList->First();
	for(pObj = pList->GetCurItem(); pObj != NULL; pObj = pList->GetNextItem())
	{
		if(pObj->GetObjectClass() == OBJECT_CLASS_GROUP || pObj->GetObjectClass() == OBJECT_CLASS_LENTITY)
		{
			if(((CObjectGroup*)pObj)->IsVisible() == false)
				continue;
			AddLightToBrushList(((CObjectGroup*)pObj)->GetObjectList(), pLight, pWorldList);

			//
			// Restore position. We need to do this because the intermediate call may have walked the world
			// list leaving it at an unknown location.
			//
			pList->SetPosition(pObj);
			continue;
		}

		if(pObj->GetObjectClass() != OBJECT_CLASS_BRUSH)
			continue;
		pBrush = (CQBrush*)pObj;
		if(pBrush->IsVisible() == false)
			continue;

		//
		// This computes the surface light contribution of the specified light on the
		// faces of the specified brush. The last param is an optional occlusion list.
		// Passing null means that no shadows will be computed.
		//
		AddLightToBrush(pBrush, pLight, (m_bShadows) ? pWorldList : NULL);

		//
		// Restore position. We need to do this because the intermediate call may have walked the world
		// list leaving it at an unknown location.
		//
		pList->SetPosition(pObj);
	}
}

void CLightPreview::ClearBrushLight(CQBrush* pBrush)
{
	CFace* pFace;
	pBrush->m_pFaceList->First();
	for(pFace = pBrush->m_pFaceList->GetCurItem(); pFace != NULL; pFace = pBrush->m_pFaceList->GetNextItem())
		((CLitFace*)pFace)->ResetLight();
}

//
// Computes the surface light contribution of the light on each face of the specified brush.
//
void CLightPreview::AddLightToBrush(CQBrush* pBrush, POINTLIGHT* pLight, CLinkedList<CBaseObject>* pList)
{
	CFace* pFace;
	pBrush->m_pFaceList->First();
	for(pFace = pBrush->m_pFaceList->GetCurItem(); pFace != NULL; pFace = pBrush->m_pFaceList->GetNextItem())
	{
		((CLitFace*)pFace)->AddLight(pLight->vOrigin, pLight->vColor, pLight->vBrightness, pList);
		pBrush->m_pFaceList->SetPosition(pFace);
	}
}

// We replace all CFace objects with the CLitFace.
CBaseObject* CLightPreview::CreateBaseObject(int nObjectClass)
{
	if(nObjectClass == OBJECT_CLASS_FACE)
	{
		return new CLitFace();
	}

	return NULL;
}

// Draws an object.
bool CLightPreview::DrawObject(CMapView* pView, CTread3DDoc* pDocument, CBaseWindowRenderer* pWindowRenderer, CDC* pDC, void* pContext, CBaseObject* pObject, int nObjectClass, int nObjectInterfaces, int nDrawFlags, int nFlags, int nPAKUID, int nStage, int nType)
{
	//
	// We have only bound the 3D stage of drawing, so we don't have to check that.
	//

	//
	// We only draw brushes.
	//
	if(nObjectClass != OBJECT_CLASS_BRUSH)
		return false;

	// We only support the built-in OGL 3D rendering package.
	if(nPAKUID != GL3DWINDOWRENDERER_UID)
		return false;

	//
	// Setup the brush renderers params.
	//
	CQBrush* pBrush = (CQBrush*)pObject;
	m_BR.m_pWR = (CGL3DWindowRenderer*)pWindowRenderer;
	m_BR.m_pGLFT = (WOGLFUNCTIONTABLE*)pContext;

	//
	// Draw the requested brush.
	//
	m_BR.RenderBrush(pBrush, nDrawFlags, m_bShowLight);

	return true;
}