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

#if __USE_SCRIPTING__
#include "scripting/scriptserver.h"
#endif
#if __USE_HTTP__
#include "http/httpserver.h"
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
    Ptr<IO::IoServer> ioServer;
    Ptr<Interface::IOInterface> ioInterface;  
#if __USE_SCRIPTING__    
    Ptr<Scripting::ScriptServer> scriptServer; 
#endif    
#if __USE_HTTP__
    Ptr<Http::HttpServer> httpServer;
#endif
};

}; // namespace CoreFeature
//------------------------------------------------------------------------------
#endif