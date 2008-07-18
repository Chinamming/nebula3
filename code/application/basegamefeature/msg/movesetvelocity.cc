//------------------------------------------------------------------------------
//  msg/movesetvelocity.cc
//  (C) 2005 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "msg/movesetvelocity.h"

namespace BaseGameFeature
{
ImplementClass(MoveSetVelocity, 'MMSV', Messaging::Message);
ImplementMsgId(MoveSetVelocity);
};
