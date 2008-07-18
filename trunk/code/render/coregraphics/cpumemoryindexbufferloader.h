#pragma once
#ifndef DIRECT3D9_D3D9MEMORYINDEXBUFFERLOADER_H
#define DIRECT3D9_D3D9MEMORYINDEXBUFFERLOADER_H
//------------------------------------------------------------------------------
/**
    @class CoreGraphics::CPUMemoryIndexBufferLoader
    
    Initialize a D3D9IndexBuffer from data in memory. This resource loader
    only creates static IndexBuffers which are initialized once and are
    not accessible by the CPU.
    
    (C) 2007 Radon Labs GmbH
*/
#include "coregraphics/base/memoryindexbufferloaderbase.h"

//------------------------------------------------------------------------------
namespace CoreGraphics
{
class CPUMemoryIndexBufferLoader : public Base::MemoryIndexBufferLoaderBase
{
    DeclareClass(CPUMemoryIndexBufferLoader);
public:
    /// called by resource when a load is requested
    virtual bool OnLoadRequested();
};

} // namespace CoreGraphics
//------------------------------------------------------------------------------
#endif
