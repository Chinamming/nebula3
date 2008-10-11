//------------------------------------------------------------------------------
//  transformnode.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "models/nodes/transformnode.h"
#include "models/nodes/transformnodeinstance.h"
#include "models/attributes.h"

namespace Models
{
__ImplementClass(Models::TransformNode, 'TRFN', Models::ModelNode);

using namespace Attr;
using namespace Math;

//------------------------------------------------------------------------------
/**
*/
TransformNode::TransformNode() :
    scale(1.0f, 1.0f, 1.0f)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
TransformNode::~TransformNode()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
Ptr<ModelNodeInstance>
TransformNode::CreateNodeInstance() const
{
    Ptr<ModelNodeInstance> newInst = (ModelNodeInstance*) TransformNodeInstance::Create();
    return newInst;
}

//------------------------------------------------------------------------------
/**
*/
void
TransformNode::LoadFromAttrs(const AttributeContainer& attrs)
{
    this->SetPosition(attrs.GetFloat4(Attr::Position, float4(0.0f, 0.0f, 0.0f, 1.0)));
    const float4& r = attrs.GetFloat4(Attr::Rotate, float4(0.0f, 0.0f, 0.0f, 1.0f));
    this->SetRotate(quaternion(r.x(), r.y(), r.z(), r.w()));
    this->SetScale(attrs.GetFloat4(Attr::Scale, float4(1.0f, 1.0f, 1.0f, 0.0f)));
    this->SetRotatePivot(attrs.GetFloat4(Attr::RotatePivot, float4(0.0f, 0.0f, 0.0f, 1.0)));
    this->SetScalePivot(attrs.GetFloat4(Attr::ScalePivot, float4(0.0f, 0.0f, 0.0f, 1.0f)));
    ModelNode::LoadFromAttrs(attrs);
}

//------------------------------------------------------------------------------
/**
*/
void
TransformNode::SaveToAttrs(AttributeContainer& attrs)
{
    ModelNode::SaveToAttrs(attrs);
    attrs.SetFloat4(Attr::Position, this->GetPosition());
    attrs.SetFloat4(Attr::Rotate, float4(this->rotate.x(), this->rotate.y(), this->rotate.z(), this->rotate.w()));
    attrs.SetFloat4(Attr::Scale, this->GetScale());
    attrs.SetFloat4(Attr::RotatePivot, this->GetRotatePivot());
    attrs.SetFloat4(Attr::ScalePivot, this->GetScalePivot());
}

} // namespace Models
