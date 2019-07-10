// GLRENDERPACKAGE.H
// Copyright © 1999 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#ifndef GLRENDERPACKAGE_H
#define GLRENDERPACKAGE_H

#include "RenderAPI.h"

#ifdef _WIN32
#include "wogl.h"
#elif macintosh
#include <agl.h>
#define APIENTRY
#endif

// $sb "glrenderpackage.h block1"

struct WOGLFUNCTIONTABLE
{
	void (APIENTRY *glArrayElement) (GLint i);
	void (APIENTRY *glBegin) (GLenum mode);
	void (APIENTRY *glBindTexture) (GLenum target, GLuint texture);
	void (APIENTRY *glBitmap) (GLsizei width, GLsizei height, GLfloat xorig, GLfloat yorig, GLfloat xmove, GLfloat ymove, const GLubyte *bitmap);
	void (APIENTRY *glBlendFunc) (GLenum sfactor, GLenum dfactor);
	void (APIENTRY *glCallList) (GLuint list);
	void (APIENTRY *glCallLists) (GLsizei n, GLenum type, const GLvoid *lists);
	void (APIENTRY *glClear) (GLbitfield mask);
	void (APIENTRY *glClearAccum) (GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
	void (APIENTRY *glClearColor) (GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);
	void (APIENTRY *glClearDepth) (GLclampd depth);
	void (APIENTRY *glColor3f) (GLfloat red, GLfloat green, GLfloat blue);
	void (APIENTRY *glColor3fv) (const GLfloat *v);
	void (APIENTRY *glColor4f) (GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
	void (APIENTRY *glCullFace) (GLenum mode);
	void (APIENTRY *glDeleteLists) (GLuint list, GLsizei range);
	void (APIENTRY *glDeleteTextures) (GLsizei n, const GLuint *textures);
	void (APIENTRY *glDepthFunc) (GLenum func);
	void (APIENTRY *glDepthMask) (GLboolean flag);
	void (APIENTRY *glDepthRange) (GLclampd zNear, GLclampd zFar);
	void (APIENTRY *glDisable) (GLenum cap);
	void (APIENTRY *glDisableClientState) (GLenum array);
	void (APIENTRY *glDrawArrays) (GLenum mode, GLint first, GLsizei count);
	void (APIENTRY *glDrawBuffer) (GLenum mode);
	void (APIENTRY *glDrawElements) (GLenum mode, GLsizei count, GLenum type, const GLvoid *indices);
	void (APIENTRY *glDrawPixels) (GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *pixels);
	void (APIENTRY *glEnable) (GLenum cap);
	void (APIENTRY *glEnableClientState) (GLenum array);
	void (APIENTRY *glEnd) (void);
	void (APIENTRY *glEndList) (void);
	void (APIENTRY *glFinish) (void);
	void (APIENTRY *glFlush) (void);
	void (APIENTRY *glFrontFace) (GLenum mode);
	void (APIENTRY *glFrustum) (GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar);
	GLenum (APIENTRY *glGetError) (void);
	const GLubyte * (APIENTRY *glGetString) (GLenum name);
	void (APIENTRY *glHint) (GLenum target, GLenum mode);
	void (APIENTRY *glListBase) (GLuint base);
	void (APIENTRY *glLoadIdentity) (void);
	void (APIENTRY *glMatrixMode) (GLenum mode);
	void (APIENTRY *glPushMatrix) (void);
	void (APIENTRY *glPopMatrix) (void);
	void (APIENTRY *glGetIntegerv) (GLenum pname, GLint *params);
	void (APIENTRY *glPolygonOffset) (GLfloat factor, GLfloat units);

	void (APIENTRY *glInitNames) (void);
	void (APIENTRY *glSelectBuffer) (GLsizei size, GLuint *buffer);
	void (APIENTRY *glLoadName) (GLuint name);
	void (APIENTRY *glPushName) (GLuint name);

	void (APIENTRY *glNewList) (GLuint list, GLenum mode);
	void (APIENTRY *glRasterPos2i) (GLint x, GLint y);
	GLint (APIENTRY *glRenderMode) (GLenum mode);
	void (APIENTRY *glRotatef) (GLfloat angle, GLfloat x, GLfloat y, GLfloat z);
	void (APIENTRY *glScalef) (GLfloat x, GLfloat y, GLfloat z);
	void (APIENTRY *glTexCoord2f) (GLfloat s, GLfloat t);
	void (APIENTRY *glTexCoord2fv) (const GLfloat *v);
	void (APIENTRY *glTexCoord2i) (GLint s, GLint t);
	GLboolean (APIENTRY* glIsTexture) (GLuint texture);
	void (APIENTRY *glTexEnvi) (GLenum target, GLenum pname, GLint param);
	void (APIENTRY *glTexImage2D) (GLenum target, GLint level, GLint components, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid *pixels);
	void (APIENTRY *glTexParameteri) (GLenum target, GLenum pname, GLint param);
	void (APIENTRY *glTexSubImage2D) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *pixels);
	void (APIENTRY *glTranslatef) (GLfloat x, GLfloat y, GLfloat z);
	void (APIENTRY *glVertex2i) (GLint x, GLint y);
	void (APIENTRY *glVertex2f) (GLfloat x, GLfloat y);
	void (APIENTRY *glVertex3f) (GLfloat x, GLfloat y, GLfloat z);
	void (APIENTRY *glVertex3fv) (const GLfloat *v);
	void (APIENTRY *glVertex3i) (GLint x, GLint y, GLint z);
	void (APIENTRY *glVertex3iv) (const GLint *v);
	void (APIENTRY *glVertex3s) (GLshort x, GLshort y, GLshort z);
	void (APIENTRY *glVertex3sv) (const GLshort *v);
	void (APIENTRY *glVertexPointer) (GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
	void (APIENTRY *glViewport) (GLint x, GLint y, GLsizei width, GLsizei height);
	void (APIENTRY *glPixelStorei) (GLenum pname, GLint param);
	void (APIENTRY *glPixelTransferf) (GLenum pname, GLfloat param);
	void (APIENTRY *glShadeModel) (GLenum mode);

	// Glu extensions.
	void (APIENTRY *gluPerspective) (GLdouble fovy, GLdouble aspect, GLdouble zNear, GLdouble zFar);
	void (APIENTRY *gluLookAt) (GLdouble eyeX, GLdouble eyeY, GLdouble eyeZ, GLdouble centerX, GLdouble centerY, GLdouble centerZ, GLdouble upX, GLdouble upY, GLdouble upZ);
	void (APIENTRY *gluOrtho2D) (GLdouble left, GLdouble right, GLdouble bottom, GLdouble top);
	GLint (APIENTRY *gluScaleImage) (GLenum format, GLsizei wIn, GLsizei hIn, GLenum typeIn, const void *dataIn, GLsizei wOut, GLsizei hOut, GLenum typeOut, GLvoid* dataOut);
	void (APIENTRY *gluPickMatrix) (GLdouble x, GLdouble y, GLdouble delX, GLdouble delY, GLint *viewport);

#ifdef _WIN32
	// Windows extensions.
	BOOL  (WINAPI *e_wglCopyContext)(HGLRC, HGLRC, UINT);
	HGLRC (WINAPI *e_wglCreateContext)(HDC);
	HGLRC (WINAPI *e_wglCreateLayerContext)(HDC, int);
	BOOL  (WINAPI *e_wglDeleteContext)(HGLRC);
	HGLRC (WINAPI *e_wglGetCurrentContext)(VOID);
	HDC   (WINAPI *e_wglGetCurrentDC)(VOID);
	PROC  (WINAPI *e_wglGetProcAddress)(LPCSTR);
	BOOL  (WINAPI *e_wglMakeCurrent)(HDC, HGLRC);
	BOOL  (WINAPI *e_wglShareLists)(HGLRC, HGLRC);
	BOOL  (WINAPI *e_wglUseFontBitmapsA)(HDC, DWORD, DWORD, DWORD);
	BOOL  (WINAPI *e_wglUseFontBitmapsW)(HDC, DWORD, DWORD, DWORD);
	BOOL  (WINAPI *e_SwapBuffers)(HDC);
	int   (WINAPI *e_ChoosePixelFormat)(HDC, CONST PIXELFORMATDESCRIPTOR *);
	BOOL  (WINAPI *e_SetPixelFormat)(HDC, int, CONST PIXELFORMATDESCRIPTOR *);
#endif

};

// $eb

class CGLRenderPackage : public CBaseRenderPackage
{
private:

	CString m_sName;
	
#ifdef _WIN32
	HMODULE m_hOpenGLLib, m_hGluLib;
#endif

	bool LoadGLLibrary(CString sOpenGL, CString sGlu);	
	int* LoadGLRoutine(CString sFunction);
	WOGLFUNCTIONTABLE m_woglFunctionTable;

	

public:

	CGLRenderPackage();
	~CGLRenderPackage();

	static bool StaticClaimPackageName(CString sName);
	WOGLFUNCTIONTABLE* GetFunctionTable() { return &m_woglFunctionTable; }

	int GetRenderFlags() { return RF_3D_API|RF_2D_API; }
	bool LoadPackage(CString sName);
	bool UnloadPackage();
	CString GetPackageName() { return m_sName; }
	CBaseWindowRenderer* GetWindowRenderer(CMapView* pView, CDC* pDC, int nType);

};

#endif