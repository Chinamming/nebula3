//------------------------------------------------------------------------------
//  msg/camerafocus.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "msg/camerafocus.h"

namespace GraphicsFeature
{
ImplementClass(CameraFocus, 'CAFC', Messaging::Message);
ImplementMsgId(CameraFocus);

//------------------------------------------------------------------------------
/**
*/
CameraFocus::CameraFocus() : 
	obtainFocus(true)
{
}

};
