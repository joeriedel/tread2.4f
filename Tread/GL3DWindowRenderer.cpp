// GL3DWINDOWRENDERER.CPP
// Copyright © 1999 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#include "stdafx.h"
#include "GLRenderPackage.h"
#include "GL3DWindowRenderer.h"
#include "TreadSys.h"
#include "ColorTable.h"
#include "GL3DBrushRenderer.h"
#include "GL3DEntityRenderer.h"
#include "Camera.h"
#include "MapView.h"
#include "Tread3D.h"
#include "PluginAPI.h"

#ifdef _WIN32
static PIXELFORMATDESCRIPTOR gGlPFD = 
{
	sizeof( PIXELFORMATDESCRIPTOR ),
	1,
	PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
	PFD_TYPE_RGBA,
	16,
	0, 0, 0, 0, 0, 0,
	0,
	0,
	0,
	0, 0, 0, 0,
	16,                // 16-bit depth buffer
	0,                  // no stencil buffer
	0,                  // no aux buffers
	0,			/* main layer */
	0,	
	0, 0, 0
};
#endif

#define GL_TEXADD_MASK		0xFFFFFFFF
#define GL_TEXADD_SHIFT		0
#define MAX_GL_NAMES		0xFFFFFFFF

#define GL_MODULATE_SELECT

// Generates the pixel format.
bool CGL3DWindowRenderer::GeneratePixelFormat(CDC* pDC)
{
#ifdef _WIN32
	// Select the correct pixel format.
	m_nPixelFormat = ChoosePixelFormat(pDC->m_hDC, &gGlPFD);

	return m_nPixelFormat != 0;
#else
	return true;
#endif
}

// Set's the GL pixel format.
bool CGL3DWindowRenderer::SetPixelFormat(CDC* pDC)
{
#ifdef _WIN32
	if(!::SetPixelFormat(pDC->m_hDC, m_nPixelFormat, &gGlPFD))
		return(false);
#endif
	return true;
}

void CGL3DWindowRenderer::DrawObject(CMapView* pView, CTread3DDoc* pDocument, CDC* pDC, CBaseObject* pObject, int nObjectClass, int nObjectInterfaces, int nDrawFlags, int nFlags)
{
	// Allowing plugins to intercept drawing is done on a per renderer basis.
	// We allow it.
	if(GetPluginAPI()->m_n3DPreDrawFlags&nObjectClass)
	{
		if(GetPluginAPI()->PreDrawObject(pView, pDocument, this, pDC, m_pGLFT, pObject, nObjectClass, nObjectInterfaces, nDrawFlags, nFlags, PDT_3D, GL3DWINDOWRENDERER_UID))
			return;
	}

	switch(nObjectClass)
	{
	case OBJECT_CLASS_BRUSH:
		m_pBR->RenderBrush((CQBrush*)pObject, nDrawFlags, nFlags);
		break;
	case OBJECT_CLASS_ENTITY:
		m_pER->RenderEntity((CEntity*)pObject, nDrawFlags, nFlags);
	}

	if(GetPluginAPI()->m_n3DPostDrawFlags&nObjectClass)
		GetPluginAPI()->PostDrawObject(pView, pDocument, this, pDC, m_pGLFT, pObject, nObjectClass, nObjectInterfaces, nDrawFlags, nFlags, PDT_3D, GL3DWINDOWRENDERER_UID);
}

void CGL3DWindowRenderer::OnSize(CMapView* pView, CDC* pDC)
{
}

void CGL3DWindowRenderer::ClearWindow(CMapView* pView, CTread3DDoc* pDocument, CDC* pDC, int nClearColor, int nFlags)
{
	m_pGLFT->glClearColor(GetColorTable()->GetFloatV(CL_3D_BACKGROUND)[0], GetColorTable()->GetFloatV(CL_3D_BACKGROUND)[1], GetColorTable()->GetFloatV(CL_3D_BACKGROUND)[2], 0.5F);
	m_pGLFT->glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
}

