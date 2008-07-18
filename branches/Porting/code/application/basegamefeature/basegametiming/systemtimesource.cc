//--------------------------------------------------------------------
//  systemtimesource.cc
//  (C) 2007 Radon Labs GmbH
//--------------------------------------------------------------------
#include "stdneb.h"
#include "basegametiming/systemtimesource.h"

namespace BaseGameFeature
{
ImplementClass(SystemTimeSource, 'TSTS', TimeSource);
ImplementSingleton(SystemTimeSource);
//--------------------------------------------------------------------
/**
*/
SystemTimeSource::SystemTimeSource()
{
    ConstructSingleton;
}

//--------------------------------------------------------------------
/**
*/
SystemTimeSource::~SystemTimeSource()
{
    DestructSingleton;
}

}; // namespace Timing