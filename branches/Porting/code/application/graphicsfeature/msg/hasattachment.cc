//------------------------------------------------------------------------------
//  hasattachment.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "msg/hasattachment.h"

namespace GraphicsFeature
{
ImplementClass(GraphicsFeature::HasAttachment, 'HASA', Messaging::Message);
ImplementMsgId(HasAttachment);
}
