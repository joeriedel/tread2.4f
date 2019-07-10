#include "stdafx.h"
#include "DialogControl.h"

static CDialog* dlgTable[DIALOG_MAX];
static bool bShowTable[DIALOG_MAX];

void RegisterDialog(CDialog* pDlg, UINT nCode)
{
	//add the dialog pointer to the array
	//in a pre defined position
	dlgTable[nCode] = pDlg;
	bShowTable[nCode] = false;
}

void UnregisterDialog(UINT nCode)
{
	if(dlgTable[nCode] != NULL)
	{
		dlgTable[nCode]->DestroyWindow();
		delete dlgTable[nCode];
		dlgTable[nCode] = NULL;
		bShowTable[nCode] = false;
	}
}

CDialog* GetDialog(int nCode)
{
	//get a pointer to the dialog from the array
	//at a pre defined position
	return dlgTable[nCode];
}

void ShowDialog(int nCode, bool bShow)
{
	//check to see if the dialog has been registered
	if(dlgTable[nCode] == NULL)
		return;

	//show or hide the dialog based on the parameter
	if(bShow)
	{
		dlgTable[nCode]->ShowWindow(SW_NORMAL);
		bShowTable[nCode] = false;
	}
	else
	{
		dlgTable[nCode]->ShowWindow(SW_HIDE);
		bShowTable[nCode] = true;
	}
}

bool IsDialogShown(int nCode)
{
	if(dlgTable[nCode] == NULL)
		return false;

	return bShowTable[nCode];
}

void ShowAllDialogs(bool bShow)
{
	for(int n = 0; n < DIALOG_MAX; n++)
	{
		if(dlgTable[n] != NULL && n != DLG_DEBUG)
		{
			if(bShow)
			{
				dlgTable[n]->ShowWindow(SW_NORMAL);
				bShowTable[n] = false;
			}
			else
			{
				dlgTable[n]->ShowWindow(SW_HIDE);
				bShowTable[n] = true;
			}
		}
	}
}

CConsoleDialog* GetConsoleDialog()
{
	return (CConsoleDialog*)dlgTable[DLG_DEBUG];
}

CSurfaceDialog* GetSurfaceDialog()
{
	return (CSurfaceDialog*)dlgTable[DLG_SURFACE];
}

CPreviewDialog* GetPreviewDialog()
{
	return (CPreviewDialog*)dlgTable[DLG_PREVIEW];
}

CQuickHelpDialog* GetQuickHelpDialog()
{
	return (CQuickHelpDialog*)dlgTable[DLG_QUICKHELP];
}