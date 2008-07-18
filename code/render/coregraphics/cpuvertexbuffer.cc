//------------------------------------------------------------------------------
//  cpuvertexbuffer.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "coregraphics/cpuvertexbuffer.h"

namespace CoreGraphics
{
ImplementClass(CoreGraphics::CPUVertexBuffer, 'CPVB', Base::VertexBufferBase);

//------------------------------------------------------------------------------
/**
*/
CPUVertexBuffer::CPUVertexBuffer() : 
    vertexBuffer(0),
    mapCount(0)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
CPUVertexBuffer::~CPUVertexBuffer()
{
    n_assert(!vertexBuffer.isvalid());
    n_assert(0 == this->mapCount);
}

//------------------------------------------------------------------------------
/**
*/
void
CPUVertexBuffer::Unload()
{
    n_assert(0 == this->mapCount);
    if (0 != this->vertexBuffer)
    {        
        this->vertexBuffer = 0;
    }
    VertexBufferBase::Unload();
}

//------------------------------------------------------------------------------
/**
*/
void*
CPUVertexBuffer::Map(MapType mapType)
{
    n_assert(0 != this->vertexBuffer);        
    this->mapCount++;
    
    if (!this->vertexBuffer->IsOpen())
    {
        this->vertexBuffer->Open();
    }
    return vertexBuffer->Map();
}

//------------------------------------------------------------------------------
/**
*/
void
CPUVertexBuffer::Unmap()
{
    n_assert(0 != this->vertexBuffer);
    n_assert(this->mapCount > 0);    
    this->mapCount--;
    vertexBuffer->Unmap();
    if (this->vertexBuffer->IsOpen())
    {
        this->vertexBuffer->Close();
    }
}

} // namespace CoreGraphics