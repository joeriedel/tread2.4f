// VISGROUP.CPP
// Copyright © 1999 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#include "stdafx.h"
#include "LinkedList.h"
#include "TreadSys.h"
#include "MapObject.h"
#include "Visgroup.h"
#include "Tread3DDoc.h"
#include "ChildFrm.h"
#include "futils.h"

bool CVisgroupAPI::WriteVisgroups(CFile& hFile, int nVersion)
{
	CVisgroup* pV;
	int nCount = m_VisgroupList.GetCount();

	WriteInt(hFile, nCount);
	if(nCount == 0)
		return true;

	m_VisgroupList.First();
	for(pV = m_VisgroupList.GetCurItem(); pV != NULL; pV = m_VisgroupList.GetNextItem())
	{
		if(!pV->Write(hFile, nVersion))
			return false;
	}

	return true;
}

bool CVisgroupAPI::ReadVisgroups(CFile& hFile, int nVersion)
{
	int i, nCount;

	ReadInt(hFile, nCount);
	if(nCount == 0)
		return true;

	CVisgroup* pV;
	for(i = 0; i < nCount; i++)
	{
		pV = new CVisgroup();
		if(!pV->Read(hFile, nVersion))
			return false;
		AddVisgroup(pV);
	}

	return true;
}

void CVisgroupAPI::AddVisgroup(CVisgroup* pVisgroup)
{
	m_VisgroupList.AddItem(pVisgroup);
}

// Reads a visgroup from disk
bool CVisgroup::Read(CFile& hFile, int nVersion)
{
	int nLen;
	hFile.Read(&m_bVisible, 4);
	ReadInt(hFile, nLen);
	if(nLen > 255)
		return false;
	char szName[256];
	if(nLen > 0)
	{
		hFile.Read(szName, nLen);
		szName[nLen] = 0;
		m_sName = szName;
	}
	else
		m_sName = "";

	ReadInt(hFile, nLen);
	if(nLen > 255)
		return false;
	if(nLen > 0)
	{
		hFile.Read(szName, nLen);
		szName[nLen] = 0;
		m_sCategory = szName;
	}
	else
		m_sCategory = "";

	return true;
}

// Writes a visgroup to disk.
bool CVisgroup::Write(CFile& hFile, int nVersion)
{
	hFile.Write(&m_bVisible, 4);
	int nLen = m_sName.GetLength();
	if(nLen > 0)
	{
		WriteInt(hFile, nLen);
		hFile.Write((LPCTSTR)m_sName, nLen);
	}
	nLen = m_sCategory.GetLength();
	if(nLen > 0)
	{
		WriteInt(hFile, nLen);
		hFile.Write((LPCTSTR)m_sCategory, nLen);
	}

	return true;
}

// Get's the current visgroup API.
CVisgroupAPI* GetVisgroupAPI()
{
	if(GetMapManager()->GetMapCount() == 0)
		return NULL;

	if(GetMapManager()->GetCurrentMap() == NULL)
		return NULL;

	CTread3DDoc* pDoc = GetMapManager()->GetCurrentMap()->GetDocument();
	if(pDoc == NULL)
		return NULL;

	return pDoc->GetVisgroupAPI();
}

// Adds a visgroup.
CVisgroup* CVisgroupAPI::AddVisgroup(CString sName, CString sCategory)
{
	CVisgroup* pVisgroup;

	pVisgroup = FindVisgroup(sName, sCategory);
	if(pVisgroup)
		return pVisgroup;

	pVisgroup = new CVisgroup(sName, sCategory);
	if(pVisgroup == NULL)
	{
		Warning("CVisgroupAPI::AddVisgroup: unable to allocate memory for visgroup\n");
		return NULL;
	}

	m_VisgroupList.AddItem(pVisgroup);
	return pVisgroup;
}

// Delete's a visgroup.
bool CVisgroupAPI::DeleteVisgroup(CString sName, CString sCategory)
{
	CVisgroup* pVisgroup;

	pVisgroup = FindVisgroup(sName, sCategory);
	if(pVisgroup == NULL)
		return false;

	// We're wiping this one clean. NULL all references.
	UpdateReferences(sName, sCategory, NULL);

	m_VisgroupList.DeleteItem(pVisgroup);
	return true;
}

bool CVisgroupAPI::DeleteVisgroup(CVisgroup* pVisgroup)
{
	UpdateReferences(pVisgroup->GetName(), pVisgroup->GetCategory(), NULL);
	m_VisgroupList.DeleteItem(pVisgroup);
	return true;
}

bool CVisgroupAPI::CombineVisgroups(CVisgroup* pDstGroup, CVisgroup* pSrcGroup)
{
	UpdateReferences(pSrcGroup->GetName(), pSrcGroup->GetCategory(), pDstGroup);
	m_VisgroupList.DeleteItem(pSrcGroup);
	return true;
}

// Finds a visgroup in the list.
CVisgroup* CVisgroupAPI::FindVisgroup(CString sName, CString sCategory)
{
	CVisgroup* pVisgroup;

	if(sName == "" || sCategory == "")
		return NULL;

	m_VisgroupList.First();
	for(pVisgroup = m_VisgroupList.GetCurItem(); pVisgroup != NULL; pVisgroup = m_VisgroupList.GetNextItem())
	{
		if(pVisgroup->m_sName == sName &&
			pVisgroup->m_sCategory == sCategory)
				return pVisgroup;
	}

	return NULL;
}

// Updates all reference objects.
void CVisgroupAPI::UpdateReferences(CString sName, CString sCategory, CVisgroup* pNewVisgroup)
{
	CVisgroupRef* pVisgroupRef;

	m_VisgroupRefList.First();
	for(pVisgroupRef = m_VisgroupRefList.GetCurItem(); pVisgroupRef != NULL; pVisgroupRef = m_VisgroupRefList.GetNextItem())
	{
		if(pVisgroupRef->GetName() == sName && 
			pVisgroupRef->GetCategory() == sCategory)
		{
			pVisgroupRef->SetVisgroup(pNewVisgroup);
		}
	}
}

// Adds, deletes visgroup refs.
void CVisgroupAPI::AddVisgroupRef(CVisgroupRef* pVisgroupRef)
{
	// Not contained?
	if(m_VisgroupRefList.InList(pVisgroupRef))
		return;

	m_VisgroupRefList.AddItem(pVisgroupRef);
}

void CVisgroupAPI::DeleteVisgroupRef(CVisgroupRef* pVisgroupRef)
{
	if(!m_VisgroupRefList.InList(pVisgroupRef))
		return;

	m_VisgroupRefList.RemoveItem(pVisgroupRef);
}