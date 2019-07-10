// MATH.H - Athens math routines header file.
// Copyright © 1997 Joe Riedel.
// Author: Joe Riedel.

#ifndef MATHLIB_H
#define MATHLIB_H

#include <math.h>
#include "LinkedList.h"

// Define basic structures.
#define PI					3.14159265359F
#define MAX_TRIG_ANGLES		2048

#define SIN(a) (gSinTable[a & (MAX_TRIG_ANGLES - 1)])
#define COS(a) (gCosTable[a & (MAX_TRIG_ANGLES - 1)])

// Normal flags.
#define NF_FACING_X		0
#define NF_FACING_Y		1
#define NF_FACING_Z		2
#define NF_FACING_ANY	3

typedef float vec_t;
typedef vec_t vec3_t[3];
typedef float vec_t;
typedef vec_t vec2_t[2];
typedef vec_t vec4_t[4];

extern vec3_t vec3_origin;

////////////////////////////////////////////////
// Quaternion routines.
////////////////////////////////////////////////
// Quaternions appear to be some kind of vector with a 4th component for a scalar value.
// Why this is I'm not exactly sure, all I know are the basic properties I read out of
// some math book. They apparently handle succesive rotations better than a standard 4x4
// matrix. Other than that, they also seem to be kinda magically setup to do arbitrary
// orthogonal definitions for rotations and stuff...
//
// I think I just confused myself.
void Q_Clear(vec4_t vQ);
void Q_Copy(vec4_t vSrc, vec4_t vDst);
void Q_SetFromVector(vec3_t vVec, float fScaler, vec4_t vQ);
void Q_GetVector(vec4_t vQ, vec3_t vVec, float* fScaler);
void Q_SetFromAxisRotation(vec3_t vAxis, float fRot, vec4_t vQ);
void Q_GetAxisRotation(vec4_t vQ, vec3_t vAxis, float* fRot);
void Q_Multiply(vec4_t vQ1, vec4_t vQ2, vec4_t vQOut);
void Q_RotateVector(vec4_t vQ, vec3_t vVec, vec3_t vOut);
float Q_Magnitude(vec4_t vQ);
void Q_Normalize(vec4_t vQ);
void Q_Inverse(vec4_t vQ, vec4_t vQOut);
void Q_Add(vec4_t vQ1, vec4_t vQ2, vec4_t vQOut);
void Q_Subtract(vec4_t vQ1, vec4_t vQ2, vec4_t vQOut);
void Q_Scale(vec4_t vQ, float fScale, vec4_t vQOut);
void Q_GetEulerAngles(vec4_t vQ, vec3_t vRot);
void Q_SetEulerAngles(vec3_t vRot, vec4_t vQ);


// Slerpy routines.
// Does spherical interpoation between q1, and q2.
// Temperal=0=q1, Temperal=1=q2.
void Q_InterpolateSphereNormal(vec4_t vQ1, vec4_t vQ2, vec4_t vQOut, float fTemperal);
void Q_InterpolateSphereShortest(vec4_t vQ1, vec4_t vQ2, vec4_t vQOut, float fTemperal);
void Q_InterpolateLinear(vec4_t vQ1, vec4_t vQ2, vec4_t vQOut, float fTemperal);

// Returns the type of vector.
int		ClassifyVector(vec3_t);
void	VectorAdd(vec3_t a, vec3_t b, vec3_t out);
void	VectorSubtract(vec3_t a, vec3_t b, vec3_t out);
vec_t	VectorLength(vec3_t a);
vec_t	DotProduct(vec3_t a, vec3_t b);

#ifdef _WIN32
vec_t	DotProduct_asm(vec3_t a, vec3_t b);
#else
#define DotProduct_asm(a, b) DotProduct(a, b)
#endif

void	CrossProduct(vec3_t a, vec3_t b, vec3_t out);
void	VectorCopy(const vec3_t src, vec3_t dst);
void	VectorMultiply(vec3_t a, vec3_t b, vec3_t out);
void	VectorDivide(vec3_t a, vec3_t b, vec3_t out);
void	VectorClear(vec3_t a);
bool	VectorCompare(vec3_t a, vec3_t b, float fEpsilon);
bool	VectorIsZero(vec3_t a);
void VectorClamp(vec3_t v);

// Interpolation stuff.


// Adds: va + scale*vb;
void VectorMA (vec3_t va, vec_t scale, vec3_t vb, vec3_t vc);
// Normalizes a vector.
void VectorNormalize(vec3_t a);
// Computes a orthogonal normal vector based on two vectors.
void CalcOrthoVector(vec3_t a, vec3_t b, vec3_t out);
// Scales a vector.
void VectorScale(vec3_t a, vec_t scale, vec3_t out);
// Rotate with lookups.
void VectorRotate(vec3_t, int, int, int, vec3_t out);
// Rotate without lookups.
void VectorRotate(vec3_t, vec3_t rot, vec3_t out);
// Inserts a range.
void InsertRange(vec3_t v, vec3_t mins, vec3_t maxs);
// Orders a range.
void OrderRange(vec3_t mins, vec3_t maxs);
// Initializes a range.
void InitializeRange(vec3_t mins, vec3_t maxs);
// Computes the origin if the range.
void ComputeRangeOrigin(vec3_t mins, vec3_t maxs, vec3_t origin);
// Returns true if ranges overlap.
bool RangesIntersect(vec3_t mins_a, vec3_t maxs_a, vec3_t mins_b, vec3_t maxs_b);
// Returns true if point in within range.
bool InRange(vec3_t vPoint, vec3_t vMins, vec3_t vMaxs);

class CPlane
{
public:

	vec_t m_vDist;
	vec3_t m_vNormal;

	void Flip()
	{
		VectorScale(m_vNormal, -1.0F, m_vNormal);
		m_vDist = -m_vDist;
	}
};

class CLLPlane : public CPlane, virtual public CLLObject
{
	CLLPlane(const CPlane& PlaneToCopy)
	{
		m_vDist = PlaneToCopy.m_vDist;
		VectorCopy(PlaneToCopy.m_vNormal, m_vNormal);
	}
};

// Trig tables.
extern float gSinTable[MAX_TRIG_ANGLES];
extern float gCosTable[MAX_TRIG_ANGLES];

bool BuildTrigTables(void);

#endif