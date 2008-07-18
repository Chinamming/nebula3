//------------------------------------------------------------------------------
//  testgameapplication.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "tools/testgame/testgameapplication.h"

#include "application/appgame/gamestatehandler.h"
#include "basegamefeature/basegamefeatureunit.h"
#include "graphicsfeature/graphicsfeatureunit.h"

namespace Tools
{
using namespace Graphics;
using namespace Math;
using namespace Util;
using namespace Lighting;
using namespace Resources;
using namespace Timing;

ImplementSingleton(TestGameApplication);   
//------------------------------------------------------------------------------
/**
*/
TestGameApplication::TestGameApplication()
{
    ConstructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
TestGameApplication::~TestGameApplication()
{
    if (this->IsOpen())
    {
        this->Close();
    }
}

//------------------------------------------------------------------------------
/**
*/
void 
TestGameApplication::SetupStateHandlers()
{
    // create our standard gamestatehandler
    Ptr<App::GameStateHandler> gameState = App::GameStateHandler::Create();
    gameState->SetSetupMode(App::GameStateHandler::NewGame);
    gameState->SetName("GameState");
    gameState->SetLevelName("testlevel");
    
    this->AddStateHandler(gameState.get());

    this->SetState("GameState");
}

//------------------------------------------------------------------------------
/**
*/
void
TestGameApplication::SetupGameFeatures()
{
    GameApplication::SetupGameFeatures();
#if __USE_PHYSICS__
    // create and attach core features
    this->physicsFeature = PhysicsFeature::PhysicsFeatureUnit::Create();    
    this->gameServer->AttachGameFeature(this->physicsFeature.upcast<Game::FeatureUnit>());
#endif	
}

//------------------------------------------------------------------------------
/**
    Cleanup all added game features
*/
void
TestGameApplication::CleanupGameFeatures()
{    
#if __USE_PHYSICS__
    this->gameServer->RemoveGameFeature(this->physicsFeature.upcast<Game::FeatureUnit>());
    this->physicsFeature = 0;
 #endif
    GameApplication::CleanupGameFeatures();
}
} // namespace Tools
