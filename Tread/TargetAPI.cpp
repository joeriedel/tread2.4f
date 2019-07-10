// TARGETAPI.CPP
// Copyright © 1999 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#include "stdafx.h"
#include "MapObject.h"
#include "TargetAPI.h"

CString CTargetAPI::GenerateTargetName()
{
	int i;
	CString sBase = "Object", sString;
	char szBuff[256];

	// NOTE: Could loop endlessly, but I don't care.
	for(i = 1;;i++)
	{
		itoa(i, szBuff, 10);
		sString = sBase + szBuff;
		if(!FindTargetSet(sString))
			break;
	}

	return sString;
}

TTARGET* CTargetAPI::FindTargetSet(CString sName)
{
	TTARGET* pSet;
	
	m_TargetList.First();
	for(pSet = m_TargetList.GetCurItem(); pSet != NULL; pSet = m_TargetList.GetNextItem())
		if(pSet->sName == sName)
			return pSet;

	return NULL;
}

TREF* CTargetAPI::FindTRef(TTARGET* pTarget, CBaseObject* pObj)
{
	TREF* pRef;
	pTarget->TRefList.First();
	for(pRef = pTarget->TRefList.GetCurItem(); pRef != NULL; pRef = pTarget->TRefList.GetNextItem())
		if(pRef->pObject == pObj)
			return pRef;

	return false;
}

void CTargetAPI::SetTargetBinding(CString sTarget, CBaseObject* pObj)
{
	// Exists in a set?
	CBaseTargetObject* pT = (CBaseTargetObject*)pObj->GetInterface(I_TARGET);
	TTARGET* pSet = FindTargetSet(pT->GetTargetableName());
	if(pSet != NULL)
	{
		// Remove TREF from set.
		TREF* pRef = FindTRef(pSet, pObj);
		if(pRef != NULL)
			pSet->TRefList.DeleteItem(pRef);
		if(pSet->TRefList.IsEmpty())
			// Remove the set.
			m_TargetList.DeleteItem(pSet);

	}

	if(sTarget == "")
		return;

	// Set exists?
	pSet = FindTargetSet(sTarget);
	if(pSet == NULL)
	{
		// Add one.
		pSet = new TTARGET();
		pSet->sName = sTarget;
		m_TargetList.AddItem(pSet);
	}

	// Add a TREF.
	TREF* pRef = new TREF();
	pRef->pObject = pObj;
	pSet->TRefList.AddItem(pRef);

	// We now have a TREF setup and the targetting is complete.
}
