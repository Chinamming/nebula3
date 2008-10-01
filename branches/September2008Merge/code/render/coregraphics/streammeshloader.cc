//------------------------------------------------------------------------------
//  streammeshloader.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "coregraphics/streammeshloader.h"

#if __WIN32__
namespace CoreGraphics
{
ImplementClass(CoreGraphics::StreamMeshLoader, 'SMLD', Direct3D9::D3D9StreamMeshLoader);
}
#elif __XBOX360__
namespace CoreGraphics
{
ImplementClass(CoreGraphics::StreamMeshLoader, 'SMLD', Xbox360::Xbox360StreamMeshLoader);
}
#elif __WII__
namespace CoreGraphics
{
ImplementClass(CoreGraphics::StreamMeshLoader, 'SMLD', Wii::WiiStreamMeshLoader);
}
#else
#error "StreamMeshLoader class not implemented on this platform!"
#endif
