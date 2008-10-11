//------------------------------------------------------------------------------
//  streamtextureloader.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "coregraphics/streamtextureloader.h"

#if (__WIN32__ || __XBOX360__)
namespace CoreGraphics
{
__ImplementClass(CoreGraphics::StreamTextureLoader, 'STXL', Win360::D3D9StreamTextureLoader);
}
#elif __WII__
namespace CoreGraphics
{
__ImplementClass(CoreGraphics::StreamTextureLoader, 'STXL', Wii::WiiStreamTextureLoader);
}
#else
#error "StreamTextureLoader class not implemented on this platform!"
#endif
