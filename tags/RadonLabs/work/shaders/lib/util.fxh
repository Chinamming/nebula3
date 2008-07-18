#ifndef UTIL_FXH
#define UTIL_FXH
//------------------------------------------------------------------------------
//  util.fxh
//
//  Shader utility functions.
//
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "config.fxh"

//------------------------------------------------------------------------------
/**
    Sample a texel with manual bilinear filtering (for texture formats which
    don't support filtering).
    
    FIXME: on 360 we can actually do the same without knowing texSize, saves
    the coordinate computations!
*/
float4
SampleBilinear(sampler texSampler, float2 uv, float2 texSize)
{
#if USE_HARDWARE_SAMPLING
    return tex2D(texSampler, uv);
#else    
    float2 invTexSize = float2(1.0, 1.0) / texSize;
    float2 uv01 = float2(uv.x + invTexSize.x, uv.y);
    float2 uv10 = float2(uv.x, uv.y + invTexSize.y);
    float2 uv11 = uv + invTexSize;
    
    float4 c00 = tex2D(texSampler, uv);
    float4 c01 = tex2D(texSampler, uv01);
    float4 c10 = tex2D(texSampler, uv10);
    float4 c11 = tex2D(texSampler, uv11);

    float2 ratios = frac(uv * texSize);
    float4 c = lerp(lerp(c00, c01, ratios.x), lerp(c10, c11, ratios.x), ratios.y);
    return c;    
#endif    
}

//------------------------------------------------------------------------------
/**
    Encode 2 values in the range 0..1 into a 4-channel vector. Used
    for encoding PSSM-depth values into a 32-bit-rgba value.
*/
float4
Encode2(float2 inVals)
{
    return float4(inVals.x, frac(inVals.x * 256.0), inVals.y, frac(inVals.y * 256.0));
}

//------------------------------------------------------------------------------
/**
    Decode 2 values encoded by Encode2().
*/
float2
Decode2(float4 inVals)
{
    return float2(inVals.x + (inVals.y / 256.0), inVals.z + (inVals.w / 256.0));
}

//------------------------------------------------------------------------------
/**
    Encode PSSM depth values from the projection space depth (0..1)
*/
float4
EncodePSSMDepth(float depth)
{
    return float4(depth, depth*depth, 0.0, 0.0);
}

//------------------------------------------------------------------------------
/**
    Decode PSSM depth values from a shadow map texture.
*/
float2
DecodePSSMDepth(sampler texSampler, float2 uv, float2 texSize)
{
    return SampleBilinear(texSampler, uv, texSize).xy;
}

//------------------------------------------------------------------------------
/**
    Sample a surface normal from a DXT5nm-compressed texture, return 
    TangentSurfaceNormal.
*/
float3
SampleNormal(sampler bumpSampler, float2 uv)
{
    float3 n;
    n.xy = (tex2D(bumpSampler, uv).ag * 2.0) - 1.0;    
    n.z = sqrt(1.0 - dot(n.xy, n.xy));
    return n;
}

//------------------------------------------------------------------------------
/**
    Sample tangent surface normal from bump map and transform into 
    world space.
*/
float3
psWorldSpaceNormalFromBumpMap(sampler bumpMapSampler, float2 uv, float3 worldNormal, float3 worldTangent, float3 worldBinormal)
{
    float3x3 tangentToWorld = float3x3(worldTangent, worldBinormal, worldNormal);
    float3 tangentNormal = SampleNormal(bumpMapSampler, uv);
    return mul(tangentNormal, tangentToWorld);
}

//------------------------------------------------------------------------------
#endif
