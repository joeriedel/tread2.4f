// Tread3DDoc.cpp : implementation of the CTread3DDoc class
//

#include "stdafx.h"
#include "Tread3D.h"

#ifdef _WIN32
#include "GameDialog.h"
#include "viewControl.h"
#include "ModelDialog.h"
#include "MapDialog.h"
#include "GameSetupDialog.h"

#endif

#include "GameAPI.h"
#include "TreadSys.h"

#include "Tread3DDoc.h"
#include "MapView.h"

#include "Face.h"
#include "Brush.h"
#include "EventManager.h"
#include "MapObject.h"
#include "TreadMapAPI.h"
#include "MathLib.h"
#include "Models.h"
#include "Geometry.h"
#include "RenderAPI.h"
#include "Camera.h"
#include "Anchor.h"

#include "CameraModeResponder.h"
#include "VertexModeResponder.h"
#include "EntityModeResponder.h"
#include "BrushModeResponder.h"
#include "PointerModeResponder.h"
#include "ZoomModeResponder.h"
#include "PaintModeResponder.h"
#include "ClipToolModeResponder.h"
#include "FaceModeResponder.h"
#include "ColorCycleController.h"

#include "szstring.h"
#include "PluginAPI.h"
#include "MainFrm.h"

#ifdef _WIN32
#include <mmsystem.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTread3DDoc

IMPLEMENT_DYNCREATE(CTread3DDoc, CDocument)

