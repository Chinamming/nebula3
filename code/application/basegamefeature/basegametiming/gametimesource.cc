//------------------------------------------------------------------------------
//  gametimesource.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "basegametiming/gametimesource.h"

namespace BaseGameFeature
{
ImplementClass(GameTimeSource, 'TSRC', TimeSource);
ImplementSingleton(GameTimeSource);

//------------------------------------------------------------------------------
/**
*/
GameTimeSource::GameTimeSource()
{
    ConstructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
GameTimeSource::~GameTimeSource()
{
    DestructSingleton;
}

}; // namespace Timing
