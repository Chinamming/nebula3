//------------------------------------------------------------------------------
//  shared.fx
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "lighting_shared.fxh"

shared float saturation  : Saturation;
shared float4 balance    : Balance;
shared float4 luminance  : Luminance;
shared float4 displayRes : DisplayResolution;
shared float4x4 view     : View;
shared float time        : Time;

technique t0
{
    pass p0 { }
}
