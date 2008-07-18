//------------------------------------------------------------------------------
//  cpumemoryvertexbufferloader.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "io/ioserver.h"
#include "io/memorystream.h"
#include "coregraphics/cpumemoryvertexbufferloader.h"
#include "coregraphics/cpuvertexbuffer.h"
#include "coregraphics/vertexlayoutserver.h"

namespace CoreGraphics
{
ImplementClass(CoreGraphics::CPUMemoryVertexBufferLoader, 'CPVL', Base::MemoryVertexBufferLoaderBase);

using namespace Resources;

//------------------------------------------------------------------------------
/**
    This will create a D3D9 vertex buffer and vertex declaration object
    from the data provided in the Setup() method and setup our resource
    object (which must be a D3D9VertexBuffer object). The data pointer provided
    to Setup() will be invalidated inside OnLoadRequested(). Resource usage
    will be set to UsageImmutable and resource access to AccessNone.
*/
bool
CPUMemoryVertexBufferLoader::OnLoadRequested()
{
    n_assert(this->GetState() == Resource::Initial);
    n_assert(this->resource.isvalid());
    n_assert(!this->resource->IsAsyncEnabled());
    n_assert(0 != this->vertexDataPtr);

    // copy vertex data to vertex buffer in stream
    Ptr<IO::MemoryStream> data = IO::MemoryStream::Create();
    data->SetSize(this->vertexDataSize);
    data->SetAccessMode(IO::Stream::WriteAccess);
    data->Open();    
    data->Write(this->vertexDataPtr, this->vertexDataSize);
    data->Close();

    // create vertex layout
    Ptr<VertexLayout> vertexLayout = VertexLayoutServer::Instance()->CreateSharedVertexLayout(this->vertexComponents);
        
    // setup our resource object
    n_assert(this->resource->IsA(CPUVertexBuffer::RTTI));
    const Ptr<CPUVertexBuffer>& res = this->resource.downcast<CPUVertexBuffer>();
    n_assert(!res->IsLoaded());       
    res->SetAccess(CPUVertexBuffer::AccessReadWrite);       
    res->SetVertexLayout(vertexLayout);
    res->SetNumVertices(this->numVertices);
    res->SetCPUVertexBuffer(data);
    
    // invalidate setup data (because we don't own our data)
    this->vertexDataPtr = 0;
    this->vertexDataSize = 0;

    this->SetState(Resource::Loaded);
    return true;
}

} // namespace CoreGraphics
