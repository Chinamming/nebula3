//------------------------------------------------------------------------------
//  msg/inputfocus.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "msg/inputfocus.h"

namespace GraphicsFeature
{
ImplementClass(InputFocus, 'INFC', Messaging::Message);
ImplementMsgId(InputFocus);

//------------------------------------------------------------------------------
/**
*/
InputFocus::InputFocus() : 
	obtainFocus(true)
{
}

};

