//------------------------------------------------------------------------------
//  b_shape.fx
//  Batch shader for debug visualization.
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/**
*/
technique Default
{
    pass p0
    {
        ColorWriteEnable  = RED|GREEN|BLUE|ALPHA;
        ZEnable           = False;
        ZWriteEnable      = False;
        StencilEnable     = False;
        FogEnable         = False;
        AlphaBlendEnable  = True;
        SrcBlend          = SrcAlpha;
        DestBlend         = InvSrcAlpha;
        AlphaTestEnable   = False;
        ScissorTestEnable = False;
        CullMode          = CW;        
    }
}