BEGIN_MESSAGE_MAP(CTread3DDoc, CDocument)
	//{{AFX_MSG_MAP(CTread3DDoc)
	ON_COMMAND(ID_BUILD_MODEL, OnBuildModel)
	ON_COMMAND(IDM_MAP_PROPERTIES, OnMapProperties)
	ON_UPDATE_COMMAND_UI(ID_BUILD_MODEL, OnUpdateBuildModel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

#endif
/////////////////////////////////////////////////////////////////////////////
// CTread3DDoc construction/destruction

CTread3DDoc::CTread3DDoc()
{
	m_bNewMap = false;

	m_pObjectList = new CLinkedList<CBaseObject>();
	m_pSelectedObjectList = new CLinkedList<CBaseObject>();
	m_pUIList = new CLinkedList<CBaseObject>();
	m_pTextureMRU = new CLinkedList<TEXMRUREF>();
	m_pSurfaceObjList = new CLinkedList<SURFACEOBJREF>();

	m_nClassDrawFilter = OBJECT_CLASS_CAMERA;
	m_nGlobalRenderFlags = RF_REALTIME3D|RF_TEXTURED|RF_NOBILINEARFILTER|RF_SHADED;
	m_nClassSelectionFilter = 0;
	m_Mode.SetDocument(this);
	m_nGridSize = 16;
	m_fHollowDepth = 32.0F;
	m_nCurveSubDepth = GetRegInt(I_CURVESUBDEPTH);
	m_bSnapToGrid = true;

	m_pWorldspawn = NULL;

	m_pRunStates = NULL;
	m_nNumStates = 0;
	m_pCurView=NULL;
	m_nNumSelEnts = m_nNumSelBrushes = 0;

	m_bBrushHandles = true;
	m_bDrawTargets = true;
	m_bUpdateAllViews = true;

	// Default to texlock on.
	m_Mode.SetFlags(FLAG_TEXLOCK);

	SetupModeResponders();
}

CTread3DDoc::~CTread3DDoc()
{
}

void CTread3DDoc::CycleColorIfSelected()
{
	if(m_pSelectedObjectList->IsEmpty() == false)
		GetColorCycleController()->StartAnimating();
	else
		GetColorCycleController()->StartAnimating(false);
}

SURFACEOBJREF* CTread3DDoc::FindSurfaceObjectRef(CBaseObject* pObject)
{
	SURFACEOBJREF* pRef;
	m_pSurfaceObjList->First();
	for(pRef = m_pSurfaceObjList->GetCurItem(); pRef != NULL; pRef = m_pSurfaceObjList->GetNextItem())
		if(pRef->pObj == pObject)
			return pRef;

	return NULL;
}

void CTread3DDoc::StoreXY(int nX, int nY, void* pParam)
{
	m_nStoreX = nX;
	m_nStoreY = nY;
	m_pStoreParam = pParam;
}

void CTread3DDoc::RetrieveXY(int* pX, int* pY, void** pParam)
{
	*pX = m_nStoreX;
	*pY = m_nStoreY;
	*pParam = m_pStoreParam;
}

bool CTread3DDoc::AddObjectToSurfaceSelection(CBaseObject* pObject)
{
	if(FindSurfaceObjectRef(pObject) != NULL)
		return false;

	SURFACEOBJREF* pRef = new SURFACEOBJREF();
	pRef->pObj = pObject;
	pObject->m_nObjectRenderFlags |= RF_SELECTED;
	m_pSurfaceObjList->AddItem(pRef);
	return true;
}

bool CTread3DDoc::RemoveObjectFromSurfaceSelection(CBaseObject* pObject)
{
	SURFACEOBJREF* pRef = FindSurfaceObjectRef(pObject);
	if(pRef == NULL)
		return false;
	pRef->pObj->m_nObjectRenderFlags &= ~RF_SELECTED;
	m_pSurfaceObjList->DeleteItem(pRef);
	return true;
}

bool CTread3DDoc::ClearSurfaceSelection()
{
	SURFACEOBJREF* pRef;
	m_pSurfaceObjList->First();
	for(pRef = m_pSurfaceObjList->GetCurItem(); pRef != NULL; pRef = m_pSurfaceObjList->GetNextItem())
		pRef->pObj->m_nObjectRenderFlags &= ~RF_SELECTED;
	m_pSurfaceObjList->DestroyList();
	return true;
}

void CTread3DDoc::SetSelectedObjectsVisgroup(CString sName, CString sCategory)
{
	CBaseObject* pObj;
	CVisgroupRefObject* pVisRef;

	// First does this visgroup exist?
	CVisgroup* pVisgroup = m_VisgroupAPI.FindVisgroup(sName, sCategory);
	if(pVisgroup == NULL)
		m_VisgroupAPI.AddVisgroup(sName, sCategory);

	m_pSelectedObjectList->First();
	for(pObj = m_pSelectedObjectList->GetCurItem(); pObj != NULL; pObj = m_pSelectedObjectList->GetNextItem())
	{
		if(pObj->GetObjectInterfaces()&I_VISGROUPREF)
		{
			pVisRef = (CVisgroupRefObject*)pObj->GetInterface(I_VISGROUPREF);
			if(pVisRef == NULL)
			{
				Warning("CTread3DDoc::SetSelectedOBjectsVisgroup(): I_VISGROUPREF interface is NULL!!\n");
				continue;
			}

			pVisRef->SetVisgroup(sName, sCategory);
		}
	}
}

void CTread3DDoc::AdjustView(CMapView* pView)
{
	// Does the selection go off an edge?
	if(VMO(VEC_X, pView) > 0)
	{
		if(m_vSelMaxs[VI(VEC_X, pView)] >= VMMAX_X(pView))
			VMC_X(pView) += (m_vSelMaxs[VI(VEC_X, pView)] - VMMAX_X(pView)) + m_nGridSize;
	}
	else
	{
		if(m_vSelMaxs[VI(VEC_X, pView)] >= VMMIN_X(pView))
			VMC_X(pView) += (m_vSelMaxs[VI(VEC_X, pView)] - VMMIN_X(pView)) + m_nGridSize;
	}

	if(VMO(VEC_Y, pView) > 0)
	{
		if(m_vSelMaxs[VI(VEC_Y, pView)] >= VMMAX_Y(pView))
			VMC_Y(pView) += (m_vSelMaxs[VI(VEC_Y, pView)] - VMMAX_Y(pView)) + m_nGridSize;
	}
	else
	{
		if(m_vSelMaxs[VI(VEC_Y, pView)] >= VMMIN_Y(pView))
			VMC_Y(pView) += (m_vSelMaxs[VI(VEC_Y, pView)] - VMMIN_Y(pView)) + m_nGridSize;
	}

	pView->CalcMapBounds();

	if(VMO(VEC_X, pView) > 0)
	{
		if(m_vSelMins[VI(VEC_X, pView)] <= VMMIN_X(pView))
			VMC_X(pView) -= (VMMIN_X(pView) - m_vSelMins[VI(VEC_X, pView)]) - m_nGridSize;
	}
	else
	{
		if(m_vSelMins[VI(VEC_X, pView)] <= VMMAX_X(pView))
			VMC_X(pView) -= (VMMAX_X(pView) - m_vSelMins[VI(VEC_X, pView)]) - m_nGridSize;
	}

	if(VMO(VEC_Y, pView) > 0)
	{
		if(m_vSelMins[VI(VEC_Y, pView)] <= VMMIN_Y(pView))
			VMC_Y(pView) -= (VMMIN_Y(pView) - m_vSelMins[VI(VEC_Y, pView)]) - m_nGridSize;
	}
	else
	{
		if(m_vSelMins[VI(VEC_Y, pView)] <= VMMAX_Y(pView))
			VMC_Y(pView) -= (VMMAX_Y(pView) - m_vSelMins[VI(VEC_Y, pView)]) - m_nGridSize;
	}

	pView->CalcMapBounds();
}

void CTread3DDoc::AdjustViews()
{
#ifdef _WIN32
	CView* pView;
	CMapView* pMapView;

	POSITION pos = GetFirstViewPosition();
	while(pos != NULL)
	{
		pView = GetNextView(pos);
		if(!pView->IsKindOf(RUNTIME_CLASS(CMapView)))
			continue;
		pMapView = (CMapView*)pView;
		if(pMapView->m_nView != VIEW_3D)
			AdjustView(pMapView);
	}
#endif

}

void CTread3DDoc::CenterViewOnSelection(CMapView* pView)
{
	if(pView->m_nView == VIEW_3D)
		return;

	pView->m_vMapCenter[0] = m_vSelCenter[VI(VEC_X, pView)];
	pView->m_vMapCenter[1] = m_vSelCenter[VI(VEC_Y, pView)];
	pView->CalcMapBounds();
}

void CTread3DDoc::CenterViewsOnSelection()
{
#ifdef _WIN32
	CView* pView;
	CMapView* pMapView;

	POSITION pos = GetFirstViewPosition();
	while(pos != NULL)
	{
		pView = GetNextView(pos);
		if(!pView->IsKindOf(RUNTIME_CLASS(CMapView)))
			continue;
		pMapView = (CMapView*)pView;
		if(pMapView->m_nView != VIEW_3D)
			CenterViewOnSelection(pMapView);
	}
#endif
}

void CTread3DDoc::CenterViewOnUIObject(CMapView* pView, CBaseObject* pObject)
{
	vec3_t vC;

	if(pObject->GetObjectClass()==OBJECT_CLASS_CAMERA)
	{
		CCamera* pCam = (CCamera*)pObject;
		VectorCopy(pCam->GetOrigin(), vC);
	}
	else if(pObject->GetObjectClass()==OBJECT_CLASS_ANCHOR)
	{
		CAnchor* pAnc = (CAnchor*)pObject;
		VectorCopy(pAnc->GetOrigin(), vC);
	}
	else
		return;

	pView->m_vMapCenter[0] = vC[VI(VEC_X, pView)];
	pView->m_vMapCenter[1] = vC[VI(VEC_Y, pView)];
	pView->CalcMapBounds();
}

void CTread3DDoc::CenterViewsOnUIObject(CBaseObject* pObject)
{
#ifdef _WIN32
	CView* pView;
	CMapView* pMapView;

	POSITION pos = GetFirstViewPosition();
	while(pos != NULL)
	{
		pView = GetNextView(pos);
		if(!pView->IsKindOf(RUNTIME_CLASS(CMapView)))
			continue;
		pMapView = (CMapView*)pView;
		if(pMapView->m_nView != VIEW_3D)
			CenterViewOnUIObject(pMapView, pObject);
	}
#endif	
}

void CTread3DDoc::SetupDefaultRunStates()
{
	if(m_pRunStates != NULL)
		delete m_pRunStates;

	m_nNumStates = 0;
	m_pRunStates = NULL;

	DEFBUILD* pDefBuild = GetGameAPI()->GetCurrentGame()->GetDefBuild();
	if(pDefBuild == NULL)
		return;
	
	// Traverse the list.
	int i=0, nCount=pDefBuild->pModuleList->GetCount();
	m_pRunStates = new RUNSTATE[nCount];

	m_nNumStates = nCount;

	DEFMODULE* pMod;

	pDefBuild->pModuleList->First();
	for(pMod = pDefBuild->pModuleList->GetCurItem(); pMod != NULL; pMod = pDefBuild->pModuleList->GetNextItem(),i++)
	{
		m_pRunStates[i].sName = pMod->sName;
		m_pRunStates[i].sValue = pMod->sPath;
		if(pMod->pDefParam != NULL)
			m_pRunStates[i].nValue = pMod->pDefParam->nValue;
		else
			m_pRunStates[i].nValue = 0;
		m_pRunStates[i].sCmdLine = "";
	}
}

bool CTread3DDoc::ExportMap(CString sExportName)
{
	TREAD2XHEADERINFO header;

	// Try to open the file.
	CFile hFile;
	if(!hFile.Open(sExportName, CFile::modeCreate|CFile::modeWrite|CFile::typeBinary|CFile::shareExclusive))
	{
		Error("Unable to open map file for export");
		return false;
	}

	header.m_nFlags = T2XHF_MAPFILE;
	if(!GetTreadMapAPI()->WriteMap(hFile, &header, this, m_pObjectList, m_pSelectedObjectList, m_pUIList))
	{
		hFile.Close();
		Error("Unable to export map");
		return false;
	}

	Console("Exported file to: %s\n", hFile.GetFilePath());

	hFile.Close();

	return true;
}

// Runs the map based on the runstates.
bool CTread3DDoc::RunMap()
{
	RUNBLOCK* pBlock = new RUNBLOCK();
	pBlock->pDocument = this;
	pBlock->pGameDef = GetGameAPI()->GetCurrentGame();


	char szFile[256];
	GetFileTitle(GetPathName(), szFile, 256);
	pBlock->sTreadFile = SetFileExtension(szFile, "");

	// Run.
	GetGameAPI()->GetCurrentGame()->RunMap(pBlock);

	return true;
}

int CTread3DDoc::GetRunStateValue(CString sName)
{
	if(m_pRunStates == NULL)
		return false;

	for(int i = 0; i < m_nNumStates; i++)
	{
		if(m_pRunStates[i].sName == sName)
			return m_pRunStates[i].nValue;
	}

	return 0;
}

CString CTread3DDoc::GetRunStateCmdLine(CString sName)
{
	if(m_pRunStates == NULL)
		return "";

	for(int i = 0; i < m_nNumStates; i++)
	{
		if(m_pRunStates[i].sName == sName)
			return m_pRunStates[i].sCmdLine;
	}

	return "";
}

bool CTread3DDoc::SaveRunState(CString sName, int nValue, CString sCmdLine)
{
	if(m_pRunStates == NULL)
		return false;

	for(int i = 0; i < m_nNumStates; i++)
	{
		if(m_pRunStates[i].sName == sName)
		{
			m_pRunStates[i].nValue = nValue;
			m_pRunStates[i].sCmdLine = sCmdLine;
			return true;
		}
	}

	return false;
}

RUNSTATE* CTread3DDoc::GetRunState(int nNum)
{
	if(m_pRunStates == NULL)
		return NULL;

	return &m_pRunStates[nNum];
}

RUNSTATE* CTread3DDoc::GetRunState(CString sName)
{
	if(m_pRunStates == NULL)
		return NULL;

	for(int i = 0; i < m_nNumStates; i++)
	{
		if(m_pRunStates[i].sName == sName)
			return &m_pRunStates[i];
	}

	return NULL;
}

TEXMRUREF* CTread3DDoc::FindMRURef(CTexture* pTexture)
{
	TEXMRUREF* pRef;

	m_pTextureMRU->First();
	for(pRef = m_pTextureMRU->GetCurItem(); pRef != NULL; pRef = m_pTextureMRU->GetNextItem())
	{
		if(pRef->pTex == pTexture)
			return pRef;
	}

	return NULL;
}

#define MAX_MRU_SIZE	50
void CTread3DDoc::AddTextureToMRU(CTexture* pTexture)
{
#ifdef _WIN32
	TEXMRUREF* pRef;
	
	CTextureView* pView = (CTextureView*)GetView(TEXTURE_VIEW);

	// Is it already in there?
	pRef = FindMRURef(pTexture);
	if(pRef != NULL)
	{
		m_pTextureMRU->RemoveItem(pRef);
		m_pTextureMRU->AddItem(pRef, LL_HEAD);
		pView->LoadMRU(m_pTextureMRU);
		return;
	}

	pRef = new TEXMRUREF();
	
	pRef->pTex = pTexture;
	pRef->sName = pTexture->m_sName;

	m_pTextureMRU->AddItem(pRef, LL_HEAD);
	if(m_pTextureMRU->GetCount() > MAX_MRU_SIZE)
		m_pTextureMRU->DeleteItem(LL_TAIL);

	pView->LoadMRU(m_pTextureMRU);
#endif
}

void CTread3DDoc::TextureMRUIAL()
{
	TEXMRUREF* pRef;

	m_pTextureMRU->First();
	for(;;)
	{
		pRef = m_pTextureMRU->GetCurItem();
		if(pRef == NULL)
			break;

		// Load it.
		pRef->pTex = GetTexAPI()->GetTextureFromName(pRef->sName);
		if(pRef->pTex == NULL)
		{
			Warning("IAL: MRU Texture " + pRef->sName + " not found, deleting.");
			m_pTextureMRU->DeleteItem(LL_CURRENT);
			continue;
		}

		m_pTextureMRU->GetNextItem();
	}
}

CBaseTextureableObject* CTread3DDoc::GetFirstSurfaceObjectAsTextureable()
{
	m_pSurfaceObjList->First();
	SURFACEOBJREF* pRef = m_pSurfaceObjList->GetCurItem();
	CBaseTextureableObject* pT = (CBaseTextureableObject*)pRef->pObj->GetInterface(I_TEXTUREABLE);
	return pT;
}

int CTread3DDoc::GetContentsFlags()
{
	CBaseTextureableObject* pT = GetFirstSurfaceObjectAsTextureable();
	return pT->m_nContents;
}

int CTread3DDoc::GetSurfaceFlags()
{
	CBaseTextureableObject* pT = GetFirstSurfaceObjectAsTextureable();
	return pT->m_nSurface;
}

int CTread3DDoc::GetValueFlags()
{
	CBaseTextureableObject* pT = GetFirstSurfaceObjectAsTextureable();
	return pT->m_nValue;
}

void CTread3DDoc::UpdateSurfaceAttributes()
{
	SURFACEOBJREF* pRef;
	CFace* pFace;
	CQBrush* pBrush;

	m_pSurfaceObjList->First();
	for(pRef = m_pSurfaceObjList->GetCurItem(); pRef != NULL; pRef = m_pSurfaceObjList->GetNextItem())
	{
		if(pRef->pObj->GetObjectClass() != OBJECT_CLASS_FACE)
			continue;
		pFace = (CFace*)pRef->pObj;
		pBrush = (CQBrush*)pFace->m_pParent;
		if(pBrush != NULL)
			pBrush->OnAttributeChange();
		else
			pFace->OnAttributeChange();
	}
}

void CTread3DDoc::SetContentsFlags(int nContents)
{
	SURFACEOBJREF* pRef;
	CBaseTextureableObject* pT;

	m_pSurfaceObjList->First();
	for(pRef = m_pSurfaceObjList->GetCurItem(); pRef != NULL; pRef = m_pSurfaceObjList->GetNextItem())
	{
		pT = (CBaseTextureableObject*)pRef->pObj->GetInterface(I_TEXTUREABLE);
		pT->m_nContents = nContents;
	}

	UpdateSurfaceAttributes();
}

void CTread3DDoc::SetSurfaceFlags(int nFlags)
{
	SURFACEOBJREF* pRef;
	CBaseTextureableObject* pT;

	m_pSurfaceObjList->First();
	for(pRef = m_pSurfaceObjList->GetCurItem(); pRef != NULL; pRef = m_pSurfaceObjList->GetNextItem())
	{
		pT = (CBaseTextureableObject*)pRef->pObj->GetInterface(I_TEXTUREABLE);
		pT->m_nSurface = nFlags;
	}

	UpdateSurfaceAttributes();
}

void CTread3DDoc::SetValueFlags(int nValue)
{
	SURFACEOBJREF* pRef;
	CBaseTextureableObject* pT;

	m_pSurfaceObjList->First();
	for(pRef = m_pSurfaceObjList->GetCurItem(); pRef != NULL; pRef = m_pSurfaceObjList->GetNextItem())
	{
		pT = (CBaseTextureableObject*)pRef->pObj->GetInterface(I_TEXTUREABLE);
		pT->m_nValue = nValue;
	}

	UpdateSurfaceAttributes();
}

void CTread3DDoc::SetTexShifts(vec_t vTexShifts[2])
{
	SURFACEOBJREF* pRef;
	CBaseTextureableObject* pT;

	m_pSurfaceObjList->First();
	for(pRef = m_pSurfaceObjList->GetCurItem(); pRef != NULL; pRef = m_pSurfaceObjList->GetNextItem())
	{
		pT = (CBaseTextureableObject*)pRef->pObj->GetInterface(I_TEXTUREABLE);
		pT->SetTexShifts(vTexShifts);
	}
}

void CTread3DDoc::GetTexShifts(vec_t vTexShifts[2])
{
	CBaseTextureableObject* pT = GetFirstSurfaceObjectAsTextureable();
	pT->GetTexShifts(vTexShifts);
}

void CTread3DDoc::SetTexScales(vec_t vTexScales[2])
{
	SURFACEOBJREF* pRef;
	CBaseTextureableObject* pT;

	m_pSurfaceObjList->First();
	for(pRef = m_pSurfaceObjList->GetCurItem(); pRef != NULL; pRef = m_pSurfaceObjList->GetNextItem())
	{
		pT = (CBaseTextureableObject*)pRef->pObj->GetInterface(I_TEXTUREABLE);
		pT->SetTexScales(vTexScales);
	}

}

void CTread3DDoc::GetTexScales(vec_t vTexScales[2])
{
	CBaseTextureableObject* pT = GetFirstSurfaceObjectAsTextureable();
	pT->GetTexScales(vTexScales);
}

CString CTread3DDoc::GetTextureName()
{
	CBaseTextureableObject* pT = GetFirstSurfaceObjectAsTextureable();
	return pT->GetTextureName();
}

void CTread3DDoc::SetTextureName(LPCTSTR szName, bool bEvent)
{
	SURFACEOBJREF* pRef;
	CBaseTextureableObject* pT;

	m_pSurfaceObjList->First();
	for(pRef = m_pSurfaceObjList->GetCurItem(); pRef != NULL; pRef = m_pSurfaceObjList->GetNextItem())
	{
		pT = (CBaseTextureableObject*)pRef->pObj->GetInterface(I_TEXTUREABLE);
		pT->SetTextureName(szName, bEvent);
	}
}

CTexture* CTread3DDoc::GetTexturePtr()
{
	CBaseTextureableObject* pT = GetFirstSurfaceObjectAsTextureable();
	return pT->GetTexturePtr();
}

vec_t CTread3DDoc::GetTexRotate()
{
	CBaseTextureableObject* pT = GetFirstSurfaceObjectAsTextureable();
	if(pT != NULL)
		return pT->GetRotation();

	return 0.0F;
}

void CTread3DDoc::SetTexRotate(vec_t pRot)
{
	SURFACEOBJREF* pRef;
	CBaseTextureableObject* pT;

	m_pSurfaceObjList->First();
	for(pRef = m_pSurfaceObjList->GetCurItem(); pRef != NULL; pRef = m_pSurfaceObjList->GetNextItem())
	{
		pT = (CBaseTextureableObject*)pRef->pObj->GetInterface(I_TEXTUREABLE);
		pT->SetRotation(pRot);
	}
}

void CTread3DDoc::ToggleRenderFlag(int nFlag)
{
	if(m_nGlobalRenderFlags&nFlag)
		m_nGlobalRenderFlags &= ~nFlag;
	else
		m_nGlobalRenderFlags |= nFlag;
}

void CTread3DDoc::InvokeAction(int nAction, void* pParam)
{
	m_EventManager.InvokeAction(nAction, pParam, this);
}

void CTread3DDoc::SetSelectedObjectsTexture(CString sTexture)
{
	G_TextureList(m_pSelectedObjectList, sTexture);
}

bool CTread3DDoc::AddObjectToSelection(CBaseObject* pObject)
{
	if(pObject->GetObjectInterfaces()&I_UI)
	{
		if(m_pUIList->InList(pObject))
			m_pUIList->RemoveItem(pObject);
		if(!m_pSelectedObjectList->InList(pObject))
			m_pSelectedObjectList->AddItem(pObject);
		else
			return false;
	}
	else
	{
		if(pObject->GetObjectInterfaces()&I_VISGROUPREF)
			((CVisgroupRefObject*)pObject->GetInterface(I_VISGROUPREF))->BindToDocument(this);

		if(m_pObjectList->InList(pObject))
			m_pObjectList->RemoveItem(pObject);
		if(!m_pSelectedObjectList->InList(pObject))
		{
			m_pSelectedObjectList->AddItem(pObject);
			if(pObject->GetObjectClass()&OBJECT_CLASS_BRUSH)
				m_nNumSelBrushes++;
			if(pObject->GetObjectClass()&OBJECT_CLASS_ENTITY)
				m_nNumSelEnts++;
			if(pObject->GetObjectClass()&OBJECT_CLASS_LENTITY)
				m_nNumSelEnts++;
		}
		else
			return false;
	}

	return true;
}

void CTread3DDoc::AddClassSelectionFilters(int nFlags)
{
	m_nClassSelectionFilter |= nFlags;
}

void CTread3DDoc::RemoveClassSelectionFilters(int nFlags)
{
	m_nClassSelectionFilter &= ~nFlags;
}

bool CTread3DDoc::ClearSelection()
{
	CBaseObject* pObject;

	if(m_pSelectedObjectList->IsEmpty())
		return false;

	for(;;)
	{
		m_pSelectedObjectList->First();
		pObject = m_pSelectedObjectList->RemoveItem(LL_HEAD);
		if(pObject == NULL)
			break;
		AddObjectToMap(pObject);
	}

	m_nNumSelBrushes = m_nNumSelEnts = 0;

	return true;
}

int CTread3DDoc::GetSelectedObjectCount(int nObjectClassFilter)
{
	return G_GetClassCount(m_pSelectedObjectList, nObjectClassFilter);
}

int CTread3DDoc::GetSelectedInterfaceCount(int nInterface)
{
	return G_GetInterfaceCount(m_pSelectedObjectList, nInterface);
}

bool CTread3DDoc::IsInterfaceSelected(int nInterface)
{
	return GetSelectedInterfaceCount(nInterface) > 0;
}

bool CTread3DDoc::DeleteSelection()
{
	if(m_pSelectedObjectList->IsEmpty())
		return false;

	G_PreDelete(m_pSelectedObjectList, this);
	m_pSelectedObjectList->ReleaseList();
	return true;
}

bool CTread3DDoc::DuplicateSelection()
{
	G_CopyList(m_pSelectedObjectList, m_pObjectList);
	return m_pSelectedObjectList->IsEmpty() == false;
}

void CTread3DDoc::BuildSelectionBBox()
{
	CBaseObject* pObject;
	CBaseTransformableObject* pTrans;

	InitializeRange(m_vSelMins, m_vSelMaxs);

	/*m_pSelectedUIList->First();
	for(pObject = m_pSelectedUIList->GetCurItem(); pObject != NULL; pObject = m_pSelectedUIList->GetNextItem())
	{
		pTrans = (CBaseTransformableObject*)pObject->GetInterface(I_TRANSFORMABLE);
		if(pTrans != NULL)
			pTrans->InsertIntoRange(m_vSelMins, m_vSelMaxs);
		
	}*/

	m_pSelectedObjectList->First();
	for(pObject = m_pSelectedObjectList->GetCurItem(); pObject != NULL; pObject = m_pSelectedObjectList->GetNextItem())
	{
		pTrans = (CBaseTransformableObject*)pObject->GetInterface(I_TRANSFORMABLE);
		if(pTrans != NULL)
			pTrans->InsertIntoRange(m_vSelMins, m_vSelMaxs);
	}

	// Compute center.
	VectorAdd(m_vSelMins, m_vSelMaxs, m_vSelCenter);
	VectorScale(m_vSelCenter, 0.5F, m_vSelCenter);
	VectorSubtract(m_vSelMaxs, m_vSelMins, m_vSelSize);
}

void CTread3DDoc::UnbindViews(int nType)
{
#ifdef _WIN32
	// Rebinds all the views to the RenderPackage.
	CView* pView;
	CMapView* pMapView;

	POSITION pos = GetFirstViewPosition();
	while(pos != NULL)
	{
		pView = GetNextView(pos);
		if(!pView->IsKindOf(RUNTIME_CLASS(CMapView)))
			continue;
		pMapView = (CMapView*)pView;
		if(pMapView->m_nView&nType)
			pMapView->UnbindView();
	}
#endif
}

void CTread3DDoc::RebindViews(int nType)
{
#ifdef _WIN32
	// Rebinds all the views to the RenderPackage.
	CView* pView;
	CMapView* pMapView;

	POSITION pos = GetFirstViewPosition();
	while(pos != NULL)
	{
		pView = GetNextView(pos);
		if(!pView->IsKindOf(RUNTIME_CLASS(CMapView)))
			continue;
		pMapView = (CMapView*)pView;
		if(pMapView->m_nView&nType)
		{
			// Calling SetView() rebinds the view to it's
			// renderer.
			pMapView->SetView(pMapView->m_nView);
		}
	}
#endif
}

void CTread3DDoc::BuildHotSpots(int nFlags)
{
#ifdef _WIN32
	CView* pView;
	CMapView* pMapView;

	POSITION pos = GetFirstViewPosition();
	while(pos != NULL)
	{
		pView = GetNextView(pos);
		if(!pView->IsKindOf(RUNTIME_CLASS(CMapView)))
			continue;
		pMapView = (CMapView*)pView;
		if(pMapView->m_nView != VIEW_3D && (pMapView->m_nView&nFlags))
			pMapView->GetHotSpotManager()->BuildHotSpots(this, pMapView);
	}
#endif
}

void CTread3DDoc::UpdateObjectCounts()
{
	CBaseObject* pObject;

	m_nNumSelBrushes = 0;
	m_nNumSelEnts = 0;

	m_pSelectedObjectList->First();
	for(pObject = m_pSelectedObjectList->GetCurItem(); pObject != NULL; pObject = m_pSelectedObjectList->GetNextItem())
	{
		if(pObject->GetObjectClass()==OBJECT_CLASS_BRUSH)
			m_nNumSelBrushes++;
		if(pObject->GetObjectClass()==OBJECT_CLASS_ENTITY)
			m_nNumSelEnts++;
	}
}

bool CTread3DDoc::RemoveObjectFromMap(CBaseObject* pObject)
{
	if(pObject->GetObjectInterfaces()&I_UI)
	{
		if(m_pSelectedObjectList->InList(pObject))
			m_pSelectedObjectList->RemoveItem(pObject);
		else if(m_pUIList->InList(pObject))
			m_pUIList->RemoveItem(pObject);
		else
			return false;
	}
	else
	{
		if(m_pSelectedObjectList->InList(pObject))
		{
			// Decrement the counters.
			if(pObject->GetObjectClass()&OBJECT_CLASS_BRUSH)
				m_nNumSelBrushes--;
			if(pObject->GetObjectClass()&OBJECT_CLASS_ENTITY)
				m_nNumSelEnts--;

			m_pSelectedObjectList->RemoveItem(pObject);
		}
		else if(m_pObjectList->InList(pObject))
			m_pObjectList->RemoveItem(pObject);
		else
			return false;
	}

	return true;
}

bool CTread3DDoc::AddObjectToMap(CBaseObject* pObject)
{ 
	if(pObject->GetObjectInterfaces()&I_UI)
	{
		if(m_pSelectedObjectList->InList(pObject))
			m_pSelectedObjectList->RemoveItem(pObject);
		if(!m_pUIList->InList(pObject))
			m_pUIList->AddItem(pObject);
		else
			return false;
	}
	else
	{
		if(pObject->GetObjectInterfaces()&I_VISGROUPREF)
			((CVisgroupRefObject*)pObject->GetInterface(I_VISGROUPREF))->BindToDocument(this);

		if(m_pSelectedObjectList->InList(pObject))
		{
			// Decrement the counters.
			if(pObject->GetObjectClass()&OBJECT_CLASS_BRUSH)
				m_nNumSelBrushes--;
			if(pObject->GetObjectClass()&OBJECT_CLASS_ENTITY)
				m_nNumSelEnts--;
			if(pObject->GetObjectClass()&OBJECT_CLASS_LENTITY)
				m_nNumSelEnts--;

			m_pSelectedObjectList->RemoveItem(pObject);
		}
		if(!m_pSelectedObjectList->InList(pObject))
			m_pObjectList->AddItem(pObject);
		else
			return false;
	}

	return true;
}

void CTread3DDoc::SetupModeResponders()
{
	m_EventManager.AddEventResponder(new CCameraModeResponder());
	m_EventManager.AddEventResponder(new CVertexModeResponder());
	m_EventManager.AddEventResponder(new CEntityModeResponder());
	m_EventManager.AddEventResponder(new CBrushModeResponder());
	m_EventManager.AddEventResponder(new CPointerModeResponder());
	m_EventManager.AddEventResponder(new CZoomModeResponder());
	m_EventManager.AddEventResponder(new CPaintModeResponder());
	m_EventManager.AddEventResponder(new CClipToolModeResponder());
	m_EventManager.AddEventResponder(new CFaceModeResponder());
}

void CTread3DDoc::OnModeChangeEvent(int nOldMode, int nOldSubmode, int nNewMode, int nNewSubmode)
{
	m_EventManager.OnModeChange(&m_Mode, this, nOldMode, nOldSubmode, nNewMode, nNewSubmode);
}

CMapView* CTread3DDoc::GetViewByType(int nType)
{
#ifdef _WIN32
	CView* pView;
	CMapView* pMapView;

	POSITION pos = GetFirstViewPosition();
	while(pos != NULL)
	{
		pView = GetNextView(pos);
		if(!pView->IsKindOf(RUNTIME_CLASS(CMapView)))
			continue;
		pMapView = (CMapView*)pView;
		if(pMapView->m_nView == nType)
			return pMapView;
	}
#endif
	return NULL;
}

CMapView* CTread3DDoc::GetOtherViewType(int nType)
{
#ifdef _WIN32
	CView* pView;
	CMapView* pMapView;

	POSITION pos = GetFirstViewPosition();
	while(pos != NULL)
	{
		pView = GetNextView(pos);
		if(!pView->IsKindOf(RUNTIME_CLASS(CMapView)))
			continue;
		pMapView = (CMapView*)pView;
		if(pMapView->m_nView != nType && pMapView->m_nView != VIEW_3D)
			return pMapView;
	}
#endif

	return NULL;
}

void CTread3DDoc::CenterViews(vec3_t vPoint, int nViewFilter)
{
#ifdef _WIN32
	CView* pView;
	CMapView* pMapView;

	POSITION pos = GetFirstViewPosition();
	while(pos != NULL)
	{
		pView = GetNextView(pos);
		if(!pView->IsKindOf(RUNTIME_CLASS(CMapView)))
			continue;
		pMapView = (CMapView*)pView;
		if(pMapView->m_nView&nViewFilter)
			pMapView->SetMapXY(vPoint[VI(VEC_X, pMapView)], vPoint[VI(VEC_Y, pMapView)]);
	}
#endif
}

void CTread3DDoc::ToggleViewRenderFlag(int nViewFlags, int nFlag)
{
#ifdef _WIN32
	CView* pView;
	CMapView* pMapView;

	POSITION pos = GetFirstViewPosition();
	while(pos != NULL)
	{
		pView = GetNextView(pos);
		if(!pView->IsKindOf(RUNTIME_CLASS(CMapView)))
			continue;
		pMapView = (CMapView*)pView;
		if(pMapView->m_nView&nViewFlags)
		{
			if(pMapView->GetGlobalRenderFlags()&nFlag)
				pMapView->RemoveGlobalRenderFlags(nFlag);
			else
				pMapView->AddGlobalRenderFlags(nFlag);
		}
	}
#endif
}

void CTread3DDoc::ToggleViewDrawFilter(int nViewFlags, int nFlag)
{
#ifdef _WIN32
	CView* pView;
	CMapView* pMapView;

	POSITION pos = GetFirstViewPosition();
	while(pos != NULL)
	{
		pView = GetNextView(pos);
		if(!pView->IsKindOf(RUNTIME_CLASS(CMapView)))
			continue;
		pMapView = (CMapView*)pView;
		if(pMapView->m_nView&nViewFlags)
		{
			if(pMapView->GetDrawFilters()&nFlag)
				pMapView->RemoveDrawFilters(nFlag);
			else
				pMapView->AddDrawFilters(nFlag);
		}
	}
#endif
}


void CTread3DDoc::AddViewDrawFilters(int nViewFlags, int nFlags)
{
#ifdef _WIN32
	CView* pView;
	CMapView* pMapView;

	POSITION pos = GetFirstViewPosition();
	while(pos != NULL)
	{
		pView = GetNextView(pos);
		if(!pView->IsKindOf(RUNTIME_CLASS(CMapView)))
			continue;
		pMapView = (CMapView*)pView;
		if(pMapView->m_nView&nViewFlags)
			pMapView->AddDrawFilters(nFlags);
	}
#endif
}

void CTread3DDoc::RemoveViewDrawFilters(int nViewFlags, int nFlags)
{
#ifdef _WIN32
	CView* pView;
	CMapView* pMapView;

	POSITION pos = GetFirstViewPosition();
	while(pos != NULL)
	{
		pView = GetNextView(pos);
		if(!pView->IsKindOf(RUNTIME_CLASS(CMapView)))
			continue;
		pMapView = (CMapView*)pView;
		if(pMapView->m_nView&nViewFlags)
			pMapView->RemoveDrawFilters(nFlags);
	}
#endif
}

void CTread3DDoc::AddViewGlobalRenderFlags(int nViewFlags, int nFlags)
{
#ifdef _WIN32
	CView* pView;
	CMapView* pMapView;

	POSITION pos = GetFirstViewPosition();
	while(pos != NULL)
	{
		pView = GetNextView(pos);
		if(!pView->IsKindOf(RUNTIME_CLASS(CMapView)))
			continue;
		pMapView = (CMapView*)pView;
		if(pMapView->m_nView&nViewFlags)
			pMapView->AddGlobalRenderFlags(nFlags);
	}
#endif
}

void CTread3DDoc::RemoveViewGlobalRenderFlags(int nViewFlags, int nFlags)
{
#ifdef _WIN32
	CView* pView;
	CMapView* pMapView;

	POSITION pos = GetFirstViewPosition();
	while(pos != NULL)
	{
		pView = GetNextView(pos);
		if(!pView->IsKindOf(RUNTIME_CLASS(CMapView)))
			continue;
		pMapView = (CMapView*)pView;
		if(pMapView->m_nView&nViewFlags)
			pMapView->RemoveGlobalRenderFlags(nFlags);
	}
#endif
}

void CTread3DDoc::UpdateWindows(int nUpdateFlags)
{
#ifdef _WIN32
	CView* pView;
	CMapView* pMapView;

	POSITION pos = GetFirstViewPosition();
	while(pos != NULL)
	{
		pView = GetNextView(pos);
		if(!pView->IsKindOf(RUNTIME_CLASS(CMapView)))
			continue;
		pMapView = (CMapView*)pView;
		if(pMapView->m_nView&nUpdateFlags)
			pMapView->RedrawWindow();
	}
#endif
}

void CTread3DDoc::UpdateWindowsSelection(int nUpdateFlags)
{
#ifdef _WIN32
	CView* pView;
	CMapView* pMapView;

	POSITION pos = GetFirstViewPosition();
	while(pos != NULL)
	{
		pView = GetNextView(pos);
		if(!pView->IsKindOf(RUNTIME_CLASS(CMapView)))
			continue;
		pMapView = (CMapView*)pView;
		if(pMapView->m_nView&nUpdateFlags)
			pMapView->RedrawWindow();
	}
#endif
}

int CTread3DDoc::GetViewIndex(CMapView* pTestView)
{
#ifdef _WIN32
	int i=0;

	CView* pView;
	CMapView* pMapView;

	POSITION pos = GetFirstViewPosition();
	while(pos != NULL)
	{
		pView = GetNextView(pos);
		if(!pView->IsKindOf(RUNTIME_CLASS(CMapView)))
			continue;
		pMapView = (CMapView*)pView;
		if(pMapView == pTestView)
			return i;
		i++;
	}
#endif
	return -1;
}

CMapView* CTread3DDoc::GetViewByIndex(int idx)
{
#ifdef _WIN32
	int i=0;

	CView* pView;
	CMapView* pMapView;

	POSITION pos = GetFirstViewPosition();
	while(pos != NULL)
	{
		pView = GetNextView(pos);
		if(!pView->IsKindOf(RUNTIME_CLASS(CMapView)))
			continue;
		if(i != idx)
		{
			i++;
			continue;
		}

		pMapView = (CMapView*)pView;
		return pMapView;
	}
#endif

	return NULL;
}

void CTread3DDoc::SetScale(float fScale)
{
#ifdef _WIN32
	CView* pView;
	CMapView* pMapView;

	POSITION pos = GetFirstViewPosition();
	while(pos != NULL)
	{
		pView = GetNextView(pos);
		if(!pView->IsKindOf(RUNTIME_CLASS(CMapView)))
			continue;
		pMapView = (CMapView*)pView;
		pMapView->SetScale(fScale);
	}
#endif
}

void CTread3DDoc::SetWindowCursors(int nCursor, int nViews)
{
#ifdef _WIN32
	CView* pView;
	CMapView* pMapView;

	POSITION pos = GetFirstViewPosition();
	while(pos != NULL)
	{
		pView = GetNextView(pos);
		if(!pView->IsKindOf(RUNTIME_CLASS(CMapView)))
			continue;
		pMapView = (CMapView*)pView;
		if(pMapView->m_nView&nViews)
			TreadSetCursor(pMapView, nCursor);
	}
#endif
}

void CTread3DDoc::SetupDefaultCameras()
{
#ifdef _WIN32
	CView* pView;
	CMapView* pMapView;

	POSITION pos = GetFirstViewPosition();
	while(pos != NULL)
	{
		pView = GetNextView(pos);
		if(!pView->IsKindOf(RUNTIME_CLASS(CMapView)))
			continue;
		pMapView = (CMapView*)pView;
		if(pMapView->m_nView&VIEW_3D)
			pMapView->SetupDefaultCamera();
	}
#endif
}

void CTread3DDoc::AddDrawFilters(int nFlags)
{
	m_nClassDrawFilter |= nFlags;
}

void CTread3DDoc::RemoveDrawFilters(int nFlags)
{
	m_nClassDrawFilter &= ~nFlags;
}

void CTread3DDoc::AddGlobalRenderFlags(int nFlags)
{
	m_nGlobalRenderFlags |= nFlags;
}

void CTread3DDoc::RemoveGlobalRenderFlags(int nFlags)
{
	m_nGlobalRenderFlags &= ~nFlags;
}

void CTread3DDoc::CreateTestMap()
{
	// Make a test map.
	{
/*		CBezierPatch* pPatch = CBezierPatch::TestPatch();

		pPatch->SetTextureName("e1u1/color1_2");
		//pPatch->SetTextureName("church1_2");

		pPatch->BuildPatch(3);
		AddObjectToSelection(pPatch);

		MODEL model;
		model.m_Header.m_nFlags = T2XHF_BRUSH;
		model.m_Header.m_sName = "4x4 Patch";
		model.m_Header.m_sCategory = "Curves";
		model.m_pObjectList = m_pSelectedObjectList;
		GetGameAPI()->GetCurrentGame()->SaveModel(&model);
		model.m_pObjectList = NULL;
*/
/*
		CQBrush* pBrush = new CQBrush();

		// Make 6 axial faces.
#define p1 {-256, 256, 256}
#define	p2 {256, 256, 256} 
#define p3 {256, -256, 256}
#define	p4 {-256, -256, 256}
#define	p5 {-256, 256, -256}
#define	p6 {256, 256, -256}
#define	p7 {256, -256, -256}
#define	p8 {-256, -256, -256}

		vec3_t f1[4] = {p1, p2, p3, p4};
		vec3_t f2[4] = {p8, p7, p6, p5};
		vec3_t f3[4] = {p4, p3, p7, p8};
		vec3_t f4[4] = {p2, p1, p5, p6};
		vec3_t f5[4] = {p1, p4, p8, p5};
		vec3_t f6[4] = {p3, p2, p6, p7};
		//vec3_t f6[4] = {p7, p6, p2, p3};

		pBrush->m_pFaceList->AddItem(new CFace(4, f1));
		pBrush->m_pFaceList->AddItem(new CFace(4, f2));
		pBrush->m_pFaceList->AddItem(new CFace(4, f3));
		pBrush->m_pFaceList->AddItem(new CFace(4, f4));
		pBrush->m_pFaceList->AddItem(new CFace(4, f5));
		pBrush->m_pFaceList->AddItem(new CFace(4, f6));

		// Texture it!
		pBrush->SetTextureName("e1u1/color1_2");

		// Add the brush to the map.
		AddObjectToSelection(pBrush);
		
		MODEL model;
		model.m_Header.m_nFlags = T2XHF_BRUSH;
		model.m_Header.m_sName = "Cube";
		model.m_Header.m_sCategory = "Primitives";
		model.m_pObjectList = m_pSelectedObjectList;
		GetGameAPI()->GetCurrentGame()->SaveModel(&model);
		model.m_pObjectList = NULL;
*/
/*		vec3_t trans = {-512-128, 0, 0};

		pBrush = new CQBrush(*pBrush);
		pBrush->Translate(trans);

		m_pSelectedObjectList->AddItem(pBrush);
*/
/*
		CEntity* pEntity = GetGameAPI()->GetCurrentGame()->CreateEntityFromDefinition("info_player_start");
		if(pEntity != NULL)
		{
			vec3_t org = { 0, 0, 0 };
			pEntity->SetOrigin(org);
			AddObject(pEntity);
		}
*/
	}
}


BOOL CTread3DDoc::OnNewDocument()
{
	m_bNewMap = true;

#ifdef _WIN32
	if (!CDocument::OnNewDocument())
		return false;
#endif

	if(GetMapManager()->GetMapCount() == 1)
	{
	
#ifdef _WIN32
		CGameDialog dlgTemp;

		if(dlgTemp.DoModal() != IDOK)
			return false;

		if(!GetGameAPI()->SetCurrentGame(dlgTemp.GetGameSelection()))
			return false;

		CLinkedList<GAMEPAKFILE>* m_pPakList = GetGameAPI()->GetCurrentGame()->GetPakFileList();
		if(m_pPakList->GetCount() <= 0)
		{
			int nResponse = MessageBox(NULL, "There are no textures specified for this game.\n Would you like to continue creating a new map?\n Click NO - Add textures now.",
										"Create New Map",
										MB_ICONQUESTION | MB_YESNOCANCEL);
			if(nResponse == IDCANCEL)
				return false;
			else if(nResponse == IDNO)
			{	
				GetGameAPI()->PurgeAndReset();
				CGameSetupDialog setupDlg;
				setupDlg.DoModal();

				GetGameAPI()->SetCurrentGame(dlgTemp.GetGameSelection());
			}
		}

		CTextureView* pTextureView = (CTextureView*)GetView(TEXTURE_VIEW);
		pTextureView->LoadList();

		CObjectView* pObjectView = (CObjectView*)GetView(OBJECT_VIEW);
		pObjectView->LoadBrushList();

		CVisgroupView* pVisgroupView = (CVisgroupView*)GetView(VISGROUP_VIEW);
		pVisgroupView->LoadVisgroup();
#endif

	}

	m_Header.m_sGame = GetGameAPI()->GetCurrentGame()->GetGameName();
	m_Header.m_sAuthor = "Author name here";
	m_Header.m_sComments = "Comments here";

	// Create a worldspawn.
	m_pWorldspawn = (CEntity*)NewBaseObject(OBJECT_CLASS_ENTITY);
	m_pWorldspawn->InitFromDefinition(GetGameAPI()->GetCurrentGame()->GetWorldspawnDefinition());

	CreateTestMap();
	SetupDefaultCameras();
	SetupDefaultRunStates();


	GetModeManager()->Set(MODE_POINTER, POINTER_ALL);
	GetModeManager()->RefreshMode();
	GetMapManager()->UpdateInterface();

	return true;
}


#ifdef _WIN32
/////////////////////////////////////////////////////////////////////////////
// CTread3DDoc serialization

void CTread3DDoc::Serialize(CArchive& ar)
{

	if (ar.IsStoring())
	{
		// Is it a .map file?
		if(CompareExt(ar.GetFile()->GetFileName(), ".map"))
		{
			m_Header.m_nFlags |= T2XHF_MAPFILE;
			m_Header.m_nFlags &= ~T2XHF_TRD2FILE;
		}
		else
		{
			m_Header.m_nFlags |= T2XHF_TRD2FILE;
			m_Header.m_nFlags &= ~T2XHF_MAPFILE;
		}

		// TODO: add storing code here
		if(!GetTreadMapAPI()->WriteMap(*ar.GetFile(), &m_Header, this, m_pObjectList, m_pSelectedObjectList, m_pUIList))
		{
			Error("Unable to save map!");
			//throw new CArchiveException();
		}
	}
	else if(ar.IsLoading())
	{
//		return;

		// TODO: add loading code here.
		if(!GetTreadMapAPI()->ReadMap(*ar.GetFile(), &m_Header, this, m_pObjectList, m_pUIList))
		{
			Error("Unable to read map!");
			throw new CArchiveException();
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// CTread3DDoc diagnostics

#ifdef _DEBUG
void CTread3DDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CTread3DDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

#endif

/////////////////////////////////////////////////////////////////////////////
// CTread3DDoc commands

void CTread3DDoc::Undo()
{
}

void CTread3DDoc::Cut()
{
	Copy();
	this->DeleteSelection();
}

void CTread3DDoc::Copy()
{
	if(m_pSelectedObjectList->IsEmpty())
		return;
	GetGlobalClipboard()->DestroyList();
	G_CopyList(m_pSelectedObjectList, GetGlobalClipboard());
}

void CTread3DDoc::Paste()
{
	CMapView* pView;

	if(m_pCurView == NULL || m_pCurView->m_nView == VIEW_3D)
	{
		pView = GetViewByType(VIEW_TOP);
		if(pView == NULL)
			pView = GetViewByType(VIEW_SIDE);
		if(pView == NULL)
			pView = GetViewByType(VIEW_FRONT);
		if(pView == NULL)
			return;
	}
	else
		pView = m_pCurView;

	vec3_t vTrans;
	GDATA GData;

	CLinkedList<CBaseObject>* pList = new CLinkedList<CBaseObject>();
	G_CopyList(GetGlobalClipboard(), pList);
	CBaseObject* pObj;

	GData.m_nFlags = GF_ORIGIN|GF_RANGE;
	GData.m_pList = pList;
	G_Build(&GData);

	// Move it over in the X direction, by the grid size.
	if(this->GetSelectedObjectCount(OBJECT_CLASS_ALL) > 0)
	{
		vTrans[VI(VEC_X, pView)] = m_vSelMins[VI(VEC_X, pView)] - GData.m_vMins[VI(VEC_X, pView)] + m_nGridSize;
		vTrans[VI(VEC_Y, pView)] = m_vSelMins[VI(VEC_Y, pView)] - GData.m_vMins[VI(VEC_Y, pView)] + m_nGridSize;
		vTrans[VI(VEC_Z, pView)] = 0.0F;
	}
	else
	{
		vec3_t vVec;
		CMapView* pOV = GetOtherViewType(pView->m_nView);

		vVec[VI(VEC_X, pView)] = pView->m_vMapCenter[0];
		vVec[VI(VEC_Y, pView)] = pView->m_vMapCenter[1];
		vVec[VI(VEC_Z, pView)] = pOV->m_vMapCenter[V2I(VU(pView), pOV)];
		G_SetOrigin(vVec, &GData);
	}

	// Clear the selection.
	this->ClearSelection();
	
	pList->First();
	for(;;)
	{
		pObj = pList->RemoveItem(LL_HEAD);
		if(pObj == NULL)
			break;
		AddObjectToSelection(pObj);
	}
	delete pList;

	GData.m_pList = m_pSelectedObjectList;
	G_Translate(vTrans, &GData);
	/*GData.m_nFlags = GF_ORIGIN|GF_RANGE;
	G_Build(&GData);*/
}

void CTread3DDoc::OnCloseDocument() 
{
	// TODO: Add your specialized code here and/or call the base class
	if(m_pObjectList != NULL)
		delete m_pObjectList;
	m_pObjectList = NULL;
	if(m_pSelectedObjectList != NULL)
		delete m_pSelectedObjectList;
	m_pSelectedObjectList = NULL;
	if(m_pUIList != NULL)
		delete m_pUIList;
	m_pUIList = NULL;
	if(m_pTextureMRU != NULL)
		delete m_pTextureMRU;
	if(m_pRunStates != NULL)
		delete[] m_pRunStates;
	if(m_pSurfaceObjList != NULL)
		delete m_pSurfaceObjList;
	m_pSurfaceObjList = NULL;
	if(m_pWorldspawn)
		delete m_pWorldspawn;

#ifdef _WIN32
	CDocument::OnCloseDocument();

	if(GetMapManager()->GetMapCount() == 0)
	{
		CTextureView* pTextureView = (CTextureView*)GetView(TEXTURE_VIEW);
		pTextureView->UnloadList();

		CObjectView* pObjectView = (CObjectView*)GetView(OBJECT_VIEW);
		//pObjectView->UnloadBrushList();
		//pObjectView->UnloadEntityList();
	}
#endif
}

BOOL CTread3DDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
#ifdef _WIN32
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;
#endif

	// TODO: Add your specialized creation code here

	if((m_Header.m_nFlags&T2XHF_IGNORE)==0)
	{
		if(m_Header.m_sGame == "")
		{
			Error("Bad map header!\n");
			return FALSE;
		}

		if(!GetGameAPI()->SetCurrentGame(m_Header.m_sGame))
			return FALSE;

#ifdef _WIN32
		CLinkedList<GAMEPAKFILE>* m_pPakList = GetGameAPI()->GetCurrentGame()->GetPakFileList();
		if(m_pPakList->GetCount() <= 0)
		{
			int nResponse = MessageBox(NULL, "There are no textures specified for this game.\n Would you like to continue creating a new map?\n Click NO - Add textures now.",
										"Create New Map",
										MB_ICONQUESTION | MB_YESNOCANCEL);
			if(nResponse == IDCANCEL)
				return false;
			else if(nResponse == IDNO)
			{	
				GetGameAPI()->PurgeAndReset();
				CGameSetupDialog setupDlg;
				setupDlg.DoModal();

				GetGameAPI()->SetCurrentGame(m_Header.m_sGame);
			}
		}
#endif

		SetGridSize(m_Header.m_nGridSize);
		GetViewByIndex(0)->SetScale(m_Header.m_fScale[0]);
		GetViewByIndex(1)->SetScale(m_Header.m_fScale[1]);
		GetViewByIndex(2)->SetScale(m_Header.m_fScale[2]);
		GetViewByIndex(3)->SetScale(m_Header.m_fScale[3]);
		GetViewByIndex(0)->SetMapXY(m_Header.m_fX[0], m_Header.m_fY[0]);
		GetViewByIndex(1)->SetMapXY(m_Header.m_fX[1], m_Header.m_fY[1]);
		GetViewByIndex(2)->SetMapXY(m_Header.m_fX[2], m_Header.m_fY[2]);
		GetViewByIndex(3)->SetMapXY(m_Header.m_fX[3], m_Header.m_fY[3]);
		GetViewByIndex(0)->SetView(m_Header.m_nViewTypes[0]);
		GetViewByIndex(1)->SetView(m_Header.m_nViewTypes[1]);
		GetViewByIndex(2)->SetView(m_Header.m_nViewTypes[2]);
		GetViewByIndex(3)->SetView(m_Header.m_nViewTypes[3]);
	
		// Hollow depth?
		if(m_Header.m_nVersion > _2X_HEADER_VERSION_1)
		{
			m_fHollowDepth = m_Header.m_fHollowDepth;
			m_nCurveSubDepth = m_Header.m_nCurveSubDepth;
		}
	
		m_nNumStates = m_Header.m_nNumStates;
		m_pRunStates = m_Header.m_pRunStates;
		if(m_pRunStates == NULL)
			SetupDefaultRunStates();
		else
		{
			DEFBUILD* pBuild = GetGameAPI()->GetCurrentGame()->GetDefBuild();
			if(pBuild != NULL)
			{
				if(m_nNumStates != pBuild->pModuleList->GetCount())
				{

					m_nNumStates = 0;
					delete[] m_pRunStates;
					m_pRunStates = NULL;
					SetupDefaultRunStates();
				}
			}
		}

		// a funky state when we first start.
		RemoveGlobalRenderFlags(RF_ALL);
		AddGlobalRenderFlags(
			m_Header.m_nRenderFlags&(RF_TEXTURED|RF_SHADED|RF_SOLID|RF_LINE|RF_REALTIME3D|
				RF_NOENTNAMES|RF_NOENTITYFILL|RF_NOBILINEARFILTER|RF_NO3DSELOUTLINE|
				RF_NO3DVIEW|RF_NO3DSELBOX));
		AddGlobalRenderFlags(RF_NO3DSELBOX);

		// Rebind the 3D view if bilinear is on.
		if((GetGlobalRenderFlags()&RF_NOBILINEARFILTER)==0)
		{
			Debug("bilinear filtering detected, execing rebind...\n");
			this->RebindViews(VIEW_3D);
		}

		if(m_Header.m_nVersion > _2X_HEADER_VERSION_8)
			m_bBrushHandles = m_Header.m_bBrushHandles;
		if(m_Header.m_nVersion > _2X_HEADER_VERSION_9)
			m_bDrawTargets = m_Header.m_bDrawTargets;
		if(m_Header.m_nVersion > _2X_HEADER_VERSION_10)
			m_bUpdateAllViews = m_Header.m_bUpdateAllViews;
	}
	else
	{
		m_Header.m_sGame = GetGameAPI()->GetCurrentGame()->GetGameName();
		SetupDefaultRunStates();
	}

	m_Header.m_nFlags &= ~T2XHF_IGNORE;


	int nDeleted;
	int nRestarts;
	int nRemoved;

	DWORD nStartTick = timeGetTime();
	DWORD nEndTick;

	Debug("... post init sequence\n");

	// Go through each object and do post load initialization.
	RemoveIALFlags(IALF_ALL);

	int alf;
	CBaseObject* pObject;
	
	nDeleted = nRestarts = nRemoved = 0;
	m_pObjectList->First();
	for(;;)
	{
		pObject = m_pObjectList->GetCurItem();
		if(pObject == NULL)
			break;

		alf = pObject->InitAfterLoad(this);
		switch(alf)
		{
		case IALF_OK:

			m_pObjectList->GetNextItem();
			break;
		
		case IALF_RESTART:
			
			m_pObjectList->First();
			nRestarts++;
			continue;

		case IALF_DELETE:
		case IALF_DELETEQ:

			if(alf == IALF_DELETE)
			{
				Warning("IALF: Object deleted from map due to IALF request.\n");
				Debug("IALF: Object deleted from map due to IALF request.\n");
			}
			m_pObjectList->DeleteItem(LL_CURRENT);
			nDeleted++;
			break;

		case IALF_REMOVE:
		case IALF_REMOVEQ:

			if(alf == IALF_REMOVE)
			{
				Warning("IALF: Object removed from map do to IALF request.\n");
				Debug("IALF: Object removed from map do to IALF request.\n");
			}

			m_pObjectList->RemoveItem(LL_CURRENT);
			nRemoved++;
			break;

		case IALF_FATAL:

			Debug("IALF: a fatal error was encountered during IALF.\n");
			Error("A fatal error was encountered during post load initialization, map will not be loaded.");
			return FALSE;
			break;
		}

		if(alf == IALF_END)
			break;

		if(alf != IALF_DELETE && alf != IALF_DELETEQ)
		{
			if(pObject->PostInitAfterLoad(this, alf) == IALF_FATAL)
			{
				Debug("IALF: a fatal error was encountered during IALF.\n");
				Error("A fatal error was encountered during post load initialization, map will not be loaded.");
				return FALSE;
			}
		}

	}

	Debug("... unchained %d object(s)\n", nRemoved);
	Debug("... deleted %d object(s)\n", nDeleted);
	Debug("... IAL restarts: %d\n", nRestarts);
	
	nEndTick = timeGetTime();
	Debug("... elapsed: %d\n", nEndTick-nStartTick);
	nStartTick = nEndTick;

	Debug("... perforimg ui init\n");

	nRestarts = nDeleted = nRemoved = 0;

	m_pUIList->First();
	for(;;)
	{
		pObject = m_pUIList->GetCurItem();
		if(pObject == NULL)
			break;

		alf = pObject->InitAfterLoad(this);
		switch(alf)
		{
		case IALF_OK:

			m_pUIList->GetNextItem();
			break;

		case IALF_RESTART:
			
			m_pUIList->First();
			nRestarts++;
			continue;

		case IALF_DELETE:
		case IALF_DELETEQ:

			if(alf == IALF_DELETE)
			{
				Warning("IALF: Object deleted from map due to IALF request.\n");
				Debug("IALF: Object deleted from map due to IALF request.\n");
			}
			m_pObjectList->DeleteItem(LL_CURRENT);
			nDeleted++;
			break;

		case IALF_REMOVE:
		case IALF_REMOVEQ:

			if(alf == IALF_REMOVE)
			{
				Warning("IALF: Object removed from map due to IALF request.\n");
				Debug("IALF: Object removed from map due to IALF request.\n");
			}

			m_pObjectList->RemoveItem(LL_CURRENT);
			nRemoved++;
			break;

		case IALF_FATAL:

			Debug("IALF: a fatal error was encountered during IALF.\n");
			Error("A fatal error was encountered during post load initialization, map will not be loaded.");
			return FALSE;
		}

		if(alf != IALF_DELETE && alf != IALF_DELETEQ)
		{
			if(pObject->PostInitAfterLoad(this, alf) == IALF_FATAL)
			{
				Debug("IALF: a fatal error was encountered during IALF.\n");
				Error("A fatal error was encountered during post load initialization, map will not be loaded.");
				return FALSE;
			}
		}
	}	

	if(GetIALFlags()&IALF_NOENTDEF)
	{
		Warning("IALF Sequence: Tread was not able to find definitions for all entities in this map.", true);
		Console("\n");
	}
	if(GetIALFlags()&IALF_NOTEXTURES)
	{
		Warning("IALF Sequence: Tread was not able to find all the textures for this map", true);
		Console("\n");
	}
	if(GetIALFlags()&IALF_NOKEYDEF)
	{
		Warning("IALF Sequence: Tread was not able to find definitions for all keys in this map.", true);
		Console("\n");
	}

	// Init the MRU.
	TextureMRUIAL();

	Debug("... unchained %d object(s)\n", nRemoved);
	Debug("... deleted %d object(s)\n", nDeleted);
	Debug("... IAL restarts: %d\n", nRestarts);

	Debug("init sequence complete, elapsed: %u\n", timeGetTime()-nStartTick);
	Debug("... init texlist\n");

#ifdef _WIN32
	CTextureView* pTextureView = (CTextureView*)GetView(TEXTURE_VIEW);
	pTextureView->LoadList();

	Debug("... init brushlist\n");

	CObjectView* pObjectView = (CObjectView*)GetView(OBJECT_VIEW);
	pObjectView->LoadBrushList();

	Debug("... init visgroups\n");

	CVisgroupView* pVisgroupView = (CVisgroupView*)GetView(VISGROUP_VIEW);
	pVisgroupView->LoadVisgroup();
#endif

	Debug("... refreshing mode status\n");

	GetModeManager()->Set(MODE_POINTER, POINTER_ALL);
	GetModeManager()->RefreshMode();
	GetMapManager()->UpdateInterface();

	return TRUE;
}

BOOL CTread3DDoc::OnSaveDocument(LPCTSTR lpszPathName) 
{
	// TODO: Add your specialized code here and/or call the base class
	m_Header.m_nRenderFlags = GetGlobalRenderFlags();
	m_Header.m_nGridSize = GetGridSize();
	m_Header.m_fScale[0] = GetViewByIndex(0)->GetScale();
	m_Header.m_fScale[1] = GetViewByIndex(1)->GetScale();
	m_Header.m_fScale[2] = GetViewByIndex(2)->GetScale();
	m_Header.m_fScale[3] = GetViewByIndex(3)->GetScale();
	m_Header.m_fX[0] = GetViewByIndex(0)->m_vMapCenter[0];
	m_Header.m_fY[0] = GetViewByIndex(0)->m_vMapCenter[1];
	m_Header.m_fX[1] = GetViewByIndex(1)->m_vMapCenter[0];
	m_Header.m_fY[1] = GetViewByIndex(1)->m_vMapCenter[1];
	m_Header.m_fX[2] = GetViewByIndex(2)->m_vMapCenter[0];
	m_Header.m_fY[2] = GetViewByIndex(2)->m_vMapCenter[1];
	m_Header.m_fX[3] = GetViewByIndex(3)->m_vMapCenter[0];
	m_Header.m_fY[3] = GetViewByIndex(3)->m_vMapCenter[1];
	m_Header.m_nViewTypes[0] = GetViewByIndex(0)->m_nView;
	m_Header.m_nViewTypes[1] = GetViewByIndex(1)->m_nView;
	m_Header.m_nViewTypes[2] = GetViewByIndex(2)->m_nView;
	m_Header.m_nViewTypes[3] = GetViewByIndex(3)->m_nView;
	m_Header.m_nNumStates = m_nNumStates;
	m_Header.m_pRunStates = m_pRunStates;
	m_Header.m_fHollowDepth = m_fHollowDepth;
	m_Header.m_nCurveSubDepth = m_nCurveSubDepth;
	m_Header.m_bBrushHandles = m_bBrushHandles;
	m_Header.m_bDrawTargets = m_bDrawTargets;
	m_Header.m_bUpdateAllViews = m_bUpdateAllViews;

#ifdef _WIN32
	return CDocument::OnSaveDocument(lpszPathName);
#else
	return true;
#endif
}

CMode* CTread3DDoc::GetModePtr()
{
	return &m_Mode;
}

TREAD2XHEADERINFO* CTread3DDoc::GetTread2XHeader()
{
	return &m_Header;
}

#ifdef _WIN32
void CTread3DDoc::OnBuildModel() 
{
	// TODO: Add your command handler code here

	CModelDialog dlgTemp;
	if(dlgTemp.DoModal() == IDCANCEL)
		return;

	MODEL* model = NULL;
	CString sName = dlgTemp.GetName();
	CString sCategory = dlgTemp.GetCategory();
	int nType = dlgTemp.GetType();

	model = new MODEL();
	if(nType == 0)
		model->m_Header.m_nFlags = T2XHF_BRUSH;
	else
		model->m_Header.m_nFlags = T2XHF_MODEL;

	model->m_Header.m_sName = sName;
	model->m_Header.m_sCategory = sCategory;

	model->m_pObjectList = new CLinkedList<CBaseObject>();
	G_CopyList(m_pSelectedObjectList, model->m_pObjectList);
	GetGameAPI()->GetCurrentGame()->SaveModel(model);
	CLinkedList<MODEL>* pList = GetGameAPI()->GetCurrentGame()->GetModelList();
	pList->AddItem(model);
	//model.m_pObjectList = NULL;
	
	CObjectView* pView = (CObjectView*)GetView(OBJECT_VIEW);
	pView->ResetContent();
	pView->LoadBrushList();
}

void CTread3DDoc::OnMapProperties() 
{
	CMapDialog dlgTemp;
	dlgTemp.SetStrings(m_Header.m_sAuthor, m_Header.m_sComments);
	if(dlgTemp.DoModal() == IDCANCEL)
		return;

	m_Header.m_sAuthor = dlgTemp.GetAuthor();
	m_Header.m_sComments = dlgTemp.GetComment();
}



void CTread3DDoc::OnUpdateBuildModel(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(GetSelectedObjectCount() > 0);
}

BOOL CTread3DDoc::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
	if (pHandlerInfo == NULL)
	{
		if((nID >= ID_PLUGIN_MIN) && (nID < ID_PLUGIN_MAX))
		{
			if (nCode == CN_COMMAND)
			{
				// Handle WM_COMMAND message
				CallPluginCommand(nID);
				return true;
			}
			else if (nCode == CN_UPDATE_COMMAND_UI)
			{
				// Update UI element state
				UpdatePluginItem(nID, (CCmdUI*)pExtra);
				return true;
			}
		}
	}
	
	return CDocument::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

void CTread3DDoc::UpdatePluginItem(int nID, CCmdUI* pCmdUI)
{
	GetPluginAPI()->UpdateCommandUI(nID, pCmdUI);
}

void CTread3DDoc::CallPluginCommand(int nID)
{
	Debug("...invoking command id (%d)\n", nID);
	GetPluginAPI()->InvokePlugin(nID, this);
}

#endif