void CGL3DWindowRenderer::DrawI_TARGET(CMapView* pView, CTread3DDoc* pDocument, CDC* pDC, CBaseObject* pObject, int nDrawFlags, int nType)
{
	CBaseTargetObject* pT = (CBaseTargetObject*)pObject->GetInterface(I_TARGET);
	CBaseTransformableObject* pSrcTR = (CBaseTransformableObject*)pObject->GetInterface(I_TRANSFORMABLE);
	CString sName = pT->GetTargetName();
	if(sName == "")
		return;
	TTARGET* pTSet = pDocument->GetTargetAPI()->FindTargetSet(sName);
	if(pTSet == NULL)
		return;

	//vec_t* pColor = GetColorTable()->GetFloatV(CL_LINK);
	//m_pGLFT->glColor4f(pColor[0], pColor[1], pColor[2], GL3D_BASE_ALPHA);
	m_pGLFT->glDisable(GL_TEXTURE_2D);
	//m_pGLFT->glDisable(GL_BLEND);
	m_pGLFT->glColor3fv(GetColorTable()->GetFloatV(CL_LINK));
	m_pGLFT->glBegin(GL_LINES);

	TREF* pRef;
	pTSet->TRefList.First();
	for(pRef = pTSet->TRefList.GetCurItem(); pRef != NULL; pRef = pTSet->TRefList.GetNextItem())
	{
		CBaseTransformableObject* pDstTR = (CBaseTransformableObject*)pRef->pObject->GetInterface(I_TRANSFORMABLE);

		// Draw the line.
		m_pGLFT->glVertex3fv(pDstTR->m_vOrigin);
		m_pGLFT->glVertex3fv(pSrcTR->m_vOrigin);
	}

	m_pGLFT->glEnd();

	if(nDrawFlags&RF_TEXTURED)
		m_pGLFT->glEnable(GL_TEXTURE_2D);
}

void CGL3DWindowRenderer::DrawBBox(CMapView* pView, CTread3DDoc* pDocument, CDC* pDC, vec3_t vMins, vec3_t vMaxs, int nDrawColor, int nFlags)
{
	// No 3D selbox.
	if((pDocument->GetGlobalRenderFlags()|GetRenderAPI()->GetGlobalRenderFlags())&RF_NO3DSELBOX)
		return;

	m_pGLFT->glDisable(GL_TEXTURE_2D);
	m_pGLFT->glDepthFunc(GL_ALWAYS);
	m_pGLFT->glColor3fv(GetColorTable()->GetFloatV(nDrawColor));

	// Draw top.
	m_pGLFT->glBegin(GL_LINE_LOOP);
	{
		m_pGLFT->glVertex3i(vMins[0], vMins[1], vMins[2]);
		m_pGLFT->glVertex3i(vMaxs[0], vMins[1], vMins[2]);
		m_pGLFT->glVertex3i(vMaxs[0], vMaxs[1], vMins[2]);
		m_pGLFT->glVertex3i(vMins[0], vMaxs[1], vMins[2]);
	}
	m_pGLFT->glEnd();
	m_pGLFT->glBegin(GL_LINE_LOOP);
	{
		m_pGLFT->glVertex3i(vMins[0], vMins[1], vMaxs[2]);
		m_pGLFT->glVertex3i(vMaxs[0], vMins[1], vMaxs[2]);
		m_pGLFT->glVertex3i(vMaxs[0], vMaxs[1], vMaxs[2]);
		m_pGLFT->glVertex3i(vMins[0], vMaxs[1], vMaxs[2]);
	}
	m_pGLFT->glEnd();
	m_pGLFT->glBegin(GL_LINES);
	{
		m_pGLFT->glVertex3i(vMins[0], vMins[1], vMins[2]);
		m_pGLFT->glVertex3i(vMins[0], vMins[1], vMaxs[2]);
		m_pGLFT->glVertex3i(vMaxs[0], vMins[1], vMins[2]);
		m_pGLFT->glVertex3i(vMaxs[0], vMins[1], vMaxs[2]);
		m_pGLFT->glVertex3i(vMaxs[0], vMaxs[1], vMins[2]);
		m_pGLFT->glVertex3i(vMaxs[0], vMaxs[1], vMaxs[2]);
		m_pGLFT->glVertex3i(vMins[0], vMaxs[1], vMins[2]);
		m_pGLFT->glVertex3i(vMins[0], vMaxs[1], vMaxs[2]);
	}
	m_pGLFT->glEnd();
	m_pGLFT->glEnable(GL_TEXTURE_2D);
	m_pGLFT->glDepthFunc(GL_LESS);
	m_pGLFT->glColor4f(1.0F, 1.0F, 1.0F, GL3D_BASE_ALPHA);
}

