// TREADSYS.H
// Copyright © 1999 Joe Riedel, Nick Randal.
// Author: Joe Riedel, Nick Randal.


//since all of these functions are small and get used often
//should they be made "inline"?

#ifndef MODE_H
#define MODE_H

#include "linkedlist.h"
// $sb "mode.h block1"
struct _T_EXPORT MODE_FLAGS;
struct _T_EXPORT MODE_FLAGS
{
	int nMode;
	int nSubmode;

	MODE_FLAGS* next;
	MODE_FLAGS* prev;
};

class CTread3DDoc;

#define MODES 9

class _T_EXPORT CMode
{
public:
	CMode();
	~CMode();

	void Push();
	void Pop();
	void ClearStack();
	bool IsStackEmpty();

	void FlipLast();
	//void FlipLastMode();
	//void FlipLastSubmode();
	
	void Set(int nMode, int nSubmode, bool bSet = true);
	void SetMode(int nFlag);
	void SetSubmode(int nFlag);
	void SetSubmode(int nFlag, int nMask);
	void AddSubmode(int nFlag);
	void SubtractSubmode(int nFlag);

	void RefreshMode();

	int GetMode();
	int GetSubmode();
	int GetSubmode(int nMask);
	int GetSubmodeFor(int nMode);
	int GetSubmodeFor(int nMode, int nMask);

	bool IsMode(int nMode);
	bool IsSubmode(int nSubmode) ;

	void Default();
	void DefaultMode();
	void DefaultSubmode();

	void ClearFlags();
	void SetFlags(int nFlags);
	void SetFlag(int nFlag, bool bValue);
	void AddFlags(int nFlags);
	void SubtractFlags(int nFlags);
	void ToggleFlag(int nFlags);
	int GetFlags();
	bool IsFlag(int nFlags);

	void SetDocument(CTread3DDoc* pDocument) { m_pDocument = pDocument; }

private:
	int m_nMode;
	int m_nSubmode[MODES];
	int m_nFlags;

	int m_nLastMode;
	int m_nLastSubmode;

	CTread3DDoc* m_pDocument;
	CLinkedList<MODE_FLAGS> m_modeList;
};
// $eb
#endif