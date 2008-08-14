#pragma once
#ifndef LIGHTING_SPOTLIGHTENTITY_H
#define LIGHTING_SPOTLIGHTENTITY_H
//------------------------------------------------------------------------------
/**
    @class Lighting::SpotLightEntity
    
    Implements a local spot light. The spot light's cone is computed
    from the transformation matrix of the light entity. The spot light
    cone will point along -z, and the cone's angle will be defined
    by the length of x and y component at the end of the z component.
    
    (C) 2007 Radon Labs GmbH
*/
#include "lighting/abstractlightentity.h"

//------------------------------------------------------------------------------
namespace Lighting
{
class SpotLightEntity : public AbstractLightEntity
{
    DeclareClass(SpotLightEntity);
public:
    /// constructor
    SpotLightEntity();
    /// compute clip status against bounding box
    virtual Math::ClipStatus::Type ComputeClipStatus(const Math::bbox& box);
    
protected:
    /// called when transform matrix changed
    virtual void OnTransformChanged();
};

} // namespace Lighting
//------------------------------------------------------------------------------
#endif
