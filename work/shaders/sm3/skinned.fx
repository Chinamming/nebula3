//------------------------------------------------------------------------------
//  skinned.fx
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "lighting.fxh"
#include "common.fxh"

matrix<float,4,3> jPalette[72] : JointPalette;

struct vsInputSkinnedDepth
{
    float4 position : POSITION;
    float4 weights  : BLENDWEIGHT;
    uint4 indices   : BLENDINDICES;
};

struct vsVSMPassOutput
{
    float4 position : POSITION;
    float depth : TEXCOORD0;
};

struct vsColorPassInputSkinned
{
    float4 position : POSITION;
    float3 normal   : NORMAL;
    float3 tangent  : TANGENT;
    float3 binormal : BINORMAL;    
    float4 weights  : BLENDWEIGHT;
    uint4 indices   : BLENDINDICES;
    float2 uv0      : TEXCOORD0;
};

struct vsColorPassOutput
{
    float4 position    : POSITION;
    float2 uv0         : TEXCOORD0;
    float4 worldPos    : TEXCOORD1;     // position in world space
    float4 projPos     : TEXCOORD2;     // position in projection space    
    float3 normal      : TEXCOORD3;
    float3 tangent     : TEXCOORD4;    
    float3 binormal    : TEXCOORD5;
    float3 worldEyeVec : COLOR0;        // normalized world space eye vector
};

//------------------------------------------------------------------------------
/**
    skinnedPosition()
    
    Compute a skinned position.
    
    @param  inPos           input vertex position
    @param  weights         4 weights
    @param  indices         4 joint indices into the joint palette
    @param  jointPalette    joint palette as vector4 array
    @return                 the skinned position
*/
float4
skinnedPosition(const float4 inPos, const float4 weights, const uint4 indices, const matrix<float,4,3> jointPalette[72])
{    
    float3 pos[4];
    pos[0] = (mul(inPos, jointPalette[indices[0]])) * weights[0];
    pos[1] = (mul(inPos, jointPalette[indices[1]])) * weights[1];
    pos[2] = (mul(inPos, jointPalette[indices[2]])) * weights[2];
    pos[3] = (mul(inPos, jointPalette[indices[3]])) * weights[3];
    return float4(pos[0] + pos[1] + pos[2] + pos[3], 1.0f);
}

//------------------------------------------------------------------------------
/**
    skinnedNormal()
    
    Compute a skinned normal vector (without renormalization).
    
    @param  inNormal        input normal vector
    @param  weights         4 weights
    @param  indices         4 joint indices into the joint palette
    @param  jointPalette    joint palette as vector4 array
    @return                 the skinned normal
*/
float3
skinnedNormal(const float3 inNormal, const float4 weights, const uint4 indices, const matrix<float,4,3> jointPalette[72])
{
    float3 normal[4];
    normal[0] = mul(inNormal, (matrix<float,3,3>)jointPalette[indices[0]]) * weights[0];
    normal[1] = mul(inNormal, (matrix<float,3,3>)jointPalette[indices[1]]) * weights[1];
    normal[2] = mul(inNormal, (matrix<float,3,3>)jointPalette[indices[2]]) * weights[2];
    normal[3] = mul(inNormal, (matrix<float,3,3>)jointPalette[indices[3]]) * weights[3];
    return float3(normal[0] + normal[1] + normal[2] + normal[3]);
}

//------------------------------------------------------------------------------
/**
    Vertex shader for skinning (write depth values).
*/
float4 DepthPassVertexShaderSkinned(const vsInputSkinnedDepth vsIn) : POSITION
{
    float4 weights = UnpackWeights(vsIn.weights);
    float4 skinPos = skinnedPosition(vsIn.position, weights, vsIn.indices, jPalette);
    skinPos = mul(skinPos, mvp);    
    return skinPos;
}

//------------------------------------------------------------------------------
/**
    Vertex shader function for rendering variance shadow map.
*/
vsVSMPassOutput
VSMPassVertexShaderSkinned(const vsInputSkinnedDepth vsIn)
{
    vsVSMPassOutput vsOut;
    float4 weights = UnpackWeights(vsIn.weights);
    float4 skinPos = skinnedPosition(vsIn.position, weights, vsIn.indices, jPalette);    

    vsOut.position = mul(skinPos, mvp);   
    vsOut.depth = vsOut.position.z;
    return vsOut;
}

