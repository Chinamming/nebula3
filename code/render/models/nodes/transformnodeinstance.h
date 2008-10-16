#pragma once
#ifndef MODELS_TRANSFORMNODEINSTANCE_H
#define MODELS_TRANSFORMNODEINSTANCE_H
//------------------------------------------------------------------------------
/**
    @class Models::TransformNodeInstance

    Holds and applies per-node-instance transformation.
    
    (C) 2007 Radon Labs GmbH
*/
#include "models/modelnodeinstance.h"
#include "math/vector.h"
#include "math/point.h"
#include "math/quaternion.h"
#include "math/transform44.h"

//------------------------------------------------------------------------------
namespace Models
{
class TransformNodeInstance : public ModelNodeInstance
{
    __DeclareClass(TransformNodeInstance);
public:
    /// constructor
    TransformNodeInstance();
    /// destructor
    virtual ~TransformNodeInstance();

    /// perform per-frame updates
    virtual void Update();
    /// apply per-instance state prior to rendering
    virtual void ApplyState();

    /// set position
    void SetPosition(const Math::point& p);
    /// get position
    const Math::point& GetPosition() const;
    /// set rotate quaternion
    void SetRotate(const Math::quaternion& r);
    /// get rotate quaternion
    const Math::quaternion& GetRotate() const;
    /// set scale
    void SetScale(const Math::vector& s);
    /// get scale
    const Math::vector& GetScale() const;
    /// set rotate pivot
    void SetRotatePivot(const Math::point& p);
    /// get rotate pivot
    const Math::point& GetRotatePivot() const;
    /// set scale pivot
    void SetScalePivot(const Math::point& p);
    /// get scale pivot
    const Math::point& GetScalePivot() const;

    /// get resulting local transform matrix in local parent space
    const Math::matrix44& GetLocalTransform();
    /// get model space transform (valid after Update())
    const Math::matrix44& GetModelTransform() const;

protected:
    /// called when attached to ModelInstance
    virtual void OnAttachToModelInstance(const Ptr<ModelInstance>& inst, const Ptr<ModelNode>& node, const Ptr<ModelNodeInstance>& parentNodeInst);
    /// called when removed from ModelInstance
    virtual void OnRemoveFromModelInstance();

    Ptr<TransformNodeInstance> parentTransformNodeInstance;
    Math::transform44 tform;
    Math::matrix44 modelTransform;
};

//------------------------------------------------------------------------------
/**
*/
inline void
TransformNodeInstance::SetPosition(const Math::point& p)
{
    this->tform.setposition(p);
}

//------------------------------------------------------------------------------
/**
*/
inline const Math::point&
TransformNodeInstance::GetPosition() const
{
    return this->tform.getposition();
}

//------------------------------------------------------------------------------
/**
*/
inline void
TransformNodeInstance::SetRotate(const Math::quaternion& r)
{
    this->tform.setrotate(r);
}

//------------------------------------------------------------------------------
/**
*/
inline const Math::quaternion&
TransformNodeInstance::GetRotate() const
{
    return this->tform.getrotate();
}

//------------------------------------------------------------------------------
/**
*/
inline void
TransformNodeInstance::SetScale(const Math::vector& s)
{
    this->tform.setscale(s);
}

//------------------------------------------------------------------------------
/**
*/
inline const Math::vector&
TransformNodeInstance::GetScale() const
{
    return this->tform.getscale();
}

//------------------------------------------------------------------------------
/**
*/
inline void
TransformNodeInstance::SetRotatePivot(const Math::point& p)
{
    this->tform.setrotatepivot(p);
}

//------------------------------------------------------------------------------
/**
*/
inline const Math::point&
TransformNodeInstance::GetRotatePivot() const
{
    return this->tform.getrotatepivot();
}

//------------------------------------------------------------------------------
/**
*/
inline void
TransformNodeInstance::SetScalePivot(const Math::point& p)
{
    this->tform.setscalepivot(p);
}

//------------------------------------------------------------------------------
/**
*/
inline const Math::point&
TransformNodeInstance::GetScalePivot() const
{
    return this->tform.getscalepivot();
}

//------------------------------------------------------------------------------
/**
*/
inline const Math::matrix44&
TransformNodeInstance::GetLocalTransform()
{
    return this->tform.getmatrix();
}

//------------------------------------------------------------------------------
/**
*/
inline const Math::matrix44&
TransformNodeInstance::GetModelTransform() const
{
    return this->modelTransform;
}

} // namespace Models
//------------------------------------------------------------------------------
#endif
