#pragma once
#ifndef MODELS_MODEL_H
#define MODELS_MODEL_H
//------------------------------------------------------------------------------
/**
    @class Models::Model
    
    A Model represents the template for a renderable object, consisting
    of a hierarchy of ModelNodes which represent transformations and shapes.
    Models should generally be created through the ModelServer, which 
    guarantees that a given Model is only loaded once into memory. To
    render a Model, at least one ModelInstance must be created from the
    Model. Usually one ModelInstance is created per game object. Generally
    speaking, all per-instance data lives in the ModelInstance objects, while
    all constant data lives in the Model object. 

    (C) 2007 Radon Labs GmbH
*/
#include "resources/resource.h"
#include "math/bbox.h"
#include "models/modelnode.h"
#include "attr/attributecontainer.h"
#include "models/visresolvecontainer.h"

//------------------------------------------------------------------------------
namespace Models
{
class ModelReader;
class ModelWriter;
class ModelInstance;

class Model : public Resources::Resource
{
    __DeclareClass(Model);
public:
    /// constructor
    Model();
    /// destructor
    virtual ~Model();

    /// unload the resource, or cancel the pending load
    virtual void Unload();

    /// load node resources (meshes, textures, shaders, ...)
    void LoadResources();
    /// unload node resources
    void UnloadResources();
    /// get the overall resource loading state (Initial -> Pending -> Loaded/Failed/Cancelled)
    State GetResourceState() const;

    /// set the model's local bounding box
    void SetBoundingBox(const Math::bbox& b);
    /// get the model's local bounding box
    const Math::bbox& GetBoundingBox() const;

    /// check if a ModelNode exists
    bool HasNode(const Util::Atom<Util::String>& name) const;
    /// lookup a ModelNode in the Model
    const Ptr<ModelNode>& LookupNode(const Util::Atom<Util::String>& name) const;
    /// attach a model node to the Model
    void AttachNode(const Ptr<ModelNode>& node);
    /// remove a model node from the Model
    void RemoveNode(const Ptr<ModelNode>& node);
    /// access to model nodes
    const Util::Array<Ptr<ModelNode> >& GetNodes() const;

    /// create a ModelInstance of the Model
    Ptr<ModelInstance> CreateInstance();
    /// get all attached model instances
    const Util::Array<Ptr<ModelInstance> >& GetInstances() const;

    /// load model attributes from attribute set
    bool LoadFromAttrs(const Attr::AttributeContainer& attrs);
    /// save model attributes to attribute set
    bool SaveToAttrs(Attr::AttributeContainer& attrs);
    /// read access to model attributes
    const Attr::AttributeContainer& GetAttrs() const;
    /// check if model attribute exists
    bool HasAttr(const Attr::AttrId& attrId) const;
    /// set generic model attribute
    void SetAttr(const Attr::Attribute& attr);
    /// get generic model attribute
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

private:
    friend class VisResolver;
    friend class ModelServer;
    friend class ModelInstance;
    friend class ModelNode;

    /// attach a ModelInstance to the Model
    void AttachInstance(const Ptr<ModelInstance>& inst);
    /// remove a ModelInstance from the Model
    void RemoveInstance(const Ptr<ModelInstance>& inst);
    /// get visible model nodes
    const Util::Array<Ptr<ModelNode> >& GetVisibleModelNodes(ModelNodeType::Code t) const;
    /// lookup node index by name
    IndexT GetNodeIndexByName(const Util::Atom<Util::String>& name) const;
    /// called by ModelNode as a result of NotifyVisible()
    void AddVisibleModelNode(IndexT frameIndex, ModelNodeType::Code t, const Ptr<ModelNode>& modelNode);

