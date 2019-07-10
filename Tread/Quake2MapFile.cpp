// QUAKE2MAPFILE.CPP
// Copyright © 1999 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#include "stdafx.h"
#include "TreadMapAPI.h"
#include "QuakeMapFile.h"
#include "Quake2MapFile.h"
#include "szstring.h"
#include "Face.h"
#include "TreadSys.h"
#include "Tread3DDoc.h"
#include <math.h>
#include <fstream.h>

int CQuake2MapFile::ReadPlane(CQBrushPlane* pPlane, CTread3DDoc* pDocument, CTokenizer& Tokenizer)
{
	// Read the 3pt plane def.
	int i;
	vec3_t v[3];
	vec3_t vE[2];
	CString sToken;
	CString sTexture;

	for(i = 0; i < 3; i++)
	{
		Tokenizer.Skip();	// (
		Tokenizer.GetToken(sToken);
		v[i][0] = (float)atof(sToken);
		Tokenizer.GetToken(sToken);
		v[i][1] = (float)atof(sToken);
		Tokenizer.GetToken(sToken);
		v[i][2] = (float)atof(sToken);
		Tokenizer.Skip(); // )
	}

	// Get the texture name.
	if(!Tokenizer.GetToken(sTexture))
	{
		Error("CQuake2MapFile::ReadPlane(): Missing brush texture!\n");
		return QRF_FATAL;
	}

	sTexture.MakeLower();
	pPlane->m_bUseFlags = false;
	pPlane->m_sTexture = sTexture;

	// Read shifts, scales, and rotations.
	Tokenizer.GetToken(sToken);
	pPlane->m_vShifts[0] = (float)atof(sToken);
	Tokenizer.GetToken(sToken);
	pPlane->m_vShifts[1] = (float)atof(sToken);
	Tokenizer.GetToken(sToken);
	pPlane->m_fRotate = (float)atof(sToken);
	Tokenizer.GetToken(sToken);
	pPlane->m_vScales[0] = (float)atof(sToken);
	Tokenizer.GetToken(sToken);
	pPlane->m_vScales[1] = (float)atof(sToken);
	
	// Read contents/surface/value flags.
	Tokenizer.GetToken(sToken);
	// They may not be present.
	if(sToken != "}" && sToken != "(")
	{
		pPlane->m_bUseFlags = true;
		pPlane->m_nContents = atoi(sToken);
		Tokenizer.GetToken(sToken);
		pPlane->m_nSurface = atoi(sToken);
		Tokenizer.GetToken(sToken);
		pPlane->m_nValue = atoi(sToken);

		// the next 3 values might not be present (wayne fix).
		Tokenizer.GetToken(sToken);
		if(sToken != "}" && sToken != "(")
			// skip the next two.
			Tokenizer.Skip(2);
		else
			Tokenizer.UngetToken();
	}
	else
		Tokenizer.UngetToken();

	// Make the plane.
	VectorSubtract(v[0], v[1], vE[0]);
	VectorSubtract(v[2], v[1], vE[1]);
	CalcOrthoVector(vE[0], vE[1], pPlane->m_Plane.m_vNormal);

	// Bad normal?
	if(VectorIsZero(pPlane->m_Plane.m_vNormal))
	{
		Warning("CQuake2MapFile::ReadPlane(): bad plane normal!\n");
		return QRF_ERROR;
	}

	// Get a distance.
	pPlane->m_Plane.m_vDist = DotProduct(v[0], pPlane->m_Plane.m_vNormal);

	m_nFaceCount++;
	return QRF_OK;
}

bool CQuake2MapFile::WriteFace(CFace* pFace, CTread3DDoc* pDocument, fstream& file)
{
	int i;
	// Just skip.
	if(pFace->GetNumPoints() < 3)
	{
		Warning("CQuake2MapFile::WriteFace(): < 3 pts.");
		return true;
	}

	// Write the 3 point plane def.
	vec3_t* vPoints = pFace->GetPoints();
	for(i = 0; i < 3; i++)
		file << "( " << (int)(floor(vPoints[i][0] - 0.5) + 1) << " " << (int)(floor(vPoints[i][1] - 0.5) + 1) << " " << (int)(floor(vPoints[i][2] - 0.5) + 1) << " ) ";

	CString sTexture = pFace->GetTextureName();
	sTexture.MakeLower();
	file << sTexture;

	vec_t vShifts[2], vScales[2];
	pFace->GetTexScales(vScales);
	pFace->GetTexShifts(vShifts);

	file << " " << (int)vShifts[0] << " " << (int)vShifts[1] << " " << (int)pFace->GetRotation() << " " << vScales[0] << " " << vScales[1];
	file << " " << pFace->m_nContents << " " << pFace->m_nSurface << " " << pFace->m_nValue;
	file << "\n";

	m_nFaceCount++;
	return true;
}

