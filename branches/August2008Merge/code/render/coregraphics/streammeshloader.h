#pragma once
#ifndef COREGRAPHICS_STREAMMESHLOADER_H
#define COREGRAPHICS_STREAMMESHLOADER_H
//------------------------------------------------------------------------------
/**
    @class CoreGraphics::StreamMeshLoader
    
    Resource loader to setup a Mesh object from a stream.
    
    (C) 2008 Radon Labs GmbH
*/
#if __WIN32__
#include "coregraphics/d3d9/d3d9streammeshloader.h"
namespace CoreGraphics
{
class StreamMeshLoader : public Direct3D9::D3D9StreamMeshLoader
{
    DeclareClass(StreamMeshLoader);
};
}
#elif __XBOX360__
#include "coregraphics/xbox360/xbox360streammeshloader.h"
namespace CoreGraphics
{
class StreamMeshLoader : public Xbox360::Xbox360StreamMeshLoader
{
    DeclareClass(StreamMeshLoader);
};
}
#elif __WII__
#include "coregraphics/wii/wiistreammeshloader.h"
namespace CoreGraphics
{
class StreamMeshLoader : public Wii::WiiStreamMeshLoader
{
    DeclareClass(StreamMeshLoader);
};
}
#else
#error "StreamMeshLoader class not implemented on this platform!"
#endif
//------------------------------------------------------------------------------
#endif
    