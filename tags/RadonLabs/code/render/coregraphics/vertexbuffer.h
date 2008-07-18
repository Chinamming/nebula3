#pragma once
#ifndef COREGRAPHICS_VERTEXBUFFER_H
#define COREGRAPHICS_VERTEXBUFFER_H
//------------------------------------------------------------------------------
/**
    @class CoreGraphics::VertexBuffer
  
    A resource which holds an array of vertices.
    
    (C) 2007 Radon Labs GmbH
*/    
#if __WIN32__
#include "coregraphics/d3d9/d3d9vertexbuffer.h"
namespace CoreGraphics
{
class VertexBuffer : public Direct3D9::D3D9VertexBuffer
{
    DeclareClass(VertexBuffer);
};
}
#elif __XBOX360__
#include "coregraphics/xbox360/xbox360vertexbuffer.h"
namespace CoreGraphics
{
class VertexBuffer : public Xbox360::Xbox360VertexBuffer
{
    DeclareClass(VertexBuffer);
};
}
#elif __WII__
#include "coregraphics/wii/wiivertexbuffer.h"
namespace CoreGraphics
{
class VertexBuffer : public Wii::WiiVertexBuffer
{
    DeclareClass(VertexBuffer);
};
}
#else
#error "VertexBuffer class not implemented on this platform!"
#endif
//------------------------------------------------------------------------------
#endif
