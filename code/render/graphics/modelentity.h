#pragma once
#ifndef GRAPHICS_MODELENTITY_H
#define GRAPHICS_MODELENTITY_H
//------------------------------------------------------------------------------
/**
    @class Graphics::ModelEntity
    
    Client-side proxy of a InternalGraphics::InternalModelEntity.
    
    (C) 2008 Radon Labs GmbH
*/
#include "graphics/graphicsentity.h"

//------------------------------------------------------------------------------
namespace Graphics
{
class ModelEntity : public GraphicsEntity
{
    DeclareClass(ModelEntity);
public:
    /// constructor
    ModelEntity();
    /// destructor
    virtual ~ModelEntity();

    /// set the model's resource id
    void SetResourceId(const Resources::ResourceId& resId);
    /// get the model's resource id
    const Resources::ResourceId& GetResourceId() const;

private:
    /// called by stage when entity should setup itself
    virtual void Setup(const Ptr<Stage>& stage);

    Resources::ResourceId resId;
};

//------------------------------------------------------------------------------
/**
*/
inline void
ModelEntity::SetResourceId(const Resources::ResourceId& id)
{
    this->resId = id;
}

//------------------------------------------------------------------------------
/**
*/
inline const Resources::ResourceId&
ModelEntity::GetResourceId() const
{
    return this->resId;
}

} // namespace Graphics
//------------------------------------------------------------------------------
#endif