//------------------------------------------------------------------------------
/**
    Pixel shader function for rendering variance shadow map. Contains
    the light space depth in R, and squared depth in G.
*/
float4
VSMPassPixelShader(vsVSMPassOutput psIn) : COLOR
{
    return EncodePSSMDepth(psIn.depth.x);
}

//------------------------------------------------------------------------------
/**
    Vertex shader function for the color pass.
*/
vsColorPassOutput
ColorPassVertexShaderSkinned(const vsColorPassInputSkinned vsIn)
{
    vsColorPassOutput vsOut;
    float4 weights = UnpackWeights(vsIn.weights);
    float4 skinPos = skinnedPosition(vsIn.position, weights, vsIn.indices, jPalette);  
    vsOut.position     = mul(skinPos, mvp);
    vsOut.projPos      = vsOut.position;    
    vsOut.worldPos     = mul(skinPos, model);    
    vsOut.worldEyeVec  = normalize(eyePos - vsOut.worldPos);    
    float3 skinNormal   = skinnedNormal(UnpackNormal(vsIn.normal), weights, vsIn.indices, jPalette);
    float3 skinTangent  = skinnedNormal(UnpackNormal(vsIn.tangent), weights, vsIn.indices, jPalette);
    float3 skinBinormal = skinnedNormal(UnpackNormal(vsIn.binormal), weights, vsIn.indices, jPalette);
    vsOut.normal      = normalize(mul(skinNormal, model));
    vsOut.tangent     = normalize(mul(skinTangent, model));
    vsOut.binormal    = normalize(mul(skinBinormal, model));
    vsOut.uv0 = UnpackUv(vsIn.uv0);
    return vsOut;
}

//------------------------------------------------------------------------------
/**
*/
float4 
ColorPassPixelShader(const vsColorPassOutput psIn, uniform int numLights) : COLOR
{
    float3 worldNormal = psWorldSpaceNormalFromBumpMap(bumpMapSampler, psIn.uv0, psIn.normal, psIn.tangent, psIn.binormal);
    float4 matDiffuse = tex2D(diffMapSampler, psIn.uv0);
    float4 matSpecular = tex2D(specMapSampler, psIn.uv0);
    float matSpecPower = 32.0;
    float4 matEmissive = float4(0.0, 0.0, 0.0, 1.0);
    float4 color = psLight(numLights, psIn.worldPos, psIn.projPos, worldNormal, psIn.worldEyeVec, matDiffuse, matSpecular, matSpecPower, matEmissive);
    color.a = matDiffuse.a;
    return color;
}


//------------------------------------------------------------------------------
SimpleTechnique(Depth, "Depth|Skinned", DepthPassVertexShaderSkinned, DummyPixelShader);
SimpleTechnique(VSMDepth, "VSMDepth|Skinned", VSMPassVertexShaderSkinned, VSMPassPixelShader);

LightTechnique(SolidSkinned0, "Solid|Skinned|LocalLights0", ColorPassVertexShaderSkinned, ColorPassPixelShader, 0);
LightTechnique(SolidSkinned1, "Solid|Skinned|LocalLights1", ColorPassVertexShaderSkinned, ColorPassPixelShader, 1);
LightTechnique(SolidSkinned2, "Solid|Skinned|LocalLights2", ColorPassVertexShaderSkinned, ColorPassPixelShader, 2);
LightTechnique(SolidSkinned3, "Solid|Skinned|LocalLights3", ColorPassVertexShaderSkinned, ColorPassPixelShader, 3);
LightTechnique(SolidSkinned4, "Solid|Skinned|LocalLights4", ColorPassVertexShaderSkinned, ColorPassPixelShader, 4);

LightTechnique(AlphaSkinned0, "Alpha|Skinned|LocalLights0", ColorPassVertexShaderSkinned, ColorPassPixelShader, 0);
LightTechnique(AlphaSkinned1, "Alpha|Skinned|LocalLights1", ColorPassVertexShaderSkinned, ColorPassPixelShader, 1);
LightTechnique(AlphaSkinned2, "Alpha|Skinned|LocalLights2", ColorPassVertexShaderSkinned, ColorPassPixelShader, 2);
LightTechnique(AlphaSkinned3, "Alpha|Skinned|LocalLights3", ColorPassVertexShaderSkinned, ColorPassPixelShader, 3);
LightTechnique(AlphaSkinned4, "Alpha|Skinned|LocalLights4", ColorPassVertexShaderSkinned, ColorPassPixelShader, 4);
