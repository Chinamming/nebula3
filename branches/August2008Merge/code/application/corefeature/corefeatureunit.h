#ifndef COREFEATURE_COREFEATUREUNIT_H
#define COREFEATURE_COREFEATUREUNIT_H
//------------------------------------------------------------------------------
/**
    @class Game::CoreFeature
    
    The CoreFeature just creates all core systems of Nebula3.
    That are Core, Io, Script and the Http subsystems.

    (C) 2007 Radon Labs GmbH
*/
#include "game/featureunit.h"
#include "core/coreserver.h"
#include "io/ioserver.h"
#include "interface/iointerface.h"
#include "appgame/appconfig.h"
#include "debug/debuginterface.h"

#if __NEBULA3_SCRIPTING__
#include "scripting/scriptserver.h"
#endif
#if __NEBULA3_HTTP__
#include "http/httpinterface.h"
#include "http/httpserverproxy.h"
#endif

//------------------------------------------------------------------------------
namespace CoreFeature
{

class CoreFeatureUnit : public Game::FeatureUnit    
{
    DeclareClass(CoreFeatureUnit);
    DeclareSingleton(CoreFeatureUnit);    

public:
    /// constructor
    CoreFeatureUnit();
    /// destructor
    virtual ~CoreFeatureUnit();

    /// called from GameServer::ActivateProperties()
    virtual void OnActivate();
    /// called from GameServer::DeactivateProperties()
    virtual void OnDeactivate();
   
    /// called on begin of frame
    virtual void OnBeginFrame();    
        
protected:
    Ptr<Core::CoreServer> coreServer;
    Ptr<Debug::DebugInterface> debugInterface;
    Ptr<IO::IoServer> ioServer;
    Ptr<Interface::IOInterface> ioInterface;  
#if __NEBULA3_SCRIPTING__    
    Ptr<Scripting::ScriptServer> scriptServer; 
#endif    
#if __NEBULA3_HTTP__
    Ptr<Http::HttpInterface> httpInterface;
    Ptr<Http::HttpServerProxy> httpServerProxy;
#endif
};

}; // namespace CoreFeature
//------------------------------------------------------------------------------
#endif