//------------------------------------------------------------------------------
//  getgraphicsentities.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "msg/getgraphicsentities.h"

namespace GraphicsFeature
{
ImplementClass(GetGraphicsEntities, 'GGEN', Messaging::Message);
ImplementMsgId(GetGraphicsEntities);
}; // namespace Msg