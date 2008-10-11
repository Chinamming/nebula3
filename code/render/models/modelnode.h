#pragma once
#ifndef MODELS_MODELNODE_H
#define MODELS_MODELNODE_H
//------------------------------------------------------------------------------
/**
    @class Models::ModelNode
  
    Represents a transformation hierarchy element inside a Model. Subclasses
    of ModelNodes represent transformations and geometry of a 3D model
    arranged in 3d hierarchy (but not in a logical hierarchy of C++ object,
    instead model nodes live in a flat array to prevent recursive iteration).
    
    A ModelNode is roughly comparable to a nSceneNode in Nebula2.

    (C) 2007 Radon Labs GmbH
*/
#include "core/refcounted.h"
#include "attr/attributecontainer.h"
#include "util/atom.h"
#include "resources/resource.h"
#include "models/modelnodetype.h"
#include "models/visresolvecontainer.h"
#include "math/bbox.h"

//------------------------------------------------------------------------------
namespace Models
{
class Model;
class ModelNodeInstance;

class ModelNode : public Core::RefCounted
{
    __DeclareClass(ModelNode);
public:
    /// constructor
    ModelNode();
    /// destructor
    virtual ~ModelNode();

    /// apply state shared by all my ModelNodeInstances
    virtual void ApplySharedState();

    /// get overall state of contained resources (Initial, Loaded, Pending, Failed, Cancelled)
    virtual Resources::Resource::State GetResourceState() const;
    /// get model node name
    const Util::Atom<Util::String>& GetName() const;
    /// get the ModelNodeType
    ModelNodeType::Code GetType() const;
    /// return true if node has a parent
    bool HasParent() const;
    /// get parent node
    const Ptr<ModelNode>& GetParent() const;
    /// get child nodes
    const Util::Array<Ptr<ModelNode> >& GetChildren() const;
    /// return true if currently attached to a Model
    bool IsAttachedToModel() const;
    /// get model this node is attached to
    const Ptr<Model>& GetModel() const;
    /// get bounding box of model node
    const Math::bbox& GetBoundingBox() const;

    /// read access to model node attributes
    const Attr::AttributeContainer& GetAttrs() const;
    /// check if model node attribute exists
    bool HasAttr(const Attr::AttrId& attrId) const;
    /// set generic model node attribute
    void SetAttr(const Attr::Attribute& attr);
    /// get generic model node attribute
    const Attr::Attribute& GetAttr(const Attr::AttrId& attrId) const;
    /// set bool value
    void SetBool(const Attr::BoolAttrId& attrId, bool val);
    /// get bool value
    bool GetBool(const Attr::BoolAttrId& attrId) const;
    /// set float value
    void SetFloat(const Attr::FloatAttrId& attrId, float val);
    /// get float value
    float GetFloat(const Attr::FloatAttrId& attrId) const;
    /// set int value
    void SetInt(const Attr::IntAttrId& attrId, int val);
    /// get int value
    int GetInt(const Attr::IntAttrId& attrId) const;
    /// set string value
    void SetString(const Attr::StringAttrId& attrId, const Util::String& val);
    /// get string value
    const Util::String& GetString(const Attr::StringAttrId& attrId) const;
    /// set float4 value
    void SetFloat4(const Attr::Float4AttrId& attrId, const Math::float4& val);
    /// get float4 value
    Math::float4 GetFloat4(const Attr::Float4AttrId& attrId) const;
    /// set matrix44 value
    void SetMatrix44(const Attr::Matrix44AttrId& attrId, const Math::matrix44& val);
    /// get matrix44 value
    const Math::matrix44& GetMatrix44(const Attr::Matrix44AttrId& attrId) const;
    /// set guid value
    void SetGuid(const Attr::GuidAttrId& attrId, const Util::Guid& guid);
    /// get guid value
    const Util::Guid& GetGuid(const Attr::GuidAttrId& attrId) const;
    /// set blob value
    void SetBlob(const Attr::BlobAttrId& attrId, const Util::Blob& blob);
    /// get blob value
    const Util::Blob& GetBlob(const Attr::BlobAttrId& attrId) const;

