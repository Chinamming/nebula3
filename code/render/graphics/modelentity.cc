//------------------------------------------------------------------------------
//  modelentity.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "graphics/modelentity.h"
#include "graphics/stage.h"

namespace Graphics
{
__ImplementClass(Graphics::ModelEntity, 'MDLE', Graphics::GraphicsEntity);

//------------------------------------------------------------------------------
/**
*/
ModelEntity::ModelEntity()
{
    this->SetType(GraphicsEntityType::Model);
}

//------------------------------------------------------------------------------
/**
*/
ModelEntity::~ModelEntity()
{
    // empty
}

//------------------------------------------------------------------------------
/**
    Setup the server-side ModelEntity.
*/
void
ModelEntity::Setup(const Ptr<Stage>& stage_)
{
    n_assert(this->resId.Value().IsValid());

    // call parent class
    GraphicsEntity::Setup(stage_);

    // setup and send an entity creation message
    Ptr<CreateModelEntity> msg = CreateModelEntity::Create();
    msg->SetStageName(this->stage->GetName());
    msg->SetTransform(this->transform);
    msg->SetVisible(this->isVisible);
    msg->SetResourceId(this->resId);
    this->SendCreateMsg(msg.cast<CreateGraphicsEntity>());
}

//------------------------------------------------------------------------------
/**
*/
void
ModelEntity::SetShaderVariable(const Util::String& nodeName, const Util::String& variableSemantic, const Util::Variant& value)
{
    Ptr<UpdateInstanceShaderVariable> msg = UpdateInstanceShaderVariable::Create();
    msg->SetNodeInstanceName(nodeName);
    msg->SetSemantic(variableSemantic);
    msg->SetValue(value);
    this->SendMsg(msg.cast<GraphicsEntityMsg>());
}

} // namespace Graphics