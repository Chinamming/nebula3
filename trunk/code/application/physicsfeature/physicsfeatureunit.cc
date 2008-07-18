//------------------------------------------------------------------------------
//  game/PhysicsFeatureUnit.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "physicsfeatureunit.h"
#include "corefeature/corefeatureunit.h"
#include "graphicsfeature/graphicsfeatureunit.h"
#include "game/gameserver.h"
#include "basegametiming/systemtimesource.h"
#include "physics/level.h"
#include "physics/meshcache.h"

// include all properties for known by managers::factorymanager
#include "properties/physicsproperty.h"
#include "properties/actorphysicsproperty.h"
#include "properties/mousegripperproperty.h"
#include "properties/environmentcollideproperty.h"

namespace PhysicsFeature
{
ImplementClass(PhysicsFeatureUnit, 'FPHY' , Game::FeatureUnit);
ImplementSingleton(PhysicsFeatureUnit);

using namespace Physics;
using namespace Game;
using namespace CoreFeature;
//------------------------------------------------------------------------------
/**
*/
PhysicsFeatureUnit::PhysicsFeatureUnit()
{
    ConstructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
PhysicsFeatureUnit::~PhysicsFeatureUnit()
{
    DestructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
void
PhysicsFeatureUnit::OnActivate()
{
    FeatureUnit::OnActivate();

    IO::IoServer::Instance()->SetAssign(IO::Assign("physics", "export:physics"));

    // this feature needs the core feature
    n_assert2(CoreFeatureUnit::HasInstance(), "PhysicsFeatureUnit needs the CoreFeature!");
    n_assert(CoreFeatureUnit::Instance()->IsActive());
    
    this->physicsServer = PhysicsServer::Create();
    this->physicsServer->Open();

    if (GraphicsFeature::GraphicsFeatureUnit::HasInstance())
    {
        /// add this feature for render debug callback
        GraphicsFeature::GraphicsFeatureUnit::Instance()->AddRenderDebugCallback(this, "Physics");
    }
}

//------------------------------------------------------------------------------
/**
*/
void
PhysicsFeatureUnit::OnDeactivate()
{
    this->physicsServer->Close();
    this->physicsServer = 0;

    FeatureUnit::OnDeactivate();    
}

//------------------------------------------------------------------------------
/**
*/
void
PhysicsFeatureUnit::OnLoad()
{
    Physics::MeshCache::Instance()->Open();
}

//------------------------------------------------------------------------------
/**
*/
void
PhysicsFeatureUnit::OnFrame()
{    
    // update time in physics system
    Timing::Time time = BaseGameFeature::SystemTimeSource::Instance()->GetTime();
    this->physicsServer->SetTime(time);
    // trigger physics system, steps physics world etc.
    this->physicsServer->Trigger();
}

//------------------------------------------------------------------------------
/**
*/
void
PhysicsFeatureUnit::OnRenderDebug()
{
    Physics::PhysicsServer::Instance()->RenderDebug();
}

//------------------------------------------------------------------------------
/**
*/
void
PhysicsFeatureUnit::CreateDefaultPhysicsWorld()
{
    Physics::MeshCache::Instance()->Open();
    Ptr<Level> physicsLevel = Level::Create();
    PhysicsServer::Instance()->SetLevel(physicsLevel);
}

//------------------------------------------------------------------------------
/**
*/
void
PhysicsFeatureUnit::CleanupPhysicsWorld()
{
    PhysicsServer::Instance()->SetLevel(0);
    Physics::MeshCache::Instance()->Close();
}

}; // namespace Game
