// ENTITYMODEWINRESPONDER.CPP
// Copyright © 1999 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#include "stdafx.h"
#include "EventManager.h"
#include "EntityModeWinResponder.h"
#include "Geometry.h"
#include "MapView.h"
#include "Entity.h"
#include "GameAPI.h"

CEntityModeWinResponder::CEntityModeWinResponder()
{
	m_nMode = MODE_ENTITY;
	m_nSubmode = ALL_MODES;
}

// Drops an entity in the map.
void CEntityModeWinResponder::OnMouseDown(CBaseWindowEventResponder* pParent, CWindowEventManager* pEventManager, CMapView* pView, CTread3DDoc* pDocument, int nMouseX, int nMouseY, int nButtons)
{
	if(GetGameAPI()->GetCurrentGame()->GetCurrentPointEntity() == NULL)
	{
		Warning("No point entity selected.\n");
		return;
	}

	// Create the entity.
	CEntity* pEntity = (CEntity*)NewBaseObject(OBJECT_CLASS_ENTITY);
	pEntity->InitFromDefinition(GetGameAPI()->GetCurrentGame()->GetCurrentPointEntity());

	float fX, fY, fZ;
	
	fX = WX_TO_MX(nMouseX, pView);
	fY = WY_TO_MY(nMouseY, pView);

	// Setup z.
	{
		CMapView* pOther = pDocument->GetOtherViewType(pView->m_nView);
		if(pOther == NULL)
			fZ = 0.0F;
		else
			fZ = pOther->m_vMapCenter[V2I(VU(pView), pOther)];
	}

	// Snap if necessary.
	if(pDocument->GetGridSnap())
	{
		fX = G_SnapFloat(fX, pDocument->GetGridSize());
		fY = G_SnapFloat(fY, pDocument->GetGridSize());
		fZ = G_SnapFloat(fZ, pDocument->GetGridSize());
	}

	// Store position.
	pEntity->m_vOrigin[VI(VEC_X, pView)] = fX;
	pEntity->m_vOrigin[VI(VEC_Y, pView)] = fY;
	pEntity->m_vOrigin[VI(VEC_Z, pView)] = fZ;

	pDocument->AddObjectToMap(pEntity);
	pDocument->UpdateWindows();
}
