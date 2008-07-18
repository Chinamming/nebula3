#pragma once
#ifndef DIRECT3D9_D3D9MEMORYVERTEXBUFFERLOADER_H
#define DIRECT3D9_D3D9MEMORYVERTEXBUFFERLOADER_H
//------------------------------------------------------------------------------
/**
    @class Direct3D9::D3D9MemoryVertexBufferLoader
    
    Initialize a D3D9VertexBuffer from data in memory. This resource loader
    only creates static VertexBuffers which are initialized once and are
    not accessible by the CPU.
    
    (C) 2007 Radon Labs GmbH
*/
#include "coregraphics/base/memoryvertexbufferloaderbase.h"

//------------------------------------------------------------------------------
namespace Direct3D9
{
class D3D9MemoryVertexBufferLoader : public Base::MemoryVertexBufferLoaderBase
{
    DeclareClass(D3D9MemoryVertexBufferLoader);
public:
    /// called by resource when a load is requested
    virtual bool OnLoadRequested();
};

} // namespace Direct3D9

//------------------------------------------------------------------------------
#endif

