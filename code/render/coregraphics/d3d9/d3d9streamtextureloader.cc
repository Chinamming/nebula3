//------------------------------------------------------------------------------
//  d3d9streamtextureloader.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "coregraphics/d3d9/d3d9streamtextureloader.h"
#include "interface/iointerface.h"
#include "coregraphics/d3d9/d3d9texture.h"
#include "coregraphics/d3d9/d3d9renderdevice.h"
#include "io/memorystream.h"
#include "resources/resource.h"

namespace Direct3D9
{
ImplementClass(Direct3D9::D3D9StreamTextureLoader, 'D9TL', Resources::ResourceLoader);

using namespace CoreGraphics;
using namespace Resources;
using namespace IO;
using namespace Interface;
using namespace Messaging;

//------------------------------------------------------------------------------
/**
*/
bool
D3D9StreamTextureLoader::CanLoadAsync() const
{
    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool
D3D9StreamTextureLoader::OnLoadRequested()
{
    n_assert(this->GetState() == Resource::Initial);
    n_assert(this->resource.isvalid());
    if (this->resource->IsAsyncEnabled())
    {
        // perform asynchronous load
        // send off an asynchronous loader job
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
        if (this->SetupTextureFromStream(stream))
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
D3D9StreamTextureLoader::OnLoadCancelled()
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
D3D9StreamTextureLoader::OnPending()
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
            if (this->SetupTextureFromStream(this->readStreamMsg->GetStream()))
            {
                // everything ok!
                this->SetState(Resource::Loaded);                
                retval = true;
            }
            else
            {
                // this probably wasn't a texture file...
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
    This method actually setups the Texture object from the data in the stream.
*/
bool
D3D9StreamTextureLoader::SetupTextureFromStream(const Ptr<Stream>& stream)
{
    n_assert(stream.isvalid());
    n_assert(stream->CanBeMapped());
    HRESULT hr;
    IDirect3DDevice9* d3d9Device = D3D9RenderDevice::Instance()->GetDirect3DDevice();
    n_assert(0 != d3d9Device);
    n_assert(this->resource->IsA(D3D9Texture::RTTI));
    const Ptr<D3D9Texture>& res = this->resource.downcast<D3D9Texture>();
    n_assert(!res->IsLoaded());

    stream->SetAccessMode(Stream::ReadAccess);
    if (stream->Open())
    {
        void* srcData = stream->Map();
        UINT srcDataSize = stream->GetSize();

        // first need to check image info whether to determine texture type
        D3DXIMAGE_INFO imageInfo = { 0 };
        hr = D3DXGetImageInfoFromFileInMemory(srcData, srcDataSize, &imageInfo);
        if (FAILED(hr))
        {
            n_error("D3D9StreamTextureLoader: failed to obtain image info from file '%s'!", this->resource->GetResourceId().Value().AsCharPtr());
            return false;
        }

        // load texture based on texture type
        if (D3DRTYPE_TEXTURE == imageInfo.ResourceType)
        {
            // load 2D texture
            IDirect3DTexture9* d3d9Texture = 0;
            hr = D3DXCreateTextureFromFileInMemory(d3d9Device, srcData, srcDataSize, &d3d9Texture);
            if (FAILED(hr))
            {
                n_error("D3D9StreamTextureLoader: D3DXCreateTextureFromFileInMemory() failed for file '%s'!", this->resource->GetResourceId().Value().AsCharPtr());
                return false;
            }
            res->SetupFromD3D9Texture(d3d9Texture);
        }
        else if (D3DRTYPE_VOLUMETEXTURE == imageInfo.ResourceType)
        {
            // load 3D texture
            IDirect3DVolumeTexture9* d3d9VolumeTexture = 0;
            hr = D3DXCreateVolumeTextureFromFileInMemory(d3d9Device, srcData, srcDataSize, &d3d9VolumeTexture);
            if (FAILED(hr))
            {
                n_error("D3D9StreamTextureLoader: D3DXCreateVolumeTextureFromFileInMemory() failed for file '%s'!", this->resource->GetResourceId().Value().AsCharPtr());
                return false;
            }
            res->SetupFromD3D9VolumeTexture(d3d9VolumeTexture);
        }
        else if (D3DRTYPE_CUBETEXTURE == imageInfo.ResourceType)
        {
            // load cube texture
            IDirect3DCubeTexture9* d3d9CubeTexture = 0;
            hr = D3DXCreateCubeTextureFromFileInMemory(d3d9Device, srcData, srcDataSize, &d3d9CubeTexture);
            if (FAILED(hr))
            {
                n_error("D3D9StreamTextureLoader: D3DXCreateCubeTextureFromFileInMemory() failed for file '%s'!", this->resource->GetResourceId().Value().AsCharPtr());
                return false;
            }
            res->SetupFromD3D9CubeTexture(d3d9CubeTexture);
        }
        stream->Unmap();
        stream->Close();
        return true;
    }
    return false;
}

} // namespace Direct3D9