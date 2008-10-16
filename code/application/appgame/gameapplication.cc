//------------------------------------------------------------------------------
//  gameapplication.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "appgame/gameapplication.h"
#include "graphicsfeature/graphicsfeatureunit.h"
#include "core/debug/corepagehandler.h"
#include "threading/debug/threadpagehandler.h"
#include "memory/debug/memorypagehandler.h"
#include "io/debug/iopagehandler.h"

namespace App
{
using namespace Util;

__ImplementSingleton(App::GameApplication);

using namespace Util;
using namespace Core;
using namespace IO;
using namespace Http;
using namespace Interface;
using namespace Debug;

//------------------------------------------------------------------------------
/**
*/
GameApplication::GameApplication()
{
    __ConstructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
GameApplication::~GameApplication()
{
    n_assert(!this->IsOpen());
    __DestructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
bool
GameApplication::Open()
{
    n_assert(!this->IsOpen());
    if (Application::Open())
    {
        // setup basic Nebula3 runtime system
        this->coreServer = CoreServer::Create();
        this->coreServer->SetCompanyName(Application::Instance()->GetCompanyName());
        this->coreServer->SetAppName(Application::Instance()->GetAppName());
        this->coreServer->Open();

        // setup io subsystem
        this->ioServer = IoServer::Create();
        this->ioServer->RegisterStandardUriSchemes();
        this->ioServer->SetupStandardAssigns();
        this->ioServer->MountStandardZipArchives();

        this->ioInterface = IOInterface::Create();
        this->ioInterface->Open();

        // setup http subsystem
        this->httpInterface = Http::HttpInterface::Create();
        this->httpInterface->Open();
        this->httpServerProxy = Http::HttpServerProxy::Create();
        this->httpServerProxy->Open();
        this->httpServerProxy->AttachRequestHandler(Debug::CorePageHandler::Create());
        this->httpServerProxy->AttachRequestHandler(Debug::ThreadPageHandler::Create());
        this->httpServerProxy->AttachRequestHandler(Debug::MemoryPageHandler::Create());
        this->httpServerProxy->AttachRequestHandler(Debug::IoPageHandler::Create());

        // setup remote subsystem
        this->remoteInterface = Remote::RemoteInterface::Create();
        this->remoteInterface->Open();
        this->remoteControlProxy = Remote::RemoteControlProxy::Create();
        this->remoteControlProxy->Open();

        // setup debug subsystem
        this->debugInterface = DebugInterface::Create();
        this->debugInterface->Open();

        // create our game server and open it
        this->gameServer = Game::GameServer::Create();
        this->gameServer->Open();

        // create and add new game features
        this->SetupGameFeatures();

        // setup application state handlers
        this->curState.Clear();
        this->nextState.Clear();
        this->SetupStateHandlers();

        // setup profiling stuff
        _setup_timer(GameApplicationFrameTimeAll);

        return true;
    }
    return false;
}

//------------------------------------------------------------------------------
/**
*/
void
GameApplication::Close()
{
    n_assert(this->IsOpen());

    _discard_timer(GameApplicationFrameTimeAll);

    this->CleanupGameFeatures();
    this->CleanupStateHandlers();
    this->gameServer->Close();
    this->gameServer = 0;

    this->debugInterface->Close();
    this->debugInterface = 0;

    this->remoteControlProxy->Close();
    this->remoteControlProxy = 0;
    this->remoteInterface->Close();
    this->remoteInterface = 0;

    // shutdown basic Nebula3 runtime
    this->httpServerProxy->Close();
    this->httpServerProxy = 0;
    this->httpInterface->Close();
    this->httpInterface = 0;
        
    this->ioInterface->Close();
    this->ioInterface = 0;
    this->ioServer = 0;

    this->coreServer->Close();
    this->coreServer = 0;

    Application::Close();
}

//------------------------------------------------------------------------------
/**
    Setup the application state handlers. This method is called by App::Open()
    after the Mangalore subsystems have been initialized. Override this method
    to create and attach your application state handlers with the 
    application object.
*/
void
GameApplication::SetupStateHandlers()
{
    // FIXME
}

//------------------------------------------------------------------------------
/**
    Cleanup the application state handlers. This will call the
    OnRemoveFromApplication() method on all attached state handlers 
    and release them. Usually you don't need to override this method in
    your app.
*/
void
GameApplication::CleanupStateHandlers()
{
    // release game state handlers
    this->requestedState.Clear();
    this->curState.Clear();
    this->nextState.Clear();

    // cleanup state handlers
    int i;
    int num = this->stateHandlers.Size();
    for (i = 0; i < num; i++)
    {
        this->stateHandlers.ValueAtIndex(i)->OnRemoveFromApplication();
    }
    this->stateHandlers.Clear();
}

//------------------------------------------------------------------------------
/**
    Run the application. This method will return when the application wishes 
    to exist.
*/
void
GameApplication::Run()
{
    while (this->GetCurrentState() != "Exit")
    {
        _start_timer(GameApplicationFrameTimeAll)

        this->httpServerProxy->HandlePendingRequests(); 
        this->coreServer->Trigger();

        // first trigger our game server, which triggers all game features
        this->gameServer->OnFrame();

        // call the current state handler
        const Ptr<StateHandler>& curStateHandler = this->FindStateHandlerByName(this->GetCurrentState());
        n_assert(curStateHandler);
        String newState = curStateHandler->OnFrame();

        // call the app's Run() method
        Application::Run();

        // a requested state always overrides the returned state
        if (this->requestedState.IsValid())
        {
            this->SetState(this->requestedState);
        }
        else if (newState != curStateHandler->GetName())
        {
            // a normal state transition
            this->SetState(newState);
        }
        _stop_timer(GameApplicationFrameTimeAll);
    }
}

//------------------------------------------------------------------------------
/**
    Do a state transition. This method is called by SetState() when the
    new state is different from the previous state.
*/
void
GameApplication::DoStateTransition()
{
    // cleanup previous state
    if (this->curState.IsValid())
    {
        const Ptr<StateHandler>& curStateHandler = this->FindStateHandlerByName(this->curState);
        n_assert(curStateHandler);
        curStateHandler->OnStateLeave(this->nextState);
    }

    // initialize new state
    String prevState = this->curState;
    this->curState = this->nextState;
    if (this->nextState.IsValid() && this->nextState != "Exit")
    {
        const Ptr<StateHandler>& nextStateHandler =  this->FindStateHandlerByName(this->nextState);
        if (nextStateHandler)
        {
            nextStateHandler->OnStateEnter(prevState);
        }
    }

    this->requestedState.Clear();
}

//------------------------------------------------------------------------------
/**
    Request a new state. This is a public method to switch states
    (SetState() is private because it invokes some internal voodoo). The
    requested state will be activated at the end of the frame.
*/
void
GameApplication::RequestState(const String& s)
{
    this->requestedState = s;
}

//------------------------------------------------------------------------------
/**
    Set a new application state. This method will call DoStateTransition().
*/
void
GameApplication::SetState(const String& s)
{
    this->nextState = s;
    this->DoStateTransition();
}

//------------------------------------------------------------------------------
/**
    Register a state handler object with the application.

    @param  state   pointer to a state handler object
*/
void
GameApplication::AddStateHandler(const Ptr<StateHandler>& handler)
{
    this->stateHandlers.Add(StringAtom(handler->GetName()), handler);
    handler->OnAttachToApplication();
}

//------------------------------------------------------------------------------
/**
    Find a state handler by name.

    @param          name of state to return the state handler for
    @return         pointer to state handler object associated with the state (can be 0)
*/
const Ptr<StateHandler>&
GameApplication::FindStateHandlerByName(const String& name) const
{
    n_assert(this->stateHandlers.Contains(StringAtom(name)));
    return this->stateHandlers[StringAtom(name)];
}

//------------------------------------------------------------------------------
/**
    Get the current state handler.
*/
const Ptr<StateHandler>&
GameApplication::GetCurrentStateHandler() const
{
    n_assert(this->curState.IsValid());
    const Ptr<StateHandler>& curStateHandler = this->FindStateHandlerByName(this->curState);
    n_assert(0 != curStateHandler);
    return curStateHandler;
}

//------------------------------------------------------------------------------
/**
    Setup new game features which should be used by this application.
    Overwrite if other features or more features have to be used.
*/
void
GameApplication::SetupGameFeatures()
{
    // create and attach default graphic features
    this->graphicsFeature = GraphicsFeature::GraphicsFeatureUnit::Create();
    this->graphicsFeature->SetCmdLineArgs(this->GetCmdLineArgs());
	this->graphicsFeature->SetRenderDebug(true);
    this->gameServer->AttachGameFeature(this->graphicsFeature.upcast<Game::FeatureUnit>());
}

//------------------------------------------------------------------------------
/**
    Cleanup all added game features
*/
void
GameApplication::CleanupGameFeatures()
{    
    this->gameServer->RemoveGameFeature(this->graphicsFeature.upcast<Game::FeatureUnit>());
    this->graphicsFeature = 0;
}

}
