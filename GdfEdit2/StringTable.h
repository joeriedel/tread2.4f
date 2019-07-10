// STRINGTABLE.H
// Copyright © 1999 Joe Riedel, Nick Randal.
// Author: Nick Randal.

/*
6/21/99, By: Nick Randal
--------------------------------
Increased speed of table by about 95%. Added a find function. 
It now keeps track of the total number of duplicate entries from the 
AddItem function.
*/

#ifndef STRINGTABLE_H
#define STRINGTABLE_H

#include "LinkedList.h"

struct TABLEENTRY;
struct TABLEENTRY
{
	CString sEntry;
	TABLEENTRY* next;
	TABLEENTRY* prev;
};

class CStringTable
{
public:
	CStringTable();
	~CStringTable();

	int AddItem(const CString &sInput);
	bool GetItem(int nPos, CString &sOutput);
	void ClearTable();
	int GetCount();
	int GetDuplicateCount();
	int FindItem(const CString &sInput);
	bool IsEmpty();

	//bool SaveTable(CString sFilename);
	//bool LoadTable(CString sFilename);

private:
	CLinkedList<TABLEENTRY> m_Table[27];
	int m_nCount[27];
	int m_nDuplicate;
};

CStringTable* GetStringTable();

#endif