    /// set model node name
    void SetName(const Util::Atom<Util::String>& n);
    /// set ModelNodeType
    void SetType(ModelNodeType::Code t);
    /// set parent node
    void SetParent(const Ptr<ModelNode>& p);
    /// add a child node
    void AddChild(const Ptr<ModelNode>& c);
    /// called to initialize from attributes
    virtual void LoadFromAttrs(const Attr::AttributeContainer& attrs);

protected:
    friend class ModelServer;
    friend class Model;
    friend class ModelInstance;
    friend class ModelNodeInstance;
    friend class StreamModelLoader;
    friend class VisResolver;
    
    /// set bounding box
    void SetBoundingBox(const Math::bbox& b);
    /// create a model node instance
    virtual Ptr<ModelNodeInstance> CreateNodeInstance() const;
    /// called when attached to model node
    virtual void OnAttachToModel(const Ptr<Model>& model);
    /// called when removed from model node
    virtual void OnRemoveFromModel();
    /// called to save state back to attributes
    virtual void SaveToAttrs(Attr::AttributeContainer& attrs);
    /// called when resources should be loaded
    virtual void LoadResources();
    /// called when resources should be unloaded
    virtual void UnloadResources();

    /// get visible model node instances
    const Util::Array<Ptr<ModelNodeInstance> >& GetVisibleModelNodeInstances(ModelNodeType::Code t) const;

public: // PRIVATE!!! may only be called by subclassed of ModelNodeInstance
    /// called by model node instance on NotifyVisible()
    void AddVisibleNodeInstance(IndexT frameIndex, const Ptr<ModelNodeInstance>& nodeInst);

protected:
    Ptr<Model> model;
    Util::Atom<Util::String> name;
    Ptr<ModelNode> parent;
    Util::Array<Ptr<ModelNode> > children;
    Attr::AttributeContainer attrs;
    VisResolveContainer<ModelNodeInstance> visibleModelNodeInstances;
    ModelNodeType::Code type;
    Math::bbox boundingBox;
    bool inLoadResources;
};

//------------------------------------------------------------------------------
/**
*/
inline void
ModelNode::SetType(ModelNodeType::Code t)
{
    this->type = t;
}

//------------------------------------------------------------------------------
/**
*/
inline ModelNodeType::Code
ModelNode::GetType() const
{
    return this->type;
}

//------------------------------------------------------------------------------
/**
*/
inline void
ModelNode::SetName(const Util::Atom<Util::String>& n)
{
	Util::String name2 =n.Value();
    this->name = n;
}

//------------------------------------------------------------------------------
/**
*/
inline const Util::Atom<Util::String>&
ModelNode::GetName() const
{
    return this->name;
}

//------------------------------------------------------------------------------
/**
*/
inline void
ModelNode::SetParent(const Ptr<ModelNode>& p)
{
    this->parent = p;
}

//------------------------------------------------------------------------------
/**
*/
inline bool
ModelNode::HasParent() const
{
    return this->parent.isvalid();
}

//------------------------------------------------------------------------------
/**
*/
inline const Ptr<ModelNode>&
ModelNode::GetParent() const
{
    return this->parent;
}

//------------------------------------------------------------------------------
/**
*/
inline void
ModelNode::AddChild(const Ptr<ModelNode>& c)
{
    this->children.Append(c);
}

//------------------------------------------------------------------------------
/**
*/
inline const Util::Array<Ptr<ModelNode> >&
ModelNode::GetChildren() const
{
    return this->children;
}

//------------------------------------------------------------------------------
/**
*/
inline const Attr::AttributeContainer&
ModelNode::GetAttrs() const
{
    return this->attrs;
}

//------------------------------------------------------------------------------
/**
*/
inline bool
ModelNode::HasAttr(const Attr::AttrId& attrId) const
{
    return this->attrs.HasAttr(attrId);
}

//------------------------------------------------------------------------------
/**
*/
inline void
ModelNode::SetAttr(const Attr::Attribute& attr)
{
    this->attrs.SetAttr(attr);
}

//------------------------------------------------------------------------------
/**
*/
inline const Attr::Attribute&
ModelNode::GetAttr(const Attr::AttrId& attrId) const
{
    return this->attrs.GetAttr(attrId);
}

//------------------------------------------------------------------------------
/**
*/
inline void
ModelNode::SetBool(const Attr::BoolAttrId& attrId, bool val)
{
    this->attrs.SetBool(attrId, val);
}

//------------------------------------------------------------------------------
/**
*/
inline bool
ModelNode::GetBool(const Attr::BoolAttrId& attrId) const
{
    return this->attrs.GetBool(attrId);
}

//------------------------------------------------------------------------------
/**
*/
inline void
ModelNode::SetFloat(const Attr::FloatAttrId& attrId, float val)
{
    this->attrs.SetFloat(attrId, val);
}

//------------------------------------------------------------------------------
/**
*/
inline float
ModelNode::GetFloat(const Attr::FloatAttrId& attrId) const
{
    return this->attrs.GetFloat(attrId);
}

//------------------------------------------------------------------------------
/**
*/
inline void
ModelNode::SetInt(const Attr::IntAttrId& attrId, int val)
{
    this->attrs.SetInt(attrId, val);
}

//------------------------------------------------------------------------------
/**
*/
inline int
ModelNode::GetInt(const Attr::IntAttrId& attrId) const
{
    return this->attrs.GetInt(attrId);
}

//------------------------------------------------------------------------------
/**
*/
inline void
ModelNode::SetString(const Attr::StringAttrId& attrId, const Util::String& val)
{
    this->attrs.SetString(attrId, val);
}

//------------------------------------------------------------------------------
/**
*/
inline const Util::String&
ModelNode::GetString(const Attr::StringAttrId& attrId) const
{
    return this->attrs.GetString(attrId);
}

//------------------------------------------------------------------------------
/**
*/
inline void
ModelNode::SetFloat4(const Attr::Float4AttrId& attrId, const Math::float4& val)
{
    this->attrs.SetFloat4(attrId, val);
}

//------------------------------------------------------------------------------
/**
*/
inline Math::float4
ModelNode::GetFloat4(const Attr::Float4AttrId& attrId) const
{
    return this->attrs.GetFloat4(attrId);
}

//------------------------------------------------------------------------------
/**
*/
inline void
ModelNode::SetMatrix44(const Attr::Matrix44AttrId& attrId, const Math::matrix44& val)
{
    this->attrs.SetMatrix44(attrId, val);
}

//------------------------------------------------------------------------------
/**
*/
inline const Math::matrix44&
ModelNode::GetMatrix44(const Attr::Matrix44AttrId& attrId) const
{
    return this->attrs.GetMatrix44(attrId);
}

//------------------------------------------------------------------------------
/**
*/
inline void
ModelNode::SetGuid(const Attr::GuidAttrId& attrId, const Util::Guid& val)
{
    this->attrs.SetGuid(attrId, val);
}

//------------------------------------------------------------------------------
/**
*/
inline const Util::Guid&
ModelNode::GetGuid(const Attr::GuidAttrId& attrId) const
{
    return this->attrs.GetGuid(attrId);
}

//------------------------------------------------------------------------------
/**
*/
inline void
ModelNode::SetBlob(const Attr::BlobAttrId& attrId, const Util::Blob& val)
{
    this->attrs.SetBlob(attrId, val);
}

//------------------------------------------------------------------------------
/**
*/
inline const Util::Blob&
ModelNode::GetBlob(const Attr::BlobAttrId& attrId) const
{
    return this->attrs.GetBlob(attrId);
}

//------------------------------------------------------------------------------
/**
*/
inline const Util::Array<Ptr<ModelNodeInstance> >&
ModelNode::GetVisibleModelNodeInstances(ModelNodeType::Code t) const
{
    return this->visibleModelNodeInstances.Get(t);
}

//------------------------------------------------------------------------------
/**
*/
inline void
ModelNode::SetBoundingBox(const Math::bbox& b)
{
    this->boundingBox = b;
}

//------------------------------------------------------------------------------
/**
*/
inline const Math::bbox&
ModelNode::GetBoundingBox() const
{
    return this->boundingBox;
}

} // namespace Models
//------------------------------------------------------------------------------
#endif
