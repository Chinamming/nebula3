//------------------------------------------------------------------------------
//  msg/addattachment.cc
//  (C) 2005 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "msg/addattachment.h"

namespace GraphicsFeature
{
ImplementClass(GraphicsFeature::AddAttachment, 'ADDA', Messaging::Message);
ImplementMsgId(AddAttachment);

//------------------------------------------------------------------------------
/**
*/
AddAttachment::AddAttachment():
    remCurAttachments(true),
    attachEntityToLevel(true),
    isCharGraphics(false)
{
    // empty
}

} // namespace GraphicsFeature
