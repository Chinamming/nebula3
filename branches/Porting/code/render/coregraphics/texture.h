#pragma once
#ifndef COREGRAPHICS_TEXTURE_H
#define COREGRAPHICS_TEXTURE_H
//------------------------------------------------------------------------------
/**
    @class CoreGraphics::Texture
    
    Front-end class for texture objects.
    
    (C) 2007 Radon Labs GmbH
*/
#if (__WIN32__ || __XBOX360__)
#include "coregraphics/win360/d3d9texture.h"
namespace CoreGraphics
{
class Texture : public Win360::D3D9Texture
{
    __DeclareClass(Texture);
};
}
#elif __WII__
#include "coregraphics/wii/wiitexture.h"
namespace CoreGraphics
{
class Texture : public Wii::WiiTexture
{
    __DeclareClass(Texture);
};
}
#else
#error "Texture class not implemented on this platform!"
#endif
//------------------------------------------------------------------------------
#endif
