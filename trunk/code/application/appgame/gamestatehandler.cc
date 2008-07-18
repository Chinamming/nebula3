//------------------------------------------------------------------------------
//  application/statehandler.cc
//  (C) 2003 RadonLabs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "appgame/gamestatehandler.h"
#include "appgame/gameapplication.h"
#include "game/gameserver.h"

namespace App
{
ImplementClass(App::GameStateHandler, 'AGST', App::StateHandler);

//------------------------------------------------------------------------------
/**
*/
GameStateHandler::GameStateHandler()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
GameStateHandler::~GameStateHandler()
{
    // empty
}

//------------------------------------------------------------------------------
/**
    This method is called when the state associated with this state handler
    is entered. The parameter indicates the previously active state.

    @param  prevState   previous state
*/
void
GameStateHandler::OnStateEnter(const Util::String& prevState)
{ 
    // create default game feature
    this->defaultGameFeature = BaseGameFeature::BaseGameFeatureUnit::Create();
    App::GameApplication* app = App::GameApplication::Instance();
    this->defaultGameFeature->SetCmdLineArgs(app->GetCmdLineArgs()); 
    Game::GameServer::Instance()->AttachGameFeature(this->defaultGameFeature.upcast<Game::FeatureUnit>());

    // setup the game
    switch (this->setupMode)
    {
        case EmptyWorld:
            this->defaultGameFeature->SetupEmptyWorld();
            break;

        case NewGame:   
            this->defaultGameFeature->NewGame();
            break;

        case ContinueGame:
            this->defaultGameFeature->ContinueGame();
            break;

        case LoadLevel:
            this->defaultGameFeature->LoadLevel(this->GetLevelName());
            break;

        case LoadSaveGame:
            this->defaultGameFeature->LoadGame(this->GetSaveGame());
            break;

        default:
            n_error("GameStateHandler: Unkown setup mode !!!");
    }

    // clear the startup level and save game name
    this->SetLevelName("");
    this->SetSaveGame("");

    // start game world
    Game::GameServer::Instance()->Start();
}

//------------------------------------------------------------------------------
/**
    This method is called when the state associated with this state handler
    is left. The parameter indicates the next active state.

    @param  nextState   next state
*/
void
GameStateHandler::OnStateLeave(const Util::String& nextState)
{
    this->defaultGameFeature->CleanupWorld();

    // remove our default game feature
    Game::GameServer::Instance()->RemoveGameFeature(this->defaultGameFeature.upcast<Game::FeatureUnit>());
    this->defaultGameFeature = 0;

    // stop game world
    Game::GameServer::Instance()->Stop();
}

//------------------------------------------------------------------------------
/**
    This method is called once a frame while the state is active. The method
    must return a state identifier. If this is different from the current
    state, a state switch will occur after the method returns. 

    @return     a state identifier
*/
Util::String
GameStateHandler::OnFrame()
{
    if (Game::GameServer::Instance()->IsQuitRequested())
    {
        return "Exit";
    }
    else
    {
        return this->GetName();
    }
}

} // namespace Application