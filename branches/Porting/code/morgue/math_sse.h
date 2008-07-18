#pragma once
#ifndef MATH_MATHSSE_H
#define MATH_MATHSSE_H
//------------------------------------------------------------------------------
/**
    @file math/math_sse.h
    
    Implements the Nebula3 lowlevel math library for the SSE instruction
    set.
    
    (C) 2006 Radon Labs GmbH
*/
#include "core/types.h"
#define _USE_MATH_DEFINES (1)
#include <math.h>
#include <float.h>
#include <xmmintrin.h>

// typedef for 4-component vector
typedef __m128 float4_sse;
typedef __m128 float4_arg_sse;
typedef __m128& float4_ref_sse;

// typedef for 4x4 matrix
struct float4x4_sse_struct
{
    __m128 col[4];
};
typedef float4x4_sse_struct float4x4_sse;
typedef const float4x4_sse_struct& float4x4_arg_sse;
typedef float4x4_sse_struct& float4x4_ref_sse;

//--- constants ---
const float pi_sse = M_PI;
extern const float4_sse clearx_mask_sse;
extern const float4_sse cleary_mask_sse;
extern const float4_sse clearz_mask_sse;
extern const float4_sse clearw_mask_sse;
extern const float4_sse zerovec_sse;
extern const float4_sse minusonevec_sse;
extern const float4_sse deg2radvec_sse;
extern const float4_sse rad2degvec_sse;

//--- read data from memory ---
void prefetch_sse(const float* ptr);
float4_sse load_sse(const float* ptr);
float4_sse loadu_sse(const float* ptr);
float4x4_sse loadcm_sse(const float* ptr);
float4x4_sse loadrm_sse(const float* ptr);
float4x4_sse loadcmu_sse(const float* ptr);
float4x4_sse loadrmu_sse(const float* ptr);

//--- write data to memory (cached) ---
void store_sse(float* ptr, float4_arg_sse val);
void storeu_sse(float* ptr, float4_arg_sse val);
void storecm_sse(float* ptr, float4x4_arg_sse val);
void storecmu_sse(float* ptr, float4x4_arg_sse val);
void storerm_sse(float* ptr, float4x4_arg_sse val);
void storermu_sse(float* ptr, float4x4_arg_sse val);

//--- write data to memory without polluting the cache --
void stream_sse(float* ptr, float4_arg_sse val);
void streamcm_sse(float* ptr, float4x4_arg_sse val);
void streamrm_sse(float* ptr, float4x4_arg_sse val);

//--- pseudo constructors ---
float4_sse _float4_sse();        
float4_sse _float4_sse(float s);
float4_sse _float4_sse(float x, float y, float z, float w);
float4x4_sse _float4x4_sse();

//--- set data ---
float4_sse setzero_sse();
float4_sse set_sse(float s);
float4_sse set_sse(float x, float y, float z, float w);
float4_sse setx_sse(float4_arg_sse in, float x);
float4_sse sety_sse(float4_arg_sse in, float y);
float4_sse setz_sse(float4_arg_sse in, float z);
float4_sse setw_sse(float4_arg_sse in, float w);

float4x4_sse setidentity_sse();
void setcol0_sse(float4x4_ref_sse in, float4_arg_sse col0);
void setcol1_sse(float4x4_ref_sse in, float4_arg_sse col1);
void setcol2_sse(float4x4_ref_sse in, float4_arg_sse col2);
void setcol3_sse(float4x4_ref_sse in, float4_arg_sse col3);
void setrow0_sse(float4x4_ref_sse in, float4_arg_sse row0);
void setrow1_sse(float4x4_ref_sse in, float4_arg_sse row1);
void setrow2_sse(float4x4_ref_sse in, float4_arg_sse row2);
void setrow3_sse(float4x4_ref_sse in, float4_arg_sse row3);

//--- get component data ---
float getx_sse(float4_arg_sse val);
float gety_sse(float4_arg_sse val);
float getz_sse(float4_arg_sse val);
float getw_sse(float4_arg_sse val);
float4_sse getcol0_sse(float4x4_arg_sse val);
float4_sse getcol1_sse(float4x4_arg_sse val);
float4_sse getcol2_sse(float4x4_arg_sse val);
float4_sse getcol3_sse(float4x4_arg_sse val);
float4_sse getrow0_sse(float4x4_arg_sse val);
float4_sse getrow1_sse(float4x4_arg_sse val);
float4_sse getrow2_sse(float4x4_arg_sse val);
float4_sse getrow3_sse(float4x4_arg_sse val);

//--- component-wise and scalar operators ---
float4_sse operator+(float4_arg_sse v0, float4_arg_sse v1);
float4_sse operator+(float4_arg_sse v, float s);
float4_sse operator+(float s, float4_arg_sse v);
float4_sse operator-(float4_arg_sse v0, float4_arg_sse v1);
float4_sse operator-(float4_arg_sse v, float s);
float4_sse operator*(float4_arg_sse v0, float4_arg_sse v1);
float4_sse operator*(float4_arg_sse v, float s);
float4_sse operator*(float s, float4_arg_sse v);
float4_sse operator/(float4_arg_sse v0, float4_arg_sse v1);
float4_sse operator/(float4_arg_sse v, float s);

// compare operators and functions (SLOW!!!)
bool operator==(float4_arg_sse v0, float4_arg_sse v1);
bool isequal_sse(float4_arg_sse v0, float4_arg_sse v1, float tolerance);

//--- component-wise min/max ---
float max_sse(float a, float b);
float4_sse max_sse(float4_arg_sse a, float4_arg_sse b);

//--- component-wise min ---
float min_sse(float a, float b);
float4_sse min_sse(float4_arg_sse a, float4_arg_sse b);

//--- component-wise reciprocal (1 / a)---
float rcp_sse(float a);
float4_sse rcp_sse(float4_arg_sse a);

//--- component-wise square root ---
float sqrt_sse(float a);
float4_sse sqrt_sse(float4_arg_sse a);

//--- component-wise reciprocal square root (1 / sqrt(a)) ---
float rsqrt_sse(float a);
float4_sse rsqrt_sse(float4_arg_sse a);

//--- return horizontal sum of vector components ---
float hsum_sse(float4_arg_sse a);

//--- component-wise absolute value ---
float abs_sse(float a);
float4_sse abs_sse(float4_arg_sse a);

//--- component-wise arccosine ---
float acos_sse(float a);
float4_sse acos_x86(float4_arg_sse a);

