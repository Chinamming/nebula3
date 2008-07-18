//------------------------------------------------------------------------------
//  pe_copy.fx
//
//  A simple copy posteffect shader.
//
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------

texture sourceBuffer : SourceBuffer;
sampler SourceBufferSampler = sampler_state
{
    Texture = <sourceBuffer>;
    AddressU = Clamp;
    AddressV = Clamp;
    MinFilter = Point;
    MagFilter = Point;
    MipFilter = None;
};

struct vsInOut
{
    float4 position : POSITION;
    float2 uv0      : TEXCOORD0;
};

//------------------------------------------------------------------------------
/**
*/
vsInOut vsMain(const vsInOut vsIn)
{
    vsInOut vsOut;
    vsOut.position = vsIn.position;
    vsOut.uv0 = vsIn.uv0;
    return vsOut;
}

//------------------------------------------------------------------------------
/**
*/
float4 psMain(const vsInOut psIn) : COLOR
{
    return tex2D(SourceBufferSampler, psIn.uv0);
}

//------------------------------------------------------------------------------
technique t0
{
    pass p0
    {
        ZWriteEnable     = False;
        ZEnable          = False;
        ColorWriteEnable = RED|GREEN|BLUE|ALPHA;
        AlphaBlendEnable = False;
        AlphaTestEnable  = False;
        CullMode         = None;
        StencilEnable    = False;
        VertexShader     = compile vs_3_0 vsMain();
        PixelShader      = compile ps_3_0 psMain();
    }
}
