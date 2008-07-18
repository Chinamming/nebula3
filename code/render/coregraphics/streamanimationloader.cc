//------------------------------------------------------------------------------
//  streamanimationloader.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "coregraphics/streamanimationloader.h"
#include "io/memorystream.h"
#include "interface/iointerface.h"
#include "coregraphics/legacy/nax2streamreader.h"

namespace CoreGraphics
{
ImplementClass(CoreGraphics::StreamAnimationLoader, 'SANL', Resources::ResourceLoader);

using namespace IO;
using namespace Resources;
using namespace Interface;
using namespace Util;
using namespace Messaging;

//------------------------------------------------------------------------------
/**
*/
StreamAnimationLoader::StreamAnimationLoader()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
StreamAnimationLoader::~StreamAnimationLoader()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
bool
StreamAnimationLoader::CanLoadAsync() const
{
    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool
StreamAnimationLoader::OnLoadRequested()
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
        if (this->SetupAnimationFromStream(stream))
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
StreamAnimationLoader::OnLoadCancelled()
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
StreamAnimationLoader::OnPending()
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
            if (this->SetupAnimationFromStream(this->readStreamMsg->GetStream()))
            {
                // everything ok!
                this->SetState(Resource::Loaded);                
                retval = true;
            }
            else
            {
                // file not found or not a valid animation file...
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
StreamAnimationLoader::SetupAnimationFromStream(const Ptr<Stream>& stream)
{
    n_assert(stream.isvalid());
    n_assert(this->resource.isvalid());
    #if NEBULA3_LEGACY_SUPPORT
    if (this->resource->GetResourceId().Value().GetFileExtension() == "nax2")
    {
        return this->SetupAnimationFromNax2(stream);
    }
    else 
    #endif
    if (this->resource->GetResourceId().Value().GetFileExtension() == "nax3")
    {
        return this->SetupAnimationFromNax3(stream);
    }
    else if (this->resource->GetResourceId().Value().GetFileExtension() == "nanim3")
    {
        return this->SetupAnimationFromNanim3(stream);
    }
    else
    {
        n_error("StreamAnimationLoader::SetupAnimationFromStream(): unrecognized file extension in '%s'\n", this->resource->GetResourceId().Value().AsCharPtr());
        return false;
    }
}

//------------------------------------------------------------------------------
/**
    Setup the Animation resource from legacy nvx2 file (Nebula2 binary animation format).
*/
#if NEBULA3_LEGACY_SUPPORT
bool
StreamAnimationLoader::SetupAnimationFromNax2(const Ptr<Stream>& stream)
{
    n_assert(stream.isvalid());    
    Ptr<Legacy::Nax2StreamReader> nax2Reader = Legacy::Nax2StreamReader::Create();
    nax2Reader->SetStream(stream);
    const Ptr<Anim::Animation>& res = this->resource.downcast<Anim::Animation>();
    n_assert(!res->IsLoaded());
    nax2Reader->SetAnimation(res);

    if (nax2Reader->Open())
    {
        nax2Reader->Close();
        return true;
    }
    return false;
}
#endif

//------------------------------------------------------------------------------
/**
    Setup the Animation resource from a nvx3 file (Nebula3's
    native binary animation file format).
*/
bool
StreamAnimationLoader::SetupAnimationFromNax3(const Ptr<Stream>& stream)
{
    // FIXME!
    n_error("StreamAnimationLoader::SetupAnimationFromNvx3() not yet implemented");
    return false;
}

//------------------------------------------------------------------------------
/**
    Setup the Animation resource from a n3d3 file (Nebula3's
    native ascii animation file format).
*/
bool
StreamAnimationLoader::SetupAnimationFromNanim3(const Ptr<Stream>& stream)
{
    // FIXME!
    n_error("StreamAnimationLoader::SetupAnimationFromN3d3() not yet implemented");
    return false;
}

} // namespace CoreGraphics
