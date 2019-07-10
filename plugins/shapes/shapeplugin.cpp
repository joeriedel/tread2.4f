// SHAPEPLUGIN.CPP
// Copyright © 2000 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#include "stdafx.h"
#include "resource.h"
#include "../TreadAPI.h"
#include "ShapePlugin.h"
#include "ShapeCreator.h"

T3DPLUGINAPI CTreadPlugin* T3D_CreatePluginInstance()
{ 
	return new CShapesPlugin();
}

CShapesPlugin::CShapesPlugin()
{}

CShapesPlugin::~CShapesPlugin()
{}

int CShapesPlugin::GetAPIVersion()
{ return TP_API_VERSION; }

int CShapesPlugin::InitializePlugin()
{
	// Add the shape options.
	m_hArch = GetPluginAPI()->AddMenuItem("Shapes\nMake Arch...", this);
	m_hCone = GetPluginAPI()->AddMenuItem("Shapes\nMake Cone...", this);
	m_hCylinder = GetPluginAPI()->AddMenuItem("Shapes\nMake Cylinder...", this);

	return TPF_OK;
}
int CShapesPlugin::ShutdownPlugin()
{ return TPF_OK; }

CString CShapesPlugin::GetPluginName()
{ return "ShapeCreator"; }

int CShapesPlugin::InvokePlugin(CTread3DDoc* pDocument, TPHMENUITEM nMenuID)
{
	if(nMenuID == m_hArch)
		MakeArch();
	else
	if(nMenuID == m_hCone)
		MakeCone();
	else
	if(nMenuID == m_hCylinder)
		MakeCylinder();

	return TPF_OK;
}

void CShapesPlugin::MakeArch()
{
	if(m_ArchDialog.DoModal() == IDCANCEL)
		return;

	// TODO: Add your command handler code here
	CObjectGroup* pGrp = CShapeCreator::CreateArch(m_ArchDialog.m_nAxis, m_ArchDialog.m_nNumSections, m_ArchDialog.m_nSize/2, m_ArchDialog.m_nWidth, m_ArchDialog.m_nDepth);
	if(!pGrp)
		return;

	// Get where the object should be centered.
	vec3_t vCenter;
	CMapView* pView = GetMapManager()->GetDocPtr()->GetCurView();
	if(pView->m_nView == VIEW_3D)
		pView = GetMapManager()->GetDocPtr()->GetViewByType(VIEW_TOP);

	CMapView* pOther = GetMapManager()->GetDocPtr()->GetOtherViewType(pView->m_nView);

	vCenter[VI(VEC_X, pView)] = pView->m_vMapCenter[0];
	vCenter[VI(VEC_Y, pView)] = pView->m_vMapCenter[1];

	int a = VI(VEC_Z, pView);
	int b = V2I(VU(pView), pOther);
	vCenter[VI(VEC_Z, pView)] = pOther->m_vMapCenter[V2I(VU(pView), pOther)];

	pGrp->Translate(vCenter);

	GetMapManager()->GetDocPtr()->ClearSelection();
	GetMapManager()->GetDocPtr()->AddObjectToSelection((CBaseObject*)pGrp);
	GetMapManager()->GetDocPtr()->BuildSelectionBBox();
	GetMapManager()->GetDocPtr()->BuildHotSpots();
	if(GetModeManager()->IsMode(MODE_POINTER))
		GetMapManager()->GetDocPtr()->RemoveGlobalRenderFlags(RF_NOSELBOX);
	GetMapManager()->GetDocPtr()->UpdateWindows();
}

void CShapesPlugin::MakeCone()
{
	if(m_ConeDialog.DoModal() == IDCANCEL)
		return;

	// TODO: Add your command handler code here
	CQBrush* pCone = CShapeCreator::CreateCone(m_ConeDialog.m_nAxis, m_ConeDialog.m_nNumSides, m_ConeDialog.m_nSize/2, m_ConeDialog.m_nLength);
	if(!pCone)
		return;

	// Get where the object should be centered.
	vec3_t vCenter;
	CMapView* pView = GetMapManager()->GetDocPtr()->GetCurView();
	if(pView->m_nView == VIEW_3D)
		pView = GetMapManager()->GetDocPtr()->GetViewByType(VIEW_TOP);

	CMapView* pOther = GetMapManager()->GetDocPtr()->GetOtherViewType(pView->m_nView);

	vCenter[VI(VEC_X, pView)] = pView->m_vMapCenter[0];
	vCenter[VI(VEC_Y, pView)] = pView->m_vMapCenter[1];
	vCenter[VI(VEC_Z, pView)] = pOther->m_vMapCenter[V2I(VU(pView), pOther)];

	pCone->Translate(vCenter);

	GetMapManager()->GetDocPtr()->ClearSelection();
	GetMapManager()->GetDocPtr()->AddObjectToSelection((CBaseObject*)pCone);
	GetMapManager()->GetDocPtr()->BuildSelectionBBox();
	GetMapManager()->GetDocPtr()->BuildHotSpots();
	if(GetModeManager()->IsMode(MODE_POINTER))
		GetMapManager()->GetDocPtr()->RemoveGlobalRenderFlags(RF_NOSELBOX);
	GetMapManager()->GetDocPtr()->UpdateWindows();
}

void CShapesPlugin::MakeCylinder()
{
	if(m_CylinderDialog.DoModal() == IDCANCEL)
		return;

	// TODO: Add your command handler code here
	CQBrush* pCylinder = CShapeCreator::CreateCylinder(m_CylinderDialog.m_nAxis, m_CylinderDialog.m_nNumSides, m_CylinderDialog.m_nSize/2, m_CylinderDialog.m_nDepth);
	if(!pCylinder)
		return;

	// Get where the object should be centered.
	vec3_t vCenter;
	CMapView* pView = GetMapManager()->GetDocPtr()->GetCurView();
	if(pView->m_nView == VIEW_3D)
		pView = GetMapManager()->GetDocPtr()->GetViewByType(VIEW_TOP);

	CMapView* pOther = GetMapManager()->GetDocPtr()->GetOtherViewType(pView->m_nView);

	vCenter[VI(VEC_X, pView)] = pView->m_vMapCenter[0];
	vCenter[VI(VEC_Y, pView)] = pView->m_vMapCenter[1];
	vCenter[VI(VEC_Z, pView)] = pOther->m_vMapCenter[V2I(VU(pView), pOther)];

	pCylinder->Translate(vCenter);

	GetMapManager()->GetDocPtr()->ClearSelection();
	GetMapManager()->GetDocPtr()->AddObjectToSelection((CBaseObject*)pCylinder);
	GetMapManager()->GetDocPtr()->BuildSelectionBBox();
	GetMapManager()->GetDocPtr()->BuildHotSpots();
	if(GetModeManager()->IsMode(MODE_POINTER))
		GetMapManager()->GetDocPtr()->RemoveGlobalRenderFlags(RF_NOSELBOX);
	GetMapManager()->GetDocPtr()->UpdateWindows();
}
