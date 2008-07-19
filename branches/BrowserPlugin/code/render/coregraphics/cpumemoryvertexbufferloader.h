#pragma once
#ifndef COREGRAPHICS_CPUMEMORYVERTEXBUFFERLOADER_H
#define COREGRAPHICS_CPUMEMORYVERTEXBUFFERLOADER_H
//------------------------------------------------------------------------------
/**
    @class CoreGraphics::CPUMemoryVertexBufferLoader
    
    Initialize a vertex buffer blob from data in memory. This resource loader
    creates VertexBuffers which are read and writeable by the CPU.
    
    (C) 2007 Radon Labs GmbH
*/
#include "coregraphics/base/memoryvertexbufferloaderbase.h"

//------------------------------------------------------------------------------
namespace CoreGraphics
{
class CPUMemoryVertexBufferLoader : public Base::MemoryVertexBufferLoaderBase
{
    DeclareClass(CPUMemoryVertexBufferLoader);
public:
    /// called by resource when a load is requested
    virtual bool OnLoadRequested();
};

} // namespace CoreGraphics

//------------------------------------------------------------------------------
#endif

