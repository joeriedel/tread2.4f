// ZOOMMODEWINRESPONDER.CPP
// Copyright © 1999 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#include "stdafx.h"
#include "Tread3DDoc.h"
#include "MapView.h"
#include "ZoomModeWinResponder.h"

CZoomModeWinResponder::CZoomModeWinResponder()
{
	m_nMode = MODE_ZOOM;
	m_nSubmode = ALL_MODES;
}

void CZoomModeWinResponder::OnMouseDown(CBaseWindowEventResponder* pParent, CWindowEventManager* pEventManager, CMapView* pView, CTread3DDoc* pDocument, int nMouseX, int nMouseY, int nButtons)
{
	// We zoom in and out by keeping the point clicked on in the same
	// spot.
	float /*fDX, fDY,*/ fOldScale, fNewScale;

	fOldScale = pView->GetScale();

	if(nButtons&EF_LBUTTON)
	{
		if(fOldScale < (1000.0F/2.0F))
			fNewScale = fOldScale * 2.0F;
		else
			return;
	}
	else if(nButtons&EF_RBUTTON)
	{
		if(fOldScale > (0.01F*2.0F))
			fNewScale = fOldScale/2.0F;
		else
			return;
	}

	/*fDX = nMouseX - pView->m_vSize[0];
	fDY = nMouseY - pView->m_vSize[1];*/

	float fOX = WX_TO_MX(nMouseX, pView);
	float fOY = WY_TO_MY(nMouseY, pView);

	pView->SetScale(fNewScale);

	float fNX = WX_TO_MX(nMouseX, pView);
	float fNY = WY_TO_MY(nMouseY, pView);

	float fMX = pView->m_vMapCenter[0] + (fOX - fNX);
	float fMY = pView->m_vMapCenter[1] + (fOY - fNY);

	/*fMX += -VO(VT(pView), VEC_X) * (fDX * pView->GetInvScale());
	fMY += -VO(VT(pView), VEC_Y) * (fDY * pView->GetInvScale());*/

	pView->SetMapXY(fMX, fMY);
	pView->RedrawWindow();
}
