// GLRENDERPACKAGE.CPP.
// Copyright © 1999 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#include "stdafx.h"
#include "TreadSys.h"
#include "GLRenderPackage.h"
#include "GL3DWindowRenderer.h"
#include "GLWindowRenderer.h"
#include "Tread3D.h"

#define OPENGL_DLL		"\\opengl32.dll"
#define GLU_DLL			"\\glu32.dll"

CBaseWindowRenderer* CGLRenderPackage::GetWindowRenderer(CMapView* pView, CDC* pDC, int nType)
{
	if(nType == VIEW_3D)
	{
		CGL3DWindowRenderer* pWR=NULL;
		pWR = new CGL3DWindowRenderer(&m_woglFunctionTable);
		if(pWR->Initialize(pView, pDC, nType))
			return pWR;
		else
			delete pWR;
	}
	else
	{
		CGLWindowRenderer* pWR=NULL;
		pWR = new CGLWindowRenderer(&m_woglFunctionTable);
		if(pWR->Initialize(pView, pDC, nType))
			return pWR;
		else
			delete pWR;
	}

	return NULL;
}

CGLRenderPackage::CGLRenderPackage()
{
#ifdef _WIN32
	m_hOpenGLLib = NULL;
	m_hGluLib = NULL;
#endif
}

CGLRenderPackage::~CGLRenderPackage()
{
	UnloadPackage();
}

bool CGLRenderPackage::StaticClaimPackageName(CString sName)
{
	if(sName == "hdw_wogl" || sName == "sft_wogl" || sName == "dflt_wogl")
		return true;

	return false;
}

// Loads the package.
bool CGLRenderPackage::LoadPackage(CString sName)
{
#ifdef _WIN32
	// For now just load the default GL drivers installed on the machine.
	if(sName == "hdw_wogl")
	{

		// Get windows system.
		char szWinSys[256];
		GetSystemDirectory(szWinSys, 256);
		CString sWinSys = szWinSys;
		//sWinSys += '\\';
		if(!LoadGLLibrary(sWinSys + OPENGL_DLL, sWinSys + GLU_DLL))
			return false;

		m_sName = sName;
	}
	else if(sName == "sft_wogl")
	{
		if(!LoadGLLibrary(GetRegString(S_TREAD_PATH) + OPENGL_DLL, GetRegString(S_TREAD_PATH) + GLU_DLL))
			return false;
		m_sName = sName;
	}
	else if(sName == "dflt_wogl")
		return LoadPackage("hdw_wogl") || LoadPackage("sft_wogl");
	else
		return false;
#endif

	return true;
}


bool CGLRenderPackage::UnloadPackage()
{
#ifdef _WIN32
	if(m_hOpenGLLib != NULL)
		FreeLibrary(m_hOpenGLLib);
	if(m_hGluLib != NULL)
		FreeLibrary(m_hGluLib);

	m_hOpenGLLib = NULL;
	m_hGluLib = NULL;
#endif
	return true;
}

// Loads a GL routine.
int* CGLRenderPackage::LoadGLRoutine(CString sFunction)
{
	int* func=0;

#ifdef _WIN32
	func = (int*)GetProcAddress(m_hOpenGLLib, sFunction);
	if(!func)
		return (int*)GetProcAddress(m_hGluLib, sFunction);
#endif

	return (int*)func;
}

