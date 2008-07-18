//------------------------------------------------------------------------------
//  msg/updateattachments.cc
//  (C) 2006 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "msg/updateattachments.h"

namespace GraphicsFeature
{
ImplementClass(GraphicsFeature::UpdateAttachments, 'UPAT', Messaging::Message);
ImplementMsgId(UpdateAttachments);

//------------------------------------------------------------------------------
/**
*/
UpdateAttachments::UpdateAttachments()
{
    // empty
}


}