// UNDOAPI.H
// Copyright © 1999 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#ifndef UNDOAPI_H
#define UNDOAPI_H

#include "MapObject.h"
#include "TExport.h"

class CTread3DDoc;

//#define UAF_SELECT		0x00000001	// Useless Flag!
// $sb "undoapi.h block1"
#define UAF_GMEM		0x00000002	// Group member
#define UAF_HEAD		0x00000004	// Head items are always empty.
#define UAF_TAIL		0x00000008
#define UAF_TRANSLATE	0x00000010
#define UAF_ROTATE		0x00000020
#define UAF_CARVE		0x00000040
#define UAF_DELETE		0x00000080
#define UAF_CREATE		0x00000100
#define UAF_SIZE		0x00000200
#define UAF_GROUP		0x00000400
#define UAF_UNGROUP		0x00000800
#define UAF_HOLLOW		0x00001000
#define UAF_LINK		0x00002000
#define UAF_UNLINK		0x00004000

struct UNDOREF
{
	UNDOREF* prev, *next;
	CBaseObject* pObj;
};

struct UNDOITEM
{
	UNDOITEM* prev, *next;

	int nFlags;
	int nCount;
	int nURF;
	vec3_t vUVals;
	vec3_t vUOrigin;
	vec3_t vRVals;
	vec3_t vROrigin;
	void* pData;
	void* pData2;
	CLinkedList<UNDOREF> RefList;
};

#define URF_FAIL			-1
#define URF_OK				0x00000000
#define URF_CLEARSEL		0x00000001
#define URF_ADDTOSEL		0x00000002
#define URF_BUILDSELBOX		0x00000004
#define URF_BUILDHOTSPOTS	0x00000008	

class _T_EXPORT CUndoAPI
{
private:

	bool m_bEOL;
	UNDOITEM* m_pStackPos;
	CLinkedList<UNDOITEM> m_UndoList;
	void AddItemToSelection(UNDOITEM* pItem, CTread3DDoc* pDocument);
	void AddItemToMap(UNDOITEM* pItem, CTread3DDoc* pDocument);
	void RemoveItemFromMap(UNDOITEM* pItem, CTread3DDoc* pDocument);
	void PurgeLastItem(CTread3DDoc* pDocument);
	void PurgeFromPos(CTread3DDoc* pDocument);
	void PurgePP(CTread3DDoc* pDocument, int nCount);
	void PurgeBF(CTread3DDoc* pDocument, int nCount);
	void Translate(UNDOITEM* pItem, vec3_t vVec);
	void SetSize(UNDOITEM* pItem, vec3_t vOldSize, vec3_t vNewSize, vec3_t vOrigin);
	void Scale(UNDOITEM* pItem, vec3_t vScale, vec3_t vOrigin);
	void SetOrigin(UNDOITEM* pItem, vec3_t vOldOrigin, vec3_t vNewOrigin);
	void Rotate(UNDOITEM* pItem, vec3_t vRot, vec3_t vOrigin);
	UNDOITEM* GetRedoItem();
	int PerformUndoRedo(UNDOITEM* pItem, CTread3DDoc* pDocument, bool bUndo = true);

public:

	CUndoAPI();

	void ClearUndoStack(CTread3DDoc* pDocument);
	void AddUndoItem(UNDOITEM* pItem, CTread3DDoc* pDocument);
	int Undo(CTread3DDoc* pDocument);
	int Redo(CTread3DDoc* pDocument);
	bool CanUndo();
	bool CanRedo();
	CString GetUndoString();
	CString GetRedoString();

	static UNDOITEM* CreateUndoItem(CLinkedList<CBaseObject>* pList);
};
// $eb
#endif