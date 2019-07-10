// PreviewDialog.cpp : implementation file
//

#include "stdafx.h"
#include "tread3d.h"
//#include "PreviewDialog.h"
#include "TreadSys.h"
#include "GLRenderPackage.h"
#include "DialogControl.h"
#include "GameAPI.h"
#include "Face.h"
#include "Entity.h"
#include "Brush.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPreviewDialog dialog

CPreviewDialog::CPreviewDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CPreviewDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPreviewDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_dTrans[0] = 0;
	m_dTrans[1] = m_dTrans[2] = m_dTrans[3] = 0;

	m_pDC = NULL;

	m_sCategory = "Test";
	m_sName = "Name";
}


void CPreviewDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPreviewDialog)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPreviewDialog, CDialog)
	//{{AFX_MSG_MAP(CPreviewDialog)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_CTLCOLOR()
	ON_WM_SHOWWINDOW()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPreviewDialog message handlers

int CPreviewDialog::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	RegisterDialog(this, DLG_PREVIEW);

	glPack.LoadPackage("dflt_wogl");
	m_pDC = GetDC();
	m_pGL = glPack.GetFunctionTable();

	// Fill in the Pixel Format Descriptor
	PIXELFORMATDESCRIPTOR pfd ;
	memset(&pfd,0, sizeof(PIXELFORMATDESCRIPTOR)) ;

    pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);   
    pfd.nVersion = 1;                           // Version number
	pfd.dwFlags =  PFD_DOUBLEBUFFER |            // Use double buffer
	               PFD_SUPPORT_OPENGL |	        // Use OpenGL
	               PFD_DRAW_TO_WINDOW;          // Pixel format is for a window.
	pfd.iPixelType = PFD_TYPE_RGBA ;
    pfd.cColorBits = 24;                         // 8-bit color
	pfd.cDepthBits = 32;					   	 // 32-bit depth buffer
    pfd.iLayerType = PFD_MAIN_PLANE ;            // Layer type

    int nPixelFormat = ChoosePixelFormat(m_pDC->m_hDC, &pfd);
	if (nPixelFormat == 0)
	{
		Error("ChoosePixelFormat Failed " + GetLastError());
		return -1 ;
	}

	bool bResult = SetPixelFormat(m_pDC->m_hDC, nPixelFormat, &pfd);
	if (!bResult)
	{
		Error("SetPixelFormat Failed " + GetLastError());
		return -1 ;
	}
	
	// Create a rendering context.
    m_glContext = m_pGL->e_wglCreateContext(m_pDC->m_hDC);
	if(!m_glContext)
	{
		Error("wglCreateContext Failed " + GetLastError());
		return -1;
	}
	
	return 0;
}

void CPreviewDialog::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	m_nWidth = cx;
	m_nHeight = cy;

	// Make the rendering context m_glContext current
    bool bResult = m_pGL->e_wglMakeCurrent(m_pDC->m_hDC, m_glContext);
	if (!bResult)
	{
		TRACE("wglMakeCurrent Failed %x\r\n", GetLastError() ) ;
		return ;
	}

    m_dAspect = (GLdouble)m_nWidth / (GLdouble)m_nHeight;
	
	m_pGL->glViewport(0, 0, m_nWidth, m_nHeight);
	m_pGL->glMatrixMode(GL_PROJECTION);
	m_pGL->glLoadIdentity();
   	m_pGL->gluPerspective(90.0, m_dAspect, 1.0, 5000.0);
	m_pGL->glMatrixMode(GL_MODELVIEW);
	m_pGL->glLoadIdentity();

	m_pGL->glClearColor(0.0, 0.0, 0.0, 0.0);
	
	//glEnable(GL_COLOR_MATERIAL);
	//glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	m_pGL->glRenderMode(GL_RENDER);

	m_pGL->glShadeModel(GL_FLAT);
	//glEnable(GL_POLYGON_OFFSET_FILL);
	//glPolygonOffset(1.0, 1.0);

	m_pGL->e_wglMakeCurrent(NULL, NULL);
}

