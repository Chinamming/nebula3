#pragma once
#ifndef COREGRAPHICS_CPUINDEXBUFFER_H
#define COREGRAPHICS_CPUINDEXBUFFER_H
//------------------------------------------------------------------------------
/**
    @class CoreGraphics::CPUIndexBuffer
  
    CPU implementation of index buffer.
    
    FIXME: need to handle DeviceLost render event!
    
    (C) 2007 Radon Labs GmbH
*/
#include "coregraphics/base/indexbufferbase.h"
#include "io/memorystream.h"

//------------------------------------------------------------------------------
namespace CoreGraphics
{
class CPUIndexBuffer : public Base::IndexBufferBase
{
    DeclareClass(CPUIndexBuffer);
public:
    /// constructor
    CPUIndexBuffer();
    /// destructor
    virtual ~CPUIndexBuffer();
    
    /// unload the resource, or cancel the pending load
    virtual void Unload();
    /// map index buffer for CPU access
    void* Map(MapType mapType);
    /// unmap the resource
    void Unmap();

    /// set index buffer depth, to allow changing depth
    void SetIndexBufferDepth(CoreGraphics::IndexType::Code depth);
    /// get index buffer depth
    CoreGraphics::IndexType::Code GetIndexBufferDepth() const;

private:
    friend class CPUMemoryIndexBufferLoader;

    /// set index buffer pointer
    void SetCPUIndexBuffer(const Ptr<IO::MemoryStream>& ptr);

    Ptr<IO::MemoryStream> indexBuffer;
    CoreGraphics::IndexType::Code indexDepth;
    int mapCount;
};

//------------------------------------------------------------------------------
/**
*/
inline 
void
CPUIndexBuffer::SetCPUIndexBuffer(const Ptr<IO::MemoryStream>& ptr)
{
    n_assert(0 != ptr);
    n_assert(!this->indexBuffer.isvalid());
    this->indexBuffer = ptr;
}

//------------------------------------------------------------------------------
/**
*/
inline void 
CPUIndexBuffer::SetIndexBufferDepth(CoreGraphics::IndexType::Code depth)
{
    this->indexDepth = depth;
}
//------------------------------------------------------------------------------
/**
*/
inline CoreGraphics::IndexType::Code 
CPUIndexBuffer::GetIndexBufferDepth() const
{
    return this->indexDepth;
}

} // namespace CoreGraphics
//------------------------------------------------------------------------------
#endif
