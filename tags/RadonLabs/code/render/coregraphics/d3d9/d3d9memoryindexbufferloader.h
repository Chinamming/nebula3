#pragma once
#ifndef DIRECT3D9_D3D9MEMORYINDEXBUFFERLOADER_H
#define DIRECT3D9_D3D9MEMORYINDEXBUFFERLOADER_H
//------------------------------------------------------------------------------
/**
    @class Direct3D9::D3D9MemoryIndexBufferLoader
    
    Initialize a D3D9IndexBuffer from data in memory. This resource loader
    only creates static IndexBuffers which are initialized once and are
    not accessible by the CPU.
    
    (C) 2007 Radon Labs GmbH
*/
#include "coregraphics/base/memoryindexbufferloaderbase.h"

//------------------------------------------------------------------------------
namespace Direct3D9
{
class D3D9MemoryIndexBufferLoader : public Base::MemoryIndexBufferLoaderBase
{
    DeclareClass(D3D9MemoryIndexBufferLoader);
public:
    /// called by resource when a load is requested
    virtual bool OnLoadRequested();
};

} // namespace Direct3D9
//------------------------------------------------------------------------------
#endif
