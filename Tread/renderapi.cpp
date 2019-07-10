// RENDERAPI.CPP
// Copyright © 1999 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#include "stdafx.h"
#include "TreadSys.h"
#include "RenderAPI.h"
#include "ColorTable.h"
#include "GLRenderPackage.h"
#include "MapObject.h"
#include "MapView.h"
#include "HotSpotManager.h"
#include "Tread3d.h"
#include "TExport.h"
#include "PluginAPI.h"
#ifdef _WIN32
#include "GDIRenderPackage.h"
#endif

#define _2D_PAK	"wogl"
#define _3D_PAK	"wogl"

// Defines viewing orientation.
// Basically this array defines the mapping of 3D space to the view
// windows. In RW1 I didn't do this and as a result some of the drawing
// was a little funky, and sometimes the grid wouldn't align correctly.

// NOTE: These vector map the positive screen axis to the corresonding
// axis.
_T_EXPORT float gViewOr[9][2] = { {0.0F,  0.0F}, {1.0F, -1.0F}, 
						{-1.0F,  -1.0F}, {0.0F,  0.0F},
						{1.0F, -1.0F}, {0.0F, 0.0F},
						{0.0F, 0.0F}, {0.0F, 0.0F},
						{1.0F, 1.0F}
						};
// This array tells the index into each vector to map the axis.
// For example, X/Y in view space may map to Y/Z in 3D space.
_T_EXPORT int   gViewIn[5][3] = { {0, 0, 0}, {0, 1, 2}, 
						{1, 2, 0}, {0, 0, 0},
						{0, 2, 1} 
						};

//int	 gUnknown[5] = {0, VMI(VIEW_TOP, VEC_Z), VMI(VIEW_SIDE, VEC_Z), 0, VMI(VIEW_FRONT, VEC_Z)};

_T_EXPORT int  g2DIndex[5][3] = { {0, 0, 0}, {0, 1, 0}, 
						{0, 0, 1}, {0, 0, 0},
						{0, 0, 1} 
						};

CRenderAPI gRenderAPI;

CRenderAPI* GetRenderAPI() { return &gRenderAPI; }

void CBaseWindowRenderer::SetCameraPos(vec3_t vCamPos)
{
	VectorCopy(vCamPos, m_vCamPos);
}

void CBaseWindowRenderer::SetCameraRot(vec3_t vCamRot)
{
	VectorCopy(vCamRot, m_vCamRot);
}

void CBaseWindowRenderer::SetCameraScale(vec3_t vCamScale)
{
	VectorCopy(vCamScale, m_vCamScale);
}

void CRenderAPI::AddDrawFilters(int nFlags)
{
	m_nClassDrawFilter |= nFlags;
}

void CRenderAPI::RemoveDrawFilters(int nFlags)
{
	m_nClassDrawFilter &= ~nFlags;
}

void CRenderAPI::AddGlobalRenderFlags(int nFlags)
{
	m_nGlobalRenderFlags |= nFlags;
}

void CRenderAPI::RemoveGlobalRenderFlags(int nFlags)
{
	m_nGlobalRenderFlags &= ~nFlags;
}

CString CRenderAPI::Get2DPackageName()
{
	if(m_p2DPackage == NULL)
		return "";

	return m_p2DPackage->GetPackageName();
}

CString CRenderAPI::Get3DPackageName()
{
	if(m_p3DPackage == NULL)
		return "";

	return m_p3DPackage->GetPackageName();
}
	
// Creates a render package.
CBaseRenderPackage* CRenderAPI::CreateRenderPackage(CString sName)
{
	sName.MakeLower();

#ifdef _WIN32
	if(CGDIRenderPackage::StaticClaimPackageName(sName))
		return new CGDIRenderPackage();
#endif
	if(CGLRenderPackage::StaticClaimPackageName(sName))
		return new CGLRenderPackage();

	return NULL;
}

