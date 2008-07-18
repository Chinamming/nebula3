//------------------------------------------------------------------------------
//  displaydevice.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "coregraphics/displaydevice.h"

namespace CoreGraphics
{
#if __WIN32__
ImplementClass(CoreGraphics::DisplayDevice, 'DDVC', Direct3D9::D3D9DisplayDevice);
ImplementSingleton(CoreGraphics::DisplayDevice);
#elif __XBOX360__
ImplementClass(CoreGraphics::DisplayDevice, 'DDVC', Xbox360::Xbox360DisplayDevice);
ImplementSingleton(CoreGraphics::DisplayDevice);
#elif __WII__
ImplementClass(CoreGraphics::DisplayDevice, 'DDVC', Wii::WiiDisplayDevice);
ImplementSingleton(CoreGraphics::DisplayDevice);
#else
#error "DisplayDevice class not implemented on this platform!"
#endif

//------------------------------------------------------------------------------
/**
*/
DisplayDevice::DisplayDevice()
{
    ConstructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
DisplayDevice::~DisplayDevice()
{
    DestructSingleton;
}

} // namespace CoreGraphics

