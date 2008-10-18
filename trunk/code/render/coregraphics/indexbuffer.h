#pragma once
#ifndef COREGRAPHICS_INDEXBUFFER_H
#define COREGRAPHICS_INDEXBUFFER_H
//------------------------------------------------------------------------------
/**
    @class CoreGraphics::IndexBuffer
  
    A resource which holds an array of indices into an array of vertices.  
    
    (C) 2007 Radon Labs GmbH
*/
#if (__WIN32__ || __XBOX360__)
#include "coregraphics/win360/d3d9indexbuffer.h"
namespace CoreGraphics
{
class IndexBuffer : public Win360::D3D9IndexBuffer
{
    __DeclareClass(IndexBuffer);
};
}
#elif __WII__
#include "coregraphics/wii/wiiindexbuffer.h"
namespace CoreGraphics
{
class IndexBuffer : public Wii::WiiIndexBuffer
{
    __DeclareClass(IndexBuffer);
};
}
#else
#error "IndexBuffer class not implemented on this platform!"
#endif
//------------------------------------------------------------------------------
#endif
