#pragma once
#ifndef LIGHTING_LIGHTTYPE_H
#define LIGHTING_LIGHTTYPE_H
//------------------------------------------------------------------------------
/**
    @class Lighting::LightType
    
    Identifies different light types.
    
    (C) 2007 Radon Labs GmbH
*/
#include "core/types.h"

//------------------------------------------------------------------------------
namespace Lighting
{
class LightType
{
public:
    enum Code
    {
        Global = 0,
        Spot,
        Point,

        NumLightTypes,
        InvalidLightType = 0xffffffff,      // force size to 32 bit
    };
}; 

}   // namespace Lighting
//------------------------------------------------------------------------------
#endif
    