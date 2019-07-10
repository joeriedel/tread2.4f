// GDIRENDERPACKAGE.H
// Copyright © 1999 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#ifndef GDIRENDERPACKAGE_H
#define GDIRENDERPACKAGE_H

#include "RenderAPI.h"

class CGDIRenderPackage : public CBaseRenderPackage
{
public:

	CGDIRenderPackage() {}
	~CGDIRenderPackage() {}

	static bool StaticClaimPackageName(CString sName) { return sName == "wingdi"; }

	int GetRenderFlags() { return RF_2D_API; }
	bool LoadPackage(CString sName) { return true; }
	bool UnloadPackage() { return true; }
	CString GetPackageName() { return "wingdi"; }
	CBaseWindowRenderer* GetWindowRenderer(CMapView* pView, CDC* pDC, int nType);

};

#endif