// Intializes the RenderAPI.
bool CRenderAPI::Initialize()
{

	// Draw ALL classes.
	m_nClassDrawFilter = 0;
	m_nGlobalRenderFlags = 0;//RF_TEXTURED;//|RF_NOENTITYFILL;//|RF_NOBILINEARFILTER;

	Load2DRenderPackage(GetRegString(S_2D_PAK));
	Load3DRenderPackage(GetRegString(S_3D_PAK));
		
	return true;
}

// Shutsdown packages.
bool CRenderAPI::Shutdown()
{
	ShutdownPackages();

	return true;
}

void CRenderAPI::Get2DRenderPackageList(CString sNames[MAX_RENDER_PACKAGES], CString sDescs[MAX_RENDER_PACKAGES], int& nNumPaks)
{
	nNumPaks = 0;
	GetPluginAPI()->Get2DRenderPackageList(sNames, sDescs, nNumPaks);
	if(nNumPaks >= 256)
		nNumPaks = 256-3;

	sNames[nNumPaks] = "wingdi";
	sDescs[nNumPaks] = "Standard Windows GDI Drawing";
	sNames[nNumPaks+1] = "sft_wogl";
	sDescs[nNumPaks+1] = "2D Software OpenGL Implementation";
	sNames[nNumPaks+2] = "hdw_wogl";
	sDescs[nNumPaks+2] = "2D Hardware Accelerated OpenGL (NOTE: Must have drivers installed)";

	nNumPaks += 3;
}

void CRenderAPI::Get3DRenderPackageList(CString sNames[MAX_RENDER_PACKAGES], CString sDescs[MAX_RENDER_PACKAGES], int& nNumPaks)
{
	nNumPaks = 0;
	GetPluginAPI()->Get3DRenderPackageList(sNames, sDescs, nNumPaks);
	if(nNumPaks >= 256)
		nNumPaks = 256-2;

	sNames[nNumPaks] = "sft_wogl";
	sDescs[nNumPaks] = "3D Software OpenGL Implementation";
	sNames[nNumPaks+1] = "hdw_wogl";
	sDescs[nNumPaks+1] = "3D Hardware Accelerated OpenGL (NOTE: Must have drivers installed)";

	nNumPaks += 2;
}


// Get's a window renderer object.
CBaseWindowRenderer* CRenderAPI::GetWindowRenderer(CMapView* pView, CDC* pDC, int nType)
{
	if(nType == VIEW_3D)
	{
		if(m_p3DPackage == NULL)
		{
			Debug("rdxAPI: no 3D rdx-pak loaded. Ignored create render request.\n\n");
			Warning("rdxAPI: no 3D rdx-pak loaded. Ignored create render request.\n");

			return NULL;
		}

		CBaseWindowRenderer* pWR = m_p3DPackage->GetWindowRenderer(pView, pDC, nType);
		if(pWR == NULL)
		{
			Debug("rdxAPI: 3D rdx-pak create render request failed.\n\n");
			Warning("rdxAPI: 3D rdx-pak create render request failed.\n");
			return NULL;
		}

		return pWR;
	}

	if(m_p2DPackage == NULL)
	{
		Warning("rdxAPI: no 2D rdx-pak loaded. Ignored create render request.\n");
		return NULL;
	}

	// It's flat.
	CBaseWindowRenderer* pWR = m_p2DPackage->GetWindowRenderer(pView, pDC, nType);
	if(pWR == NULL)
	{
		Warning("rdxAPI: 2D rdx-pak create render request failed.\n");
		return NULL;
	}

	return pWR;
}

// Shuts down both the 2D/3D package.
void CRenderAPI::ShutdownPackages()
{
	Shutdown2DPackage();
	Shutdown3DPackage();
}

// Shuts down the 3D render package.
void CRenderAPI::Shutdown3DPackage()
{
	if(m_p3DPackage == NULL)
		return;

	CString sName = m_p3DPackage->GetPackageName();

	m_p3DPackage->UnloadPackage();
	delete m_p3DPackage;

	m_p3DPackage = NULL;

	Debug("rdxAPI: '" + sName + "' 3D rdx-pak unloaded.\n");
	return;
}

