// HOTSPOTMANAGER.H
// Copyright © 1999 Joe Riedel.
// Author: Joe Riedel.
#ifndef HOTSPOTMANAGER_H
#define HOTSPOTMANAGER_H

#include "MathLib.h"
#include "MapObject.h"
#include "EventManager.h"

class CMapView;

// $sb "hotspotmanager.h block1"
#define HSDF_NODRAW			0x00000001
#define HSDF_ROUND			0x00000002
#define HSDF_SQUARE			0x00000004
#define HSDF_DIAMOND		0x00000008
#define HSDF_CROSS			0x00000010
#define HSDF_LINE			0x00000020
#define HSDF_SEL			0x00000040
#define HSDF_HD_COLOR		0x00000080

#define HSDR_VERT		0x00000001
#define HSDR_HORZ		0x00000002
#define HSDR_NONE		(HSDR_VERT|HSDR_HORZ)

class CBaseHotSpotEventResponder;
class _T_EXPORT CHotSpot : public CBaseObject
{
public:
	
	CHotSpot();
	virtual ~CHotSpot();

	virtual CBaseObject* Clone() { return new CHotSpot(*this); }

	void CopyToLXY();
	void ComputeMapXY(CMapView* pView);
	void ComputeWindowXY(CMapView* pView);

	int m_nClass;
	int m_nCode;
	int m_nCursor;
	int m_nDrawFlags;
	int m_nDragRestrictions;
	int m_nFlags;

	float m_fMX, m_fMY;	// Map X/Y.
	float m_fLX, m_fLY;
	int m_nWX, m_nWY;	// Window X/Y.

	int m_nDataFlags;
	void* m_pData;
	vec3_t m_vPt;
	CHotSpot* m_pLineSpot;
	CBaseObject* m_pObject;

	CBaseHotSpotEventResponder* m_pEventResponder;
};

class CHotSpotManager;
// Generates hotspots.
class _T_EXPORT CBaseHotSpotGenerator
{
public:

	CBaseHotSpotGenerator() {}
	virtual ~CBaseHotSpotGenerator() {}
	
	virtual void GenerateHotSpots(CHotSpotManager* pHotSpotManager, CTread3DDoc* pDocument, CMapView* pView) {}
};

class _T_EXPORT CBaseHotSpotModeGenerator : public CLLObject
{

public:
	
	CBaseHotSpotModeGenerator() {}
	virtual ~CBaseHotSpotModeGenerator() {}

	int m_nMode;
	int m_nSubmode;

	virtual void GenerateHotSpots(CHotSpotManager* pHotSpotManager, CTread3DDoc* pDocument, CMapView* pView) {}
};


#define TS_NONE		0x00000000
#define TS_TRACKING	0x00000001
#define TS_DRAGGING	0x00000002
#define TS_BBOX		0x00000003

#define ISF_ALL		0xFFFFFFFF
#define ISF_NONE	0x00000000
#define ISF_WINX	0x00000001
#define ISF_WINY	0x00000002
#define ISF_PTX		0x00000004
#define ISF_PTY		0x00000008
#define	ISF_PTZ		0x00000010
#define ISF_CLASS	0x00000020
#define ISF_MASK	0x00000040
#define ISF_MASK1	0x00000080
#define ISF_MASK2	0x00000100
#define ISF_MASK3	0x00000200
#define ISF_MASK4	0x00000400
#define ISF_MASKS	(ISF_MASK1|ISF_MASK2|ISF_MASK3|ISF_MASK4)
#define ISF_CODE	0x00000800

struct HSPOTREF
{
	HSPOTREF* prev, *next;
	CHotSpot* pSpot;
};

class CRenderAPI;
class _T_EXPORT CHotSpotManager : public CBaseWindowEventResponder
{
private:

	int m_nMouseX, m_nMouseY, m_nButtons;
	int m_nOldX, m_nOldY;
	int m_nTrackState;
	int m_nOldRenderFlags;
	bool m_bGetFromSave;
	bool m_bMulti;
	CHotSpot* m_pTrackSpot, *m_pIdentSpot;
	CLinkedList<CHotSpot>* m_pHotSpotList;
	CLinkedList<HSPOTREF>* m_pStoreList;
	CLinkedList<HSPOTREF>* m_pSelList;
	CLinkedList<CBaseHotSpotModeGenerator>* m_pGeneratorList;

	bool OverSpot(CHotSpot* pSpot, int nMouseX, int nMouseY);
	bool FindTrackSpot(CWindowEventManager* pManager, CMapView* pView, CTread3DDoc* pDocument, int nMouseX, int nMouseY, int nButtons);

public:

