//------------------------------------------------------------------------------
//  shaderserver.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "coregraphics/shaderserver.h"

namespace CoreGraphics
{
#if __WIN32__
ImplementClass(CoreGraphics::ShaderServer, 'SHSV', Direct3D9::D3D9ShaderServer);
ImplementSingleton(CoreGraphics::ShaderServer);
#elif __XBOX360__
// FIXME!
ImplementClass(CoreGraphics::ShaderServer, 'SHSV', Xbox360::Xbox360ShaderServer);
ImplementSingleton(CoreGraphics::ShaderServer);
#elif __WII__
ImplementClass(CoreGraphics::ShaderServer, 'SHSV', Wii::WiiShaderServer);
ImplementSingleton(CoreGraphics::ShaderServer);
#else
#error "ShaderServer class not implemented on this platform!"
#endif

//------------------------------------------------------------------------------
/**
*/
ShaderServer::ShaderServer()
{
    ConstructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
ShaderServer::~ShaderServer()
{
    DestructSingleton;
}

} // namespace CoreGraphics