// Initializes the rendering window.
bool CGL3DWindowRenderer::Initialize(CMapView* pView, CDC* pDC, int nType)
{
	m_nCurName = 0;

	Debug("gl3dwindowrenderer init\n");
	Debug("... making pixel format\n");

	if(!GeneratePixelFormat(pDC))
		return false;

	Debug("... setting pixel format\n");

	// Set the pixel format.
	SetPixelFormat(pDC);

	Debug("... making context\n");

#ifdef _WIN32
	// Create the rendering context.
	m_hglrc = m_pGLFT->e_wglCreateContext(pDC->m_hDC);
	if(m_hglrc == NULL)
		return false;
	
	Debug("... binding context\n");

	m_pGLFT->e_wglMakeCurrent(pDC->m_hDC, m_hglrc);

#endif

	Debug("... seting gl defaults\n");

	// Set Initial states.
	m_pGLFT->glEnable(GL_DEPTH_TEST);
	m_pGLFT->glEnable(GL_CULL_FACE);
	m_pGLFT->glEnable(GL_TEXTURE_2D);
	m_pGLFT->glFrontFace(GL_CW);
	m_pGLFT->glCullFace(GL_BACK);
	m_pGLFT->glShadeModel(GL_FLAT);

	m_pGLFT->glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	m_pGLFT->glClearColor(0.0F, 0.0F, 0.0F, 0.5F);
	m_pGLFT->glDepthRange(0.0, 1.0);
	m_pGLFT->glDepthFunc(GL_LESS);

	m_pGLFT->glShadeModel(GL_FLAT);
	m_pGLFT->glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	//m_pGLFT->glPixelTransferf(GL_ALPHA_BIAS, 0.20F);
   	
	m_pGLFT->glBindTexture(GL_TEXTURE_2D, 0);

	m_bBilinear = (pView->GetDocument()->GetGlobalRenderFlags()&RF_NOBILINEARFILTER) == RF_NOBILINEARFILTER;	
	m_pGLFT->glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

	m_pGLFT->glDisable(GL_DEPTH_TEST);
	m_pGLFT->glDisable(GL_CULL_FACE);
	m_pGLFT->glDisable(GL_TEXTURE_2D);

	Debug("... vendor: %s\n", m_pGLFT->glGetString(GL_VENDOR));
	Debug("... renderer: %s\n", m_pGLFT->glGetString(GL_RENDERER));
	Debug("... version: %s\n", m_pGLFT->glGetString(GL_VERSION));
	Debug("done\n");

#ifdef _WIN32
	m_pGLFT->e_wglMakeCurrent(NULL, NULL);
#endif

	return true;
}

bool CGL3DWindowRenderer::Shutdown(CMapView* pView, CDC* pDC)
{
	Debug("gl3dwindowrenderer shutdown\n");
	
	// Build a list of texture id's and delete them.
	int i, nNumTexs;
	unsigned int *pTexNames;
	CTexture** pTexs;

	pTexs = GetTexAPI()->GetFullPakTexList(nNumTexs);
	pTexNames = new unsigned int[nNumTexs];

	Debug("... killing %d textures\n", nNumTexs);

	for(i = 0; i < nNumTexs; i++)
		pTexNames[i] = pTexs[i]->m_nUniqueID;

#ifdef _WIN32
	// Make the context current.
	m_pGLFT->e_wglMakeCurrent(pDC->m_hDC, m_hglrc);
	// Delete textures.
	m_pGLFT->glDeleteTextures(nNumTexs, pTexNames);
#else
	// Delete textures.
	m_pGLFT->glDeleteTextures(nNumTexs, (const unsigned long*)pTexNames);
#endif
	Debug("... destroying context\n");

	// Destroy GL bindings.
#ifdef _WIN32
	m_pGLFT->e_wglMakeCurrent(NULL, NULL);
	if(m_hglrc != NULL)
		m_pGLFT->e_wglDeleteContext(m_hglrc);
	m_hglrc = NULL;
#endif

	delete[] pTexNames;

	Debug("done\n");

	return true;
}

