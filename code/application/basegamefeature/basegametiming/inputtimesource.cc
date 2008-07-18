//------------------------------------------------------------------------------
//  inputtimesource.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "basegametiming/inputtimesource.h"

namespace BaseGameFeature
{
ImplementClass(InputTimeSource, 'TSIN', TimeSource);
ImplementSingleton(InputTimeSource);

//------------------------------------------------------------------------------
/**
*/
InputTimeSource::InputTimeSource()
{
    ConstructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
InputTimeSource::~InputTimeSource()
{
    DestructSingleton;
}

}; // namespace Timing
