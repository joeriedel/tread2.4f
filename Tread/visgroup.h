// VISGROUP.H
// Copyright © 1999 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#ifndef VISGROUP_H
#define VISGROUP_H

#include "LinkedList.h"
#include "MapObject.h"
#include "TExport.h"

// A Visgroup.
class _T_EXPORT CVisgroupAPI;
// $sb "visgroup.h block1"
class _T_EXPORT CVisgroup : virtual public CLLObject
{
private:

	CString m_sName;
	CString m_sCategory;
	bool	m_bVisible, m_bPending;

public:

	CVisgroup()
	{
		m_sName = "";
		m_sCategory = "";
		m_bVisible = true;
	}

	CVisgroup(CString sName, CString sCategory, bool bVisible = true)
	{
		m_sName = sName;
		m_sCategory = sCategory;
		m_bVisible = bVisible;
	}

	bool Read(CFile& hFile, int nVersion);
	bool Write(CFile& hFile, int nVersion);

	CString GetName() const { return m_sName; }
	CString GetCategory() const { return m_sCategory; }

	void SetName(CString sName) { m_sName = sName; }
	void SetCategory(CString sCategory) { m_sCategory = sCategory; }
	void SetVisible(bool bVisible = true) { m_bVisible = bVisible; }
	void SetPending(bool bVisible = true) { m_bPending = bVisible; }
	void ApplyPending() { m_bVisible = m_bPending; }
	bool GetVisible() const { return m_bVisible; }
	bool GetPending() const { return m_bPending; }
	
	friend class CVisgroupAPI;
};

// The VisgroupAPI.
// Maintains a list of visgroups, and the references that reference the groups.
class CVisgroupAPI
{
private:

	CLinkedList<CVisgroup> m_VisgroupList;
	CLinkedList<CVisgroupRef> m_VisgroupRefList;

	void UpdateReferences(CString sName, CString sCategory, CVisgroup* pNewVisgroup);

public:

	CVisgroupAPI()
	{
		m_VisgroupRefList.SetAutoDelete(false);
	}

	void AddVisgroup(CVisgroup* pVisgroup);
	CVisgroup* AddVisgroup(CString sName, CString sCategory);
	bool DeleteVisgroup(CString sName, CString sCategory);
	bool DeleteVisgroup(CVisgroup* pVisgroup);
	bool CombineVisgroups(CVisgroup* pDstGroup, CVisgroup* pSrcGroup);
	CVisgroup* FindVisgroup(CString sName, CString sCategory);

	// Add's a reference visgroup.
	void AddVisgroupRef(CVisgroupRef* pVisgroupRef);
	void DeleteVisgroupRef(CVisgroupRef* pVisgroupRef);
	CLinkedList<CVisgroup>* GetVisgroupList() { return &m_VisgroupList; }

	// Reads/Writes all visgroups.
	bool WriteVisgroups(CFile& hFile, int nVersion);
	bool ReadVisgroups(CFile& hFile, int nVersion);
};
// $eb
CVisgroupAPI* GetVisgroupAPI();

#endif