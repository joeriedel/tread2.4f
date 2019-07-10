// msg.cpp : implementation of the CMsg class
//
// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) 1992-1998 Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.

#include "stdafx.h"
#include "msg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMsg

IMPLEMENT_DYNCREATE(CMsg, CObject)

/////////////////////////////////////////////////////////////////////////////
// CMsg construction/destruction

CMsg::CMsg()
{
	Init();
}

CMsg::~CMsg()
{
}

/////////////////////////////////////////////////////////////////////////////
// CMsg Operations

void CMsg::Init()
{
	m_bClose = false;
	m_sText = _T("");
}

/////////////////////////////////////////////////////////////////////////////
// CMsg serialization

void CMsg::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		ar << (WORD)m_bClose;
		ar << m_sText;
	}
	else
	{
		WORD wd;
		ar >> wd;
		m_bClose = (wd != 0) ? true : false;
		ar >> m_sText;
	}
}

/////////////////////////////////////////////////////////////////////////////
// CMsg diagnostics

#ifdef _DEBUG
void CMsg::AssertValid() const
{
	CObject::AssertValid();
}

void CMsg::Dump(CDumpContext& dc) const
{
	CObject::Dump(dc);
}
#endif //_DEBUG
