//------------------------------------------------------------------------------
//  d3d9indexbuffer.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "coregraphics/cpuindexbuffer.h"

namespace CoreGraphics
{
ImplementClass(CoreGraphics::CPUIndexBuffer, 'CPIB', Base::IndexBufferBase);

//------------------------------------------------------------------------------
/**
*/
CPUIndexBuffer::CPUIndexBuffer() :
    indexBuffer(0),
    mapCount(0)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
CPUIndexBuffer::~CPUIndexBuffer()
{
    n_assert(!this->indexBuffer.isvalid());
    n_assert(0 == this->mapCount);
}

//------------------------------------------------------------------------------
/**
*/
void
CPUIndexBuffer::Unload()
{
    n_assert(0 == this->mapCount);
    if (0 != this->indexBuffer)
    {        
        this->indexBuffer = 0;
    }
    IndexBufferBase::Unload();
}

//------------------------------------------------------------------------------
/**
*/
void*
CPUIndexBuffer::Map(MapType mapType)
{
    n_assert(0 != this->indexBuffer);
    
    if (!this->indexBuffer->IsOpen())
    {
        this->indexBuffer->Open();
    }
    this->mapCount++;
    return this->indexBuffer->Map();
}

//------------------------------------------------------------------------------
/**
*/
void
CPUIndexBuffer::Unmap()
{
    n_assert(0 != this->indexBuffer);

    n_assert(this->mapCount > 0);
    this->mapCount--;
    this->indexBuffer->Unmap();
    if (this->indexBuffer->IsOpen())
    {
        this->indexBuffer->Close();
    }
}

} // namespace CoreGraphics