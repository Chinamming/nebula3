#pragma once
#ifndef LIGHTING_SHADOWSERVER_H
#define LIGHTING_SHADOWSERVER_H
//------------------------------------------------------------------------------
/**
    @class Lighting::ShadowServer
    
    The ShadowServer setups and controls the global aspects of the dynamic
    shadow system.
    
    (C) 2007 Radon Labs GmbH
*/
#if (__WIN32__ || __XBOX360__)
#include "lighting/sm30/sm30shadowserver.h"
namespace Lighting
{
class ShadowServer : public SM30ShadowServer
{
    __DeclareClass(ShadowServer);
    __DeclareSingleton(ShadowServer);
public:
    /// constructor
    ShadowServer();
    /// destructor
    virtual ~ShadowServer();
};
} // namespace Lighting
#elif __WII__
#include "lighting/wii/wiishadowserver.h"
namespace Lighting
{
class ShadowServer : public WiiShadowServer
{
    __DeclareClass(ShadowServer);
    __DeclareSingleton(ShadowServer);
public:
    /// constructor
    ShadowServer();
    /// destructor
    virtual ~ShadowServer();
};
} // namespace Lighting
#else
#error "ShadowServer class not implemented on this platform!"
#endif
//------------------------------------------------------------------------------
#endif
