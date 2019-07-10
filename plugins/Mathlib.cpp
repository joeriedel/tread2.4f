// MATH.CPP - Athens math library.
// Copyright © 1997 Joe Riedel.
// Author: Joe Riedel.

#include "stdafx.h"
#include <math.h>

#ifdef macintosh
#include <string.h>
#endif

#include "mathlib.h"

// Tables.
float gSinTable[MAX_TRIG_ANGLES];
float gCosTable[MAX_TRIG_ANGLES];
vec3_t vec3_origin = {0.0F, 0.0F, 0.0F};

#define W	0
#define X	1
#define Y	2
#define Z	3

void Q_Clear(vec4_t vQ)
{
	vQ[W] = 1.0F;
	vQ[X] = 0.0F;
	vQ[Y] = 0.0F;
	vQ[Z] = 0.0F;
}

void Q_Copy(vec4_t vSrc, vec4_t vDst)
{
	vDst[W] = vSrc[W];
	vDst[X] = vSrc[X];
	vDst[Y] = vSrc[Y];
	vDst[Z] = vSrc[Z];
}

void Q_SetFromVector(vec3_t vVec, float fScaler, vec4_t vQ)
{
	vQ[W] = fScaler;
	vQ[X] = vVec[0];
	vQ[Y] = vVec[1];
	vQ[Z] = vVec[2];
}

void Q_GetVector(vec4_t vQ, vec3_t vVec, float* fScaler)
{
	if(fScaler != NULL)
		*fScaler = vQ[W];
	vVec[0] = vQ[X];
	vVec[1] = vQ[Y];
	vVec[2] = vQ[Z];
}

void Q_SetFromAxisRotation(vec3_t vAxis, float fRot, vec4_t vQ)
{
	float fSin;
	
	fRot = fRot * 0.5F;
	vQ[W] = (float)cos(fRot);
	fSin = (float)sin(fRot);
	vQ[X] = fSin * vAxis[0];
	vQ[Y] = fSin * vAxis[1];
	vQ[Z] = fSin * vAxis[2];
}

void Q_GetAxisRotation(vec4_t vQ, vec3_t vAxis, float* fRot)
{
	if(fRot == NULL)
		return;

	float fTheta;
	float fSin;

	fTheta = (float)acos(vQ[W]);
	
	if(fTheta > 0.00001)
	{
		fSin = 1.0F / (float)sin(fTheta);
		vAxis[0] = fSin * vQ[X];
		vAxis[1] = fSin * vQ[Y];
		vAxis[2] = fSin * vQ[Z];
		*fRot = fTheta * 2.0F;
	}
	else
	{
		vAxis[0] = vAxis[1] = vAxis[2] = 0.0F;
		*fRot = 0.0F;
	}
}

void Q_Multiply(vec4_t vQ1, vec4_t vQ2, vec4_t vQOut)
{
	vQOut[W] = (	(vQ1[W]*vQ2[W]) - (vQ1[X]*vQ2[X])
				-	(vQ1[Y]*vQ2[Y]) - (vQ1[Z]*vQ2[Z]) );

	vQOut[X] = (	(vQ1[W]*vQ2[X]) + (vQ1[X]*vQ2[W])
				+	(vQ1[Y]*vQ2[Z]) - (vQ1[Z]*vQ2[Y]) );

	vQOut[Y] = (	(vQ1[W]*vQ2[Y]) - (vQ1[X]*vQ2[Z])
				+	(vQ1[Y]*vQ2[W]) + (vQ1[Z]*vQ2[X]) );

	vQOut[Z] = (	(vQ1[W]*vQ2[Z]) + (vQ1[X]*vQ2[Y])
				-	(vQ1[Y]*vQ2[X]) + (vQ1[Z]*vQ2[W]) );
}

void Q_RotateVector(vec4_t vQ, vec3_t vVec, vec3_t vOut)
{
	vec4_t vQInv, vQRot, vQt, vQv;

	Q_SetFromVector(vVec, 0.0F, vQv);
	Q_Inverse(vQ, vQInv);
	Q_Multiply(vQ, vQv, vQt);
	Q_Multiply(vQt, vQInv, vQRot);
	Q_GetVector(vQRot, vOut, NULL);
}

