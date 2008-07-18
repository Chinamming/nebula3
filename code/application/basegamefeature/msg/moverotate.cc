//------------------------------------------------------------------------------
//  msg/moverotate.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "msg/moverotate.h"

namespace BaseGameFeature
{
ImplementClass(MoveRotate, 'MMRO', Messaging::Message);
ImplementMsgId(MoveRotate);


//------------------------------------------------------------------------------
/**
*/
MoveRotate::MoveRotate() : 
	angle(0.0f)
{
}


};

