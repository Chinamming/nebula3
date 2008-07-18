//------------------------------------------------------------------------------
//  msg/getactiveanimation.cc
//  (C) 2005 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "msg/getactiveanimation.h"

namespace GraphicsFeature
{
ImplementClass(GraphicsFeature::GetActiveAnimation, 'GEAA', Messaging::Message);
ImplementMsgId(GetActiveAnimation);
};
