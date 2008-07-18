#pragma once
#ifndef GRAPHICS_STAGEBUILDER_H
#define GRAPHICS_STAGEBUILDER_H
//------------------------------------------------------------------------------
/**
    @class Graphics::StageBuilder
    
    Stage builders are application-derived classes which construct a stage
    (build a hierarchy of Cells and populate them with graphics entities).
    
    (C) 2007 Radon Labs GmbH
*/
#include "core/refcounted.h"
#include "attr/attributecontainer.h"

//------------------------------------------------------------------------------
namespace Graphics
{
class Stage;

class StageBuilder : public Core::RefCounted
{
    DeclareClass(StageBuilder);
public:
    /// constructor
    StageBuilder();
    /// destructor
    virtual ~StageBuilder();

    /// set stage builder attributes
    void SetAttributes(const Attr::AttributeContainer& attrs);
    /// get stage builder attributes
    const Attr::AttributeContainer& GetAttributes() const;
    /// setup the stage
    virtual void BuildStage(const Ptr<Stage>& stage);

protected:
    Attr::AttributeContainer attrs;
};

//------------------------------------------------------------------------------
/*
*/
inline void
StageBuilder::SetAttributes(const Attr::AttributeContainer& a)
{
    this->attrs = a;
}

//------------------------------------------------------------------------------
/*
*/
inline const Attr::AttributeContainer&
StageBuilder::GetAttributes() const
{
    return this->attrs;
}

} // namespace Graphics
//------------------------------------------------------------------------------
#endif
    