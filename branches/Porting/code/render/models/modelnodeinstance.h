#pragma once
#ifndef MODELS_MODELNODEINSTANCE_H
#define MODELS_MODELNODEINSTANCE_H
//------------------------------------------------------------------------------
/**
    @class Models::ModelNodeInstance
    
    A ModelNodeInstance holds the per-instance data of a ModelNode and
    does most of the actually interesting Model rendering stuff.
    
    (C) 2007 Radon Labs GmbH
*/
#include "core/refcounted.h"
#include "attr/attributecontainer.h"

//------------------------------------------------------------------------------
namespace Models
{
class ModelInstance;
class ModelNode;

class ModelNodeInstance : public Core::RefCounted
{
    __DeclareClass(ModelNodeInstance);
public:
    /// constructor
    ModelNodeInstance();
    /// destructor
    virtual ~ModelNodeInstance();

    /// perform per-frame updates
    virtual void Update();
    /// apply per-instance state prior to rendering
    virtual void ApplyState();
    /// perform rendering
    virtual void Render();

    /// return true if node has a parent
    bool HasParent() const;
    /// get parent node
    const Ptr<ModelNodeInstance>& GetParent() const;
    /// get child nodes
    const Util::Array<Ptr<ModelNodeInstance> >& GetChildren() const;
    /// return true if attached to ModelInstance
    bool IsAttachedToModelInstance() const;
    /// get the ModelInstance we are attached to
    const Ptr<ModelInstance>& GetModelInstance() const;
    /// get the ModelNode we're associated with
    const Ptr<ModelNode>& GetModelNode() const;

    /// set visible, used by charactersystem
    virtual void SetVisible(bool b);
    /// is visible
    bool IsVisible() const;
    
protected:
    friend class ModelInstance;

    /// set parent node
    void SetParent(const Ptr<ModelNodeInstance>& p);
    /// add a child node
    void AddChild(const Ptr<ModelNodeInstance>& c);
    /// called when attached to ModelInstance
    virtual void OnAttachToModelInstance(const Ptr<ModelInstance>& inst, const Ptr<ModelNode>& node, const Ptr<ModelNodeInstance>& parentNodeInst);
    /// called when removed from ModelInstance
    virtual void OnRemoveFromModelInstance();
    /// notify that we are visible
    virtual void OnNotifyVisible(IndexT frameIndex);
    /// render node specific debug shape
    virtual void RenderDebug();
    /// set visible flag of children
    void SetChildrenVisiblity(ModelNodeInstance* parent, bool b);

    Ptr<ModelInstance> modelInstance;
    Ptr<ModelNode> modelNode;
    Ptr<ModelNodeInstance> parent;
    Util::Array<Ptr<ModelNodeInstance> > children;
    bool visible;
};

//------------------------------------------------------------------------------
/**
*/
inline void
ModelNodeInstance::SetParent(const Ptr<ModelNodeInstance>& p)
{
    this->parent = p;
}

//------------------------------------------------------------------------------
/**
*/
inline bool
ModelNodeInstance::HasParent() const
{
    return this->parent.isvalid();
}

//------------------------------------------------------------------------------
/**
*/
inline const Ptr<ModelNodeInstance>&
ModelNodeInstance::GetParent() const
{
    return this->parent;
}

//------------------------------------------------------------------------------
/**
*/
inline void
ModelNodeInstance::AddChild(const Ptr<ModelNodeInstance>& c)
{
    this->children.Append(c);
}

//------------------------------------------------------------------------------
/**
*/
inline const Util::Array<Ptr<ModelNodeInstance> >&
ModelNodeInstance::GetChildren() const
{
    return this->children;
}

//------------------------------------------------------------------------------
/**
    Set visibility of node and its children, should not be call per frame!
*/
inline void 
ModelNodeInstance::SetVisible(bool b)
{
    this->visible = b;

    // recursively go thru all children and set their visiblity too
    this->SetChildrenVisiblity(this, b);
}

//------------------------------------------------------------------------------
/**
*/
inline bool 
ModelNodeInstance::IsVisible() const
{
    return this->visible;
}

} // namespace Models
//------------------------------------------------------------------------------
#endif

