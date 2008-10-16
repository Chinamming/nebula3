#pragma once
#ifndef COREGRAPHICS_MEMORYVERTEXBUFFERLOADER_H
#define COREGRAPHICS_MEMORYVERTEXBUFFERLOADER_H
//------------------------------------------------------------------------------
/**
    @class CoreGraphics::MemoryVertexBufferLoader
    
    Initialize a vertex buffer object from vertex data in memory.
    
    (C) 2007 Radon Labs GmbH
*/
#if (__WIN32__ || __XBOX360__)
#include "coregraphics/win360/d3d9memoryvertexbufferloader.h"
namespace CoreGraphics
{
class MemoryVertexBufferLoader : public Win360::D3D9MemoryVertexBufferLoader
{
    __DeclareClass(MemoryVertexBufferLoader);
};
}
#elif __WII__
#include "coregraphics/wii/wiimemoryvertexbufferloader.h"
namespace CoreGraphics
{
class MemoryVertexBufferLoader : public Wii::WiiMemoryVertexBufferLoader
{
    __DeclareClass(MemoryVertexBufferLoader);
};
}
#else
#error "MemoryVertexBufferLoader class not implemented on this platform!"
#endif
//------------------------------------------------------------------------------
#endif
