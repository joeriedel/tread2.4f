#include "stdafx.h"
#include "Tread3D.h"
#include "TreadSys.h"
#include "ColorTable.h"
#include "futils.h"

// Edit(1): 9/25/199 By: Joe Riedel.
// Added Clear Colors for 3D view, and Flat views.

CColorTable colorTable;

CColorTable* GetColorTable()
{
	return &colorTable;
}

CColorTable::CColorTable()
{
	for(int n = 0; n < CL_TABLE_LENGTH; n++)
	{
		m_clrTable[n].m_fColor[0] = 0;
		m_clrTable[n].m_fColor[1] = 0;
		m_clrTable[n].m_fColor[2] = 0;
#ifdef _WIN32
		m_clrTable[n].m_pBrush = NULL;
		m_clrTable[n].m_pDot = NULL;
		m_clrTable[n].m_pSolid = NULL;
#endif
	}
}

CColorTable::~CColorTable()
{
	Destroy();
}

void CColorTable::Destroy()
{
#ifdef _WIN32
	for(int n = 0; n < CL_TABLE_LENGTH; n++)
	{
		if(m_clrTable[n].m_pDot != NULL)
		{
			delete m_clrTable[n].m_pDot;
			m_clrTable[n].m_pDot = NULL;
		}

		if(m_clrTable[n].m_pSolid != NULL)
		{
			delete m_clrTable[n].m_pSolid;
			m_clrTable[n].m_pSolid = NULL;
		}

		if(m_clrTable[n].m_pBrush != NULL)
		{
			delete m_clrTable[n].m_pBrush;
			m_clrTable[n].m_pBrush = NULL;
		}
	}
#endif
}

void CColorTable::Initialize()
{
	SetColorItem(CL_LINE, RGB(255, 255, 255), CI_FLOAT | CI_PENSOLID | CI_STRING, "Line");
	SetColorItem(CL_LINE_SEL, RGB(215, 0, 0), CI_FLOAT | CI_PENSOLID | CI_STRING, "Line selection");
	SetColorItem(CL_LINE_HATCH, RGB(255, 255, 255), CI_FLOAT | CI_PENDOT | CI_STRING, "Hatched Line");
	SetColorItem(CL_GRID, RGB(50, 50, 70), CI_FLOAT | CI_PENSOLID | CI_STRING, "Grid lines");
	SetColorItem(CL_GRID_ORIG, RGB(75, 75, 75), CI_FLOAT | CI_PENSOLID | CI_STRING, "Grid origin");
	SetColorItem(CL_HOTSPOT, RGB(100, 200, 150), CI_FLOAT | CI_PENSOLID | CI_BRUSH | CI_STRING, "Hot spots");
	SetColorItem(CL_HOTSPOT_SEL, RGB(215, 0, 0), CI_FLOAT | CI_PENSOLID | CI_BRUSH | CI_STRING, "Selected hot spots");
	SetColorItem(CL_HOTSPOT_BOX, RGB(100, 200, 150), CI_FLOAT | CI_PENDOT | CI_STRING, "Selection Box");
	SetColorItem(CL_CAMERA_BASE, RGB(145, 150, 50), CI_FLOAT | CI_PENSOLID | CI_BRUSH | CI_STRING, "Camera base");
	SetColorItem(CL_CAMERA_LINE, RGB(145, 150, 50), CI_FLOAT | CI_PENSOLID | CI_STRING, "Camera pointer");
	SetColorItem(CL_CAMERA_SEL, RGB(180, 180, 0), CI_FLOAT | CI_PENSOLID | CI_BRUSH | CI_STRING, "Selected Camera");
	SetColorItem(CL_CLIP, RGB(213, 213, 0), CI_FLOAT | CI_PENSOLID | CI_STRING, "Clip tool");
	SetColorItem(CL_ENTITY, RGB(50, 100, 255), CI_FLOAT | CI_PENSOLID | CI_STRING, "Entities");
	SetColorItem(CL_OWNED, RGB(242, 230, 77), CI_FLOAT | CI_PENSOLID | CI_STRING, "Linked brushes");
	SetColorItem(CL_LEAK, RGB(230, 230, 0), CI_FLOAT | CI_PENSOLID | CI_STRING, "Leak trace tool");
	SetColorItem(CL_LINK, RGB(240, 0, 240), CI_FLOAT | CI_PENSOLID | CI_STRING, "Target links");
	SetColorItem(CL_FLAT_BACKGROUND, RGB(0, 0, 0), CI_FLOAT | CI_PENSOLID | CI_STRING, "Map background");
	SetColorItem(CL_3D_BACKGROUND, RGB(0, 0, 0), CI_FLOAT | CI_PENSOLID | CI_STRING, "3D view background");
	SetColorItem(CL_HOTSPOT_LINE, RGB(128, 0, 128), CI_FLOAT | CI_PENSOLID | CI_STRING, "Hot Spot Lines");
	SetColorItem(CL_ANCHOR, RGB(0, 200, 100), CI_FLOAT | CI_PENSOLID | CI_BRUSH | CI_STRING, "Anchor");
	SetColorItem(CL_ANCHOR2, RGB(0, 100, 180), CI_FLOAT|CI_PENSOLID|CI_BRUSH|CI_STRING, "Anchor Border");
	SetColorItem(CL_ANCHOR_SEL, RGB(0, 100, 200), CI_FLOAT | CI_PENSOLID | CI_BRUSH | CI_STRING, "Selected Anchor");
	SetColorItem(CL_BRUSH_HANDLE, RGB(10, 150, 10), CI_FLOAT|CI_PENSOLID|CI_BRUSH|CI_STRING, "Brush Handles");
	SetColorItem(CL_VIEW_SELECTED, RGB(0, 150, 200), CI_FLOAT|CI_PENSOLID|CI_BRUSH|CI_STRING, "View Text Selected");
	SetColorItem(CL_VIEW_NORMAL, RGB(255, 255, 255), CI_FLOAT|CI_PENSOLID|CI_BRUSH|CI_STRING, "View Text Normal");
	SetColorItem(CL_BBOX_SIZE, RGB(255, 128, 0), CI_FLOAT|CI_PENSOLID|CI_BRUSH|CI_STRING, "BBox Size Text");

	Debug("... initing color table\n");
	CString sPath;
	sPath.Format("%s\\data2.tread", GetRegString(S_TREAD_PATH));
	if(!LoadTable(sPath))
	{
		Debug("... default colors\n");
		Default();
	}
	else
		Debug("... user colors loaded\n");
	
}

