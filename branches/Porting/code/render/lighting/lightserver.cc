//------------------------------------------------------------------------------
//  lightserver.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "lighting/lightserver.h"

namespace Lighting
{
#if (__WIN32__ || __XBOX360__)
ImplementClass(Lighting::LightServer, 'LISV', Lighting::SM30LightServer);
#elif __WII__
ImplementClass(Lighting::LightServer, 'LISV', Lighting::WiiLightServer);
#else
#error "LightServer class not implemented on this platform!"
#endif
ImplementSingleton(Lighting::LightServer);

//------------------------------------------------------------------------------
/**
*/
LightServer::LightServer()
{
    ConstructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
LightServer::~LightServer()
{
    DestructSingleton;
}

} // namespace Lighting