// Loads a 3D render package.
bool CRenderAPI::Load3DRenderPackage(CString sName)
{
	Shutdown3DPackage();
	
	CBaseRenderPackage* pPackage = CreateRenderPackage(sName);
	if(pPackage == NULL)
	{
		PrintSplashLine("rdxAPI: " + sName + " not found.");
		Debug("rdxAPI: " + sName + " not found.");
		Error("Render package " + sName + " not loaded.\n");
		return false;
	}

	if(!(pPackage->GetRenderFlags() & RF_3D_API))
	{
		PrintSplashLine("rdxAPI: not 3D rdx-pak.");
		Debug("rdxAPI: not 3D rdx-pak.\n");	
		Error("Render package " + sName + " not loaded.");
		delete pPackage;
		return false;
	}

	// Load it.
	if(!pPackage->LoadPackage(sName))
	{
		PrintSplashLine("rdxAPI: load failed.");
		Debug("rdxAPI: load failed.\n");
		Error("Render package " + sName + " not loaded.\n");
		delete pPackage;
		return false;
	}

	m_p3DPackage = pPackage;
	Debug("rdxAPI: '" + sName + "' 3D rdx-pak loaded.\n");
	PrintSplashLine("rdxAPI: '" + sName + "' 3D rdx-pak loaded.");

	return true;
}

// Shuts down the 2D render package.
void CRenderAPI::Shutdown2DPackage()
{
	if(m_p2DPackage == NULL)
		return;

	CString sName = m_p2DPackage->GetPackageName();

	m_p2DPackage->UnloadPackage();
	delete m_p2DPackage;

	m_p2DPackage = NULL;

	Debug("rdxAPI: '" + sName + "' 2D rdx-pak unloaded.\n");
	return;
}

// Loads a 2D render package.
bool CRenderAPI::Load2DRenderPackage(CString sName)
{
	Shutdown2DPackage();
	
	CBaseRenderPackage* pPackage = CreateRenderPackage(sName);
	if(pPackage == NULL)
	{
		PrintSplashLine("rdxAPI: " + sName + " not found.");
		Debug("rdxAPI: " + sName + " not found.\n");
		Error("Render package " + sName + " not loaded.");
		return false;
	}

	if(!(pPackage->GetRenderFlags() & RF_2D_API))
	{
		PrintSplashLine("rdxAPI: not 2D rdx-pak.");	
		Debug("rdxAPI: not 2D rdx-pak.\n");	
		Error("Render package " + sName + " not loaded.");
		delete pPackage;
		return false;
	}

	// Load it.
	if(!pPackage->LoadPackage(sName))
	{
		PrintSplashLine("rdxAPI: load failed.");
		Debug("rdxAPI: load failed.\n");
		Error("Render package " + sName + " not loaded.");
		delete pPackage;
		return false;
	}

	m_p2DPackage = pPackage;
	Debug("rdxAPI: '" + sName + "' 2D rdx-pak loaded.\n");
	PrintSplashLine("rdxAPI: '" + sName + "' 2D rdx-pak loaded.");
	return true;
}

