#pragma once
#ifndef DIRECT3D9_D3D9TEXTURE_H
#define DIRECT3D9_D3D9TEXTURE_H
//------------------------------------------------------------------------------
/**
    @class Direct3D9::D3D9Texture
    
    D3D9 implementation of Texture class.

    FIXME: need to handle DeviceLost through RenderDevice event handler!
    
    (C) 2007 Radon Labs GmbH
*/
#include "coregraphics/base/texturebase.h"

//------------------------------------------------------------------------------
namespace Direct3D9
{
class D3D9Texture : public Base::TextureBase
{
    DeclareClass(D3D9Texture);
public:
    /// constructor
    D3D9Texture();
    /// destructor
    virtual ~D3D9Texture();

    /// unload the resource, or cancel the pending load
    virtual void Unload();
    /// map a texture mip level for CPU access
    bool Map(IndexT mipLevel, MapType mapType, MapInfo& outMapInfo);
    /// unmap texture after CPU access
    void Unmap(IndexT mipLevel);
    /// map a cube map face for CPU access
    bool MapCubeFace(CubeFace face, IndexT mipLevel, MapType mapType, MapInfo& outMapInfo);
    /// unmap cube map face after CPU access
    void UnmapCubeFace(CubeFace face, IndexT mipLevel);

    /// get d3d9 base texture pointer
    IDirect3DBaseTexture9* GetD3D9BaseTexture() const;
    /// get d3d9 texture pointer
    IDirect3DTexture9* GetD3D9Texture() const;
    /// get d3d9 cube texture pointer
    IDirect3DCubeTexture9* GetD3D9CubeTexture() const;
    /// get d3d9 volume texture pointer
    IDirect3DVolumeTexture9* GetD3D9VolumeTexture() const;

protected:
    friend class D3D9StreamTextureLoader;
    friend class D3D9RenderTarget;

    /// setup from a IDirect3DTexture9
    void SetupFromD3D9Texture(IDirect3DTexture9* ptr);
    /// setup from a IDirect3DCubeTexture
    void SetupFromD3D9CubeTexture(IDirect3DCubeTexture9* ptr);
    /// setup from a IDirect3DVolumeTexture
    void SetupFromD3D9VolumeTexture(IDirect3DVolumeTexture9* ptr);

    IDirect3DBaseTexture9* d3d9BaseTexture;
    IDirect3DTexture9* d3d9Texture;                 // valid if type is Texture2D
    IDirect3DCubeTexture9* d3d9CubeTexture;         // valid if type is TextureCube
    IDirect3DVolumeTexture9* d3d9VolumeTexture;     // valid if type is Texture3D
    int mapCount;
};

//------------------------------------------------------------------------------
/**
*/
inline IDirect3DBaseTexture9*
D3D9Texture::GetD3D9BaseTexture() const
{
    n_assert(0 != this->d3d9BaseTexture);
    n_assert(0 == this->mapCount);
    return this->d3d9BaseTexture;
}

//------------------------------------------------------------------------------
/**
*/
inline IDirect3DTexture9*
D3D9Texture::GetD3D9Texture() const
{
    n_assert(0 != this->d3d9Texture);
    n_assert(0 == this->mapCount);
    return this->d3d9Texture;
}

//------------------------------------------------------------------------------
/**
*/
inline IDirect3DCubeTexture9*
D3D9Texture::GetD3D9CubeTexture() const
{
    n_assert(0 != this->d3d9CubeTexture);
    n_assert(0 == this->mapCount);
    return this->d3d9CubeTexture;
}

//------------------------------------------------------------------------------
/**
*/
inline IDirect3DVolumeTexture9*
D3D9Texture::GetD3D9VolumeTexture() const
{
    n_assert(0 != this->d3d9VolumeTexture);
    n_assert(0 == this->mapCount);
    return this->d3d9VolumeTexture;
}

} // namespace Direct3D9
//------------------------------------------------------------------------------
#endif
    