//--- component-wise arcsine ---
float asin_sse(float a);
float4_sse asin_sse(float4_arg_sse a);

//--- component-wise arctangent ---
float atan_sse(float a);
float4_sse atan_sse(float4_arg_sse a);

//--- component-wise arctangent of x/y ---
float atan2_sse(float x, float y);
float4_sse atan2_sse(float4_arg_sse x, float4_arg_sse y);

//--- component-wise ceil() ---
float ceil_sse(float x);
float4_sse ceil_sse(float4_arg_sse x);

//--- component-wise clamp to min/max ---
float clamp_sse(float x, float min, float max);
float4_sse clamp_x86(float4_arg_sse x, float4_arg_sse min, float4_arg_sse max);

//--- component-wise cosine ---
float cos_sse(float x);
float4_sse cos_sse(float4_arg_sse x);

//--- component-wise hyperbolic cosine ---
float cosh_sse(float x);
float4_sse cosh_sse(float4_arg_sse x);

//--- cross-product of 2 vectors ---
float4_sse cross_sse(float4_arg_sse x, float4_arg_sse y);

//--- component-wise convert from radians to degrees ---
float degrees_sse(float x);
float4_sse degrees_sse(float4_arg_sse x);

//--- returns the determinant of matrix ---
float determinant_sse(float4x4_arg_sse m);

//--- distance between 2 points x and y ---
float distance_sse(float4_arg_sse x, float4_arg_sse y);

//--- distance squared between 2 points x and y ---
float distancesquared_sse(float4_arg_sse x, float4_arg_sse y);

//--- dot product of 2 vectors x and y ---
float dot_sse(float4_arg_sse x, float4_arg_sse y);

//--- component-wise base-e exponential (e^x) ---
float exp_sse(float x);
float4_sse exp_sse(float4_arg_sse x);

//--- component-wise floor() ---
float floor_sse(float x);
float4_sse floor_sse(float4_arg_sse x);

//--- component-wise floating point remainder ---
float fmod_sse(float x);
float4_sse fmod_sse(float4_arg_sse x, float4_arg_sse y);

//--- component-wise fractional part ---
float frac_sse(float x);
float4_sse frac_sse(float4_arg_sse x);

//--- length of vector ---
float length_sse(float4_arg_sse x);

//--- length-squared of vector ---
float lengthsquared_sse(float4_arg_sse x);

//--- returns x + s * (y - x) ---
float lerp_sse(float x, float y, float s);
float4_sse lerp_sse(float4_arg_sse x, float4_arg_sse y, float4_arg_sse s);
float4_sse lerp_sse(float4_arg_sse x, float4_arg_sse y, float s);

//--- component-wise base-e logarithm ---
float log_sse(float x);
float4_sse log_sse(float4_arg_sse x);

//--- component-wise base-10 logarithm ---
float log10_sse(float x);
float4_sse log10_sse(float4_arg_sse x);

//--- component-wise split fractional and integer part ---
float modf_sse(float x, float& out_ip);
float4_sse modf_sse(float4_arg_sse x, float4_ref_sse out_ip);

//--- matrix multiplications ---
float mul_sse(float4_arg_sse v0, float4_arg_sse v1);

float4_sse mul_sse(float s, float4_arg_sse v0);
float4_sse mul_sse(float4_arg_sse v0, float s);
float4_sse mul_sse(float4_arg_sse v0, float4x4_arg_sse m0);
float4_sse mul_sse(float4x4_arg_sse m0, float4_arg_sse v0);

float4x4_sse mul_sse(float s, float4x4_arg_sse m0);
float4x4_sse mul_sse(float4x4_arg_sse m0, float s);
float4x4_sse mul_sse(float4x4_arg_sse m0, float4x4_arg_sse m1);

//--- return normalized vector ---
float4_sse normalize_sse(float4_arg_sse x);

//--- component-wise x^y ---
float pow_sse(float x, float y);
float4_sse pow_sse(float4_arg_sse x, float4_arg_sse y);

//--- component-wise convert degree to radians ---
float radians_sse(float x);
float4_sse radians_sse(float4_arg_sse x);

//--- compute reflection vector v = i - 2 * dot(i, n) * n ---
float4_sse reflect_sse(float4_arg_sse i, float4_arg_sse n);

//--- compute refraction vector ---
float4_sse refract_sse(float4_arg_sse i, float4_arg_sse n, float ri);

//--- component wise round to nearest integer ---
float round_sse(float x);
float4_sse round_sse(float4_arg_sse x);

//--- component-wise clamp to (0,1) ---
float saturate_sse(float x);
float4_sse saturate_sse(float4_arg_sse x);

//--- component-wise compute sign ---
float sign_sse(float x);
float4_sse sign_sse(float4_arg_sse x);

//--- component-wise compute sine ---
float sin_sse(float x);
float4_sse sin_sse(float4_arg_sse x);

//--- component-wise compute sine and cosine ---
void sincos_sse(float x, float& s, float& c);
void sincos_sse(float4_arg_sse x, float4_ref_sse s, float4_ref_sse c);

//--- component-wise hyperbolic sine ---
float sinh_sse(float x);
float4_sse sinh_sse(float4_arg_sse x);

//--- compute smooth Hermite interpolation between 0 and 1 ---
float smoothstep_sse(float min, float max, float x);
float4_sse smoothstep_sse(float4_arg_sse min, float4_arg_sse max, float4_arg_sse x);

//--- component-wise tangent ---
float tan_sse(float x);
float4_sse tan_sse(float4_arg_sse x);

//--- component-wise hyperbolic tangent ---
float tanh_sse(float x);
float4_sse tanh_sse(float4_arg_sse x);

//--- return transpose of input matrix ---
float4x4_sse transpose_sse(float4x4_arg_sse m);

//--- return inverted matrix ---
float4x4_sse invert_sse(float4x4_arg_sse m);

//--- return inverted matrix, assume it's an rotate/scale/translate matrix ---
float4x4_sse invertsimple_sse(float4x4_arg_sse m);

//------------------------------------------------------------------------------
/**
    Load data at ptr to a location closer to the processor.
    FIXME: What to the _MM_HINT_XX macros mean? They are undocumented
    in the platform SDK.
*/
inline void
prefetch_sse(const float* ptr)
{
    _mm_prefetch((const char*) ptr, _MM_HINT_NTA);
}