// Renders an object list.
void CRenderAPI::DrawObjectList(CMapView* pView, CTread3DDoc* pDocument, CDC* pDC, CBaseWindowRenderer* pWR, CLinkedList<CBaseObject>* pObjectList, int nDrawFlags, int nType)
{
	int nInterfaces, nClass;
	CBaseObject* pObject;
	CObjectGroup* pGroup;

	// LittleX.
	bool bDrawX = (nDrawFlags&RF_DRAWLITTLEX);
	nDrawFlags &= ~RF_DRAWLITTLEX;

	if(pWR == NULL)
		return;

	pObjectList->First();
	for(pObject = pObjectList->GetCurItem(); pObject; pObject = pObjectList->GetNextItem())
	{
		// Skip if filtered.
		if(pObject->m_nViewTypeFilters&pView->m_nView)
			continue;

		// Is this a group?
		nClass = pObject->GetObjectClass();
		// Draw this class?
		if((nClass & (m_nClassDrawFilter|pDocument->m_nClassDrawFilter|pView->m_nClassDrawFilter)) != 0)
			continue;	// Nope, it's filtered.
		
		// Get the interface.
		nInterfaces = pObject->GetObjectInterfaces();
		
		// Is it a visgroup?
		if(nInterfaces&I_VISGROUPREF)
		{
			CVisgroupRefObject* pRef = (CVisgroupRefObject*)pObject->GetInterface(I_VISGROUPREF);
			if(pRef->IsVisible() == false)
				continue;
		}

		// Draw the interfaces.
		if(!(pObject->m_nObjectRenderFlags & RF_NODRAW_INTERFACES))
		{
			// Draw the group.
			if(nInterfaces&(I_GROUP|I_FGROUP))
			{
				pGroup = (CObjectGroup*)pObject->GetInterface(I_GROUP);
				DrawObjectList(pView, pDocument, pDC, pWR, (CLinkedList<CBaseObject>*)pGroup->GetObjectList(), nDrawFlags|pObject->m_nInterfaceRenderFlags, nType);
			}
			if(nInterfaces&I_TARGET && pDocument->GetDrawTargets())
				pWR->DrawI_TARGET(pView, pDocument, pDC, pObject, nDrawFlags, nType);
		
		}
		
		// Draw the object.
		if((nDrawFlags&RF_PICKMODE)==0 && (nDrawFlags&RF_NOTRANSPARENCY)== 0 && (pObject->m_nObjectRenderFlags&RF_TRANSPARENT) && (nType&RT_TRANSPARENT)==0)
		{
			if(nDrawFlags&RF_SELECTED)
			{
				if(AddSelectedTransparent(pObject))
					continue;
			}
			else
				if(AddTransparent(pObject))
					continue;
		}

		if(!(pObject->m_nObjectRenderFlags & RF_NODRAW))
		{
			// Draw.
			pWR->DrawObject(pView, pDocument, pDC, pObject, nClass, nInterfaces, nDrawFlags|pObject->m_nObjectRenderFlags, nType);
		}

		if(bDrawX)
		{
			CBaseTransformableObject* pT = (CBaseTransformableObject*)pObject->GetInterface(I_TRANSFORMABLE);
			if(pT != NULL)
			{
				vec2_t vMapPos;

				if(VMO(VEC_X, pView) > 0)
					vMapPos[0] = pT->m_vMins[VI(VEC_X, pView)];
				else
					vMapPos[0] = pT->m_vMaxs[VI(VEC_X, pView)];

				if(VMO(VEC_Y, pView) > 0)
					vMapPos[1] = pT->m_vMins[VI(VEC_Y, pView)];
				else
					vMapPos[1] = pT->m_vMaxs[VI(VEC_Y, pView)];

				pWR->DrawLittleX(pView, pDocument, pDC, vMapPos, CL_LINE);
				bDrawX = false;
			}
		}
	}
}

bool CRenderAPI::AddTransparent(CBaseObject* pObject)
{
	if(m_nNumTransparents == MAX_TRANSPARENTS)
		return false;

	m_pTransparents[m_nNumTransparents++] = pObject;
	return true;
}

bool CRenderAPI::AddSelectedTransparent(CBaseObject* pObject)
{
	if(m_nNumSelectedTransparents == MAX_TRANSPARENTS)
		return false;

	m_pSelectedTransparents[m_nNumSelectedTransparents++] = pObject;
	return true;
}

void CRenderAPI::ResetTransparents()
{
	m_nNumTransparents = 0;
}

void CRenderAPI::ResetSelectedTransparents()
{
	m_nNumSelectedTransparents =0;
}

