//------------------------------------------------------------------------------
//  msg/setvisible.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "msg/setvisible.h"

namespace GraphicsFeature
{
ImplementClass(GraphicsFeature::SetVisibleMsg, 'MGFV', Messaging::Message);
ImplementMsgId(SetVisibleMsg);

//------------------------------------------------------------------------------
/**
*/
SetVisibleMsg::SetVisibleMsg()
{
    // empty
}

};
