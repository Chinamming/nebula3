//------------------------------------------------------------------------------
//  d3d9rendertarget.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "coregraphics/config.h"
#include "coregraphics/d3d9/d3d9rendertarget.h"
#include "coregraphics/d3d9/d3d9renderdevice.h"
#include "coregraphics/d3d9/d3d9types.h"
#include "coregraphics/displaydevice.h"
#include "resources/sharedresourceserver.h"

namespace Direct3D9
{
ImplementClass(Direct3D9::D3D9RenderTarget, 'D9RT', Base::RenderTargetBase);

using namespace CoreGraphics;
using namespace Resources;

//------------------------------------------------------------------------------
/**
*/
D3D9RenderTarget::D3D9RenderTarget() :
    d3d9RenderTarget(0),
    d3d9DepthStencil(0),
    d3d9ResolveTexture(0),
    d3d9MultiSampleType(D3DMULTISAMPLE_NONE),
    d3d9MultiSampleQuality(0),
    d3d9ColorBufferFormat(D3DFMT_UNKNOWN),
    needsResolve(false)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
D3D9RenderTarget::~D3D9RenderTarget()
{
    n_assert(!this->isValid);
    n_assert(0 == this->d3d9RenderTarget);
    n_assert(0 == this->d3d9DepthStencil);
    n_assert(0 == this->d3d9ResolveTexture);
}

//------------------------------------------------------------------------------
/**
*/
void
D3D9RenderTarget::Setup()
{
    n_assert(0 == this->d3d9RenderTarget);
    n_assert(0 == this->d3d9DepthStencil);
    n_assert(0 == this->d3d9ResolveTexture);
    HRESULT hr;
    IDirect3DDevice9* d3d9Dev = D3D9RenderDevice::Instance()->GetDirect3DDevice();
    
    // call parent class
    RenderTargetBase::Setup();

    // if we're the default render target, query display device
    // for setup parameters
    if (this->isDefaultRenderTarget)
    {
        // NOTE: the default render target will never be anti-aliased!
        // this assumes a render pipeline where the actual rendering goes
        // into an offscreen render target and is then resolved to the back buffer
        DisplayDevice* displayDevice = DisplayDevice::Instance();
        this->SetWidth(displayDevice->GetDisplayMode().GetWidth());
        this->SetHeight(displayDevice->GetDisplayMode().GetHeight());
        this->SetAntiAliasQuality(AntiAliasQuality::None);
        this->AddColorBuffer(displayDevice->GetDisplayMode().GetPixelFormat());
    }

    // setup our pixel format and multisample parameters (order important!)
    this->d3d9ColorBufferFormat = D3D9Types::AsD3D9PixelFormat(this->colorBufferFormats[0]);
    this->SetupMultiSampleType();

    // check if a resolve texture must be allocated
    if (this->mipMapsEnabled ||
        (D3DMULTISAMPLE_NONE != this->d3d9MultiSampleType) ||
        (this->resolveTextureDimensionsValid &&
         ((this->resolveTextureWidth != this->width) ||
          (this->resolveTextureHeight != this->height))))
    {
        this->needsResolve = true;
    }
    else
    {
        this->needsResolve = false;
    }

    // create the render target either as a texture, or as
    // a surface, or don't create it if rendering goes
    // into backbuffer
    if (!this->needsResolve)
    {
        if (!this->isDefaultRenderTarget)
        {
            // inplace resolve texture can be used (render target == resolve texture)
            hr = d3d9Dev->CreateTexture(this->width,                    // Width
                                        this->height,                   // Height
                                        1,                              // Levels
                                        D3DUSAGE_RENDERTARGET,          // Usage
                                        this->d3d9ColorBufferFormat,    // Format
                                        D3DPOOL_DEFAULT,                // Pool
                                        &(this->d3d9ResolveTexture),    // ppTexture
                                        NULL);                          // pSharedHandle
            n_assert(SUCCEEDED(hr));
            n_assert(0 != this->d3d9ResolveTexture);
            
            // get the actual render target surface of the texture
            hr = this->d3d9ResolveTexture->GetSurfaceLevel(0, &(this->d3d9RenderTarget));
            n_assert(SUCCEEDED(hr));
        }
        else
        {
            // NOTE: if we are the default render target and not antialiased, 
            // rendering will go directly to the backbuffer, so there's no
            // need to allocate a render target
        }
    }
    else
    {
        // need to create an extra resolve texture, create the
        // actual render target directly as surface
        hr = d3d9Dev->CreateRenderTarget(this->width,                   // Width
                                         this->height,                  // Height
                                         this->d3d9ColorBufferFormat,   // Format
                                         this->d3d9MultiSampleType,     // MultiSample
                                         this->d3d9MultiSampleQuality,  // MultisampleQuality
                                         FALSE,                         // Lockable
                                         &(this->d3d9RenderTarget),     // ppSurface
                                         NULL);                         // pSharedHandle
        n_assert(SUCCEEDED(hr));
        n_assert(0 != this->d3d9RenderTarget);

        // create the resolve texture
        SizeT resolveWidth = this->resolveTextureDimensionsValid ? this->resolveTextureWidth : this->width;
        SizeT resolveHeight = this->resolveTextureDimensionsValid ? this->resolveTextureHeight : this->height;

        DWORD usage = D3DUSAGE_RENDERTARGET;
        if (this->mipMapsEnabled)
        {
            usage |= D3DUSAGE_AUTOGENMIPMAP;
        }
        hr = d3d9Dev->CreateTexture(resolveWidth,                   // Width
                                    resolveHeight,                  // Height
                                    1,                              // Levels
                                    usage,                          // Usage
                                    this->d3d9ColorBufferFormat,    // Format
                                    D3DPOOL_DEFAULT,                // Pool
                                    &(this->d3d9ResolveTexture),    // ppTexture
                                    NULL);                          // pSharedHandle
        n_assert(SUCCEEDED(hr));
        n_assert(0 != this->d3d9RenderTarget);
    }

    // create the depth/stencil buffer if needed
    if (this->HasDepthStencilBuffer())
    {
        hr = d3d9Dev->CreateDepthStencilSurface(this->width,                   // Width
                                                this->height,                  // Height
                                                D3DFMT_D24S8,                  // Format
                                                this->d3d9MultiSampleType,     // MultiSample
                                                this->d3d9MultiSampleQuality,  // MultisampleQuality
                                                TRUE,                          // Discard
                                                &(this->d3d9DepthStencil),     // ppSurface
                                                NULL);                         // pSharedHandle
        n_assert(SUCCEEDED(hr));
        n_assert(0 != this->d3d9DepthStencil);
    }

    // if a resolve texture exists, create a shared texture resource, so that
    // the texture is publicly visible
    if ((0 != this->d3d9ResolveTexture) && this->resolveTextureResId.IsValid())
    {
        this->resolveTexture = SharedResourceServer::Instance()->CreateSharedResource(this->resolveTextureResId, Texture::RTTI).downcast<Texture>();
        this->resolveTexture->SetupFromD3D9Texture(this->d3d9ResolveTexture);
        this->d3d9ResolveTexture->AddRef();
    }
}

//------------------------------------------------------------------------------
/**
*/
void
D3D9RenderTarget::Discard()
{
    RenderTargetBase::Discard();
    if (0 != this->d3d9RenderTarget)
    {
        this->d3d9RenderTarget->Release();
        this->d3d9RenderTarget = 0;
    }
    if (0 != this->d3d9DepthStencil)
    {
        this->d3d9DepthStencil->Release();
        this->d3d9DepthStencil = 0;
    }
    if (0 != this->d3d9ResolveTexture)
    {
        this->d3d9ResolveTexture->Release();
        this->d3d9ResolveTexture = 0;
    }
}

//------------------------------------------------------------------------------
/**
    Select the antialias parameters that most closely resembly 
    the preferred settings in the DisplayDevice object.
*/
void
D3D9RenderTarget::SetupMultiSampleType()
{
    n_assert(D3DFMT_UNKNOWN != this->d3d9ColorBufferFormat);
    D3D9RenderDevice* renderDevice = D3D9RenderDevice::Instance();
    IDirect3D9* d3d9 = renderDevice->GetDirect3D();

    #if NEBULA3_DIRECT3D_DEBUG
        this->d3d9MultiSampleType = D3DMULTISAMPLE_NONE;
        this->d3d9MultiSampleQuality = 0;
    #else
        // convert Nebula3 antialias quality into D3D type
        this->d3d9MultiSampleType = D3D9Types::AsD3D9MultiSampleType(this->antiAliasQuality);
        
        // check if the multisample type is compatible with the selected display mode
        DWORD availableQualityLevels = 0;
        HRESULT renderTargetResult = d3d9->CheckDeviceMultiSampleType(0, 
                                     NEBULA3_DIRECT3D_DEVICETYPE,
                                     this->d3d9ColorBufferFormat,
                                     FALSE,
                                     this->d3d9MultiSampleType,
                                     &availableQualityLevels);
        HRESULT depthBufferResult = d3d9->CheckDeviceMultiSampleType(0,
                                    NEBULA3_DIRECT3D_DEVICETYPE,
                                    D3DFMT_D24S8,
                                    FALSE,
                                    this->d3d9MultiSampleType,
                                    NULL);
        if ((D3DERR_NOTAVAILABLE == renderTargetResult) || (D3DERR_NOTAVAILABLE == depthBufferResult))
        {
            // reset to no multisampling
            this->d3d9MultiSampleType = D3DMULTISAMPLE_NONE;
            this->d3d9MultiSampleQuality = 0;
        }
        else
        {
            n_assert(SUCCEEDED(renderTargetResult) && SUCCEEDED(depthBufferResult));
        }

        // clamp multisample quality to the available quality levels
        if (availableQualityLevels > 0)
        {
            this->d3d9MultiSampleQuality = availableQualityLevels - 1;
        }
        else
        {
            this->d3d9MultiSampleQuality = 0;
        }
    #endif
}  

//------------------------------------------------------------------------------
/**
*/
void
D3D9RenderTarget::BeginPass()
{
    HRESULT hr;
    IDirect3DDevice9* d3d9Dev = D3D9RenderDevice::Instance()->GetDirect3DDevice();

    // apply the render target (may be the back buffer)
    DWORD clearFlags = D3DCLEAR_TARGET;
    if (0 != this->d3d9RenderTarget)
    {
        d3d9Dev->SetRenderTarget(0, this->d3d9RenderTarget);
    }
    else
    {
        n_assert(this->IsDefaultRenderTarget());
        IDirect3DSurface9* backBuffer = 0;
        hr = d3d9Dev->GetBackBuffer(0,0, D3DBACKBUFFER_TYPE_MONO, &backBuffer);
        n_assert(SUCCEEDED(hr));
        hr = d3d9Dev->SetRenderTarget(0, backBuffer);
        n_assert(SUCCEEDED(hr));
        backBuffer->Release();        
    }
    // apply the depth stencil buffer
    if (0 != this->d3d9DepthStencil)
    {
        hr = d3d9Dev->SetDepthStencilSurface(this->d3d9DepthStencil);
        n_assert(SUCCEEDED(hr));
        clearFlags |= D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL;
    }

    // clear the render target
    RenderTargetBase::BeginPass();
    hr = d3d9Dev->Clear(0,             // Count
                        NULL,          // pRects
                        clearFlags,    // Flags
                        D3DCOLOR_COLORVALUE(this->clearColor.x(), 
                            this->clearColor.y(), 
                            this->clearColor.z(), 
                            this->clearColor.w()),  // Color
                        this->clearDepth,           // Z
                        this->clearStencil);        // Stencil
    n_assert(SUCCEEDED(hr));
}

//------------------------------------------------------------------------------
/**
*/
void
D3D9RenderTarget::EndPass()
{
    HRESULT hr;
    IDirect3DDevice9* d3d9Dev = D3D9RenderDevice::Instance()->GetDirect3DDevice();

    // if necessary need to resolve the render target, either
    // into our resolve texture, or into the back buffer
    if (this->needsResolve)
    {
        RECT destRect;
        CONST RECT* pDestRect = NULL;
        if (this->resolveRectValid)
        {
            destRect.left   = this->resolveRect.left;
            destRect.right  = this->resolveRect.right;
            destRect.top    = this->resolveRect.top;
            destRect.bottom = this->resolveRect.bottom;
            pDestRect = &destRect;
        }
        IDirect3DSurface9* resolveSurface = 0;
        hr = this->d3d9ResolveTexture->GetSurfaceLevel(0, &resolveSurface);
        hr = d3d9Dev->StretchRect(this->d3d9RenderTarget, NULL, resolveSurface, pDestRect, D3DTEXF_NONE);
        n_assert(SUCCEEDED(hr));
        resolveSurface->Release();
    }
    RenderTargetBase::EndPass();
}

//------------------------------------------------------------------------------
/**
*/
void
D3D9RenderTarget::GenerateMipLevels()
{
    n_assert(0 != this->d3d9ResolveTexture);
    n_assert(this->mipMapsEnabled);
    this->d3d9ResolveTexture->GenerateMipSubLevels();
}

} // namespace Direct3D9
