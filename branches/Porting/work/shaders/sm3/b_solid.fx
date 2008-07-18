//------------------------------------------------------------------------------
//  b_solid.fx
//
//  Batch shader for solid shapes.
//
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------

technique t0
{
    pass p0
    {
        ColorWriteEnable  = RED|GREEN|BLUE|ALPHA;
        ZEnable           = True;
        ZWriteEnable      = False;
        ZFunc             = Equal;
        AlphaBlendEnable  = false;
    }
}
