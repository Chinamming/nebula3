//------------------------------------------------------------------------------
//  game/server.cc
//  (C) 2003 RadonLabs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "game/gameserver.h"
#include "core/factory.h"
#include "appgame/appconfig.h"
#include "graphicsfeature/graphicsfeatureunit.h"
#include "coregraphics/shaperenderer.h"
#include "coregraphics/transformdevice.h"

namespace Game
{
ImplementClass(Game::GameServer, 'GSRV', Core::RefCounted);
ImplementSingleton(Game::GameServer);

//------------------------------------------------------------------------------
/**
*/
GameServer::GameServer() :
    isOpen(false),
    isStarted(false),
    quitRequested(false)
{
    ConstructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
GameServer::~GameServer()
{
    n_assert(!this->isOpen);
    DestructSingleton;
}

//------------------------------------------------------------------------------
/**
    Initialize the game server object. This will create and initialize all
    subsystems.
*/
bool
GameServer::Open()
{
    n_assert(!this->isOpen);
    n_assert(!this->isStarted);
    this->isOpen = true;

    return true;
}

//------------------------------------------------------------------------------
/**
*/
void
GameServer::AttachGameFeature(const Ptr<FeatureUnit>& feature)
{
    n_assert(0 != feature);
    n_assert(InvalidIndex == this->gameFeatures.FindIndex(feature));
    feature->OnActivate();
    this->gameFeatures.Append(feature);
}

//------------------------------------------------------------------------------
/**
*/
void
GameServer::RemoveGameFeature(const Ptr<FeatureUnit>& feature)
{
    n_assert(0 != feature);
    IndexT index = this->gameFeatures.FindIndex(feature);
    n_assert(InvalidIndex != index);
    feature->OnDeactivate();
    this->gameFeatures.EraseIndex(index);
}

//------------------------------------------------------------------------------
/**
    Start the game world, called after loading has completed.
*/
bool
GameServer::Start()
{
    n_assert(this->isOpen);
    n_assert(!this->isStarted);

    // call the OnStart method on all gameFeatures
    int i;
    int num = this->gameFeatures.Size();
    for (i = 0; i < num; i++)
    {
        this->gameFeatures[i]->OnStart();
    }
    
    this->isStarted = true;
    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool
GameServer::HasStarted() const
{
    return this->isStarted;
}

//------------------------------------------------------------------------------
/**
    Stop the game world, called before the world(current level) is cleaned up.
*/
void
GameServer::Stop()
{
    n_assert(this->isOpen);
    n_assert(this->isStarted);
    
    this->isStarted = false;
}

//------------------------------------------------------------------------------
/**
    Close the game server object.
*/
void
GameServer::Close()
{
    n_assert(!this->isStarted);
    n_assert(this->isOpen);
    
    // remove all gameFeatures
    while (this->gameFeatures.Size() > 0)
    {
        this->gameFeatures[0]->OnDeactivate();
        this->gameFeatures.EraseIndex(0);
    }
    this->isOpen = false;
}

//------------------------------------------------------------------------------
/**
    Trigger the game server. If your application introduces new or different
    manager objects, you may also want to override the Game::GameServer::Trigger()
    method if those gameFeatures need per-frame callbacks.
*/
void
GameServer::OnFrame()
{
    // call trigger functions on game features   
    int i;
    int num = this->gameFeatures.Size();
    for (i = 0; i < num; i++)
    {
        this->gameFeatures[i]->OnBeginFrame();
    }
    for (i = 0; i < num; i++)
    {
        this->gameFeatures[i]->OnFrame();
    } 
    for (i = 0; i < num; i++)
    {
        this->gameFeatures[i]->OnEndFrame();
    }   
}

//------------------------------------------------------------------------------
/**    
*/
void
GameServer::NotifyGameLoad()
{
    // call the OnLoad method on all gameFeatures
    int i;
    int num = this->gameFeatures.Size();
    for (i = 0; i < num; i++)
    {
        this->gameFeatures[i]->OnLoad();
    }
}
//------------------------------------------------------------------------------
/**    
*/
void
GameServer::NotifyGameSave()
{
    // call the OnLoad method on all gameFeatures
    int i;
    int num = this->gameFeatures.Size();
    for (i = 0; i < num; i++)
    {
        this->gameFeatures[i]->OnSave();
    }
}

} // namespace Game