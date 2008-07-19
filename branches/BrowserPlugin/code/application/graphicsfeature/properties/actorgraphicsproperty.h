#ifndef PROPERTIES_ACTORGRAPHICSPROPERTY_H
#define PROPERTIES_ACTORGRAPHICSPROPERTY_H
//------------------------------------------------------------------------------
/**
    A specialized graphics property for actors. This creates a 
    Graphics::ActorEntity, knows how to switch animations and
    manages attachments.
    
    (C) 2005 Radon Labs GmbH
*/
#include "properties/graphicsproperty.h"
#include "graphics/actorentity.h"
#include "msg/setanimation.h"
#include "msg/fadeanimation.h"
#include "msg/setfadeanimationmix.h"
#include "msg/animationhotspottriggered.h"
#include "msg/getactiveanimation.h"
#include "graphicsutil/charenhancementutil.h"

namespace Attr
{
    DeclareString(AnimSet, 'ANST', Attr::ReadWrite);
    DeclareString(CharacterSet, 'CHST', Attr::ReadWrite);
}

//------------------------------------------------------------------------------
namespace GraphicsFeature
{
class ActorGraphicsProperty : public GraphicsProperty
{
	DeclareClass(ActorGraphicsProperty);
public:
    /// constructor
    ActorGraphicsProperty();
    /// destructor
    virtual ~ActorGraphicsProperty();
    /// called from Entity::ActivateProperties()
    virtual void OnActivate();
    /// called from Entity::DeactivateProperties()
    virtual void OnDeactivate();
    /// setup callbacks for this property, call by entity in OnActivate()
    virtual void SetupCallbacks();
    /// setup default entity attributes
    virtual void SetupDefaultAttributes();
    /// called from within Entity::OnStart() after OnLoad when the complete world exist
    virtual void OnStart();
    /// called before rendering happens
    virtual void OnRender();

    /// override to register accepted messages
    virtual void SetupAcceptedMessages();
    /// handle a single message
    virtual void HandleMessage(const Ptr<Messaging::Message>& msg);
    /// get graphics entity cast to Graphics::ActorEntity
    Graphics::ActorEntity* GetGraphicsEntity() const;

protected:
    /// setup graphics entities
    virtual void SetupGraphicsEntities();
    /// set base or overlay animation
    void SetAnimation(SetAnimation* msg);
    /// set fade animation
    void SetFadeAnimation(SetFadeAnimationMix* msg);
    /// fade an animation(-mix)
    void FadeAnimation(FadeAnimation* msg);

    /// handle the GetActiveAnimation message
    void HandleGetActiveAnimation(GetActiveAnimation* msg);

    Ptr<CharEnhancementUtil> charEnhancementUtil;
    Ptr<AnimationHotspotTriggered> gfxHotspotTriggeredMsg;
    Timing::Time lastHotspotUpdateTime;
};
RegisterClass(ActorGraphicsProperty);

//------------------------------------------------------------------------------
/**
*/
inline
Graphics::ActorEntity*
ActorGraphicsProperty::GetGraphicsEntity() const
{
    n_assert(this->graphicsEntities[0]->IsA(Graphics::ActorEntity::RTTI));
    return (Graphics::ActorEntity*) this->graphicsEntities[0].get();
}

}; // namespace Properties
//------------------------------------------------------------------------------
#endif