//------------------------------------------------------------------------------
//  d3d9memoryvertexbufferloader.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "coregraphics/vertexlayoutserver.h"
#include "coregraphics/d3d9/d3d9memoryvertexbufferloader.h"
#include "coregraphics/d3d9/d3d9types.h"
#include "coregraphics/d3d9/d3d9renderdevice.h"
#include "coregraphics/d3d9/d3d9vertexbuffer.h"

namespace Direct3D9
{
ImplementClass(Direct3D9::D3D9MemoryVertexBufferLoader, 'DMVL', Base::MemoryVertexBufferLoaderBase);

using namespace Resources;
using namespace CoreGraphics;

//------------------------------------------------------------------------------
/**
    This will create a D3D9 vertex buffer and vertex declaration object
    from the data provided in the Setup() method and setup our resource
    object (which must be a D3D9VertexBuffer object). The data pointer provided
    to Setup() will be invalidated inside OnLoadRequested(). Resource usage
    will be set to UsageImmutable and resource access to AccessNone.
*/
bool
D3D9MemoryVertexBufferLoader::OnLoadRequested()
{
    n_assert(this->GetState() == Resource::Initial);
    n_assert(this->resource.isvalid());
    n_assert(!this->resource->IsAsyncEnabled());
    

    IDirect3DDevice9* d3d9Device = D3D9RenderDevice::Instance()->GetDirect3DDevice();
    n_assert(0 != d3d9Device);

    // create a d3d9 vertex buffer object
    IDirect3DVertexBuffer9* d3dVertexBuffer = 0;
    HRESULT hr = d3d9Device->CreateVertexBuffer(this->vertexDataSize,       // Length
                                                0,                          // Usage
                                                0,                          // FVF
                                                D3DPOOL_MANAGED,            // Pool
                                                &d3dVertexBuffer,           // ppVertexBuffer
                                                NULL);                      // pSharedHandle
    n_assert(SUCCEEDED(hr));
    n_assert(0 != d3dVertexBuffer);

	// check if there is any data to write into the buffer
	if (0 != vertexDataPtr)
	{
		// copy vertex data to vertex buffer
		void* dstPtr = 0;
		hr = d3dVertexBuffer->Lock(0, 0, &dstPtr, D3DLOCK_NOSYSLOCK);
		n_assert(SUCCEEDED(hr));
		n_assert(0 != dstPtr);
		Memory::Copy(this->vertexDataPtr, dstPtr, this->vertexDataSize);
		hr = d3dVertexBuffer->Unlock();
		n_assert(SUCCEEDED(hr));
	}

    // setup the vertex layout (contains the D3D9 vertex declaration)
    Ptr<VertexLayout> vertexLayout = VertexLayoutServer::Instance()->CreateSharedVertexLayout(this->vertexComponents);
    n_assert((this->numVertices * vertexLayout->GetVertexByteSize()) == this->vertexDataSize);

    // setup our resource object
    n_assert(this->resource->IsA(D3D9VertexBuffer::RTTI));
    const Ptr<D3D9VertexBuffer>& res = this->resource.downcast<D3D9VertexBuffer>();
    n_assert(!res->IsLoaded());
	res->SetUsage(this->vertexBufferUsage);
	res->SetAccess(this->accessMode);
    res->SetVertexLayout(vertexLayout);
    res->SetNumVertices(this->numVertices);
    res->SetVertexLayout(vertexLayout);
    res->SetD3D9VertexBuffer(d3dVertexBuffer);

    // invalidate setup data (because we don't own our data)
    this->vertexDataPtr = 0;
    this->vertexDataSize = 0;

    this->SetState(Resource::Loaded);
    return true;
}

} // namespace CoreGraphics