void CRenderAPI::DrawTransparents(CBaseObject** m_pObjArray, int nNum, CMapView* pView, CTread3DDoc* pDocument, CDC* pDC, CBaseWindowRenderer* pWR, int nDrawFlags, int nType)
{
	int nInterfaces, nClass;
	CBaseObject* pObject;
	CObjectGroup* pGroup;

	if(pWR == NULL)
		return;

	pWR->EnableTransparency();

	for(int i = 0; i < nNum; i++)
	{
		pObject = m_pObjArray[i];
		// Skip if filtered.
		if(pObject->m_nViewTypeFilters&pView->m_nView)
			continue;

		// Is this a group?
		nClass = pObject->GetObjectClass();
		// Draw this class?
		if((nClass & (m_nClassDrawFilter|pDocument->m_nClassDrawFilter|pView->m_nClassDrawFilter)) != 0)
			continue;	// Nope, it's filtered.
		
		// Get the interface.
		nInterfaces = pObject->GetObjectInterfaces();
		
		// Draw the interfaces.
		if(!(pObject->m_nObjectRenderFlags & RF_NODRAW_INTERFACES))
		{
			// Draw the group.
			if(nInterfaces&(I_GROUP|I_FGROUP))
			{
				pGroup = (CObjectGroup*)pObject->GetInterface(I_GROUP);
				DrawObjectList(pView, pDocument, pDC, pWR, (CLinkedList<CBaseObject>*)pGroup->GetObjectList(), nDrawFlags|pObject->m_nInterfaceRenderFlags|RF_TRANSPARENT, RT_TRANSPARENT);
			}
		}
		
		// Draw the class.
		if(!(pObject->m_nObjectRenderFlags & RF_NODRAW))
		{
			// Draw.
			pWR->DrawObject(pView, pDocument, pDC, pObject, nClass, nInterfaces, nDrawFlags|pObject->m_nObjectRenderFlags|RF_TRANSPARENT, nType);
		}
	}

	pWR->EnableTransparency(false);
}

