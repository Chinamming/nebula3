//------------------------------------------------------------------------------
//  indexbuffer.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "coregraphics/indexbuffer.h"

#if __WIN32__
namespace CoreGraphics
{
ImplementClass(CoreGraphics::IndexBuffer, 'IDXB', Direct3D9::D3D9IndexBuffer);
}
#elif __XBOX360__
namespace CoreGraphics
{
ImplementClass(CoreGraphics::IndexBuffer, 'IDXB', Xbox360::Xbox360IndexBuffer);
}
#elif __WII__
namespace CoreGraphics
{
ImplementClass(CoreGraphics::IndexBuffer, 'IDXB', Wii::WiiIndexBuffer);
}
#else
#error "IndexBuffer class not implemented on this platform!"
#endif