void Q_GetEulerAngles(vec4_t vQ, vec3_t vR)
{
	float fax, fbz, fcz, fay, faz, fbx, fby;

	fax = 1.0F-(2.0F*vQ[2]*vQ[2])-(2.0F*vQ[3]*vQ[3]);
	fay = (2.0F*vQ[1]*vQ[2])-(2.0F*vQ[3]*vQ[0]);
	faz = (2.0F*vQ[1]*vQ[3])+(2.0F*vQ[2]*vQ[0]);
	fbx = (2.0F*vQ[1]*vQ[2])+(2.0F*vQ[3]*vQ[0]);
	fby = 1.0F-(2.0F*vQ[1]*vQ[1])-(2.0F*vQ[3]*vQ[3]);
	fbz = (2.0F*vQ[2]*vQ[3])+(2.0F*vQ[1]*vQ[0]);
	fcz = 1.0F-(2.0F*vQ[1]*vQ[1])-(2.0F*vQ[2]*vQ[2]);

	if(faz > 1.0F)
		faz = 1.0F;
	if(faz < -1.0F)
		faz = -1.0F;

	vR[1] = (float)-sin(-faz);
	if(cos(vR[1]) != 0.0F)
	{
		vR[0] = (float)-atan2(fbz, fcz);
		vR[2] = (float)-atan2(fay, fax);
	}
	else
	{
		vR[0] = (float)-atan2(fbx, fby);
		vR[2] = 0.0F;
	}
}

void Q_SetEulerAngles(vec3_t vR, vec4_t vQ)
{
	vec_t cx, cy, cz;
	vec_t sx, sy, sz;

	cx = (float)cos(vR[0]/2.0F);
	sx = (float)sin(vR[0]/2.0F);
	cy = (float)cos(vR[1]/2.0F);
	sy = (float)sin(vR[1]/2.0F);
	cz = (float)cos(vR[2]/2.0F);
	sz = (float)sin(vR[2]/2.0F);

	vQ[0] = cx*cy*cz+sx*sy*sz;
	vQ[1] = sx*cy*cz+cx*sy*sz;
	vQ[2] = cx*sy*cz+sx*cy*sz;
	vQ[3] = cx*cy*sz+sx*sy*cz;
}

float Q_Magnitude(vec4_t vQ)
{
	return (float)sqrt(vQ[0]*vQ[0])+(vQ[1]*vQ[1])+(vQ[2]*vQ[2])+(vQ[3]*vQ[3]);
}

void Q_Normalize(vec4_t vQ)
{
	float fMag = Q_Magnitude(vQ);

	if(fMag < 0.00001F && fMag > -0.00001F)
		return;

	fMag = 1.0F/fMag;

	vQ[0] *= fMag;
	vQ[1] *= fMag;
	vQ[2] *= fMag;
	vQ[3] *= fMag;
}

void Q_Inverse(vec4_t vQ, vec4_t vQOut)
{
	vQOut[0] = vQ[0];
	vQOut[1] = -vQ[1];
	vQOut[2] = -vQ[2];
	vQOut[3] = -vQ[3];
}

void Q_Add(vec4_t vQ1, vec4_t vQ2, vec4_t vQOut)
{
	vQOut[0] = vQ1[0] + vQ2[0];
	vQOut[1] = vQ1[1] + vQ2[1];
	vQOut[2] = vQ1[2] + vQ2[2];
	vQOut[3] = vQ1[3] + vQ2[3];
}

void Q_Subtract(vec4_t vQ1, vec4_t vQ2, vec4_t vQOut)
{
	vQOut[0] = vQ1[0] - vQ2[0];
	vQOut[1] = vQ1[1] - vQ2[1];
	vQOut[2] = vQ1[2] - vQ2[2];
	vQOut[3] = vQ1[3] - vQ2[3];
}

void Q_Scale(vec4_t vQ, float fScale, vec4_t vQOut)
{
	vQOut[0] = vQ[0] * fScale;
	vQOut[1] = vQ[1] * fScale;
	vQOut[2] = vQ[2] * fScale;
	vQOut[3] = vQ[3] * fScale;
}