bool CColorTable::SetColor(int nID, COLORREF nColor)
{
	//int nTemp;

	if((m_clrTable[nID].m_nMask & CI_FLOAT) == CI_FLOAT)
	{
		m_clrTable[nID].m_fColor[0] = (float)(nColor & 0xFF)/ 255;
		m_clrTable[nID].m_fColor[1] = (float)((nColor & 0xFF00) >> 8)/ 255;
		m_clrTable[nID].m_fColor[2] = (float)((nColor & 0xFF0000) >> 16)/ 255;
	}

#ifdef _WIN32
	if((m_clrTable[nID].m_nMask & CI_PENDOT) == CI_PENDOT)
	{
		if(m_clrTable[nID].m_pDot != NULL)
		{	
			delete m_clrTable[nID].m_pDot;
			m_clrTable[nID].m_pDot = NULL;
		}

		m_clrTable[nID].m_pDot = new CPen(PS_DOT, 1, nColor);
		if(m_clrTable[nID].m_pDot == NULL)
			return false;
	}

	if((m_clrTable[nID].m_nMask & CI_PENSOLID) == CI_PENSOLID)
	{
		if(m_clrTable[nID].m_pSolid != NULL)
		{	
			delete m_clrTable[nID].m_pSolid;
			m_clrTable[nID].m_pSolid = NULL;
		}

		m_clrTable[nID].m_pSolid = new CPen(PS_SOLID, 1, nColor);
		if(m_clrTable[nID].m_pSolid == NULL)
			return false;
	}

	if((m_clrTable[nID].m_nMask & CI_BRUSH) == CI_BRUSH)
	{
		if(m_clrTable[nID].m_pBrush != NULL)
		{	
			delete m_clrTable[nID].m_pBrush;
			m_clrTable[nID].m_pBrush = NULL;
		}

		m_clrTable[nID].m_pBrush  = new CBrush(nColor);
		if(m_clrTable[nID].m_pBrush == NULL)
			return false;
	}
#endif

	return true;
}

