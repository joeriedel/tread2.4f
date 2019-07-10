// CGDIRENDERPACKAGE.CPP
// Copyright © 1999 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#include "stdafx.h"
#include "TreadSys.h"
#include "RenderAPI.h"
#include "GDIRenderPackage.h"
#include "GDIWindowRenderer.h"

CBaseWindowRenderer* CGDIRenderPackage::GetWindowRenderer(CMapView* pView, CDC* pDC, int nType)
{
	CGDIWindowRenderer* pWR= new CGDIWindowRenderer();
	
	if(!pWR->Initialize(pView, pDC, nType))
	{
		delete pWR;
		return NULL;
	}

	return pWR;
}


