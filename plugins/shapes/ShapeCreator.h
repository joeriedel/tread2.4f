// SHAPECREATOR.H
// Copyright © 2000 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#ifndef SHAPECREATOR_H
#define SHAPECREATOR_H

class CQBrush;
class CObjectGroup;

class CShapeCreator
{
public:

	static CQBrush* CreateCone(int nMajorAxis, int nNumSides, float fRadius, float fLength);
	static CQBrush* CreateCylinder(int nMajorAxis, int nNumSides, float fRadius, float fLength);
	static CObjectGroup* CreateArch(int nMajorAxis, int nNumSides, float fRadius, float fWidth, float fDepth);
};


#endif