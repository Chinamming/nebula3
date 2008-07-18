#pragma once
#ifndef COREGRAPHICS_STREAMTEXTURELOADER_H
#define COREGRAPHICS_STREAMTEXTURELOADER_H
//------------------------------------------------------------------------------
/**
    @class CoreGraphics::StreamTextureLoader
  
    Resource loader for loading texture data from a Nebula3 stream. Supports
    synchronous and asynchronous loading.
    
    (C) 2007 Radon Labs GmbH
*/    
#if __WIN32__
#include "coregraphics/d3d9/d3d9streamtextureloader.h"
namespace CoreGraphics
{
class StreamTextureLoader : public Direct3D9::D3D9StreamTextureLoader
{
    DeclareClass(StreamTextureLoader);
};
}
#elif __XBOX360__
#include "coregraphics/xbox360/xbox360streamtextureloader.h"
namespace CoreGraphics
{
class StreamTextureLoader : public Xbox360::Xbox360StreamTextureLoader
{
    DeclareClass(StreamTextureLoader);
};
}
#elif __WII__
#include "coregraphics/wii/wiistreamtextureloader.h"
namespace CoreGraphics
{
class StreamTextureLoader : public Wii::WiiStreamTextureLoader
{
    DeclareClass(StreamTextureLoader);
};
}
#else
#error "StreamTextureLoader class not implemented on this platform!"
#endif
//------------------------------------------------------------------------------
#endif

