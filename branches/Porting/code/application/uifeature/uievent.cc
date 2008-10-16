//------------------------------------------------------------------------------
//  uifeature/uievent.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "uifeature/uievent.h"

namespace UI
{
__ImplementClass(UIEvent, 'UIEV', Messaging::Message);
ImplementMsgId(UIEvent);
};
