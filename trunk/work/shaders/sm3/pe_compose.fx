//------------------------------------------------------------------------------
//  pe_compose.fx
//
//  Simple frame composition post effect shader.
//
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------

texture colorBuffer : ColorBuffer;
sampler ColorBufferSampler = sampler_state
{
    Texture = <colorBuffer>;
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

shared float Saturation : Saturation = 1.0;
shared float4 Balance : Balance = float4(1.0f, 1.0f, 1.0f, 1.0f);
shared float4 Luminance : Lumincance = float4(0.299f, 0.587f, 0.114f, 0.0f);

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
    float4 c = tex2D(ColorBufferSampler, psIn.uv0);
    float4 grey = dot(c.xyz, Luminance.xyz);
    return Balance * lerp(grey, c, Saturation);
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