//------------------------------------------------------------------------------
/**
    Load 4 values from 16-byte aligned memory location into 
    4-component vector.
*/
inline float4_sse 
load_sse(const float* ptr)
{
    return _mm_load_ps(ptr);
}

//------------------------------------------------------------------------------
/**
    Load 4 values from non-aligned memory location into 
    4-component vector. This is slower then load_sse()!
*/
inline float4_sse
loadu_sse(const float* ptr)
{
    return _mm_loadu_ps(ptr);
}

//------------------------------------------------------------------------------
/**
    Load 4x4 matrix from memory in column major format from 16-byte-aligned
    memory. This is the canonical matrix format and thus faster then the 
    row-major version of the method.
*/
inline float4x4_sse
loadcm_sse(const float* ptr)
{
    float4x4_sse m;
    m.col[0] = _mm_load_ps(ptr);
    m.col[1] = _mm_load_ps(ptr + 4);
    m.col[2] = _mm_load_ps(ptr + 8);
    m.col[3] = _mm_load_ps(ptr + 12);
    return m;
}

//------------------------------------------------------------------------------
/**
    Load 4x4 matrix from memory in row major format from 16-byte-aligned
    memory. This performs a transpose operation because the canonical 
    matrix format is column major, and thus is slower the loadcm_sse().
*/
inline float4x4_sse
loadrm_sse(const float* ptr)
{
    float4x4_sse m;
    m.col[0] = _mm_loadu_ps(ptr);
    m.col[1] = _mm_loadu_ps(ptr + 4);
    m.col[2] = _mm_loadu_ps(ptr + 8);
    m.col[3] = _mm_loadu_ps(ptr + 12);
    _MM_TRANSPOSE4_PS(m.col[0], m.col[1], m.col[2], m.col[3]);
    return m;
}

//------------------------------------------------------------------------------
/**
    Load 4x4 matrix from memory in column major format from non-aligned
    memory.
*/
inline float4x4_sse
loadcmu_sse(const float* ptr)
{
    float4x4_sse m;
    m.col[0] = _mm_loadu_ps(ptr);
    m.col[1] = _mm_loadu_ps(ptr + 4);
    m.col[2] = _mm_loadu_ps(ptr + 8);
    m.col[3] = _mm_loadu_ps(ptr + 12);
    return m;
}

//------------------------------------------------------------------------------
/**
    Store 4-component vector to 16-byte-aligned memory through the cache. 
*/
inline void
store_sse(float* ptr, float4_arg_sse val)
{
    _mm_store_ps(ptr, val);
}

//------------------------------------------------------------------------------
/**
    Store 4-component vector to unaligned memory through the cache. 
*/
inline void
storeu_sse(float* ptr, float4_arg_sse val)
{
    _mm_storeu_ps(ptr, val);
}

//------------------------------------------------------------------------------
/**
    Store 4x4 matrix in column-major order to 16-byte-aligned memory.
    This is the faster then the row-major version of this method.
*/
inline void
storecm_sse(float* ptr, float4x4_arg_sse val)
{
    _mm_store_ps(ptr, val.col[0]);
    _mm_store_ps(ptr + 4, val.col[1]);
    _mm_store_ps(ptr + 8, val.col[2]);
    _mm_store_ps(ptr + 12, val.col[3]);
}

//------------------------------------------------------------------------------
/**
    Store 4x4 matrix in row-major order to 16-byte-aligned memory. This
    is slower then storing the matrix in column major order, because
    a transpose is involved.
*/
inline void
storerm_sse(float* ptr, float4x4_arg_sse val)
{
    float4x4_sse t = val;
    _MM_TRANSPOSE4_PS(t.col[0], t.col[1], t.col[2], t.col[3]);
    _mm_store_ps(ptr, t.col[0]);
    _mm_store_ps(ptr + 4, t.col[1]);
    _mm_store_ps(ptr + 8, t.col[2]);
    _mm_store_ps(ptr + 12, t.col[3]);
}

//------------------------------------------------------------------------------
/**
    Store 4x4 matrix in column-major order to unaligned memory.
*/
inline void
storecmu_sse(float* ptr, float4x4_arg_sse val)
{
    _mm_storeu_ps(ptr, val.col[0]);
    _mm_storeu_ps(ptr + 4, val.col[1]);
    _mm_storeu_ps(ptr + 8, val.col[2]);
    _mm_storeu_ps(ptr + 12, val.col[3]);
}

//------------------------------------------------------------------------------
/**
    Store 4x4 matrix in row-major order to unaligned memory.
*/
inline void
storermu_sse(float* ptr, float4x4_arg_sse val)
{
    float4x4_sse t = val;
    _MM_TRANSPOSE4_PS(t.col[0], t.col[1], t.col[2], t.col[3]);
    _mm_storeu_ps(ptr, t.col[0]);
    _mm_storeu_ps(ptr + 4, t.col[1]);
    _mm_storeu_ps(ptr + 8, t.col[2]);
    _mm_storeu_ps(ptr + 12, t.col[3]);
}

//------------------------------------------------------------------------------
/**
    Write a 4-component vector to 16-byte-aligned memory without 
    polluting the cache.
*/
inline void
stream_sse(float* ptr, float4_arg_sse val)
{
    _mm_stream_ps(ptr, val);
}

//------------------------------------------------------------------------------
/**
    Write a 4x4-matrix in colum-majororder to 16-byte-aligned memory 
    without polluting the cache. This is faster then the row-major
    version.
*/
inline void
streamcm_sse(float* ptr, float4x4_arg_sse val)
{
    _mm_stream_ps(ptr, val.col[0]);
    _mm_stream_ps(ptr + 4, val.col[1]);
    _mm_stream_ps(ptr + 8, val.col[2]);
    _mm_stream_ps(ptr + 12, val.col[3]);
}

//------------------------------------------------------------------------------
/**
    Write a 4x4-matrix in row-majororder to 16-byte-aligned memory 
    without polluting the cache. This is slower then the column-major
    version.
*/
inline void
streamrm_sse(float* ptr, float4x4_arg_sse val)
{
    float4x4_sse t = val;
    _MM_TRANSPOSE4_PS(t.col[0], t.col[1], t.col[2], t.col[3]);
    _mm_stream_ps(ptr, t.col[0]);
    _mm_stream_ps(ptr + 4, t.col[1]);
    _mm_stream_ps(ptr + 8, t.col[2]);
    _mm_stream_ps(ptr + 12, t.col[3]);
}

