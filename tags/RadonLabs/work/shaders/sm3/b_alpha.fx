//------------------------------------------------------------------------------
//  b_alpha.fx
//
//  Batch shader for translucent shapes.
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
        ZFunc             = LessEqual;
        AlphaBlendEnable  = true;
        DestBlend         = InvSrcAlpha;
        SrcBlend          = SrcAlpha;
    }
}
