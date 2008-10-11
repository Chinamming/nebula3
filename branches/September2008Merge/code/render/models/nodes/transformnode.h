#pragma once
#ifndef MODELS_TRANSFORMNODE_H
#define MODELS_TRANSFORMNODE_H
//------------------------------------------------------------------------------
/**
    @class Models::TransformNode
    
    Defines a transformation in a transform hierarchy.
    
    (C) 2007 Radon Labs GmbH
*/
#include "models/modelnode.h"
#include "math/point.h"
#include "math/vector.h"
#include "math/quaternion.h"

//------------------------------------------------------------------------------
namespace Models
{
class TransformNode : public ModelNode
{
    __DeclareClass(TransformNode);
public:
    /// constructor
    TransformNode();
    /// destructor
    virtual ~TransformNode();

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

protected:
    /// create a model node instance
    virtual Ptr<ModelNodeInstance> CreateNodeInstance() const;
    /// called to initialize from attributes
    virtual void LoadFromAttrs(const Attr::AttributeContainer& attrs);
    /// called to save state back to attributes
    virtual void SaveToAttrs(Attr::AttributeContainer& attrs);

    Math::point position;
    Math::quaternion rotate;
    Math::vector scale;
    Math::point rotatePivot;
    Math::point scalePivot;
};

//------------------------------------------------------------------------------
/**
*/
inline void
TransformNode::SetPosition(const Math::point& p)
{
    this->position = p;
}

//------------------------------------------------------------------------------
/**
*/
inline const Math::point&
TransformNode::GetPosition() const
{
    return this->position;
}

//------------------------------------------------------------------------------
/**
*/
inline void
TransformNode::SetRotate(const Math::quaternion& r)
{
    this->rotate = r;
}

//------------------------------------------------------------------------------
/**
*/
inline const Math::quaternion&
TransformNode::GetRotate() const
{
    return this->rotate;
}

//------------------------------------------------------------------------------
/**
*/
inline void
TransformNode::SetScale(const Math::vector& s)
{
    this->scale = s;
}

//------------------------------------------------------------------------------
/**
*/
inline const Math::vector&
TransformNode::GetScale() const
{
    return this->scale;
}

//------------------------------------------------------------------------------
/**
*/
inline void
TransformNode::SetRotatePivot(const Math::point& p)
{
    this->rotatePivot = p;
}

//------------------------------------------------------------------------------
/**
*/
inline const Math::point&
TransformNode::GetRotatePivot() const
{
    return this->rotatePivot;
}

//------------------------------------------------------------------------------
/**
*/
inline void
TransformNode::SetScalePivot(const Math::point& p)
{
    this->scalePivot = p;
}

//------------------------------------------------------------------------------
/**
*/
inline const Math::point&
TransformNode::GetScalePivot() const
{
    return this->scalePivot;
}

} // namespace Models
//------------------------------------------------------------------------------
#endif
    