//------------------------------------------------------------------------------
//  msg/gethotspottime.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "msg/gethotspottime.h"

namespace GraphicsFeature
{
ImplementClass(GraphicsFeature::GetHotspotTime, 'GHOT', Messaging::Message);
ImplementMsgId(GetHotspotTime);

//------------------------------------------------------------------------------
/**
*/
GetHotspotTime::GetHotspotTime()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
GetHotspotTime::~GetHotspotTime()
{
    // empty
}

}