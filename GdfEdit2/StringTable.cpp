// STRINGTABLE.CPP
// Copyright © 1999 Joe Riedel, Nick Randal.
// Author: Nick Randal.

#include "stdafx.h"
#include "StringTable.h"

#define TABLE_SPAN 20000

CStringTable m_StringTable;
CStringTable* GetStringTable() { return &m_StringTable; }

CStringTable::CStringTable()
{
	for(int n = 0; n < 27; n++)
		m_nCount[n] = 0;
	m_nDuplicate = 0;
}

CStringTable::~CStringTable()
{
	for(int n = 0; n < 27; n++)
	{
		m_nCount[n] = 0;
		m_Table[n].DestroyList();
	}
}

int CStringTable::AddItem(const CString &sInput)
{
	TABLEENTRY* pNode = NULL;
	int n;
	int k;

	if(sInput.IsEmpty())
		return -1;

	k = sInput.GetAt(0);

	if((k > 64) && (k < 91))
		k = k - 65;
	else if((k > 96) && (k < 123))
		k = k - 97;
	else 
		k = 26;
	
	for(n = 0; n < m_nCount[k]; n++)
	{
		pNode = m_Table[k].GetItem(n);
		if(pNode == NULL)
			return -1;	//failed linked list is corrupt

		if(pNode->sEntry == sInput)
		{
			m_nDuplicate++;
			//return n;
			break;
		}
		else
			pNode = NULL;
	}

	//new entry
	if(pNode == NULL)
	{
		pNode = new TABLEENTRY;
		if(pNode == NULL)
			return -1;

		pNode->sEntry = sInput;

		m_Table[k].AddItem(pNode, LL_TAIL);
		 n = m_nCount[k];
		 m_nCount[k]++;
	}

	return (k * TABLE_SPAN) + n;
}

bool CStringTable::GetItem(int nRef, CString &sOutput)
{
	TABLEENTRY* pNode = m_Table[nRef / TABLE_SPAN].GetItem(nRef % TABLE_SPAN);

	if(pNode != NULL)
	{
		sOutput = pNode->sEntry;
		return true;
	}
	else
		return false;
}

int CStringTable::FindItem(const CString &sInput)
{
	int n;
	int k;
	TABLEENTRY* pNode = NULL;

	k = sInput.GetAt(0);

	if((k > 64) && (k < 91))
		k = k - 65;
	else if((k > 96) && (k < 123))
		k = k - 97;
	else 
		k = 26;
	
	for(n = 0; n < m_nCount[k]; n++)
	{
		pNode = m_Table[k].GetItem(n);
		if(pNode == NULL)
			return -1;	//failed linked list is corrupt

		if(pNode->sEntry == sInput)
			return (k * TABLE_SPAN) + n;
	}

	return -1;
}

void CStringTable::ClearTable()
{
	for(int n = 0; n < 27; n++)
	{
		m_nCount[n] = 0;
		m_Table[n].DestroyList();
	}
	m_nDuplicate = 0;
}

int CStringTable::GetCount()
{
	int k = 0;
	for(int n = 0; n < 27; n++)
		k += m_nCount[n];

	return k;
}

int CStringTable::GetDuplicateCount()
{
	return m_nDuplicate;
}

bool CStringTable::IsEmpty()
{
	int k = 0;
	for(int n = 0; n < 27; n++)
		k += m_nCount[n];

	if(k <= 0)
		return true;
	else
		return false;
}

