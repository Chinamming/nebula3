//------------------------------------------------------------------------------
//  msg/animationhotspottriggered.cc
//  (C) 2006 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "msg/animationhotspottriggered.h"
#include "graphics/modelentity.h"

namespace GraphicsFeature
{
ImplementClass(GraphicsFeature::AnimationHotspotTriggered, 'AHST', Messaging::Message);
ImplementMsgId(AnimationHotspotTriggered);

//------------------------------------------------------------------------------
/**
*/
AnimationHotspotTriggered::AnimationHotspotTriggered()
{
    // empty
}
}