void CGL3DWindowRenderer::SetBias(bool bBias, int nDrawFlags)
{
	// No bias in pickmode.
	if(nDrawFlags & RF_PICKMODE)
		return;

	if(m_bBias == bBias)
		return;

	if(bBias)
	{
		if(nDrawFlags&RF_TEXTURED)
			m_pGLFT->glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		m_pGLFT->glColor4f(1.0F, 0.5F, 0.5F, GL3D_BASE_ALPHA);
	}
	else
	{
		if((nDrawFlags&(RF_TEXTURED|RF_SELECTED|RF_TRANSPARENT))==RF_TEXTURED || (nDrawFlags&(RF_SHADED|RF_TEXTURED|RF_SELECTED|RF_TRANSPARENT))==(RF_SHADED|RF_TEXTURED))
			m_pGLFT->glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
		m_pGLFT->glColor4f(1.0F, 1.0F, 1.0F, GL3D_BASE_ALPHA);
	}
}

void CGL3DWindowRenderer::MakePowerOf2(CTexture* pTex)
{
	// Get dimensions.
	int w, h;
	int b_mask;
	unsigned char* data;

	if(pTex->m_nSclWidth > 0)
	{
		// Get the width.
		b_mask=0x80000000;
		while(!(pTex->m_nSclWidth & b_mask))
			b_mask >>= 1;
		w = -(b_mask); 
	}
	else
		w = 1;

	if(pTex->m_nSclHeight > 0)
	{
		// Get the height.
		b_mask = 0x80000000;
		while(!(pTex->m_nSclHeight & b_mask))
			b_mask >>= 1;
		h = -(b_mask);
	}
	else
		h = 1;

	// Same?
	if(w == pTex->m_nSclWidth &&
		h == pTex->m_nSclHeight)
		return;

	// Allocate new memory.
	data = new unsigned char[w * h * 3];
	if(!data)
		return;

	// Scale the image.
	m_pGLFT->gluScaleImage(GL_RGB, pTex->m_nSclWidth, pTex->m_nSclHeight, GL_UNSIGNED_BYTE, pTex->m_pData,
		w, h, GL_UNSIGNED_BYTE, data);

	// Store the resulting image.
	delete pTex->m_pData;
	pTex->m_pData = data;
	pTex->m_nSclWidth = w;
	pTex->m_nSclHeight = h;
	pTex->m_nSize = w * h * 3;
}

void CGL3DWindowRenderer::DetachTexture(CTexture* pTexture)
{
#ifdef _WIN32
	m_pGLFT->glDeleteTextures(1, (const unsigned int*)&pTexture->m_nUniqueID);
#else
	m_pGLFT->glDeleteTextures(1, (const unsigned long*)&pTexture->m_nUniqueID);
#endif
	if(m_pCurTex == pTexture)
		BindTexture(NULL, 0);
}

