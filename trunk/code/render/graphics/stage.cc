//------------------------------------------------------------------------------
//  stage.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "graphics/stage.h"
#include "graphics/stagebuilder.h"
#include "graphics/cell.h"
#include "graphics/cameraentity.h"
#include "timing/timer.h"

namespace Graphics
{
ImplementClass(Graphics::Stage, 'GSTG', Core::RefCounted);

using namespace Util;

//------------------------------------------------------------------------------
/**
*/
Stage::Stage() :
    isAttachedToServer(false),
    updateEntitiesFrameCount(InvalidIndex)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
Stage::~Stage()
{
    // make sure we've been properly cleaned up
    n_assert(!this->IsAttachedToServer());
    n_assert(!this->rootCell.isvalid());
    n_assert(!this->stageBuilder.isvalid());
    n_assert(this->entities.IsEmpty());
    IndexT i;
    for (i = 0; i < GraphicsEntity::NumTypes; i++)
    {
        n_assert(this->entitiesByType[i].IsEmpty());
    }
}

//------------------------------------------------------------------------------
/**
    Setting a root cell will also initialize it.
*/
void
Stage::SetRootCell(const Ptr<Cell>& cell)
{
    n_assert(!this->rootCell.isvalid());
    this->rootCell = cell;
    this->rootCell->OnAttachToStage(this);
}

//------------------------------------------------------------------------------
/**
*/
const Ptr<Cell>&
Stage::GetRootCell() const
{
    return this->rootCell;
}

//------------------------------------------------------------------------------
/**
*/
void
Stage::SetStageBuilder(const Ptr<StageBuilder>& b)
{
    this->stageBuilder = b;
}

//------------------------------------------------------------------------------
/**
*/
const Ptr<StageBuilder>&
Stage::GetStageBuilder() const
{
    return this->stageBuilder;
}

//------------------------------------------------------------------------------
/**
*/
void
Stage::OnAttachToServer()
{
    n_assert(!this->IsAttachedToServer());
    this->isAttachedToServer = true;

    // if a stage builder has been set, ask it to initialize us,
    // otherwise require a valid root cell
    if (this->stageBuilder.isvalid())
    {
        this->stageBuilder->BuildStage(this);
    }
    n_assert(this->rootCell.isvalid());
    n_assert(this->rootCell->IsAttachedToStage());
}

//------------------------------------------------------------------------------
/**
*/
void
Stage::OnRemoveFromServer()
{
    n_assert(this->IsAttachedToServer());
    n_assert(this->rootCell.isvalid());

    // properly shutdown root cell
    n_assert(this->rootCell->IsAttachedToStage());
    this->rootCell->OnRemoveFromStage();
    this->rootCell = 0;

    // properly cleanup entities
    IndexT entityIndex;
    for (entityIndex = 0; entityIndex < this->entities.Size(); entityIndex++)
    {
        const Ptr<GraphicsEntity> curEntity = this->entities[entityIndex];
        curEntity->OnRemoveFromStage();
        curEntity->OnDeactivate();
    }
    this->entities.Clear();
    IndexT typeIndex;
    for (typeIndex = 0; typeIndex < GraphicsEntity::NumTypes; typeIndex++)
    {
        this->entitiesByType[typeIndex].Clear();
    }

    // release stage builder
    this->stageBuilder = 0;

    this->isAttachedToServer = false;
}

//------------------------------------------------------------------------------
/**
    Add an entity to the stage. The method OnAttachToStage() will be
    invoked on the entity, and the entity will be inserted into 
    the cell hierarchy of the stage (which in turn call OnAttachToCell()
    on the entity).
*/
void
Stage::AttachEntity(const Ptr<GraphicsEntity>& entity)
{
    n_assert(!entity->IsActive());
    n_assert(!entity->IsAttachedToStage());
    n_assert(entity->GetType() < GraphicsEntity::NumTypes);

    this->entities.Append(entity);
    this->entitiesByType[entity->GetType()].Append(entity);
    entity->OnActivate();
    entity->OnAttachToStage(this);
    this->rootCell->InsertEntity(entity);
}

//------------------------------------------------------------------------------
/**
    Remove an entity from the stage. This will remove the entity from
    the cell hierarchy of the stage (which invoked OnRemoveFromCell()
    on the entity), and then the method OnRemoveFromStage() will
    be called on the entity.
*/
void
Stage::RemoveEntity(const Ptr<GraphicsEntity>& entity)
{
    n_assert(entity->IsActive());
    n_assert(entity->IsAttachedToStage());
    n_assert(entity->GetStage().get() == this);
    n_assert(entity->IsAttachedToCell());
    n_assert(entity->GetType() < GraphicsEntity::NumTypes);

    // first remove entity from its cell, stage and deactivate it
    entity->GetCell()->RemoveEntity(entity);
    entity->OnRemoveFromStage();
    entity->OnDeactivate();

    IndexT entitiesIndex = this->entities.FindIndex(entity);
    n_assert(InvalidIndex != entitiesIndex);
    this->entities.EraseIndex(entitiesIndex);
    
    IndexT entitiesByTypeIndex = this->entitiesByType[entity->GetType()].FindIndex(entity);
    n_assert(InvalidIndex != entitiesByTypeIndex);
    this->entitiesByType[entity->GetType()].EraseIndex(entitiesByTypeIndex);
}

//------------------------------------------------------------------------------
/**
    Update the entities in the stage. This usually implements graphics
    animations, etc... This method must be called first after
    BeginUpdate() and before any of the visibility link update methods to 
    make sure entity transforms and bounding boxes are uptodate before resolving 
    visibility.

    FIXME: this is no good for worlds with many static entities, entities should
    know whether they need to have their Update method called, and
    register themselves somewhere!
*/
void
Stage::UpdateEntities(Timing::Time curTime, IndexT curFrameCount)
{
    // check if entities have already been updated for this frame
    if (curFrameCount != this->updateEntitiesFrameCount)
    {
        this->updateEntitiesFrameCount = curFrameCount;
        IndexT entityIndex;
        SizeT numEntities = this->entities.Size();
        for (entityIndex = 0; entityIndex < numEntities; entityIndex++)
        {
            this->entities[entityIndex]->SetTime(curTime);
            this->entities[entityIndex]->OnUpdate();
        }
    }
}

//------------------------------------------------------------------------------
/**
    Update visibility links for a given camera. This will create bidirectional
    visibility links between the camera and all other entities (most importantly
    light and model entities) which are visible through this camera. This
    method must be called once for each active camera after UpdateEntities()
    and before UpdateVisibleLightLinks().
*/
void
Stage::UpdateCameraLinks(const Ptr<CameraEntity>& cameraEntity)
{
    n_assert(cameraEntity.isvalid());

    // clear camera links in all entities
    IndexT entityIndex;
    SizeT numEntities = this->entities.Size();
    for (entityIndex = 0; entityIndex < numEntities; entityIndex++)
    {
        this->entities[entityIndex]->ClearLinks(GraphicsEntity::CameraLink);
    }

    if (cameraEntity.isvalid())
    {
        // resolve visible model and light entities from this camera
        uint entityTypeMask = (1 << GraphicsEntity::ModelType) | (1 << GraphicsEntity::LightType);
        this->rootCell->UpdateLinks(cameraEntity.upcast<GraphicsEntity>(), entityTypeMask, GraphicsEntity::CameraLink);
    }
}

//------------------------------------------------------------------------------
/**
    For each visible light entity, this method will create light links
    between the light entities, and model entities influenced by 
    this light. This method must be called after UpdateCameraLinks() (this
    makes sure that no invisible lights and models will be checked).
*/
void
Stage::UpdateLightLinks()
{
    // clear light links in all entities
    IndexT entityIndex;
    SizeT numEntities = this->entities.Size();
    for (entityIndex = 0; entityIndex < numEntities; entityIndex++)
    {
        this->entities[entityIndex]->ClearLinks(GraphicsEntity::LightLink);
    }

    // for each visible light...
    const Array<Ptr<GraphicsEntity> >& lightEntities = this->entitiesByType[GraphicsEntity::LightType];
    IndexT lightIndex;
    for (lightIndex = 0; lightIndex < lightEntities.Size(); lightIndex++)
    {
        const Ptr<GraphicsEntity>& lightEntity = lightEntities[lightIndex];
        if (lightEntity->GetLinks(GraphicsEntity::CameraLink).Size() > 0)
        {
            // find model entities influenced by this light
            this->rootCell->UpdateLinks(lightEntity, (1<<GraphicsEntity::ModelType), GraphicsEntity::LightLink);
        }
    }
}

} // namespace Graphics