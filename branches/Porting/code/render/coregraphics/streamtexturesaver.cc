//------------------------------------------------------------------------------
//  streamtexturesaver.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "coregraphics/streamtexturesaver.h"

#if __WIN32__
namespace CoreGraphics
{
ImplementClass(CoreGraphics::StreamTextureSaver, 'STXS', Direct3D9::D3D9StreamTextureSaver);
}
#elif __XBOX360__
namespace CoreGraphics
{
ImplementClass(CoreGraphics::StreamTextureSaver, 'STXS', Xbox360::Xbox360StreamTextureSaver);
}
#elif __WII__
#else
#error "StreamTextureSaver class not implemented on this platform!"
#endif