    Math::bbox boundingBox;
    Util::Array<Ptr<ModelNode> > nodes;
    Util::Dictionary<Util::Atom<Util::String>, IndexT> nodeMap;  // map names to model nodes
    Util::Array<Ptr<ModelInstance> > instances;
    Attr::AttributeContainer attrs;
    VisResolveContainer<ModelNode> visibleModelNodes;
    bool inLoadResources;
};

//------------------------------------------------------------------------------
/**
*/
inline void
Model::SetBoundingBox(const Math::bbox& b)
{
    this->boundingBox = b;
}

//------------------------------------------------------------------------------
/**
*/
inline const Math::bbox&
Model::GetBoundingBox() const
{
    return this->boundingBox;
}

//------------------------------------------------------------------------------
/**
*/
inline bool
Model::HasNode(const Util::Atom<Util::String>& name) const
{
    return this->nodeMap.Contains(name);
}

//------------------------------------------------------------------------------
/**
*/
inline const Ptr<ModelNode>&
Model::LookupNode(const Util::Atom<Util::String>& name) const
{
    n_assert(this->HasNode(name));
    return this->nodes[this->nodeMap[name]];
}

//------------------------------------------------------------------------------
/**
*/
inline const Util::Array<Ptr<ModelNode> >&
Model::GetNodes() const
{
    return this->nodes;
}

//------------------------------------------------------------------------------
/**
*/
inline IndexT
Model::GetNodeIndexByName(const Util::Atom<Util::String>& name) const
{
    return this->nodeMap[name];
}

//------------------------------------------------------------------------------
/**
*/
inline const Attr::AttributeContainer&
Model::GetAttrs() const
{
    return this->attrs;
}

//------------------------------------------------------------------------------
/**
*/
inline bool
Model::HasAttr(const Attr::AttrId& attrId) const
{
    return this->attrs.HasAttr(attrId);
}

//------------------------------------------------------------------------------
/**
*/
inline void
Model::SetAttr(const Attr::Attribute& attr)
{
    this->attrs.SetAttr(attr);
}

//------------------------------------------------------------------------------
/**
*/
inline const Attr::Attribute&
Model::GetAttr(const Attr::AttrId& attrId) const
{
    return this->attrs.GetAttr(attrId);
}

//------------------------------------------------------------------------------
/**
*/
inline void
Model::SetBool(const Attr::BoolAttrId& attrId, bool val)
{
    this->attrs.SetBool(attrId, val);
}

//------------------------------------------------------------------------------
/**
*/
inline bool
Model::GetBool(const Attr::BoolAttrId& attrId) const
{
    return this->attrs.GetBool(attrId);
}

//------------------------------------------------------------------------------
/**
*/
inline void
Model::SetFloat(const Attr::FloatAttrId& attrId, float val)
{
    this->attrs.SetFloat(attrId, val);
}

//------------------------------------------------------------------------------
/**
*/
inline float
Model::GetFloat(const Attr::FloatAttrId& attrId) const
{
    return this->attrs.GetFloat(attrId);
}

//------------------------------------------------------------------------------
/**
*/
inline void
Model::SetInt(const Attr::IntAttrId& attrId, int val)
{
    this->attrs.SetInt(attrId, val);
}

//------------------------------------------------------------------------------
/**
*/
inline int
Model::GetInt(const Attr::IntAttrId& attrId) const
{
    return this->attrs.GetInt(attrId);
}

//------------------------------------------------------------------------------
/**
*/
inline void
Model::SetString(const Attr::StringAttrId& attrId, const Util::String& val)
{
    this->attrs.SetString(attrId, val);
}

//------------------------------------------------------------------------------
/**
*/
inline const Util::String&
Model::GetString(const Attr::StringAttrId& attrId) const
{
    return this->attrs.GetString(attrId);
}

//------------------------------------------------------------------------------
/**
*/
inline void
Model::SetFloat4(const Attr::Float4AttrId& attrId, const Math::float4& val)
{
    this->attrs.SetFloat4(attrId, val);
}

//------------------------------------------------------------------------------
/**
*/
inline Math::float4
Model::GetFloat4(const Attr::Float4AttrId& attrId) const
{
    return this->attrs.GetFloat4(attrId);
}

//------------------------------------------------------------------------------
/**
*/
inline void
Model::SetMatrix44(const Attr::Matrix44AttrId& attrId, const Math::matrix44& val)
{
    this->attrs.SetMatrix44(attrId, val);
}

//------------------------------------------------------------------------------
/**
*/
inline const Math::matrix44&
Model::GetMatrix44(const Attr::Matrix44AttrId& attrId) const
{
    return this->attrs.GetMatrix44(attrId);
}

//------------------------------------------------------------------------------
/**
*/
inline void
Model::SetGuid(const Attr::GuidAttrId& attrId, const Util::Guid& val)
{
    this->attrs.SetGuid(attrId, val);
}

//------------------------------------------------------------------------------
/**
*/
inline const Util::Guid&
Model::GetGuid(const Attr::GuidAttrId& attrId) const
{
    return this->attrs.GetGuid(attrId);
}

//------------------------------------------------------------------------------
/**
*/
inline void
Model::SetBlob(const Attr::BlobAttrId& attrId, const Util::Blob& val)
{
    this->attrs.SetBlob(attrId, val);
}

//------------------------------------------------------------------------------
/**
*/
inline const Util::Blob&
Model::GetBlob(const Attr::BlobAttrId& attrId) const
{
    return this->attrs.GetBlob(attrId);
}

//------------------------------------------------------------------------------
/**
*/
inline const Util::Array<Ptr<ModelNode> >&
Model::GetVisibleModelNodes(ModelNodeType::Code t) const
{
    return this->visibleModelNodes.Get(t);
}

//------------------------------------------------------------------------------
/**
*/
inline const Util::Array<Ptr<ModelInstance> >&
Model::GetInstances() const
{
    return this->instances;
}

} // namespace Models
//------------------------------------------------------------------------------
#endif

    