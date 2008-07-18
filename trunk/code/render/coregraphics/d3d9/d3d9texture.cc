//------------------------------------------------------------------------------
//  d3d9texture.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "coregraphics/d3d9/d3d9texture.h"
#include "coregraphics/d3d9/d3d9types.h"

namespace Direct3D9
{
ImplementClass(Direct3D9::D3D9Texture, 'D9TX', Base::TextureBase);

using namespace CoreGraphics;

//------------------------------------------------------------------------------
/**
*/
D3D9Texture::D3D9Texture() :
    d3d9BaseTexture(0),
    d3d9Texture(0),
    d3d9CubeTexture(0),
    d3d9VolumeTexture(0),
    mapCount(0)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
D3D9Texture::~D3D9Texture()
{
    n_assert(0 == this->d3d9BaseTexture);
    n_assert(0 == this->d3d9Texture);
    n_assert(0 == this->d3d9CubeTexture);
    n_assert(0 == this->d3d9VolumeTexture);
    n_assert(0 == this->mapCount);
}

//------------------------------------------------------------------------------
/**
*/
void
D3D9Texture::Unload()
{
    n_assert(0 == this->mapCount);
    if (0 != this->d3d9BaseTexture)
    {
        this->d3d9BaseTexture->Release();
        this->d3d9BaseTexture = 0;
    }
    if (0 != this->d3d9Texture)
    {
        this->d3d9Texture->Release();
        this->d3d9Texture = 0;
    }
    if (0 != this->d3d9CubeTexture)
    {
        this->d3d9CubeTexture->Release();
        this->d3d9CubeTexture = 0;
    }
    if (0 != this->d3d9VolumeTexture)
    {
        this->d3d9VolumeTexture->Release();
        this->d3d9VolumeTexture = 0;
    }
    TextureBase::Unload();
}

//------------------------------------------------------------------------------
/**
*/
bool
D3D9Texture::Map(IndexT mipLevel, MapType mapType, MapInfo& outMapInfo)
{
    n_assert((this->type == Texture2D) || (this->type == Texture3D));
    n_assert(MapWriteNoOverwrite != mapType);
    bool retval = false;
    DWORD lockFlags = D3DLOCK_NO_DIRTY_UPDATE | D3DLOCK_NOSYSLOCK;
    switch (mapType)
    {
        case MapRead:
            n_assert((UsageStaging == this->usage) && (AccessRead == this->access));
            lockFlags |= D3DLOCK_READONLY;
            break;
        case MapWrite:
            n_assert((UsageStaging == this->usage) && (AccessWrite == this->access));
            break;
        case MapReadWrite:
            n_assert((UsageStaging == this->usage) && (AccessReadWrite == this->access));
            break;
        case MapWriteDiscard:
            n_assert((UsageDynamic == this->usage) && (AccessWrite == this->access));
            lockFlags |= D3DLOCK_DISCARD;
            break;
    }

    if (Texture2D == this->type)
    {
        n_assert(0 != this->d3d9Texture);
        D3DLOCKED_RECT lockedRect = { 0 };
        HRESULT hr = this->d3d9Texture->LockRect(mipLevel, &lockedRect, NULL, lockFlags);
        if (SUCCEEDED(hr))
        {
            outMapInfo.data = lockedRect.pBits;
            outMapInfo.rowPitch = lockedRect.Pitch;
            outMapInfo.depthPitch = 0;
            retval = true;
        }
    }
    else if (Texture3D == this->type)
    {
        n_assert(0 != this->d3d9VolumeTexture);
        D3DLOCKED_BOX lockedBox = { 0 };
        HRESULT hr = this->d3d9VolumeTexture->LockBox(mipLevel, &lockedBox, NULL, lockFlags);
        if (SUCCEEDED(hr))
        {
            outMapInfo.data = lockedBox.pBits;
            outMapInfo.rowPitch = lockedBox.RowPitch;
            outMapInfo.depthPitch = lockedBox.SlicePitch;
            retval = true;
        }
    }
    if (retval)
    {
        this->mapCount++;
    }
    return retval;
}

//------------------------------------------------------------------------------
/**
*/
void
D3D9Texture::Unmap(IndexT mipLevel)
{
    n_assert(this->mapCount > 0);
    n_assert((Texture2D == this->type) || (Texture3D == this->type));
    if (Texture2D == this->type)
    {
        n_assert(0 != this->d3d9Texture);
        HRESULT hr = this->d3d9Texture->UnlockRect(mipLevel);
        n_assert(SUCCEEDED(hr));
    }
    else if (Texture3D == this->type)
    {
        n_assert(0 != this->d3d9VolumeTexture);
        HRESULT hr = this->d3d9VolumeTexture->UnlockBox(mipLevel);
        n_assert(SUCCEEDED(hr));
    }
    this->mapCount--;
}

//------------------------------------------------------------------------------
/**
*/
bool
D3D9Texture::MapCubeFace(CubeFace face, IndexT mipLevel, MapType mapType, MapInfo& outMapInfo)
{
    n_assert(TextureCube == this->type);
    n_assert(0 != this->d3d9CubeTexture);
    n_assert(MapWriteNoOverwrite != mapType);
    DWORD lockFlags = D3DLOCK_NO_DIRTY_UPDATE | D3DLOCK_NOSYSLOCK;
    switch (mapType)
    {
        case MapRead:
            n_assert((UsageStaging == this->usage) && (AccessRead == this->access));
            lockFlags |= D3DLOCK_READONLY;
            break;
        case MapWrite:
            n_assert((UsageStaging == this->usage) && (AccessWrite == this->access));
            break;
        case MapReadWrite:
            n_assert((UsageStaging == this->usage) && (AccessReadWrite == this->access));
            break;
        case MapWriteDiscard:
            n_assert((UsageDynamic == this->usage) && (AccessWrite == this->access));
            lockFlags |= D3DLOCK_DISCARD;
            break;
    }
    D3DCUBEMAP_FACES d3d9Face = (D3DCUBEMAP_FACES) face;
    D3DLOCKED_RECT lockedRect = { 0 };
    HRESULT hr = this->d3d9CubeTexture->LockRect(d3d9Face, mipLevel, &lockedRect, NULL, lockFlags);
    if (SUCCEEDED(hr))
    {
        outMapInfo.data = lockedRect.pBits;
        outMapInfo.rowPitch = lockedRect.Pitch;
        outMapInfo.depthPitch = 0;
        this->mapCount++;
        return true;
    }
    else
    {
        return false;
    }
}

//------------------------------------------------------------------------------
/**
*/
void
D3D9Texture::UnmapCubeFace(CubeFace face, IndexT mipLevel)
{
    n_assert(TextureCube == this->type);
    n_assert(0 != this->d3d9CubeTexture);
    n_assert(this->mapCount > 0);
    D3DCUBEMAP_FACES d3d9Face = (D3DCUBEMAP_FACES) face;
    HRESULT hr = this->d3d9CubeTexture->UnlockRect(d3d9Face, mipLevel);
    n_assert(SUCCEEDED(hr));
    this->mapCount--;
}

//------------------------------------------------------------------------------
/**
    Helper method to setup the texture object from a D3D9 2D texture.
*/
void
D3D9Texture::SetupFromD3D9Texture(IDirect3DTexture9* tex2D)
{
    n_assert(0 != tex2D);    

    this->d3d9Texture = tex2D;
    HRESULT hr = d3d9Texture->QueryInterface(IID_IDirect3DBaseTexture9, (void**) &this->d3d9BaseTexture);
    n_assert(SUCCEEDED(hr));
    this->SetType(D3D9Texture::Texture2D);

    D3DSURFACE_DESC desc;
    Memory::Clear(&desc, sizeof(desc));
    hr = d3d9Texture->GetLevelDesc(0, &desc);
    n_assert(SUCCEEDED(hr));
    this->SetWidth(desc.Width);
    this->SetHeight(desc.Height);
    this->SetDepth(1);
    this->SetNumMipLevels(d3d9Texture->GetLevelCount());
    this->SetPixelFormat(D3D9Types::AsNebulaPixelFormat(desc.Format));
    this->SetState(Resource::Loaded);
}

//------------------------------------------------------------------------------
/**
    Helper method to setup the texture object from a D3D9 volume texture.
*/
void
D3D9Texture::SetupFromD3D9VolumeTexture(IDirect3DVolumeTexture9* texVolume)
{
    n_assert(0 != texVolume);

    this->d3d9VolumeTexture = texVolume;
    HRESULT hr = texVolume->QueryInterface(IID_IDirect3DBaseTexture9, (void**) &this->d3d9VolumeTexture);
    n_assert(SUCCEEDED(hr));
    this->SetType(D3D9Texture::Texture3D);

    D3DVOLUME_DESC desc;
    Memory::Clear(&desc, sizeof(desc));
    hr = d3d9VolumeTexture->GetLevelDesc(0, &desc);
    n_assert(SUCCEEDED(hr));
    this->SetWidth(desc.Width);
    this->SetHeight(desc.Height);
    this->SetDepth(desc.Depth);
    this->SetNumMipLevels(d3d9VolumeTexture->GetLevelCount());
    this->SetPixelFormat(D3D9Types::AsNebulaPixelFormat(desc.Format));
    this->SetState(Resource::Loaded);
}

//------------------------------------------------------------------------------
/**
    Helper method to setup the texture object from a D3D9 cube texture.
*/
void
D3D9Texture::SetupFromD3D9CubeTexture(IDirect3DCubeTexture9* texCube)
{
    n_assert(0 != texCube);

    this->d3d9CubeTexture = texCube;
    HRESULT hr = texCube->QueryInterface(IID_IDirect3DBaseTexture9, (void**) &this->d3d9BaseTexture);
    n_assert(SUCCEEDED(hr));
    this->SetType(D3D9Texture::TextureCube);

    D3DSURFACE_DESC desc;
    Memory::Clear(&desc, sizeof(desc));
    hr = d3d9CubeTexture->GetLevelDesc(0, &desc);
    n_assert(SUCCEEDED(hr));
    this->SetWidth(desc.Width);
    this->SetHeight(desc.Height);
    this->SetNumMipLevels(d3d9CubeTexture->GetLevelCount());
    this->SetPixelFormat(D3D9Types::AsNebulaPixelFormat(desc.Format));
    this->SetState(Resource::Loaded);
}

} // namespace CoreGraphics