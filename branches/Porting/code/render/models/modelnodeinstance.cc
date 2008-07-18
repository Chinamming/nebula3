//------------------------------------------------------------------------------
//  modelnodeinstance.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "models/modelnodeinstance.h"
#include "models/modelinstance.h"

namespace Models
{
ImplementClass(Models::ModelNodeInstance, 'MNDI', Core::RefCounted);

using namespace Util;

//------------------------------------------------------------------------------
/**
*/
ModelNodeInstance::ModelNodeInstance():
visible(true)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
ModelNodeInstance::~ModelNodeInstance()
{
    n_assert(!this->IsAttachedToModelInstance());
}

//------------------------------------------------------------------------------
/**
*/
bool
ModelNodeInstance::IsAttachedToModelInstance() const
{
    return this->modelInstance.isvalid();
}

//------------------------------------------------------------------------------
/**
*/
const Ptr<ModelInstance>&
ModelNodeInstance::GetModelInstance() const
{
    return this->modelInstance;
}

//------------------------------------------------------------------------------
/**
*/
const Ptr<ModelNode>&
ModelNodeInstance::GetModelNode() const
{
    return this->modelNode;
}

//------------------------------------------------------------------------------
/**
*/
void
ModelNodeInstance::OnAttachToModelInstance(const Ptr<ModelInstance>& inst, const Ptr<ModelNode>& node, const Ptr<ModelNodeInstance>& parentNodeInst)
{
    n_assert(!this->IsAttachedToModelInstance());
    this->modelInstance = inst;
    this->modelNode = node;
    this->parent = parentNodeInst;
    if (this->parent.isvalid())
    {
        this->parent->AddChild(this);
    }
}

//------------------------------------------------------------------------------
/**
*/
void
ModelNodeInstance::OnRemoveFromModelInstance()
{
    n_assert(this->IsAttachedToModelInstance());
    this->parent = 0;
    this->children.Clear();
    this->modelInstance = 0;
    this->modelNode = 0;
}

//------------------------------------------------------------------------------
/**
    The Update() method is called exactly once per frame before visibility 
    culling on every *potentially* visible model node instance. Actions 
    performed in the Update() usually include resolving the world space
    transformation (if changed) and performing animation. The Update()
    method will be called from ModelInstance::Update() in "hierarchy
    order" (parent nodes will called before their child nodes).
*/
void
ModelNodeInstance::Update()
{
    // n_printf("ModelNodeInstance::Update() called on '%s'!\n", this->modelNode->GetName().Value().AsCharPtr());
}

//------------------------------------------------------------------------------
/**
    The ApplyState() method is called when per-instance shader-state should 
    be applied. This method may be called several times per frame. The
    calling order is always in "render order", first, the ApplyState()
    method on the ModelNode will be called, then each ApplyState() and Render()
    method of the ModelNodeInstance objects.
*/
void
ModelNodeInstance::ApplyState()
{
    // n_printf("ModelNodeInstance::Apply() called on '%s'!\n", this->modelNode->GetName().Value().AsCharPtr());
}

//------------------------------------------------------------------------------
/**
    The Render() method is called when the ModelNodeInstance needs to render
    itself. There will always be a call to the Apply() method before Render()
    is called, however, Render() may be called several times per Apply()
    invokation.
*/
void
ModelNodeInstance::Render()
{
    // n_printf("ModelNodeInstance::Render() called on '%s'!\n", this->modelNode->GetName().Value().AsCharPtr());
}

//------------------------------------------------------------------------------
/**
    This method is called from the NotifyVisible() methode of our
    ModelInstance object. If the ModelNodeInstance provides
    something renderable it should respond by adding itself as
    visible node instance to its model node.
*/
void
ModelNodeInstance::OnNotifyVisible(IndexT frameIndex)
{
    // empty, override in subclass
}

//------------------------------------------------------------------------------
/**
*/
void 
ModelNodeInstance::RenderDebug()
{
   // empty, override in subclass
}


//------------------------------------------------------------------------------
/**
*/ 
void 
ModelNodeInstance::SetChildrenVisiblity(ModelNodeInstance* parent, bool b)
{
    // iterate thru children and set the same visibility
    const Util::Array<Ptr<ModelNodeInstance> >& children = parent->GetChildren();
    IndexT index;
    for (index = 0; index < children.Size(); index++)
    {
        children[index]->SetVisible(b);
        SetChildrenVisiblity(children[index], b);
    }
}
} // namespace Models
