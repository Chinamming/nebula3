//------------------------------------------------------------------------------
//  msg/getphysicsentity.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "msg/getphysicsentity.h"

namespace PhysicsFeature
{
ImplementClass(GetPhysicsEntity, 'MGPE', Messaging::Message);
ImplementMsgId(GetPhysicsEntity);
};