//------------------------------------------------------------------------------
/**
    Pseudo constructor. This is syntactic sugar for the setzero_sse()
    function.
*/
inline float4_sse
_float4_sse()
{
    return _mm_setzero_ps();
}

//------------------------------------------------------------------------------
/**
    Pseudo constructor. This is syntactic sugar for the set_sse(s) function.
*/
inline float4_sse
_float4_sse(float s)
{
    return _mm_set1_ps(s);
}

//------------------------------------------------------------------------------
/**
    Pseudo constructor. This is syntactic sugar for the set_sse(x,y,z,w)
    functions.
*/
inline float4_sse
_float4_sse(float x, float y, float z, float w)
{
    return _mm_set_ps(w, z, y, x);
}

//------------------------------------------------------------------------------
/**
    Pseudo constructor for identity matrix. This is syntactic sugar for the 
    setidentity_sse() function.
*/
inline float4x4_sse
_float4x4_sse()
{
    float4x4_sse m;
    m.col[0] = _mm_set_ps(0.0f, 0.0f, 0.0f, 1.0f);
    m.col[1] = _mm_set_ps(0.0f, 0.0f, 1.0f, 0.0f);
    m.col[2] = _mm_set_ps(0.0f, 1.0f, 0.0f, 0.0f);
    m.col[3] = _mm_set_ps(1.0f, 0.0f, 0.0f, 0.0f);
    return m;
}

//------------------------------------------------------------------------------
/**
    Set all components of a vector to zero.
*/
inline float4_sse
setzero_sse()
{
    return _mm_setzero_ps();
}

//------------------------------------------------------------------------------
/**
    Set all components of a vector to the same value.
*/
inline float4_sse
set_sse(float s)
{
    return _mm_set1_ps(s);
}

//------------------------------------------------------------------------------
/**
    Set components of a vector.
*/
inline float4_sse
set_sse(float x, float y, float z, float w)
{
    return _mm_set_ps(w, z, y, x);
}

//------------------------------------------------------------------------------
/**
    Set x component of a vector.
*/
inline float4_sse
setx_sse(float4_arg_sse in, float x)
{
    return _mm_or_ps(_mm_set_ps(0.0f, 0.0f, 0.0f, x), _mm_and_ps(in, clearx_mask_sse));
}

//------------------------------------------------------------------------------
/**
    Set y component of a vector.
*/
inline float4_sse
sety_sse(float4_arg_sse in, float y)
{
    return _mm_or_ps(_mm_set_ps(0.0f, 0.0f, y, 0.0f), _mm_and_ps(in, cleary_mask_sse));
}

//------------------------------------------------------------------------------
/**
    Set z component of a vector.
*/
inline float4_sse
setz_sse(float4_arg_sse in, float z)
{
    return _mm_or_ps(_mm_set_ps(0.0f, z, 0.0f, 0.0f), _mm_and_ps(in, clearz_mask_sse));
}

//------------------------------------------------------------------------------
/**
    Set w component of a vector.
*/
inline float4_sse
setw_sse(float4_arg_sse in, float w)
{
    return _mm_or_ps(_mm_set_ps(w, 0.0f, 0.0f, 0.0f), _mm_and_ps(in, clearw_mask_sse));
}

//------------------------------------------------------------------------------
/**
    Set matrix to identity.
*/
inline float4x4_sse
setidentity_sse()
{
    float4x4_sse m;
    m.col[0] = _mm_set_ps(0.0f, 0.0f, 0.0f, 1.0f);
    m.col[1] = _mm_set_ps(0.0f, 0.0f, 1.0f, 0.0f);
    m.col[2] = _mm_set_ps(0.0f, 1.0f, 0.0f, 0.0f);
    m.col[3] = _mm_set_ps(1.0f, 0.0f, 0.0f, 0.0f);
    return m;
}

//------------------------------------------------------------------------------
/**
    Set matrix column 0.
*/
inline void
setcol0_sse(float4x4_ref_sse m, float4_arg_sse col0)
{
    m.col[0] = col0;
}

//------------------------------------------------------------------------------
/**
    Set matrix column 1.
*/
inline void
setcol1_sse(float4x4_ref_sse m, float4_arg_sse col1)
{
    m.col[1] = col1;
}

//------------------------------------------------------------------------------
/**
    Set matrix column 2.
*/
inline void
setcol2_sse(float4x4_ref_sse m, float4_arg_sse col2)
{
    m.col[2] = col2;
}

//------------------------------------------------------------------------------
/**
    Set matrix column 3.
*/
inline void
setcol3_sse(float4x4_ref_sse m, float4_arg_sse col3)
{
    m.col[3] = col3;
}

//------------------------------------------------------------------------------
/**
    Set matrix row 0.
*/
inline void
setrow0_sse(float4x4_ref_sse m, float4_arg_sse row0)
{
    _MM_TRANSPOSE4_PS(m.col[0], m.col[1], m.col[2], m.col[3]);
    m.col[0] = row0;
    _MM_TRANSPOSE4_PS(m.col[0], m.col[1], m.col[2], m.col[3]);
}

//------------------------------------------------------------------------------
/**
    Set matrix row 1.
*/
inline void
setrow1_sse(float4x4_ref_sse m, float4_arg_sse row1)
{
    _MM_TRANSPOSE4_PS(m.col[0], m.col[1], m.col[2], m.col[3]);
    m.col[1] = row1;
    _MM_TRANSPOSE4_PS(m.col[0], m.col[1], m.col[2], m.col[3]);
}

//------------------------------------------------------------------------------
/**
    Set matrix row 2.
*/
inline void
setrow2_sse(float4x4_ref_sse m, float4_arg_sse row2)
{
    _MM_TRANSPOSE4_PS(m.col[0], m.col[1], m.col[2], m.col[3]);
    m.col[2] = row2;
    _MM_TRANSPOSE4_PS(m.col[0], m.col[1], m.col[2], m.col[3]);
}

//------------------------------------------------------------------------------
/**
    Set matrix row 3.
*/
inline void
setrow3_sse(float4x4_ref_sse m, float4_arg_sse row3)
{
    _MM_TRANSPOSE4_PS(m.col[0], m.col[1], m.col[2], m.col[3]);
    m.col[3] = row3;
    _MM_TRANSPOSE4_PS(m.col[0], m.col[1], m.col[2], m.col[3]);
}

//------------------------------------------------------------------------------
/**
    Get x component of vector.
*/
inline float
getx_sse(float4_arg_sse val)
{
    return val.m128_f32[0];
}

