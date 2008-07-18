//------------------------------------------------------------------------------
//  msg/camerareset.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "msg/camerareset.h"

namespace GraphicsFeature
{
ImplementClass(CameraReset, 'CARE', Messaging::Message);
ImplementMsgId(CameraReset);
}; // namespace Msg