void CPreviewDialog::OnDestroy() 
{
	CDialog::OnDestroy();
	
	m_pGL->e_wglMakeCurrent(NULL, NULL); 
	ReleaseDC(m_pDC);

	if(m_glContext) 
	{
		m_pGL->e_wglDeleteContext(m_glContext);
		m_glContext = NULL;
	}
}

void CPreviewDialog::OnPaint() 
{
	//The next line of code must be present or the buttons wont draw
	//CPaintDC dc(this); // device context for painting

	//make the glContext handle current
	if (!m_pGL->e_wglMakeCurrent(m_pDC->m_hDC, m_glContext))
	{
		ShowWindow(SW_HIDE);
		Error("Cannot make GL context current");
		return;
	}

	// Draw	
	Render(); 

	//Swap Buffers
	SwapBuffers(m_pDC->m_hDC);
	m_pGL->e_wglMakeCurrent(NULL, NULL);
}

void CPreviewDialog::Render()
{
	m_pGL->glMatrixMode(GL_MODELVIEW);
	m_pGL->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	m_pGL->glLoadIdentity();
	m_pGL->glTranslatef(0, 0, m_dTrans[0]);
	m_pGL->glRotatef(m_dTrans[1], 1.0, 0.0, 0.0);
	m_pGL->glRotatef(m_dTrans[2], 0.0, 1.0, 0.0);
	m_pGL->glRotatef(m_dTrans[3], 0.0, 0.0, 1.0);

	m_pGL->glColor3f(1.0, 1.0, 1.0);
	m_pGL->glCallList(1);
	
	//START text overlay
	/*
	m_pGL->glMatrixMode(GL_PROJECTION);
	m_pGL->glPushMatrix();

	m_pGL->glLoadIdentity();
	//m_pGL->glOrtho(0, m_nWidth, 0, m_nHeight, -1.0, 1.0);

	m_pGL->glMatrixMode(GL_MODELVIEW);
	m_pGL->glLoadIdentity();
   
	m_pGL->glColor3f(1.0, 1.0, 1.0);
	Print(25, 15, "%s \\ %s", m_sCategory, m_sName);
	
	m_pGL->glMatrixMode(GL_PROJECTION);
	m_pGL->glPopMatrix();
	*/
	//END text overlay
	
	m_pGL->glFlush();
}

void CPreviewDialog::DrawGroup(CObjectGroup* pGroup)
{
	DrawObjectList(pGroup->GetObjectList());
}

void CPreviewDialog::DrawObjectList(CLinkedList<CBaseObject>* pList)
{
	CBaseObject* pObject;

	pList->First();
	for(pObject = pList->GetCurItem(); pObject != NULL; pObject = pList->GetNextItem())
	{
		if(pObject->GetObjectClass() == OBJECT_CLASS_ENTITY)
			DrawEntity((CEntity*)pObject);
		else if(pObject->GetObjectClass() == OBJECT_CLASS_BRUSH)
			DrawBrush((CQBrush*)pObject);
		else if(pObject->GetObjectClass() == OBJECT_CLASS_GROUP)
			DrawGroup((CObjectGroup*)pObject);
	}
}

void CPreviewDialog::PrepareObject()
{
	if (!m_pGL->e_wglMakeCurrent(m_pDC->m_hDC, m_glContext))
	{
		TRACE("wglMakeCurrent Failed %x\r\n", GetLastError());
		return;
	}

	m_pModel = GetGameAPI()->GetCurrentGame()->GetCurrentModel();

	//initialize rotation values
	m_dTrans[1] = -90;
	m_dTrans[2] = 0;
	m_dTrans[3] = 90;

	m_dTrans[0] = m_pModel->vMins[2] - m_pModel->vMaxs[2] - 250;

	m_vTrans[0] = -m_pModel->vOrigin[0];
	m_vTrans[1] = -m_pModel->vOrigin[1];
	m_vTrans[2] = -m_pModel->vOrigin[2];

	CLinkedList<CBaseObject>* pList = m_pModel->m_pObjectList;
	if(pList == NULL)
		return;

	m_pGL->glNewList(1, GL_COMPILE);

	DrawObjectList(pList);	
	m_pGL->glEndList();
	
	InvalidateRect(NULL, true);
}

