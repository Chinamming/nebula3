#pragma once
#ifndef COREGRAPHICS_RENDERTARGET_H
#define COREGRAPHICS_RENDERTARGET_H
//------------------------------------------------------------------------------
/**
    @class CoreGraphics::RenderTarget
    
    A render targets wraps up to 4 color buffers and an optional depth/stencil 
    buffer into a C++ object. The special default render target represents the 
    backbuffer and default depth/stencil surface.

    (C) 2007 Radon Labs GmbH
*/
#if __WIN32__
#include "coregraphics/d3d9/d3d9rendertarget.h"
namespace CoreGraphics
{
class RenderTarget : public Direct3D9::D3D9RenderTarget
{
    __DeclareClass(RenderTarget);
};
}
#elif __XBOX360__
#include "coregraphics/xbox360/xbox360rendertarget.h"
namespace CoreGraphics
{
class RenderTarget : public Xbox360::Xbox360RenderTarget
{
    __DeclareClass(RenderTarget);
};
}
#elif __WII__
#include "coregraphics/wii/wiirendertarget.h"
namespace CoreGraphics
{
class RenderTarget : public Wii::WiiRenderTarget
{
    __DeclareClass(RenderTarget);
};
}
#else
#error "RenderTarget class not implemented on this platform!"
#endif
//------------------------------------------------------------------------------
#endif
