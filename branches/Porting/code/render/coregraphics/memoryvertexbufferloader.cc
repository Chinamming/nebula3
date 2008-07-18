//------------------------------------------------------------------------------
//  memoryvertexbufferloader.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "coregraphics/memoryvertexbufferloader.h"

#if __WIN32__
namespace CoreGraphics
{
ImplementClass(CoreGraphics::MemoryVertexBufferLoader, 'MVBL', Direct3D9::D3D9MemoryVertexBufferLoader);
}
#elif __XBOX360__
namespace CoreGraphics
{
ImplementClass(CoreGraphics::MemoryVertexBufferLoader, 'MVBL', Xbox360::Xbox360MemoryVertexBufferLoader);
}
#elif __WII__
namespace CoreGraphics
{
ImplementClass(CoreGraphics::MemoryVertexBufferLoader, 'MVBL', Wii::WiiMemoryVertexBufferLoader);
}
#else
#error "MemoryVertexBufferLoader class not implemented on this platform!"
#endif
