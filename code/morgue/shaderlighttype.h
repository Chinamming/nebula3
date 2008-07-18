#pragma once
#ifndef LIGHTING_SHADERLIGHTTYPE_H
#define LIGHTING_SHADERLIGHTTYPE_H
//------------------------------------------------------------------------------
/**
    @class Lighting::ShaderLightType
    
    The ShaderLightType class is used to identify a light type to shaders. An 
    application may define its own light types by deriving from the 
    ShaderLightType class.
    
    (C) 2007 Radon Labs GmbH
*/
#include "core/types.h"

//------------------------------------------------------------------------------
namespace Lighting
{
class ShaderLightType
{
public:
    /// light type typedef
    typedef int Code;
    
    /// an outdoor global sunlight
    static const Code Sun = 0;
    /// a point light
    static const Code Point = 1;
    /// a simple spot light
    static const Code Spot = 2;
    /// a double spot light (2 opposite spots)
    static const Code DoubleSpot = 3;

    /// number of lights defined in this class
    static const int NumShaderLightTypes = 4;
};

} // namespace Lighting
//------------------------------------------------------------------------------
#endif
    