// Loads a GL Library.
bool CGLRenderPackage::LoadGLLibrary(CString sOpenGL, CString sGlu)
{
#ifdef _WIN32
	char szName[256];

	if(m_hOpenGLLib != NULL)
		FreeLibrary(m_hOpenGLLib);
	if(m_hGluLib != NULL)
		FreeLibrary(m_hGluLib);

	m_hOpenGLLib = m_hGluLib = NULL;
	m_hOpenGLLib = LoadLibrary(sOpenGL);
	if(m_hOpenGLLib == NULL)
		return false;

	GetModuleFileName((HMODULE)m_hOpenGLLib, szName, 256);
	Debug("gl: loaded: %s\n", szName);
	CString sString; sString.Format("gl: loaded: %s", szName);
	PrintSplashLine(sString);

	if(sGlu != "")
	{
		m_hGluLib = LoadLibrary(sGlu);
		if(m_hGluLib == NULL)
			return false;
		
		GetModuleFileName((HMODULE)m_hGluLib, szName, 256);
		Debug("gl: loaded: %s\n", szName);
		sString.Format("gl: loaded: %s", szName);
		PrintSplashLine(sString);
	}
#endif

	PrintSplashLine("gl: loading function tables");
	m_woglFunctionTable.glArrayElement = (void (APIENTRY*)(GLint))LoadGLRoutine("glArrayElement");
	if(m_woglFunctionTable.glArrayElement == NULL)
		return false;
	m_woglFunctionTable.glBegin = (void (APIENTRY*)(GLenum mode))LoadGLRoutine("glBegin");
	if(m_woglFunctionTable.glBegin == NULL)
		return false;
	m_woglFunctionTable.glBindTexture = (void (APIENTRY*) (GLenum, GLuint))LoadGLRoutine("glBindTexture");
	if(m_woglFunctionTable.glBindTexture == NULL)
		return false;
	m_woglFunctionTable.glBitmap = (void (APIENTRY*) (GLsizei, GLsizei, GLfloat, GLfloat, GLfloat, GLfloat, const GLubyte*))LoadGLRoutine("glBitmap");
	if(m_woglFunctionTable.glBitmap == NULL)
		return false;
	m_woglFunctionTable.glBlendFunc = (void (APIENTRY*) (GLenum, GLenum))LoadGLRoutine("glBlendFunc");
	if(m_woglFunctionTable.glBlendFunc == NULL)
		return false;
	m_woglFunctionTable.glCallList = (void (APIENTRY*) (GLuint))LoadGLRoutine("glCallList");
	if(m_woglFunctionTable.glCallList == NULL)
		return false;
	m_woglFunctionTable.glCallLists = (void (APIENTRY*) (GLsizei, GLenum, const GLvoid*))LoadGLRoutine("glCallLists");
	if(m_woglFunctionTable.glCallLists == NULL)
		return false;
	m_woglFunctionTable.glClear = (void (APIENTRY*) (GLbitfield))LoadGLRoutine("glClear");
	if(m_woglFunctionTable.glClear == NULL)
		return false;
	m_woglFunctionTable.glClearAccum = (void (APIENTRY*) (GLfloat, GLfloat, GLfloat, GLfloat))LoadGLRoutine("glClearAccum");
	if(m_woglFunctionTable.glClearAccum == NULL)
		return false;
	m_woglFunctionTable.glClearColor = (void (APIENTRY*) (GLclampf, GLclampf, GLclampf, GLclampf))LoadGLRoutine("glClearColor");
	if(m_woglFunctionTable.glClearColor == NULL)
		return false;
	m_woglFunctionTable.glClearDepth = (void (APIENTRY*) (GLclampd))LoadGLRoutine("glClearDepth");
	if(m_woglFunctionTable.glClearDepth == NULL)
		return false;
	m_woglFunctionTable.glColor3f = (void (APIENTRY*) (GLfloat, GLfloat, GLfloat))LoadGLRoutine("glColor3f");
	if(m_woglFunctionTable.glColor3f == NULL)
		return false;
	m_woglFunctionTable.glColor3fv = (void (APIENTRY*) (const GLfloat *v))LoadGLRoutine("glColor3fv");
	if(m_woglFunctionTable.glColor3fv == NULL)
		return false;
	m_woglFunctionTable.glColor4f = (void (APIENTRY*) (GLfloat, GLfloat, GLfloat, GLfloat))LoadGLRoutine("glColor4f");
	if(m_woglFunctionTable.glColor4f == NULL)
		return false;
	m_woglFunctionTable.glCullFace = (void (APIENTRY*) (GLenum))LoadGLRoutine("glCullFace");
	if(m_woglFunctionTable.glCullFace == NULL)
		return false;
	m_woglFunctionTable.glDeleteLists = (void (APIENTRY*) (GLuint, GLsizei))LoadGLRoutine("glDeleteLists");
	if(m_woglFunctionTable.glDeleteLists == NULL)
		return false;
	m_woglFunctionTable.glDeleteTextures = (void (APIENTRY*) (GLsizei, const GLuint*))LoadGLRoutine("glDeleteTextures");
	if(m_woglFunctionTable.glDeleteTextures == NULL)
		return false;
	m_woglFunctionTable.glDepthFunc = (void (APIENTRY*) (GLenum))LoadGLRoutine("glDepthFunc");
	if(m_woglFunctionTable.glDepthFunc == NULL)
		return false;
	m_woglFunctionTable.glDepthMask = (void (APIENTRY*) (GLboolean))LoadGLRoutine("glDepthMask");
	if(m_woglFunctionTable.glDepthMask == NULL)
		return false;
	m_woglFunctionTable.glDepthRange = (void (APIENTRY*) (GLclampd, GLclampd))LoadGLRoutine("glDepthRange");
	if(m_woglFunctionTable.glDepthRange == NULL)
		return false;
	m_woglFunctionTable.glDisable = (void (APIENTRY*) (GLenum))LoadGLRoutine("glDisable");
	if(m_woglFunctionTable.glDisable == NULL)
		return false;
	m_woglFunctionTable.glDisableClientState = (void (APIENTRY*) (GLenum))LoadGLRoutine("glDisableClientState");
	if(m_woglFunctionTable.glDisableClientState == NULL)
		return false;
	m_woglFunctionTable.glDrawArrays = (void (APIENTRY*) (GLenum, GLint, GLsizei))LoadGLRoutine("glDrawArrays");
	if(m_woglFunctionTable.glDrawArrays == NULL)
		return false;
	m_woglFunctionTable.glDrawBuffer = (void (APIENTRY*) (GLenum))LoadGLRoutine("glDrawBuffer");
	if(m_woglFunctionTable.glDrawBuffer == NULL)
		return false;
	m_woglFunctionTable.glDrawElements = (void (APIENTRY*) (GLenum, GLsizei, GLenum, const GLvoid*))LoadGLRoutine("glDrawElements");
	if(m_woglFunctionTable.glDrawElements == NULL)
		return false;
	m_woglFunctionTable.glDrawPixels = (void (APIENTRY*) (GLsizei, GLsizei, GLenum, GLenum, const GLvoid*))LoadGLRoutine("glDrawPixels");
	if(m_woglFunctionTable.glDrawPixels == NULL)
		return false;
	m_woglFunctionTable.glEnable = (void (APIENTRY*) (GLenum))LoadGLRoutine("glEnable");
	if(m_woglFunctionTable.glEnable == NULL)
		return false;
	m_woglFunctionTable.glEnableClientState = (void (APIENTRY*) (GLenum))LoadGLRoutine("glEnableClientState");
	if(m_woglFunctionTable.glEnableClientState == NULL)
		return false;
	m_woglFunctionTable.glEnd = (void (APIENTRY*) (void))LoadGLRoutine("glEnd");
	if(m_woglFunctionTable.glEnd == NULL)
		return false;
	m_woglFunctionTable.glEndList = (void (APIENTRY*) (void))LoadGLRoutine("glEndList");
	if(m_woglFunctionTable.glEndList == NULL)
		return false;
	m_woglFunctionTable.glFinish = (void (APIENTRY*) (void))LoadGLRoutine("glFinish");
	if(m_woglFunctionTable.glFinish == NULL)
		return false;
	m_woglFunctionTable.glFlush = (void (APIENTRY*) (void))LoadGLRoutine("glFlush");
	if(m_woglFunctionTable.glFlush == NULL)
		return false;
	m_woglFunctionTable.glFrontFace = (void (APIENTRY*) (GLenum mode))LoadGLRoutine("glFrontFace");
	if(m_woglFunctionTable.glFrontFace == NULL)
		return false;
	m_woglFunctionTable.glFrustum = (void (APIENTRY*) (GLdouble, GLdouble, GLdouble, GLdouble, GLdouble, GLdouble))LoadGLRoutine("glFrustum");
	if(m_woglFunctionTable.glFrustum == NULL)
		return false;
	m_woglFunctionTable.glGetError = (GLenum (APIENTRY*) (void))LoadGLRoutine("glGetError");
	if(m_woglFunctionTable.glGetError == NULL)
		return false;
	m_woglFunctionTable.glHint = (void (APIENTRY*) (GLenum, GLenum))LoadGLRoutine("glHint");
	if(m_woglFunctionTable.glHint == NULL)
		return false;
	m_woglFunctionTable.glListBase = (void (APIENTRY*) (GLuint))LoadGLRoutine("glListBase");
	if(m_woglFunctionTable.glListBase == NULL)
		return false;
	m_woglFunctionTable.glLoadIdentity = (void (APIENTRY*) (void))LoadGLRoutine("glLoadIdentity");
	if(m_woglFunctionTable.glLoadIdentity == NULL)
		return false;
	m_woglFunctionTable.glMatrixMode = (void (APIENTRY*) (GLenum))LoadGLRoutine("glMatrixMode");
	if(m_woglFunctionTable.glMatrixMode == NULL)
		return false;
	m_woglFunctionTable.glNewList = (void (APIENTRY*) (GLuint, GLenum))LoadGLRoutine("glNewList");
	if(m_woglFunctionTable.glNewList == NULL)
		return false;
	m_woglFunctionTable.glRasterPos2i = (void (APIENTRY*) (GLint, GLint))LoadGLRoutine("glRasterPos2i");
	if(m_woglFunctionTable.glRasterPos2i == NULL)
		return false;
	m_woglFunctionTable.glRenderMode = (GLint (APIENTRY*) (GLenum))LoadGLRoutine("glRenderMode");
	if(m_woglFunctionTable.glRenderMode == NULL)
		return false;
	m_woglFunctionTable.glRotatef = (void (APIENTRY*) (GLfloat, GLfloat, GLfloat, GLfloat))LoadGLRoutine("glRotatef");
	if(m_woglFunctionTable.glRotatef == NULL)
		return false;
	m_woglFunctionTable.glScalef = (void (APIENTRY*) (GLfloat, GLfloat, GLfloat))LoadGLRoutine("glScalef");
	if(m_woglFunctionTable.glScalef == NULL)
		return false;
	m_woglFunctionTable.glTexCoord2f = (void (APIENTRY*) (GLfloat, GLfloat))LoadGLRoutine("glTexCoord2f");
	if(m_woglFunctionTable.glTexCoord2f == NULL)
		return false;
	m_woglFunctionTable.glTexCoord2fv = (void (APIENTRY*) (const GLfloat*))LoadGLRoutine("glTexCoord2fv");
	if(m_woglFunctionTable.glTexCoord2fv == NULL)
		return false;
	m_woglFunctionTable.glTexCoord2i = (void (APIENTRY*) (GLint, GLint))LoadGLRoutine("glTexCoord2i");
	if(m_woglFunctionTable.glTexCoord2i == NULL)
		return false;
	m_woglFunctionTable.glIsTexture = (GLboolean (APIENTRY*) (GLuint texture))LoadGLRoutine("glIsTexture");
	if(m_woglFunctionTable.glIsTexture == NULL)
		return false;
	m_woglFunctionTable.glTexEnvi = (void (APIENTRY*) (GLenum, GLenum, GLint param))LoadGLRoutine("glTexEnvi");
	if(m_woglFunctionTable.glTexEnvi == NULL)
		return false;
	m_woglFunctionTable.glTexImage2D = (void (APIENTRY*) (GLenum, GLint, GLint, GLsizei, GLsizei, GLint, GLenum, GLenum, const GLvoid*))LoadGLRoutine("glTexImage2D");
	if(m_woglFunctionTable.glTexImage2D == NULL)
		return false;
	m_woglFunctionTable.glTexParameteri = (void (APIENTRY*) (GLenum, GLenum, GLint))LoadGLRoutine("glTexParameteri");
	if(m_woglFunctionTable.glTexParameteri == NULL)
		return false;
	m_woglFunctionTable.glTexSubImage2D = (void (APIENTRY*) (GLenum, GLint, GLint, GLint, GLsizei, GLsizei, GLenum, GLenum, const GLvoid*))LoadGLRoutine("glTexSubImage2D");
	if(m_woglFunctionTable.glTexSubImage2D == NULL)
		return false;
	m_woglFunctionTable.glTranslatef = (void (APIENTRY*) (GLfloat, GLfloat, GLfloat))LoadGLRoutine("glTranslatef");
	if(m_woglFunctionTable.glTranslatef == NULL)
		return false;
	m_woglFunctionTable.glVertex2i = (void (APIENTRY*) (GLint, GLint))LoadGLRoutine("glVertex2i");
	if(m_woglFunctionTable.glVertex2i == NULL)
		return false;
	m_woglFunctionTable.glVertex2f = (void (APIENTRY*) (GLfloat, GLfloat))LoadGLRoutine("glVertex2f");
	if(m_woglFunctionTable.glVertex2f == NULL)
		return false;
	m_woglFunctionTable.glVertex3f = (void (APIENTRY*) (GLfloat, GLfloat, GLfloat))LoadGLRoutine("glVertex3f");
	if(m_woglFunctionTable.glVertex3f == NULL)
		return false;
	m_woglFunctionTable.glVertex3fv = (void (APIENTRY *) (const GLfloat*))LoadGLRoutine("glVertex3fv");
	if(m_woglFunctionTable.glVertex3fv == NULL)
		return false;
	m_woglFunctionTable.glVertex3i = (void (APIENTRY*) (GLint, GLint, GLint))LoadGLRoutine("glVertex3i");
	if(m_woglFunctionTable.glVertex3i == NULL)
		return false;
	m_woglFunctionTable.glVertex3iv = (void (APIENTRY*) (const GLint*))LoadGLRoutine("glVertex3iv");
	if(m_woglFunctionTable.glVertex3iv == NULL)
		return false;
	m_woglFunctionTable.glVertex3s = (void (APIENTRY*) (GLshort, GLshort, GLshort))LoadGLRoutine("glVertex3s");
	if(m_woglFunctionTable.glVertex3s == NULL)
		return false;
	m_woglFunctionTable.glVertex3sv = (void (APIENTRY*) (const GLshort*))LoadGLRoutine("glVertex3sv");
	if(m_woglFunctionTable.glVertex3sv == NULL)
		return false;
	m_woglFunctionTable.glVertexPointer = (void (APIENTRY*) (GLint, GLenum, GLsizei, const GLvoid*))LoadGLRoutine("glVertexPointer");
	if(m_woglFunctionTable.glVertexPointer == NULL)
		return false;
	m_woglFunctionTable.glViewport = (void (APIENTRY*) (GLint, GLint, GLsizei, GLsizei))LoadGLRoutine("glViewport");
	if(m_woglFunctionTable.glViewport == NULL)
		return false;
	m_woglFunctionTable.glPixelStorei = (void (APIENTRY*)(GLenum, GLint))LoadGLRoutine("glPixelStorei");
	if(m_woglFunctionTable.glPixelStorei == NULL)
		return false;
	m_woglFunctionTable.glGetString = (const GLubyte* (APIENTRY*) (GLenum name))LoadGLRoutine("glGetString");
	if(m_woglFunctionTable.glGetString == NULL)
		return false;
	m_woglFunctionTable.glPixelTransferf = (void (APIENTRY *)(GLenum pname, GLfloat param))LoadGLRoutine("glPixelTransferf");
	if(m_woglFunctionTable.glPixelTransferf == NULL)
		return false;
	m_woglFunctionTable.glShadeModel = (void (APIENTRY*)(GLenum mode))LoadGLRoutine("glShadeModel");
	if(m_woglFunctionTable.glShadeModel == NULL)
		return false;
	m_woglFunctionTable.glPushMatrix = (void (APIENTRY*)(void))LoadGLRoutine("glPushMatrix");
	if(m_woglFunctionTable.glPushMatrix == NULL)
		return false;
	m_woglFunctionTable.glPopMatrix = (void (APIENTRY*)(void))LoadGLRoutine("glPopMatrix");
	if(m_woglFunctionTable.glPopMatrix == NULL)
		return false;
	m_woglFunctionTable.glLoadName = (void (APIENTRY*)(GLuint))LoadGLRoutine("glLoadName");
	if(m_woglFunctionTable.glLoadName == NULL)
		return false;
	m_woglFunctionTable.glInitNames = (void (APIENTRY*) (void))LoadGLRoutine("glInitNames");
	if(m_woglFunctionTable.glInitNames == NULL)
		return false;
	m_woglFunctionTable.glSelectBuffer = (void (APIENTRY*) (GLsizei size, GLuint *buffer))LoadGLRoutine("glSelectBuffer");
	if(m_woglFunctionTable.glSelectBuffer == NULL)
		return false;
	m_woglFunctionTable.glPushName = (void (APIENTRY*) (GLuint name))LoadGLRoutine("glPushName");
	if(m_woglFunctionTable.glPushName == NULL)
		return false;
	m_woglFunctionTable.glGetIntegerv = (void (APIENTRY*) (GLenum pname, GLint *params))LoadGLRoutine("glGetIntegerv");
	if(m_woglFunctionTable.glGetIntegerv == NULL)
		return false;
	m_woglFunctionTable.glPolygonOffset = (void (APIENTRY*) (GLfloat factor, GLfloat units))LoadGLRoutine("glPolygonOffset");
	if(m_woglFunctionTable.glPolygonOffset == NULL)
		return false;

	// Glu extensions.
	m_woglFunctionTable.gluPerspective = (void (APIENTRY*) (GLdouble, GLdouble, GLdouble, GLdouble))LoadGLRoutine("gluPerspective");
	if(m_woglFunctionTable.gluPerspective == NULL)
		return false;
	m_woglFunctionTable.gluLookAt = (void (APIENTRY*) (GLdouble, GLdouble, GLdouble, GLdouble, GLdouble, GLdouble, GLdouble, GLdouble, GLdouble))LoadGLRoutine("gluLookAt");
	if(m_woglFunctionTable.gluLookAt == NULL)
		return false;
	m_woglFunctionTable.gluOrtho2D = (void (APIENTRY*) (GLdouble, GLdouble, GLdouble, GLdouble))LoadGLRoutine("gluOrtho2D");
	if(m_woglFunctionTable.gluOrtho2D == NULL)
		return false;
	m_woglFunctionTable.gluScaleImage = (GLint (APIENTRY*) (GLenum format, GLsizei wIn, GLsizei hIn, GLenum typeIn, const void *dataIn, GLsizei wOut, GLsizei hOut, GLenum typeOut, GLvoid* dataOut))LoadGLRoutine("gluScaleImage");
	if(m_woglFunctionTable.gluScaleImage == NULL)
		return false;
	m_woglFunctionTable.gluPickMatrix = (void (APIENTRY*) (GLdouble x, GLdouble y, GLdouble delX, GLdouble delY, GLint *viewport))LoadGLRoutine("gluPickMatrix");
	if(m_woglFunctionTable.gluPickMatrix == NULL)
		return false;
	
#ifdef _WIN32
	// Windows extensions.
	m_woglFunctionTable.e_wglCopyContext = (BOOL  (WINAPI*)(HGLRC, HGLRC, UINT))LoadGLRoutine("wglCopyContext");
	if(m_woglFunctionTable.e_wglCopyContext == NULL)
		return false;
	m_woglFunctionTable.e_wglCreateContext = (HGLRC (WINAPI*)(HDC))LoadGLRoutine("wglCreateContext");
	if(m_woglFunctionTable.e_wglCreateContext == NULL)
		return false;
	m_woglFunctionTable.e_wglCreateLayerContext = (HGLRC (WINAPI*)(HDC, int))LoadGLRoutine("wglCreateLayerContext");
	if(m_woglFunctionTable.e_wglCreateLayerContext == NULL)
		return false;
	m_woglFunctionTable.e_wglDeleteContext = (BOOL  (WINAPI*)(HGLRC))LoadGLRoutine("wglDeleteContext");
	if(m_woglFunctionTable.e_wglDeleteContext == NULL)
		return false;
	m_woglFunctionTable.e_wglGetCurrentContext = (HGLRC (WINAPI*)(VOID))LoadGLRoutine("wglGetCurrentContext");
	if(m_woglFunctionTable.e_wglGetCurrentContext == NULL)
		return false;
	m_woglFunctionTable.e_wglGetCurrentDC = (HDC   (WINAPI*)(VOID))LoadGLRoutine("wglGetCurrentDC");
	if(m_woglFunctionTable.e_wglGetCurrentDC == NULL)
		return false;
	m_woglFunctionTable.e_wglGetProcAddress = (PROC  (WINAPI*)(LPCSTR))LoadGLRoutine("wglGetProcAddress");
	if(m_woglFunctionTable.e_wglGetProcAddress == NULL)
		return false;
	m_woglFunctionTable.e_wglMakeCurrent = (BOOL  (WINAPI*)(HDC, HGLRC))LoadGLRoutine("wglMakeCurrent");
	if(m_woglFunctionTable.e_wglMakeCurrent == NULL)
		return false;
	m_woglFunctionTable.e_wglShareLists = (BOOL  (WINAPI*)(HGLRC, HGLRC))LoadGLRoutine("wglShareLists");
	if(m_woglFunctionTable.e_wglShareLists == NULL)
		return false;
	m_woglFunctionTable.e_wglUseFontBitmapsA = (BOOL  (WINAPI*)(HDC, DWORD, DWORD, DWORD))LoadGLRoutine("wglUseFontBitmapsA");
	if(m_woglFunctionTable.e_wglUseFontBitmapsA == NULL)
		return false;
	m_woglFunctionTable.e_wglUseFontBitmapsW = (BOOL  (WINAPI*)(HDC, DWORD, DWORD, DWORD))LoadGLRoutine("wglUseFontBitmapsW");
	if(m_woglFunctionTable.e_wglUseFontBitmapsW == NULL)
		return false;
#endif

	return true;
}