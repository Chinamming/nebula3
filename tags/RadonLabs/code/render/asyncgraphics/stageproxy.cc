//------------------------------------------------------------------------------
//  stageproxy.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "asyncgraphics/stageproxy.h"
#include "asyncgraphics/protocol.h"
#include "asyncgraphics/asyncgraphicsinterface.h"
#include "graphics/simplestagebuilder.h"

namespace AsyncGraphics
{
ImplementClass(AsyncGraphics::StageProxy, 'AGSP', Core::RefCounted);

using namespace Util;
using namespace Messaging;
using namespace Graphics;

//------------------------------------------------------------------------------
/**
*/
StageProxy::StageProxy() :
    stageBuilderClass(&SimpleStageBuilder::RTTI),
    stageHandle(0)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
StageProxy::~StageProxy()
{
    n_assert(!this->IsValid());
}

//------------------------------------------------------------------------------
/**
*/
void
StageProxy::Setup()
{
    n_assert(0 != this->stageBuilderClass);
    n_assert(!this->IsValid());
    n_assert(this->name.Value().IsValid());

    // send a CreateGraphicsStage message and wait for completion 
    Ptr<AsyncGraphics::CreateGraphicsStage> msg = AsyncGraphics::CreateGraphicsStage::Create();
    msg->SetName(this->name);
    msg->SetStageBuilderClass(this->stageBuilderClass);
    msg->SetStageBuilderAttrs(this->stageBuilderAttrs);
    AsyncGraphicsInterface::Instance()->SendWait(msg.cast<Message>());
    n_assert(msg->Handled());
    this->stageHandle = msg->GetResult();
    n_assert(0 != this->stageHandle);
}

//------------------------------------------------------------------------------
/**
*/
void
StageProxy::Discard()
{
    n_assert(this->IsValid());

    // send a DiscardGraphicsStage message and wait for completion
    Ptr<DiscardGraphicsStage> msg = DiscardGraphicsStage::Create();
    msg->SetStageHandle(this->stageHandle);
    AsyncGraphicsInterface::Instance()->SendWait(msg.cast<Message>());
    this->stageHandle = 0;
}

//------------------------------------------------------------------------------
/**
*/
void
StageProxy::AttachEntityProxy(const Ptr<GraphicsEntityProxy>& entityProxy)
{
    n_assert(!entityProxy->IsValid());
    n_assert(entityProxy->GetType() < GraphicsEntity::NumTypes);

    this->entityProxies.Append(entityProxy);
    this->entityProxiesByType[entityProxy->GetType()].Append(entityProxy);
    this->pendingEntityProxies.Append(entityProxy);
    entityProxy->Setup(this);
}

//------------------------------------------------------------------------------
/**
*/
void
StageProxy::RemoveEntityProxy(const Ptr<GraphicsEntityProxy>& entityProxy)
{
    n_assert(entityProxy->IsValid());
    n_assert(entityProxy->GetStageProxy().get() == this);
    n_assert(entityProxy->GetType() < GraphicsEntity::NumTypes);

    entityProxy->Discard();

    IndexT entitiesIndex = this->entityProxies.FindIndex(entityProxy);
    n_assert(InvalidIndex != entitiesIndex);
    this->entityProxies.EraseIndex(entitiesIndex);
    
    IndexT entityProxiesByTypeIndex = this->entityProxiesByType[entityProxy->GetType()].FindIndex(entityProxy);
    n_assert(InvalidIndex != entityProxiesByTypeIndex);
    this->entityProxiesByType[entityProxy->GetType()].EraseIndex(entityProxiesByTypeIndex);

    IndexT pendingEntitiesIndex = this->pendingEntityProxies.FindIndex(entityProxy);
    if (InvalidIndex != pendingEntitiesIndex)
    {
        this->pendingEntityProxies.EraseIndex(pendingEntitiesIndex);
    }
}

//------------------------------------------------------------------------------
/**
    The per-frame method. This will validate each pending entity proxies (proxies
    whose server-side entity hasn't been created yet), and remove any entity
    proxies which are valid.
*/
void
StageProxy::OnFrame()
{
    n_assert(this->IsValid());

    // validate pending entity proxies
    IndexT i;
    for (i = 0; i < this->pendingEntityProxies.Size();)
    {
        this->pendingEntityProxies[i]->ValidateEntityHandle(false);
        if (this->pendingEntityProxies[i]->IsEntityHandleValid())
        {
            // this entity is now valid, remove it from the pending array
            this->pendingEntityProxies.EraseIndex(i);
        }
        else
        {
            i++;
        }
    }
}

} // namespace AsyncGraphics
