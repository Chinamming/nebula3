#pragma once
#ifndef INTERNALGRAPHICS_INTERNALGRAPHICSENTITY_H
#define INTERNALGRAPHICS_INTERNALGRAPHICSENTITY_H
//------------------------------------------------------------------------------
/**
    @class InternalGraphics::InternalGraphicsEntity
    
    A graphics entity represents an atomic graphics object which can be
    attached to a Stage. Graphics entities come in three flavours:
    
    - ModelEntity: a visible model instance
    - LightEntity: a light source
    - CameraEntity: a camera 
    
    Visibility queries set graphics entities in relation to each other 
    through bidirectional links. A CameraEntity links to all 
    ModelEntities and LightEntities visible through the camera. Since
    visibility links are bidirectional, ModelEntities and LightEntities
    also know through which cameras they are visible. LightEntities
    have links to all ModelEntities they influence, and ModelEntities
    know by which lights they are lit. 
    
    (C) 2007 Radon Labs GmbH
*/
#include "core/refcounted.h"
#include "math/bbox.h"
#include "math/clipstatus.h"
#include "timing/time.h"
#include "internalgraphics/internalgraphicsentitytype.h"

//------------------------------------------------------------------------------
namespace InternalGraphics
{
class InternalStage;
class Cell;

class InternalGraphicsEntity : public Core::RefCounted
{
    __DeclareClass(InternalGraphicsEntity);
public:
    /// visibility link types
    enum LinkType
    {
        CameraLink = 0,
        LightLink,
        
        NumLinkTypes,
    };

    /// constructor
    InternalGraphicsEntity();
    /// destructor
    virtual ~InternalGraphicsEntity();

    /// return true if entity is currently active (is between OnActivate()/OnDeactivate()
    bool IsActive() const;
    /// return true if entity is current valid (ready for rendering)
    bool IsValid() const;
    /// get the entity type
    InternalGraphicsEntityType::Code GetType() const;
    /// set the entity's world space transform
    void SetTransform(const Math::matrix44& m);
    /// get the entity's world space transform
    const Math::matrix44& GetTransform() const;
    /// set the entity's visibility
    void SetVisible(bool b);
    /// return true if entity is set to visible
    bool IsVisible() const;
    /// get the stage this entity is attached to
    const Ptr<InternalStage>& GetStage() const;
    /// return true if entity is attached to stage
    bool IsAttachedToStage() const;
    /// get the cell this entity is attached to
    const Ptr<Cell>& GetCell() const;
    /// return true if entity is attached to cell
    bool IsAttachedToCell() const;
    /// get the local space bounding box
    const Math::bbox& GetLocalBoundingBox() const;
    /// get bounding box in global space
    const Math::bbox& GetGlobalBoundingBox();

    /// clear all visibility links
    void ClearLinks(LinkType linkType);
    /// add visibility link
    void AddLink(LinkType linkType, const Ptr<InternalGraphicsEntity>& entity);
    /// get visibility links by type
    const Util::Array<Ptr<InternalGraphicsEntity> >& GetLinks(LinkType type) const;

    /// compute clip status against bounding box
    virtual Math::ClipStatus::Type ComputeClipStatus(const Math::bbox& box);

    /// set graphics time
    void SetTime (Timing::Time t);
    /// get graphics time
    Timing::Time GetTime() const;

protected:
    friend class Cell;
    friend class InternalStage;
    friend class InternalView;
    friend class InternalGraphicsServer;

    /// set entity type, call in constructor of derived class!
    void SetType(InternalGraphicsEntityType::Code t);
    /// set the local space bounding box
    void SetLocalBoundingBox(const Math::bbox& b);
    /// set to valid state (when the entity becomes ready for rendering)
    void SetValid(bool b);
    /// create a clone of the entity
    virtual Ptr<InternalGraphicsEntity> CreateClone() const;
    /// called when entity is created
    virtual void OnActivate();
    /// called before entity is destroyed
    virtual void OnDeactivate();
    /// called when attached to Stage
    virtual void OnAttachToStage(const Ptr<InternalStage>& stage);
    /// called when removed from Stage
    virtual void OnRemoveFromStage();
    /// called when attached to a Cell
    virtual void OnAttachToCell(const Ptr<Cell>& cell);
    /// called when removed from a Cell
    virtual void OnRemoveFromCell();
    /// called when the entity becomes visible
    virtual void OnShow();
    /// called when the entity becomes invisible
    virtual void OnHide();
    /// called when transform matrix changed
    virtual void OnTransformChanged();
    /// called to update the entity before rendering
    virtual void OnUpdate();
    /// called before the entity is rendered
    virtual void OnRender();
    /// called to render a debug visualization of the entity
    virtual void OnRenderDebug();
    /// update our position in the cell hierarchy
    void UpdatePositionInCellTree();
    /// update the global bounding box from the transform and local box
    void UpdateGlobalBoundingBox();

