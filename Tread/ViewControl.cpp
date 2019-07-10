#include "stdafx.h"
#include "TreadSys.h"
#include "ViewControl.h"

CView* viewList[VIEW_MAX];

void InitializeViewList()
{
	for(int n = 0; n < VIEW_MAX; n++)
		viewList[n] = NULL;
}

bool RegisterView(UINT nCode, CView* pView)
{
	if((nCode >= VIEW_MAX) || (nCode < 0) || (pView == NULL))
		return false;

	viewList[nCode] = pView;

	return true;
}

CView* GetView(UINT nCode)
{
	if((nCode >= VIEW_MAX) || (nCode < 0))
		return NULL;

	return viewList[nCode];
}

void EnableView(UINT nCode, bool bEnable)
{
	if((nCode >= VIEW_MAX) || (nCode < 0) || (viewList[nCode] == NULL))
		return;

	viewList[nCode]->EnableWindow(bEnable);
}

void EnableAllViews(bool bEnable)
{
	for(int n = 0; n < VIEW_MAX; n++)
	{
		if(viewList[n] != NULL)
			viewList[n]->EnableWindow(bEnable);
	}
}