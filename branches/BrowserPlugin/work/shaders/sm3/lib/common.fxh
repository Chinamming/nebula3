#ifndef COMMON_H
#define COMMON_H
//------------------------------------------------------------------------------
//  common.fxh
//
//  Definitions and functions needed by all shaders.
//
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------

#define SimpleTechnique(name, features, vertexShader, pixelShader) \
technique name < string Mask = features; > \
{ \
    pass p0 \
    { \
        VertexShader = compile vs_3_0 vertexShader(); \
        PixelShader = compile ps_3_0 pixelShader(); \
    } \
}

#define LightTechnique(name, features, vertexShader, pixelShader, numLights) \
technique name < string Mask = features; > \
{ \
    pass p0 \
    { \
        VertexShader = compile vs_3_0 vertexShader(); \
        PixelShader = compile ps_3_0 pixelShader(numLights); \
    } \
}

float4x4 mvp   : ModelViewProjection;
float4x4 model : Model;

texture diffMap : DiffMap0;
sampler diffMapSampler = sampler_state
{
    Texture = <diffMap>;
    AddressU = Wrap;
    AddressV = Wrap;
    MinFilter = Linear;
    MagFilter = Linear;
    MipFilter = Linear;
};

texture bumpMap : BumpMap0;
sampler bumpMapSampler = sampler_state
{
    Texture = <bumpMap>;
    AddressU = Wrap;
    AddressV = Wrap;
    MinFilter = Linear;
    MagFilter = Linear;
    MipFilter = Linear;
};

texture specMap : SpecMap0;
sampler specMapSampler = sampler_state
{
    Texture = <specMap>;
    AddressU = Wrap;
    AddressV = Wrap;
    MinFilter = Linear;
    MagFilter = Linear;
    MipFilter = Linear;
};

//------------------------------------------------------------------------------
/**
*/
float4 DummyPixelShader() : COLOR
{
    return 0;
}

//------------------------------------------------------------------------------
#endif