//------------------------------------------------------------------------------
/**
    Get y component of vector.
*/
inline float
gety_sse(float4_arg_sse val)
{
    return val.m128_f32[1];
}

//------------------------------------------------------------------------------
/**
    Get z component of vector.
*/
inline float
getz_sse(float4_arg_sse val)
{
    return val.m128_f32[2];
}

//------------------------------------------------------------------------------
/**
    Get w component of vector.
*/
inline float
getw_sse(float4_arg_sse val)
{
    return val.m128_f32[3];
}

//------------------------------------------------------------------------------
/**
    Get column 0 component of 4x4 matrix.
*/
inline float4_sse
getcol0_sse(float4x4_arg_sse val)
{
    return val.col[0];
}

//------------------------------------------------------------------------------
/**
    Get column 1 component of 4x4 matrix.
*/
inline float4_sse
getcol1_sse(float4x4_arg_sse val)
{
    return val.col[1];
}

//------------------------------------------------------------------------------
/**
    Get column 2 component of 4x4 matrix.
*/
inline float4_sse
getcol2_sse(float4x4_arg_sse val)
{
    return val.col[2];
}

//------------------------------------------------------------------------------
/**
    Get column 3 component of 4x4 matrix.
*/
inline float4_sse
getcol3_sse(float4x4_arg_sse val)
{
    return val.col[3];
}

//------------------------------------------------------------------------------
/**
    Get row 0 component of 4x4 matrix.
*/
inline float4_sse
getrow0_sse(float4x4_arg_sse val)
{
    float4x4_sse m = val;
    _MM_TRANSPOSE4_PS(m.col[0], m.col[1], m.col[2], m.col[3]);
    return m.col[0];
}

//------------------------------------------------------------------------------
/**
    Get row 1 component of 4x4 matrix.
*/
inline float4_sse
getrow1_sse(float4x4_arg_sse val)
{
    float4x4_sse m = val;
    _MM_TRANSPOSE4_PS(m.col[0], m.col[1], m.col[2], m.col[3]);
    return m.col[1];
}

//------------------------------------------------------------------------------
/**
    Get row 2 component of 4x4 matrix.
*/
inline float4_sse
getrow2_sse(float4x4_arg_sse val)
{
    float4x4_sse m = val;
    _MM_TRANSPOSE4_PS(m.col[0], m.col[1], m.col[2], m.col[3]);
    return m.col[2];
}

//------------------------------------------------------------------------------
/**
    Get row 3 component of 4x4 matrix.
*/
inline float4_sse
getrow3_sse(float4x4_arg_sse val)
{
    float4x4_sse m = val;
    _MM_TRANSPOSE4_PS(m.col[0], m.col[1], m.col[2], m.col[3]);
    return m.col[3];
}

//------------------------------------------------------------------------------
/**
    Component wise add.
*/
inline float4_sse
operator+(float4_arg_sse v0, float4_arg_sse v1)
{
    return _mm_add_ps(v0, v1);
}

//------------------------------------------------------------------------------
/**
    Add scalar to all vector components.
*/
inline float4_sse
operator+(float4_arg_sse v, float s)
{
    return _mm_add_ps(v, _mm_set1_ps(s));
}

//------------------------------------------------------------------------------
/**
    Add scalar to all vector components.
*/
inline float4_sse
operator+(float s, float4_arg_sse v)
{
    return _mm_add_ps(_mm_set1_ps(s), v);
}

//------------------------------------------------------------------------------
/**
    Component wise subtract v1 from v0.
*/
inline float4_sse
operator-(float4_arg_sse v0, float4_arg_sse v1)
{
    return _mm_sub_ps(v0, v1);
}

//------------------------------------------------------------------------------
/**
    Subtract scalar from all components of v.
*/
inline float4_sse
operator-(float4_arg_sse v, float s)
{
    return _mm_sub_ps(v, _mm_set1_ps(s));
}

//------------------------------------------------------------------------------
/**
    Component wise multiply v0 and v1.
*/
inline float4_sse
operator*(float4_arg_sse v0, float4_arg_sse v1)
{
    return _mm_mul_ps(v0, v1);
}

//------------------------------------------------------------------------------
/**
    Component wise multiply with scalar.
*/
inline float4_sse
operator*(float4_arg_sse v, float s)
{
    return _mm_mul_ps(v, _mm_set1_ps(s));
}

//------------------------------------------------------------------------------
/**
    Component wise multiply with scalar.
*/
inline float4_sse
operator*(float s, float4_arg_sse v)
{
    return _mm_mul_ps(_mm_set1_ps(s), v);
}

//------------------------------------------------------------------------------
/**
    Component wise divide.
*/
inline float4_sse
operator/(float4_arg_sse v0, float4_arg_sse v1)
{
    return _mm_div_ps(v0, v1);
}

//------------------------------------------------------------------------------
/**
    Component wise divide by scalar.
*/
inline float4_sse
operator/(float4_arg_sse v0, float s)
{
    return _mm_div_ps(v0, _mm_set1_ps(s));
}

//------------------------------------------------------------------------------
/**
    Equality operator (slow!).
*/
inline bool
operator==(float4_arg_sse v0, float4_arg_sse v1)
{
    float4_sse res = _mm_cmpeq_ps(v0, v1);
    return (res.m128_f32[0] != 0) &&
           (res.m128_f32[1] != 0) &&
           (res.m128_f32[2] != 0) &&
           (res.m128_f32[3] != 0);
}

//------------------------------------------------------------------------------
/**
    Equality check with tolerance (slow!).
*/
inline bool
isequal_sse(float4_arg_sse v0, float4_arg_sse v1, float tolerance)
{
    float4_sse res = _mm_sub_ps(v1, v0);
    return ((res.m128_f32[0] > -tolerance) && (res.m128_f32[0] < tolerance) &&
            (res.m128_f32[1] > -tolerance) && (res.m128_f32[1] < tolerance) &&
            (res.m128_f32[2] > -tolerance) && (res.m128_f32[2] < tolerance) &&
            (res.m128_f32[3] > -tolerance) && (res.m128_f32[3] < tolerance));
}

//------------------------------------------------------------------------------
/**
    Compute minimum of 2 scalars.
*/
inline float
min_sse(float a, float b)
{
    return (a < b) ? a : b;
}

