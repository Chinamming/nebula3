//------------------------------------------------------------------------------
//  msg/moveturn.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "msg/moveturn.h"

namespace BaseGameFeature
{
ImplementClass(MoveTurn, 'MOVT', Messaging::Message);
ImplementMsgId(MoveTurn);


//------------------------------------------------------------------------------
/**
*/
MoveTurn::MoveTurn() : 
	dir(1.0f, 0.0f, 0.0f),
	camRelative(false)
{
}


};

