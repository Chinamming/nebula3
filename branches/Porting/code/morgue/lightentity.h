#pragma once
#ifndef LIGHTING_LIGHTENTITY_H
#define LIGHTING_LIGHTENTITY_H
//------------------------------------------------------------------------------
/**
    @class Lighting::LightEntity
 
    Represents a local light source in a graphics stage.
    
    (C) 2007 Radon Labs GmbH
*/
#include "graphics/graphicsentity.h"
#include "graphics/cameraentity.h"
#include "lighting/shaderlighttype.h"
#include "resources/resourceid.h"
#include "resources/managedtexture.h"
#include "coregraphics/texture.h"

//------------------------------------------------------------------------------
namespace Lighting
{
class LightEntity : public Graphics::GraphicsEntity
{
    DeclareClass(LightEntity);
public:
    /// constructor
    LightEntity();
    /// destructor
    virtual ~LightEntity();    

    /// compute clip status against bounding box
    virtual Math::ClipStatus::Type ComputeClipStatus(const Math::bbox& box);

    /// set the light type for shader 
    void SetShaderLightType(ShaderLightType::Code lightType);
    /// get the light type
    ShaderLightType::Code GetShaderLightType() const;
    /// set light color
    void SetColor(const Math::float4& c);
    /// get light color
    const Math::float4& GetColor() const;
    /// set projection map UV offset
    void SetProjMapUvOffset(const Math::float4& r);
    /// get projection map UV rect
    const Math::float4& GetProjMapUvOffset() const;
    /// enable/disable shadow casting
    void SetCastShadows(bool b);
    /// get shadow casting flags
    bool GetCastShadows() const;
    /// get inverse transform (transforms from world to light space)
    const Math::matrix44& GetInvTransform() const;
    /// get light-projection matrix (transforms from light space to light projection space)
    const Math::matrix44& GetProjTransform() const;
    /// get inverse light * projection transform (transform from world to light projection space)
    const Math::matrix44& GetInvLightProjTransform() const;

private:
    friend class LightServer;
    friend class ShadowServer;

    /// called when transform matrix changed
    virtual void OnTransformChanged();
    /// check state of asynchronously loaded resources
    void ValidateManagedResources();
    /// set shadow buffer uv rectangle
    void SetShadowBufferUvOffset(const Math::float4& uvOffset);
    /// get shadow buffer uv rectangle
    const Math::float4& GetShadowBufferUvOffset() const;    

    Math::matrix44 invTransform;            // transform from world to light space
    Math::matrix44 projTransform;           // transform from light space to light projection space
    Math::matrix44 invLightProjTransform;   // transform from world to light projection space
    Math::float4 color;
    Math::float4 projMapUvOffset;
    ShaderLightType::Code shaderLightType;
    bool castShadows;
    Math::float4 shadowBufferUvOffset;
};

//------------------------------------------------------------------------------
/**
*/
inline void
LightEntity::SetShaderLightType(ShaderLightType::Code t)
{
    this->shaderLightType = t;
}

//------------------------------------------------------------------------------
/**
*/
inline ShaderLightType::Code
LightEntity::GetShaderLightType() const
{
    return this->shaderLightType;
}

//------------------------------------------------------------------------------
/**
*/
inline void
LightEntity::SetColor(const Math::float4& c)
{
    this->color = c;
}

//------------------------------------------------------------------------------
/**
*/
inline const Math::float4&
LightEntity::GetColor() const
{
    return this->color;
}

//------------------------------------------------------------------------------
/**
*/
inline void
LightEntity::SetProjMapUvOffset(const Math::float4& r)
{
    this->projMapUvOffset = r;
}

//------------------------------------------------------------------------------
/**
*/
inline const Math::float4&
LightEntity::GetProjMapUvOffset() const
{
    return this->projMapUvOffset;
}

//------------------------------------------------------------------------------
/**
*/
inline void
LightEntity::SetCastShadows(bool b)
{
    this->castShadows = b;
}

//------------------------------------------------------------------------------
/**
*/
inline bool
LightEntity::GetCastShadows() const
{
    return this->castShadows;
}

//------------------------------------------------------------------------------
/**
*/
inline const Math::matrix44& 
LightEntity::GetInvTransform() const
{
    return this->invTransform;
}

//------------------------------------------------------------------------------
/**
*/
inline const Math::matrix44&
LightEntity::GetProjTransform() const
{
    return this->projTransform;
}

//------------------------------------------------------------------------------
/**
*/
inline const Math::matrix44&
LightEntity::GetInvLightProjTransform() const
{
    return this->invLightProjTransform;
}

//------------------------------------------------------------------------------
/**
*/
inline void
LightEntity::SetShadowBufferUvOffset(const Math::float4& uvOffset)
{
    this->shadowBufferUvOffset = uvOffset;
}

//------------------------------------------------------------------------------
/**
*/
inline const Math::float4&
LightEntity::GetShadowBufferUvOffset() const
{
    return this->shadowBufferUvOffset;
}

} // namespace Lighting
//------------------------------------------------------------------------------
#endif