#ifdef _WIN32
CPen* CColorTable::GetDotPen(int nID)
{
	return m_clrTable[nID].m_pDot;
}

CPen* CColorTable::GetSolidPen(int nID)
{
	return m_clrTable[nID].m_pSolid;
}

CPen* CColorTable::GetPen(int nID)
{
	if((m_clrTable[nID].m_nMask & CI_PENSOLID) == CI_PENSOLID)
		return m_clrTable[nID].m_pSolid;
	else if((m_clrTable[nID].m_nMask & CI_PENDOT) == CI_PENDOT)
		return m_clrTable[nID].m_pDot;
	else
		return NULL;
}

CBrush* CColorTable::GetBrush(int nID)
{
	return m_clrTable[nID].m_pBrush;
}

#endif

float* CColorTable::GetFloatV(int nID)
{
	return m_clrTable[nID].m_fColor;
}

COLORREF CColorTable::GetColor(int nID)
{
	int r, g, b;
	r = (int)(m_clrTable[nID].m_fColor[0] * 255);
	g = (int)(m_clrTable[nID].m_fColor[1] * 255);
	b = (int)(m_clrTable[nID].m_fColor[2] * 255);
	return RGB(r, g, b);
}

void CColorTable::SaveTable(CString sPath)
{
	CFile hFile;

	if(hFile.Open(sPath, CFile::modeWrite | CFile::modeCreate))
	{
		hFile.Write("CLRT", 4);	//file ID
		WriteInt(hFile, CL_VERSION);

		for(int n = 0; n < CL_TABLE_LENGTH; n++)
		{
			/*hFile.Write(&m_clrTable[n].m_nMask, sizeof(int));
			hFile.Write(&m_clrTable[n].m_fColor[0], sizeof(float));
			hFile.Write(&m_clrTable[n].m_fColor[1], sizeof(float));
			hFile.Write(&m_clrTable[n].m_fColor[2], sizeof(float));*/
			WriteInt(hFile, m_clrTable[n].m_nMask);
			WriteFloat(hFile, m_clrTable[n].m_fColor[0]);
			WriteFloat(hFile, m_clrTable[n].m_fColor[1]);
			WriteFloat(hFile, m_clrTable[n].m_fColor[2]);
		}

		hFile.Close();
	}
}

bool CColorTable::LoadTable(CString sPath)
{
	CFile hFile;
	int nTemp;
	int nItem = 1;

	if(hFile.Open(sPath, CFile::modeRead | CFile::modeNoTruncate))
	{
		hFile.Read(&nTemp, 4);	//file ID
		ReadInt(hFile, nTemp);
		
		if(nTemp != CL_VERSION)
			return false;

		for(int n = 0; n < CL_TABLE_LENGTH; n++)
		{
			/*hFile.Read(&m_clrTable[n].m_nMask, sizeof(int));
			hFile.Read(&m_clrTable[n].m_fColor[0], sizeof(float));
			hFile.Read(&m_clrTable[n].m_fColor[1], sizeof(float));
			hFile.Read(&m_clrTable[n].m_fColor[2], sizeof(float));*/
			ReadInt(hFile, m_clrTable[n].m_nMask);
			ReadFloat(hFile, m_clrTable[n].m_fColor[0]);
			ReadFloat(hFile, m_clrTable[n].m_fColor[1]);
			ReadFloat(hFile, m_clrTable[n].m_fColor[2]);

			if((m_clrTable[n].m_nMask & CI_STRING) != CI_STRING)
			{
				m_clrTable[n].m_sName.Format("Color Option %d", nItem);
				nItem++;
			}

#ifdef _WIN32
			int r, g, b;
			r = (int)(m_clrTable[n].m_fColor[0] * 255);
			g = (int)(m_clrTable[n].m_fColor[1] * 255);
			b = (int)(m_clrTable[n].m_fColor[2] * 255);

			nTemp = RGB(r, g ,b);

			if((m_clrTable[n].m_nMask & CI_PENDOT) == CI_PENDOT)
			{
				if(m_clrTable[n].m_pDot != NULL)
				{	
					delete m_clrTable[n].m_pDot;
					m_clrTable[n].m_pDot = NULL;
				}

				m_clrTable[n].m_pDot = new CPen(PS_DOT, 1, nTemp);
				if(m_clrTable[n].m_pDot == NULL)
					return false;
			}

			if((m_clrTable[n].m_nMask & CI_PENSOLID) == CI_PENSOLID)
			{
				if(m_clrTable[n].m_pSolid != NULL)
				{	
					delete m_clrTable[n].m_pSolid;
					m_clrTable[n].m_pSolid = NULL;
				}

				m_clrTable[n].m_pSolid = new CPen(PS_SOLID, 1, nTemp);
				if(m_clrTable[n].m_pSolid == NULL)
					return false;
			}

			if((m_clrTable[n].m_nMask & CI_BRUSH) == CI_BRUSH)
			{
				if(m_clrTable[n].m_pBrush != NULL)
				{	
					delete m_clrTable[n].m_pBrush;
					m_clrTable[n].m_pBrush = NULL;
				}

				m_clrTable[n].m_pBrush  = new CBrush(nTemp);
				if(m_clrTable[n].m_pBrush == NULL)
					return false;
			}
#endif

		}

		hFile.Close();

		return true;
	}

	return false;
}

