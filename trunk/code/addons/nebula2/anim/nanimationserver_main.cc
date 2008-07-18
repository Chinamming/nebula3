//------------------------------------------------------------------------------
//  nanimationserver_main.cc
//  (C) 2003 RadonLabs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "nanimationserver.h"
#include "nmemoryanimation.h"
#include "managedanimation.h"
#include "../nebula2wrapper.h"
#include "resources/resourcemanager.h"

ImplementClass(Nebula2::nAnimationServer, 'ASRV', Core::RefCounted);
ImplementSingleton(Nebula2::nAnimationServer);

namespace Nebula2
{
//------------------------------------------------------------------------------
/**
*/
nAnimationServer::nAnimationServer()
{
    ConstructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
nAnimationServer::~nAnimationServer()
{
    // TODO: unload all animation resources
    
    DestructSingleton;
}

//------------------------------------------------------------------------------
/**
    Create a new memory animation object. Memory animations keep the
    entire animation data in memory, but are generally shared. Memory anims
    should be used when many objects are driven by the same animation.

    @param  rsrcName    the resource name for resource sharing
*/
const Ptr<ManagedAnimation>
nAnimationServer::NewMemoryAnimation(const Util::String& rsrcName)
{
    return Resources::ResourceManager::Instance()->CreateManagedResource(nMemoryAnimation::RTTI, Resources::ResourceId(rsrcName)).downcast<ManagedAnimation>();
}

}; // namespace Nebula2