void CGL3DWindowRenderer::BindTexture(CTexture* pTexture, int nDrawFlags, bool bClamp)
{
	bool bNoPrint = false;

	// No bind in pick mode.
	if(nDrawFlags & RF_PICKMODE)
		return;

	if(pTexture == NULL)
	{
		m_pCurTex = NULL;
		m_nTextureDrawFlags = 0;
		//m_pGLFT->glColor4f(1.0F, 1.0F, 1.0F, GL3D_BASE_ALPHA);
		return;
	}

	// Already selected?
	if(m_pCurTex != NULL)
	{
		if(m_pCurTex == pTexture && (m_nTextureDrawFlags&(RF_TEXTURED|RF_SOLID|RF_LINE)) == (nDrawFlags&(RF_TEXTURED|RF_SOLID|RF_LINE)))
			return;
	}
	
	// Force a re-download if bias has changed.
	m_nTextureDrawFlags = nDrawFlags;

	// Not in textured mode.
	if(nDrawFlags & (RF_SOLID|RF_LINE))
	{
		if(m_bBias)
		{
			m_pGLFT->glColor4f(1.0F, 0.0F, 0.0F, GL3D_BASE_ALPHA);
			return;
		}

		if(!pTexture->Load())
		{
			Debug("bindTex: failed to load: " + pTexture->m_sName + "\n");
			Warning("bindTex: failed to load: " + pTexture->m_sName + "\n");
			return;
		}

		// Index to the center of the texture, and pick a color.
		float r, g, b;
		unsigned char* pData;

		pData = pTexture->m_pData + (pTexture->m_nSize >> 1);
		r = pData[0] / 255.0F;
		b = pData[1] / 255.0F;
		g = pData[2] / 255.0F;

		m_pGLFT->glColor4f(r, g, b, GL3D_BASE_ALPHA);

		return;
	}

	bool bDownload = m_pGLFT->glIsTexture(pTexture->m_nUniqueID) == false;
	//Console("%d %d\n", pTexture->m_nUniqueID, bDownload);

	// Bind our texture.
	if(m_nCurName != (unsigned int)pTexture->m_nUniqueID)
		m_pGLFT->glBindTexture(GL_TEXTURE_2D, pTexture->m_nUniqueID);
	m_nCurName = pTexture->m_nUniqueID;

	// Do we need to download it?
	if(bDownload)
	{
		if(!pTexture->Load())
		{
			Debug("bindTex: failed to load: " + pTexture->m_sName + "\n");
			Warning("bindTex: failed to load: " + pTexture->m_sName + "\n");
			m_pGLFT->glBindTexture(GL_TEXTURE_2D, 0);
			return;
		}
		
		MakePowerOf2(pTexture);

#ifdef _WIN32
		m_pGLFT->glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, pTexture->m_nSclWidth, pTexture->m_nSclHeight, 
			0, GL_BGR_EXT, GL_UNSIGNED_BYTE, pTexture->m_pData);
#endif

		if(bClamp)
		{
			m_pGLFT->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
			m_pGLFT->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		}
		else
		{
			m_pGLFT->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			m_pGLFT->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		}

		if(m_bBilinear)
		{
			m_pGLFT->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			m_pGLFT->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		}
		else
		{
			m_pGLFT->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			m_pGLFT->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		}

		//Debug("...gl3d: downloaded %s\n", pTexture->m_sName); 
		//Console("...gl3d: downloaded %s\n", pTexture->m_sName);

		m_nDownloadCount++;
	}

	m_pCurTex = pTexture;
}

void CGL3DWindowRenderer::EnableTransparency(bool bTransparent)
{
	if(bTransparent)
	{
		m_pGLFT->glEnable(GL_BLEND);
		m_pGLFT->glDisable(GL_CULL_FACE);
		m_pGLFT->glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	
		m_pGLFT->glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
		m_pGLFT->glColor4f(1.0f, 1.0f, 1.0f, GL3D_BASE_ALPHA);
		//m_pGLFT->glDepthFunc(GL_LESS);
		m_pGLFT->glDepthMask(false);
	}
	else
	{
		m_pGLFT->glDisable(GL_BLEND);
		m_pGLFT->glEnable(GL_CULL_FACE);
		m_pGLFT->glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
		//m_pGLFT->glDepthFunc(GL_LESS);
		m_pGLFT->glDepthMask(true);
	}
}

