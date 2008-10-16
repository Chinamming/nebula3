//------------------------------------------------------------------------------
//  texture.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "coregraphics/texture.h"

#if (__WIN32__ || __XBOX360__)
namespace CoreGraphics
{
__ImplementClass(CoreGraphics::Texture, 'TEXR', Win360::D3D9Texture);
}
#elif __WII__
namespace CoreGraphics
{
__ImplementClass(CoreGraphics::Texture, 'TEXR', Wii::WiiTexture);
}

#else
#error "Texture class not implemented on this platform!"
#endif
