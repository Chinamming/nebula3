//------------------------------------------------------------------------------
//  msg/movedirection.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "msg/movedirection.h"

namespace BaseGameFeature
{
ImplementClass(MoveDirection, 'MOVD', Messaging::Message);
ImplementMsgId(MoveDirection);
};