//------------------------------------------------------------------------------
/**
    Compute component-wise minimum of 2 vectors.
*/
inline float4_sse
min_sse(float4_arg_sse a, float4_arg_sse b)
{
    return _mm_min_ps(a, b);
}

//------------------------------------------------------------------------------
/**
    Compute component-wise maximum of 2 scalars.
*/
inline float
max_sse(float a, float b)
{
    return (a > b) ? a : b;
}

//------------------------------------------------------------------------------
/**
    Compute component-wise maximum of 2 vectors.
*/
inline float4_sse
max_sse(float4_arg_sse a, float4_arg_sse b)
{
    return _mm_max_ps(a, b);
}

//------------------------------------------------------------------------------
/**
    Compute reciprocal (1 / a) of scalar.
*/
inline float
rcp_sse(float a)
{
    return 1.0f / a;
}

//------------------------------------------------------------------------------
/**
    Compute component-wise reciprocal (1 / a) of vector.
*/
inline float4_sse
rcp_sse(float4_arg_sse a)
{
    return _mm_rcp_ps(a);
}

//------------------------------------------------------------------------------
/**
    Compute square root of scalar.
*/
inline float
sqrt_sse(float a)
{
    return sqrtf(a);
}

//------------------------------------------------------------------------------
/**
    Component-wise square root of vector.
*/
inline float4_sse
sqrt_sse(float4_arg_sse a)
{
    return _mm_sqrt_ps(a);
}

//------------------------------------------------------------------------------
/**
    Compute reciprocal square root of scalar.
*/
inline float
rsqrt_sse(float a)
{
    return 1.0f / sqrtf(a);
}

//------------------------------------------------------------------------------
/**
    Component-wise reciprocal square root of vector.
*/
inline float4_sse
rsqrt_sse(float4_arg_sse a)
{
    return _mm_rsqrt_ps(a);
}

//------------------------------------------------------------------------------
/**
    Return the horizontal sum of vector componts (r = x + y + z + w).
*/
inline float
hsum_sse(float4_arg_sse a)
{
    float4_sse r = _mm_add_ss(a,_mm_add_ss(_mm_shuffle_ps(a,a,1),_mm_add_ss(_mm_shuffle_ps(a,a,2),_mm_shuffle_ps(a,a,3))));
    return r.m128_f32[0];
}

//------------------------------------------------------------------------------
/**
    Return absolute value of scalar a.
*/
inline float
abs_sse(float a)
{
    return fabs(a);
}

//------------------------------------------------------------------------------
/**
    Return component-wise absolute values of x.
*/
inline float4_sse
abs_sse(float4_arg_sse x)
{
    float4_sse mask = _mm_cmplt_ps(x, zerovec_sse);
    float4_sse a = _mm_mul_ps(x, _mm_and_ps(mask, minusonevec_sse));
    float4_sse b = _mm_andnot_ps(mask, x);
    return _mm_or_ps(a, b);
}

//------------------------------------------------------------------------------
/**
    Return arccosine of scalar a.
*/
inline float
acos_sse(float a)
{
    return acosf(a);
}

//------------------------------------------------------------------------------
/**
    Return component-wise arccosines of vector a.
*/
inline float4_sse
acos_sse(float4_arg_sse a)
{
    return _float4_sse(acosf(a.m128_f32[0]),
                       acosf(a.m128_f32[1]),
                       acosf(a.m128_f32[2]),
                       acosf(a.m128_f32[3]));
}

//------------------------------------------------------------------------------
/**
    Return arcsine of scalar a.
*/
inline float
asin_sse(float a)
{
    return asinf(a);
}

//------------------------------------------------------------------------------
/**
    Return component-wise arcsines of vector a.
*/
inline float4_sse
asin_sse(float4_arg_sse a)
{
    return _float4_sse(asinf(a.m128_f32[0]),
                       asinf(a.m128_f32[1]),
                       asinf(a.m128_f32[2]),
                       asinf(a.m128_f32[3]));
}

//------------------------------------------------------------------------------
/**
    Return arctangent of scalar a.
*/
inline float
atan_sse(float a)
{
    return atanf(a);
}

//------------------------------------------------------------------------------
/**
    Return component-wise arctangent of vector a.
*/
inline float4_sse
atan_sse(float4_arg_sse a)
{
    return _float4_sse(atanf(a.m128_f32[0]),
                       atanf(a.m128_f32[1]),
                       atanf(a.m128_f32[2]),
                       atanf(a.m128_f32[3]));
}

//------------------------------------------------------------------------------
/**
    Return arctangent of x/y.
*/
inline float
atan2_sse(float x, float y)
{
    return atan2f(x, y);
}

//------------------------------------------------------------------------------
/**
    Return component-wise arctangent of x/y.
*/
inline float4_sse
atan2_sse(float4_arg_sse x, float4_arg_sse y)
{
    return _float4_sse(atan2f(x.m128_f32[0], y.m128_f32[0]),
                       atan2f(x.m128_f32[1], y.m128_f32[1]),
                       atan2f(x.m128_f32[2], y.m128_f32[2]),
                       atan2f(x.m128_f32[3], y.m128_f32[3]));
}

//------------------------------------------------------------------------------
/**
    Return the ceil of scalar x.
*/
inline float
ceil_sse(float x)
{
    return ceilf(x);
}

//------------------------------------------------------------------------------
/**
    Return the component-wise ceils of vector x.
*/
inline float4_sse
ceil_sse(float4_arg_sse x)
{
    return _float4_sse(ceilf(x.m128_f32[0]),
                       ceilf(x.m128_f32[1]),
                       ceilf(x.m128_f32[2]),
                       ceilf(x.m128_f32[3]));
}

//------------------------------------------------------------------------------
/**
    Clamp scalar x against minimum and maximum value.
*/
inline float
clamp_sse(float x, float min, float max)
{
    if (x < min) return min;
    else if (x > max) return max;
    else return x;
}

//------------------------------------------------------------------------------
/**
    Clamp components of vector x agains min and max components.
*/
inline float4_sse
clamp_sse(float4_arg_sse x, float4_arg_sse min, float4_arg_sse max)
{
    float4_sse minMask = _mm_cmplt_ps(x, min);
    float4_sse maxMask = _mm_cmpgt_ps(x, max);
    float4_sse res = x;
    res = _mm_or_ps(_mm_andnot_ps(minMask, res), _mm_and_ps(minMask, min));
    res = _mm_or_ps(_mm_andnot_ps(maxMask, res), _mm_and_ps(maxMask, max));
}

