//------------------------------------------------------------------------------
//  particle.fx
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "lighting.fxh"
#include "common.fxh"

bool Switch0 = true;
struct vsColorPassInput
{
    float4 position  : POSITION;    // the particle position in world space
    float4 velocity  : NORMAL;      // orientation or stretch-velocity 
    float2 uv        : TEXCOORD0;   // the uv coordinate
    float2 rotation  : TEXCOORD1;   // x-component contains overall rotation angle
    float4 data      : COLOR0;      // alpha, cornerRotation, scale and color-code
};

struct vsColorPassOutput
{
    float4 position     : POSITION;
    float4 diffuse      : COLOR0;
    float2 uv0          : TEXCOORD0;
    float4 lightVecFog  : TEXCOORD1;
    float3 halfVec      : TEXCOORD2;
    float3 eyeVec       : TEXCOORD3;
};

//------------------------------------------------------------------------------
/**
    Compute particle vertex position from input.
*/
void ComputeParticleVertex(in vsColorPassInput vsIn, 
                           out float4 outPos, 
                           out float3 outModelNormal,
                           out float3 outModelTangent,
                           out float3 outModelBinormal,
                           out float4 outColor)
{
    float alphaCode = vsIn.data[0];
    float cornerRotation = vsIn.data[1];
    float size = vsIn.data[2];
    float colorCode = vsIn.data[3];
    float spriteRotation = vsIn.rotation.x;

    // build corner-rotation matrix
    float cornerSinAng, cornerCosAng;
    sincos(cornerRotation, cornerSinAng, cornerCosAng);
    float3x3 cornerRot = {
        cornerCosAng, -cornerSinAng, 0.0f,
        cornerSinAng,  cornerCosAng, 0.0f,
        0.0f,    0.0f,   1.0f,
    };
    
    // build sprite-rotation matrix
    float spriteSinAng, spriteCosAng;
    sincos(spriteRotation, spriteSinAng, spriteCosAng);
    float3x3 spriteRot = {
        spriteCosAng, -spriteSinAng, 0.0f,
        spriteSinAng,  spriteCosAng, 0.0f,
        0.0f,    0.0f,   1.0f,
    };

    // decode color data
    float4 rgba;
    rgba.z = modf(colorCode/256.0f,colorCode);
    rgba.y = modf(colorCode/256.0f,colorCode);
    rgba.x = modf(colorCode/256.0f,colorCode);
    rgba.w = modf(alphaCode/256.0f,alphaCode);
    rgba *= 256.0f/255.0f;

    float4 position =  vsIn.position;

    // the corner offset gets calculated from the velocity
    float3 extrude;
    
    // billboard orientation    
    if (Switch0)
    {
        extrude = mul(imv, float3(vsIn.velocity.x, vsIn.velocity.y,vsIn.velocity.z));
        if (alphaCode != 0.0f) 
        {
            extrude = normalize(extrude);
            float vis = abs(extrude.z);
            size *= cos(vis*3.14159f*0.5f);
            rgba.w *= cos(vis*3.14159f*0.5f);
        };
        extrude.z = 0.0f;
        extrude = normalize(extrude);    
        extrude *= size;
        extrude = mul(cornerRot, extrude);
        extrude = mul(extrude, (float3x3) imv);    
    }
    // emitter orientation    
    else
    {
        extrude = vsIn.velocity;        //changed!
        if (alphaCode != 0.0f) 
        {
            extrude = normalize(extrude);
            float vis = abs(extrude.z);
            size *= cos(vis * 3.14159f * 0.5f);
            rgba.w *= cos(vis * 3.14159f * 0.5f);
        };
        extrude.z = 0.0f;
        extrude = normalize(extrude);    
        extrude *= size;
        extrude = mul(cornerRot, extrude);
        //extrude = mul(extrude, 1);
    }
    position.xyz += extrude.xyz;
    //position.w = 1.0f;
    outPos = position;
    
    // generate a tangent in view space, rotate it, and transform back to model space
    outModelTangent = float3(1, 0, 0);
    outModelTangent = mul(spriteRot, outModelTangent);    
    outModelTangent = mul(outModelTangent, (float3x3) imv);
    
    // same for the binormal...
    outModelBinormal = float3(0, 1, 0);
    outModelBinormal = mul(spriteRot, outModelBinormal);
    outModelBinormal = mul(outModelBinormal, (float3x3) imv);
    
    // normal doesn't need to be rotated
    outModelNormal = imv[2].xyz;

    outColor = rgba;
}

//------------------------------------------------------------------------------
/**
    Vertex shader function for the color pass.
*/
vsColorPassOutput
ParticlePassVertexShader(const vsColorPassInput vsIn)
{
    vsColorPassOutput vsOut;

    // compute per-vertex-data
    float3 modelNormal, modelTangent, modelBinormal;
    float4 modelPos;
    float4 rgba;    
    ComputeParticleVertex(vsIn, modelPos, modelNormal, modelTangent, modelBinormal, rgba);
    //vsLight(modelPos, modelNormal, modelTangent, modelBinormal, ModelEyePos, ModelLightPos, vsOut.lightVecFog.xyz, vsOut.halfVec, vsOut.eyeVec);
    vsOut.position = mul(modelPos, mvp);
    //vsOut.lightVecFog.w = vsFog(vsOut.position);
    //VS_SETSCREENPOS(vsOut.position);    
    vsOut.diffuse = rgba;
    vsOut.uv0 = vsIn.uv;
    //dont need just now
    vsOut.lightVecFog = float4(0,0,0,0);
    vsOut.halfVec = float4(0,0,0,0);
    vsOut.eyeVec = float3(0,0,0);
    
    return vsOut;
 /*   
    vsColorPassOutput vsOut;
    vsOut.position    = mul(vsIn.position, mvp);
    vsOut.projPos     = vsOut.position;
    vsOut.worldPos    = mul(vsIn.position, model);
    vsOut.worldEyeVec = normalize(eyePos - vsOut.worldPos);
    vsOut.normal      = normalize(mul(UnpackNormal(vsIn.normal), model));
    vsOut.tangent     = normalize(mul(UnpackNormal(vsIn.tangent), model));
    vsOut.binormal    = normalize(mul(UnpackNormal(vsIn.binormal), model));
    vsOut.uv0 = UnpackUv(vsIn.uv0);
    return vsOut;*/
}

//------------------------------------------------------------------------------
/**
*/
float4 
ParticlePassPixelShader(const vsColorPassOutput psIn) : COLOR
{
    /*float3 worldNormal = psWorldSpaceNormalFromBumpMap(bumpMapSampler, psIn.uv0, psIn.normal, psIn.tangent, psIn.binormal);
    float4 matDiffuse = tex2D(diffMapSampler, psIn.uv0);
    float4 matSpecular = tex2D(specMapSampler, psIn.uv0);
    float matSpecPower = 32.0;
    float4 matEmissive = float4(0.0, 0.0, 0.0, 1.0);
    float4 color = psLight(numLights, psIn.worldPos, psIn.projPos, worldNormal, psIn.worldEyeVec, matDiffuse, matSpecular, matSpecPower, matEmissive);*/
    float4 finalColor = tex2D(diffMapSampler, psIn.uv0) * psIn.diffuse;
    
    return finalColor;
}

//------------------------------------------------------------------------------
SimpleTechnique(ParticleNoLight, "Alpha", ParticlePassVertexShader, ParticlePassPixelShader); 

