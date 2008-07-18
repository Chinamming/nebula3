//------------------------------------------------------------------------------
//  graphicsentity.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "graphics/graphicsentity.h"
#include "graphics/stage.h"
#include "graphics/cell.h"

namespace Graphics
{
ImplementClass(Graphics::GraphicsEntity, 'GREN', Core::RefCounted);

using namespace Util;
using namespace Math;

//------------------------------------------------------------------------------
/**
*/
GraphicsEntity::GraphicsEntity() :
    type(InvalidType),
    isActive(false),
    isValid(false),
    isVisible(true),
    globalBoxDirty(true),
    boundingBoxChanged(false),
    transformChanged(false),
    transform(matrix44::identity())
{
    this->links.SetSize(NumLinkTypes);
    // empty
}

//------------------------------------------------------------------------------
/**
*/
GraphicsEntity::~GraphicsEntity()
{
    n_assert(!this->IsActive());
}

//------------------------------------------------------------------------------
/**
    Activate the entity. This method is called when the entity
    is created an attached to the graphics server. During OnActivate()
    the entity should perform any one-time initializations.
*/
void
GraphicsEntity::OnActivate()
{
    n_assert(!this->IsActive());
    this->isActive = true;
    this->isValid = true;
}

//------------------------------------------------------------------------------
/**
    Deactivate the entity, this method is called when the entity
    is removed from the graphics server. Any initialization done
    in OnActivate() should be undone here.
*/
void
GraphicsEntity::OnDeactivate()
{
    n_assert(this->IsActive());
    if (this->IsAttachedToStage())
    {
        this->stage->RemoveEntity(this);
    }
    n_assert(!this->IsAttachedToStage());
    n_assert(!this->IsAttachedToCell());
    IndexT i;
    for (i = 0; i < NumLinkTypes; i++)
    {
        n_assert(this->links[i].IsEmpty());
    }
    this->isActive = false;
}

//------------------------------------------------------------------------------
/**
    This method is called when the graphics entity is attached to a stage. An
    entity may only be attached to one stage at any time, but can be
    attached to different stages during its lifetime. Attachging an entity
    to a stage may be relatively slow because the entity must be inserted
    into the cell hierarchy.
*/
void
GraphicsEntity::OnAttachToStage(const Ptr<Stage>& s)
{
    n_assert(this->IsActive());
    n_assert(!this->IsAttachedToStage());
    n_assert(!this->IsAttachedToCell());
    IndexT i;
    for (i = 0; i < NumLinkTypes; i++)
    {
        n_assert(this->links[i].IsEmpty());
    }
    this->stage = s;
}

//------------------------------------------------------------------------------
/**
    This method is called when the graphics entity is removed from a stage.
*/
void
GraphicsEntity::OnRemoveFromStage()
{
    n_assert(this->IsActive());
    n_assert(this->IsAttachedToStage());
    n_assert(!this->IsAttachedToCell());
    this->ClearLinks(CameraLink);
    this->ClearLinks(LightLink);
    this->stage = 0;
}

//------------------------------------------------------------------------------
/**
    This method is called when the graphics entity is attached to a cell inside
    a stage. When entity travel through the graphics world, they will be remove
    and attached themselves from and to Cells as they cross Cell borders.
*/
void
GraphicsEntity::OnAttachToCell(const Ptr<Cell>& c)
{
    n_assert(this->IsActive());
    n_assert(this->IsAttachedToStage());
    n_assert(!this->IsAttachedToCell());
    this->cell = c;
}

//------------------------------------------------------------------------------
/**
    Called when the graphics entity is removed from a cell inside a
    stage.
*/
void
GraphicsEntity::OnRemoveFromCell()
{
    n_assert(this->IsActive());
    n_assert(this->IsAttachedToStage());
    n_assert(this->IsAttachedToCell());
    this->cell = 0;
}

//------------------------------------------------------------------------------
/**
    This method is called from the SetVisible() method when the entity 
    changes from invisible to visible state.
*/
void
GraphicsEntity::OnShow()
{
    // empty
}

//------------------------------------------------------------------------------
/**
    This method is called from the SetVisible() method when the entity
    changes from visible to invisible state.
*/
void
GraphicsEntity::OnHide()
{
    // empty
}

//------------------------------------------------------------------------------
/**
    This method is called on the graphics entity to update itself,
    for instance to implement hierarchy animation or particle 
    updates.
*/
void
GraphicsEntity::OnUpdate()
{
    // if our transformation or bounding box has changed
    // we need to update our position in the cell tree
    if (this->transformChanged || this->boundingBoxChanged)
    {
        this->transformChanged = false;
        this->boundingBoxChanged = false;
        this->UpdatePositionInCellTree();
    }
}

//------------------------------------------------------------------------------
/**
    This method is called on the graphics entity to render itself. This method
    will only be called if the entity is visible through the camera of the 
    currently rendered View.
*/
void
GraphicsEntity::OnRender()
{
    // empty
}

//------------------------------------------------------------------------------
/**
    This method is called whenever the transformation matrix has changed
    by a call to SetTransform(). Override this method in a subclass if
    it wants to be informed about changes to the transformation matrix.
*/
void
GraphicsEntity::OnTransformChanged()
{
    // empty
}

//------------------------------------------------------------------------------
/**
    This method is called on the graphics entity to render a debug 
    visualization of itself.
*/
void
GraphicsEntity::OnRenderDebug()
{
    // empty
}

//------------------------------------------------------------------------------
/**
    This method is called to create an exact clone of this graphics entity.
    The new entity will not be attached to a stage.
*/
Ptr<GraphicsEntity>
GraphicsEntity::CreateClone() const
{
    Ptr<GraphicsEntity> clone = (GraphicsEntity*) this->GetRtti()->Create();
    clone->SetTransform(this->transform);
    clone->SetLocalBoundingBox(this->localBox);
    return clone;
}

//------------------------------------------------------------------------------
/**
*/
void
GraphicsEntity::ClearLinks(LinkType linkType)
{
    this->links[linkType].Clear();
}

//------------------------------------------------------------------------------
/**
*/
void
GraphicsEntity::UpdateGlobalBoundingBox()
{
    n_assert(this->globalBoxDirty);
    this->globalBox = this->localBox;
    this->globalBox.transform(this->transform);
    this->globalBoxDirty = false;
}

//------------------------------------------------------------------------------
/**
    This method is called from OnUpdate() when either the 
    entity's transformation or bounding box has changed. It checks if the
    entity still fits into its current cell, and if not, moves the entity
    in a new cell.
*/
void
GraphicsEntity::UpdatePositionInCellTree()
{
    // see if we need to move from one cell to another
    if (this->cell.isvalid())
    {
        Ptr<GraphicsEntity> thisPtr(this);
        const Ptr<Cell>& newCell = this->cell->FindEntityContainmentCell(thisPtr);
        if (this->cell != newCell)
        {
            this->cell->RemoveEntity(thisPtr);
            newCell->AttachEntity(thisPtr);
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
const Ptr<Stage>&
GraphicsEntity::GetStage() const
{
    return this->stage;
}

//------------------------------------------------------------------------------
/**
*/
bool
GraphicsEntity::IsAttachedToStage() const
{
    return this->stage.isvalid();
}

//------------------------------------------------------------------------------
/**
*/
const Ptr<Cell>&
GraphicsEntity::GetCell() const
{
    return this->cell;
}

//------------------------------------------------------------------------------
/**
*/
bool
GraphicsEntity::IsAttachedToCell() const
{
    return this->cell.isvalid();
}

//------------------------------------------------------------------------------
/**
    Compute the clip status between this entity and a bounding box in
    global space. This method must be overwritten in a derived class.
*/
ClipStatus::Type
GraphicsEntity::ComputeClipStatus(const bbox& box)
{
    return ClipStatus::Outside;
}

} // namespace Graphics