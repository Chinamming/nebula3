//------------------------------------------------------------------------------
//  math_sse.cc
//  (C) 2006 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "math/math_sse.h"

//--- globals ---
const unsigned int allbits = 0xffffffff;
const float nan_sse = *(float*)&allbits;

const float4_sse clearx_mask_sse = _mm_set_ps(nan_sse, nan_sse, nan_sse, 0.0f);
const float4_sse cleary_mask_sse = _mm_set_ps(nan_sse, nan_sse, 0.0f, nan_sse);
const float4_sse clearz_mask_sse = _mm_set_ps(nan_sse, 0.0f, nan_sse, nan_sse);
const float4_sse clearw_mask_sse = _mm_set_ps(0.0f, nan_sse, nan_sse, nan_sse);
const float4_sse zerovec_sse = _mm_setzero_ps();
const float4_sse minusonevec_sse = _mm_set_ps(-1.0f, -1.0f, -1.0f, -1.0f);
const float4_sse deg2radvec_sse = _mm_set_ps(pi_sse / 180.0f, pi_sse / 180.0f, pi_sse / 180.0f, pi_sse / 180.0f);
const float4_sse rad2degvec_sse = _mm_set_ps(180.0f / pi_sse, 180.0f / pi_sse, 180.0f / pi_sse, 180.0f / pi_sse);
