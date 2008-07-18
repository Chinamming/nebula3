//------------------------------------------------------------------------------
//  msg/updatetransform.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "msg/updatetransform.h"

namespace BaseGameFeature
{
ImplementClass(UpdateTransform, 'UPTR', Messaging::Message);
ImplementMsgId(UpdateTransform);
};

