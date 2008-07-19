#pragma once
#ifndef COREGRAPHICS_CPUVERTEXBUFFER_H
#define COREGRAPHICS_CPUVERTEXBUFFER_H
//------------------------------------------------------------------------------
/** 
    @class CoreGraphics::CPUVertexBuffer
  
    Iimplementation of a CPU VertexBuffer hold in main memory.
    
    (C) 2007 Radon Labs GmbH
*/    
#include "coregraphics/base/vertexbufferbase.h"
#include "io/memorystream.h"

//------------------------------------------------------------------------------
namespace CoreGraphics
{
class CPUVertexBuffer : public Base::VertexBufferBase
{
    DeclareClass(CPUVertexBuffer);
public:
    /// constructor
    CPUVertexBuffer();
    /// destructor
    virtual ~CPUVertexBuffer();

    /// unload the resource, or cancel the pending load
    virtual void Unload();
    /// map the vertices for CPU access
    void* Map(MapType mapType);
    /// unmap the resource
    void Unmap();

private:
    friend class CPUMemoryVertexBufferLoader;

    /// set d3d9 vertex buffer pointer
    void SetCPUVertexBuffer(const Ptr<IO::MemoryStream>&  ptr);    

    Ptr<IO::MemoryStream> vertexBuffer;
    int mapCount;
};

//------------------------------------------------------------------------------
/**
*/
inline 
void
CPUVertexBuffer::SetCPUVertexBuffer(const Ptr<IO::MemoryStream>& ptr)
{
    n_assert(0 != ptr);
    n_assert(!this->vertexBuffer.isvalid());
    this->vertexBuffer = ptr;
}
} // namespace Direct3D9
//------------------------------------------------------------------------------
#endif

