//------------------------------------------------------------------------------
//  transformdevice.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "coregraphics/transformdevice.h"

namespace CoreGraphics
{
#if __WII__
ImplementClass(CoreGraphics::TransformDevice, 'TRDV', Wii::WiiTransformDevice);
ImplementSingleton(CoreGraphics::TransformDevice);
#else
ImplementClass(CoreGraphics::TransformDevice, 'TRDV', Base::TransformDeviceBase);
ImplementSingleton(CoreGraphics::TransformDevice);
#endif

//------------------------------------------------------------------------------
/**
*/
TransformDevice::TransformDevice()
{
    ConstructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
TransformDevice::~TransformDevice()
{
    DestructSingleton;
}

} // namespace CoreGraphics
