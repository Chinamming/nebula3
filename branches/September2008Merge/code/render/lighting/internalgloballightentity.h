#pragma once
#ifndef LIGHTING_INTERNALGLOBALLIGHTENTITY_H
#define LIGHTING_INTERNALGLOBALLIGHTENTITY_H
//------------------------------------------------------------------------------
/**
    @class Lighting::InternalGlobalLightEntity
  
    Implements a global directional light intended for outdoor 
    areas or as the basis for indoor rendering. Besides the primary directional 
    light component, a global light also usually provides some sort of ambient 
    component for areas in shadow. There should only be one GlobalLight active 
    during rendering a frame. The primary light is directed along the negative
    z-axis of the light's transform matrix. Scaling and position has no
    effect on the global light.
    
    (C) 2007 Radon Labs GmbH
*/
#include "lighting/internalabstractlightentity.h"

//------------------------------------------------------------------------------
namespace Lighting
{
class InternalGlobalLightEntity : public InternalAbstractLightEntity
{
    __DeclareClass(InternalGlobalLightEntity);
public:
    /// constructor
    InternalGlobalLightEntity();
    /// compute clip status against bounding box
    virtual Math::ClipStatus::Type ComputeClipStatus(const Math::bbox& box);
    
    /// set the backlight color
    void SetBackLightColor(const Math::float4& c);
    /// get the backlight color
    const Math::float4& GetBackLightColor() const;
    /// get the light direction (extracted from negative z-axis of transformation matrix)
    const Math::vector& GetLightDirection() const;

protected:
    /// called when transform matrix changed
    virtual void OnTransformChanged();

    Math::float4 backLightColor;
    Math::vector lightDir;
}; 

//------------------------------------------------------------------------------
/**
*/
inline void
InternalGlobalLightEntity::SetBackLightColor(const Math::float4& c)
{
    this->backLightColor = c;
}

//------------------------------------------------------------------------------
/**
*/
inline const Math::float4&
InternalGlobalLightEntity::GetBackLightColor() const
{
    return this->backLightColor;
}

//------------------------------------------------------------------------------
/**
*/
inline const Math::vector&
InternalGlobalLightEntity::GetLightDirection() const
{
    return this->lightDir;
}

} // namespace Lighting
//------------------------------------------------------------------------------
#endif