	friend class CRenderAPI;

	CHotSpotManager();
	virtual ~CHotSpotManager();

	void ClearGeneratorList() { m_pGeneratorList->DestroyList(); }
	void AddModeGenerator(CBaseHotSpotModeGenerator* pHG);

	void AddHotSpot(CHotSpot* pSpot);
	void DeleteHotSpots();

	void SetTrackSpot(CHotSpot* pTrackSpot);
	void SetDragSpot(CHotSpot* pDragSpot, int nMouseX, int nMouseY, int nButtons, CTread3DDoc* pDocument, CMapView* pView, CWindowEventManager* pEventManager);

	// Returns true if spot found.
	bool Track(CWindowEventManager* pEventManager, CMapView* pView, CTread3DDoc* pDocument, int nMouseX, int nMouseY, int nButtons);

	void BuildHotSpots(CTread3DDoc* pDocument, CMapView* pView);

	bool EnableBBoxSelection(bool bEnable = true);
	bool EnableMultipleSelection(bool bEnable = true);

	void ClearSelectedHotSpots();
	void AddHotSpotToSelection(CHotSpot* pSpot);
	void RemoveHotSpotFromSelection(CHotSpot* pSpot);

	void InitIdenticalSearch();
	void SaveIdenticalSearch(CHotSpot* pHotSpot, CMapView* pView, int nFlags);
	void RestoreIdenticalSearch();
	CHotSpot* GetNextIdenticalHotSpot(CHotSpot* pHotSpot, CMapView* pView, int nFlags);

	bool BeginBBoxSelection(CBaseWindowEventResponder* pParent, CWindowEventManager* pEventManager, CMapView* pView, CTread3DDoc* pDocument, int nMouseX, int nMouseY, int nButtons);

	virtual void OnMouseMove(CBaseWindowEventResponder* pParent, CWindowEventManager* pEventManager, CMapView* pView, CTread3DDoc* pDocument, int nMouseX, int nMouseY, int nButtons);
	virtual void OnMouseDown(CBaseWindowEventResponder* pParent, CWindowEventManager* pEventManager, CMapView* pView, CTread3DDoc* pDocument, int nMouseX, int nMouseY, int nButtons);
	virtual void OnMouseUp(CBaseWindowEventResponder* pParent, CWindowEventManager* pEventManager, CMapView* pView, CTread3DDoc* pDocument, int nMouseX, int nMouseY, int nButtons);
	virtual void OnKeyDown(CBaseWindowEventResponder* pParent, CWindowEventManager* pEventManager, CMapView* pView, CTread3DDoc* pDocument, int nKeyCode);
};


#define HEF_NONE	0x00000000
#define HEF_GROUP	0x00000001
#define HEF_HEAD	0x00000002
#define HEF_TAIL	0x00000004
#define HEF_MULTI	0x00000008

// A hotspot event trapper.
class _T_EXPORT CBaseHotSpotEventResponder
{
public:
	
	CBaseHotSpotEventResponder() {}
	virtual ~CBaseHotSpotEventResponder() {}

	virtual void OnHotSpotDrag(CWindowEventManager* pEventManager, CHotSpotManager* pHotSpotManager, CHotSpot* pHotSpot, CMapView* pView, CTread3DDoc* pDocument, int nMouseX, int nMouseY, int nButtons, int nFlags) {}
	virtual void OnHotSpotMouseUp(CWindowEventManager* pEventManager, CHotSpotManager* pHotSpotManager, CHotSpot* pHotSpot, CMapView* pView, CTread3DDoc* pDocument, int nMouseX, int nMouseY, int nButtons, int nFlags) {}
	virtual void OnHotSpotMouseDown(CWindowEventManager* pEventManager, CHotSpotManager* pHotSpotManager, CHotSpot* pHotSpot, CMapView* pView, CTread3DDoc* pDocument, int nMouseX, int nMouseY, int nButtons, int nFlags) {}
	virtual void OnHotSpotMouseEnter(CWindowEventManager* pEventManager, CHotSpotManager* pHotSpotManager, CHotSpot* pHotSpot, CMapView* pView, CTread3DDoc* pDocument, int nMouseX, int nMouseY, int nButtons, int nFlags) {}
	virtual void OnHotSpotMouseLeave(CWindowEventManager* pEventManager, CHotSpotManager* pHotSpotManager, CHotSpot* pHotSpot, CMapView* pView, CTread3DDoc* pDocument, int nMouseX, int nMouseY, int nButtons, int nFlags) {}
};

// $eb
#endif