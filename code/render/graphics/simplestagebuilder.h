#pragma once
#ifndef GRAPHICS_SIMPLESTAGEBUILDER_H
#define GRAPHICS_SIMPLESTAGEBUILDER_H
//------------------------------------------------------------------------------
/**
    @class Graphics::SimpleStageBuilder
    
    The most simple stage builder just creates a single cell where all
    entities live in. Use this stage builder when you only have one
    or very few graphics entities where no culling is needed.
    
    (C) 2007 Radon Labs GmbH
*/
#include "graphics/stagebuilder.h"

//------------------------------------------------------------------------------
namespace Graphics
{
class SimpleStageBuilder : public StageBuilder
{
    DeclareClass(SimpleStageBuilder);
public:
    /// constructor
    SimpleStageBuilder();
    /// destructor
    virtual ~SimpleStageBuilder();
    /// setup the stage
    virtual void BuildStage(const Ptr<Stage>& stage);
};

} // namespace Graphics
//------------------------------------------------------------------------------
#endif
    