void CColorTable::Default()
{
	int nColor;
	CString sBuffer;
	int nItem = 1;

	for(int n = 0; n < CL_TABLE_LENGTH; n ++)
	{
		nColor = m_clrTable[n].m_nDefault;

		if((m_clrTable[n].m_nMask & CI_FLOAT) == CI_FLOAT)
		{
			m_clrTable[n].m_fColor[0] = (float)((nColor & 0xFF))/ 255;
			m_clrTable[n].m_fColor[1] = (float)((nColor & 0xFF00) >> 8)/ 255;
			m_clrTable[n].m_fColor[2] = (float)((nColor & 0xFF0000) >> 16)/ 255;
		}

#ifdef _WIN32
		if((m_clrTable[n].m_nMask & CI_PENDOT) == CI_PENDOT)
		{
			if(m_clrTable[n].m_pDot != NULL)
			{	
				delete m_clrTable[n].m_pDot;
				m_clrTable[n].m_pDot = NULL;
			}

			m_clrTable[n].m_pDot = new CPen(PS_DOT, 1, nColor);
			if(m_clrTable[n].m_pDot == NULL)
				return;
		}

		if((m_clrTable[n].m_nMask & CI_PENSOLID) == CI_PENSOLID)
		{
			if(m_clrTable[n].m_pSolid != NULL)
			{	
				delete m_clrTable[n].m_pSolid;
				m_clrTable[n].m_pSolid = NULL;
			}

			m_clrTable[n].m_pSolid = new CPen(PS_SOLID, 1, nColor);
			if(m_clrTable[n].m_pSolid == NULL)
				return;
		}

		if((m_clrTable[n].m_nMask & CI_BRUSH) == CI_BRUSH)
		{
			if(m_clrTable[n].m_pBrush != NULL)
			{	
				delete m_clrTable[n].m_pBrush;
				m_clrTable[n].m_pBrush = NULL;
			}

			m_clrTable[n].m_pBrush  = new CBrush(nColor);
			if(m_clrTable[n].m_pBrush == NULL)
				return;
		}
#endif

		if((m_clrTable[n].m_nMask & CI_STRING) != CI_STRING)
		{
			sBuffer.Format("Color Option %d", nItem);
			nItem++;
		}
	}
}

void CColorTable::SetColorItem(int nID, COLORREF nColor)
{
	m_clrTable[nID].m_nDefault = nColor;
}

void CColorTable::SetColorItem(int nID, COLORREF nColor, int nMask)
{
	m_clrTable[nID].m_nDefault = nColor;
	m_clrTable[nID].m_nMask = nMask;
}

void CColorTable::SetColorItem(int nID, COLORREF nColor, int nMask, CString sName)
{
	m_clrTable[nID].m_nDefault = nColor;
	m_clrTable[nID].m_nMask = nMask;
	m_clrTable[nID].m_sName = sName;
}

int CColorTable::GetTableCount()
{
	return CL_TABLE_LENGTH;
}

CString CColorTable::GetColorItemString(int nID)
{
	return m_clrTable[nID].m_sName;
}