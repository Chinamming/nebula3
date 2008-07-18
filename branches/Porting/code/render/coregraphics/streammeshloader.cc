//------------------------------------------------------------------------------
//  streammeshloader.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "coregraphics/streammeshloader.h"
#include "io/memorystream.h"
#include "interface/iointerface.h"
#include "coregraphics/mesh.h"
#include "coregraphics/legacy/nvx2streamreader.h"

namespace CoreGraphics
{
ImplementClass(CoreGraphics::StreamMeshLoader, 'SMLD', Resources::ResourceLoader);

using namespace IO;
using namespace Resources;
using namespace Interface;
using namespace Util;
using namespace Messaging;

//------------------------------------------------------------------------------
/**
*/
StreamMeshLoader::StreamMeshLoader()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
StreamMeshLoader::~StreamMeshLoader()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
bool
StreamMeshLoader::CanLoadAsync() const
{
    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool
StreamMeshLoader::OnLoadRequested()
{
    n_assert(this->GetState() == Resource::Initial);
    n_assert(this->resource.isvalid());
    if (this->resource->IsAsyncEnabled())
    {
        // perform asynchronous load
        n_assert(!this->readStreamMsg.isvalid());
        this->readStreamMsg = ReadStream::Create();
        this->readStreamMsg->SetURI(this->resource->GetResourceId().Value());
        this->readStreamMsg->SetStream(MemoryStream::Create());
        IOInterface::Instance()->Send(this->readStreamMsg.upcast<Message>());
        
        // go into Pending state
        this->SetState(Resource::Pending);
        return true;
    }
    else
    {
        // perform synchronous load
        Ptr<Stream> stream = IoServer::Instance()->CreateStream(this->resource->GetResourceId().Value());
        if (this->SetupMeshFromStream(stream))
        {
            this->SetState(Resource::Loaded);
            return true;
        }
        // fallthrough: synchronous loading failed
        this->SetState(Resource::Failed);
        return false;
    }
}

//------------------------------------------------------------------------------
/**
*/
void
StreamMeshLoader::OnLoadCancelled()
{
    n_assert(this->GetState() == Resource::Pending);
    n_assert(this->readStreamMsg.isvalid());
    IOInterface::Instance()->Cancel(this->readStreamMsg.upcast<Message>());
    this->readStreamMsg = 0;
    ResourceLoader::OnLoadCancelled();
}

//------------------------------------------------------------------------------
/**
*/
bool
StreamMeshLoader::OnPending()
{
    n_assert(this->GetState() == Resource::Pending);
    n_assert(this->readStreamMsg.isvalid());
    bool retval = false;

    // check if asynchronous loader job has finished
    if (this->readStreamMsg->Handled())
    {
        // ok, loader job has finished
        if (this->readStreamMsg->GetResult())
        {
            // IO operation was successful
            if (this->SetupMeshFromStream(this->readStreamMsg->GetStream()))
            {
                // everything ok!
                this->SetState(Resource::Loaded);                
                retval = true;
            }
            else
            {
                // file not found or not a valid mesh file...
                this->SetState(Resource::Failed);
            }
        }
        else
        {
            // error during IO operation
            this->SetState(Resource::Failed);
        }
        // we no longer need the loader job message
        this->readStreamMsg = 0;
    }
    return retval;
}

//------------------------------------------------------------------------------
/**
*/
bool
StreamMeshLoader::SetupMeshFromStream(const Ptr<Stream>& stream)
{
    n_assert(stream.isvalid());
    n_assert(this->resource.isvalid());
    #if NEBULA3_LEGACY_SUPPORT
    if (this->resource->GetResourceId().Value().GetFileExtension() == "nvx2")
    {
        return this->SetupMeshFromNvx2(stream);
    }
    else 
    #endif
    if (this->resource->GetResourceId().Value().GetFileExtension() == "nvx3")
    {
        return this->SetupMeshFromNvx3(stream);
    }
    else if (this->resource->GetResourceId().Value().GetFileExtension() == "n3d3")
    {
        return this->SetupMeshFromN3d3(stream);
    }
    else
    {
        n_error("StreamMeshLoader::SetupMeshFromStream(): unrecognized file extension in '%s'\n", this->resource->GetResourceId().Value().AsCharPtr());
        return false;
    }
}

//------------------------------------------------------------------------------
/**
    Setup the mesh resource from legacy nvx2 file (Nebula2 binary mesh format).
*/
#if NEBULA3_LEGACY_SUPPORT
bool
StreamMeshLoader::SetupMeshFromNvx2(const Ptr<Stream>& stream)
{
    n_assert(stream.isvalid());    
    Ptr<Legacy::Nvx2StreamReader> nvx2Reader = Legacy::Nvx2StreamReader::Create();
    nvx2Reader->SetStream(stream);
    if (nvx2Reader->Open())
    {
        const Ptr<Mesh>& res = this->resource.downcast<Mesh>();
        n_assert(!res->IsLoaded());
        res->SetVertexBuffer(nvx2Reader->GetVertexBuffer().downcast<CoreGraphics::VertexBuffer>());
        res->SetIndexBuffer(nvx2Reader->GetIndexBuffer().downcast<CoreGraphics::IndexBuffer>());
        res->SetPrimitiveGroups(nvx2Reader->GetPrimitiveGroups());
        nvx2Reader->Close();
        return true;
    }
    return false;
}
#endif

//------------------------------------------------------------------------------
/**
    Setup the mesh resource from a nvx3 file (Nebula3's
    native binary mesh file format).
*/
bool
StreamMeshLoader::SetupMeshFromNvx3(const Ptr<Stream>& stream)
{
    // FIXME!
    n_error("StreamMeshLoader::SetupMeshFromNvx3() not yet implemented");
    return false;
}

//------------------------------------------------------------------------------
/**
    Setup the mesh resource from a n3d3 file (Nebula3's
    native ascii mesh file format).
*/
bool
StreamMeshLoader::SetupMeshFromN3d3(const Ptr<Stream>& stream)
{
    // FIXME!
    n_error("StreamMeshLoader::SetupMeshFromN3d3() not yet implemented");
    return false;
}

} // namespace CoreGraphics
