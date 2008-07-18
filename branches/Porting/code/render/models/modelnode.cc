//------------------------------------------------------------------------------
//  modelnode.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "models/attributes.h"
#include "models/modelnode.h"
#include "models/model.h"
#include "models/modelnodeinstance.h"

namespace Models
{
ImplementClass(Models::ModelNode, 'MDND', Core::RefCounted);

using namespace Resources;

//------------------------------------------------------------------------------
/**
*/
ModelNode::ModelNode() :
    type(ModelNodeType::Solid),
    inLoadResources(false)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
ModelNode::~ModelNode()
{
    n_assert(!this->IsAttachedToModel());
    n_assert(!this->inLoadResources);
}

//------------------------------------------------------------------------------
/**
*/
bool
ModelNode::IsAttachedToModel() const
{
    return this->model.isvalid();
}

//------------------------------------------------------------------------------
/**
*/
const Ptr<Model>&
ModelNode::GetModel() const
{
    return this->model;
}

//------------------------------------------------------------------------------
/**
*/
Ptr<ModelNodeInstance>
ModelNode::CreateNodeInstance() const
{
    n_error("ModelNode::CreateNodeInstance() called!");
    return Ptr<ModelNodeInstance>();
}

//------------------------------------------------------------------------------
/**
*/
void
ModelNode::OnAttachToModel(const Ptr<Model>& m)
{
    n_assert(!this->IsAttachedToModel());
    this->model = m;
}

//------------------------------------------------------------------------------
/**
*/
void
ModelNode::OnRemoveFromModel()
{
    n_assert(this->IsAttachedToModel());
    if (this->inLoadResources)
    {
        this->UnloadResources();
    }
    this->model = 0;
    this->parent = 0;
    this->children.Clear();
    this->visibleModelNodeInstances.Reset();
}

//------------------------------------------------------------------------------
/**
*/
void
ModelNode::LoadFromAttrs(const Attr::AttributeContainer& a)
{
    this->attrs = a;

    if (this->attrs.HasAttr(Attr::BoxCenter) && this->attrs.HasAttr(Attr::BoxExtents))
    {
        // setup bounding box
        this->boundingBox.set(this->attrs.GetFloat4(Attr::BoxCenter), this->attrs.GetFloat4(Attr::BoxExtents));
    }
}

//------------------------------------------------------------------------------
/**
*/
void
ModelNode::SaveToAttrs(Attr::AttributeContainer& a)
{
    a = this->attrs;
}

//------------------------------------------------------------------------------
/**
    This method is called when required resources should be loaded.
*/
void
ModelNode::LoadResources()
{
    n_assert(!this->inLoadResources);
    this->inLoadResources = true;
}

//------------------------------------------------------------------------------
/**
    This method is called when required resources should be unloaded.
*/
void
ModelNode::UnloadResources()
{
    n_assert(this->inLoadResources);
    this->inLoadResources = false;
}

//------------------------------------------------------------------------------
/**
    Returns the overall resource state (Initial, Loaded, Pending,
    Failed, Cancelled). Higher states override lower state (if some resources
    are already Loaded, and some are Pending, then Pending will be returned,
    if some resources failed to load, then Failed will be returned, etc...).
    Subclasses which load resource must override this method, and modify
    the return value of the parent class as needed).
*/
Resource::State
ModelNode::GetResourceState() const
{
    return Resource::Initial;
}

//------------------------------------------------------------------------------
/**
    This method is called once before rendering the ModelNode's visible 
    instance nodes through the ModelNodeInstance::ApplyState() and 
    ModelNodeInstance::Render() methods. The method must apply the 
    shader state that is shared across all instances. Since this state is
    constant across all instance nodes, this only happens once before
    rendering an instance set.
*/
void
ModelNode::ApplySharedState()
{
    // n_printf("ModelNode::ApplySharedState() called on '%s'!\n", this->GetName().Value().AsCharPtr());
}

//------------------------------------------------------------------------------
/**
*/
void
ModelNode::AddVisibleNodeInstance(IndexT frameIndex, const Ptr<ModelNodeInstance>& nodeInst)
{
    this->visibleModelNodeInstances.Add(frameIndex, this->type, nodeInst);
    if (!this->visibleModelNodeInstances.IsResolved(this->type))
    {
        this->visibleModelNodeInstances.SetResolved(this->type, true);
        this->model->AddVisibleModelNode(frameIndex, this->type, this);
    }
}

} // namespace Models