// Slerpy routines.
// Does spherical interpoation between q1, and q2.
// Temperal=0=q1, Temperal=1=q2.
void Q_InterpolateSphereNormal(vec4_t vQ1, vec4_t vQ2, vec4_t vQOut, float fTemperal)
{
	float fOmega, fCosom, fSinom, fScale0, fScale1;

	// Honestly, I don't know how this works, so I got it from the Genesis Lib.
	fCosom = (vQ1[0] * vQ2[0]) + (vQ1[1] * vQ2[1])
			+ (vQ1[2] * vQ2[2]) + (vQ1[3] * vQ2[3]);

	if((1.0F + fCosom) > 0.00001)
	{
		if((1.0F - fCosom) > 0.00001)
		{
			fOmega = (float)acos(fCosom);
			fSinom = (float)sin(fOmega);

			if(fSinom < 0.00001)
			{
				// Linear interpolate.
				Q_InterpolateLinear(vQ1, vQ2, vQOut, fTemperal);
				return;
			}
			else
			{
				fScale0 = (float)sin((1.0F-fTemperal) * fOmega) / fSinom;
				fScale1 = (float)sin(fTemperal * fOmega) / fSinom;
			}
		}
		else
		{
			// Linear interpolate.
			Q_InterpolateLinear(vQ1, vQ2, vQOut, fTemperal);
			return;
		}

		
		vQOut[0] = fScale0 * vQ1[0] + fScale1 * vQ2[0];
		vQOut[1] = fScale0 * vQ1[1] + fScale1 * vQ2[1];
		vQOut[2] = fScale0 * vQ1[2] + fScale1 * vQ2[2];
		vQOut[3] = fScale0 * vQ1[3] + fScale1 * vQ2[3];
	}
	else
	{
		vQOut[1] = -vQ1[2];
		vQOut[2] = vQ1[1];
		vQOut[3] = -vQ1[0];
		vQOut[0] = vQ1[3];

		fScale0 = (float)sin((1.0F-fTemperal)*PI*0.5F);
		fScale1 = (float)sin(fTemperal*PI*0.5F);
		
		vQOut[1] = fScale0 * vQ2[1] * fScale1 * vQOut[1];
		vQOut[2] = fScale0 * vQ2[2] * fScale1 * vQOut[2];
		vQOut[3] = fScale0 * vQ2[3] * fScale1 * vQOut[3];
		vQOut[0] = fScale0 * vQ2[0] * fScale1 * vQOut[0];

	}
}

void Q_InterpolateSphereShortest(vec4_t vQ1, vec4_t vQ2, vec4_t vQOut, float fTemperal)
{
	float fOmega, fCosom, fSinom, fScale0, fScale1;
	vec4_t vQl;

	// Honestly, I don't know how this works, so I got it from the Genesis Lib.
	fCosom = (vQ1[0] * vQ2[0]) + (vQ1[1] * vQ2[1])
			+ (vQ1[2] * vQ2[2]) + (vQ1[3] * vQ2[3]);

	if(fCosom < 0.0F)
	{
		fCosom = -fCosom;
		Q_Inverse(vQ2, vQl);
	}
	else
		Q_Copy(vQ2, vQl);

	if((1.0F-fCosom) > 0.00001)
	{
		fOmega = (float)acos(fCosom);
		fSinom = (float)sin(fOmega);
		fScale0 = (float)sin((1.0F-fTemperal)*fOmega)/fSinom;
		fScale1 = (float)sin(fTemperal*fOmega)/fSinom;
	}
	else
	{
		Q_InterpolateLinear(vQ1, vQ2, vQOut, fTemperal);
		return;
	}

	vQOut[0] = fScale0 * vQ1[0] + fScale1 * vQ2[0];
	vQOut[1] = fScale0 * vQ1[1] + fScale1 * vQ2[1];
	vQOut[2] = fScale0 * vQ1[2] + fScale1 * vQ2[2];
	vQOut[3] = fScale0 * vQ1[3] + fScale1 * vQ2[3];
}

// I definently understand how this one works :þ
void Q_InterpolateLinear(vec4_t vQ1, vec4_t vQ2, vec4_t vQOut, float fTemperal)
{
	float fScale0, fScale1;

	fScale0 = 1.0F - fTemperal;
	fScale1 = fTemperal;

	vQOut[0] = fScale0 * vQ1[0] + fScale1 * vQ2[0];
	vQOut[1] = fScale0 * vQ1[1] + fScale1 * vQ2[1];
	vQOut[2] = fScale0 * vQ1[2] + fScale1 * vQ2[2];
	vQOut[3] = fScale0 * vQ1[3] + fScale1 * vQ2[3];
}

bool VectorCompare(vec3_t a, vec3_t b, float fEpsilon)
{
	if(a[0] > (b[0]-fEpsilon) && a[0] < (b[0]+fEpsilon))
		if(a[1] > (b[1]-fEpsilon) && a[1] < (b[1]+fEpsilon))
			if(a[2] > (b[2]-fEpsilon) && a[2] < (b[2]+fEpsilon))
				return true;

	return false;
}

void VectorClamp(vec3_t v)
{
	v[0] = (float)floor(v[0] - 0.5f) + 1.0f;
	v[1] = (float)floor(v[1] - 0.5f) + 1.0f;
	v[2] = (float)floor(v[2] - 0.5f) + 1.0f;
}

