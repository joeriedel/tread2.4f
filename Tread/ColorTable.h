// COLORTABLE.H
// Copyright © 1999 Joe Riedel, Nick Randal.
// Author: Nick Randal.

/*
//99, By: 
--------------------------------
*/

#ifndef COLORTABLE_H
#define COLORTABLE_H

#include "texport.h"

// $sb "colortable.h block1"

#define CL_LINE				0
#define CL_LINE_SEL			1
#define CL_LINE_HATCH		2
#define CL_GRID				3
#define CL_GRID_ORIG		4
#define CL_HOTSPOT			5
#define CL_HOTSPOT_SEL		6
#define CL_HOTSPOT_BOX		7
#define CL_CAMERA_BASE		8
#define CL_CAMERA_LINE		9
#define CL_CAMERA_SEL		10
#define CL_CLIP				11
#define CL_ENTITY			12
#define CL_OWNED			13
#define CL_LEAK				14
#define CL_LINK				15
#define CL_FLAT_BACKGROUND	16
#define CL_3D_BACKGROUND	17
#define CL_HOTSPOT_LINE		18
#define CL_ANCHOR			19
#define CL_ANCHOR_SEL		20
#define CL_ANCHOR2			21
#define CL_BRUSH_HANDLE		22
#define CL_VIEW_SELECTED	23
#define CL_VIEW_NORMAL		24
#define CL_BBOX_SIZE		25
//----------------------------
#define CL_TABLE_LENGTH		26

//item masks
#define CI_FLOAT	1
#define CI_PENDOT	2
#define CI_PENSOLID 4
#define CI_BRUSH	8
#define CI_STRING	16

//Version number - color gets added, then change this
#define CL_VERSION 9

struct COLORITEM
{
	int m_nMask;
	float m_fColor[3];
	COLORREF m_nDefault;
#ifdef _WIN32
	CPen* m_pDot;
	CPen* m_pSolid;
	CBrush* m_pBrush;
#endif
	CString m_sName;
};

class _T_EXPORT CColorTable
{
public:
	CColorTable();
	~CColorTable();

	bool SetColor(int nID, COLORREF nColor);
	bool SetColor(int nID, float r, float g, float b);

	void Default();
	void Initialize();
	void Destroy();

#ifdef _WIN32
	CPen* GetDotPen(int nID);
	CPen* GetSolidPen(int nID);
	CPen* GetPen(int nID);
	CBrush* GetBrush(int nID);
#endif

	float* GetFloatV(int nID);
	COLORREF GetColor(int nID);

	int GetTableCount();
	CString GetColorItemString(int nID);

	void SetColorItem(int nID, COLORREF nColor);
	void SetColorItem(int nID, COLORREF nColor, int nMask);
	void SetColorItem(int nID, COLORREF nColor, int nMask, CString sName);

	void SaveTable(CString sPath);
	bool LoadTable(CString sPath);

private:
	COLORITEM m_clrTable[CL_TABLE_LENGTH];
};

// $eb

CColorTable* GetColorTable();

#endif