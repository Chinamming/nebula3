#pragma once
#ifndef LIGHTING_GLOBALLIGHT_H
#define LIGHTING_GLOBALLIGHT_H
//------------------------------------------------------------------------------
/**
    @class Lighting::GlobalLight
  
    A GlobalLight provides all illumination that is not handled
    by LocalLights (most notably the ambient term, but usually also a
    directional term that affects all objects in the scene (for instance
    the sunlight). There may only be one GlobalLight per scene.
    
    (C) 2007 Radon Labs GmbH
*/
#include "graphics/lightentity.h"

//------------------------------------------------------------------------------
namespace Lighting
{
class GlobalLight : public Graphics::LightEntity
{
    DeclareClass(GlobalLight);
public:
    /// constructor
    GlobalLight();
    /// destructor
    virtual ~GlobalLight();    

};

} // namespace Lighting
//------------------------------------------------------------------------------
#endif