// Returns true if point is within range.
bool InRange(vec3_t vPoint, vec3_t vMins, vec3_t vMaxs)
{
	if(vPoint[0] >= vMins[0] && vPoint[0] <= vMaxs[0] &&
		vPoint[1] >= vMins[1] && vPoint[1] <= vMaxs[1] &&
		vPoint[2] >= vMins[2] && vPoint[2] <= vMaxs[2])
		return true;

	return false;
}

bool VectorIsZero(vec3_t a)
{
	return VectorCompare(a, vec3_origin, 0.001F);
}

void VectorClear(vec3_t a)
{
	a[0] = 0.0F;
	a[1] = 0.0F;
	a[2] = 0.0F;
}

// Divides two vectors.
void VectorDivide(vec3_t a, vec3_t b, vec3_t out)
{
	out[0] = a[0] / b[0];
	out[1] = a[1] / b[1];
	out[2] = a[2] / b[2];
}

// Computes origin of range.
void ComputeRangeOrigin(vec3_t mins, vec3_t maxs, vec3_t origin)
{
	memset(origin, 0, sizeof(vec3_t));
	VectorAdd(mins, maxs, origin);
	VectorScale(origin, 0.5F, origin);
}

// Orders a range.
void OrderRange(vec3_t mins, vec3_t maxs)
{
	int i;
	vec_t t;

	for(i = 0; i < 3; i++)
	{
		if(mins[i] > maxs[i])
		{
			t = mins[i];
			mins[i] = maxs[i];
			maxs[i] = t;
		}
	}
}

// Returns "true" if ranges intersect.
bool RangesIntersect(vec3_t mins_a, vec3_t maxs_a, vec3_t mins_b, vec3_t maxs_b)
{
	int i;

	for(i = 0; i < 3; i++)
	{
		// Doesn't intersect on axis?
		if(mins_a[i] >= maxs_b[i] || maxs_a[i] <= mins_b[i])
			return(false);
	}

	return(true);
}

// Intializes range values.
void InitializeRange(vec3_t mins, vec3_t maxs)
{
	int i;

	for(i = 0; i < 3; i++)
	{
		mins[i] = (float) 0x7FFFFFFF;
		maxs[i] = (float)-0x7FFFFFFF;
	}
}

// Inserts a range into the mins/maxs.
void InsertRange(vec3_t v, vec3_t mins, vec3_t maxs)
{
	int i;

	// Mins.
	for(i = 0; i < 3; i++)
	{
		if(v[i] < mins[i])
			mins[i] = v[i];
		if(v[i] > maxs[i])
			maxs[i] = v[i];
	}
}

void VectorMA (vec3_t va, float scale, vec3_t vb, vec3_t vc)
{
	vc[0] = va[0] + scale*vb[0];
	vc[1] = va[1] + scale*vb[1];
	vc[2] = va[2] + scale*vb[2];
}

// Vector Copy.
void VectorCopy(const vec3_t src, vec3_t dst)
{
	dst[0]=src[0];
	dst[1]=src[1];
	dst[2]=src[2];
}

// Determines the type of vector.
int ClassifyVector(vec3_t vec)
{
	float n;

	n = (float)fabs(vec[0]);
	if(n > 0.9F)
		return(NF_FACING_X);
	n = (float)fabs(vec[1]);
	if(n > 0.9F)
		return(NF_FACING_Y);
	n = (float)fabs(vec[2]);
	if(n > 0.9F)
		return(NF_FACING_Z);

	return(NF_FACING_ANY);
}

// Rotates a point without lookups.
void VectorRotate(vec3_t a, vec3_t rot, vec3_t out)
{
	float x, y, z;

	x = a[0];
	y = a[1];
	z = a[2];
	
	if(a != out)
		VectorCopy(a, out);

	// Rotate on Z axis.
	if(rot[2] != 0.0F)
	{
		out[0] = x * (float)cos(rot[2]) - y * (float)sin(rot[2]);
		out[1] = y * (float)cos(rot[2]) + x * (float)sin(rot[2]);

		x = out[0];
		y = out[1];
	}

	// Rotate on X axis.
	if(rot[0] != 0.0F)
	{
		out[1] = y * (float)cos(rot[0]) - z * (float)sin(rot[0]);
		out[2] = y * (float)sin(rot[0]) + z * (float)cos(rot[0]);
	
		y = out[1];
		z = out[2];
	}

	// Rotate on Y axis.
	if(rot[1] != 0.0F)
	{
		out[2] = z * (float)cos(rot[1]) - x * (float)sin(rot[1]);
		out[0] = z * (float)sin(rot[1]) + x * (float)cos(rot[1]);
	}

}

