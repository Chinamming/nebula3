#ifndef N_ANIMATIONSERVER_H
#define N_ANIMATIONSERVER_H
//------------------------------------------------------------------------------
/**
    @class nAnimationServer
    @ingroup Anim2

    @brief The animation server offers central services of the animation
    subsystem (at the moment it is merely a factory for nAnimation objects).
    
    (C) 2003 RadonLabs GmbH
*/
#include "core/refcounted.h"
#include "core/singleton.h"

namespace Nebula2
{
class ManagedAnimation;

//------------------------------------------------------------------------------
class nAnimationServer : public Core::RefCounted
{    
    __DeclareClass(nAnimationServer);
    __DeclareSingleton(nAnimationServer);
public:
    /// constructor
    nAnimationServer();
    /// destructor
    virtual ~nAnimationServer();
    /// create an in-memory animation object (always shared)
    virtual const Ptr<ManagedAnimation> NewMemoryAnimation(const Util::String& rsrcName);
    /// create a new streaming animation object (never shared), not implemented
    //virtual const Ptr<ManagedAnimation>& NewStreamingAnimation();    
};

}; // namespace Nebula2
//------------------------------------------------------------------------------
#endif