void CPreviewDialog::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	if(bShow)
	{
		if(m_pDC == NULL)
			m_pDC = GetDC();
		else
		{
			ReleaseDC(m_pDC);
			m_pDC = GetDC();
		}
		PrepareObject();
	}
	else
	{
		if(m_pDC != NULL)
		{
			ReleaseDC(m_pDC);	
			m_pDC = NULL;
		}
	}
}

void CPreviewDialog::DrawEntity(CEntity* pEntity)
{

	CLinkedList<CBaseObject>* pList = pEntity->GetObjectList();
	if(pList == NULL)
		return;

	DrawObjectList(pList);

}

void CPreviewDialog::DrawBrush(CQBrush* pBrush)
{
	int i;
	CFace* pFace;
	vec_t* vV;
	CLinkedList<CFace>* pList;

	pList = pBrush->m_pFaceList;

	pList->First();
	
	
	for(pFace = pList->GetCurItem(); pFace != NULL; pFace = pList->GetNextItem())
	{
		m_pGL->glBegin(GL_LINE_LOOP);
		for(i = 0; i < pFace->GetNumPoints(); i++)
		{
			vV = pFace->m_vPoints[i];
			m_pGL->glVertex3fv(vV);
		}
		m_pGL->glEnd();
	}
}

void CPreviewDialog::OnLButtonDown(UINT nFlags, CPoint point) 
{
	BeginWaitCursor();
	SetCapture();

	m_ptLast = point;

	CDialog::OnLButtonDown(nFlags, point);
}

void CPreviewDialog::OnLButtonUp(UINT nFlags, CPoint point) 
{
	ReleaseCapture();
	EndWaitCursor();
	
	CDialog::OnLButtonUp(nFlags, point);
}

void CPreviewDialog::OnMouseMove(UINT nFlags, CPoint point) 
{
	int xDiff;
	int yDiff;

	//positive difference = movement to the right
	xDiff = point.x - m_ptLast.x;
	//positive difference = movement to the bottom
	yDiff = point.y - m_ptLast.y;
	
	if(nFlags & MK_LBUTTON)
	{
		m_dTrans[1] += yDiff;
		m_dTrans[3] += xDiff;

		if(m_dTrans[1] >= 180)
			m_dTrans[1] = -180;
		else if(m_dTrans[1] <= -180)
			m_dTrans[1] = 180;

		if(m_dTrans[3] >= 180)
			m_dTrans[3] = -180;
		else if(m_dTrans[2] <= -180)
			m_dTrans[3] = 180;
	}

	if(nFlags & MK_RBUTTON)
	{
		m_dTrans[0] -= yDiff * 2;

		if(m_dTrans[0] > 0)
			m_dTrans[0] = 0;
	}

	m_ptLast = point;
	
	CDialog::OnMouseMove(nFlags, point);
}

void CPreviewDialog::OnRButtonDown(UINT nFlags, CPoint point) 
{
	BeginWaitCursor();
	SetCapture();

	m_ptLast = point;
	
	CDialog::OnRButtonDown(nFlags, point);
}

void CPreviewDialog::OnRButtonUp(UINT nFlags, CPoint point) 
{
	ReleaseCapture();
	EndWaitCursor();
	
	CDialog::OnRButtonUp(nFlags, point);
}

HBRUSH CPreviewDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = (HBRUSH)GetStockObject(BLACK_BRUSH);
	return hbr;
}