// Rotates a point with lookups.
void VectorRotate(vec3_t a, int xrot, int yrot, int zrot, vec3_t out)
{
	float x, y, z;

	x = a[0];
	y = a[1];
	z = a[2];

	if(a != out)
		VectorCopy(a, out);

	// Rotate on Z axis.
	if(zrot != 0.0F)
	{
		out[0] = x * COS(zrot) - y * SIN(zrot);
		out[1] = y * COS(zrot) + x * SIN(zrot);

		x = out[0];
		y = out[1];
	}

	// Rotate on X axis.
	if(xrot != 0.0F)
	{
		out[1] = y * COS(xrot) - z * SIN(xrot);
		out[2] = y * COS(xrot) + z * COS(xrot);
	
		y = out[1];
		z = out[2];
	}

	// Rotate on Y axis.
	if(yrot != 0.0F)
	{
		out[2] = z * COS(yrot) - x * SIN(yrot);
		out[0] = z * SIN(yrot) + x * COS(yrot);
	}

}

// Builds trigonometry tables.
bool BuildTrigTables(void)
{
	int index;
	double dstep = (2.0 * PI) / ((float)MAX_TRIG_ANGLES);
	double dangle = 0.0F;

	for(index = 0; index < MAX_TRIG_ANGLES; index++)
	{
		gSinTable[index] = (float)sin(dangle);
		gCosTable[index] = (float)cos(dangle);

		dangle += dstep;
	}

	return(true);
}

// Multiplies two vectors.
void VectorMultiply(vec3_t a, vec3_t b, vec3_t out)
{
	out[0] = a[0] * b[0];
	out[1] = a[1] * b[1];
	out[2] = a[2] * b[2];
}

// Scales a point.
void VectorScale(vec3_t p, vec_t scale, vec3_t out)
{
	out[0] = p[0] * scale;
	out[1] = p[1] * scale;
	out[2] = p[2] * scale;
}

// Computes an orthogal vector.
void CalcOrthoVector(vec3_t a, vec3_t b, vec3_t out)
{
	// Compute the cross dot.
	CrossProduct(a, b, out);

	// Normalize new vector.
	VectorNormalize(out);
}

// Adds two frikin' vectors.
void VectorAdd(vec3_t a, vec3_t b, vec3_t out)
{
	// Add the vector components.
	out[0] = a[0] + b[0];
	out[1] = a[1] + b[1];
	out[2] = a[2] + b[2];
}

// Subtracts vectors.
void VectorSubtract(vec3_t a, vec3_t b, vec3_t out)
{
	// Add the vector components.
	out[0] = a[0] - b[0];
	out[1] = a[1] - b[1];
	out[2] = a[2] - b[2];
}

// Returns the magnitude of a vector.
vec_t VectorLength(vec3_t a)
{
	float mag;
	
	mag = (float)(a[0] * a[0] + a[1] * a[1] + a[2] * a[2]);
	mag = (float)sqrt((double)mag);
	return(mag);
}

// Performs dot product of two vectors.
vec_t DotProduct(vec3_t a, vec3_t b)
{
	float dot;
	
	// Perform the dot.
	dot = (float)(a[0] * b[0] + a[1] * b[1] + a[2] * b[2]);
	return(dot);
}

#ifdef _WIN32
// Assembly version of the DotPoint.
vec_t DotProduct_asm(vec3_t a, vec3_t b)
{
	float dot;

	_asm
	{
		fld   [a+0]
		fmul  [b+0]
		fld   [a+4]
		fmul  [b+4]
		fld   [a+8]
		fmul  [b+8]
		fxch  st(1)
		faddp st(2), st(0)
		faddp st(1), st(0)
		fstp  [dot]
	}

	return(dot);
}
#endif

// Performs the cross dot of two vectors leaving the result
// in the first.
void CrossProduct(vec3_t a, vec3_t b, vec3_t out)
{
	// Perform the dot.
	out[0] = (a[1] * b[2]) - (a[2] * b[1]);
	out[1] = (a[2] * b[0]) - (a[0] * b[2]);
	out[2] = (a[0] * b[1]) - (a[1] * b[0]);

}

// Ahh, tis ahab, and his fat whale!!! Is the whale white?
// Iye captain, sure tis white, and sure tis fat. Shall we
// normalize 'im matey?
void VectorNormalize(vec3_t a)
{
	// Get the magnitude of the vector.
	float mag = VectorLength(a);

	// Divide all components by the length, thusly normalizing
	// it.
	a[0] = a[0] / mag;
	a[1] = a[1] / mag;
	a[2] = a[2] / mag;
}