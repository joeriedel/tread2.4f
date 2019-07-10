#ifndef DIALOGCONTROL_H
#define DIALOGCONTROL_H

#include "ConsoleDialog.h"
#include "PreviewDialog.h"
#include "SurfaceDialog.h"
#include "QuickHelpDialog.h"
#include "TExport.h"

//dialog entries

#define DLG_DEBUG		0
#define DLG_SURFACE		1
#define DLG_PREVIEW		2
#define DLG_QUICKHELP	3
//-----------------------
#define DIALOG_MAX		4	//number of dialog entries

void RegisterDialog(CDialog* pDlg, UINT nCode);
void UnregisterDialog(UINT nCode);
CDialog* GetDialog(int nCode);
void ShowDialog(int nCode, bool bShow = true);
bool IsDialogShown(int nCode);
void ShowAllDialogs(bool bShow = true);

CConsoleDialog* GetConsoleDialog();
CSurfaceDialog* GetSurfaceDialog();
CPreviewDialog* GetPreviewDialog();
CQuickHelpDialog* GetQuickHelpDialog();

#endif