//------------------------------------------------------------------------------
/**
    Compute cosine of scalar x.
*/
inline float
cos_sse(float x)
{
    return cosf(x);
}

//------------------------------------------------------------------------------
/**
    Compute component-wise cosine of vector x.
*/
inline float4_sse
cos_sse(float4_arg_sse x)
{
    return _float4_sse(cosf(x.m128_f32[0]),
                       cosf(x.m128_f32[1]),
                       cosf(x.m128_f32[2]),
                       cosf(x.m128_f32[3]));
}

//------------------------------------------------------------------------------
/**
    Compute hyperbolic cosine of scalar x.
*/
inline float
cosh_sse(float x)
{
    return coshf(x);
}

//------------------------------------------------------------------------------
/**
    Compute component-wise hyperbolic cosine of vector x.
*/
inline float4_sse
cosh_sse(float4_arg_sse x)
{
    return _float4_sse(coshf(x.m128_f32[0]),
                       coshf(x.m128_f32[1]),
                       coshf(x.m128_f32[2]),
                       coshf(x.m128_f32[3]));
}

//------------------------------------------------------------------------------
/**
    Compute the cross product of 2 vectors.
*/
inline float4_sse
cross_sse(float4_arg_sse x, float4_arg_sse y)
{
    // x = v0.y * v1.z - v0.z * v1.y
    // y = v0.z * v1.x - v0.x * v1.z
    // z = v0.x * v1.y - v0.y * v1.x
    //
    // a = v0.y | v0.z | v0.x | xxx
    // b = v1.z | v1.x | v1.y | xxx
    // c = v0.z | v0.x | v0.y | xxx
    // d = v1.y | v1.z | v1.x | xxx
    float4_sse a = _mm_shuffle_ps(x, x, _MM_SHUFFLE(3, 0, 2, 1));
    float4_sse b = _mm_shuffle_ps(y, y, _MM_SHUFFLE(3, 1, 0, 2));
    float4_sse c = _mm_shuffle_ps(x, x, _MM_SHUFFLE(3, 1, 0, 2));
    float4_sse d = _mm_shuffle_ps(y, y, _MM_SHUFFLE(3, 0, 2, 1));    
    return _mm_sub_ps(_mm_mul_ps(a, b), _mm_mul_ps(c, d));
}

//------------------------------------------------------------------------------
/**
    Convert scalar from radians to degrees.
*/
inline float
degrees_sse(float x)
{
    return (x * 180.0f) / pi_sse;
}

//------------------------------------------------------------------------------
/**
    Component-wise convert vector from radians to degrees.
*/
inline float4_sse
degrees_sse(float4_arg_sse x)
{
    return _mm_mul_ps(x, rad2degvec_sse);
}

//------------------------------------------------------------------------------
/**
    Convert scalar from degrees to radians.
*/
inline float
radians_sse(float x)
{
    return (x * pi_sse) / 180.0f;
}

//------------------------------------------------------------------------------
/**
    Component-wise convert vector from degrees to radians.
*/
inline float4_sse
radians_sse(float4_arg_sse x)
{
    return _mm_mul_ps(x, deg2radvec_sse);
}

//------------------------------------------------------------------------------
/**
    Compute the determinant of matrix m.
*/
inline float
determinant_sse(float4x4_arg_sse /*m*/)
{
    n_error("IMPLEMENT ME!");
}

//------------------------------------------------------------------------------
/**
    Compute the distance between 2 points x and y.
*/
inline float
distance_sse(float4_arg_sse x, float4_arg_sse y)
{
    float4_sse d = _mm_sub_ps(y, x);
    float4_sse v = _mm_mul_ps(d, d);
    float4_sse lsq = _mm_add_ss(v,_mm_add_ss(_mm_shuffle_ps(v,v,1),_mm_add_ss(_mm_shuffle_ps(v,v,2),_mm_shuffle_ps(v,v,3))));
    float4_sse l = _mm_sqrt_ss(lsq);
    return l.m128_f32[0];
}

//------------------------------------------------------------------------------
/**
    Compute the squared between 2 points x and y.
*/
inline float
distancesquared_sse(float4_arg_sse x, float4_arg_sse y)
{
    float4_sse d = _mm_sub_ps(y, x);
    float4_sse v = _mm_mul_ps(d, d);
    float4_sse lsq = _mm_add_ss(v,_mm_add_ss(_mm_shuffle_ps(v,v,1),_mm_add_ss(_mm_shuffle_ps(v,v,2),_mm_shuffle_ps(v,v,3))));
    return lsq.m128_f32[0];
}

//------------------------------------------------------------------------------
/**
    Compute the dot product between 2 vectors.
*/
inline float
dot_sse(float4_arg_sse x, float4_arg_sse y)
{
    float4_sse v = _mm_mul_ps(x, y);
    float4_sse d = _mm_add_ss(v,_mm_add_ss(_mm_shuffle_ps(v,v,1),_mm_add_ss(_mm_shuffle_ps(v,v,2),_mm_shuffle_ps(v,v,3))));
    return d.m128_f32[0];
}

//------------------------------------------------------------------------------
/**
    Compute base-e exponential (e^x) of scalar x.
*/
inline float
exp_sse(float x)
{
    return expf(x);
}

//------------------------------------------------------------------------------
/**
    Compute component-wise base-e exponential (e^x) of vector x.
*/
inline float4_sse
exp_sse(float4_arg_sse x)
{
    return _float4_sse(expf(x.m128_f32[0]),
                       expf(x.m128_f32[1]),
                       expf(x.m128_f32[2]),
                       expf(x.m128_f32[3]));
}

//------------------------------------------------------------------------------
/**
    Compute floor of scalar x.
*/
inline float
floor_sse(float x)
{
    return floorf(x);
}

//------------------------------------------------------------------------------
/**
    Compute component-wise floor of vector x.
*/
inline float4_sse
floor_sse(float4_sse x)
{
    return _float4_sse(floorf(x.m128_f32[0]),
                       floorf(x.m128_f32[1]),
                       floorf(x.m128_f32[2]),
                       floorf(x.m128_f32[3]));
}

//------------------------------------------------------------------------------
/**
    Compute floating point remainder of scalars x/y.
*/
inline float
fmod_sse(float x, float y)
{
    return fmodf(x, y);
}

