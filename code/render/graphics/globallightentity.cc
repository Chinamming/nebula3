//------------------------------------------------------------------------------
//  globallightentity.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "graphics/globallightentity.h"
#include "graphics/stage.h"

namespace Graphics
{
ImplementClass(Graphics::GlobalLightEntity, 'GLEP', Graphics::AbstractLightEntity);

//------------------------------------------------------------------------------
/**
*/
GlobalLightEntity::GlobalLightEntity() :
    backLightColor(0.0f, 0.0f, 0.0f, 0.0f)
{
    this->SetLightType(Lighting::LightType::Global);
}

//------------------------------------------------------------------------------
/**
*/
void
GlobalLightEntity::Setup(const Ptr<Stage>& stage_)
{
    // call parent class
    AbstractLightEntity::Setup(stage_);

    // setup and send entity creation message
    Ptr<CreateGlobalLightEntity> msg = CreateGlobalLightEntity::Create();
    msg->SetStageName(this->stage->GetName());
    msg->SetTransform(this->transform);
    msg->SetVisible(this->isVisible);
    msg->SetLightColor(this->color);
    msg->SetProjMapUvOffsetAndScale(this->projMapUvOffsetAndScale);
    msg->SetCastShadows(this->castShadows);
    msg->SetBackLightColor(this->backLightColor);
    this->SendCreateMsg(msg.cast<CreateGraphicsEntity>());
}

} // namespace Graphics
