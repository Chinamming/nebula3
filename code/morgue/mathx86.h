#pragma once
#ifndef MATH_MATHX86_H
#define MATH_MATHX86_H
//------------------------------------------------------------------------------
/**
    @file math/mathx86.h
    
    Implements the Nebula3 lowlevel math library for the generic X86
    FPU. The goals of the new Nebula3 math lib are:
    
    * enable portability without compromising performance
    * look like HLSL code
    * speed is more important then anything else
    
    The consequences of these goals are:

    * The vector and matrix classes do not follow the C++ text-book-style
      with constructors, operands and methods. Instead, these classes
      are just simple typedefs, and all functionality is provided in C-style
      functions. On platforms with native 4-component-vector datatypes
      this enables better compiler optimizations.
    * HLSL shows that C-style math code is at least as readable (if not
      more readable) then C++ style code with heavy operator overloading.
    * Function parameters are handed by-value or by-ref depending on the
      target platform (on platforms with native vector datatypes, by-
      value is often faster then by-ref.
    * Math datatypes are generally not initialized in their default
      constructors. Profiling classic C++ math code often shows that
      a serious amount of time is spent in the default constructors.
        
    (C) 2006 Radon Labs GmbH
*/
#include "core/types.h"

namespace Math
{

// typedef of vector datatypes
typedef float[2] float2;
typedef float[3] float3;
typedef float[4] float4;

// typedef of matrix datatypes
typedef float[4] float2x2
typedef float[9] float3x3
typedef float[16] float4x4

// typedefs of vector function arguments
typedef const float float_arg
typedef const float2& float2_arg
typedef const float3& float3_arg
typedef const float4& float4_arg
typedef const float2x2& float2x2_arg
typedef const float3x3& float3x3_arg
typedef const float4x4& float4x3_arg

// typedefs of vector references
typedef float& float_ref
typedef float2& float2_ref
typedef float3& float3_ref
typedef float4& float4_ref
typedef float2x2& float2x2_ref
typedef float3x3& float3x3_ref
typedef float4x4& float4x3_ref

// component definitions
enum vector_comp
{
    x = 0,
    y = 1,
    z = 2,
    w = 3
};

enum matrix_comp
{
    row0 = 0,
    row1 = 1,
    row2 = 2,
    row3 = 3,
};

// math functions

//--- load data from memory ---
extern void load_x86(float2_ref val, float2* ptr);
extern void load_x86(float3_ref val, float3* ptr);
extern void load_x86(float4_ref val, float4* ptr);
extern void load_x86(float2x2_ref val, float2x2* ptr);
extern void load_x86(float3x3_ref val, float3x3* ptr);
extern void load_x86(float4x3_ref val, float4x4* ptr);

//--- store data to memory ---
extern void store_x86(float2_arg val, float* ptr);
extern void store_x86(float3_arg val, float* ptr);
extern void store_x86(float4_arg val, float* ptr);
extern void store_x86(float2x2_arg val, float* ptr);
extern void store_x86(float3x3_arg val, float* ptr);
extern void store_x86(float4x4_arg val, float* ptr);

//--- create new element ---
extern float2 setup_x86(float x, float y);
extern float3 setup_x86(float x, float y, float z);
extern float4 setup_x86(float x, float y, float z, float w);

extern float2 setup_x86(float2* ptr);
extern float3 setup_x86(float3* ptr);
extern float4 setup_x86(float4* ptr);

extern float2x2 setup_x86(float2_arg row0, float2_arg row1);
extern float3x3 setup_x86(float3_arg row0, float3_arg row1, float3_arg row2);
extern float4x3 setup_x86(float4_arg row0, float4_arg row1, float4_arg row2, float4_arg row3);

extern float2x2 setup_x86(float2x2* ptr);
extern float3x3 setup_x86(float3x3* ptr);
extern float4x3 setup_x86(float4x4* ptr);

//--- set data  ---
extern void set_x86(float2_ref val, float x, float y);
extern void set_x86(float3_ref val, float x, float y, float z);
extern void set_x86(float4_ref val, float x, float y, float z, float w);
extern void set_x86(float2x2_ref val, float2_arg row0, float2_arg row1);
extern void set_x86(float3x3_ref val, float3_arg row0, float3_arg row1, float3_arg row2);
extern void set_x86(float4x4_ref val, float4_arg row0, float4_arg row1, float4_arg row2, float4_arg row3);

//--- set components ---
extern void set_x86(float2_ref vec, vector_comp comp, float val);
extern void set_x86(float3_ref vec, vector_comp comp, float val);
extern void set_x86(float4_ref vec, vector_comp comp, float val);
extern void set_x86(float2x2_ref m, matrix_comp row, float2 val);
extern void set_x86(float3x3_ref m, matrix_comp row, float3 val);
extern void set_x86(float4x4_ref m, matrix_comp_row, float4 val);
extern void set_x86(float2x2_ref m, matrix_comp row, vector_comp col, float val);
extern void set_x86(float3x3_ref m, matrix_comp row, vector_comp col, float val);
extern void set_x86(float4x4_ref m, matrix_comp row, vector_comp col, float val);

//--- get components ---
extern float get_x86(float2_arg vec, vector_comp comp);
extern float get_x86(float3_arg vec, vector_comp comp);
extern float get_x86(float3_arg vec, vector_comp comp);
extern float2_arg get_x86(float2x2_arg vec, matrix_comp row);
extern float3_arg get_x86(float2x2_arg vec, matrix_comp row);
extern float3_arg get_x86(float2x2_arg vec, matrix_comp row);

//--- component-wise absolute value ---
extern float abs_x86(float_arg val);
extern float2 abs_x86(float2_arg val);
extern float3 abs_x86(float3_arg val);
extern float4 abs_x86(float4_arg val);

//--- component-wise arccosine ---
extern float acos_x86(float_arg val);
extern float2 acos_x86(float2_arg val);
extern float3 acos_x86(float3_arg val);
extern float4 acos_x86(float4_arg val);

//--- test if all components are non-zero ---
extern bool all_x86(float_arg val);
extern bool all_x86(float2_arg val);
extern bool all_x86(float3_arg val);
extern bool all_x86(float4_arg val);

//--- test if any component is non-zero ---
extern bool any_x86(float_arg val);
extern bool any_x86(float2_arg val);
extern bool any_x86(float3_arg val);
extern bool any_x86(float4_arg val);

//--- component-wise arcsine ---
extern float asin_x86(float_arg val);
extern float2 asin_x86(float2_arg val);
extern float3 asin_x86(float3_arg val);
extern float4 asin_x86(float4_arg val);

//--- component-wise arctangent ---
extern float atan_x86(float_arg val);
extern float2 atan_x86(float2_arg val);
extern float3 atan_x86(float3_arg val);
extern float4 atan_x86(float4_arg val);

//--- component-wise arctangent of x/y ---
extern float atan2_x86(float_arg x, float_arg y);
extern float2 atan2_x86(float2_arg x, float2_arg y);
extern float3 atan2_x86(float3_arg x, float3_arg y);
extern float4 atan2_x86(float4_arg x, float4_arg y);

//--- component-wise ceil() ---
extern float ceil_x86(float_arg val);
extern float2 ceil_x86(float2_arg val);
extern float3 ceil_x86(float3_arg val);
extern float4 ceil_x86(float4_arg val);

//--- component-wise clamp to min/max ---
extern float clamp_x86(float_arg val, float_arg min, float_arg max);
extern float2 clamp_x86(float2_arg val, float2_arg min, float2_arg max);
extern float3 clamp_x86(float3_arg val, float3_arg min, float3_arg max);
extern float4 clamp_x86(float4_arg val, float4_arg min, float4_arg max);

//--- component-wise cosine ---
extern float cos_x86(float_arg val);
extern float2 cos_x86(float2_arg val);
extern float3 cos_x86(float3_arg val);
extern float4 cos_x86(float4_arg val);

//--- component-wise hyperbolic cosine ---
extern float cosh_x86(float_arg val);
extern float2 cosh_x86(float2_arg val);
extern float3 cosh_x86(float3_arg val);
extern float4 cosh_x86(float4_arg val);

//--- cross-product of 2 vectors ---
extern float2 cross_x86(float2_arg x, float2_arg y);
extern float3 cross_x86(float3_arg x, float3_arg y);
extern float4 cross_x86(float4_arg x, float4_arg y);

//--- component-wise convert from radians to degrees ---
extern float degrees_x86(float_arg val);
extern float2 degrees_x86(float2_arg val);
extern float3 degrees_x86(float3_arg val);
extern float4 degrees_x86(float4_arg val);

//--- returns the determinant of matrix ---
extern float determinant_x86(float2x2_arg m);
extern float determinant_x86(float3x3_arg m);
extern float determinant_x86(float4x4_arg m);

//--- distance between 2 points x and y ---
extern float distance_x86(float2_arg x, float2_arg y);
extern float distance_x86(float3_arg x, float3_arg y);
extern float distance_x86(float4_arg x, float4_arg y);

//--- dot product between 2 vector x and y ---
extern float dot_x86(float2_arg x, float2_arg y);
extern float dot_x86(float3_arg x, float3_arg y);
extern float dot_x86(float4_arg x, float4_arg y);

//--- component-wise base-e exponential (e^x) ---
extern float exp_x86(float_arg val);
extern float2 exp_x86(float2_arg val);
extern float3 exp_x86(float3_arg val);
extern float4 exp_x86(float4_arg val);

//--- component-wise base-2 exponential (2^x) ---
extern float exp2_x86(float_arg val);
extern float2 exp2_x86(float2_arg val);
extern float3 exp2_x86(float3_arg val);
extern float4 exp2_x86(float4_arg val);

//--- component-wise floor() ---
extern float floor_x86(float_arg val);
extern float2 floor_x86(float2_arg val);
extern float3 floor_x86(float3_arg val);
extern float4 floor_x86(float4_arg val);

//--- component-wise floating point remainder ---
extern float fmod_x86(float_arg x, float_arg y);
extern float2 fmod_x86(float2_arg x, float2_arg y);
extern float3 fmod_x86(float3_arg x, float3_arg y);
extern float4 fmod_x86(float4_arg x, float4_arg y);

//--- component-wise fractional part ---
extern float frac_x86(float_arg x, float_arg y);
extern float2 frac_x86(float2_arg x, float2_arg y);
extern float3 frac_x86(float3_arg x, float3_arg y);
extern float4 frac_x86(float4_arg x, float4_arg y);

//--- length of vector ---
extern float length_x86(float2_arg val);
extern float length_x86(float3_arg val);
extern float length_x86(float4_arg val);

//--- returns x + s * (y - x) ---
extern float lerp_x86(float_arg x, float_arg y, float_arg s);
extern float2 lerp_x86(float2_arg x, float2_arg y, float2_arg s);
extern float3 lerp_x86(float3_arg x, float3_arg y, float3_arg s);
extern float4 lerp_x86(float4_arg x, float4_arg y, float4_arg s);
extern float2 lerp_x86(float2_arg x, float2_arg y, float_arg s);
extern float3 lerp_x86(float3_arg x, float3_arg y, float_arg s);
extern float4 lerp_x86(float4_arg x, float4_arg y, float_arg s);

//--- component-wise base-e logarithm ---
extern float log_x86(float_arg val);
extern float2 log_x86(float2_arg val);
extern float3 log_x86(float3_arg val);
extern float4 log_x86(float4_arg val);

//--- component-wise base-2 logarithm ---
extern float log2_x86(float_arg val);
extern float2 log2_x86(float2_arg val);
extern float3 log2_x86(float3_arg val);
extern float4 log2_x86(float4_arg val);

//--- component-wise base-10 logarithm ---
extern float log10_x86(float_arg val);
extern float2 log10_x86(float2_arg val);
extern float3 log10_x86(float3_arg val);
extern float4 log10_x86(float4_arg val);

//--- component-wise max ---
extern float max_x86(float_arg val);
extern float2 max_x86(float2_arg val);
extern float3 max_x86(float3_arg val);
extern float4 max_x86(float4_arg val);

//--- component-wise min ---
extern float min_x86(float_arg val);
extern float2 min_x86(float2_arg val);
extern float3 min_x86(float3_arg val);
extern float4 min_x86(float4_arg val);

//--- component-wise split fractional and integer part ---
extern float modf_x86(float_arg val, float_ref out_ip);
extern float2 modf_x86(float2_arg val, float2_ref out_ip);
extern float3 modf_x86(float3_arg val, float3_ref out_ip);
extern float4 modf_x86(float4_arg val, float4_ref out_ip);

//--- matrix multiplications ---
extern float2 mul_x86(float_arg s, float2_arg v0);
extern float3 mul_x86(float_arg s, float3_arg v0);
extern float4 mul_x86(float_arg s, float4_arg v0);

extern float2x2 mul_x86(float_arg s, float2x2_arg m0);
extern float3x3 mul_x86(float_arg s, float3x3_arg m0);
extern float4x4 mul_x86(float_arg s, float4x4_arg m0);

extern float2 mul_x86(float2_arg v0, float_arg s);
extern float3 mul_x86(float3_arg v0, float_arg s);
extern float4 mul_x86(float4_arg v0, float_arg s);

extern float mul_x86(float2_arg v0, float2_arg v1);
extern float mul_x86(float3_arg v0, float3_arg v1);
extern float mul_x86(float4_arg v0, float4_arg v1);

extern float2 mul_x86(float2_arg v0, float2x2_arg m0);
extern float3 mul_x86(float3_arg v0, float3x3_arg m0);
extern float4 mul_x86(float4_arg v0, float4x4_arg m0);

extern float2x2 mul_x86(float2x2_arg m0, float_arg s);
extern float3x3 mul_x86(float3x3_arg m0, float_arg s);
extern float4x4 mul_x86(float4x4_arg m0, float_arg s);

extern float2 mul_x86(float2x2_arg m0, float2_arg v0);
extern float3 mul_x86(float3x3_arg m0, float3_arg v0);
extern float4 mul_x86(float4x4_arg m0, float4_arg v0);

extern float2x2 mul_x86(float2x2_arg m0, float2x2_arg m1);
extern float3x3 mul_x86(float3x3_arg m0, float3x3_arg m1);
extern float4x4 mul_x86(float4x4_arg m0, float4x4_arg m1);

//--- return normalized vector ---
extern float2 normalize_x86(float2_arg v);
extern float3 normalize_x86(float3_arg v);
extern float4 normalize_x86(float4_arg v);

//--- component-wise x^y ---
extern float pow_x86(float_arg x, float_arg y);
extern float2 pow_x86(float2_arg x, float2_arg y);
extern float3 pow_x86(float3_arg x, float3_arg y);
extern float4 pow_x86(float4_arg x, float4_arg y);

//--- component-wise convert degree to radians ---
extern float radians_x86(float_arg v);
extern float2 radians_x86(float2_arg v);
extern float3 radians_x86(float3_arg v);
extern float4 radians_x86(float4_arg v);

//--- compute reflection vector v = i - 2 * dot(i, n) * n ---
extern float2 reflect_x86(float2_arg i, float2_arg n);
extern float3 reflect_x86(float3_arg i, float3_arg n);
extern float4 reflect_x86(float4_arg i, float4_arg n);

//--- compute refraction vector ---
extern float2 refract_x86(float2_arg i, float2_arg n, float_arg ri);
extern float3 refract_x86(float3_arg i, float3_arg n, float_arg ri);
extern float4 refract_x86(float4_arg i, float4_arg n, float_arg ri);

//--- component wise round to nearest integer ---
extern float round_x86(float_arg v);
extern float2 round_x86(float2_arg v);
extern float3 round_x86(float3_arg v);
extern float4 round_x86(float4_arg v);

//--- component-wise compute 1 / sqrt(v) ---
extern float rsqrt_x86(float_arg v);
extern float2 rsqrt_x86(float2_arg v);
extern float3 rsqrt_x86(float3_arg v);
extern float4 rsqrt_x86(float4_arg v);

//--- component-wise clamp to (0,1) ---
extern float saturate_x86(float_arg v);
extern float2 saturate_x86(float2_arg v);
extern float3 saturate_x86(float3_arg v);
extern float4 saturate_x86(float4_arg v);

//--- component-wise compute sign ---
extern float sign_x86(float_arg v);
extern float2 sign_x86(float2_arg v);
extern float3 sign_x86(float3_arg v);
extern float4 sign_x86(float4_arg v);

//--- component-wise compute sine ---
extern float sin_x86(float_arg v);
extern float2 sin_x86(float2_arg v);
extern float3 sin_x86(float3_arg v);
extern float4 sin_x86(float4_arg v);

//--- component-wise compute sine and cosine ---
extern void sincos_x86(float_arg v, float_ref s, float_ref c);
extern void sincos_x86(float2_arg v, float2_ref s, float2_ref c);
extern void sincos_x86(float3_arg v, float3_ref s, float3_ref c);
extern void sincos_x86(float4_arg v, float4_ref s, float4_ref c);

//--- component-wise hyperbolic sine ---
extern float sinh_x86(float_arg v);
extern float2 sinh_x86(float2_arg v);
extern float3 sinh_x86(float3_arg v);
extern float4 sinh_x86(float4_arg v);

//--- compute smooth Hermite interpolation between 0 and 1 ---
extern float smoothstep_x86(float_arg min, float_arg max, float_arg x);
extern float2 smoothstep_x86(float2_arg min, float2_arg max, float2_arg x);
extern float3 smoothstep_x86(float3_arg min, float3_arg max, float3_arg x);
extern float4 smoothstep_x86(float4_arg min, float4_arg max, float4_arg x);

//--- component-wise sqrt ---
extern float sqrt_x86(float_arg v);
extern float2 sqrt_x86(float2_arg v);
extern float3 sqrt_x86(float3_arg v);
extern float4 sqrt_x86(float4_arg v);

//--- component-wise tangent ---
extern float tan_x86(float_arg v);
extern float2 tan_x86(float2_arg v);
extern float3 tan_x86(float3_arg v);
extern float4 tan_x86(float4_arg v);

//--- component-wise hyperbolic tangent ---
extern float tanh_x86(float_arg v);
extern float2 tanh_x86(float2_arg v);
extern float3 tanh_x86(float3_arg v);
extern float4 tanh_x86(float4_arg v);

//--- return transpose of input matrix ---
extern float2x2 transpose_x86(float2x2_arg m);
extern float3x3 transpose_x86(float3x3_arg m);
extern float4x4 transpose_x86(float4x4_arg m);

//--- return inverted matrix ---
extern float2x2 invert_x86(float2x2_arg m);
extern float3x3 invert_x86(float3x3_arg m);
extern float4x4 invert_x86(float4x4_arg m);

//--- return inverted matrix, assume it's an rotate/scale/translate matrix ---
extern float4x4 invertsimple_x86(float4x4_arg m);
}