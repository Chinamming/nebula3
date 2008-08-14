#ifndef PROPERTIES_SIMPLEMOVEPROPERTY_H
#define PROPERTIES_SIMPLEMOVEPROPERTY_H
//------------------------------------------------------------------------------
/**
    
    
    (C) 2005 Radon Labs GmbH
*/
#include "properties/transformableproperty.h"
#include "msg/movedirection.h"
#include "msg/movestop.h"
#include "msg/settransform.h"
#include "msg/moveturn.h"
#include "msg/moverotate.h"
#include "game/entity.h"
#include "math/pfeedbackloop.h"
#include "math/angularpfeedbackloop.h"

namespace Attr
{
    DeclareFloat(Velocity, 'VELO', ReadWrite);
};

//------------------------------------------------------------------------------
namespace GraphicsFeature
{
class SimpleMoveProperty : public BaseGameFeature::TransformableProperty
{
	DeclareClass(SimpleMoveProperty);
public:
    /// constructor
    SimpleMoveProperty();
    /// destructor
    virtual ~SimpleMoveProperty();

    /// setup callbacks for this property, call by entity in OnActivate()
    virtual void SetupCallbacks();
    /// setup default entity attributes
    virtual void SetupDefaultAttributes();
    /// called from Entity::ActivateProperties()
    virtual void OnActivate();

    /// called after movement has happened
    virtual void OnMoveAfter();
    
    /// override to register accepted messages
    virtual void SetupAcceptedMessages();
    /// handle a single message
    virtual void HandleMessage(const Ptr<Messaging::Message>& msg);

protected:

    /// immediately stop the entity
    void Stop();
    /// send a synchronous MoveStop message to self
    void SendStop();
    /// handle a MoveDirection message
    void HandleMoveDirection(BaseGameFeature::MoveDirection* msg);
    /// handle a SetTransform message
    void HandleSetTransform(BaseGameFeature::SetTransform* msg);
    /// handle a MoveTurn message
    void HandleMoveTurn(BaseGameFeature::MoveTurn* msg);
    /// handle a MoveRotate message
    void HandleMoveRotate(BaseGameFeature::MoveRotate* msg);
    
    Math::PFeedbackLoop<Math::vector> smoothedPosition;
    Math::AngularPFeedbackLoop smoothedHeading;
    float headingGain;
    float positionGain;
    float curSpeed;
    Math::vector moveDir;
};
RegisterClass(SimpleMoveProperty);
}; // namespace Properties
//------------------------------------------------------------------------------
#endif

