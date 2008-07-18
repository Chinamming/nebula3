//------------------------------------------------------------------------------
//  vertexbuffer.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "coregraphics/vertexbuffer.h"
#if __WIN32__
namespace CoreGraphics
{
ImplementClass(CoreGraphics::VertexBuffer, 'VTXB', Direct3D9::D3D9VertexBuffer);
}
#elif __XBOX360__
namespace CoreGraphics
{
ImplementClass(CoreGraphics::VertexBuffer, 'VTXB', Xbox360::Xbox360VertexBuffer);
}
#elif __WII__
namespace CoreGraphics
{
ImplementClass(CoreGraphics::VertexBuffer, 'VTXB', Wii::WiiVertexBuffer);
}
#else
#error "VertexBuffer class not implemented on this platform!"
#endif