    Math::matrix44 transform;
    Math::bbox localBox;
    Math::bbox globalBox;
    Timing::Time time;
    Ptr<InternalStage> stage;
    Ptr<Cell> cell;
    Util::FixedArray<Util::Array<Ptr<InternalGraphicsEntity> > > links;
    InternalGraphicsEntityType::Code type;
    bool isActive;
    bool isValid;
    bool isVisible;
    bool globalBoxDirty;
    bool transformChanged;
    bool boundingBoxChanged;
};

//------------------------------------------------------------------------------
/**
*/
inline bool
InternalGraphicsEntity::IsActive() const
{
    return this->isActive;
}

//------------------------------------------------------------------------------
/**
*/
inline void
InternalGraphicsEntity::SetValid(bool b)
{
    this->isValid = b;
}

//------------------------------------------------------------------------------
/**
*/
inline bool
InternalGraphicsEntity::IsValid() const
{
    return this->isValid;
}

//------------------------------------------------------------------------------
/**
*/
inline void
InternalGraphicsEntity::SetType(InternalGraphicsEntityType::Code t)
{
    this->type = t;
}

//------------------------------------------------------------------------------
/**
*/
inline InternalGraphicsEntityType::Code
InternalGraphicsEntity::GetType() const
{
    return this->type;
}

//------------------------------------------------------------------------------
/**
*/
inline void
InternalGraphicsEntity::SetTransform(const Math::matrix44& m)
{
    this->transform = m;
    this->globalBoxDirty = true;
    this->transformChanged = true;
    this->OnTransformChanged();
}

//------------------------------------------------------------------------------
/**
*/
inline const Math::matrix44&
InternalGraphicsEntity::GetTransform() const
{
    return this->transform;
}

//------------------------------------------------------------------------------
/**
*/
inline void
InternalGraphicsEntity::SetVisible(bool b)
{
    if (b != this->isVisible)
    {
        this->isVisible = b;
        if (this->isVisible)
        {
            this->OnShow();
        }
        else
        {
            this->OnHide();
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
inline bool
InternalGraphicsEntity::IsVisible() const
{
    return this->isVisible;
}

//------------------------------------------------------------------------------
/**
*/
inline void
InternalGraphicsEntity::SetLocalBoundingBox(const Math::bbox& b)
{
    this->localBox = b;
    this->boundingBoxChanged = true;
    this->globalBoxDirty = true;
}

//------------------------------------------------------------------------------
/**
*/
inline const Math::bbox&
InternalGraphicsEntity::GetLocalBoundingBox() const
{
    return this->localBox;
}

//------------------------------------------------------------------------------
/**
*/
inline const Math::bbox&
InternalGraphicsEntity::GetGlobalBoundingBox()
{
    if (this->globalBoxDirty)
    {
        this->UpdateGlobalBoundingBox();
    }
    return this->globalBox;
}

//------------------------------------------------------------------------------
/**
*/
inline void
InternalGraphicsEntity::AddLink(LinkType linkType, const Ptr<InternalGraphicsEntity>& entity)
{
    this->links[linkType].Append(entity);
}

//------------------------------------------------------------------------------
/**
*/
inline const Util::Array<Ptr<InternalGraphicsEntity> >&
InternalGraphicsEntity::GetLinks(LinkType linkType) const
{
    return this->links[linkType];
}

//------------------------------------------------------------------------------
/**
*/
inline void
InternalGraphicsEntity::SetTime(Timing::Time t)
{
    this->time = t;
}

//------------------------------------------------------------------------------
/**
*/
inline Timing::Time
InternalGraphicsEntity::GetTime() const
{
    return this->time;
}

} // namespace InternalGraphics
//------------------------------------------------------------------------------
#endif
    