//------------------------------------------------------------------------------
/**
    Compute component-wise floating point remainders of vectors x/y.
*/
inline float4_sse
fmod_sse(float4_sse x, float4_sse y)
{
    return _float4_sse(fmodf(x.m128_f32[0], y.m128_f32[0]),
                       fmodf(x.m128_f32[1], y.m128_f32[1]),
                       fmodf(x.m128_f32[2], y.m128_f32[2]),
                       fmodf(x.m128_f32[3], y.m128_f32[3]));
}

//------------------------------------------------------------------------------
/**
    Compute the fractional part of scalar x.
*/
inline float
frac_sse(float x)
{
    float intPart;
    return modff(x, &intPart);
}

//------------------------------------------------------------------------------
/**
    Compute component-wise fractinal part of vector x.
*/
inline float4_sse
frac_sse(float4_arg_sse x)
{
    float tmp;
    return _float4_sse(modf(x.m128_f32[0], &tmp),
                       modf(x.m128_f32[1], &tmp),
                       modf(x.m128_f32[2], &tmp),
                       modf(x.m128_f32[3], &tmp));
}

//------------------------------------------------------------------------------
/**
    Compute the length of vector x.
*/
inline float
length_sse(float4_arg_sse x)
{
    float4_sse v = _mm_mul_ps(x, x);
    float4_sse lsq = _mm_add_ss(v,_mm_add_ss(_mm_shuffle_ps(v,v,1),_mm_add_ss(_mm_shuffle_ps(v,v,2),_mm_shuffle_ps(v,v,3))));
    float4_sse l = _mm_sqrt_ss(lsq);
    return l.m128_f32[0];
}

//------------------------------------------------------------------------------
/**
    Compute the squared length of vector x.
*/
inline float
lengthsquared_sse(float4_arg_sse x)
{
    float4_sse v = _mm_mul_ps(x, x);
    float4_sse lsq = _mm_add_ss(v,_mm_add_ss(_mm_shuffle_ps(v,v,1),_mm_add_ss(_mm_shuffle_ps(v,v,2),_mm_shuffle_ps(v,v,3))));
    return lsq.m128_f32[0];
}

//------------------------------------------------------------------------------
/**
    Returns x + s * (y - x) where x, y, s are scalars.
*/
inline float
lerp_sse(float x, float y, float s)
{
    return x + s * (y - x);
}

//------------------------------------------------------------------------------
/**
    Returns x + s * (y - x) where x, y, s are vectors.
*/
inline float4_sse
lerp_sse(float4_arg_sse x, float4_arg_sse y, float4_arg_sse s)
{
    return _mm_add_ps(x, _mm_mul_ps(s, _mm_sub_ps(y, x)));
}

//------------------------------------------------------------------------------
/**
    Returns x + s * (y - x) where x, y are vectors and s is a scalar.
*/
inline float4_sse
lerp_sse(float4_arg_sse x, float4_arg_sse y, float s)
{
    return _mm_add_ps(x, _mm_mul_ps(_mm_set1_ps(s), _mm_sub_ps(y, x)));
}

//------------------------------------------------------------------------------
/**
    Return base-e logarithm of scalar x.
*/
inline float
log_sse(float x)
{
    return logf(x);
}

//------------------------------------------------------------------------------
/**
    Return component-wise base-e logarithm.
*/
inline float4_sse
log_sse(float4_arg_sse x)
{
    return _float4_sse(logf(x.m128_f32[0]),
                       logf(x.m128_f32[1]),
                       logf(x.m128_f32[2]),
                       logf(x.m128_f32[3]));
}

//------------------------------------------------------------------------------
/**
    Return base-10 logarithm of scalar x.
*/
inline float
log10_sse(float x)
{
    return log10f(x);
}

//------------------------------------------------------------------------------
/**
    Return component-wise base-10 logarithm of vector x.
*/
inline float4_sse
log10_sse(float4_arg_sse x)
{
    return _float4_sse(log10f(x.m128_f32[0]),
                       log10f(x.m128_f32[1]),
                       log10f(x.m128_f32[2]),
                       log10f(x.m128_f32[3]));
}

//------------------------------------------------------------------------------
/**
    Return fractional and integer part of scalar x.
*/
inline float
modf_sse(float x, float& out_ip)
{
    return modff(x, &out_ip);
}

//------------------------------------------------------------------------------
/**
    Return fractional and integer part of vector x.
*/
inline float4_sse
modf_sse(float4_arg_sse x, float4_ref_sse out_ip)
{
    return _float4_sse(modff(x.m128_f32[0], &(out_ip.m128_f32[0])),
                       modff(x.m128_f32[1], &(out_ip.m128_f32[1])),
                       modff(x.m128_f32[2], &(out_ip.m128_f32[2])),
                       modff(x.m128_f32[3], &(out_ip.m128_f32[3])));
}

//------------------------------------------------------------------------------
/**
    Multiply 2 vectors. This is identical with the dot product.
*/
inline float
mul_sse(float4_arg_sse v0, float4_arg_sse v1)
{
    float4_sse v = _mm_mul_ps(v0, v1);
    float4_sse d = _mm_add_ss(v,_mm_add_ss(_mm_shuffle_ps(v,v,1),_mm_add_ss(_mm_shuffle_ps(v,v,2),_mm_shuffle_ps(v,v,3))));
    return d.m128_f32[0];
}

//------------------------------------------------------------------------------
/**
    Multiply a scalar and a vector.
*/
inline float4_sse
mul_sse(float s, float4_arg_sse v0)
{
    return _mm_mul_ps(_mm_set1_ps(s), v0);
}

//------------------------------------------------------------------------------
/**
    Multiply a vector and a scalar.
*/
inline float4_sse
mul_sse(float4_arg_sse v0, float s)
{
    return _mm_mul_ps(v0, _mm_set1_ps(s));
}

//------------------------------------------------------------------------------
/**
    Multiply a vector and a matrix.
*/
inline float4_sse
mul_sse(float4_arg_sse v0, float4x4_arg_sse m0)
{
    float4_sse r0 = _mm_mul_ps(v0, m0.col[0]);
    float4_sse r1 = _mm_mul_ps(v0, m0.col[1]);
    float4_sse r2 = _mm_mul_ps(v0, m0.col[2]);
    float4_sse r3 = _mm_mul_ps(v0, m0.col[3]);
    _MM_TRANSPOSE4_PS(r0, r1, r2, r3);
    return _mm_add_ps(r0, _mm_add_ps(r1, _mm_add_ps(r2, r3)));
}

//------------------------------------------------------------------------------
#endif
