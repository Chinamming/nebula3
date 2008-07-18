//------------------------------------------------------------------------------
//  d3d9memoryindexbufferloader.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "coregraphics/d3d9/d3d9memoryindexbufferloader.h"
#include "coregraphics/d3d9/d3d9indexbuffer.h"
#include "coregraphics/d3d9/d3d9renderdevice.h"

namespace Direct3D9
{
ImplementClass(Direct3D9::D3D9MemoryIndexBufferLoader, 'DMIL', Base::MemoryIndexBufferLoaderBase);

using namespace Resources;
using namespace CoreGraphics;

//------------------------------------------------------------------------------
/**
    This will create a D3D9 IndexBuffer using the data provided by our
    Setup() method and set our resource object (which must be a
    D3D9IndexBuffer object). The data pointer provided to Setup() will
    be invalidated inside OnLoadRequested(). Usage will be set to
    UsageImmutable and Access to AccessNone.
*/
bool
D3D9MemoryIndexBufferLoader::OnLoadRequested()
{
    n_assert(this->GetState() == Resource::Initial);
    n_assert(this->resource.isvalid());
    n_assert(!this->resource->IsAsyncEnabled());
    //n_assert(0 != this->indexDataPtr);
    n_assert(this->indexType != IndexType::None);
    n_assert(this->numIndices > 0);
    n_assert(this->indexDataSize == (this->numIndices * IndexType::SizeOf(this->indexType)));

    IDirect3DDevice9* d3d9Device = D3D9RenderDevice::Instance()->GetDirect3DDevice();
    n_assert(0 != d3d9Device);
   
	// create a D3D9 index buffer object
    D3DFORMAT d3d9Format = (IndexType::Index16 == this->indexType) ? D3DFMT_INDEX16 : D3DFMT_INDEX32;
    IDirect3DIndexBuffer9* d3dIndexBuffer = 0;
    HRESULT hr = d3d9Device->CreateIndexBuffer(this->indexDataSize,     // Length,
                                               0,                       // Usage
                                               d3d9Format,              // Format
                                               D3DPOOL_MANAGED,         // Pool
                                               &d3dIndexBuffer,         // ppIndexBuffer
                                               NULL);                   // pSharedHandle
    n_assert(SUCCEEDED(hr));
    n_assert(0 != d3dIndexBuffer);

	// check if no data should be loaded on creation
	if (0 != this->indexDataPtr)
	{
		// copy data to d3d9 index buffer
		void* dstPtr = 0;
		hr = d3dIndexBuffer->Lock(0, 0, &dstPtr, D3DLOCK_NOSYSLOCK);
		n_assert(SUCCEEDED(hr));
		n_assert(0 != dstPtr);
		Memory::Copy(this->indexDataPtr, dstPtr, this->indexDataSize);
		hr = d3dIndexBuffer->Unlock();
		n_assert(SUCCEEDED(hr));
	}

    // setup our IndexBuffer resource
    n_assert(this->resource->IsA(D3D9IndexBuffer::RTTI));
    const Ptr<D3D9IndexBuffer>& res = this->resource.downcast<D3D9IndexBuffer>();
    n_assert(!res->IsLoaded());
	res->SetUsage(this->indexBufferUsage);
	res->SetAccess(this->accessMode);
    res->SetIndexType(this->indexType);
    res->SetNumIndices(this->numIndices);
    res->SetD3D9IndexBuffer(d3dIndexBuffer);

    // invalidate setup data (because we don't own our data)
    this->indexDataPtr = 0;
    this->indexDataSize = 0;

    this->SetState(Resource::Loaded);
    return true;
}

} // namespace CoreGraphics
