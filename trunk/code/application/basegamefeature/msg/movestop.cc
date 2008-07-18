//------------------------------------------------------------------------------
//  msg/movestop.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "msg/movestop.h"

namespace BaseGameFeature
{
ImplementClass(MoveStop, 'MOVS', Messaging::Message);
ImplementMsgId(MoveStop);
};