void CRenderAPI::RenderMapSelection(CMapView* pView, CTread3DDoc* pDocument, CDC* pDC, CBaseWindowRenderer* pWR, int nRenderFlags, int nType)
{
	int nDrawFlags = m_nGlobalRenderFlags|nRenderFlags|pDocument->m_nGlobalRenderFlags|pView->m_nGlobalRenderFlags;

	if(pWR == NULL)
		return;
	if(pDocument->GetSelectedObjectList() == NULL)
		return;

	// Lock the device.
	if(!pWR->LockDevice(pView, pDocument, pDC, nDrawFlags))
	{
		Warning("rdxAPI: couldn't lock render device.\n");
		return;
	}

	// Kill if not drawing anything else.
	if(pView->m_nView == VIEW_3D)
	{
		if(nDrawFlags&RF_NO3DVIEW)
		{
			// Flush the renderer.
			pWR->Flush(pView, pDocument, pDC);
			pWR->UnlockDevice(pView, pDocument, pDC, nDrawFlags);
			return;
		}
	}

	// Draw the selection box.
	if((nDrawFlags&RF_SELBOXONTOP)==0 && ((nDrawFlags&RF_FORCESELBOX) == RF_FORCESELBOX || (pDocument->IsInterfaceSelected(I_ALL&~I_UI) && (nDrawFlags&RF_NOSELBOX)==0)))
		pWR->DrawBBox(pView, pDocument, pDC, pDocument->m_vSelMins, pDocument->m_vSelMaxs, CL_HOTSPOT_BOX);

	if(nDrawFlags&RF_NODRAWSEL)
	{
		DrawObjectList(pView, pDocument, pDC, pWR, pDocument->GetSelectedObjectList(), nDrawFlags, nType);
		if((nDrawFlags&RF_NOTRANSPARENCY)==0 && (nDrawFlags&RF_PICKMODE)==0)
			DrawTransparents(m_pTransparents, m_nNumTransparents, pView, pDocument, pDC, pWR, nDrawFlags, 0);
	}
	else
	{
		// Draw the selected list.
		ResetSelectedTransparents();

		DrawObjectList(pView, pDocument, pDC, pWR, pDocument->GetSelectedObjectList(), nDrawFlags|RF_SELECTED|RF_DRAWLITTLEX, nType);

		// Draw normal transparency list.
		if((nDrawFlags&RF_NOTRANSPARENCY)==0 && (nDrawFlags&RF_PICKMODE)==0)
		{
			DrawTransparents(m_pTransparents, m_nNumTransparents, pView, pDocument, pDC, pWR, nDrawFlags, 0);
			DrawTransparents(m_pSelectedTransparents, m_nNumSelectedTransparents, pView, pDocument, pDC, pWR, nDrawFlags|RF_SELECTED, 0);
		}

	}

	// Draw the selection box.
	if((nDrawFlags&RF_SELBOXONTOP)==RF_SELBOXONTOP && ((nDrawFlags&RF_FORCESELBOX) == RF_FORCESELBOX || (pDocument->IsInterfaceSelected(I_ALL&~I_UI) && (nDrawFlags&RF_NOSELBOX)==0)))
		pWR->DrawBBox(pView, pDocument, pDC, pDocument->m_vSelMins, pDocument->m_vSelMaxs, CL_HOTSPOT_BOX);

	// Draw the hotspot list.
	if((nDrawFlags&RF_NOHOTSPOTS)==0)
	{
		CHotSpot* pSpot;
		CHotSpotManager* pHSM = pView->GetHotSpotManager();
		pHSM->m_pHotSpotList->First();
		for(pSpot = pHSM->m_pHotSpotList->GetCurItem(); pSpot != NULL; pSpot = pHSM->m_pHotSpotList->GetNextItem())
			pWR->DrawHotSpot(pView, pDocument, pDC, pSpot, nDrawFlags|pSpot->m_nObjectRenderFlags, 0);
	}

	// Draw size?
	if((nDrawFlags&RF_NOSELBOXSIZE)==0 && ((nDrawFlags&RF_FORCESELBOX)==RF_FORCESELBOX || ((nDrawFlags&RF_NOSELBOX)==0 && pDocument->IsInterfaceSelected(I_ALL&~I_UI))))
		pWR->DrawBBoxSize(pView, pDocument, pDC, pDocument->m_vSelMins, pDocument->m_vSelMaxs, pDocument->m_vSelCenter, 0);

	// Flush the renderer.
	pWR->Flush(pView, pDocument, pDC);
	pWR->UnlockDevice(pView, pDocument, pDC, nDrawFlags);
}

