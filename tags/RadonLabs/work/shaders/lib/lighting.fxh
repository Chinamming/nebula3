#ifndef LIGHTING_FXH
#define LIGHTING_FXH
//------------------------------------------------------------------------------
//  lighting.fxh
//
//  Lib functions for lighting.
//
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "util.fxh"
#include "lighting_shared.fxh"

const float lightShadowBias = 0.01;
const float lightVSMEpsilon = 0.0001;

const float halfLambertScale = 1.0f;
const float halfLambertBias  = 0.0f;

//------------------------------------------------------------------------------
/**
    Evaluate a global light.
*/
void
GlobalLight(in float4 worldPos,            // surface position in world space
            in float4 projPos,             // surface position in projection space
            in float3 worldNormal,         // surface normal in world space
            in float3 worldEyeVec,         // normalized eye vector in world space
            in float matSpecularPower,     // material specular power
            in out float4 diffuse,         // computed diffuse component
            in out float4 specular)        // computed specular component
{
    float shadowContrib = 1.0;
    if (globalLightCastShadows)
    {
        float4x4 lightProjMatrix;
        float4 uvOffsetAndScale;
        
        // choose the right view frustum split
        if (projPos.z < globalLightPSSMDistances[1])
        {
            lightProjMatrix = globalLightPSSMTransforms[0];
            uvOffsetAndScale = float4(0.0f, 0.0f, 0.25f, 1.0f);
//diffuse = float4(1.0f, 0.0f, 0.0f, 1.0f);
        }
        else if (projPos.z < globalLightPSSMDistances[2])
        {        
            lightProjMatrix = globalLightPSSMTransforms[1];
            uvOffsetAndScale = float4(0.25f, 0.0f, 0.25f, 1.0f);            
//diffuse = float4(1.0f, 1.0f, 0.0f, 1.0f);
        }
        else if (projPos.z < globalLightPSSMDistances[3])
        {
            lightProjMatrix = globalLightPSSMTransforms[2];
            uvOffsetAndScale = float4(0.5f, 0.0f, 0.25f, 1.0f);
//diffuse = float4(0.0f, 0.0f, 1.0f, 1.0f);
        }
        else
        {
            lightProjMatrix = globalLightPSSMTransforms[3];
            uvOffsetAndScale = float4(0.75f, 0.0f, 0.25f, 1.0f);
//diffuse = float4(0.0f, 1.0f, 1.0f, 1.0f);
        }

        // lookup shadow map        
        float4 projLightPos = mul(worldPos, lightProjMatrix);
        projLightPos.xy /= projLightPos.w;
        if (projLightPos.z > 0.0)
        {
            // compute shadow term
            float2 lightSpaceUv = (projLightPos.xy * float2(0.5, -0.5)) + 0.5f;
            float2 shadowMapUv = lightSpaceUv * uvOffsetAndScale.zw + uvOffsetAndScale.xy;

            // lookup VSM moments from shadow map
            // simulate linear sampling
            float2 vsmMoments = DecodePSSMDepth(PSSMShadowBufferSampler, shadowMapUv, lightShadowBufferSize);
            float distToLight = projLightPos.z - lightShadowBias;
            shadowContrib = distToLight <= vsmMoments.x;

            float E_x2 = vsmMoments.y;
            float Ex_2 = vsmMoments.x * vsmMoments.x;
            float variance = min(max(E_x2 - Ex_2, 0.0) + lightVSMEpsilon, 1.0);
            float m_d = (vsmMoments.x - distToLight);
            float p = variance / (variance + m_d * m_d);                    
            shadowContrib = max(shadowContrib, p);
        }
    }        
    // compute "half-lambert" term.
    float diffIntensity = shadowContrib * (dot(worldNormal, globalLightDir) * halfLambertScale + halfLambertBias);
    float3 halfVec = normalize(globalLightDir + worldEyeVec);
    float specIntensity = pow(saturate(dot(worldNormal, halfVec)), matSpecularPower);
    diffuse  += lerp(globalBackLightColor, globalLightColor, diffIntensity);
    specular += globalLightColor * specIntensity;
}        

