//------------------------------------------------------------------------------
//  renderdevice.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "coregraphics/renderdevice.h"

namespace CoreGraphics
{
#if __WIN32__
ImplementClass(CoreGraphics::RenderDevice, 'RDVC', Direct3D9::D3D9RenderDevice);
#elif __XBOX360__
ImplementClass(CoreGraphics::RenderDevice, 'RDVC', Xbox360::Xbox360RenderDevice);
#elif __WII__
ImplementClass(CoreGraphics::RenderDevice, 'RDVC', Wii::WiiRenderDevice);
#else
#error "RenderDevice class not implemented on this platform!"
#endif

ImplementSingleton(CoreGraphics::RenderDevice);

//------------------------------------------------------------------------------
/**
*/
RenderDevice::RenderDevice()
{
    ConstructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
RenderDevice::~RenderDevice()
{
    DestructSingleton;
}

} // namespace CoreGraphics
