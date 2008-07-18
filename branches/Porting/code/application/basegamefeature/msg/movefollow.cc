//------------------------------------------------------------------------------
//  msg/movefollow.cc
//  (C) 2005 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "msg/movefollow.h"

namespace BaseGameFeature
{
ImplementClass(MoveFollow, 'MOVF', Messaging::Message);
ImplementMsgId(MoveFollow);
};
