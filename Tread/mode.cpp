#include "stdafx.h"
#include "treadSys.h"
#include "mode.h"
#include "Tread3DDoc.h"

/*
Joe Riedel : 10/10/99
Added mode change messages.

Nick Randal : 11/9/99
Added submodes for each mode and redifined how the stack works
*/
CMode::CMode()
{
	m_nMode = DEFAULT_MODE;
	for(int n = 0; n < MODES; n++)
		m_nSubmode[n] = DEFAULT_SUBMODE;
	m_nFlags = DEFAULT_FLAG;
	m_nLastMode = -1;
	m_nLastSubmode = -1;
}

CMode::~CMode()
{
	m_modeList.DestroyList();
}

inline int GetIndex(int nNum);
inline int GetIndex(int nNum)
{
	switch(nNum)
	{
	case 0x1: return 0;
	case 0x2: return 1;
	case 0x4: return 2;
	case 0x8: return 3;
	case 0x10: return 4;
	case 0x20: return 5;
	case 0x40: return 6;
	case 0x80: return 7;
	case 0x100: return 8;
	case 0x200: return 9;
	case 0x400: return 10;
	case 0x800: return 11;
	case 0x1000: return 12;
	case 0x2000: return 13;
	case 0x4000: return 14;
	case 0x8000: return 15;
	case 0x10000: return 16;
	case 0x20000: return 17;
	case 0x40000: return 18;
	case 0x80000: return 19;
	case 0x100000: return 20;
	case 0x200000: return 21;
	case 0x400000: return 22;
	case 0x800000: return 23;
	case 0x1000000: return 24;
	case 0x2000000: return 25;
	case 0x4000000: return 26;
	case 0x8000000: return 27;
	case 0x10000000: return 28;
	case 0x20000000: return 29;
	case 0x40000000: return 30;
	case 0x80000000: return 31;
	}

	return 0xFFFFFFFF;

}

//interesting note
//pFlag stands for Parents and Friends of Lesbians And Gays
//just thought that i would mention it
void CMode::Push()
{
	MODE_FLAGS* pFlag;
	pFlag = new MODE_FLAGS;
	if(pFlag == NULL)
	{
		//error message
		return;
	}

	pFlag->nMode = m_nMode;
	pFlag->nSubmode = m_nSubmode[GetIndex(m_nMode)];

	m_modeList.AddItem(pFlag, LL_HEAD);
}

void CMode::Pop()
{
	if(m_modeList.IsEmpty())
		return;

	m_nLastMode = m_nMode;					//save the last mode
	m_nLastSubmode = m_nSubmode[GetIndex(m_nMode)];			//save the last submode

	MODE_FLAGS* pFlag;
	pFlag = m_modeList.GetItem(LL_HEAD);	//get a pointer to the item
	m_nMode = pFlag->nMode;					//load the popped mode
	m_nSubmode[m_nMode] = pFlag->nSubmode;			//load the popped submode
	m_modeList.DeleteItem(LL_HEAD);			//remove from the list

	m_pDocument->OnModeChangeEvent(m_nLastMode, m_nLastSubmode, m_nMode, m_nSubmode[GetIndex(m_nMode)]);
}

//clear both stacks
void CMode::ClearStack()
{
	m_modeList.DestroyList();
}

bool CMode::IsStackEmpty()
{
	return m_modeList.IsEmpty();
}

bool CMode::IsMode(int nMode)
{
	if(m_nMode == nMode)
		return true;
	else
		return false;
}

bool CMode::IsSubmode(int nSubmode)
{
	if((nSubmode & m_nSubmode[GetIndex(m_nMode)]) == nSubmode)
		return true;
	else
		return false;
}

void CMode::FlipLast()
{
	int nTemp, nTemp2;

	nTemp2 = m_nSubmode[GetIndex(m_nMode)];

	if(m_nLastMode != -1)
	{
		nTemp = m_nMode;
		m_nMode = m_nLastMode;
		m_nLastMode = nTemp;
	}

	if(m_nLastSubmode != -1)
	{
		m_nSubmode[GetIndex(m_nMode)] = m_nLastSubmode;
		m_nLastSubmode = nTemp2;
	}

	if(m_nLastMode != -1 || m_nLastSubmode != -1)
		m_pDocument->OnModeChangeEvent(m_nLastMode, m_nLastSubmode, m_nMode, m_nSubmode[GetIndex(m_nMode)]);
}

/*void CMode::FlipLastMode()
{
	int nTemp;

	if(m_nLastMode != -1)
	{
		nTemp = m_nMode;
		m_nMode = m_nLastMode;
		m_nLastMode = nTemp;
		m_pDocument->OnModeChangeEvent(m_nLastMode, m_nLastSubmode, m_nMode, m_nSubmode[m_nMode]);
	}
}

void CMode::FlipLastSubmode()
{
	int nTemp;

	if(m_nLastSubmode != -1)
	{
		nTemp = m_nSubmode[m_nMode];
		m_nSubmode[m_nMode] = m_nLastSubmode;
		m_nLastSubmode = nTemp;
		m_pDocument->OnModeChangeEvent(m_nMode, m_nLastSubmode, m_nMode, m_nSubmode[m_nMode]);
	}
}*/

