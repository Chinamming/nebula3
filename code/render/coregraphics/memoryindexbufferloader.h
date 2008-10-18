#pragma once
#ifndef COREGRAPHICS_MEMORYINDEXBUFFERLOADER_H
#define COREGRAPHICS_MEMORYINDEXBUFFERLOADER_H
//------------------------------------------------------------------------------
/**
    @class CoreGraphics::MemoryIndexBufferLoader
    
    Initialize an index buffer object from index data in memory.
    
    (C) 2007 Radon Labs GmbH
*/
#if (__WIN32__ || __XBOX360__)
#include "coregraphics/win360/d3d9memoryindexbufferloader.h"
namespace CoreGraphics
{
class MemoryIndexBufferLoader : public Win360::D3D9MemoryIndexBufferLoader
{
    __DeclareClass(MemoryIndexBufferLoader);
};
}
#elif __WII__
#include "coregraphics/wii/wiimemoryindexbufferloader.h"
namespace CoreGraphics
{
class MemoryIndexBufferLoader : public Wii::WiiMemoryIndexBufferLoader
{
    __DeclareClass(MemoryIndexBufferLoader);
};
}
#else
#error "MemoryIndexBufferLoader class not implemented on this platform!"
#endif
//------------------------------------------------------------------------------
#endif
