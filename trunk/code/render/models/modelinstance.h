#pragma once
#ifndef MODELS_MODELINSTANCE_H
#define MODELS_MODELINSTANCE_H
//------------------------------------------------------------------------------
/**
    @class Models::ModelInstance
    
    A ModelInstance contains the per-instance data for rendering a Model. 
    Usually there is one ModelInstance created per game object.
    
    A ModelInstance is roughly comparable to a Nebula2 nRenderContext.
    
    (C) 2007 Radon Labs GmbH
*/
#include "core/refcounted.h"
#include "timing/time.h"
#include "models/model.h"
#include "models/modelnodeinstance.h"

namespace InternalGraphics
{
    class InternalModelEntity;
}

//------------------------------------------------------------------------------
namespace Models
{
class ModelInstance : public Core::RefCounted
{
    __DeclareClass(ModelInstance);
public:
    /// constructor
    ModelInstance();
    /// destructor
    virtual ~ModelInstance();

    /// cleanup and unlink from model
    void Discard();
    /// return true if currently attached to Model
    bool IsAttachedToModel() const;
    /// get the Model this instance was created from
    const Ptr<Model>& GetModel() const;
    /// get the ModelEntity which owns this instance
    const Ptr<InternalGraphics::InternalModelEntity>& GetModelEntity() const;
    /// set world space transform of the instance
    void SetTransform(const Math::matrix44& m);
    /// get world space transform
    const Math::matrix44& GetTransform() const;
    /// set instance time
    void SetTime (Timing::Time t);
    /// get instance time
    Timing::Time GetTime() const;

    /// return true if a model instance node exists
    bool HasNodeInstance(const Util::Atom<Util::String>& name) const;
    /// lookup an instance node
    const Ptr<ModelNodeInstance>& LookupNodeInstance(const Util::Atom<Util::String>& name) const;
    /// get all instance nodes
    const Util::Array<Ptr<ModelNodeInstance> >& GetNodeInstances() const;

    /// lookup an instance node by rtti type
    const Ptr<ModelNodeInstance>& LookupNodeInstanceByRTTI(const Core::Rtti& rtti) const;

    /// perform per-frame update (after setting transform, visibility, time, etc)
    void Update();

    /// render node specific debug shape
    void RenderDebug();

    /// set all modelnode instances visible
    void SetAllNodeInstancesVisible(bool b);

protected:
    friend class Model;
    friend class VisResolver;
    friend class InternalGraphics::InternalModelEntity;

    /// set pointer to ModelEntity which owns this instance
    void SetModelEntity(const Ptr<InternalGraphics::InternalModelEntity>& mdlEntity);
    /// notify the model instance that it is currently visible
    void NotifyVisible(IndexT frameIndex);
    /// called when attached to Model
    virtual void OnAttachToModel(const Ptr<Model>& model);
    /// called when removed from Model
    virtual void OnRemoveFromModel();

    Ptr<Model> model;
    Ptr<ModelNode> modelNode;
    Ptr<InternalGraphics::InternalModelEntity> modelEntity;
    Math::matrix44 transform;
    Timing::Time time;
    Util::Array<Ptr<ModelNodeInstance> > nodeInstances;
};

//------------------------------------------------------------------------------
/**
*/
inline bool
ModelInstance::IsAttachedToModel() const
{
    return this->model.isvalid();
}

//------------------------------------------------------------------------------
/**
*/
inline const Ptr<Model>&
ModelInstance::GetModel() const
{
    return this->model;
}

//------------------------------------------------------------------------------
/**
*/
inline void
ModelInstance::SetTransform(const Math::matrix44& m)
{
    this->transform = m;
}

//------------------------------------------------------------------------------
/**
*/
inline const Math::matrix44&
ModelInstance::GetTransform() const
{
    return this->transform;
}

//------------------------------------------------------------------------------
/**
*/
inline void
ModelInstance::SetTime(Timing::Time t)
{
    this->time = t;
}

//------------------------------------------------------------------------------
/**
*/
inline Timing::Time
ModelInstance::GetTime() const
{
    return this->time;
}

//------------------------------------------------------------------------------
/**
*/
inline const Util::Array<Ptr<ModelNodeInstance> >&
ModelInstance::GetNodeInstances() const
{
    return this->nodeInstances;
}

//------------------------------------------------------------------------------
/**
*/
inline void 
ModelInstance::SetAllNodeInstancesVisible(bool b)
{
    IndexT i;
    for (i = 0; i < this->nodeInstances.Size(); i++)
    {
        // set all instances visible, will transfer the visible state on their children
        this->nodeInstances[i]->SetVisible(b);
    }
}

} // namespace Models
//------------------------------------------------------------------------------
#endif