//------------------------------------------------------------------------------
/**
    Evaluate a spot light.
*/
void
SpotLight(in int lightIndex,            // light index
          in float4 worldPos,           // surface position in world space
          in float3 worldNormal,        // surface normal in world space
          in float3 worldEyeVec,        // normalized eye vector in world space
          in float matSpecularPower,    // material specular power
          in out float4 diffuse,        // computed diffuse component
          in out float4 specular)       // computed specular component
{
    // compute distance to light
    float3 lightVec = lightPos[lightIndex] - worldPos;
    
    // compute attenuation
    float relDist = length(lightVec) * lightInvRange[lightIndex];        
    if (relDist < 1.0)
    {
        // compute distance attenuation
        float invDist = (1.0 - relDist);            
        float attenuate = invDist * invDist;

        // normalized light vec
        lightVec = normalize(lightVec);

        // diffuse intensity
        float diffIntensity = dot(worldNormal, lightVec) * attenuate;
        if (diffIntensity > 0.0f)
        {
            float4 projLightPos = mul(worldPos, lightProjTransform[lightIndex]);
            if (projLightPos.z > 0.0)
            {
                float2 lightSpaceUv = ((projLightPos.xy / projLightPos.w) * float2(0.5, -0.5)) + 0.5f;

                // modulate light by project map
                float4 lightModColor = lightColor[lightIndex] * tex2D(LightProjMapSampler, lightSpaceUv);
                if (any(lightModColor))
                {
                    // compute shadow term
                    float shadowContrib = 1.0;
                    if (lightCastShadows[lightIndex])
                    {
                        float4 shadowUvOffset = lightShadowBufferUvOffset[lightIndex];
                        float2 shadowMapUv = lightSpaceUv * shadowUvOffset.zw + shadowUvOffset.xy;

                        // lookup VSM moments from shadow map
                        // simulate linear sampling
                        float2 vsmMoments = DecodePSSMDepth(LightShadowBufferSampler, shadowMapUv, lightShadowBufferSize);
                        
                        float distToLight = projLightPos.z - lightShadowBias;
                        shadowContrib = distToLight <= vsmMoments.x;
                        float E_x2 = vsmMoments.y;
                        float Ex_2 = vsmMoments.x * vsmMoments.x;
                        float variance = min(max(E_x2 - Ex_2, 0.0) + lightVSMEpsilon, 1.0);
                        float m_d = (vsmMoments.x - distToLight);
                        float p = variance / (variance + m_d * m_d);                    
                        shadowContrib = max(shadowContrib, p);
                    }
                    if (shadowContrib > 0)
                    {                
                        // specular intensity
                        float3 halfVec = normalize(lightVec + worldEyeVec);
                        float specIntensity = pow(saturate(dot(worldNormal, halfVec)), matSpecularPower) * attenuate;
                
                        // add diffuse and specular components
                        lightModColor *= shadowContrib;
                        
                        diffuse  += lightModColor * diffIntensity;
                        specular += lightModColor * specIntensity;
                    }
                }
            }
        }                
    }
}          

//------------------------------------------------------------------------------
/**
    Pixel shader single-pass lighting function, performs lighting
    in world space.
*/
float4
psLight(int numLocalLights,           // number of local lights
        float4 worldPos,              // surface position in world space
        float4 projPos,               // surface position in projection space
        float3 worldNormal,           // surface normal in world space
        float3 worldEyeVec,           // normalized eye vector in world space
        float4 matDiffuse,            // material diffuse component
        float4 matSpecular,           // material specular component
        float matSpecularPower,       // material specular power
        float4 matEmissive)           // material emissive component
{
    float4 diffuse = 0;
    float4 specular = 0;

    // evaluate global light
    GlobalLight(worldPos, projPos, worldNormal, worldEyeVec, matSpecularPower, diffuse, specular);
    
    // evaluate local lights
    int lightIndex;
    for (lightIndex = 0; lightIndex < numLocalLights; lightIndex++)
    {
        SpotLight(lightIndex, worldPos, worldNormal, worldEyeVec, matSpecularPower, diffuse, specular);
    }
    float4 color = matEmissive + (diffuse * matDiffuse) + (specular * matSpecular);
    return color;
}
//------------------------------------------------------------------------------
#endif