//------------------------------------------------------------------------------
//  stagebuilder.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "graphics/stagebuilder.h"

namespace Graphics
{
ImplementClass(Graphics::StageBuilder, 'STGB', Core::RefCounted);

//------------------------------------------------------------------------------
/**
*/
StageBuilder::StageBuilder()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
StageBuilder::~StageBuilder()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
void
StageBuilder::BuildStage(const Ptr<Stage>& stage)
{
    // empty, overwrite in subclass!
}

} // namespace Graphics
