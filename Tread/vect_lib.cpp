/******************************************************************************

 Copyright (c) 1999 Advanced Micro Devices, Inc.

 LIMITATION OF LIABILITY:  THE MATERIALS ARE PROVIDED *AS IS* WITHOUT ANY
 EXPRESS OR IMPLIED WARRANTY OF ANY KIND INCLUDING WARRANTIES OF MERCHANTABILITY,
 NONINFRINGEMENT OF THIRD-PARTY INTELLECTUAL PROPERTY, OR FITNESS FOR ANY
 PARTICULAR PURPOSE.  IN NO EVENT SHALL AMD OR ITS SUPPLIERS BE LIABLE FOR ANY
 DAMAGES WHATSOEVER (INCLUDING, WITHOUT LIMITATION, DAMAGES FOR LOSS OF PROFITS,
 BUSINESS INTERRUPTION, LOSS OF INFORMATION) ARISING OUT OF THE USE OF OR
 INABILITY TO USE THE MATERIALS, EVEN IF AMD HAS BEEN ADVISED OF THE POSSIBILITY
 OF SUCH DAMAGES.  BECAUSE SOME JURISDICTIONS PROHIBIT THE EXCLUSION OR LIMITATION
 OF LIABILITY FOR CONSEQUENTIAL OR INCIDENTAL DAMAGES, THE ABOVE LIMITATION MAY
 NOT APPLY TO YOU.

 AMD does not assume any responsibility for any errors which may appear in the
 Materials nor any responsibility to support or update the Materials.  AMD retains
 the right to make changes to its test specifications at any time, without notice.

 NO SUPPORT OBLIGATION: AMD is not obligated to furnish, support, or make any
 further information, software, technical information, know-how, or show-how
 available to you.

 So that all may benefit from your experience, please report  any  problems
 or  suggestions about this software to 3dsdk.support@amd.com

 AMD Developer Technologies, M/S 585
 Advanced Micro Devices, Inc.
 5900 E. Ben White Blvd.
 Austin, TX 78741
 3dsdk.support@amd.com

*******************************************************************************

 VECT_LIB.C

 AMD3D 3D library code: Vector math
    The majority of these routines are in vect.asm - this file only
    provides a C wrapper for functions needing to return a float value.

******************************************************************************/

#include "stdafx.h"
#include <math.h>
#include <amd3dx.h>
#include "vect_lib.h"


/******************************************************************************
 Routine:   _mag_vect
 Input:     a - vector (1x3) address
 Output:    return vector magnitude
******************************************************************************/
float _mag_vect (const vec3_t a)
{
    float r;
    __asm {
        femms
        mov         eax,a
        movq        mm0,[eax]
        movd        mm1,[eax+8]
        pfmul       (mm0,mm0)
        pfmul       (mm1,mm1)
        pfacc       (mm0,mm0)
        pfadd       (mm0,mm1)
        pfrsqrt     (mm1,mm0)
        movq        mm2,mm1
        pfmul       (mm1,mm1)
        pfrsqit1    (mm1,mm0)
        pfrcpit2    (mm1,mm2)
        pfmul       (mm0,mm1)
        movd        r,mm0
        femms
    }
    return r;
}

/******************************************************************************
 Routine:   _dot_vect
 Input:     a - vector (1x3) address
            b - vector (1x3) address
 Output:    return (a DOT b)
******************************************************************************/
float _dot_vect (const vec3_t a, const vec3_t b)
{
    float r;
    __asm {
        femms
        mov         eax,a
        mov         edx,b
        movq        mm0,[eax]
        movq		mm3,[edx]
        pfmul       (mm0,mm3)
        movd		mm2,[eax+8]
        movd        mm1,[edx+8]
        pfacc       (mm0,mm0)
        pfmul       (mm1,mm2)
        pfadd       (mm0,mm1)
        movd        r,mm0
        femms
    }
    return r;
}


/******************************************************************************
 Routine:   add_vect
 Input:     r - resultant vector
            a - input vector
            b - input vector
 Output:    r[i] = a[i] + b[i]
******************************************************************************/
void add_vect (vec3_t r, const vec3_t a, const vec3_t b)
{
    r[0] = a[0] + b[0];
    r[1] = a[1] + b[1];
    r[2] = a[2] + b[2];
}


/******************************************************************************
 Routine:   sub_vect
 Input:     r - resultant vector
            a - input vector
            b - input vector
 Output:    r[i] = a[i] - b[i]
******************************************************************************/
void sub_vect (vec3_t r, const vec3_t a, const vec3_t b)
{
    r[0] = a[0] - b[0];
    r[1] = a[1] - b[1];
    r[2] = a[2] - b[2];
}


/******************************************************************************
 Routine:   mult_vect
 Input:     r - resultant vector
            a - input vector
            b - input vector
 Output:    r[i] = a[i] * b[i]
******************************************************************************/
void mult_vect (vec3_t r, const vec3_t a, const vec3_t b)
{
    r[0] = a[0] * b[0];
    r[1] = a[1] * b[1];
    r[2] = a[2] * b[2];
}


/******************************************************************************
 Routine:   scale_vect
 Input:     r - resultant vector
            a - input vector
            f - scaling factor
 Output:    r[i] = a[i] * f
******************************************************************************/
void scale_vect (vec3_t r, const vec3_t a, float b)
{
    r[0] = a[0] * b;
    r[1] = a[1] * b;
    r[2] = a[2] * b;
}


/******************************************************************************
 Routine:   norm_vect
 Input:     r - resultant vector
            a - input vector
 Output:    r[i] = a[i] / mag_vect (a)
******************************************************************************/
void norm_vect(vec3_t r, const vec3_t a)
{
    float len = (float)sqrt ((a[0] * a[0] + a[1] * a[1] + a[2] * a[2]));
    if (len < 0.0001)
    {
        r[0] = a[0];
        r[1] = a[1];
        r[2] = a[2];
    }
    else
    {
        len = 1.0f/len;
        r[0] = a[0]*len;
        r[1] = a[1]*len;
        r[2] = a[2]*len;
    }
}


/******************************************************************************
 Routine:   mag_vect
 Input:     a - input vector
 Output:    return magnitude (length) of 'a'
******************************************************************************/
float mag_vect (const vec3_t a)
{
    return (float)sqrt ((a[0] * a[0] + a[1] * a[1] + a[2] * a[2]));
}


/******************************************************************************
 Routine:   dot_vect
 Input:     a - input vector
            b - input vector
 Output:    return (a DOT b)
******************************************************************************/
float dot_vect (const vec3_t a, const vec3_t b)
{
    return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}


/******************************************************************************
 Routine:   cross_vect
 Input:     r - resultant vector
            a - input vector
            b - input vector
 Output:    r = cross product of 'a' and 'b'
******************************************************************************/
void cross_vect (vec3_t r, const vec3_t a, const vec3_t b)
{
    r[0] = a[1] * b[2] - a[2] * b[1];
    r[1] = a[2] * b[0] - a[0] * b[2];
    r[2] = a[0] * b[1] - a[1] * b[0];
}
 
// eof