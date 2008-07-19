#pragma once
#ifndef GRAPHICS_MODELENTITY_H
#define GRAPHICS_MODELENTITY_H
//------------------------------------------------------------------------------
/**
    @class Graphics::ModelEntity
    
    Represents a visible graphics object.
    
    (C) 2007 Radon Labs GmbH
*/
#include "graphics/graphicsentity.h"
#include "resources/resourceid.h"
#include "models/managedmodel.h"
#include "models/modelinstance.h"

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
    /// get the state of the contained managed model resource
    Resources::Resource::State GetModelResourceState() const;
    /// get pointer to model instance (only valid if already loaded)
    const Ptr<Models::ModelInstance>& GetModelInstance() const;

protected:
    /// called when entity is created
    virtual void OnActivate();
    /// called before entity is destroyed
    virtual void OnDeactivate();
    /// called when transform matrix changed
    virtual void OnTransformChanged();
    /// called to update the entity before rendering
    virtual void OnUpdate();
    /// called to render a debug visualization of the entity
    virtual void OnRenderDebug();
    /// validate the ModelInstance
    void ValidateModelInstance();

    Resources::ResourceId resId;
    Ptr<Models::ManagedModel> managedModel;
    Ptr<Models::ModelInstance> modelInstance;
    bool setModelNodesVisible;
};

//------------------------------------------------------------------------------
/**
*/
inline void
ModelEntity::SetResourceId(const Resources::ResourceId& id)
{
    n_assert(!this->IsActive());
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

//------------------------------------------------------------------------------
/**
*/
inline Resources::Resource::State
ModelEntity::GetModelResourceState() const
{
    return this->managedModel->GetState();
}

//------------------------------------------------------------------------------
/**
*/
inline const Ptr<Models::ModelInstance>&
ModelEntity::GetModelInstance() const
{
    return this->modelInstance;
}

} // namespace Graphics
//------------------------------------------------------------------------------
#endif
    