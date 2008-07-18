//------------------------------------------------------------------------------
//  b_depth.fx
//
//  Batch shader for the depth pass.
//
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------

technique t0
{
    pass p0
    {
        ColorWriteEnable  = 0;
        ZEnable           = True;
        ZWriteEnable      = True;
        ZFunc             = LessEqual;
        AlphaBlendEnable  = false;
    }
}
