//------------------------------------------------------------------------------
//  vertexlayout.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "coregraphics/vertexlayout.h"
#if __WIN32__
namespace CoreGraphics
{
ImplementClass(CoreGraphics::VertexLayout, 'VTXL', Direct3D9::D3D9VertexLayout);
}
#elif __XBOX360__
namespace CoreGraphics
{
ImplementClass(CoreGraphics::VertexLayout, 'VTXL', Xbox360::Xbox360VertexLayout);
}
#elif __WII__
namespace CoreGraphics
{
ImplementClass(CoreGraphics::VertexLayout, 'VTXL', Wii::WiiVertexLayout);
}
#else
#error "VertexLayout class not implemented on this platform!"
#endif

