// TREADMAP.H
// Coyright © 1999 Joe Riedel.
// Author: Joe Riedel.
#ifndef TREADMAP_H
#define TREADMAP_H

////////////////////////////////////////////////////////////////////////////
// Defines the global TreadMap structure that stores everthing involved in 
// the map. References to all necessary things are stored here.
////////////////////////////////////////////////////////////////////////////
#include "LinkedList.h"
#include "GameAPI.h"
#include "MapObject.h"
#include "Entity.h"
#include "MapView.h"

#define TREAD_VIEW_TOP		0x00000001
#define TREAD_VIEW_SIDE		0x00000002
#define TREAD_VIEW_FRONT	0x00000004
#define TREAD_VIEW_3D		0x00000008

class CViewController : public CLLObject
{
private:
	
	int m_nType;
	CMapView* m_pMapView;

public:

	CMapView* GetView() { return m_pMapView; }
	void SetView(CMapView* pMapView) { m_pMapView = pMapView; }

	void SetViewType(int nType) { m_nType = nType; }
	int GetViewType() { return m_nType; }
};

class CTreadMap : virtual public CLLObject
{

private:

	int m_nHandle;
	CGameAPI* m_pGameAPI;	// The Game API Definition.
	CLinkedList<CViewController> m_ViewControllerList;	// List of views.
	CLinkedList<CBaseObject> m_ObjectList;  // List of objects in the map.
	CEntity* m_pWorldspawn;

public:

	CTreadMap();
	virtual ~CTreadMap();

	// Get/Set handle.
	int GetHandle() { return m_nHandle; }
	void SetHandle(int nHandle) { m_nHandle = nHandle; }

	CMapView* GetView(int nViewIndex);	// 0 - 3.
	CGameAPI* GetGameAPI() { return m_pGameAPI; }
	CEntity* GetWorldspawn() { return m_pWorldspawn; }

	// Updating and painting.
	void RedrawView(int nViewIndex);
	// Redraws all views of a certain type.
	void RedrawAllViews(int nViewType);

};

#endif