//------------------------------------------------------------------------------
//  model.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "models/model.h"
#include "models/modelinstance.h"
#include "models/modelserver.h"
#include "models/visresolver.h"

namespace Models
{
ImplementClass(Models::Model, 'MODL', Resources::Resource);

using namespace Util;
using namespace Attr;
using namespace Resources;

//------------------------------------------------------------------------------
/**
*/
Model::Model() :
    inLoadResources(false)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
Model::~Model()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
bool
Model::LoadFromAttrs(const AttributeContainer& a)
{
    this->attrs = a;
    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool
Model::SaveToAttrs(AttributeContainer& a)
{
    a = this->attrs;
    return true;
}

//------------------------------------------------------------------------------
/**
*/
void
Model::Unload()
{    
    // discard model nodes
    IndexT i;
    for (i = 0; i < this->nodes.Size(); i++)
    {
        this->nodes[i]->OnRemoveFromModel();
    }
    this->nodes.Clear();
    this->nodeMap.Clear();
    this->visibleModelNodes.Reset();

    // discard instances
    for (i = 0; i < this->instances.Size(); i++)
    {
        this->instances[i]->OnRemoveFromModel();
    }
    this->instances.Clear();
        
    // call parent class
    Resource::Unload();
}

//------------------------------------------------------------------------------
/**
    This method asks all model nodes to load their resources. Note that 
    actual resource loading may be asynchronous and placeholder resources
    may be in place right after this method returns.
*/
void
Model::LoadResources()
{
    n_assert(!this->inLoadResources);
    this->inLoadResources = true;
    IndexT i;
    for (i = 0; i < this->nodes.Size(); i++)
    {
        this->nodes[i]->LoadResources();
    }
}

//------------------------------------------------------------------------------
/**
    This method asks all model nodes to unload their resources.
*/
void
Model::UnloadResources()
{
    n_assert(this->inLoadResources);
    IndexT i;
    for (i = 0; i < this->nodes.Size(); i++)
    {
        this->nodes[i]->UnloadResources();
    }
    this->inLoadResources = false;
}

//------------------------------------------------------------------------------
/**
    Checks all model nodes and returns a cumulative resource loading state.
*/
Resource::State
Model::GetResourceState() const
{
    n_assert(this->inLoadResources);
    Resource::State result = Resource::Initial;
    IndexT i;
    for (i = 0; i < this->nodes.Size(); i++)
    {
        Resource::State state = this->nodes[i]->GetResourceState();
        if (state > result)
        {
            result = state;
        }
    }
    return result;
}

//------------------------------------------------------------------------------
/**
*/
void
Model::AttachNode(const Ptr<ModelNode>& node)
{
    n_assert(node->GetName().Value().IsValid());
    n_assert(!this->HasNode(node->GetName()));
    this->nodes.Append(node);
    this->nodeMap.Add(node->GetName(), this->nodes.Size() - 1);
    node->OnAttachToModel(this);
}

//------------------------------------------------------------------------------
/**
*/
void
Model::RemoveNode(const Ptr<ModelNode>& node)
{
    IndexT nodeIndex = this->nodes.FindIndex(node);
    n_assert(InvalidIndex != nodeIndex);
    node->OnRemoveFromModel();
    this->nodeMap.Erase(node->GetName());
    this->nodes.EraseIndex(nodeIndex);
}

//------------------------------------------------------------------------------
/**
*/
void
Model::AddVisibleModelNode(IndexT frameIndex, ModelNodeType::Code t, const Ptr<ModelNode>& modelNode)
{
    this->visibleModelNodes.Add(frameIndex, t, modelNode);
    if (!this->visibleModelNodes.IsResolved(t))
    {
        this->visibleModelNodes.SetResolved(t, true);
        VisResolver::Instance()->AddVisibleModel(frameIndex, t, this);
    }
}

//------------------------------------------------------------------------------
/**
*/
Ptr<ModelInstance>
Model::CreateInstance()
{
    Ptr<ModelInstance> modelInstance = ModelInstance::Create();
    this->AttachInstance(modelInstance);
    return modelInstance;
}

//------------------------------------------------------------------------------
/**
*/
void
Model::AttachInstance(const Ptr<ModelInstance>& modelInstance)
{
    n_assert(!modelInstance->IsAttachedToModel());
    this->instances.Append(modelInstance);
    modelInstance->OnAttachToModel(this);
}

//------------------------------------------------------------------------------
/**
*/
void
Model::RemoveInstance(const Ptr<ModelInstance>& modelInstance)
{
    n_assert(modelInstance->IsAttachedToModel());
    n_assert(modelInstance->GetModel().get() == this);
    modelInstance->OnRemoveFromModel();
    IndexT index = this->instances.FindIndex(modelInstance);
    n_assert(InvalidIndex != index);
    this->instances.EraseIndex(index);
}

} // namespace Models