CBaseObject* CGL3DWindowRenderer::GetHitObject(CMapView* pView, CTread3DDoc* pDocument, int nClassFlags, int nMouseX, int nMouseY)
{
	CDC* pDC = pView->GetDC();

	m_nMouseX = nMouseX;
	m_nMouseY = nMouseY;

	if(!LockDevice(pView, pDocument, pDC, RF_PICKMODE))
	{
		pView->ReleaseDC(pDC);
		return NULL;
	}

	// Draw normal objects first.
	GetRenderAPI()->DrawObjectList(pView, pDocument, pDC, this, pDocument->GetObjectList(), RF_PICKMODE, 0);
	// Draw the selected list.
	GetRenderAPI()->DrawObjectList(pView, pDocument, pDC, this, pDocument->GetSelectedObjectList(), RF_PICKMODE|RF_SELECTED, 0);
	// Draw UI list.
	GetRenderAPI()->DrawObjectList(pView, pDocument, pDC, this, pDocument->GetUIObjectList(), RF_PICKMODE, 0);

	// Flush.
	m_pGLFT->glFlush();

	int nNumHits = m_pGLFT->glRenderMode(GL_RENDER);

	// Hit nothing, bail.
	if(nNumHits == 0)
	{
		UnlockDevice(pView, pDocument, pDC, RF_PICKMODE);
		pView->ReleaseDC(pDC);
		return NULL;
	}

	int i;
	unsigned int nObjectAddress=0;
	unsigned int nDist;
	unsigned int nMinDist=0xFFFFFFFF;
	unsigned int* pPickBuff = m_nPickBuff;

	for(i = 0; i < nNumHits; i++)
	{
		// Skip name.
		pPickBuff++;
		// Get dist;
		nDist = *pPickBuff;
		// Skip.
		pPickBuff++; pPickBuff++;
		if(nDist < nMinDist)
		{
			nMinDist = nDist;
			nObjectAddress = *pPickBuff;
		}
		// To next one.
		pPickBuff++;	
	}

	UnlockDevice(pView, pDocument, pDC, RF_PICKMODE);
	pView->ReleaseDC(pDC);
	
	return (CBaseObject*)nObjectAddress;
}

