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
#if (__WIN32__ || __XBOX360__)
#include "coregraphics/win360/d3d9streamtextureloader.h"
namespace CoreGraphics
{
class StreamTextureLoader : public Win360::D3D9StreamTextureLoader
{
    __DeclareClass(StreamTextureLoader);
};
}
#elif __WII__
#include "coregraphics/wii/wiistreamtextureloader.h"
namespace CoreGraphics
{
class StreamTextureLoader : public Wii::WiiStreamTextureLoader
{
    __DeclareClass(StreamTextureLoader);
};
}
#else
#error "StreamTextureLoader class not implemented on this platform!"
#endif
//------------------------------------------------------------------------------
#endif

