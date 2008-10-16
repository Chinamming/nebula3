#pragma once
#ifndef GRAPHICS_GLOBALLIGHTENTITY_H
#define GRAPHICS_GLOBALLIGHTENTITY_H
//------------------------------------------------------------------------------
/**
    @class Graphics::GlobalLightEntity
    
    Client-side proxy of a InternalGraphics::InternalGlobalLightEntity.
    
    (C) 2008 Radon Labs GmbH
*/
#include "graphics/abstractlightentity.h"

//------------------------------------------------------------------------------
namespace Graphics
{
class GlobalLightEntity : public AbstractLightEntity
{
    __DeclareClass(GlobalLightEntity);
public:
    /// constructor
    GlobalLightEntity();
    /// set the backlight color
    void SetBackLightColor(const Math::float4& c);
    /// get the backlight color
    const Math::float4& GetBackLightColor() const;

private:
    /// called by stage when entity should setup itself
    virtual void Setup(const Ptr<Stage>& stage);

    Math::float4 backLightColor;
};

//------------------------------------------------------------------------------
/**
*/
inline void
GlobalLightEntity::SetBackLightColor(const Math::float4& c)
{
    this->backLightColor = c;
}

//------------------------------------------------------------------------------
/**
*/
inline const Math::float4&
GlobalLightEntity::GetBackLightColor() const
{
    return this->backLightColor;
}

} // namespace Graphics
//------------------------------------------------------------------------------
#endif
    