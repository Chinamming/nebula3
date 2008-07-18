#pragma once
#ifndef DIRECT3D9_D3D9INDEXBUFFER_H
#define DIRECT3D9_D3D9INDEXBUFFER_H
//------------------------------------------------------------------------------
/**
    @class Direct3D9::D3D9IndexBuffer
  
    D3D9 implementation of index buffer.
    
    FIXME: need to handle DeviceLost render event!
    
    (C) 2007 Radon Labs GmbH
*/
#include "coregraphics/base/indexbufferbase.h"

//------------------------------------------------------------------------------
namespace Direct3D9
{
class D3D9IndexBuffer : public Base::IndexBufferBase
{
    DeclareClass(D3D9IndexBuffer);
public:
    /// constructor
    D3D9IndexBuffer();
    /// destructor
    virtual ~D3D9IndexBuffer();
    
    /// unload the resource, or cancel the pending load
    virtual void Unload();
    /// map index buffer for CPU access
    void* Map(MapType mapType);
    /// unmap the resource
    void Unmap();

    /// get d3d9 index buffer pointer
    IDirect3DIndexBuffer9* GetD3D9IndexBuffer() const;

private:
    friend class D3D9MemoryIndexBufferLoader;

    /// set d3d9 index buffer pointer
    void SetD3D9IndexBuffer(IDirect3DIndexBuffer9* ptr);

    IDirect3DIndexBuffer9* d3d9IndexBuffer;
    int mapCount;
};

//------------------------------------------------------------------------------
/**
*/
inline void
D3D9IndexBuffer::SetD3D9IndexBuffer(IDirect3DIndexBuffer9* ptr)
{
    n_assert(0 != ptr);
    n_assert(0 == this->d3d9IndexBuffer);
    this->d3d9IndexBuffer = ptr;
}

//------------------------------------------------------------------------------
/**
*/
inline IDirect3DIndexBuffer9*
D3D9IndexBuffer::GetD3D9IndexBuffer() const
{
    n_assert(0 != this->d3d9IndexBuffer);
    n_assert(0 == this->mapCount);
    return this->d3d9IndexBuffer;
}

} // namespace Direct3D9
//------------------------------------------------------------------------------
#endif
