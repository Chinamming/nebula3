#pragma once
#ifndef COREGRAPHICS_STREAMTEXTURESAVER_H
#define COREGRAPHICS_STREAMTEXTURESAVER_H
//------------------------------------------------------------------------------
/**
    @class CoreGraphics::StreamTextureSaver
    
    Allows to save texture data in a standard file format into a stream.    
    
    (C) 2007 Radon Labs GmbH
*/
#if __WIN32__
#include "coregraphics/d3d9/d3d9streamtexturesaver.h"
namespace CoreGraphics
{
class StreamTextureSaver : public Direct3D9::D3D9StreamTextureSaver
{
    DeclareClass(StreamTextureSaver);
};
}
#elif __XBOX360__
#include "coregraphics/xbox360/xbox360streamtexturesaver.h"
namespace CoreGraphics
{
class StreamTextureSaver : public Xbox360::Xbox360StreamTextureSaver
{
    DeclareClass(StreamTextureSaver);
};
}
#elif __WII__

#else
#error "StreamTextureSaver class not implemented on this platform!"
#endif
//------------------------------------------------------------------------------
#endif
    