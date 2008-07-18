#pragma once
#ifndef COREGRAPHICS_VERTEXLAYOUT_H
#define COREGRAPHICS_VERTEXLAYOUT_H
//------------------------------------------------------------------------------
/**
    @class CoreGraphics::VertexLayout
    
    Describe the layout of vertices in a vertex buffer.
    
    (C) 2006 Radon Labs GmbH
*/
#if __WIN32__
#include "coregraphics/d3d9/d3d9vertexlayout.h"
namespace CoreGraphics
{
class VertexLayout : public Direct3D9::D3D9VertexLayout
{
    DeclareClass(VertexLayout);
};
}
#elif __XBOX360__
#include "coregraphics/xbox360/xbox360vertexlayout.h"
namespace CoreGraphics
{
class VertexLayout : public Xbox360::Xbox360VertexLayout
{
    DeclareClass(VertexLayout);
};
}
#elif __WII__
#include "coregraphics/wii/wiivertexlayout.h"
namespace CoreGraphics
{
class VertexLayout : public Wii::WiiVertexLayout
{
    DeclareClass(VertexLayout);
};
}
#else
#error "VertexLayout class not implemented on this platform!"
#endif
//------------------------------------------------------------------------------
#endif
