//------------------------------------------------------------------------------
//  transformnodeinstance.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "models/modelinstance.h"
#include "models/nodes/transformnodeinstance.h"
#include "models/nodes/transformnode.h"
#include "coregraphics/transformdevice.h"

namespace Models
{
ImplementClass(Models::TransformNodeInstance, 'TFNI', Models::ModelNodeInstance);

using namespace Math;
using namespace CoreGraphics;

//------------------------------------------------------------------------------
/**
*/
TransformNodeInstance::TransformNodeInstance()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
TransformNodeInstance::~TransformNodeInstance()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
void
TransformNodeInstance::OnAttachToModelInstance(const Ptr<ModelInstance>& inst, 
                                               const Ptr<ModelNode>& node, 
                                               const Ptr<ModelNodeInstance>& parentNodeInst)
{
    n_assert(inst.isvalid());
    n_assert(node.isvalid() && node->IsA(TransformNode::RTTI));
    n_assert(!this->parentTransformNodeInstance.isvalid());
    const Ptr<TransformNode>& transformNode = node.downcast<TransformNode>();

    // instantiate transform node attributes
    this->SetPosition(transformNode->GetPosition());
    this->SetRotate(transformNode->GetRotate());
    this->SetScale(transformNode->GetScale());
    this->SetRotatePivot(transformNode->GetRotatePivot());
    this->SetScalePivot(transformNode->GetScalePivot());

    // check if parent is a transform node, if yes, store a pointer here
    // so we don't need to do this check again inside Update()
    if (parentNodeInst.isvalid() && parentNodeInst->IsA(TransformNodeInstance::RTTI))
    {
        this->parentTransformNodeInstance = parentNodeInst.downcast<TransformNodeInstance>();
    }

    // hand to parent class
    ModelNodeInstance::OnAttachToModelInstance(inst, node, parentNodeInst);
}

//------------------------------------------------------------------------------
/**
*/
void
TransformNodeInstance::OnRemoveFromModelInstance()
{
    // need to clear smart pointers to prevent ref leaks
    this->parentTransformNodeInstance = 0;
    ModelNodeInstance::OnRemoveFromModelInstance();
}

//------------------------------------------------------------------------------
/**
    The update method should first invoke any animators which change 
    per-instance attributes (this is done in the parent class). Then the
    local space transforms must be flattened into model space.
*/
void
TransformNodeInstance::Update()
{
    // flatten transforms
    // @todo: do some sort of identity matrix and dirty handling
    if (this->parentTransformNodeInstance.isvalid())
    {
        const matrix44& parentTransform = this->parentTransformNodeInstance->GetModelTransform();
        const matrix44& localTransform = this->GetLocalTransform();
        this->modelTransform = matrix44::multiply(localTransform, parentTransform);    
    }
    else
    {
        // we have no parent
        this->modelTransform = matrix44::multiply(this->GetLocalTransform(), this->modelInstance->GetTransform());
    }
}

//------------------------------------------------------------------------------
/**
    Set our model matrix (computed in the Update() method) 
    as current model matrix in the TransformDevice.
*/
void
TransformNodeInstance::ApplyState()
{
    TransformDevice::Instance()->SetModelTransform(this->modelTransform);
}

} // namespace Models
