//------------------------------------------------------------------------------
//  shadowserver.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "lighting/shadowserver.h"

namespace Lighting
{
#if (__WIN32__ || __XBOX360__)
ImplementClass(Lighting::ShadowServer, 'SDSV', Lighting::SM30ShadowServer);
#elif __WII__
ImplementClass(Lighting::ShadowServer, 'SDSV', Lighting::WiiShadowServer);
#else
#error "ShadowServer class not implemented on this platform!"
#endif
ImplementSingleton(Lighting::ShadowServer);

//------------------------------------------------------------------------------
/**
*/
ShadowServer::ShadowServer()
{
    ConstructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
ShadowServer::~ShadowServer()
{
    DestructSingleton;
}

} // namespace Lighting