bool CGL3DWindowRenderer::LockDevice(CMapView* pView, CTread3DDoc* pDocument, CDC* pDC, int nDrawFlags)
{
#ifdef _WIN32
	if(m_hglrc == NULL)
		return false;

	if(!m_pGLFT->e_wglMakeCurrent(pDC->m_hDC, m_hglrc))
		return false;
#endif

	m_nDownloadCount = 0;

	// Setup the viewer orientation.
	CCamera* pCam = pView->GetCamera();
	/*if(pCam == NULL)
	{
		Warning("CGL3DWindowRenderer::LockDevice(): no active camera\n");
		m_pGLFT->e_wglMakeCurrent(NULL, NULL);
		return false;
	}*/

	// Set rendering mode.

	// Pick mode?
	if(nDrawFlags&RF_PICKMODE)
	{
#ifdef _WIN32
		m_pGLFT->glSelectBuffer(PICK_BUFF_SIZE, m_nPickBuff);
#else
		m_pGLFT->glSelectBuffer(PICK_BUFF_SIZE, (unsigned long*)m_nPickBuff);
#endif
		m_pGLFT->glRenderMode(GL_SELECT);
		m_pGLFT->glInitNames();
		m_pGLFT->glPushName(0);
		m_pGLFT->glClear(GL_DEPTH_BUFFER_BIT);
	}
	else
	{
		m_pGLFT->glColor3f(1.0F, 1.0F, 1.0F);
		m_pGLFT->glRenderMode(GL_RENDER);
		if(nDrawFlags & (RF_TEXTURED|RF_SOLID))
		{
			if(nDrawFlags&RF_TEXTURED)
				m_pGLFT->glEnable(GL_TEXTURE_2D);
			m_pGLFT->glEnable(GL_DEPTH_TEST); // Enable depth testing.
			m_pGLFT->glEnable(GL_CULL_FACE);	 // Enable culling.
		}
	}

	float fWidth = (float)pView->m_rcClientRect.right;
	float fHeight = (float)pView->m_rcClientRect.bottom;

	m_pGLFT->glMatrixMode(GL_PROJECTION);
	m_pGLFT->glLoadIdentity();

	if(nDrawFlags&RF_PICKMODE)
	{
		int nViewPort[4];

#ifdef _WIN32
		m_pGLFT->glGetIntegerv(GL_VIEWPORT, nViewPort);
		m_pGLFT->gluPickMatrix(m_nMouseX, nViewPort[3]-m_nMouseY, 1.0F, 1.0F, nViewPort);
#else
		m_pGLFT->glGetIntegerv(GL_VIEWPORT, (long*)nViewPort);
		m_pGLFT->gluPickMatrix(m_nMouseX, nViewPort[3]-m_nMouseY, 1.0F, 1.0F, (long*)nViewPort);
#endif
	}
	else
	{
		m_pGLFT->glViewport(0, 0, fWidth, fHeight);
	}

	m_pGLFT->gluPerspective(90.0F, fWidth / fHeight, 1.0F, (float)GetRegInt(I_BACKPLANE));

	m_pGLFT->glMatrixMode(GL_MODELVIEW);
	m_pGLFT->glLoadIdentity();

	m_pGLFT->glScalef(m_vCamScale[0], m_vCamScale[1], m_vCamScale[2]);
	if(pCam != NULL)
	{
		m_pGLFT->gluLookAt(pCam->GetOrigin()[0], pCam->GetOrigin()[1], pCam->GetOrigin()[2], 
			pCam->GetLook()[0], pCam->GetLook()[1], pCam->GetLook()[2], 
			0.0F, 0.0F, 1.0F);
	}
	else
	{
		m_pGLFT->glRotatef(m_vCamRot[0], 1.0F, 0.0F, 0.0F);
		m_pGLFT->glRotatef(m_vCamRot[1], 0.0F, 1.0F, 0.0F);
		m_pGLFT->glRotatef(m_vCamRot[2], 0.0F, 0.0F, 1.0F);
		m_pGLFT->glTranslatef(-m_vCamPos[0], -m_vCamPos[1], -m_vCamPos[2]);
	}

	return true;
}

bool CGL3DWindowRenderer::UnlockDevice(CMapView* pView, CTread3DDoc* pDocument, CDC* pDC, int nDrawFlags)
{
	if(nDrawFlags&RF_PICKMODE)
	{
	}
	else
	{
		m_pGLFT->glDisable(GL_TEXTURE_2D);
		m_pGLFT->glDisable(GL_DEPTH_TEST);
		m_pGLFT->glDisable(GL_CULL_FACE);

		if(m_nDownloadCount > 0)
			Debug("gl3d: downloaded %d texture(s) to card\n", m_nDownloadCount);
	}

#ifdef _WIN32
	return m_pGLFT->e_wglMakeCurrent(NULL, NULL);
#else
	return true;
#endif
}

void CGL3DWindowRenderer::Flush(CMapView* pView, CTread3DDoc* pDocument, CDC* pDC)
{
	m_pGLFT->glFlush();
#ifdef _WIN32
	SwapBuffers(pDC->m_hDC);
#endif
}

CGL3DWindowRenderer::CGL3DWindowRenderer(WOGLFUNCTIONTABLE* pGLFT) : CBaseWindowRenderer()
{
	// Get the GL function table.
	m_pGLFT = pGLFT;

	m_pCurTex = NULL;
	m_bBias = false;
#ifdef _WIN32
	m_hglrc = NULL;
#endif
	m_nBiasDrawFlags = m_nTextureDrawFlags = 0;

	m_pBR = new CGL3DBrushRenderer(this, m_pGLFT);
	m_pER = new CGL3DEntityRenderer(m_pGLFT);

}

CGL3DWindowRenderer::~CGL3DWindowRenderer()
{
	if(m_pBR)
		delete m_pBR;
	if(m_pER)
		delete m_pER;

	m_pBR = NULL;
	m_pER = NULL;
}

