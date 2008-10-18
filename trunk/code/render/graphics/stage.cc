//------------------------------------------------------------------------------
//  stage.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "graphics/stage.h"
#include "graphics/graphicsprotocol.h"
#include "graphics/graphicsinterface.h"
#include "graphics/graphicsentitytype.h"
#include "internalgraphics/simplestagebuilder.h"

namespace Graphics
{
__ImplementClass(Graphics::Stage, 'GSTG', Core::RefCounted);

using namespace Util;
using namespace Messaging;

//------------------------------------------------------------------------------
/**
*/
Stage::Stage() :
    stageBuilderClass(&InternalGraphics::SimpleStageBuilder::RTTI),
    stageHandle(0)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
Stage::~Stage()
{
    n_assert(!this->IsValid());
}

//------------------------------------------------------------------------------
/**
*/
void
Stage::Setup()
{
    n_assert(0 != this->stageBuilderClass);
    n_assert(!this->IsValid());
    n_assert(this->name.Value().IsValid());

    // send a CreateGraphicsStage message and wait for completion 
    Ptr<Graphics::CreateGraphicsStage> msg = Graphics::CreateGraphicsStage::Create();
    msg->SetName(this->name);
    msg->SetStageBuilderClass(this->stageBuilderClass);
    msg->SetStageBuilderAttrs(this->stageBuilderAttrs);
    GraphicsInterface::Instance()->SendWait(msg.cast<Message>());
    n_assert(msg->Handled());
    this->stageHandle = msg->GetResult();
    n_assert(0 != this->stageHandle);
}

//------------------------------------------------------------------------------
/**
*/
void
Stage::Discard()
{
    n_assert(this->IsValid());

    // send a DiscardGraphicsStage message and wait for completion
    Ptr<DiscardGraphicsStage> msg = DiscardGraphicsStage::Create();
    msg->SetStageHandle(this->stageHandle);
    GraphicsInterface::Instance()->SendWait(msg.cast<Message>());
    this->stageHandle = 0;
}

//------------------------------------------------------------------------------
/**
*/
void
Stage::AttachEntity(const Ptr<GraphicsEntity>& entity)
{
    n_assert(!entity->IsValid());
    n_assert(entity->GetType() < GraphicsEntityType::NumTypes);

    this->entities.Append(entity);
    this->entitiesByType[entity->GetType()].Append(entity);
    this->pendingEntities.Append(entity);
    entity->Setup(this);
}

//------------------------------------------------------------------------------
/**
*/
void
Stage::RemoveEntity(const Ptr<GraphicsEntity>& entity)
{
    n_assert(entity->IsValid());
    n_assert(entity->GetStage().get() == this);
    n_assert(entity->GetType() < GraphicsEntityType::NumTypes);

    entity->Discard();

    IndexT entitiesIndex = this->entities.FindIndex(entity);
    n_assert(InvalidIndex != entitiesIndex);
    this->entities.EraseIndex(entitiesIndex);
    
    IndexT entitiesByTypeIndex = this->entitiesByType[entity->GetType()].FindIndex(entity);
    n_assert(InvalidIndex != entitiesByTypeIndex);
    this->entitiesByType[entity->GetType()].EraseIndex(entitiesByTypeIndex);

    IndexT pendingEntitiesIndex = this->pendingEntities.FindIndex(entity);
    if (InvalidIndex != pendingEntitiesIndex)
    {
        this->pendingEntities.EraseIndex(pendingEntitiesIndex);
    }
}

//------------------------------------------------------------------------------
/**
    The per-frame method. This will validate each pending entity (an entity
    whose server-side internal entity hasn't been created yet).
*/
void
Stage::OnFrame()
{
    n_assert(this->IsValid());

    // validate pending entities
    IndexT i;
    for (i = 0; i < this->pendingEntities.Size();)
    {
        this->pendingEntities[i]->ValidateEntityHandle(false);
        if (this->pendingEntities[i]->IsEntityHandleValid())
        {
            // this entity is now valid, remove it from the pending array
            this->pendingEntities.EraseIndex(i);
        }
        else
        {
            i++;
        }
    }
}

} // namespace Graphics