// Renders a map.
void CRenderAPI::RenderMap(CMapView* pView, CTread3DDoc* pDocument, CDC* pDC, CBaseWindowRenderer* pWR, int nRenderFlags, int nType)
{
	int nDrawFlags = m_nGlobalRenderFlags|nRenderFlags|pDocument->m_nGlobalRenderFlags|pView->m_nGlobalRenderFlags;

	if(pWR == NULL)
		return;
	if(pDocument->GetObjectList() == NULL || 
		pDocument->GetSelectedObjectList() == NULL)
		return;

	// Lock the device.
	if(!pWR->LockDevice(pView, pDocument, pDC, nDrawFlags))
	{
		Warning("rdxAPI: couldn't lock render device.\n");
		return;
	}

	// Clear the window.
	pWR->ClearWindow(pView, pDocument, pDC, CL_FLAT_BACKGROUND, nType);

	// Kill if not drawing anything else.
	if(pView->m_nView == VIEW_3D)
	{
		if(nDrawFlags&RF_NO3DVIEW)
		{
			// Flush the renderer.
			pWR->Flush(pView, pDocument, pDC);
			pWR->UnlockDevice(pView, pDocument, pDC, nDrawFlags);
			return;
		}
	}

	// Draw the grid.
	if(nType != VIEW_3D)
		pWR->DrawGrid(pView, pDocument, pDC, nType);

	ResetTransparents();
	// Draw normal objects first.
	DrawObjectList(pView, pDocument, pDC, pWR, pDocument->GetObjectList(), nDrawFlags, nType);
	
	// Draw the selection box.
	if((nDrawFlags&RF_SELBOXONTOP)==0 && ((nDrawFlags&RF_FORCESELBOX) == RF_FORCESELBOX || (pDocument->IsInterfaceSelected(I_ALL&~I_UI) && (nDrawFlags&RF_NOSELBOX)==0)))
		pWR->DrawBBox(pView, pDocument, pDC, pDocument->m_vSelMins, pDocument->m_vSelMaxs, CL_HOTSPOT_BOX);

	if(nDrawFlags&RF_NODRAWSEL)
	{
		DrawObjectList(pView, pDocument, pDC, pWR, pDocument->GetSelectedObjectList(), nDrawFlags, nType);
		if((nDrawFlags&RF_NOTRANSPARENCY)==0 && (nDrawFlags&RF_PICKMODE)==0)
			DrawTransparents(m_pTransparents, m_nNumTransparents, pView, pDocument, pDC, pWR, nDrawFlags, 0);
	}
	else
	{
		// Draw the selected list.
		ResetSelectedTransparents();

		DrawObjectList(pView, pDocument, pDC, pWR, pDocument->GetSelectedObjectList(), nDrawFlags|RF_SELECTED|RF_DRAWLITTLEX, nType);

		// Draw normal transparency list.
		if((nDrawFlags&RF_NOTRANSPARENCY)==0 && (nDrawFlags&RF_PICKMODE)==0)
		{
			DrawTransparents(m_pTransparents, m_nNumTransparents, pView, pDocument, pDC, pWR, nDrawFlags, 0);
			DrawTransparents(m_pSelectedTransparents, m_nNumSelectedTransparents, pView, pDocument, pDC, pWR, nDrawFlags|RF_SELECTED, 0);
		}

	}

	// Draw UI list.
	DrawObjectList(pView, pDocument, pDC, pWR, pDocument->GetUIObjectList(), nDrawFlags, nType);

	// Draw the selection box.
	if((nDrawFlags&RF_SELBOXONTOP)==RF_SELBOXONTOP && ((nDrawFlags&RF_FORCESELBOX) == RF_FORCESELBOX || (pDocument->IsInterfaceSelected(I_ALL&~I_UI) && (nDrawFlags&RF_NOSELBOX)==0)))
		pWR->DrawBBox(pView, pDocument, pDC, pDocument->m_vSelMins, pDocument->m_vSelMaxs, CL_HOTSPOT_BOX);

	// Draw the hotspot list.
	if((nDrawFlags&RF_NOHOTSPOTS)==0)
	{
		CHotSpot* pSpot;
		CHotSpotManager* pHSM = pView->GetHotSpotManager();
		pHSM->m_pHotSpotList->First();
		for(pSpot = pHSM->m_pHotSpotList->GetCurItem(); pSpot != NULL; pSpot = pHSM->m_pHotSpotList->GetNextItem())
			pWR->DrawHotSpot(pView, pDocument, pDC, pSpot, nDrawFlags|pSpot->m_nObjectRenderFlags, 0);
	}

	// Draw size?
	if((nDrawFlags&RF_NOSELBOXSIZE)==0 && ((nDrawFlags&RF_FORCESELBOX)==RF_FORCESELBOX || ((nDrawFlags&RF_NOSELBOX)==0 && pDocument->IsInterfaceSelected(I_ALL&~I_UI))))
		pWR->DrawBBoxSize(pView, pDocument, pDC, pDocument->m_vSelMins, pDocument->m_vSelMaxs, pDocument->m_vSelCenter, 0);

	// Flush the renderer.
	pWR->Flush(pView, pDocument, pDC);
	pWR->UnlockDevice(pView, pDocument, pDC, nDrawFlags);
}