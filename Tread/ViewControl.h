#ifndef VIEWCONTROL_H
#define VIEWCONTROL_H

#include "VisgroupView.h"
#include "AnchorView.h"
#include "ObjectView.h"
#include "TextureView.h"
#include "TExport.h"

#define VIEW_MAX		4
// $sb "viewcontrol.h block1"
//-----------------------
#define VISGROUP_VIEW	0
#define ANCHOR_VIEW		1
#define OBJECT_VIEW		2
#define TEXTURE_VIEW	3
// $eb
void InitializeViewList();
bool RegisterView(UINT nCode, CView* pView);
CView* GetView(UINT nCode);
void EnableView(UINT nCode, bool bEnable = true);
void EnableAllViews(bool bEnable = true);

#endif