// TARGETAPI.H
// Copyright © 1999 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#ifndef TARGETAPI_H
#define TARGETAPI_H

#include "MapObject.h"
#include "LinkedList.h"
#include "TExport.h"

// $sb "targetapi.h block1"
struct TREF
{
	TREF* prev, *next;
	CBaseObject* pObject;
};

struct TTARGET
{
	TTARGET* prev, *next;

	CString sName;
	CLinkedList<TREF> TRefList;
};

class _T_EXPORT CTargetAPI
{
	CLinkedList<TTARGET> m_TargetList;

	TREF* FindTRef(TTARGET* pTarget, CBaseObject* pObj);

public:

	// "" == Remove binding.
	void SetTargetBinding(CString sTarget, CBaseObject* pObj);
	TTARGET* FindTargetSet(CString sName);
	CString GenerateTargetName();
};

// $eb

#endif