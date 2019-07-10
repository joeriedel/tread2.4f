// GL3DENTITYRENDERER.H
// Copyright © 1999 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#include "stdafx.h"
#include "Entity.h"
#include "RenderAPI.h"
#include "GL3DWindowRenderer.h"
#include "GL3DEntityRenderer.h"
#include "ColorTable.h"
#include "GameAPI.h"

CGL3DEntityRenderer::CGL3DEntityRenderer(WOGLFUNCTIONTABLE* pGLFT)
{
	m_pGLFT = pGLFT;
}

void CGL3DEntityRenderer::RenderEntity(CEntity* pEntity, int nDrawFlags, int nFlags)
{
	vec3_t vMins, vMaxs, vSize;
	ENTITYDEFINITION* pDef = pEntity->GetEntityDefinition();

	if(pDef == NULL)
	{
		vSize[0] = 32.0F;
		vSize[1] = 32.0F;
		vSize[2] = 32.0F;
	}
	else
		VectorCopy(pDef->vSize, vSize);

	if(nDrawFlags&RF_PICKMODE)
		m_pGLFT->glLoadName((unsigned int)pEntity);
	else
	{
		m_pGLFT->glDisable(GL_TEXTURE_2D);

		vec_t* pColor;

		// Selected?
		if(nDrawFlags&RF_SELECTED)
			pColor = GetColorTable()->GetFloatV(CL_LINE_SEL);
		else
			pColor = GetColorTable()->GetFloatV(CL_ENTITY);

		m_pGLFT->glColor4f(pColor[0], pColor[1], pColor[2], GL3D_BASE_ALPHA);
	}

	VectorAdd(pEntity->m_vOrigin, vSize, vMaxs);
	VectorSubtract(pEntity->m_vOrigin, vSize, vMins);

	if(nDrawFlags&(RF_SOLID|RF_TEXTURED|RF_PICKMODE))
	{
		m_pGLFT->glBegin(GL_QUADS);

		m_pGLFT->glVertex3i((int)vMaxs[0], (int)vMins[1], (int)vMaxs[2]);	// D
		m_pGLFT->glVertex3i((int)vMins[0], (int)vMins[1], (int)vMaxs[2]);	// C
		m_pGLFT->glVertex3i((int)vMins[0], (int)vMaxs[1], (int)vMaxs[2]);	// B
		m_pGLFT->glVertex3i((int)vMaxs[0], (int)vMaxs[1], (int)vMaxs[2]);	// A

		// Bottom.
		m_pGLFT->glVertex3i((int)vMaxs[0], (int)vMaxs[1], (int)vMins[2]);	// H
		m_pGLFT->glVertex3i((int)vMins[0], (int)vMaxs[1], (int)vMins[2]);	// G
		m_pGLFT->glVertex3i((int)vMins[0], (int)vMins[1], (int)vMins[2]);	// F
		m_pGLFT->glVertex3i((int)vMaxs[0], (int)vMins[1], (int)vMins[2]);	// E

		// Side 1.
		m_pGLFT->glVertex3i((int)vMins[0], (int)vMaxs[1], (int)vMaxs[2]);	// B
		m_pGLFT->glVertex3i((int)vMins[0], (int)vMaxs[1], (int)vMins[2]);	// G
		m_pGLFT->glVertex3i((int)vMaxs[0], (int)vMaxs[1], (int)vMins[2]);	// H
		m_pGLFT->glVertex3i((int)vMaxs[0], (int)vMaxs[1], (int)vMaxs[2]);	// A

		// Side 2.
		m_pGLFT->glVertex3i((int)vMins[0], (int)vMins[1], (int)vMaxs[2]);	// C
		m_pGLFT->glVertex3i((int)vMins[0], (int)vMins[1], (int)vMins[2]);	// F
		m_pGLFT->glVertex3i((int)vMins[0], (int)vMaxs[1], (int)vMins[2]);	// G
		m_pGLFT->glVertex3i((int)vMins[0], (int)vMaxs[1], (int)vMaxs[2]);	// B

		// Side 3.
		m_pGLFT->glVertex3i((int)vMaxs[0], (int)vMins[1], (int)vMaxs[2]);	// D
		m_pGLFT->glVertex3i((int)vMaxs[0], (int)vMins[1], (int)vMins[2]);	// E
		m_pGLFT->glVertex3i((int)vMins[0], (int)vMins[1], (int)vMins[2]);	// F
		m_pGLFT->glVertex3i((int)vMins[0], (int)vMins[1], (int)vMaxs[2]);	// C

		// Side 4.
		m_pGLFT->glVertex3i((int)vMaxs[0], (int)vMaxs[1], (int)vMaxs[2]);	// A
		m_pGLFT->glVertex3i((int)vMaxs[0], (int)vMaxs[1], (int)vMins[2]);	// H
		m_pGLFT->glVertex3i((int)vMaxs[0], (int)vMins[1], (int)vMins[2]);	// E
		m_pGLFT->glVertex3i((int)vMaxs[0], (int)vMins[1], (int)vMaxs[2]);	// D

		m_pGLFT->glEnd();
	}
	else if(nDrawFlags&RF_LINE)
	{
		m_pGLFT->glBegin(GL_LINE_LOOP);

		m_pGLFT->glVertex3i((int)vMaxs[0], (int)vMaxs[1], (int)vMaxs[2]);	// A
		m_pGLFT->glVertex3i((int)vMins[0], (int)vMaxs[1], (int)vMaxs[2]);	// B
		m_pGLFT->glVertex3i((int)vMins[0], (int)vMins[1], (int)vMaxs[2]);	// C
		m_pGLFT->glVertex3i((int)vMaxs[0], (int)vMins[1], (int)vMaxs[2]);	// D

		m_pGLFT->glEnd();
		m_pGLFT->glBegin(GL_LINE_LOOP);

		// Bottom.
		m_pGLFT->glVertex3i((int)vMaxs[0], (int)vMins[1], (int)vMins[2]);	// E
		m_pGLFT->glVertex3i((int)vMins[0], (int)vMins[1], (int)vMins[2]);	// F
		m_pGLFT->glVertex3i((int)vMins[0], (int)vMaxs[1], (int)vMins[2]);	// G
		m_pGLFT->glVertex3i((int)vMaxs[0], (int)vMaxs[1], (int)vMins[2]);	// H

		m_pGLFT->glEnd();
		m_pGLFT->glBegin(GL_LINE_LOOP);

		// Side 1.
		m_pGLFT->glVertex3i((int)vMaxs[0], (int)vMaxs[1], (int)vMaxs[2]);	// A
		m_pGLFT->glVertex3i((int)vMaxs[0], (int)vMaxs[1], (int)vMins[2]);	// H
		m_pGLFT->glVertex3i((int)vMins[0], (int)vMaxs[1], (int)vMins[2]);	// G
		m_pGLFT->glVertex3i((int)vMins[0], (int)vMaxs[1], (int)vMaxs[2]);	// B

		m_pGLFT->glEnd();
		m_pGLFT->glBegin(GL_LINE_LOOP);

		// Side 2.
		m_pGLFT->glVertex3i((int)vMins[0], (int)vMaxs[1], (int)vMaxs[2]);	// B
		m_pGLFT->glVertex3i((int)vMins[0], (int)vMaxs[1], (int)vMins[2]);	// G
		m_pGLFT->glVertex3i((int)vMins[0], (int)vMins[1], (int)vMins[2]);	// F
		m_pGLFT->glVertex3i((int)vMins[0], (int)vMins[1], (int)vMaxs[2]);	// C

		m_pGLFT->glEnd();
		m_pGLFT->glBegin(GL_LINE_LOOP);

		// Side 3.
		m_pGLFT->glVertex3i((int)vMins[0], (int)vMins[1], (int)vMaxs[2]);	// C
		m_pGLFT->glVertex3i((int)vMins[0], (int)vMins[1], (int)vMins[2]);	// F
		m_pGLFT->glVertex3i((int)vMaxs[0], (int)vMins[1], (int)vMins[2]);	// E
		m_pGLFT->glVertex3i((int)vMaxs[0], (int)vMins[1], (int)vMaxs[2]);	// D

		m_pGLFT->glEnd();
		m_pGLFT->glBegin(GL_LINE_LOOP);

		// Side 4.
		m_pGLFT->glVertex3i((int)vMaxs[0], (int)vMins[1], (int)vMaxs[2]);	// D
		m_pGLFT->glVertex3i((int)vMaxs[0], (int)vMins[1], (int)vMins[2]);	// E
		m_pGLFT->glVertex3i((int)vMaxs[0], (int)vMaxs[1], (int)vMins[2]);	// H	
		m_pGLFT->glVertex3i((int)vMaxs[0], (int)vMaxs[1], (int)vMaxs[2]);	// A

		m_pGLFT->glEnd();
	}

	if(nDrawFlags&RF_TEXTURED)
		m_pGLFT->glEnable(GL_TEXTURE_2D);

	m_pGLFT->glColor3f(1.0F, 1.0F, 1.0F);
}