void CMode::Set(int nMode, int nSubmode, bool bSet)
{
	m_nLastSubmode = m_nSubmode[GetIndex(m_nMode)];
	m_nLastMode = m_nMode;

	m_nMode = nMode;
	if(bSet)
		m_nSubmode[GetIndex(m_nMode)] = nSubmode;
	else
		m_nSubmode[GetIndex(m_nMode)] |= nSubmode;

	m_pDocument->OnModeChangeEvent(m_nLastMode, m_nLastSubmode, m_nMode, m_nSubmode[GetIndex(m_nMode)]);
}

void CMode::SetMode(int nFlag)
{
	m_nLastMode = m_nMode;
	m_nMode = nFlag;
	m_nLastSubmode = m_nSubmode[GetIndex(m_nMode)];
	m_pDocument->OnModeChangeEvent(m_nLastMode, m_nLastSubmode, m_nMode, m_nSubmode[GetIndex(m_nMode)]);
}

void CMode::RefreshMode()
{
	m_pDocument->OnModeChangeEvent(0, 0, m_nMode, m_nSubmode[GetIndex(m_nMode)]);
}

void CMode::Default()
{
	m_nLastMode = m_nMode;
	m_nMode = DEFAULT_MODE;

	m_nLastSubmode = m_nSubmode[GetIndex(m_nMode)];
	m_nSubmode[GetIndex(m_nMode)] = DEFAULT_SUBMODE;
}

void CMode::DefaultMode()
{
	m_nLastMode = m_nMode;
	m_nMode = DEFAULT_MODE;
}

void CMode::DefaultSubmode()
{
	m_nLastSubmode = m_nSubmode[GetIndex(m_nMode)];
	m_nSubmode[GetIndex(m_nMode)] = DEFAULT_SUBMODE;
}

void CMode::SetSubmode(int nFlag)
{
	m_nLastSubmode = m_nSubmode[GetIndex(m_nMode)];
	m_nSubmode[GetIndex(m_nMode)] = nFlag;
	m_pDocument->OnModeChangeEvent(m_nMode, m_nLastSubmode, m_nMode, m_nSubmode[GetIndex(m_nMode)]);
}

void CMode::SetSubmode(int nFlag, int nMask)
{
	int nIndex = GetIndex(m_nMode);
	m_nLastSubmode = m_nSubmode[nIndex];
	m_nSubmode[nIndex] = (m_nSubmode[nIndex] & ~nMask) | nFlag;
	m_pDocument->OnModeChangeEvent(m_nMode, m_nLastSubmode, m_nMode, m_nSubmode[nIndex]);
}

void CMode::AddSubmode(int nFlag)
{
	m_nLastSubmode = m_nSubmode[GetIndex(m_nMode)];
	m_nSubmode[GetIndex(m_nMode)] |= nFlag;
	m_pDocument->OnModeChangeEvent(m_nMode, m_nLastSubmode, m_nMode, m_nSubmode[GetIndex(m_nMode)]);
}

int CMode::GetMode()
{
	return m_nMode;
}

int CMode::GetSubmode()
{
	return m_nSubmode[GetIndex(m_nMode)];
}

int CMode::GetSubmode(int nMask)
{
	return (m_nSubmode[GetIndex(m_nMode)] & nMask);
}

int CMode::GetSubmodeFor(int nMode)
{
	return m_nSubmode[GetIndex(nMode)];
}

int CMode::GetSubmodeFor(int nMode, int nMask)
{
	return (m_nSubmode[GetIndex(nMode)] & nMask);
}

void CMode::ClearFlags()
{
	m_nFlags = 0;
}

void CMode::SetFlags(int nFlags)
{
	m_nFlags = nFlags;
}

void CMode::SetFlag(int nFlag, bool bValue)
{
	if(bValue)
		m_nFlags |= nFlag;
	else
		m_nFlags &= !nFlag;
}

void CMode::SubtractFlags(int nFlags)
{
	m_nFlags &= !nFlags;
}

void CMode::ToggleFlag(int nFlags)
{
	_asm
	{
		mov esi, this				;load the address of the class
		mov eax, [esi]this.m_nFlags	;move the value of the flags
		xor eax, nFlags				;xor with the new flags
		mov [esi]this.m_nFlags, eax	;store the result back in flags
	}
}

void CMode::AddFlags(int nFlags)
{
	m_nFlags |= nFlags;
}

int CMode::GetFlags()
{
	return m_nFlags;
}

bool CMode::IsFlag(int nFlags)
{
	if((m_nFlags & nFlags) == nFlags)
		return true;
	else
		return false;
}