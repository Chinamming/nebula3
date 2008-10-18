#pragma once
#ifndef MATH_D3DX9_SCALAR_H
#define MATH_D3DX9_SCALAR_H
//------------------------------------------------------------------------------
/**
    @file math/d3dx9/d3dx9_scalar.h
    
    Scalar typedef and math functions for D3DX9 math functions.
    
    (C) 2007 Radon Labs GmbH
*/
#include "core/types.h"

namespace Math
{
typedef float scalar;

#ifndef PI
#define PI (3.1415926535897932384626433832795028841971693993751f)
#endif
#define N_PI PI

#ifndef TINY
#define TINY (0.0000001f)
#endif
#define N_TINY TINY

const scalar LN_2 = 0.693147180559945f;

//------------------------------------------------------------------------------
/**
*/
__forceinline scalar
n_max(scalar a, scalar b)
{
    return (a > b) ? a : b;
}

//------------------------------------------------------------------------------
/**
*/
__forceinline double
n_max(double a, double b)
{
    return (a > b) ? a : b;
}

//------------------------------------------------------------------------------
/**
*/
__forceinline int
n_max(int a, int b)
{
    return (a > b) ? a : b;
}

//------------------------------------------------------------------------------
/**
*/
__forceinline scalar
n_min(scalar a, scalar b)
{
    return (a < b) ? a : b;
}

//------------------------------------------------------------------------------
/**
*/
__forceinline double
n_min(double a, double b)
{
    return (a < b) ? a : b;
}

//------------------------------------------------------------------------------
/**
*/
__forceinline int
n_min(int a, int b)
{
    return (a < b) ? a : b;
}

//------------------------------------------------------------------------------
/**
*/
__forceinline scalar
n_abs(scalar a)
{
    return (a < 0.0f) ? -a : a;
}

//------------------------------------------------------------------------------
/**
*/
__forceinline double
n_abs(double a)
{
    return (a < 0.0f) ? -a : a;
}

//------------------------------------------------------------------------------
/**
*/
__forceinline int
n_abs(int a)
{
    return (a < 0.0f) ? -a : a;
}

//------------------------------------------------------------------------------
/**
*/
__forceinline scalar
n_sgn(scalar a)
{
    return (a < 0.0f) ? -1.0f : 1.0f;
}

//------------------------------------------------------------------------------
/**
*/
__forceinline scalar
n_deg2rad(scalar d)
{
    return (d * PI) / 180.0f;
}

//------------------------------------------------------------------------------
/**
*/
__forceinline scalar
n_rad2deg(scalar r)
{
    return (r * 180.0f) / PI;
}

//------------------------------------------------------------------------------
/**
*/
__forceinline scalar
n_sin(scalar x)
{
    return sinf(x);
}

//------------------------------------------------------------------------------
/**
*/
__forceinline scalar
n_cos(scalar x)
{
    return cosf(x);
}

//------------------------------------------------------------------------------
/**
*/
__forceinline scalar
n_tan(scalar x)
{
    return tanf(x);
}

//------------------------------------------------------------------------------
/**
*/
__forceinline scalar
n_asin(scalar x)
{
    return asinf(x);
}

//------------------------------------------------------------------------------
/**
*/
__forceinline scalar
n_acos(scalar x)
{
    return acosf(x);
}

//------------------------------------------------------------------------------
/**
*/
__forceinline scalar
n_atan(scalar x)
{
    return atanf(x);
}

//------------------------------------------------------------------------------
/**
    log2() function.
*/
__forceinline scalar 
n_log2(scalar f) 
{ 
    return logf(f) / LN_2; 
}

//------------------------------------------------------------------------------
/**
    Integer clamping.
*/
__forceinline int 
n_iclamp(int val, int minVal, int maxVal)
{
    if (val < minVal)      return minVal;
    else if (val > maxVal) return maxVal;
    else return val;
}

//------------------------------------------------------------------------------
/**
*/
__forceinline scalar 
n_sqrt(scalar x)
{
    return sqrtf(x);
}

//------------------------------------------------------------------------------
/**
*/
__forceinline scalar
n_exp(scalar x)
{
    return expf(x);
}

//------------------------------------------------------------------------------
/**
    A fuzzy floating point equality check
*/
__forceinline bool 
n_fequal(scalar f0, scalar f1, scalar tol) 
{
    scalar f = f0 - f1;
    return ((f > (-tol)) && (f < tol));
}

//------------------------------------------------------------------------------
/**
    A fuzzy floating point less-then check.
*/
__forceinline bool 
n_fless(scalar f0, scalar f1, scalar tol) 
{
    return ((f0 - f1) < tol);
}

//------------------------------------------------------------------------------
/**
    A fuzzy floating point greater-then check.
*/
__forceinline bool 
n_fgreater(scalar f0, scalar f1, scalar tol) 
{
    return ((f0 - f1) > tol);
}

//------------------------------------------------------------------------------
/**
    Clamp a value against lower und upper boundary.
*/
__forceinline scalar
n_clamp(scalar val, scalar lower, scalar upper)
{
    if (val < lower)      return lower;
    else if (val > upper) return upper;
    else                  return val;
}

//------------------------------------------------------------------------------
/**
    Saturate a value (clamps between 0.0f and 1.0f)
*/
__forceinline scalar 
n_saturate(scalar val)
{
    if (val < 0.0f)      return 0.0f;
    else if (val > 1.0f) return 1.0f;
    else return val;
}

//------------------------------------------------------------------------------
/**
    Return a pseudo random number between 0 and 1.
*/
__forceinline
scalar n_rand()
{
    return scalar(rand()) / scalar(RAND_MAX);
}

//------------------------------------------------------------------------------
/**
    Return a pseudo random number between min and max.
*/
__forceinline scalar 
n_rand(scalar min, scalar max)
{
	scalar unit = scalar(rand()) / RAND_MAX;
	scalar diff = max - min;
	return min + unit * diff;
}

//------------------------------------------------------------------------------
/**
    Chop float to int.
*/
__forceinline int 
n_fchop(scalar f)
{
    /// @todo type cast to int is slow!
    return int(f);
}

//------------------------------------------------------------------------------
/**
    Round float to integer.
*/
__forceinline int 
n_frnd(scalar f)
{
    return n_fchop(floorf(f + 0.5f));
}

//------------------------------------------------------------------------------
/**
    Linearly interpolate between 2 values: ret = x + l * (y - x)
*/
__forceinline float 
n_lerp(scalar x, scalar y, scalar l)
{
    return x + l * (y - x);
}

//------------------------------------------------------------------------------
/**
*/
__forceinline scalar
n_fmod(scalar x, scalar y)
{
    return fmodf(x, y);
}

//------------------------------------------------------------------------------
/**
    Normalize an angular value into the range rad(0) to rad(360).
*/
__forceinline scalar 
n_modangle(scalar a) 
{
    // FIXME: hmm...
    while(a < 0.0f)
    {
        a += n_deg2rad(360.0f);
    }
    if (a >= n_deg2rad(360.0f))
    {
        a = n_fmod(a, n_deg2rad(360.0f));
    }
    return a;
}

//------------------------------------------------------------------------------
/**
    Get angular distance.
*/
__forceinline scalar 
n_angulardistance(scalar from, scalar to)
{
	scalar normFrom = n_modangle(from);
    scalar normTo   = n_modangle(to);
    scalar dist = normTo - normFrom;
    if (dist < n_deg2rad(-180.0f))
    {
        dist += n_deg2rad(360.0f);
    }
    else if (dist > n_deg2rad(180.0f))
    {
        dist -= n_deg2rad(360.0f);
    }
    return dist;
}

//------------------------------------------------------------------------------
/**
*/
__forceinline scalar
n_pow(scalar x, scalar y)
{
    return powf(x, y);
}

} // namespace Math
//------------------------------------------------------------------------------
#endif



    