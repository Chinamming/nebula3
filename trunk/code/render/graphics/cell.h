#pragma once
#ifndef GRAPHICS_CELL_H
#define GRAPHICS_CELL_H
//------------------------------------------------------------------------------
/**
    @class Graphics::Cell
  
    Hierarchies of Cell objects group graphics entities by spatial 
    relationship. They are the key class for efficient visibility
    queries. An application may derive specialized subclasses of Cell
    which must only adhere to the following 2 simple rules:
    
    - if the Cell object is fully visible, all child Cells and 
      all Entities attached to the Cell are guaranteed to be visible
    - if the Cell object is fully invisible, all child Cells and
      all Entities attached to the Cell are guaranteed to be invisible

      @todo: need to handle extra shadow bounding box
      @todo: statistics and profiling
      @todo: need to add visibility depending on LOD
      @todo: multithreaded visibility link update?
      @todo: add dirty handling to visibility links (e.g. don't need to
             update links between static lights and static objects)


    (C) 2007 Radon Labs GmbH
*/
#include "core/refcounted.h"
#include "math/bbox.h"
#include "graphics/graphicsentity.h"

//------------------------------------------------------------------------------
namespace Graphics
{
class Stage;

class Cell : public Core::RefCounted
{
    DeclareClass(Cell);
public:
    /// constructor
    Cell();
    /// destructor
    virtual ~Cell();

    /// called when attached to Stage
    virtual void OnAttachToStage(const Ptr<Stage>& stage);
    /// called when removed from Stage
    virtual void OnRemoveFromStage();
    /// return true if currently attached to a stage
    bool IsAttachedToStage() const;
    /// get stage we are attached to
    const Ptr<Stage>& GetStage() const;

    /// set the Cell's world space bounding box
    void SetBoundingBox(const Math::bbox& box);
    /// get the Cell's world space transform
    const Math::bbox& GetBoundingBox() const;
        
    /// add a child cell (only during setup phase)
    void AttachChildCell(const Ptr<Cell>& cell);
    /// get pointer to parent cell (returns invalid pointer if this is root cell)
    const Ptr<Cell>& GetParentCell() const;
    /// get current child cells
    const Util::Array<Ptr<Cell> >& GetChildCells() const;

    /// attach a graphics entity to this Cell
    void AttachEntity(const Ptr<GraphicsEntity>& entity);
    /// remove a graphics entity from this Cell
    void RemoveEntity(const Ptr<GraphicsEntity>& entity);
    /// insert an entity into the Cell hierarchy
    void InsertEntity(const Ptr<GraphicsEntity>& entity);
    /// get all entities
    const Util::Array<Ptr<GraphicsEntity> >& GetEntities() const;
    /// get entities by entity type
    const Util::Array<Ptr<GraphicsEntity> >& GetEntitiesByType(GraphicsEntity::Type type) const;
    /// get the number of entities in hierarchy
    SizeT GetNumEntitiesInHierarchy() const;
    /// get the number of entities in hierarchy by type
    SizeT GetNumEntitiesInHierarchyByType(GraphicsEntity::Type t) const;
    /// get the number of entities in hierarchy by entity type mask
    SizeT GetNumEntitiesInHierarchyByTypeMask(uint entityTypeMask) const;

    /// recursively create visibility links between observers and observed entities
    void UpdateLinks(const Ptr<GraphicsEntity>& observerEntity, uint entityTypeMask, GraphicsEntity::LinkType linkType);

private:
    friend class GraphicsEntity;

    /// starting from this cell, find smallest containment cell in cell tree
    Ptr<Cell> FindEntityContainmentCell(const Ptr<GraphicsEntity>& entity);
    /// create links between visible entities
    void RecurseUpdateLinks(const Ptr<GraphicsEntity>& observerEntity, uint entityTypeMask, GraphicsEntity::LinkType linkType, Math::ClipStatus::Type clipStatus);
    /// increment/decrement the numEntitiesInHierarchy counter (including in all parent cells)
    void UpdateNumEntitiesInHierarchy(GraphicsEntity::Type type, int num);

    Ptr<Stage> stage;
    Ptr<Cell> parentCell;
    SizeT numEntitiesInHierarchyAllTypes;
    SizeT numEntitiesInHierarchyByType[GraphicsEntity::NumTypes];
    Math::bbox boundingBox;
    Util::Array<Ptr<Cell> > childCells;
    Util::Array<Ptr<GraphicsEntity> > entities;
    Util::Array<Ptr<GraphicsEntity> > entitiesByType[GraphicsEntity::NumTypes];
};

//------------------------------------------------------------------------------
/**
*/
inline bool
Cell::IsAttachedToStage() const
{
    return this->stage.isvalid();
}

//------------------------------------------------------------------------------
/**
*/
inline const Ptr<Stage>&
Cell::GetStage() const
{
    return this->stage;
}

//------------------------------------------------------------------------------
/**
*/
inline void
Cell::SetBoundingBox(const Math::bbox& box)
{
    this->boundingBox = box;
}

//------------------------------------------------------------------------------
/**
*/
inline const Math::bbox&
Cell::GetBoundingBox() const
{
    return this->boundingBox;
}

//------------------------------------------------------------------------------
/**
*/
inline const Ptr<Cell>& 
Cell::GetParentCell() const
{
    return this->parentCell;
}

//------------------------------------------------------------------------------
/**
*/
inline const Util::Array<Ptr<Cell> >& 
Cell::GetChildCells() const
{
    return this->childCells;
}

//------------------------------------------------------------------------------
/**
*/
inline const Util::Array<Ptr<GraphicsEntity> >&
Cell::GetEntities() const
{
    return this->entities;
}

//------------------------------------------------------------------------------
/**
*/
inline const Util::Array<Ptr<GraphicsEntity> >&
Cell::GetEntitiesByType(GraphicsEntity::Type type) const
{
    n_assert((type >= 0) && (type < GraphicsEntity::NumTypes));
    return this->entitiesByType[type];
}

//------------------------------------------------------------------------------
/**
*/
inline SizeT
Cell::GetNumEntitiesInHierarchy() const
{
    return this->numEntitiesInHierarchyAllTypes;
}

//------------------------------------------------------------------------------
/**
*/
inline SizeT
Cell::GetNumEntitiesInHierarchyByType(GraphicsEntity::Type type) const
{
    return this->numEntitiesInHierarchyByType[type];
}

//------------------------------------------------------------------------------
/**
*/
inline SizeT
Cell::GetNumEntitiesInHierarchyByTypeMask(uint entityTypeMask) const
{
    SizeT numEntities = 0;
    IndexT entityType;
    for (entityType = 0; entityType < GraphicsEntity::NumTypes; entityType++)
    {
        if (0 != (entityTypeMask & (1<<entityType)))
        {
            numEntities += this->numEntitiesInHierarchyByType[entityType];
        }
    }
    return numEntities;
}

} // namespace Graphics
//------------------------------------------------------------------------------
#endif
