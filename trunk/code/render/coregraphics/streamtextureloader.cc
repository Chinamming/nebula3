//------------------------------------------------------------------------------
//  streamtextureloader.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "coregraphics/streamtextureloader.h"

#if __WIN32__
namespace CoreGraphics
{
ImplementClass(CoreGraphics::StreamTextureLoader, 'STXL', Direct3D9::D3D9StreamTextureLoader);
}
#elif __XBOX360__
namespace CoreGraphics
{
ImplementClass(CoreGraphics::StreamTextureLoader, 'STXL', Xbox360::Xbox360StreamTextureLoader);
}
#elif __WII__
namespace CoreGraphics
{
ImplementClass(CoreGraphics::StreamTextureLoader, 'STXL', Wii::WiiStreamTextureLoader);
}
#else
#error "StreamTextureLoader class not implemented on this platform!"
#endif
