//------------------------------------------------------------------------------
//  d3d9memoryindexbufferloader.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "coregraphics/cpumemoryindexbufferloader.h"
#include "coregraphics/cpuindexbuffer.h"

namespace CoreGraphics
{
ImplementClass(CoreGraphics::CPUMemoryIndexBufferLoader, 'CPIL', Base::MemoryIndexBufferLoaderBase);

using namespace Resources;

//------------------------------------------------------------------------------
/**
    This will create a D3D9 IndexBuffer using the data provided by our
    Setup() method and set our resource object (which must be a
    D3D9IndexBuffer object). The data pointer provided to Setup() will
    be invalidated inside OnLoadRequested(). Usage will be set to
    UsageImmutable and Access to AccessNone.
*/
bool
CPUMemoryIndexBufferLoader::OnLoadRequested()
{
    n_assert(this->GetState() == Resource::Initial);
    n_assert(this->resource.isvalid());
    n_assert(!this->resource->IsAsyncEnabled());
    n_assert(0 != this->indexDataPtr);
    n_assert(this->indexType != IndexType::None);
    n_assert(this->numIndices > 0);
    n_assert(this->indexDataSize == (this->numIndices * IndexType::SizeOf(this->indexType)));

    // setup our resource object
    n_assert(this->resource->IsA(CPUIndexBuffer::RTTI));
    const Ptr<CPUIndexBuffer>& res = this->resource.downcast<CPUIndexBuffer>();
    n_assert(!res->IsLoaded());   

    bool index32Size = res->GetIndexBufferDepth() == CoreGraphics::IndexType::Index32;
    // copy index data to index buffer in stream
    Ptr<IO::MemoryStream> data = IO::MemoryStream::Create();
    if (index32Size)
    {
        // grow to double size
        data->SetSize(this->indexDataSize * 2);
    }
    else
    {
        data->SetSize(this->indexDataSize);
    }
    data->SetAccessMode(IO::Stream::WriteAccess);    
    data->Open();    
    if (index32Size)
    {
        // write from 16 bit into 32 bit blocks
        uint blockIndex;
        uchar* srcIndexBuf = (uchar*)this->indexDataPtr;
        ushort zero = 0;
        for (blockIndex = 0; blockIndex < this->indexDataSize; blockIndex += 2)
        {
            data->Write(srcIndexBuf, 2);
            data->Write(&zero, 2);
            srcIndexBuf += 2;            
        }
    }
    else
    {
        data->Write(this->indexDataPtr, this->indexDataSize);
    }
    data->Close();
        
    // set data in resource object    
    res->SetAccess(CPUIndexBuffer::AccessReadWrite);       
    res->SetNumIndices(this->numIndices);
    res->SetCPUIndexBuffer(data);
    
    // invalidate setup data (because we don't own our data)
    this->indexDataPtr = 0;
    this->indexDataSize = 0;
    

    this->SetState(Resource::Loaded);
    return true;
}

} // namespace CoreGraphics
